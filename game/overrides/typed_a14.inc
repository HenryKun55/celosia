// typed_a14.inc: round A14's typed functions (tools/typed_lift2.py, stage 14: runtime state as VU0-interface slots, break slots). Generated: do not edit. Included inside a namespace (typed_a14.h: namespace typed), after typed_a7.inc. First the variants of earlier functions (<name>_x14: the same MIPS typed with extra outputs, called by stage-14 functions only), then stage 14's functions; every one declared first.
// the recursive functions (A14_REC index): 0 manager_sub_01099A08, 1 kernel_sub_001DB238
// every function of the stage declared first (variants of stage-14 functions call stage-14 functions)
inline uint32_t ps2___mcmp_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t io_h6, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001CDDE8_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3, uint64_t io_h5, uint64_t *xo = nullptr);
inline uint32_t kernel_sub_001CDF38_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr);
inline uint64_t vab_get_info_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr);
inline uint32_t vab_get_veltbl_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr);
inline uint32_t vab_get_lfotbl_param_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr);
inline void kernel_sub_001DAAB0_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t io_h2, uint64_t &io_h6, uint64_t *xo = nullptr);
inline a1::RetVF IirFilter_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2, uint64_t *xo = nullptr);
inline uint64_t GetLightClass_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t GetScriptAnimationClass_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline a1::RetVF constructor_camera_lws_anime_data_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr);
inline uint64_t dynamic_cast_to_camera_obj_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t *xo = nullptr);
inline a1::RetVF rintf_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, uint64_t *xo = nullptr);
inline uint64_t SetCamFinalizerGeometry_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h6, uint64_t *xo = nullptr);
inline a1::RetVF setFadeParam_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, uint64_t *xo = nullptr);
inline void gameModeCls_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr);
inline uint64_t getstagenumber_u14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3);
inline uint64_t getstagenumber_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3, uint64_t *xo = nullptr);
inline a1::RetVF AI_GetAccelB3XYZ_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2, uint64_t *xo = nullptr);
inline uint64_t getPathDataAddr_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001DAB10_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h5, uint64_t *xo = nullptr);
inline uint64_t GetScriptLayoutClass_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline a1::RetVF manager_sub_0110EDC8_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_0137EB38_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h3, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_01423410_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f20, float f21, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr);
inline uint64_t GetLayoutClass_u14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_0137F850_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr);
inline uint64_t sub_00101AC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t gp);
inline uint64_t sub_00101AF0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t gp);
inline uint64_t QueryIntrContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t io_c0st);
inline uint64_t sub_00109288(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0);
inline uint64_t DIntr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t EIntr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st);
inline uint64_t ps2___moddi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t3, uint64_t t8, uint64_t sp, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo = nullptr);
inline uint64_t ps2___udivdi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t6, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h6, uint64_t *xo = nullptr);
inline uint64_t ps2___umoddi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t3, uint64_t sp, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo = nullptr);
inline uint64_t ps2___muldi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t1, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline uint64_t sub_0012B1D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001AD850(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st);
inline uint64_t Shock_SetMotor(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h7, uint64_t io_h8, uint64_t *xo = nullptr);
inline void iosGetRandomVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vrr);
inline void iosGetDRandomVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vrr);
inline uint32_t getFrameBufferSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline uint32_t getFrameBufferSizeFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline uint32_t getFrameBufferSizeTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline uint32_t getDrawBufferAddressTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline uint32_t getDrawBufferAddressFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline uint32_t getTextureAreaAddressTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1);
inline uint32_t getTextureAreaAddressFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1);
inline uint64_t iosGetFontDataWithID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_hi1, uint64_t &io_lo1);
inline uint32_t Sg2SlotSetPitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint64_t *xo = nullptr);
inline uint64_t Sg2SlotSetLfoPitchSpeed(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001DA790(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline void kernel_sub_001DA850(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t io_h8, uint64_t &io_h10, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001DA8F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t *xo = nullptr);
inline void kernel_sub_001DA9C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, uint64_t io_h3, uint64_t io_h8, uint64_t &io_h10, uint64_t *xo = nullptr);
inline uint64_t Sg2ExPcmInputGetRealPlayFrameCount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0);
inline uint32_t clipByMinMaxCube(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vacc, uint32_t &io_vclip);
inline uint64_t manager_sub_01099A08(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, const a4::vec4 &io_vf4, const a4::vec4 &io_vf5, const a4::vec4 &io_vf6, const a4::vec4 &io_vf7, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vacc, uint32_t &io_vclip, uint64_t *xo = nullptr);
inline a1::RetVF manager_sub_010A1F10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h6, uint64_t io_h7, uint64_t *xo = nullptr);
inline uint32_t objectmanager_mallocerrorfunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt);
inline uint64_t manager_sub_0110F0D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr);
inline uint32_t ClipPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, const a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint32_t &io_vclip);
inline uint64_t mcGetLastSaveGameDataIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr);
inline uint64_t sheetFsGetStageTexSegIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr);
inline a1::RetVF getStageAreaIndexByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0);
inline uint32_t SeamlessStageLoadComplete(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr);
inline a1::RetVF getStageAreaID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2);
inline a1::RetVF getStageAreaName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr);
inline uint64_t getStageAreaNameUV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_hi1, uint64_t &io_lo1);
inline a1::RetVF stagemanager_getCenterFromAreaId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr);
inline uint32_t time_attack_init_gstat_adrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t io_h3, uint64_t &io_h8);
inline a1::RetVF YattsukeGetSeamlessAreaCenter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr);
inline uint64_t sub_001091C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t &io_c0st);
inline uint64_t sub_001092B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t a1);
inline uint64_t GetTimerSystemTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t sp, uint32_t &io_c0st, uint64_t &io_h9);
inline uint64_t AllocTimerCounter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t &io_c0st);
inline uint64_t GetTimerUsedUnusedCounters(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t SetTimerCount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st);
inline uint64_t GetTimerBaseTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st);
inline uint64_t TimerBusClock2USec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t t6, uint64_t sp, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h6);
inline uint64_t TimerUSec2BusClock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t6, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h6, uint64_t *xo = nullptr);
inline uint64_t sceSifSetRebootNotifyBuffer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t io_h4);
inline uint64_t sub_001199F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t _sceMcpIsSpeckOutClust(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp);
inline uint64_t sub_0012C240(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t0, uint64_t sp, uint64_t &io_h2, uint64_t io_h6, uint64_t *xo = nullptr);
inline uint64_t iosDI(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t iosEI(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001B7590(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo = nullptr);
inline uint64_t Sg2CtrlEntryTickProc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st);
inline uint64_t Sg2CtrlRemoveTickProc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st);
inline uint64_t Sg2DspEffectSetEndAdr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2DspEffectSetType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2DspEffectSetDepth(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t a1, uint32_t &io_c0st);
inline uint64_t Sg2DspEffectSetDepth2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t &io_c0st);
inline uint64_t Sg2DspEffectSetDelay(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2DspEffectSetFeedBack(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2ModMidiout1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t a1, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t Sg2ModMidiout1Exclusive(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2ModMidiout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo = nullptr);
inline uint64_t Sg2ModOpenBgm(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t io_h5, uint64_t *xo = nullptr);
inline uint64_t Sg2ModOpenSe(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001D0E50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp);
inline uint64_t Sg2RemoteSendDmaCom(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint32_t a3, uint32_t &io_c0st, uint64_t &io_h3, uint64_t io_h4, uint64_t *xo = nullptr);
inline uint64_t Sg2SlotSetDumperOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t &io_h3);
inline uint64_t Sg2SlotModKeyOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t &io_c0st, uint64_t &io_h3);
inline uint64_t Sg2SlotVabKeyOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t &io_c0st, uint64_t &io_h3);
inline uint64_t Sg2SlotSetKeyOnBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2SlotSetKeyOffBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2SlotSetReverbOnBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2SlotSetReverbOffBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2SlotSetNoiseOnBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t Sg2SlotSetNoiseOffBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st);
inline uint64_t kernel_sub_001DB088(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t a3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t Sg2ExPcmInputCtrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t manager_sub_0115C2C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t manager_sub_011641E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp);
inline uint64_t GetAttributeRelationEffectDefByName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t gamecore_sub_013458A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_01345980(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline a1::RetVF ChangeGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, float f12, float f13, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_01346090(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1);
inline void gamecore_sub_01349D90(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_01361E88(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo = nullptr);
inline a1::RetVF SetStageManagerAreaPreload(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, uint64_t &io_h2, uint64_t &io_h6, uint64_t *xo = nullptr);
inline a1::RetVF SetStageManagerAreaPreloadWithAreaOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2, uint64_t &io_h6, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_0137FD70(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vacc, float &io_vq, a4::vec4 &io_vrr, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t SpriteInitStatusClut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp);
inline a1::RetVF SpriteSetStatusClut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp);
inline uint64_t setEventLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr);
inline uint64_t unsetEventLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2);
inline uint64_t resetEventLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1);
inline uint64_t allResetLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t a0, uint64_t *xo = nullptr);
inline uint64_t setEventLinePrio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr);
inline uint64_t getPointDataFromEvent(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2);
inline uint64_t getPointPosFromEvent(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h3);
inline uint64_t getRootPointIdxForGroupID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t *xo = nullptr);
inline uint64_t iosPushVu0Registers(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, const a4::vec4 &io_vf5, const a4::vec4 &io_vf6, const a4::vec4 &io_vf7, const a4::vec4 &io_vf8, const a4::vec4 &io_vf9, const a4::vec4 &io_vf10, const a4::vec4 &io_vf11, const a4::vec4 &io_vf12, const a4::vec4 &io_vf13, const a4::vec4 &io_vf14, const a4::vec4 &io_vf15, const a4::vec4 &io_vf16, const a4::vec4 &io_vf17, const a4::vec4 &io_vf18, const a4::vec4 &io_vf19, const a4::vec4 &io_vf20, const a4::vec4 &io_vf21, const a4::vec4 &io_vf22, const a4::vec4 &io_vf23, const a4::vec4 &io_vf24, const a4::vec4 &io_vf25, const a4::vec4 &io_vf26, const a4::vec4 &io_vf27, const a4::vec4 &io_vf28, const a4::vec4 &io_vf29, const a4::vec4 &io_vf30, const a4::vec4 &io_vf31, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t iosPopVu0Registers(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf19, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vf22, a4::vec4 &io_vf23, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t Sg2DmaTransVoice(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t &io_c0st, uint64_t &io_h3, uint64_t io_h4, uint64_t *xo = nullptr);
inline uint64_t Sg2ExStAdpcmTrkDspSw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint32_t &io_c0st);
inline uint64_t all_keyoff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t sp, uint32_t &io_c0st);
inline uint64_t kernel_sub_001CFB28(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st, uint64_t &io_h3);
inline uint64_t kernel_sub_001D1360(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st, uint64_t &io_h3);
inline uint64_t kernel_sub_001D1778(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp, uint32_t &io_c0st);
inline uint64_t Sg2SepStopSndOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t Sg2SlotSetBasicParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001DAC50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h5, uint64_t &io_h6, uint64_t io_h8, uint64_t &io_h10, uint64_t *xo = nullptr);
inline uint64_t kernel_sub_001DB238(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, float f1, float f2, float f3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t *xo = nullptr);
inline uint64_t Sg2ExPcmInputSetAutoFilterDirect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t &io_h7, uint64_t io_h19);
inline uint64_t Sg2ExPcmOutputSetAutoFilterDirect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t &io_h7, uint64_t io_h18);
inline uint64_t clipBySphere(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint32_t &io_vclip);
inline uint64_t shadowReInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr);
inline a1::RetVF GetDissolveAnimation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline void manager_sub_0110EF60(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, uint64_t *xo = nullptr);
inline a1::RetVF PauseGame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t1, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline a1::RetVF ChangeGameTimeScale(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t w64_t1, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1);
inline a1::RetVF ChangeGameTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_t1, uint32_t sp, float f12, uint64_t &io_hi1, uint64_t &io_lo1);
inline a1::RetVF setGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_t1, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1);
inline a1::RetVF setFrameRateMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_t1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_hi1, uint64_t &io_lo1);
inline a1::RetVF setDemoBar(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, float f0, float f12, float f13, float f20, float f21, float f22, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_01364CD8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo = nullptr);
inline uint64_t GetStageContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t SCRFuncBirdSetLandingPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3);
inline uint64_t SCRCerberusEventLineCut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t kernel_sub_001D24B0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint32_t sp, uint32_t &io_c0st);
inline uint64_t Sg2SepVabStopSndOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st);
inline uint64_t Sg2SepAllStopSndOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st);
inline uint64_t Sg2ExPcmInputSetAutoFilter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t w64_t0, float f1, float f2, float f3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st);
inline uint64_t Sg2ExPcmOutputSetAutoFilter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t w64_t0, float f1, float f2, float f3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st);
inline a1::RetVF manager_sub_0113B1D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint32_t &io_vclip, uint64_t io_h2);
inline a1::RetVF gamecore_sub_013524D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_t1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_hi1, uint64_t &io_lo1);
inline a1::RetVF gamecore_sub_01352528(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_t1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_hi1, uint64_t &io_lo1);
inline uint64_t GetStageContextByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t GetStageCenter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t GetStageName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t GetStageAreaNumber(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t GetStageAreaNumberByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t GetStageNumberByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7);
inline uint64_t GetStageNumberNumber(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t SCRFuncBirdSetAttainPosForPathData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2);
inline uint64_t SCRFuncBirdSetAttainNearPosForPathData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2);
inline uint64_t SCRFuncBirdSetAttainFarPosForPathData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2);
inline uint64_t gamecore_sub_013813A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t0, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, a4::vec4 &io_vrr, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr);
inline a1::RetVF SCRConditionHorseNeighPermission(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t &io_h2);
inline a1::RetVF SCRConditionKirinIntervalOfMovement(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline a1::RetVF SCRConditionKameNextTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline a1::RetVF gamecore_sub_01426370(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t w64_a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, float f0, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5, uint64_t *xo = nullptr);
inline a1::RetVF gamecore_sub_01426418(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t w64_a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, float f0, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5, uint64_t *xo = nullptr);
inline uint64_t SCRFuncLineCut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t SCRFuncLineConnect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t SCRFuncLineSetRoute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t SCRFuncLineUnsetRoute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t SCRFuncLineSetForcePrio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t SCRFuncLineAllReset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2);
inline uint64_t SCRFuncSetAttainPointPosForLanding(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3);
inline uint64_t SCRFuncSetAttainPointPosForEventID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3);
inline uint64_t GAMEUTIL_get_add_cur_stage_ofst_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t resetUserPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo = nullptr);
inline uint64_t get_apply_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr);

// ps2___mcmp_x14 (0x12f0d8): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2; VU0 in h6, out h2)
inline uint32_t ps2___mcmp_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t io_h6, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    v0 = A1_R32B(a0, 16);
    v1 = A1_R32B(a1, 16);
    v0 = v0 - v1;
    if (v0 != 0) goto L_12f138;
    v1 = v1 << 2;
    t2 = a0 + 0x14u; a3xw3 = true;
    v0 = a1 + 0x14u;
    a3 = t2 + v1; a3xw0 = true;
    t1 = v0 + v1; a3xw2 = true;
    a3 = a3 - 4u; a3xw0 = true;
L_12f108:;
    t1 = t1 - 4u; a3xw2 = true;
    a1 = A1_R32(a3);
    a2 = 1u;
    a0 = A1_R32(t1);
    v0 = 0xFFFFFFFFu;
    t0 = t2 < a3 ? 1u : 0u; a3xw1 = true;
    v1 = a1 < a0 ? 1u : 0u;
    { const bool c = a1 != a0; if (a1_sx64(v1) == 0) do { const a4::q128 _v = (a4::mk128(h6, a1_sx64(a2))); h2 = a4::hi64(_v); v0 = (uint32_t)a4::lo64(_v); } while (0); if (c) goto L_12f138; }
    if (t0 != 0) { a3 = a3 - 4u; a3xw0 = true; { A3_BACK(); goto L_12f108; } }
    v0 = 0u;
L_12f138:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a3); if (a3xw1) xo[1] = a1_sx64(t0); if (a3xw2) xo[2] = a1_sx64(t1); if (a3xw3) xo[3] = a1_sx64(t2); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001CDDE8_x14 (0x1cdde8): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in h5, out h3)
inline uint64_t kernel_sub_001CDDE8_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3, uint64_t io_h5, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint32_t t8 = 0;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    (t3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967168))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 2))));
    a3 = 1u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t3))));
    t0 = 0x7Fu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    a1 = 0x40u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4096))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    a2 = 0xFFFFFFFFu;
    at = 0x270000u;
    at = at + 0x3ED8u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W8B((uint32_t)v0, 100, (uint8_t)(uint32_t)a0);
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 688))));
    A1_W32B((uint32_t)v0, 20, a3);
    t8 = 0xFFu;
    A1_W32B((uint32_t)v0, 56, t0);
    t5 = 0x40u;
    A1_W32B((uint32_t)v0, 68, (uint32_t)v1);
    t4 = 0x7Fu;
    A1_W32B((uint32_t)v0, 48, 0u);
    (t7 = (uint32_t)((int32_t)((int32_t)ADD32(0u, 4096))));
    A1_W32B((uint32_t)v0, 684, a3);
    t6 = 9u;
    A1_W32B((uint32_t)v0, 52, t0);
    A1_W32B((uint32_t)v0, 44, 0u);
    A1_W32B((uint32_t)v0, 36, 0u);
    A1_W32B((uint32_t)v0, 40, 0u);
    A1_W32B((uint32_t)v0, 28, 0u);
    A1_W32B((uint32_t)v0, 32, 0u);
    A1_W32B((uint32_t)v0, 64, 0u);
    A1_W32B((uint32_t)v0, 96, 0u);
    A1_W32B((uint32_t)v0, 16, 0u);
    A1_W32B((uint32_t)v0, 60, a1);
    a1 = 1u;
    A1_W32B((uint32_t)v0, 0, a2);
    a2 = 1u;
L_1cde90:;
    (v0 = (uint64_t)(t2 ^ a1_sx64(t6)));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t2) < (int64_t)(int32_t)16) ? 1 : 0));
    if (v0 == 0) do { const a4::q128 _v = (a4::mk128(h5, a1_sx64(a1))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(t1, 0, (uint32_t)t3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a1, (uint32_t)v1))));
    A1_W32B(t1, 4, 0u);
    A1_W32B(t1, 44, (uint32_t)v0);
    A1_W32B(t1, 8, t8);
    A1_W32B(t1, 12, t5);
    A1_W32B(t1, 16, 0u);
    A1_W32B(t1, 20, t4);
    A1_W32B(t1, 32, t4);
    A1_W32B(t1, 36, 0u);
    A1_W32B(t1, 48, 0u);
    A1_W32B(t1, 52, t5);
    A1_W8B(t1, 100, (uint8_t)0u);
    A1_W8B(t1, 101, (uint8_t)0u);
    A1_W8B(t1, 102, (uint8_t)0u);
    A1_W8B(t1, 103, (uint8_t)0u);
    A1_W32B(t1, 72, t7);
    A1_W32B(t1, 104, 0u);
    A1_W32B(t1, 108, 0u);
    A1_W16B(t1, 92, (uint16_t)0u);
    A1_W16B(t1, 94, (uint16_t)0u);
    A1_W16B(t1, 96, (uint16_t)0u);
    A1_W16B(t1, 98, (uint16_t)0u);
    A1_W32B(t1, 76, 0u);
    A1_W32B(t1, 80, 0u);
    A1_W32B(t1, 84, 0u);
    A1_W32B(t1, 88, 0u);
    A1_W32B(t1, 56, 0u);
    A1_W32B(t1, 60, 0u);
    A1_W16B(t1, 70, (uint16_t)a2);
    A1_W16B(t1, 68, (uint16_t)a2);
    A1_W32B(t1, 40, (uint32_t)v1);
    { const bool c = (int64_t)a0 != 0; t1 = t1 + 0x70u; if (c) { A3_BACK(); goto L_1cde90; } }
    { if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); xo[2] = a1_sx64(t1); xo[3] = t2; xo[4] = t3; } io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001CDF38_x14 (0x1cdf38): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in -, out -)
inline uint32_t kernel_sub_001CDF38_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t2, 2))));
    t1 = 0x7Fu;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t2))));
    t0 = 0x1000u;
    v0 = v1 << 5;
    a1 = 0xFFFFFF80u;
    v0 = v0 - v1;
    v1 = 0xFFFFFFFFu;
    v0 = v0 << 4;
    a2 = 2u;
    at = 0x270000u;
    at = at + 0x3ED8u;
    v0 = at + v0;
    a3 = 0x40u;
    A1_W32B(v0, 56, t1);
    t5 = 0xFFu;
    A1_W32B(v0, 68, t0);
    t4 = 0x64u;
    A1_W32B(v0, 48, 0u);
    t3 = 0x7Fu;
    A1_W32B(v0, 52, t1);
    A1_W32B(v0, 44, 0u);
    A1_W32B(v0, 36, 0u);
    A1_W32B(v0, 40, 0u);
    A1_W32B(v0, 28, 0u);
    A1_W32B(v0, 32, 0u);
    A1_W32B(v0, 64, 0u);
    A1_W32B(v0, 96, 0u);
    A1_W32B(v0, 16, 0u);
    A1_W32B(v0, 684, (uint32_t)a0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 688))));
    A1_W8B(v0, 100, (uint8_t)a1);
    a1 = 1u;
    A1_W32B(v0, 20, a2);
    a2 = 0x40u;
    A1_W32B(v0, 60, a3);
    a3 = 0x1000u;
    A1_W32(v0, v1);
    v1 = 0xFu;
L_1cdfe0:;
    v1 = v1 - 1u;
    A1_W32B((uint32_t)a0, 0, (uint32_t)t2);
    A1_W32B((uint32_t)a0, 4, 0u);
    A1_W32B((uint32_t)a0, 8, t5);
    A1_W32B((uint32_t)a0, 12, a2);
    A1_W32B((uint32_t)a0, 16, 0u);
    A1_W32B((uint32_t)a0, 20, t4);
    A1_W32B((uint32_t)a0, 32, t3);
    A1_W32B((uint32_t)a0, 36, 0u);
    A1_W32B((uint32_t)a0, 48, 0u);
    A1_W32B((uint32_t)a0, 52, a2);
    A1_W8B((uint32_t)a0, 100, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 101, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 102, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 103, (uint8_t)0u);
    A1_W32B((uint32_t)a0, 72, a3);
    A1_W32B((uint32_t)a0, 104, 0u);
    A1_W32B((uint32_t)a0, 108, 0u);
    A1_W16B((uint32_t)a0, 92, (uint16_t)0u);
    A1_W16B((uint32_t)a0, 94, (uint16_t)0u);
    A1_W16B((uint32_t)a0, 96, (uint16_t)0u);
    A1_W16B((uint32_t)a0, 98, (uint16_t)0u);
    A1_W32B((uint32_t)a0, 56, 0u);
    A1_W32B((uint32_t)a0, 60, 0u);
    A1_W32B((uint32_t)a0, 76, 0u);
    A1_W32B((uint32_t)a0, 80, 0u);
    A1_W32B((uint32_t)a0, 84, 0u);
    A1_W32B((uint32_t)a0, 88, 0u);
    A1_W16B((uint32_t)a0, 70, (uint16_t)a1);
    A1_W16B((uint32_t)a0, 68, (uint16_t)a1);
    { const bool c = (int32_t)v1 >= 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 112)))); if (c) { A3_BACK(); goto L_1cdfe0; } }
    { if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); xo[2] = a1_sx64(t1); xo[3] = t2; xo[4] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// vab_get_info_x14 (0x1da0e0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a3 xo[1]=t0; VU0 in -, out -)
inline uint64_t vab_get_info_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
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
    { A1_W32(a2, a1); if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// vab_get_veltbl_x14 (0x1da348): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out -)
inline uint32_t vab_get_veltbl_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo)
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
    { A1_W32(a1, a2); if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// vab_get_lfotbl_param_x14 (0x1da380): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a3 xo[1]=t0; VU0 in -, out -)
inline uint32_t vab_get_lfotbl_param_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo)
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
    { v0 = A1_R32B(v1, 8); if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DAAB0_x14 (0x1daab0): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=a3 xo[4]=t0; VU0 in h2, out h6)
inline void kernel_sub_001DAAB0_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t io_h2, uint64_t &io_h6, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    { const bool c = (int32_t)a3 <= 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1dab04; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); } a3xw2 = true;
L_1daac8:;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    v1 = v1 + t0;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 256))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 1024))));
    f0 = a1_flt(A1_R32(v1)); a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)a1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    (a0 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)256));
    if (RC_U64X(a0) != 0) do { const a4::q128 _v = (a4::mk128(h2, a1_sx64(v0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    f0 = a1f::add(f0, f1); a3xw0 = true;
    (v0 = (uint32_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)a3) ? 1 : 0));
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 0, bits); }
    { const bool c = v0 != 0; A1_W32(v1, a1_bits(f0)); if (c) { A3_BACK(); goto L_1daac8; } }
L_1dab04:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); xo[3] = a3; xo[4] = a1_sx64(t0); } io_h6 = h6; return; }
    __builtin_unreachable();
}

// IirFilter_x14 (0x1dcb48): typed by tools/typed_lift2.py (round A6; loop frame leaf, extra outputs xo[0]=f1 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=f6 xo[6]=f7 xo[7]=f8 xo[8]=f9 xo[9]=f10 xo[10]=f11 xo[11]=f12 xo[12]=f13 xo[13]=f14; VU0 in -, out -, frame 16 bytes, 8 slot accesses)
inline a1::RetVF IirFilter_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2, uint64_t *xo)
{
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
    float f14 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint8_t a2fr[16];
    f13 = a1_flt(A1_R32B(a0, 52));
    f12 = a1_flt(A1_R32B(a0, 56));
    v0 = 2u;
    f0 = a1_flt(A1_R32B(a0, 32));
    (v0 = (uint32_t)(((int64_t)(int64_t)a1_sx64(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    f8 = a1_flt(A1_R32(a2));
    f0 = a1f::mul(f12, f0);
    f11 = a1_flt(A1_R32B(a0, 60));
    f3 = a1f::mul(f13, f8);
    f4 = a1_flt(A1_R32B(a0, 28));
    f10 = a1_flt(A1_R32B(a0, 64));
    f6 = a1f::mul(f12, f8);
    f4 = a1f::mul(f11, f4);
    { uint32_t bits = A1_R32B(a0, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    f9 = a1_flt(A1_R32B(a0, 68));
    f3 = a1f::addFinite(f3, f0);
    f2 = a1_flt(A1_R32B(a0, 36));
    f5 = a1f::mul(f10, f5);
    f7 = a1_flt(A1_R32B(a2, 4));
    f2 = a1f::mul(f9, f2);
    f14 = a1_flt(A1_R32B(a2, 8));
    f1 = a1f::mul(f13, f7);
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f8));
    f3 = a1f::addFinite(f3, f4);
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f7));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    f1 = a1f::addFinite(f1, f6);
    f3 = a1f::subFinite(f3, f5);
    f3 = a1f::subFinite(f3, f2);
    A1_W32(a2, a1_bits(f3));
    f3 = a1f::mul(f10, f3);
    f0 = a1_flt(A1_R32B(a0, 32));
    f2 = a1_flt(A1_R32B(a0, 40));
    f0 = a1f::mul(f11, f0);
    f2 = a1f::mul(f9, f2);
    f1 = a1f::addFinite(f1, f0);
    f1 = a1f::subFinite(f1, f3);
    f1 = a1f::subFinite(f1, f2);
    { const bool c = v0 == 0; A1_W32B(a2, 4, a1_bits(f1)); if (c) goto L_1dcc54; }
    f5 = ((float)(f7));
    f7 = ((float)(f8));
    f6 = ((float)(f14));
    v1 = a2 + 8u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 4294967294))));
L_1dcc00:;
    f0 = a1f::mul(f13, f6);
    f2 = a1_flt(A1_R32(v1 - 4u));
    f3 = a1f::mul(f12, f5);
    f1 = a1_flt(A1_R32(v1 - 8u));
    f4 = a1f::mul(f11, f7);
    f7 = ((float)(f5));
    f2 = a1f::mul(f10, f2);
    v0 = v0 - 1u;
    f1 = a1f::mul(f9, f1);
    f5 = ((float)(f6));
    f0 = a1f::addFinite(f0, f3);
    f6 = a1_flt(A1_R32B(v1, 4));
    f0 = a1f::addFinite(f0, f4);
    f0 = a1f::subFinite(f0, f2);
    f0 = a1f::subFinite(f0, f1);
    A1_W32(v1, a1_bits(f0));
    { const bool c = v0 != 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_1dcc00; } }
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f7));
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 4, bits); }
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f6));
L_1dcc54:;
    f0 = a1_flt(a2::frLd<uint32_t>(a2fr, 0));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    f1 = a1_flt(a2::frLd<uint32_t>(a2fr, 4));
    v0 = a2 + v0;
    A1_W32B(a0, 28, a1_bits(f0));
    A1_W32B(a0, 32, a1_bits(f1));
    f0 = a1_flt(A1_R32(v0 - 8u));
    A1_W32B(a0, 36, a1_bits(f0));
    f1 = a1_flt(A1_R32(v0 - 4u));
    A1_W32B(a0, 40, a1_bits(f1));
    { if (xo) { xo[0] = a1_bits(f1); xo[1] = a1_bits(f2); xo[2] = a1_bits(f3); xo[3] = a1_bits(f4); xo[4] = a1_bits(f5); xo[5] = a1_bits(f6); xo[6] = a1_bits(f7); xo[7] = a1_bits(f8); xo[8] = a1_bits(f9); xo[9] = a1_bits(f10); xo[10] = a1_bits(f11); xo[11] = a1_bits(f12); xo[12] = a1_bits(f13); xo[13] = a1_bits(f14); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetLightClass_x14 (0x10f93f0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetLightClass_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 19))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetScriptAnimationClass_x14 (0x110a838): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetScriptAnimationClass_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// constructor_camera_lws_anime_data_x14 (0x1161c08): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a3; VU0 in -, out h2 h3)
inline a1::RetVF constructor_camera_lws_anime_data_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    a3 = 0xFFFFFFFFu;
    A1_W32B(a0, 4, 0u);
    A1_W32(a0, a3);
    { const uint32_t _dynInsn = A1_R32(0x1161C14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 8, a3);
    { const uint32_t _dynInsn = A1_R32(0x1161C1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x1161C20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    at = 0x3F800000u;
    f0 = a1_flt(a7_opq(at));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x1161C30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 48), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 84, a3);
    A4_W128(ADD32(a0, 64), a4::mk128(h3, v1));
    A1_W32B(a0, 80, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 96), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 140, a1_bits(f0));
    A4_W128(ADD32(a0, 112), a4::mk128(h3, v1));
    A1_W32B(a0, 128, 0u);
    A1_W32B(a0, 132, 0u);
    { A1_W32B(a0, 136, 0u); if (xo) { xo[0] = a1_sx64(a3); } io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// dynamic_cast_to_camera_obj_x14 (0x1162e08): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1; VU0 in -, out -, break)
inline uint64_t dynamic_cast_to_camera_obj_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 17))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    if ((int64_t)(int32_t)v1 == (int64_t)a0) goto L_1162e24;
    A6_BREAK(0x1162E1Cu);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1162e24:;
    { if (xo) { xo[0] = a1_sx64(v1); } return v0; }
    __builtin_unreachable();
}

// rintf_x14 (0x116ad80): typed by tools/typed_lift2.py (round A6; frame leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out -, frame 16 bytes, 4 slot accesses)
inline a1::RetVF rintf_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint8_t a2fr[16];
    { uint32_t bits; std::memcpy(&bits, &f12, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a2, 31))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)127 << 16))));
    t1 = 0x7FFF0000u;
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    t1 = t1 | 0xFFFFu;
    (v0 = (uint64_t)(RC_U64X(a2) & a1_sx64(t1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 23))));
    (a3 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294967169))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a1) < (int64_t)(int32_t)23) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; f0 = ((float)(f12)); if (c) goto L_116ae78; }
    if ((int64_t)a3 == 0) goto L_116ae78;
    { const bool c = (int32_t)a1 >= 0; (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a0, a1 & 0x1F)))); if (c) goto L_116ae30; }
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(a0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)64 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    a1 = 0xFFF00000u;
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(v1)));
    (a2 = (uint64_t)(RC_U64X(a2) & a1_sx64(a1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 9))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
    (a2 = (uint64_t)(RC_U64X(a2) | RC_U64X(v0)));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f12, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    at = 0x1190000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
    f1 = a1_flt(A1_R32B(at, 27488));
    f0 = a1f::add(f1, f12);
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f0));
    f0 = a1f::sub(f0, f1);
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 4))));
    (v1 = (uint64_t)(RC_U64X(v0) & a1_sx64(t1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 31))));
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); }
    goto L_116ae78;
L_116ae30:;
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 1))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v1))); if (c) goto L_116ae78; }
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)(~(0ULL | RC_U64X(v1)))); if (c) goto L_116ae58; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16 << 16))));
    (v1 = (uint64_t)(RC_U64X(a2) & v1));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, a1 & 0x1F))));
    (a2 = (uint64_t)(v1 | RC_U64X(v0)));
L_116ae58:;
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f12, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    at = 0x1190000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    f1 = a1_flt(A1_R32B(at, 27488));
    f0 = a1f::add(f1, f12);
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f0));
    f0 = a1f::sub(f0, f1);
L_116ae78:;
    { if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = a1_sx64(t0); xo[3] = a1_sx64(t1); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SetCamFinalizerGeometry_x14 (0x134a7d0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out h2 h6)
inline uint64_t SetCamFinalizerGeometry_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h6, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    v1 = 0x1460000u;
    v1 = v1 + 0x5990u;
    A4_W128(ADD32(v1, 0), a4::mk128(h6, a2));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(v1, 32), a4::mk128(h2, v0)); if (xo) { xo[0] = a2; } io_h2 = h2; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// setFadeParam_x14 (0x134db68): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=f2; VU0 in -, out -)
inline a1::RetVF setFadeParam_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t v0 = 0;
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
    { A1_W32B(a0, 4, a1_bits(f1)); if (xo) { xo[0] = a1_bits(f2); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// gameModeCls_x14 (0x1352b88): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1; VU0 in -, out -)
inline void gameModeCls_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    { const bool c = (int64_t)a0 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1352bf8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 368)))); a3xw2 = true;
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 23856)))); a3xw0 = true;
    t0 = 1u; a3xw1 = true;
L_1352ba8:;
    (v0 = (uint64_t)(a0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 1)))); if (c) goto L_1352bf0; }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, t1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a1, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 24))));
    { const bool c = (int64_t)v1 != 0; A1_W8B(a1, 0, (uint8_t)(uint32_t)v0); if (c) goto L_1352be8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, (uint32_t)a2 & 0x1F))));
    (v0 = (uint64_t)(~(0ULL | RC_U64X(v0))));
    (v1 = (uint64_t)(RC_U64X(v1) & v0));
    A1_W32B(a3, 0, (uint32_t)v1);
    goto L_1352bf0;
L_1352be8:;
    if ((int32_t)v1 < 0) { A1_W8(a1, (uint8_t)0u); goto L_1352bf0; }
L_1352bf0:;
    { const bool c = (int64_t)a0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); if (c) { A3_BACK(); goto L_1352ba8; } }
L_1352bf8:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a3); if (a3xw1) xo[1] = a1_sx64(t0); if (a3xw2) xo[2] = a1_sx64(t1); } return; }
    __builtin_unreachable();
}

// getstagenumber_u14 (0x135ff08): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h3)
inline uint64_t getstagenumber_u14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a1 = 0x1460000u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)t0, 5))));
    a3 = 0x1460000u;
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1023));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 25524))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    a2 = A1_R32B(a3, 25516);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    a1 = 0x30000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    a1 = a1 | 0xFFFFu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(a1_sx64(a2) & a1_sx64(a1)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_135ff68; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_135ff6c;
L_135ff68:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ff6c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x135FF70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_135ffa0; }
    { const uint32_t _dynInsn = A1_R32(0x135FF74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_135ffa0; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_135ffa4;
L_135ffa0:;
    a1 = 0u;
L_135ffa4:;
    { const bool c = a1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_1360060; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 27))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 27))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a1))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(a1_sx64(a2) & RC_U64X(a0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_135ffe0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_135ffe4;
L_135ffe0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ffe4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x135FFE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1360018; }
    { const uint32_t _dynInsn = A1_R32(0x135FFECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_1360018; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_136001c;
L_1360018:;
    a1 = 0u;
L_136001c:;
    { const bool c = a1 == 0; { const uint32_t _dynInsn = A1_R32(0x1360020u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1360060; }
    { const uint32_t _dynInsn = A1_R32(0x1360024u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1360028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x136002Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    (a0 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(a0) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1360044; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
L_1360044:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)52428 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 2))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)52429));
    { { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } io_h3 = h3; return v0; }
L_1360060:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// getstagenumber_x14 (0x135ff08): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0; VU0 in -, out h3)
inline uint64_t getstagenumber_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a1 = 0x1460000u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)t0, 5))));
    a3 = 0x1460000u;
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1023));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 25524))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    a2 = A1_R32B(a3, 25516);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    a1 = 0x30000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    a1 = a1 | 0xFFFFu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(a1_sx64(a2) & a1_sx64(a1)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_135ff68; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_135ff6c;
L_135ff68:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ff6c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x135FF70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_135ffa0; }
    { const uint32_t _dynInsn = A1_R32(0x135FF74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_135ffa0; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_135ffa4;
L_135ffa0:;
    a1 = 0u;
L_135ffa4:;
    { const bool c = a1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_1360060; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 27))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 27))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a1))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(a1_sx64(a2) & RC_U64X(a0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_135ffe0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_135ffe4;
L_135ffe0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ffe4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x135FFE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1360018; }
    { const uint32_t _dynInsn = A1_R32(0x135FFECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_1360018; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_136001c;
L_1360018:;
    a1 = 0u;
L_136001c:;
    { const bool c = a1 == 0; { const uint32_t _dynInsn = A1_R32(0x1360020u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1360060; }
    { const uint32_t _dynInsn = A1_R32(0x1360024u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1360028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x136002Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    (a0 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(a0) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1360044; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
L_1360044:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)52428 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 2))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)52429));
    { { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a1_sx64(a3); xo[3] = t0; } io_h3 = h3; return v0; }
L_1360060:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a1_sx64(a3); xo[3] = t0; } io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// AI_GetAccelB3XYZ_x14 (0x13d4308): typed by tools/typed_lift2.py (round A6; frame leaf, extra outputs xo[0]=f12; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 6 slot accesses)
inline a1::RetVF AI_GetAccelB3XYZ_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    bool a3xw0 = false;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(a7_opq(at));
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4324;
    f12 = ((float)(f0)); a3xw0 = true;
L_13d4324:;
    f1 = a1f::mul(f12, f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    at = 0x40800000u;
    f0 = a1_flt(a7_opq(at));
    f0 = a1f::div(fcr31, f0, f12);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0x40400000u;
    f0 = a1_flt(a7_opq(at));
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0x40C00000u;
    f0 = a1_flt(a7_opq(at));
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f12); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// getPathDataAddr_x14 (0x1433340): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a3 xo[1]=t0; VU0 in -, out -)
inline uint64_t getPathDataAddr_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    bool a3xw1 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 15440))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    at = 0x1200000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, 4296))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 144))));
    { const bool c = (int32_t)v1 <= 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_14333a4; }
    (t0 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL)); a3xw1 = true;
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
L_1433380:;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    v1 = A1_R32B(a0, 28);
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v1; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 36)))); if (c) goto L_14333a8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)t0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); { A3_BACK(); goto L_1433380; } }
L_14333a4:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14333a8:;
    { if (xo) { xo[0] = a3; if (a3xw1) xo[1] = t0; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DAB10_x14 (0x1dab10): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f7 xo[8]=f8 xo[9]=f9 xo[10]=f10 xo[11]=f11 xo[12]=f12 xo[13]=f13 xo[14]=f14; VU0 in h3, out h2 h5)
inline uint64_t kernel_sub_001DAB10_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h5, uint64_t *xo)
{
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
    float f14 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fp = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    bool a3xw14 = false;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 511))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    sp = sp - 0x60u;
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W32B(sp, 4, (uint32_t)a1);
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s6, 20))));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (fp = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s6, 4))));
L_1dab60:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0))));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0)))); if (c) goto L_1dabd0; }
    (v1 = (uint64_t)(s5 ^ (uint64_t)(uint16_t)1));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, (uint32_t)s7))));
    if (v1 == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a2, 9))));
    A1_W32B(sp, 0, (uint32_t)v0);
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 72))));
    { const bool c = (int32_t)a1 <= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(fp, 0)))); if (c) goto L_1dabd0; }
    (s1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
L_1daba0:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1024))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 256)))); A3_CHARGE(); { const a1::RetVF a2_r = IirFilter_x14(ram, rt, (uint32_t)a0, a1, (uint32_t)a2); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1024))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4294967295))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 256)))); A3_CHARGE(); uint64_t a7c[14] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = IirFilter_x14(ram, rt, (uint32_t)a0, a1, (uint32_t)a2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f1 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); f6 = a1_flt((uint32_t)a7c[5]); f7 = a1_flt((uint32_t)a7c[6]); f8 = a1_flt((uint32_t)a7c[7]); f9 = a1_flt((uint32_t)a7c[8]); f10 = a1_flt((uint32_t)a7c[9]); f11 = a1_flt((uint32_t)a7c[10]); f12 = a1_flt((uint32_t)a7c[11]); f13 = a1_flt((uint32_t)a7c[12]); f14 = a1_flt((uint32_t)a7c[13]); a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw14 = true; }
    { const bool c = (int64_t)s1 != 0; (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1daba0; } }
L_1dabd0:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    s4 = s4 + 0xC4u;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, 196)))); if (c) { A3_BACK(); goto L_1dab60; } }
    { sp = sp + 0x60u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); if (a3xw7) xo[7] = a1_bits(f7); if (a3xw8) xo[8] = a1_bits(f8); if (a3xw9) xo[9] = a1_bits(f9); if (a3xw10) xo[10] = a1_bits(f10); if (a3xw11) xo[11] = a1_bits(f11); if (a3xw12) xo[12] = a1_bits(f12); if (a3xw13) xo[13] = a1_bits(f13); if (a3xw14) xo[14] = a1_bits(f14); } io_h2 = h2; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// GetScriptLayoutClass_x14 (0x110d058): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t GetScriptLayoutClass_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 11))));
    (a1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    { const bool c = (int64_t)(int32_t)a1 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32)))); if (c) goto L_110d090; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    if ((int64_t)(int32_t)a1 != (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_110d090; }
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetScriptAnimationClass_x14(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; xo[2] = a1_sx64(a1); } io_h2 = h2; return a7_t; }
L_110d090:;
    { if (xo) { xo[0] = v1; xo[2] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// manager_sub_0110EDC8_x14 (0x110edc8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f12 xo[1]=f20 xo[2]=f21 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1; VU0 in -, out -, frame 48 bytes, 4 slot accesses)
inline a1::RetVF manager_sub_0110EDC8_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t fcr31 = 0;
    uint8_t a2fr[48];
    bool a3xw0 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f21));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f20));
    { uint32_t bits = A1_R32B((uint32_t)s0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f12);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 24, bits); } if (c) goto L_110ee08; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 24, bits); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_110ee0c;
L_110ee08:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_110ee0c:;
    if ((int64_t)v0 != 0) { goto L_110ef48; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 28)))); if (c) goto L_110ee58; }
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(v1) < (int64_t)(int32_t)3) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3)))); goto L_110ee40; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_110ee4c;
L_110ee40:;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 28)))); if (c) goto L_110ee58; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
L_110ee4c:;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 32)))); if (c) goto L_110ee58; }
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 12))));
L_110ee58:;
    { uint32_t bits = A1_R32B((uint32_t)s0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B(s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    f21 = a1f::sub(f1, f2);
    { const bool c = (int64_t)v0 == 0; f12 = a1f::mul(f0, f12); a3xw0 = true; if (c) goto L_110eef0; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    { { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); } A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = rintf_x14(ram, rt, f12, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    f12 = ((float)(f0)); a3xw0 = true;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_110eeb8;
    at = 0xBF800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_110eec4;
    { uint32_t bits = A1_R32B(s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    goto L_110eef0;
L_110eeb8:;
    fcr31 = (a1f::lt(f12, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B(s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } goto L_110eef0; }
L_110eec4:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_110eedc;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw0 = true;
L_110eedc:;
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { uint32_t bits = A1_R32B(s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } if (c) goto L_110eef0; }
    at = 0xBF800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw0 = true;
L_110eef0:;
    f0 = a1f::add(f2, f12);
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    A1_W32(s1, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)s0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::sub(f2, f0);
    f0 = a1f::mul(f21, f0);
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { goto L_110ef48; }
    fcr31 = (a1f::eq(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(s1, 0, bits); } if (c) goto L_110ef30; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    v1 = 0xFFFFFFF0u;
    (v0 = (uint64_t)(RC_U64X(v0) & a1_sx64(v1)));
    goto L_110ef40;
L_110ef30:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    v1 = 0xFFFFFFF0u;
    (v0 = (uint64_t)(RC_U64X(v0) & a1_sx64(v1)));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)3));
L_110ef40:;
    A1_W32B((uint32_t)s0, 8, (uint32_t)v0);
L_110ef48:;
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f12); xo[1] = a1_bits(f20); xo[2] = a1_bits(f21); if (a3xw3) xo[3] = a2; if (a3xw4) xo[4] = a3; if (a3xw5) xo[5] = t0; if (a3xw6) xo[6] = t1; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_0137EB38_x14 (0x137eb38): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3; VU0 in vf0 vf7 vf10 vf13 vf14 vf15 vf16 vf17 vacc vstat, out vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vacc vq vstat h3, relocated calls 2)
inline a1::RetVF gamecore_sub_0137EB38_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h3, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint32_t s5 = 0;
    uint32_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h3 = io_h3;
    sp = sp - 0xF0u;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 144))));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 304))));
    (s5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 544))));
    (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 128))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)s3, 192); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_137ebd0;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)48896 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { const uint32_t _dynInsn = A1_R32(0x137EBC0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_137ebe8; }
    { const uint32_t _dynInsn = A1_R32(0x137EBC8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    goto L_137ebf0;
L_137ebd0:;
    at = 0x3F000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { const uint32_t _dynInsn = A1_R32(0x137EBE0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_137ebf0; }
    { const uint32_t _dynInsn = A1_R32(0x137EBE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_137ebe8:;
    { const uint32_t _dynInsn = A1_R32(0x137EBECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    goto L_137ebf4;
L_137ebf0:;
    { const uint32_t _dynInsn = A1_R32(0x137EBF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
L_137ebf4:;
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(sp, 32), a4::mk128(h3, v1));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 192))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (a1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL));
    a3 = sp + 0x20u;
    { (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s3, 160)))); A3_CHARGE(); a9x::diffQuat(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a3); }
    v0 = sp + 0x30u;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(s2, 0)));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf13));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf11));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(s4, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf11, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf11, vf10); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf10, vf10); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf13, vf13); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf13, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf12, vf17); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf17 = a4v::blendv_ps(vf17, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf17, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, vf17, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf0, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    v0 = sp + 0x10u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s5, 0), a4v::castps_si128(vf5));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 560))));
    (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 320))));
    v0 = sp + 0x50u;
    A1_W32B(s5, 12, a1_bits(f0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)s3, 192); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_137ed60;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)48896 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { const uint32_t _dynInsn = A1_R32(0x137ED54u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_137ed78; }
    { const uint32_t _dynInsn = A1_R32(0x137ED5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    goto L_137ed80;
L_137ed60:;
    at = 0x3F000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { const uint32_t _dynInsn = A1_R32(0x137ED70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_137ed80; }
    { const uint32_t _dynInsn = A1_R32(0x137ED74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_137ed78:;
    { const uint32_t _dynInsn = A1_R32(0x137ED7Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    goto L_137ed84;
L_137ed80:;
    { const uint32_t _dynInsn = A1_R32(0x137ED80u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
L_137ed84:;
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(sp, 112), a4::mk128(h3, v1));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 128))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    { a3 = sp + 0x70u; A3_CHARGE(); a9x::diffQuat(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a3); }
    v0 = sp + 0x80u;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(s2, 0)));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf13));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 144))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf11));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(s6, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf11, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf11, vf10); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf10, vf10); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf13, vf13); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf13, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf12, vf17); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf17 = a4v::blendv_ps(vf17, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf17, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, vf17, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf0, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    v0 = sp + 0x60u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s4, 0), a4v::castps_si128(vf5));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 528))));
    A1_W32B(s4, 12, a1_bits(f1));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s5, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    { uint32_t bits = a7_opq(v0); std::memcpy(&f0, &bits, sizeof(bits)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 580, bits); }
    { sp = sp + 0xF0u; if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a1_sx64(a3); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01423410_x14 (0x1423410): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f2 xo[1]=f12 xo[2]=f13 xo[3]=f14 xo[4]=f20 xo[5]=f21 xo[6]=a3 xo[7]=t0; VU0 in -, out h2 h3, relocated calls 1, frame 48 bytes, 4 slot accesses)
inline a1::RetVF gamecore_sub_01423410_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f20, float f21, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    float f14 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint8_t a2fr[48];
    { const uint32_t _dynInsn = A1_R32(0x1423414u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1423424u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x142342Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, _dynSimm)))); }
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f21));
    { const uint32_t _dynInsn = A1_R32(0x1423434u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); }
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f20));
    t0 = 4u;
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 272))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f21, &bits, sizeof(bits)); }
    (a2 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h2, v0));
    f12 = ((float)(f20));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15820 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    f13 = ((float)(f20));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    f14 = ((float)(f21));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16076 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)s0, 16), a4::mk128(h2, v0));
    at = 0xBF800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 32), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 48), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 64), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 80), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 96), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B((uint32_t)s0, 140, t0);
    A4_W128(ADD32((uint32_t)s0, 112), a4::mk128(h2, v0));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 188, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 192, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 168, bits); }
    A1_W32B((uint32_t)s0, 128, 0u);
    A1_W32B((uint32_t)s0, 132, 0u);
    A1_W32B((uint32_t)s0, 136, 0u);
    A1_W32B((uint32_t)s0, 144, 0u);
    A1_W32B((uint32_t)s0, 148, 0u);
    A1_W32B((uint32_t)s0, 152, 0u);
    A1_W32B((uint32_t)s0, 156, 0u);
    A1_W32B((uint32_t)s0, 160, 0u);
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 164, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 260, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 176, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 172, bits); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 180, bits); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 212, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 232, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 228, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 224, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 236, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 240, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 244, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 256, bits); }
    A1_W32B((uint32_t)s0, 184, 0u);
    A1_W32B((uint32_t)s0, 196, 0u);
    A1_W32B((uint32_t)s0, 200, 0u);
    A1_W32B((uint32_t)s0, 204, 0u);
    A1_W32B((uint32_t)s0, 208, 0u);
    A1_W32B((uint32_t)s0, 216, 0u);
    A1_W32B((uint32_t)s0, 220, 0u);
    A1_W32B((uint32_t)s0, 248, 0u);
    { A1_W32B((uint32_t)s0, 252, 0u); A3_CHARGE(); v0 = CAM_set_all_camera_base_data(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, f12, f13, f14, h2, h3); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 320))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    f12 = ((float)(f20));
    f13 = ((float)(f20));
    { f14 = ((float)(f21)); A3_CHARGE(); v0 = CAM_set_all_camera_base_data(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, f12, f13, f14, h2, h3); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 368)))); A3_CHARGE(); uint64_t a7c[1] = {0u}; { const a1::RetVF a2_r = constructor_camera_lws_anime_data_x14(ram, rt, (uint32_t)a0, h2, h3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a3 = a7c[0]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 512))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    f12 = ((float)(f20));
    f13 = ((float)(f20));
    { f14 = ((float)(f21)); A3_CHARGE(); v0 = CAM_set_all_camera_base_data(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, f12, f13, f14, h2, h3); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 560))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    f12 = ((float)(f20));
    f14 = ((float)(f21));
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { f13 = ((float)(f12)); A3_CHARGE(); v0 = CAM_set_all_camera_base_data(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, f12, f13, f14, h2, h3); }
    A1_W32B((uint32_t)s0, 608, 0u);
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    { if (xo) { xo[0] = a1_bits(f2); xo[1] = a1_bits(f12); xo[2] = a1_bits(f13); xo[3] = a1_bits(f14); xo[4] = a1_bits(f20); xo[5] = a1_bits(f21); xo[6] = a3; xo[7] = a1_sx64(t0); } io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetLayoutClass_u14 (0x10f1a20): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, jump tables 1, frame 16 bytes, 0 slot accesses)
inline uint64_t GetLayoutClass_u14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t h2 = io_h2;
    bool a3xw1 = false;
    bool a3xw2 = false;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967287))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)14) ? 1 : 0));
    if ((int64_t)v1 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10f1aa4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16496))));
    { const uint32_t a5_jt = (uint32_t)v1; switch (a5_jt) { case 0x10F1A58u: goto L_10f1a58; case 0x10F1A60u: goto L_10f1a60; case 0x10F1A70u: goto L_10f1a70; case 0x10F1A88u: goto L_10f1a88; case 0x10F1A98u: goto L_10f1a98; case 0x10F1AA0u: goto L_10f1aa0; default: A5_BADJUMP(0x10F1A4Cu, a5_jt); } }
L_10f1a58:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    goto L_10f1aa4;
L_10f1a60:;
    { A3_CHARGE(); uint64_t a7c[3] = {0u, a0, 0u}; v0 = GetScriptLayoutClass_x14(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; a1 = a7c[2]; a3xw1 = true; a3xw2 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 112))));
    goto L_10f1aa4;
L_10f1a70:;
    { A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = dynamic_cast_to_camera_obj_x14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, a7c); v1 = a7c[0]; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw1 = true;
    { if (xo) { xo[0] = v1; xo[1] = a0; } const uint32_t a4_t = get_camera_obj_layout_class(ram, rt, (uint32_t)a0); io_h2 = h2; return a1_sx64(a4_t); }
L_10f1a88:;
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetLightClass_x14(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return a7_t; }
L_10f1a98:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    goto L_10f1aa4;
L_10f1aa0:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10f1aa4:;
    { if (xo) { xo[0] = v1; if (a3xw1) xo[1] = a0; if (a3xw2) xo[2] = a1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0137F850_x14 (0x137f850): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f21 xo[3]=f22 xo[4]=a0 xo[5]=a1 xo[6]=a2 xo[7]=a3 xo[8]=t0 xo[9]=t1 xo[10]=t2 xo[11]=t3; VU0 in vf0 vf2 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 3)
inline a1::RetVF gamecore_sub_0137F850_x14(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294966880))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 256))));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 288, (uint32_t)a0);
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 400, bits); }
    f22 = ((float)(f13));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 392, bits); }
    f21 = ((float)(f12));
    { { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 384, bits); } A3_CHARGE(); v0 = a6x::memset(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 800))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 268, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 108, bits); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f20);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 816))));
    A1_W32B((uint32_t)sp, 292, (uint32_t)v0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a3, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a3, 0), a4v::castps_si128(vf5));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf7));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 60, 0u);
    A1_W32B((uint32_t)sp, 44, 0u);
    { A1_W32B((uint32_t)sp, 28, 0u); A3_CHARGE(); v0 = iosSetCameraMatrix(ram, rt, a0, a1, a2, a3, sp, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); }
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 192))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s7, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)s7, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)s7, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)s7, 48), a4v::castps_si128(vf29));
    f0 = a1f::add(f21, f21);
    fcr31 = (a1f::lt(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (c) goto L_137fa60; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f21 = a1f::mul(f20, f0);
L_137fa60:;
    { uint32_t bits; std::memcpy(&bits, &f21, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)sp, 100, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    f13 = a1f::mul(f20, f20);
    f0 = a1f::mul(f21, f21);
    f13 = a1f::subFinite(f13, f0);
    f13 = a1f::sqrt(fcr31, f13);
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { const uint32_t _dynInsn = A1_R32(0x137FAF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x137FAFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
    { f12 = ((float)(f21)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = AI_CalcIntersectionCircleXZ(ram, rt, a0, a1, a2, (uint32_t)a3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, vf0, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f21 = a1_flt((uint32_t)a7c[0]); f22 = a1_flt((uint32_t)a7c[1]); }
    { const bool c = (int32_t)v0 <= 0; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 96)))); if (c) goto L_137fbb8; }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf5));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f22, f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 272))));
    goto L_137fbc0;
L_137fbb8:;
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 272))));
L_137fbc0:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x137FBE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x137FBE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f12, &bits, sizeof(bits)); }
    { A3_CHARGE(); f0 = a9x::acosExact(ram, rt, f12); }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f2 = ((float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)sp, 280); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288)))); if (c) goto L_137fc40; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16585 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)4058));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f2 = a1f::sub(f0, f2);
L_137fc40:;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 292))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 852); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::div(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf5));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)fp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 112))));
    A4_W128(ADD32((uint32_t)s2, 80), a4::mk128(h2, v0));
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf0));
    f2 = a1f::mul(f21, f2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22096))));
    f2 = a1f::div(fcr31, f2, f22);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 160, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 164, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::mul(f1, f0);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 160, bits); } goto L_137fcc8; }
L_137fcc8:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 80))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (t1 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s6) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    { (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelC3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelA3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 16))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f12)}; { const a1::RetVF a2_r = AI_GetAccelB3XYZ_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); }
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 392); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 384); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 416)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f21); xo[3] = a1_bits(f22); xo[4] = a0; xo[5] = a1; xo[6] = a2; xo[7] = a3; xo[8] = t0; xo[9] = t1; xo[10] = t2; xo[11] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// sub_00101AC0 (0x101ac0): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -, break)
inline uint64_t sub_00101AC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t gp)
{
    uint64_t v0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -32620))));
    if ((int64_t)a1 == 0) { A6_BREAK(0x101AC9u); goto L_101acc; }
L_101acc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { uint32_t divisor = (uint32_t)a1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v0; } }
    (v0 = (uint64_t)(lo));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    { A1_W32B((uint32_t)gp, -32620, (uint32_t)a0); return v0; }
    __builtin_unreachable();
}

// sub_00101AF0 (0x101af0): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -, break)
inline uint64_t sub_00101AF0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t gp)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -32620))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 256))));
    if ((int64_t)v1 == 0) { A6_BREAK(0x101AFDu); goto L_101b00; }
L_101b00:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 255))));
    { uint32_t divisor = (uint32_t)v1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v0; } }
    (v0 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    { A1_W32B((uint32_t)gp, -32620, (uint32_t)a0); return v0; }
    __builtin_unreachable();
}

// QueryIntrContext (0x107460): typed by tools/typed_lift2.py (round A6; leaf; VU0 in c0st, out -)
inline uint64_t QueryIntrContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t io_c0st)
{
    uint64_t v0 = 0;
    uint32_t c0st = io_c0st;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)c0st)));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)1));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); return v0; }
    __builtin_unreachable();
}

// sub_00109288 (0x109288): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -, break)
inline uint64_t sub_00109288(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    if ((int64_t)v1 == 0) { A6_BREAK(0x109299u); goto L_10929c; }
L_10929c:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    (v0 = (uint64_t)(hi));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    A1_W32B((uint32_t)a0, 36, (uint32_t)v1);
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0)))); return v0; }
    __builtin_unreachable();
}

// DIntr (0x10fb20): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a0; VU0 in c0st, out c0st)
inline uint64_t DIntr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t c0st = io_c0st;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)c0st)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { const bool c = (int64_t)v1 == 0; (a0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(v1)) ? 1 : 0)); if (c) goto L_10fb64; }
L_10fb38:;
    c0st &= ~0x10000; // Disable interrupts
    // SYNC instruction - memory barrier // In recompiled code, we don't need explicit memory barriers
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)c0st)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    if ((int64_t)v0 != 0) { A3_BACK(); goto L_10fb38; }
    { (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (xo) { xo[0] = a0; } io_c0st = c0st; return v0; }
L_10fb64:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (xo) { xo[0] = a0; } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// EIntr (0x10fb70): typed by tools/typed_lift2.py (round A6; leaf; VU0 in c0st, out c0st)
inline uint64_t EIntr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t c0st = io_c0st;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)c0st)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    c0st |= 0x10000; // Enable interrupts
    { (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(v0)) ? 1 : 0)); io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// ps2___moddi3 (0x123a18): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3 xo[6]=t6 xo[7]=t8; VU0 in h3, out h4 h5 h6, break)
inline uint64_t ps2___moddi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t3, uint64_t t8, uint64_t sp, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t t9 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    bool a3xw0 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (t2 = (uint64_t)((int64_t)t0 >> (32 + 0)));
    (a0 = (uint64_t)(t2 << (32 + 0)));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    { const bool c = (int32_t)a0 >= 0; (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_123a8c; }
    (v0 = (uint64_t)(t0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    (a2 = (uint64_t)(a2 & v1)); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a0))));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) >> (32 + 0)));
    (a2 = (uint64_t)(a2 | v0)); a3xw0 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(a2 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a2 = (uint64_t)(a2 & a0)); a3xw0 = true;
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (t0 = (uint64_t)(a2 | v1));
    (t2 = (uint64_t)((int64_t)t0 >> (32 + 0)));
L_123a8c:;
    (a0 = (uint64_t)((int64_t)a1 >> (32 + 0)));
    { const bool c = (int32_t)a0 >= 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a0)))); if (c) goto L_123ae0; }
    (v0 = (uint64_t)(a1 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) >> (32 + 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (a3 = (uint64_t)(a3 & v1));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a3 = (uint64_t)(a3 | v0));
    (v1 = (uint64_t)(a3 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (a3 = (uint64_t)(a3 & a1));
    (v1 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v1) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
    (a0 = (uint64_t)(RC_U64X(a0) << (32 + 0)));
    (a1 = (uint64_t)(a3 | a0));
L_123ae0:;
    (t1 = (uint64_t)((int64_t)a1 >> (32 + 0)));
    (t5 = (uint64_t)(t0 << (32 + 0)));
    (t5 = (uint64_t)((int64_t)t5 >> (32 + 0)));
    (t2 = (uint64_t)(t2 << (32 + 0)));
    (t2 = (uint64_t)((int64_t)t2 >> (32 + 0)));
    (a3 = (uint64_t)(a1 << (32 + 0)));
    (a3 = (uint64_t)((int64_t)a3 >> (32 + 0)));
    { const bool c = (int64_t)t1 != 0; (t9 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL)); if (c) goto L_123dc0; }
    (v0 = (uint64_t)(((uint64_t)t2 < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535)); if (c) goto L_123b88; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_123b30; }
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_123b44;
L_123b30:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)a3) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_123b44:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28872)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)t4 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t4)))); if (c) goto L_123b7c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)v0 & 0x1F))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, (uint32_t)t4 & 0x1F))));
L_123b7c:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 16)))); a3xw0 = true;
    (t1 = (uint64_t)(a3 & (uint64_t)(uint16_t)65535));
    goto L_123ce8;
L_123b88:;
    { const bool c = (int64_t)a3 != 0; (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0)); if (c) goto L_123bb0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)a3 == 0) { A6_BREAK(0x123B99u); goto L_123b9c; }
L_123b9c:;
    { uint32_t divisor = (uint32_t)t1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v0; } }
    (v0 = (uint64_t)(lo));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0));
L_123bb0:;
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_123bc8; }
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_123bdc;
L_123bc8:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_123bdc:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28872)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)t4 != 0; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t4)))); if (c) goto L_123c10; }
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t2, (uint32_t)a3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 16))));
    (t6 = (uint64_t)(a3 & (uint64_t)(uint16_t)65535)); a3xw6 = true;
    goto L_123ce0;
L_123c10:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)t7 & 0x1F))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)t7 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, (uint32_t)t4 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 16))));
    { uint32_t divisor = (uint32_t)a1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 16))));
    (t6 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)65535)); a3xw6 = true;
    (t1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    if ((int64_t)t1 == 0) { A6_BREAK(0x123C41u); goto L_123c44; }
L_123c44:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t6; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a2 = (uint64_t)(RC_U64X(v1) | a0)); a3xw0 = true;
    (v0 = (uint64_t)(((uint64_t)a2 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t7 = (uint64_t)((uint64_t)t6 + (uint64_t)0ULL)); if (c) goto L_123c88; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a3)))); a3xw0 = true;
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a2) < (uint64_t)a3) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t0)))); a3xw0 = true; goto L_123c8c; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a2) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0); a3xw0 = true;
L_123c88:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t0)))); a3xw0 = true;
L_123c8c:;
    (a0 = (uint64_t)(t2 & (uint64_t)(uint16_t)65535));
    { uint32_t divisor = (uint32_t)t1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a2 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a2 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a2; } }
    if ((int64_t)t1 == 0) { A6_BREAK(0x123C99u); goto L_123c9c; }
L_123c9c:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t7; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0)))); if (c) goto L_123ce0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0)))); if (c) goto L_123ce0; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0))));
L_123ce0:;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); a3xw0 = true;
    (t1 = (uint64_t)((uint64_t)t6 + (uint64_t)0ULL));
L_123ce8:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)t2; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, 16))));
    if ((int64_t)a2 == 0) { A6_BREAK(0x123CF5u); goto L_123cf8; }
L_123cf8:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a1 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_123d3c; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)a3) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_123d3c; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0))));
L_123d3c:;
    (a0 = (uint64_t)(t5 & (uint64_t)(uint16_t)65535));
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a1; } }
    if ((int64_t)a2 == 0) { A6_BREAK(0x123D49u); goto L_123d4c; }
L_123d4c:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_123d88;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)RC_U64X(t0)) ? 1 : 0)); if (c) goto L_123d88; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_123d88:;
    { const bool c = (int64_t)t9 == 0; (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0)))); if (c) goto L_12403c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t3 = (uint64_t)(t3 | v0)); a3xw5 = true;
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    goto L_124038;
L_123dc0:;
    (v0 = (uint64_t)(((uint64_t)t2 < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535)); if (c) goto L_123e00; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(t5 << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw5 = true;
    (v0 = (uint64_t)(t2 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw5 = true;
    A1_W64B((uint32_t)sp, 0, t3);
    goto L_12403c;
L_123e00:;
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_123e20; }
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_123e34;
L_123e20:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t1) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_123e34:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28872)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)t4 != 0; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t4)))); if (c) goto L_123eb8; }
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)t2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t5, (uint32_t)a3)))); if (c) goto L_123e70; }
    (v0 = (uint64_t)(((uint64_t)t5 < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t5, (uint32_t)a3)))); if (c) goto L_123e80; }
L_123e70:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t2, (uint32_t)t1))));
    (v1 = (uint64_t)(((uint64_t)t5 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (t5 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
L_123e80:;
    { const bool c = (int64_t)t9 == 0; (v0 = (uint64_t)(t5 << (32 + 0))); if (c) goto L_12403c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw5 = true;
    (v0 = (uint64_t)(t2 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw5 = true;
    goto L_124038;
L_123eb8:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, (uint32_t)t7 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)t7 & 0x1F))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (t1 = (uint64_t)(RC_U64X(a0) | RC_U64X(v0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)t7 & 0x1F))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, (uint32_t)t4 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(a1) | RC_U64X(v1)));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, 16)))); a3xw0 = true;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 16))));
    (a1 = (uint64_t)(RC_U64X(t1) & (uint64_t)(uint16_t)65535));
    if ((int64_t)a2 == 0) { A6_BREAK(0x123EF1u); goto L_123ef4; }
L_123ef4:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (t6 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)t6 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)t0)))); goto L_123f44; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 4294967295)))); a3xw6 = true; if (c) goto L_123f40; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)t0)))); goto L_123f44; }
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 4294967295)))); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
L_123f40:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)t0))));
L_123f44:;
    if ((int64_t)a2 == 0) { A6_BREAK(0x123F49u); goto L_123f4c; }
L_123f4c:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v1; } }
    (a0 = (uint64_t)(t2 & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_123fa0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); a3xw0 = true; if (c) goto L_123f9c; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_123fa0; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t1))));
L_123f9c:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0))));
L_123fa0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t6, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | a2));
    (t2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    { uint64_t result = (uint64_t)(uint32_t)v0 * (uint64_t)(uint32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    (a2 = (uint64_t)(hi)); a3xw0 = true;
    (t0 = (uint64_t)(lo));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(t2) < (uint64_t)a2) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)a3)))); if (c) goto L_123fd8; }
    { const bool c = (int64_t)a2 != (int64_t)t2; (v0 = (uint64_t)(((uint64_t)t5 < (uint64_t)t0) ? 1 : 0)); if (c) goto L_123fe8; }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)a3)))); if (c) goto L_123fe8; }
L_123fd8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t1))));
    (v1 = (uint64_t)(((uint64_t)t0 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (t0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1)))); a3xw0 = true;
L_123fe8:;
    { const bool c = (int64_t)t9 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t5, (uint32_t)t0)))); if (c) goto L_12403c; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (v1 = (uint64_t)(((uint64_t)t5 < (uint64_t)RC_U64X(a0)) ? 1 : 0));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t7 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, (uint32_t)t4 & 0x1F))));
    (t3 = (uint64_t)(t3 & v1)); a3xw5 = true;
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) >> (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (t3 = (uint64_t)(t3 | v0)); a3xw5 = true;
    (t3 = (uint64_t)(t3 & a0)); a3xw5 = true;
    (t3 = (uint64_t)(t3 | v1)); a3xw5 = true;
L_124038:;
    A1_W64B((uint32_t)t9, 0, t3);
L_12403c:;
    { const bool c = (int64_t)s0 == 0; (v1 = (uint64_t)(A1_R64B((uint32_t)sp, 0))); if (c) goto L_124098; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)(RC_U64X(a0) << (32 + 0)));
    (t8 = (uint64_t)(t8 & a0)); a3xw7 = true;
    (v0 = (uint64_t)(v1 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v1))));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (t8 = (uint64_t)(t8 | v0)); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) >> (32 + 0)));
    (a0 = (uint64_t)(t8 << (32 + 0)));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    (t8 = (uint64_t)(t8 & v0)); a3xw7 = true;
    (a0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)a0) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (t8 = (uint64_t)(t8 | v1)); a3xw7 = true;
    A1_W64B((uint32_t)sp, 0, t8);
L_124098:;
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 0)));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (xo) { if (a3xw0) xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; if (a3xw5) xo[5] = t3; if (a3xw6) xo[6] = t6; if (a3xw7) xo[7] = t8; } io_h4 = h4; io_h5 = h5; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// ps2___udivdi3 (0x1240a8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3 xo[5]=t6; VU0 in h3, out h4 h6, break)
inline uint64_t ps2___udivdi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t6, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h6, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    bool a3xw0 = false;
    (t0 = (uint64_t)((int64_t)a1 >> (32 + 0)));
    (t2 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    (t1 = (uint64_t)(a1 << (32 + 0)));
    (t1 = (uint64_t)((int64_t)t1 >> (32 + 0)));
    (t3 = (uint64_t)(a0 << (32 + 0)));
    (t3 = (uint64_t)((int64_t)t3 >> (32 + 0)));
    { const bool c = (int64_t)t0 != 0; (v0 = (uint64_t)(((uint64_t)t2 < (uint64_t)t0) ? 1 : 0)); if (c) goto L_124448; }
    (v0 = (uint64_t)(((uint64_t)t2 < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535)); if (c) goto L_124208; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_1240f0; }
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_124104;
L_1240f0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)t1) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_124104:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28616)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)a2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2)))); if (c) goto L_12413c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)a2 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t3, (uint32_t)v0 & 0x1F))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, (uint32_t)a2 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)a2 & 0x1F))));
L_12413c:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, 16))));
    (t0 = (uint64_t)(t1 & (uint64_t)(uint16_t)65535));
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)t2; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t3, 16))));
    if ((int64_t)a2 == 0) { A6_BREAK(0x124151u); goto L_124154; }
L_124154:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a3 * (int64_t)(int32_t)t0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1)))); goto L_1241a4; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true; if (c) goto L_1241a0; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1)))); goto L_1241a4; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
L_1241a0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
L_1241a4:;
    if ((int64_t)a2 == 0) { A6_BREAK(0x1241A9u); goto L_1241ac; }
L_1241ac:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v1; } }
    (a0 = (uint64_t)(t3 & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)t0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16)))); if (c) goto L_1241fc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); if (c) goto L_1241f8; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
L_1241f8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16))));
L_1241fc:;
    (a2 = (uint64_t)(RC_U64X(v0) | a2));
    goto L_1245f0;
L_124208:;
    { const bool c = (int64_t)t1 != 0; (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t1) ? 1 : 0)); if (c) goto L_124230; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)t1 == 0) { A6_BREAK(0x124219u); goto L_12421c; }
L_12421c:;
    { uint32_t divisor = (uint32_t)t0; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v0; } }
    (v0 = (uint64_t)(lo));
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)t1) ? 1 : 0));
L_124230:;
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_124248; }
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_12425c;
L_124248:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)t1) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_12425c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28616)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)a2 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2)))); a3xw0 = true; if (c) goto L_124298; }
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t2, (uint32_t)t1))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, 16))));
    (t4 = (uint64_t)(t1 & (uint64_t)(uint16_t)65535));
    goto L_124380;
L_124298:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)a2 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t3, (uint32_t)a3 & 0x1F))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, (uint32_t)a2 & 0x1F))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)a3 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)a2 & 0x1F))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, 16))));
    { uint32_t divisor = (uint32_t)t0; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 16))));
    (t4 = (uint64_t)(RC_U64X(t1) & (uint64_t)(uint16_t)65535));
    (a2 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL));
    if ((int64_t)a2 == 0) { A6_BREAK(0x1242C9u); goto L_1242cc; }
L_1242cc:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a3 * (int64_t)(int32_t)t4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t5 = (uint64_t)((uint64_t)t4 + (uint64_t)0ULL)); if (c) goto L_124318; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true; if (c) goto L_124318; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1)))); goto L_12431c; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
L_124318:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
L_12431c:;
    if ((int64_t)a2 == 0) { A6_BREAK(0x124321u); goto L_124324; }
L_124324:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v1; } }
    (a0 = (uint64_t)(t2 & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)t5; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16)))); if (c) goto L_124378; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); if (c) goto L_124374; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16)))); if (c) goto L_124378; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
L_124374:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16))));
L_124378:;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
    (t5 = (uint64_t)(RC_U64X(v0) | a2));
L_124380:;
    (a2 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)t4 + (uint64_t)0ULL));
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)t2; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t3, 16))));
    if ((int64_t)a2 == 0) { A6_BREAK(0x124395u); goto L_124398; }
L_124398:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a3 * (int64_t)(int32_t)t0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1)))); goto L_1243e4; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true; if (c) goto L_1243e0; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1)))); goto L_1243e4; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
L_1243e0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
L_1243e4:;
    if ((int64_t)a2 == 0) { A6_BREAK(0x1243E9u); goto L_1243ec; }
L_1243ec:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v1; } }
    (a0 = (uint64_t)(t3 & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)t0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16)))); if (c) goto L_12443c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); if (c) goto L_124438; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a1)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
L_124438:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16))));
L_12443c:;
    (a2 = (uint64_t)(RC_U64X(v0) | a2));
    goto L_1245f4;
L_124448:;
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535)); goto L_124458; }
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1245f0;
L_124458:;
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_124478; }
    (v0 = (uint64_t)(((uint64_t)t0 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_12448c;
L_124478:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t0) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_12448c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28616)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)a2 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2)))); a3xw0 = true; if (c) goto L_1244d0; }
    (v0 = (uint64_t)(((uint64_t)t0 < (uint64_t)t2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1245f0; }
    (v0 = (uint64_t)(((uint64_t)t3 < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1245f0; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1245f0;
L_1244d0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, (uint32_t)a2 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t3, (uint32_t)a3 & 0x1F))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, (uint32_t)a2 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, (uint32_t)a3 & 0x1F))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)a2 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)a2 & 0x1F))));
    (t0 = (uint64_t)(RC_U64X(a0) | RC_U64X(v0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)a3 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(a1)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, 16))));
    { uint32_t divisor = (uint32_t)a1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 16))));
    (t4 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)65535));
    if ((int64_t)a1 == 0) { A6_BREAK(0x124509u); goto L_12450c; }
L_12450c:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a3 * (int64_t)(int32_t)t4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)RC_U64X(a2)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a2)))); goto L_12455c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true; if (c) goto L_124558; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a2)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a2)))); goto L_12455c; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
L_124558:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a2))));
L_12455c:;
    if ((int64_t)a1 == 0) { A6_BREAK(0x124561u); goto L_124564; }
L_124564:;
    { uint32_t divisor = (uint32_t)a1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v1; } }
    (a0 = (uint64_t)(t2 & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)t4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a2)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16)))); if (c) goto L_1245b8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295)))); if (c) goto L_1245b4; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(a2)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16)))); if (c) goto L_1245b8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
L_1245b4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16))));
L_1245b8:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a2))));
    (a2 = (uint64_t)(RC_U64X(v0) | a1));
    { uint64_t result = (uint64_t)(uint32_t)a2 * (uint64_t)(uint32_t)t1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    (a3 = (uint64_t)(hi)); a3xw0 = true;
    (a0 = (uint64_t)(lo));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)a3) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); goto L_1245f0; }
    { const bool c = (int64_t)a3 != (int64_t)v1; (t5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1245f4; }
    (v0 = (uint64_t)(((uint64_t)t3 < (uint64_t)a0) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1245f4;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
L_1245f0:;
    (t5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1245f4:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(a2 << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (t6 = (uint64_t)(t6 & v1));
    (t6 = (uint64_t)(t6 | v0));
    (v0 = (uint64_t)(t5 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t6 = (uint64_t)(t6 & v1));
    { (v0 = (uint64_t)(t6 | v0)); if (xo) { if (a3xw0) xo[0] = a3; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; xo[5] = t6; } io_h4 = h4; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// ps2___umoddi3 (0x124628): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3 xo[5]=t6 xo[6]=t8; VU0 in h3, out h4 h5 h6, break)
inline uint64_t ps2___umoddi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t3, uint64_t sp, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t t8 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    bool a3xw1 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (t1 = (uint64_t)((int64_t)a1 >> (32 + 0)));
    (t2 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    (a3 = (uint64_t)(a1 << (32 + 0)));
    (a3 = (uint64_t)((int64_t)a3 >> (32 + 0)));
    (t5 = (uint64_t)(a0 << (32 + 0)));
    (t5 = (uint64_t)((int64_t)t5 >> (32 + 0)));
    { const bool c = (int64_t)t1 != 0; (t8 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL)); if (c) goto L_124908; }
    (v0 = (uint64_t)(((uint64_t)t2 < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535)); if (c) goto L_1246d0; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_124678; }
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_12468c;
L_124678:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)a3) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_12468c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28360)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)t4 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t4)))); if (c) goto L_1246c4; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)v0 & 0x1F))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, (uint32_t)t4 & 0x1F))));
L_1246c4:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 16))));
    (t1 = (uint64_t)(a3 & (uint64_t)(uint16_t)65535));
    goto L_124830;
L_1246d0:;
    { const bool c = (int64_t)a3 != 0; (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0)); if (c) goto L_1246f8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)a3 == 0) { A6_BREAK(0x1246E1u); goto L_1246e4; }
L_1246e4:;
    { uint32_t divisor = (uint32_t)t1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v0; } }
    (v0 = (uint64_t)(lo));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0));
L_1246f8:;
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_124710; }
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_124724;
L_124710:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a3) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_124724:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28360)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)t4 != 0; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t4)))); if (c) goto L_124758; }
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t2, (uint32_t)a3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 16))));
    (t6 = (uint64_t)(a3 & (uint64_t)(uint16_t)65535)); a3xw5 = true;
    goto L_124828;
L_124758:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)t7 & 0x1F))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)t7 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, (uint32_t)t4 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 16))));
    { uint32_t divisor = (uint32_t)a1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 16))));
    (t6 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)65535)); a3xw5 = true;
    (t1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    if ((int64_t)t1 == 0) { A6_BREAK(0x124789u); goto L_12478c; }
L_12478c:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t6; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a2 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a2 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t7 = (uint64_t)((uint64_t)t6 + (uint64_t)0ULL)); if (c) goto L_1247d0; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a2) < (uint64_t)a3) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t0)))); goto L_1247d4; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a2) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
L_1247d0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t0))));
L_1247d4:;
    (a0 = (uint64_t)(t2 & (uint64_t)(uint16_t)65535));
    { uint32_t divisor = (uint32_t)t1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a2 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a2 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a2; } }
    if ((int64_t)t1 == 0) { A6_BREAK(0x1247E1u); goto L_1247e4; }
L_1247e4:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t7; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0)))); if (c) goto L_124828; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0)))); if (c) goto L_124828; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0))));
L_124828:;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)t6 + (uint64_t)0ULL));
L_124830:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)t2 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)t2; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, 16))));
    if ((int64_t)a2 == 0) { A6_BREAK(0x12483Du); goto L_124840; }
L_124840:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a1 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_124884; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)a3) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_124884; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0))));
L_124884:;
    (a0 = (uint64_t)(t5 & (uint64_t)(uint16_t)65535));
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a1; } }
    if ((int64_t)a2 == 0) { A6_BREAK(0x124891u); goto L_124894; }
L_124894:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)t1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1248d0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)RC_U64X(t0)) ? 1 : 0)); if (c) goto L_1248d0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_1248d0:;
    { const bool c = (int64_t)t8 == 0; (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t0)))); if (c) goto L_124b84; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t3 = (uint64_t)(t3 | v0)); a3xw4 = true;
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    goto L_124b80;
L_124908:;
    (v0 = (uint64_t)(((uint64_t)t2 < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535)); if (c) goto L_124948; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(t5 << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw4 = true;
    (v0 = (uint64_t)(t2 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw4 = true;
    A1_W64B((uint32_t)sp, 0, t3);
    goto L_124b84;
L_124948:;
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_124968; }
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)(int64_t)(int32_t)256) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    goto L_12497c;
L_124968:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)t1) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
L_12497c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -28360)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    { const bool c = (int64_t)t4 != 0; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t4)))); if (c) goto L_124a00; }
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)t2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t5, (uint32_t)a3)))); if (c) goto L_1249b8; }
    (v0 = (uint64_t)(((uint64_t)t5 < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t5, (uint32_t)a3)))); if (c) goto L_1249c8; }
L_1249b8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t2, (uint32_t)t1))));
    (v1 = (uint64_t)(((uint64_t)t5 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (t5 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
L_1249c8:;
    { const bool c = (int64_t)t8 == 0; (v0 = (uint64_t)(t5 << (32 + 0))); if (c) goto L_124b84; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw4 = true;
    (v0 = (uint64_t)(t2 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    (t3 = (uint64_t)(t3 | v0)); a3xw4 = true;
    goto L_124b80;
L_124a00:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, (uint32_t)t7 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t5, (uint32_t)t7 & 0x1F))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, (uint32_t)t4 & 0x1F))));
    (t1 = (uint64_t)(RC_U64X(a0) | RC_U64X(v0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)t7 & 0x1F))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, (uint32_t)t4 & 0x1F))));
    (t2 = (uint64_t)(RC_U64X(a1) | RC_U64X(v1)));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, 16))));
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 16))));
    (a1 = (uint64_t)(RC_U64X(t1) & (uint64_t)(uint16_t)65535));
    if ((int64_t)a2 == 0) { A6_BREAK(0x124A39u); goto L_124a3c; }
L_124a3c:;
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (t6 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw5 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)t6 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw1 = true;
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)t0)))); goto L_124a8c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 4294967295)))); a3xw5 = true; if (c) goto L_124a88; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)t0)))); goto L_124a8c; }
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 4294967295)))); a3xw5 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t1))));
L_124a88:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)t0))));
L_124a8c:;
    if ((int64_t)a2 == 0) { A6_BREAK(0x124A91u); goto L_124a94; }
L_124a94:;
    { uint32_t divisor = (uint32_t)a2; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v1 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v1; } }
    (a0 = (uint64_t)(t2 & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw1 = true;
    (a1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_124ae8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); if (c) goto L_124ae4; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a1) < (uint64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0)))); goto L_124ae8; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t1))));
L_124ae4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)t0))));
L_124ae8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t6, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | a2));
    (t2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    { uint64_t result = (uint64_t)(uint32_t)v0 * (uint64_t)(uint32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    (a2 = (uint64_t)(hi));
    (t0 = (uint64_t)(lo)); a3xw1 = true;
    (v1 = (uint64_t)(((uint64_t)RC_U64X(t2) < (uint64_t)a2) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)a3)))); if (c) goto L_124b20; }
    { const bool c = (int64_t)a2 != (int64_t)t2; (v0 = (uint64_t)(((uint64_t)t5 < (uint64_t)t0) ? 1 : 0)); if (c) goto L_124b30; }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)a3)))); if (c) goto L_124b30; }
L_124b20:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t1))));
    (v1 = (uint64_t)(((uint64_t)t0 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (t0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); a3xw1 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
L_124b30:;
    { const bool c = (int64_t)t8 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t5, (uint32_t)t0)))); if (c) goto L_124b84; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (v1 = (uint64_t)(((uint64_t)t5 < (uint64_t)RC_U64X(a0)) ? 1 : 0));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, (uint32_t)t7 & 0x1F))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, (uint32_t)t4 & 0x1F))));
    (t3 = (uint64_t)(t3 & v1)); a3xw4 = true;
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) >> (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, (uint32_t)t4 & 0x1F))));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (t3 = (uint64_t)(t3 | v0)); a3xw4 = true;
    (t3 = (uint64_t)(t3 & a0)); a3xw4 = true;
    (t3 = (uint64_t)(t3 | v1)); a3xw4 = true;
L_124b80:;
    A1_W64B((uint32_t)t8, 0, t3);
L_124b84:;
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 0)));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { xo[0] = a3; if (a3xw1) xo[1] = t0; xo[2] = t1; xo[3] = t2; if (a3xw4) xo[4] = t3; if (a3xw5) xo[5] = t6; xo[6] = t8; } io_h4 = h4; io_h5 = h5; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// ps2___muldi3 (0x126f28): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=hi xo[7]=lo; VU0 in -, out hi1 lo1)
inline uint64_t ps2___muldi3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t1, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    (v0 = (uint64_t)(a1 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)a1 >> (32 + 0)));
    (v1 = (uint64_t)(a0 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    { uint64_t result = (uint64_t)(uint32_t)v1 * (uint64_t)(uint32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    (a2 = (uint64_t)(lo));
    (t0 = (uint64_t)(hi));
    (a2 = (uint64_t)(a2 << (32 + 0)));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a1 = (uint64_t)(RC_U64X(a1) << (32 + 0)));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a2 = (uint64_t)(a2 >> (32 + 0)));
    (t1 = (uint64_t)(t1 & a1));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (a3 = (uint64_t)(RC_U64X(a3) >> (32 + 0)));
    (t1 = (uint64_t)(t1 | a2));
    (t0 = (uint64_t)(t0 << (32 + 0)));
    (t1 = (uint64_t)(t1 & a3));
    (t1 = (uint64_t)(t1 | t0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)t1 >> (32 + 0)));
    (a3 = (uint64_t)(t1 & a3));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    { (v0 = (uint64_t)(a3 | v0)); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = hi; xo[7] = lo; } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// sub_0012B1D8 (0x12b1d8): typed by tools/typed_lift2.py (round A6; loop guest frame leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1; VU0 in -, out -, break)
inline uint64_t sub_0012B1D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw1 = false;
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)a2);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    { const bool c = (int32_t)a1 >= 0; (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294966976)))); if (c) goto L_12b1f8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 45))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a1))));
    goto L_12b1fc;
L_12b1f8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 43))));
L_12b1fc:;
    A1_W8B((uint32_t)a2, 0, (uint8_t)(uint32_t)v0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 308))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)10) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t1 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL)); if (c) goto L_12b290; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10)))); a3xw1 = true;
    { int32_t divisor = (int32_t)t0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
L_12b220:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295))));
    if ((int64_t)t0 == 0) { A6_BREAK(0x12B229u); goto L_12b22c; }
L_12b22c:;
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)(hi));
    (a1 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v1 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)10) ? 1 : 0));
    A1_W8B((uint32_t)a3, 0, (uint8_t)(uint32_t)v0);
    if ((int64_t)v1 == 0) { { int32_t divisor = (int32_t)t0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } } { A3_BACK(); goto L_12b220; } }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 48))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; A1_W8B((uint32_t)a3, 0, (uint8_t)(uint32_t)v0); if (c) goto L_12b2a8; }
    (a1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
L_12b268:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a3, 0)));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)a1) ? 1 : 0));
    A1_W8B((uint32_t)a2, 0, (uint8_t)(uint32_t)v0);
    { const bool c = (int64_t)v1 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); if (c) { A3_BACK(); goto L_12b268; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)a0))));
    goto L_12b2ac;
L_12b290:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 48))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 48))));
    A1_W8B((uint32_t)a2, 0, (uint8_t)(uint32_t)v0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    A1_W8B((uint32_t)a2, 0, (uint8_t)(uint32_t)v1);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_12b2a8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)a0))));
L_12b2ac:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 320)))); if (xo) { xo[0] = a3; if (a3xw1) xo[1] = t0; xo[2] = t1; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001AD850 (0x1ad850): typed by tools/typed_lift2.py (round A6; leaf; VU0 in c0st, out c0st)
inline uint64_t kernel_sub_001AD850(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t c0st = io_c0st;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294957216))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)16));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2688))));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)RC_U64X(a2)));
    A1_W64B((uint32_t)a0, 0, v0);
    A1_W32B((uint32_t)v1, 0, (uint32_t)a1);
    // SYNC instruction - memory barrier // In recompiled code, we don't need explicit memory barriers
    c0st |= 0x10000; // Enable interrupts
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Shock_SetMotor (0x1b3f68): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2; VU0 in h8, out h7, break)
inline uint64_t Shock_SetMotor(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h7, uint64_t io_h8, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    (t1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)a2, 0)));
    (t0 = (uint64_t)(a1 & (uint64_t)(uint16_t)255));
    (t2 = (uint64_t)(a0 & (uint64_t)(uint16_t)255));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)t1))));
    { const bool c = (int32_t)a1 <= 0; (a3 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL)); if (c) goto L_1b4010; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)41) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255)))); goto L_1b4050; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int32_t)61) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 8)))); if (c) goto L_1b3fe0; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int32_t)61) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 8)))); if (c) goto L_1b3fe0; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int32_t)51) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 60)))); if (c) goto L_1b3fb8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
    goto L_1b4050;
L_1b3fb8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 100))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)t0))));
    if ((int64_t)a0 == 0) { A6_BREAK(0x1B3FC5u); goto L_1b3fc8; }
L_1b3fc8:;
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    { int32_t divisor = (int32_t)a0;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    goto L_1b3ff4;
L_1b3fe0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 40))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a1))));
    if ((int64_t)v1 == 0) { A6_BREAK(0x1B3FEDu); goto L_1b3ff0; }
L_1b3ff0:;
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
L_1b3ff4:;
    (v0 = (uint64_t)(lo));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(((int64_t)(int64_t)t1 < (int64_t)(int64_t)a1) ? 1 : 0));
    if ((int64_t)v1 != 0) { (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); goto L_1b4050; }
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)a2, 2)));
    goto L_1b4054;
L_1b4010:;
    if ((int32_t)a1 >= 0) { (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)a2, 2))); goto L_1b4054; }
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)4294967266) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1b4050; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 30))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a1))));
    if ((int64_t)v1 == 0) { A6_BREAK(0x1B4035u); goto L_1b4038; }
L_1b4038:;
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(lo));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 255))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)t1) ? 1 : 0));
    (a3 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    if (RC_U64X(v1) == 0) do { const a4::q128 _v = (a4::mk128(h8, t0)); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
L_1b4050:;
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)a2, 2)));
L_1b4054:;
    (t0 = (uint64_t)(a3 & (uint64_t)(uint16_t)255));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 16))));
    A1_W16B((uint32_t)a2, 2, (uint16_t)(uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 16))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)1035) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1034)))); if (c) goto L_1b4080; }
    A1_W16B((uint32_t)a2, 2, (uint16_t)(uint32_t)v0);
    goto L_1b4088;
L_1b4080:;
    if ((int32_t)v1 < 0) { A1_W16B((uint32_t)a2, 2, (uint16_t)0u); goto L_1b4088; }
L_1b4088:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a2, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1035))));
    if ((int64_t)v1 == 0) { A6_BREAK(0x1B4095u); goto L_1b4098; }
L_1b4098:;
    A1_W8B((uint32_t)a2, 4, (uint8_t)(uint32_t)t2);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
    { uint32_t divisor = (uint32_t)v1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)a0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)a0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    A1_W8B((uint32_t)a2, 5, (uint8_t)(uint32_t)t0);
    A1_W16B((uint32_t)a2, 2, (uint16_t)(uint32_t)v0);
    (a0 = (uint64_t)(lo));
    { A1_W16B((uint32_t)a2, 0, (uint16_t)(uint32_t)a0); if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; xo[3] = t2; } io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// iosGetRandomVector (0x1be838): typed by tools/typed_lift2.py (round A6; leaf; VU0 in vf0 vf5 vrr, out vf5 vrr)
inline void iosGetRandomVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vrr)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vrr = io_vrr;
    { uint32_t r = static_cast<uint32_t>(a4v::cvtsi128_si32(a4v::castps_si128(vrr))); r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; vrr = a4v::castsi128_ps(a4v::set1_epi32(static_cast<int32_t>(r))); vf5 = a4v::dest<1, 0, 0, 0>(vf5, vrr); }
    { uint32_t r = static_cast<uint32_t>(a4v::cvtsi128_si32(a4v::castps_si128(vrr))); r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; vrr = a4v::castsi128_ps(a4v::set1_epi32(static_cast<int32_t>(r))); vf5 = a4v::dest<0, 1, 0, 0>(vf5, vrr); }
    { uint32_t r = static_cast<uint32_t>(a4v::cvtsi128_si32(a4v::castps_si128(vrr))); r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; vrr = a4v::castsi128_ps(a4v::set1_epi32(static_cast<int32_t>(r))); vf5 = a4v::dest<0, 0, 1, 0>(vf5, vrr); }
    { a4::vec4 res = a4v::subK<0, 1>(vf5, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vrr = vrr; return; }
    __builtin_unreachable();
}

// iosGetDRandomVector (0x1be858): typed by tools/typed_lift2.py (round A6; leaf; VU0 in vf0 vf5 vrr, out vf5 vrr)
inline void iosGetDRandomVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vrr)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vrr = io_vrr;
    { uint32_t r = static_cast<uint32_t>(a4v::cvtsi128_si32(a4v::castps_si128(vrr))); r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; vrr = a4v::castsi128_ps(a4v::set1_epi32(static_cast<int32_t>(r))); vf5 = a4v::dest<1, 0, 0, 0>(vf5, vrr); }
    { uint32_t r = static_cast<uint32_t>(a4v::cvtsi128_si32(a4v::castps_si128(vrr))); r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; vrr = a4v::castsi128_ps(a4v::set1_epi32(static_cast<int32_t>(r))); vf5 = a4v::dest<0, 1, 0, 0>(vf5, vrr); }
    { uint32_t r = static_cast<uint32_t>(a4v::cvtsi128_si32(a4v::castps_si128(vrr))); r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; vrr = a4v::castsi128_ps(a4v::set1_epi32(static_cast<int32_t>(r))); vf5 = a4v::dest<0, 0, 1, 0>(vf5, vrr); }
    { a4::vec4 res = a4v::subK<0, 1>(vf5, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf5, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vrr = vrr; return; }
    __builtin_unreachable();
}

// getFrameBufferSize (0x1c2058): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out hi1 lo1)
inline uint32_t getFrameBufferSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    v0 = A1_R32B(v1, 8);
    a0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a1; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = v0 << 1;
    { v0 = a0 + v0; if (xo) { xo[0] = a1_sx64(a2); } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// getFrameBufferSizeFBP (0x1c2088): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out hi1 lo1)
inline uint32_t getFrameBufferSizeFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    a0 = A1_R32B(v1, 8);
    v0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    a0 = a0 << 1;
    v0 = v0 + a0;
    { v0 = (uint32_t)((int32_t)v0 >> 11); if (xo) { xo[0] = a1_sx64(a2); } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// getFrameBufferSizeTBP (0x1c20b8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out hi1 lo1)
inline uint32_t getFrameBufferSizeTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    a0 = A1_R32B(v1, 8);
    v0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    a0 = a0 << 1;
    v0 = v0 + a0;
    { v0 = (uint32_t)((int32_t)v0 >> 6); if (xo) { xo[0] = a1_sx64(a2); } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// getDrawBufferAddressTBP (0x1c20f8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out hi1 lo1)
inline uint32_t getDrawBufferAddressTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    a0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    v0 = A1_R32B(v1, 8);
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    a0 = (uint32_t)((int32_t)a0 >> 6);
    v0 = (uint32_t)((int32_t)v0 >> 6);
    { v0 = a0 + v0; if (xo) { xo[0] = a1_sx64(a2); } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// getDrawBufferAddressFBP (0x1c2128): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out hi1 lo1)
inline uint32_t getDrawBufferAddressFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    a0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    v0 = A1_R32B(v1, 8);
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    a0 = (uint32_t)((int32_t)a0 >> 11);
    v0 = (uint32_t)((int32_t)v0 >> 11);
    { v0 = a0 + v0; if (xo) { xo[0] = a1_sx64(a2); } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// getTextureAreaAddressTBP (0x1c2198): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out hi1 lo1)
inline uint32_t getTextureAreaAddressTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    v0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    a0 = A1_R32B(v1, 8);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a2; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = (uint32_t)((int32_t)v0 >> 6);
    a0 = (uint32_t)((int32_t)a0 >> 6);
    v0 = v0 + a0;
    { v0 = v0 + a0; io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// getTextureAreaAddressFBP (0x1c21d0): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out hi1 lo1)
inline uint32_t getTextureAreaAddressFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a2 = A1_R32B(v1, 12);
    v0 = A1_R32(v1);
    a1 = A1_R32B(v1, 4);
    a0 = A1_R32B(v1, 8);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a2; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = (uint32_t)((int32_t)v0 >> 11);
    a0 = (uint32_t)((int32_t)a0 >> 11);
    v0 = v0 + a0;
    { v0 = v0 + a0; io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// iosGetFontDataWithID (0x1c90e8): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out hi1 lo1)
inline uint64_t iosGetFontDataWithID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 5))));
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 2680))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)v0; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { const bool c = (int64_t)a3 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_1c9158; }
    { uint32_t addr = ADD32((uint32_t)a2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((RC_U64X(a0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
L_1c9158:;
    { io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetPitch (0x1d6578): typed by tools/typed_lift2.py (round A6; frame leaf, extra outputs xo[0]=t2 xo[1]=t3 xo[2]=t6; VU0 in -, out -, break, frame 16 bytes, 0 slot accesses)
inline uint32_t Sg2SlotSetPitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint32_t s0 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    v0 = 0x290000u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    v0 = v0 - 0x39D0u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    (t7 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, v0))));
    (t6 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    (t3 = (uint64_t)((uint64_t)a1_sx64(t7) + (uint64_t)0ULL));
    (t2 = (uint64_t)((uint64_t)a1_sx64(t6) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(t2) + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)RC_U64X(t3) + (uint64_t)0ULL));
    s0 = 0xCu;
    t4 = 0xCu;
    t5 = 3u;
    A1_W32B((uint32_t)t2, 4, a1);
    A1_W32B((uint32_t)a0, 12, a2);
    A1_W32B(t6, 8, a3);
    A1_W32B((uint32_t)t3, 48, t0);
    A1_W32B((uint32_t)v1, 112, t1);
    if (s0 == 0) { A6_BREAK(0x1D65D5u); goto L_1d65d8; }
L_1d65d8:;
    v0 = A1_R32(t7);
    v0 = v0 | 0x80u;
    A1_W32(t7, v0);
    { if (xo) { xo[0] = t2; xo[1] = t3; xo[2] = a1_sx64(t6); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetLfoPitchSpeed (0x1d67f0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out -, break)
inline uint64_t Sg2SlotSetLfoPitchSpeed(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (t1 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 112))));
    { const bool c = (int64_t)a2 == 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1)))); if (c) goto L_1d684c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 127))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 60))));
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 240))));
    if ((int64_t)v0 == 0) { A6_BREAK(0x1D6835u); goto L_1d6838; }
L_1d6838:;
    (a1 = (uint64_t)(lo));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)a0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (a0 = (uint64_t)(lo));
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
L_1d684c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    A1_W32B((uint32_t)v0, 8, (uint32_t)a2);
    { const bool c = (int64_t)a2 == 0; A1_W32B((uint32_t)v1, 16, 0u); if (c) goto L_1d6890; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 12))));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)a0)))); if (c) goto L_1d6890; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)1024));
    { A1_W32B((uint32_t)v1, 0, (uint32_t)v0); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; } return v0; }
L_1d6890:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294966271))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    { A1_W32B((uint32_t)a1, 0, (uint32_t)v0); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DA790 (0x1da790): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in -, out hi1 lo1, break)
inline uint64_t kernel_sub_001DA790(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (t5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t7, 1))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1024))));
    { const bool c = (int32_t)a1 <= 0; (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_1da82c; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 100)))); a3xw3 = true;
L_1da7b8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, 1))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t6)))); a3xw2 = true;
L_1da7c8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)t0, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 2)))); a3xw2 = true;
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 4294967295))));
    if ((int64_t)t1 == 0) { A6_BREAK(0x1DA7D9u); goto L_1da7dc; }
L_1da7dc:;
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 1))));
    { int32_t divisor = (int32_t)t1;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int32_t divisor = (int32_t)t1; int32_t dividend = (int32_t)v1; if (divisor != 0) {     if (divisor == -1 && dividend == INT32_MIN) {         lo1 = (uint64_t)(int64_t)INT32_MIN; hi1 = 0;     } else {         lo1 = (uint64_t)(int64_t)(dividend / divisor);         hi1 = (uint64_t)(int64_t)(dividend % divisor);     } } else {     lo1 = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi1 = (uint64_t)(int64_t)dividend; } }
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(lo1));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); } a3xw1 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t3, 0, bits); }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4))));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 0, bits); }
    { const bool c = (int32_t)t4 >= 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4)))); if (c) { A3_BACK(); goto L_1da7c8; } }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t5) < (int64_t)(int64_t)a1) ? 1 : 0));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1024))));
    { const bool c = (int64_t)v0 != 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1024)))); if (c) { A3_BACK(); goto L_1da7b8; } }
L_1da82c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, (uint32_t)a1))));
    A1_W32B((uint32_t)a0, 16, (uint32_t)v0);
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { A1_W32B((uint32_t)a0, 16, 0u); goto L_1da844; }
L_1da844:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = t0; if (a3xw3) xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DA850 (0x1da850): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=t2 xo[3]=t3; VU0 in h8, out hi1 lo1 h10, break)
inline void kernel_sub_001DA850(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t io_h8, uint64_t &io_h10, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t t8 = 0;
    uint64_t t9 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h8 = io_h8;
    uint64_t h10 = io_h10;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    (t8 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t9 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 4))));
    (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 1024))));
    { const bool c = (int32_t)a1 <= 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 16)))); if (c) goto L_1da8ec; }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 100)))); a3xw3 = true;
L_1da870:;
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
L_1da878:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 1))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t7))));
    (a0 = (uint64_t)(((int64_t)(int64_t)t1 < (int64_t)(int64_t)RC_U64X(t2)) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v0, 0))));
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 4294967295))));
    if ((int64_t)t3 == 0) { A6_BREAK(0x1DA895u); goto L_1da898; }
L_1da898:;
    if (a0 != 0) do { const a4::q128 _v = (a4::mk128(h8, t0)); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a2; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int32_t divisor = (int32_t)t3;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int32_t divisor = (int32_t)t3; int32_t dividend = (int32_t)v1; if (divisor != 0) {     if (divisor == -1 && dividend == INT32_MIN) {         lo1 = (uint64_t)(int64_t)INT32_MIN; hi1 = 0;     } else {         lo1 = (uint64_t)(int64_t)(dividend / divisor);         hi1 = (uint64_t)(int64_t)(dividend % divisor);     } } else {     lo1 = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi1 = (uint64_t)(int64_t)dividend; } }
    (v0 = (uint64_t)(lo));
    (v1 = (uint64_t)(lo1));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); } a3xw1 = true;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t5, 0, bits); }
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4))));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t4, 0, bits); }
    { const bool c = (int32_t)t6 >= 0; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 4)))); if (c) { A3_BACK(); goto L_1da878; } }
    (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 256))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 1024))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t9) < (int64_t)(int64_t)a1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 1024)))); if (c) { A3_BACK(); goto L_1da870; } }
L_1da8ec:;
    { A1_W32B((uint32_t)t8, 16, (uint32_t)t2); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); xo[2] = t2; if (a3xw3) xo[3] = t3; } io_hi1 = hi1; io_lo1 = lo1; io_h10 = h10; return; }
    __builtin_unreachable();
}

// kernel_sub_001DA8F8 (0x1da8f8): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=f0 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in -, out -, break)
inline uint64_t kernel_sub_001DA8F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t *xo)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw0 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (t4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t5, 1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    { const bool c = (int32_t)a1 <= 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_1da9a4; }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 100)))); a3xw4 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255)))); a3xw2 = true;
L_1da920:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967295)))); a3xw2 = true;
    if ((int64_t)t3 == 0) { A6_BREAK(0x1DA931u); goto L_1da934; }
L_1da934:;
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int32_t divisor = (int32_t)t3;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(lo));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t0, 0, bits); }
    { const bool c = (int32_t)t1 >= 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 4)))); if (c) { A3_BACK(); goto L_1da920; } }
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 100)))); a3xw3 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255)))); a3xw2 = true;
L_1da960:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967295)))); a3xw2 = true;
    if ((int64_t)t2 == 0) { A6_BREAK(0x1DA971u); goto L_1da974; }
L_1da974:;
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int32_t divisor = (int32_t)t2;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(lo));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t0, 0, bits); }
    { const bool c = (int32_t)t1 >= 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 4)))); if (c) { A3_BACK(); goto L_1da960; } }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 512))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t4) < (int64_t)(int64_t)a1) ? 1 : 0));
    if ((int64_t)v0 != 0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255)))); a3xw2 = true; { A3_BACK(); goto L_1da920; } }
L_1da9a4:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, (uint32_t)a1))));
    A1_W32B((uint32_t)a0, 16, (uint32_t)v0);
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { A1_W32B((uint32_t)a0, 16, 0u); goto L_1da9bc; }
L_1da9bc:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = t0; if (a3xw2) xo[2] = t1; if (a3xw3) xo[3] = t2; if (a3xw4) xo[4] = t3; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DA9C8 (0x1da9c8): typed by tools/typed_lift2.py (round A6; loop guest frame leaf, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=t2 xo[3]=t3; VU0 in h3 h8, out h10, break)
inline void kernel_sub_001DA9C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, uint64_t io_h3, uint64_t io_h8, uint64_t &io_h10, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t t8 = 0;
    uint64_t t9 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h10 = io_h10;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (t8 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t9 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 4))));
    (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 1024))));
    { const bool c = (int32_t)s0 <= 0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 16)))); if (c) goto L_1daa9c; }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 100)))); a3xw3 = true;
L_1da9f8:;
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
L_1daa00:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 1))));
    if ((int64_t)t3 == 0) { A6_BREAK(0x1DAA09u); goto L_1daa0c; }
L_1daa0c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t7))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 256))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t7))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 256))));
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 4294967295))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)t1) ? 1 : 0));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    { int32_t divisor = (int32_t)t3;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    if (RC_U64X(v0) == 0) do { const a4::q128 _v = (a4::mk128(h8, t0)); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    (v0 = (uint64_t)(t2 & (uint64_t)(uint16_t)256));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 256))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    (a1 = (uint64_t)(lo));
    { int32_t divisor = (int32_t)t3;    int32_t dividend = (int32_t)a0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t5, 0, bits); }
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4))));
    (a0 = (uint64_t)(lo));
    { uint32_t bits = (uint32_t)a0; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); } a3xw1 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t4, 0, bits); }
    { const bool c = (int32_t)t6 >= 0; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 4)))); if (c) { A3_BACK(); goto L_1daa00; } }
    (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 512))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 1024))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t9) < (int64_t)(int64_t)s0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 1024)))); if (c) { A3_BACK(); goto L_1da9f8; } }
L_1daa9c:;
    A1_W32B((uint32_t)t8, 16, (uint32_t)t2);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); xo[2] = t2; if (a3xw3) xo[3] = t3; } io_h10 = h10; return; }
    __builtin_unreachable();
}

// Sg2ExPcmInputGetRealPlayFrameCount (0x1dba10): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -, break)
inline uint64_t Sg2ExPcmInputGetRealPlayFrameCount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1dba30; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 2472))));
L_1dba30:;
    { return v0; }
    __builtin_unreachable();
}

// clipByMinMaxCube (0x1087300): typed by tools/typed_lift2.py (round A6; leaf; VU0 in vf0 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf20 vf21 vacc vclip, out vf1 vf2 vf4 vf5 vf6 vf7 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf20 vf21 vacc vclip)
inline uint32_t clipByMinMaxCube(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vacc, uint32_t &io_vclip)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vacc = io_vacc;
    uint32_t vclip = io_vclip;
    vf1 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 16)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 32)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf1 = a4v::blendv_ps(vf1, vf0, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf2 = a4v::blendv_ps(vf2, vf0, a4v::castsi128_ps(mask)); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a2, 48)));
    v0 = 0x3Fu;
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, vf1, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf11 = a4v::blendv_ps(vf11, vf1, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf11 = a4v::blendv_ps(vf11, vf2, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf2, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, vf2, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf10)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf10)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf10)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf12 = a4v::blendv_ps(vf12, vf1, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf12 = a4v::blendv_ps(vf12, vf2, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf13 = a4v::blendv_ps(vf13, vf1, a4v::castsi128_ps(mask)); }
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf11)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf13 = a4v::blendv_ps(vf13, vf2, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf12)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf12)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf12)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf14 = a4v::blendv_ps(vf14, vf1, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf13)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf13)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf13)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf2, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf14)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf15 = a4v::blendv_ps(vf15, vf1, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf15)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf16 = a4v::blendv_ps(vf16, vf1, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf16)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf2, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf17)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
    if (v0 == 0) goto L_108748c;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    v0 = v0 & v1;
L_108748c:;
    { io_vf1 = vf1; io_vf2 = vf2; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf20 = vf20; io_vf21 = vf21; io_vacc = vacc; io_vclip = vclip; return v0; }
    __builtin_unreachable();
}

// manager_sub_01099A08 (0x1099a08): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=a0 xo[2]=a1 xo[3]=a2 xo[4]=a3; VU0 in vf0 vf4 vf5 vf6 vf7 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf20 vf21 vacc vclip, out vf1 vf2 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf20 vf21 vacc vclip, frame 48 bytes, 0 slot accesses)
inline uint64_t manager_sub_01099A08(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, const a4::vec4 &io_vf4, const a4::vec4 &io_vf5, const a4::vec4 &io_vf6, const a4::vec4 &io_vf7, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vacc, uint32_t &io_vclip, uint64_t *xo)
{
    A14_REC(0);
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vacc = io_vacc;
    uint32_t vclip = io_vclip;
    bool a3xw0 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    if ((int64_t)a2 != 0) goto L_1099bc0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    vf1 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 63))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 0)))); a3xw2 = true;
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, vf1, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf11 = a4v::blendv_ps(vf11, vf1, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf11 = a4v::blendv_ps(vf11, vf2, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf2, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, vf2, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf10)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf10)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf10)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf12 = a4v::blendv_ps(vf12, vf1, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf12 = a4v::blendv_ps(vf12, vf2, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf13 = a4v::blendv_ps(vf13, vf1, a4v::castsi128_ps(mask)); }
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf11)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf13 = a4v::blendv_ps(vf13, vf2, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf12)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf12)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf12)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf14 = a4v::blendv_ps(vf14, vf1, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf13)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf13)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf13)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf2, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf14)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf15 = a4v::blendv_ps(vf15, vf1, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf15)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf16 = a4v::blendv_ps(vf16, vf1, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf16)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf2, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf20, vf20)) & 0xFFFFFF;
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf17)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf5, a4v::bcast<1>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf6, a4v::bcast<2>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf7, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf21 = a4v::blendv_ps(vf21, res, a4v::castsi128_ps(mask)); }
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    vclip = ((vclip << 6) | ps2VuClipFlags(vf21, vf21)) & 0xFFFFFF;
    (v1 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | a1_sx64(v1))); a3xw2 = true;
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_1099ba8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s0, 44, (uint32_t)v0);
    goto L_1099bc4;
L_1099ba8:;
    if ((int64_t)v0 != 0) { A1_W32B((uint32_t)s0, 44, 0u); goto L_1099bc4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    A1_W32B((uint32_t)s0, 44, (uint32_t)v0);
    goto L_1099bc4;
L_1099bc0:;
    A1_W32B((uint32_t)s0, 44, (uint32_t)a2);
L_1099bc4:;
    if ((int64_t)s2 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 32)))); goto L_1099be0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 44))));
    v1 = 1u;
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1099c34; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 32))));
L_1099be0:;
    s3 = 0xFFFFFFFFu;
    { const bool c = (int64_t)v0 == (int64_t)(int32_t)s3; (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1099c08; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1)))); a3xw2 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 44)))); a3xw3 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0)))); a3xw2 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 4)))); a3xw2 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1)))); a3xw4 = true;
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a1)))); a3xw2 = true; A3_CHARGE(); uint64_t a7c[5] = {(uint64_t)a1_bits(f0), 0u, a1, a2, a3}; v0 = manager_sub_01099A08(ram, rt, a0, a1, a2, a3, f0, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf2, vf4, vf5, vf6, vf7, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf20, vf21, vacc, vclip, a7c); f0 = a1_flt((uint32_t)a7c[0]); a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; a3xw0 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
L_1099c08:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 36))));
    { const bool c = (int64_t)v0 == (int64_t)(int32_t)s3; (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1099c30; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1)))); a3xw2 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0)))); a3xw2 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 44)))); a3xw3 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 4)))); a3xw2 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1)))); a3xw4 = true;
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); a3xw2 = true; A3_CHARGE(); uint64_t a7c[5] = {(uint64_t)a1_bits(f0), 0u, a1, a2, a3}; v0 = manager_sub_01099A08(ram, rt, a0, a1, a2, a3, f0, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf2, vf4, vf5, vf6, vf7, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf20, vf21, vacc, vclip, a7c); f0 = a1_flt((uint32_t)a7c[0]); a0 = a7c[1]; a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; a3xw0 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
L_1099c30:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1099c34:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a0; if (a3xw2) xo[2] = a1; if (a3xw3) xo[3] = a2; if (a3xw4) xo[4] = a3; } io_vf1 = vf1; io_vf2 = vf2; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf20 = vf20; io_vf21 = vf21; io_vacc = vacc; io_vclip = vclip; return v0; }
    __builtin_unreachable();
}

// manager_sub_010A1F10 (0x10a1f10): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0; VU0 in h7, out h6, break)
inline a1::RetVF manager_sub_010A1F10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h6, uint64_t io_h7, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    (v1 = (uint64_t)(~(0ULL | a2)));
    if (v1 == 0) do { const a4::q128 _v = (a4::mk128(h7, a3)); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f3, &bits, sizeof(bits)); }
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a2;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    if ((int64_t)v0 == 0) { A6_BREAK(0x10A1F35u); goto L_10a1f38; }
L_10a1f38:;
    { uint32_t bits = A1_R32B((uint32_t)a1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294951256))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    f0 = a1f::mul(f0, f1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294951272))));
    (a3 = (uint64_t)(hi));
    (a2 = (uint64_t)(lo));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v1))));
    { uint32_t bits = A1_R32B((uint32_t)t0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 60); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f1 = a1f::mul(f1, f0);
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 8, bits); }
    f1 = a1f::mul(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 12, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 60); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f1 = a1f::mul(f1, f0);
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 20, bits); }
    f1 = a1f::mul(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 16, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 24, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 60); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f1 = a1f::mul(f1, f0);
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 32, bits); }
    f1 = a1f::mul(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 28, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 36, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 60); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f2);
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 40, bits); }
    { { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 44, bits); } if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; } io_h6 = h6; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// objectmanager_mallocerrorfunc (0x11023f0): typed by tools/typed_lift2.py (round A6; loop leaf; VU0 in -, out -, break)
inline uint32_t objectmanager_mallocerrorfunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x1190000u;
    v1 = A1_R32(v0 - 0x3430u);
    { const bool c = v1 == 0; v0 = 0x1190000u; if (c) goto L_1102430; }
    a0 = 0xDu;
    v1 = v0 - 0x3468u;
    v0 = A1_R32(v1);
L_1102410:;
    v1 = v1 + 4u;
    a0 = a0 - 1u;
    if (v0 == 0) { A6_BREAK(0x110241Du); goto L_1102420; }
L_1102420:;
    if ((int32_t)a0 >= 0) { v0 = A1_R32(v1); { A3_BACK(); goto L_1102410; } }
L_1102430:;
    { return v0; }
    __builtin_unreachable();
}

// manager_sub_0110F0D0 (0x110f0d0): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a1 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in -, out -)
inline uint64_t manager_sub_0110F0D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t t4 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw5 = false;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 4))));
    (v0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1023));
    { const bool c = (int64_t)v0 == 0; (t2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_110f1c4; }
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t0, 4)))); a3xw5 = true;
    (t4 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)a1_sx64(t3) + (uint64_t)0ULL)); a3xw1 = true;
L_110f0f8:;
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 16))));
    (v1 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)2));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)256));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 4)))); if (c) goto L_110f178; }
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1023));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true; if (c) goto L_110f1c4; }
    a1 = t3; a3xw0 = true;
    (t1 = (uint64_t)(t4 & (uint64_t)(uint16_t)1023));
    (a0 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL));
L_110f128:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 28))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)2));
    { const bool c = (int64_t)v0 == 0; a2 = a1; if (c) goto L_110f158; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 56))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 11))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 22))));
    if ((int64_t)v1 == (int64_t)v0) goto L_110f170;
L_110f158:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); a3xw1 = true;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a1 = a2 + 0x1Cu; a3xw0 = true; if (c) { A3_BACK(); goto L_110f128; } }
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = a1_sx64(t3); } return v0; }
L_110f170:;
    { (v0 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL)); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = a1_sx64(t3); } return v0; }
L_110f178:;
    { const bool c = (int64_t)v1 == 0; (a0 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL)); if (c) goto L_110f1ac; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 56))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 20))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 11))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 22))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (a2 = (uint32_t)((int32_t)(SRA32((int32_t)v1, 22)))); if (c) goto L_110f1a8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 24))));
    { const bool c = (int64_t)v1 == (int64_t)(int32_t)a2; (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (c) goto L_110f1c8; }
L_110f1a8:;
    (a0 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL));
L_110f1ac:;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 4))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1023));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 28)))); a3xw1 = true; if (c) { A3_BACK(); goto L_110f0f8; } }
L_110f1c4:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_110f1c8:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a1); if (a3xw1) xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; if (a3xw5) xo[5] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// ClipPoint (0x11333b8): typed by tools/typed_lift2.py (round A6; leaf; VU0 in vf0 vf26 vacc vclip, out vf27 vf28 vf29 vf30 vf31 vacc vclip)
inline uint32_t ClipPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, const a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint32_t &io_vclip)
{
    uint32_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint32_t vclip = io_vclip;
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(a1, 32)));
    vf31 = a4v::castsi128_ps(A4_R128(ADD32(a1, 48)));
    { a4::vec4 res = a4v::mul(vf28, a4v::bcast<0>(vf27)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf29, a4v::bcast<1>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf30, a4v::bcast<2>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf31, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf27, a4v::bcast<0>(vf26)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    vclip = ((vclip << 6) | ps2VuClipFlags(vf27, vf27)) & 0xFFFFFF;
    (v0 = (uint32_t)((int32_t)(vclip & 0x00FFFFFFu)));
    { v0 = v0 & 0x3Fu; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vclip = vclip; return v0; }
    __builtin_unreachable();
}

// mcGetLastSaveGameDataIndex (0x114a280): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1; VU0 in -, out -)
inline uint64_t mcGetLastSaveGameDataIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294956572))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_114a2d8; }
    (t1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); a3xw5 = true;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw2 = true;
    t0 = 1u; a3xw4 = true;
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a3, 216)))); a3xw1 = true;
L_114a2a8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    if ((int64_t)v0 != (int64_t)(int32_t)t0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); a3xw2 = true; goto L_114a2cc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (a0 = (uint64_t)(A1_R64B(a1, 64))); a3xw0 = true;
    (v1 = (uint64_t)(A1_R64B((uint32_t)v0, 0)));
    if ((int64_t)v1 == (int64_t)a0) goto L_114a2f0;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); a3xw2 = true;
L_114a2cc:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)10) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a1 = a1 + 0x98u; a3xw1 = true; if (c) { A3_BACK(); goto L_114a2a8; } }
L_114a2d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 76))));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_114a2f8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 80))));
    goto L_114a2f8;
L_114a2f0:;
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); xo[2] = a2; xo[3] = a3; xo[4] = a1_sx64(t0); xo[5] = t1; } return v0; }
L_114a2f8:;
    { (v0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a0; if (a3xw1) xo[1] = a1_sx64(a1); if (a3xw2) xo[2] = a2; xo[3] = a3; if (a3xw4) xo[4] = a1_sx64(t0); if (a3xw5) xo[5] = t1; } return v0; }
    __builtin_unreachable();
}

// sheetFsGetStageTexSegIndex (0x115ba60): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3; VU0 in -, out -, break)
inline uint64_t sheetFsGetStageTexSegIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 5))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10))));
    { int32_t divisor = (int32_t)a2;    int32_t dividend = (int32_t)a0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    if ((int64_t)a2 == 0) { A6_BREAK(0x115BA79u); goto L_115ba7c; }
L_115ba7c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, -5120))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (a3 = (uint64_t)(hi));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8)))); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// getStageAreaIndexByPos (0x1358958): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -, break)
inline a1::RetVF getStageAreaIndexByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 24192))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = (-(float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)a3, 84); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    f1 = (-(float)(f1));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 56))));
    { uint32_t bits = A1_R32B((uint32_t)a3, 80); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f0 = a1f::sub(f0, f2);
    if ((int64_t)a0 == 0) { A6_BREAK(0x1358989u); goto L_135898c; }
L_135898c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 60))));
    f1 = a1f::sub(f1, f3);
    { uint32_t bits = A1_R32B((uint32_t)a3, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 52))));
    f0 = a1f::div(fcr31, f0, f4);
    f1 = a1f::div(fcr31, f1, f5);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f2, sizeof(bits)); (a2 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a2;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v0 = (uint64_t)(lo));
    { int32_t divisor = (int32_t)a0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    (a0 = (uint64_t)(lo));
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const bool c = (int32_t)a1 < 0; (t0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL)); if (c) goto L_13589fc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1358a00;
L_13589fc:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1358a00:;
    { const bool c = (int32_t)t0 < 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 24192)))); if (c) goto L_1358a1c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 52))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(t0)) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1358a20;
L_1358a1c:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1358a20:;
    { const bool c = (int32_t)a1 < 0; (a0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL)); if (c) goto L_1358a58; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 24192))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 72))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); goto L_1358a5c; }
    if ((int32_t)a0 < 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); goto L_1358a5c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 76))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)a1)))); if (c) goto L_1358a5c; }
L_1358a58:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1358a5c:;
    { (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SeamlessStageLoadComplete (0x135b578): typed by tools/typed_lift2.py (round A6; loop frame leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out -, frame 32 bytes, 0 slot accesses)
inline uint32_t SeamlessStageLoadComplete(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t t8 = 0;
    uint32_t t9 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    a0 = 0x1460000u;
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, 24192))));
    v1 = A1_R32B(a0, 8);
    { const bool c = v1 == 0; v0 = 1u; if (c) goto L_135b6f8; }
    v0 = A1_R32B(a0, 76);
    { const bool c = (int32_t)v0 <= 0; (t3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw7 = true; if (c) goto L_135b6f4; }
    s1 = A1_R32B(a0, 72);
    (s0 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1_sx64(a0) + (uint64_t)0ULL));
L_135b5b8:;
    { const bool c = (int32_t)s1 <= 0; t0 = 0u; a3xw4 = true; if (c) goto L_135b6e4; }
    t8 = 0x1460000u;
    (t7 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 72))));
    t9 = t8;
L_135b5d0:;
    { const bool c = (int32_t)t0 < 0; a1 = 0xFFFFFFFFu; a3xw1 = true; if (c) goto L_135b60c; }
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(t8, 24192))));
    v1 = A1_R32B(a0, 72);
    v0 = (int32_t)t0 < (int32_t)v1 ? 1u : 0u;
    { const bool c = v0 == 0; v0 = a1 << 1; if (c) goto L_135b610; }
    { const bool c = (int32_t)t3 < 0; a2 = 0u; a3xw2 = true; if (c) goto L_135b614; }
    v0 = A1_R32B(a0, 76);
    { int64_t result = (int64_t)(int32_t)t3 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    (v0 = (uint32_t)(((int64_t)(int64_t)t3 < (int64_t)(int64_t)a1_sx64(v0)) ? 1 : 0));
    { const bool c = v0 != 0; a1 = a0 + t0; a3xw1 = true; if (c) goto L_135b60c; }
    a1 = 0xFFFFFFFFu; a3xw1 = true;
L_135b60c:;
    v0 = a1 << 1;
L_135b610:;
    a2 = 0u; a3xw2 = true;
L_135b614:;
    v0 = v0 + a1;
    t2 = 0xFFFFFFFFu; a3xw6 = true;
    a3 = v0 << 3; a3xw3 = true;
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32(t9, 24192)))); a3xw5 = true;
    t6 = 1u;
    t5 = 2u;
    t4 = 4u;
L_135b630:;
    { const bool c = a1 == t2; a0 = a2 << 2; if (c) goto L_135b650; }
    v0 = A1_R32B(t1, 8);
    v0 = a3 + v0;
    v0 = v0 + a0;
    v1 = A1_R32B(v0, 8);
    v1 = v1 & 0xFu;
    goto L_135b654;
L_135b650:;
    v1 = 7u;
L_135b654:;
    { const bool c = v1 == t6; v0 = 0u; if (c) goto L_135b6f8; }
    { const bool c = a1 == t2; a0 = a2 << 2; if (c) goto L_135b680; }
    v0 = A1_R32B(t1, 8);
    v0 = a3 + v0;
    v0 = v0 + a0;
    v1 = A1_R32B(v0, 8);
    v1 = v1 & 0xFu;
    goto L_135b684;
L_135b680:;
    v1 = 7u;
L_135b684:;
    { const bool c = v1 == t5; v0 = 0u; if (c) goto L_135b6f8; }
    { const bool c = a1 == t2; a0 = a2 << 2; if (c) goto L_135b6b0; }
    v0 = A1_R32B(t1, 8);
    v0 = a3 + v0;
    v0 = v0 + a0;
    v1 = A1_R32B(v0, 8);
    v1 = v1 & 0xFu;
    goto L_135b6b4;
L_135b6b0:;
    v1 = 7u;
L_135b6b4:;
    if (v1 != t4) { a2 = a2 + 1u; a3xw2 = true; goto L_135b6c8; }
    v0 = 0u;
    goto L_135b6f8;
L_135b6c8:;
    v0 = (int32_t)a2 < 2 ? 1u : 0u;
    if (v0 != 0) { A3_BACK(); goto L_135b630; }
    t0 = t0 + 1u; a3xw4 = true;
    v0 = (int32_t)t0 < (int32_t)t7 ? 1u : 0u;
    if (v0 != 0) { A3_BACK(); goto L_135b5d0; }
L_135b6e4:;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1)))); a3xw7 = true;
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t3) < (int64_t)(int64_t)s0) ? 1 : 0));
    if (v0 != 0) { A3_BACK(); goto L_135b5b8; }
L_135b6f4:;
    v0 = 1u;
L_135b6f8:;
    { if (xo) { xo[0] = a1_sx64(a0); if (a3xw1) xo[1] = a1_sx64(a1); if (a3xw2) xo[2] = a1_sx64(a2); if (a3xw3) xo[3] = a1_sx64(a3); if (a3xw4) xo[4] = a1_sx64(t0); if (a3xw5) xo[5] = a1_sx64(t1); if (a3xw6) xo[6] = a1_sx64(t2); if (a3xw7) xo[7] = t3; } return v0; }
    __builtin_unreachable();
}

// getStageAreaID (0x135d760): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -, break)
inline a1::RetVF getStageAreaID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 24192))));
    (t1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    f0 = (-(float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)a3, 84); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    (t0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 56))));
    (t2 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::sub(f0, f2);
    if ((int64_t)a2 == 0) { A6_BREAK(0x135D795u); goto L_135d798; }
L_135d798:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 60))));
    f1 = (-(float)(f1));
    { uint32_t bits = A1_R32B((uint32_t)a3, 80); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    f0 = a1f::div(fcr31, f0, f4);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 52))));
    f1 = a1f::sub(f1, f3);
    f1 = a1f::div(fcr31, f1, f5);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f2, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)a0;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(lo));
    { int32_t divisor = (int32_t)a2;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v1))));
    (a0 = (uint64_t)(lo));
    { const bool c = (int32_t)a0 < 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295)))); if (c) goto L_135d808; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a0) ? 1 : 0));
    if ((int64_t)v0 == 0) { A1_W32B((uint32_t)t0, 0, (uint32_t)a0); goto L_135d810; }
L_135d808:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    A1_W32B((uint32_t)t0, 0, (uint32_t)v0);
L_135d810:;
    { const bool c = (int32_t)a1 < 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_135d82c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 52))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_135d838; }
L_135d82c:;
    { A1_W32B((uint32_t)t1, 0, (uint32_t)v0); return a1::RetVF{(uint64_t)v0, f0}; }
L_135d838:;
    { A1_W32B((uint32_t)t1, 0, (uint32_t)a1); return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// getStageAreaName (0x135d840): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out -, break)
inline a1::RetVF getStageAreaName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    { uint32_t bits = A1_R32B((uint32_t)a1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 24192))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = (-(float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)a3, 84); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    f1 = (-(float)(f1));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 56))));
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)a3, 80); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    (t1 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    f0 = a1f::sub(f0, f2);
    if ((int64_t)a1 == 0) { A6_BREAK(0x135D879u); goto L_135d87c; }
L_135d87c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 60))));
    f1 = a1f::sub(f1, f3);
    { uint32_t bits = A1_R32B((uint32_t)a3, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 52))));
    f0 = a1f::div(fcr31, f0, f4);
    f1 = a1f::div(fcr31, f1, f5);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f2, sizeof(bits)); (a2 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)a2;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(lo));
    { int32_t divisor = (int32_t)a1;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 63))));
    (a1 = (uint64_t)(lo));
    { const bool c = (int32_t)a1 < 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 63)))); if (c) goto L_135d8ec; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 65)))); if (c) goto L_135d8f8; }
L_135d8ec:;
    A1_W8B((uint32_t)t0, 0, (uint8_t)(uint32_t)v1);
    goto L_135d8fc;
L_135d8f8:;
    A1_W8B((uint32_t)t0, 0, (uint8_t)(uint32_t)v0);
L_135d8fc:;
    if ((int32_t)a2 < 0) { A1_W8B((uint32_t)t0, 1, (uint8_t)(uint32_t)a0); goto L_135d924; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 52))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a2) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 48)))); if (c) goto L_135d920; }
    A1_W8B((uint32_t)t0, 1, (uint8_t)(uint32_t)a0);
    goto L_135d924;
L_135d920:;
    A1_W8B((uint32_t)t0, 1, (uint8_t)(uint32_t)v0);
L_135d924:;
    { A1_W8B((uint32_t)t0, 2, (uint8_t)0u); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// getStageAreaNameUV (0x135d930): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out hi1 lo1, break)
inline uint64_t getStageAreaNameUV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 24192))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 56))));
    (t1 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    if ((int64_t)v0 == 0) { A6_BREAK(0x135D945u); goto L_135d948; }
L_135d948:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 60))));
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int32_t divisor = (int32_t)v1; int32_t dividend = (int32_t)a2; if (divisor != 0) {     if (divisor == -1 && dividend == INT32_MIN) {         lo1 = (uint64_t)(int64_t)INT32_MIN; hi1 = 0;     } else {         lo1 = (uint64_t)(int64_t)(dividend / divisor);         hi1 = (uint64_t)(int64_t)(dividend % divisor);     } } else {     lo1 = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi1 = (uint64_t)(int64_t)dividend; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 52))));
    (a1 = (uint64_t)(lo));
    (a2 = (uint64_t)(lo1));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 63))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    { const bool c = (int32_t)a1 < 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295)))); if (c) goto L_135d984; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 65)))); if (c) goto L_135d990; }
L_135d984:;
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)t0);
    goto L_135d994;
L_135d990:;
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)v0);
L_135d994:;
    { const bool c = (int32_t)v1 < 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 63)))); if (c) goto L_135d9b4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 52))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)v1) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 48)))); goto L_135d9b4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 63))));
L_135d9b4:;
    A1_W8B((uint32_t)a0, 1, (uint8_t)(uint32_t)v0);
    { A1_W8B((uint32_t)a0, 2, (uint8_t)0u); io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// stagemanager_getCenterFromAreaId (0x13667b8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2 xo[1]=a3; VU0 in -, out -, break)
inline a1::RetVF stagemanager_getCenterFromAreaId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f5, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 25512))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f6, &bits, sizeof(bits)); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 52))));
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    if ((int64_t)v0 == 0) { A6_BREAK(0x13667E9u); goto L_13667ec; }
L_13667ec:;
    { uint32_t bits = A1_R32B((uint32_t)a2, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    A1_W32B((uint32_t)a0, 4, 0u);
    f2 = a1f::mul(f2, f5);
    f4 = a1f::mul(f3, f5);
    (a3 = (uint64_t)(hi));
    (a1 = (uint64_t)(lo));
    { uint32_t bits = (uint32_t)a3; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f0 = a1f::sub(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::addFinite(f0, f4);
    f0 = (-(float)(f0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { float f = f6; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 12, bits); }
    f3 = a1f::mul(f2, f5);
    f0 = a1f::mul(f0, f5);
    f1 = a1f::sub(f1, f0);
    f1 = a1f::mul(f1, f2);
    f1 = a1f::addFinite(f1, f3);
    { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 8, bits); } if (xo) { xo[0] = a2; xo[1] = a3; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// time_attack_init_gstat_adrs (0x1367d18): typed by tools/typed_lift2.py (round A6; loop leaf; VU0 in h3, out h8)
inline uint32_t time_attack_init_gstat_adrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t io_h3, uint64_t &io_h8)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    { const uint32_t _dynInsn = A1_R32(0x1367D18u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1367D1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1367D20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x1367D24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    v0 = A1_R32(a0);
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v0 ^ v1;
    if (a1_sx64(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, a1_sx64(v1))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t0, 12))));
    { const bool c = (int32_t)a3 <= 0; (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t0, 16)))); if (c) goto L_1367e24; }
    t2 = 0x30000u;
    (t1 = (uint64_t)((uint64_t)a1_sx64(a0) + (uint64_t)0ULL));
    t2 = t2 | 0xFFFFu;
    { const uint32_t _dynInsn = A1_R32(0x1367D54u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t4 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_1367d58:;
    v1 = A1_R32(a2);
    v0 = v1 & t2;
    { const bool c = v0 == 0; v0 = a2 - v0; if (c) goto L_1367d70; }
    a0 = A1_R32(v0);
    goto L_1367d74;
L_1367d70:;
    a0 = 0u;
L_1367d74:;
    { const bool c = a0 == 0; a1 = 0u; if (c) goto L_1367d8c; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { const bool c = a0 != v0; a1 = a0; if (c) goto L_1367d90; }
    a1 = 0u;
L_1367d8c:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
L_1367d90:;
    { const uint32_t _dynInsn = A1_R32(0x1367D90u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)ADD32(t4, _dynSimm)))); }
    if (v0 == a0) goto L_1367db0;
    { const bool c = a1 == a0; v0 = v1 & t2; if (c) goto L_1367dbc; }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1))));
    goto L_1367e18;
L_1367db0:;
    if (a1 != 0) { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1)))); goto L_1367e18; }
    v0 = v1 & t2;
L_1367dbc:;
    { const bool c = v0 == 0; v0 = a2 - v0; if (c) goto L_1367dd0; }
    a0 = A1_R32(v0);
    goto L_1367dd4;
L_1367dd0:;
    a0 = 0u;
L_1367dd4:;
    { const bool c = a0 == 0; v0 = 0u; if (c) goto L_1367e0c; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { const bool c = a0 == v0; v1 = v1 >> 18; if (c) goto L_1367e08; }
    a1 = A1_R32B(a0, 8);
    v1 = v1 & 0xFFFu;
    a0 = A1_R32B(a0, 16);
    v0 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = a0 + v1;
    goto L_1367e0c;
L_1367e08:;
    v0 = 0u;
L_1367e0c:;
    A1_W32B(v0, 16, a2);
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t0, 12))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1))));
L_1367e18:;
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t3) < (int64_t)(int64_t)a1_sx64(a3)) ? 1 : 0));
    { const bool c = v0 != 0; a2 = a2 + 0x14u; if (c) { A3_BACK(); goto L_1367d58; } }
L_1367e24:;
    { io_h8 = h8; return v0; }
    __builtin_unreachable();
}

// YattsukeGetSeamlessAreaCenter (0x13d0ac8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=f2 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0; VU0 in -, out -, break)
inline a1::RetVF YattsukeGetSeamlessAreaCenter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    if ((int64_t)v0 == 0) { A6_BREAK(0x13D0ADDu); goto L_13d0ae0; }
L_13d0ae0:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10))));
    A1_W32B((uint32_t)a0, 4, 0u);
    (v1 = (uint64_t)(hi));
    (a1 = (uint64_t)(lo));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294964596))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294964596))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 12, bits); }
    { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 8, bits); } if (xo) { xo[0] = a1_bits(f2); xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// sub_001091C0 (0x1091c0): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t sub_001091C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 8))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)a0 <= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); if (c) goto L_109248; }
    a1 = 1u;
L_1091f0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_10923c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)5));
    A1_W32B((uint32_t)s0, 16, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    { const bool c = (int64_t)v1 != (int64_t)(int32_t)a1; A1_W32B((uint32_t)s1, 0, (uint32_t)v1); if (c) goto L_109228; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s1, 0, (uint32_t)v0);
L_109228:;
    A1_W32B((uint32_t)s0, 20, (uint32_t)s0);
    { A1_W32B((uint32_t)s0, 24, (uint32_t)v1); A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    goto L_109254;
L_10923c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int64_t)a0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 64)))); if (c) { A3_BACK(); goto L_1091f0; } }
L_109248:;
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_109254:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// sub_001092B8 (0x1092b8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out -, 64-bit inputs a0, frame 16 bytes, 0 slot accesses)
inline uint64_t sub_001092B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = (uint32_t)w64_a0;
    if ((int32_t)a1 < 0) goto L_1092d4;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 32))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    if ((int64_t)v0 != 0) { v1 = A1_R32B(a0, 28); goto L_1092e4; }
L_1092d4:;
    { A3_CHARGE(); v0 = sub_00109288(ram, rt, w64_a0); }
    goto L_1092f0;
L_1092e4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
L_1092f0:;
    { return v0; }
    __builtin_unreachable();
}

// GetTimerSystemTime (0x110cc8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in c0st, out c0st h9)
inline uint64_t GetTimerSystemTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t sp, uint32_t &io_c0st, uint64_t &io_h9)
{
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t s0 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h9 = io_h9;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    { A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)4096));
    (s0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)v1, 0)));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (t2 = (uint64_t)(A1_R64B((uint32_t)a0, 14616)));
    (t1 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)2048));
    { const bool c = (int64_t)t1 == 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_110d08; }
    (s0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)v1, 0)));
    (t2 = (uint64_t)((int64_t)(int64_t)t2 + (int64_t)(int32_t)1));
L_110d08:;
    (t0 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)3));
    (t2 = (uint64_t)(t2 << 16));
    if (RC_U64X(t0) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    (s0 = (uint64_t)(s0 | t2));
    (t1 = (uint64_t)(t1 << ((uint32_t)t0 & 0x3F)));
    (s0 = (uint64_t)(s0 << ((uint32_t)t1 & 0x3F)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_110d34; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_110d34:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_c0st = c0st; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// AllocTimerCounter (0x110db8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t AllocTimerCounter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t &io_c0st)
{
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    { A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(iAllocTimerCounter(ram, rt)); }
    { const bool c = (int64_t)s0 == 0; (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_110de8; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_110de8:;
    (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// GetTimerUsedUnusedCounters (0x110f40): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 64 bytes, 0 slot accesses)
inline uint64_t GetTimerUsedUnusedCounters(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(iGetTimerUsedUnusedCounters(ram, rt, a0, a1)); }
    { const bool c = (int64_t)s2 == 0; (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_110f80; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_110f80:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// SetTimerCount (0x1111b8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in c0st, out c0st)
inline uint64_t SetTimerCount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t c0st = io_c0st;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 10))));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6)))); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    { const bool c = (int32_t)s1 < 0; (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_11120c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (v0 = (uint64_t)(s1 & (uint64_t)(uint16_t)1023));
    { const bool c = (int64_t)v0 != (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16)))); if (c) goto L_11120c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 14616))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 28))));
    if ((int64_t)v1 != (int64_t)s1) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12)))); goto L_111224; }
L_11120c:;
    { const bool c = (int64_t)s3 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_111268; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    goto L_111268;
L_111224:;
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)(A1_R64B((uint32_t)s0, 24))); if (c) goto L_111250; }
    { A3_CHARGE(); v0 = iGetTimerSystemTime(ram, rt); }
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 16)));
    A1_W64B((uint32_t)s0, 24, s2);
    (v1 = (uint64_t)(v0 - v1));
    A1_W64B((uint32_t)s0, 16, v0);
    (s1 = (uint64_t)((uint64_t)s1 + (uint64_t)v1));
    goto L_111254;
L_111250:;
    A1_W64B((uint32_t)s0, 24, s2);
L_111254:;
    { const bool c = (int64_t)s3 == 0; (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_111268; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_111268:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// GetTimerBaseTime (0x1112d0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t GetTimerBaseTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st)
{
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = iGetTimerBaseTime(ram, rt, a0); }
    { const bool c = (int64_t)s1 == 0; (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_111304; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_111304:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// TimerBusClock2USec (0x111558): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in h3, out h4 h6)
inline uint64_t TimerBusClock2USec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t t6, uint64_t sp, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h6)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)2250 << 16)))); A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; v0 = ps2___udivdi3(ram, rt, a0, a1, t6, h3, h4, h6, a7c); t6 = a7c[5]; }
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    if ((int64_t)s0 != 0) { A1_W32B((uint32_t)s0, 0, (uint32_t)v0); goto L_111590; }
L_111590:;
    { const bool c = (int64_t)s2 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)2250 << 16)))); if (c) goto L_1115e8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)2250 << 16))));
    { uint64_t result = (uint64_t)(uint32_t)v0 * (uint64_t)(uint32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    (a2 = (uint64_t)(lo));
    (v1 = (uint64_t)(hi));
    (a2 = (uint64_t)(a2 << (32 + 0)));
    (v1 = (uint64_t)(v1 << (32 + 0)));
    (a2 = (uint64_t)(a2 >> (32 + 0)));
    (v1 = (uint64_t)(v1 | a2));
    (v1 = (uint64_t)(s1 - v1));
    (v0 = (uint64_t)(v1 << 5));
    (v0 = (uint64_t)(v0 - v1));
    (a0 = (uint64_t)(v0 << 6));
    (a0 = (uint64_t)(a0 - v0));
    (a0 = (uint64_t)(a0 << 3));
    (a0 = (uint64_t)((uint64_t)a0 + (uint64_t)v1));
    { (a0 = (uint64_t)(a0 << 6)); A3_CHARGE(); v0 = ps2___udivdi3(ram, rt, a0, a1, t6, h3, h4, h6); }
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    A1_W32B((uint32_t)s2, 0, (uint32_t)v0);
L_1115e8:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_h4 = h4; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// TimerUSec2BusClock (0x111600): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in h3, out hi1 lo1 h4 h6)
inline uint64_t TimerUSec2BusClock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t6, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t io_h3, uint64_t &io_h4, uint64_t &io_h6, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t a2 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)2250 << 16))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { uint64_t result = (uint64_t)(uint32_t)s0 * (uint64_t)(uint32_t)v0; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    { uint64_t result = (uint64_t)(uint32_t)a0 * (uint64_t)(uint32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)16960));
    (s0 = (uint64_t)(hi1));
    (a2 = (uint64_t)(lo));
    (s0 = (uint64_t)(s0 << (32 + 0)));
    (a2 = (uint64_t)(a2 << (32 + 0)));
    (a0 = (uint64_t)(hi));
    (a2 = (uint64_t)(a2 >> (32 + 0)));
    (a0 = (uint64_t)(a0 << (32 + 0)));
    (a0 = (uint64_t)(a0 | a2));
    (a2 = (uint64_t)(lo1));
    (a2 = (uint64_t)(a2 << (32 + 0)));
    (a2 = (uint64_t)(a2 >> (32 + 0)));
    { (s0 = (uint64_t)(s0 | a2)); A3_CHARGE(); uint64_t a7c[6] = {a3, 0u, 0u, 0u, 0u, 0u}; v0 = ps2___udivdi3(ram, rt, a0, a1, t6, h3, h4, h6, a7c); a3 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; t2 = a7c[3]; t3 = a7c[4]; }
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; } io_hi1 = hi1; io_lo1 = lo1; io_h4 = h4; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// sceSifSetRebootNotifyBuffer (0x111b18): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st h4, out c0st h2, frame 80 bytes, 0 slot accesses)
inline uint64_t sceSifSetRebootNotifyBuffer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t io_h4)
{
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    v1 = 0x130000u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 14648))));
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)a1, 14652, (uint32_t)s1);
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    A1_W32B(v1, 14648, (uint32_t)s0);
    (a2 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = a6x::memset(ram, rt, a0, (uint32_t)a1, a2, h2, h4); }
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_111b78; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
L_111b78:;
    { io_c0st = c0st; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// sub_001199F8 (0x1199f8): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a1; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t sub_001199F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t &io_c0st, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    bool a3xw0 = false;
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 8))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)a0 <= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); if (c) goto L_119a80; }
    a1 = 1u; a3xw0 = true;
L_119a28:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_119a74; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)5));
    A1_W32B((uint32_t)s0, 16, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    { const bool c = (int64_t)v1 != (int64_t)(int32_t)a1; A1_W32B((uint32_t)s1, 0, (uint32_t)v1); if (c) goto L_119a60; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s1, 0, (uint32_t)v0);
L_119a60:;
    A1_W32B((uint32_t)s0, 20, (uint32_t)s0);
    { A1_W32B((uint32_t)s0, 24, (uint32_t)v1); A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    goto L_119a8c;
L_119a74:;
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int64_t)a0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 64)))); if (c) { A3_BACK(); goto L_119a28; } }
L_119a80:;
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_119a8c:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a1); } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// _sceMcpIsSpeckOutClust (0x120d58): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out -, break)
inline uint64_t _sceMcpIsSpeckOutClust(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 388))));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22072))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (a2 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)v1, 42)));
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)v1, 44)));
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    if ((int64_t)v0 == 0) { A6_BREAK(0x120D85u); goto L_120d88; }
L_120d88:;
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { (a1 = (uint64_t)(lo)); A3_CHARGE(); v0 = a1_sx64(_sceMcpIsSpeckOutBlock(ram, rt, (uint32_t)a0, a1)); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); return v0; }
    __builtin_unreachable();
}

// sub_0012C240 (0x12c240): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in h6, out h2, break)
inline uint64_t sub_0012C240(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t0, uint64_t sp, uint64_t &io_h2, uint64_t io_h6, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (t6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t6, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 16))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int64_t)RC_U64X(s0)) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_12c444; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 20)))); a3xw4 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 20))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)v0))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0)))); a3xw2 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (s6 = (uint64_t)((uint64_t)RC_U64X(t3) + (uint64_t)0ULL));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (t2 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); a3xw3 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    { uint32_t divisor = (uint32_t)v1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)t5 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)t5 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)t5; } }
    if ((int64_t)v1 == 0) { A6_BREAK(0x12C2B5u); goto L_12c2b8; }
L_12c2b8:;
    (s5 = (uint64_t)(lo));
    (s2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { const bool c = (int64_t)s2 == 0; (t4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_12c370; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_12c2d0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4)))); a3xw4 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    (a3 = (uint64_t)(((uint64_t)s3 < (uint64_t)RC_U64X(t3)) ? 1 : 0)); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 16))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)s2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)s2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v1))));
    (v1 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)65535));
    (a1 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0)))); a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 16))));
    (v0 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)65535));
    (t4 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0))));
    A1_W16B((uint32_t)t2, 0, (uint16_t)(uint32_t)v1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, 16))));
    A1_W16B((uint32_t)t2, 2, (uint16_t)(uint32_t)a1);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4)))); a3xw3 = true;
    { const bool c = (int64_t)a3 == 0; (t4 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 16)))); if (c) { A3_BACK(); goto L_12c2d0; } }
    if ((int64_t)t5 != 0) { (a1 = (uint64_t)((uint64_t)t6 + (uint64_t)0ULL)); goto L_12c374; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967292)))); a3xw2 = true;
    goto L_12c34c;
L_12c348:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295))));
L_12c34c:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)t1) ? 1 : 0));
    if ((int64_t)v0 == 0) { A1_W32B((uint32_t)s4, 16, (uint32_t)s0); goto L_12c370; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    if ((int64_t)v0 == 0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967292)))); a3xw2 = true; { A3_BACK(); goto L_12c348; } }
    A1_W32B((uint32_t)s4, 16, (uint32_t)s0);
L_12c370:;
    (a1 = (uint64_t)((uint64_t)t6 + (uint64_t)0ULL));
L_12c374:;
    { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {a3, t0, t1, t2}; v0 = a1_sx64(ps2___mcmp_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, h2, h6, a7c)); a3 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; t2 = a7c[3]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; }
    { const bool c = (int32_t)v0 < 0; (t3 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); a3xw4 = true; if (c) goto L_12c440; }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    (t4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw3 = true;
L_12c398:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4)))); a3xw4 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    (a3 = (uint64_t)(((uint64_t)s3 < (uint64_t)RC_U64X(t3)) ? 1 : 0)); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)65535));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (v0 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)65535));
    (t4 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    A1_W16B((uint32_t)t2, 0, (uint16_t)(uint32_t)v1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, 16))));
    A1_W16B((uint32_t)t2, 2, (uint16_t)(uint32_t)a1);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4)))); a3xw3 = true;
    { const bool c = (int64_t)a3 == 0; (t4 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 16)))); if (c) { A3_BACK(); goto L_12c398; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0)))); a3xw2 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_12c444; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967292)))); a3xw2 = true;
    goto L_12c41c;
L_12c418:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295))));
L_12c41c:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)t1) ? 1 : 0));
    if ((int64_t)v0 == 0) { A1_W32B((uint32_t)s4, 16, (uint32_t)s0); goto L_12c440; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    if ((int64_t)v0 == 0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967292)))); a3xw2 = true; { A3_BACK(); goto L_12c418; } }
    A1_W32B((uint32_t)s4, 16, (uint32_t)s0);
L_12c440:;
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_12c444:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a3; if (a3xw1) xo[1] = t0; if (a3xw2) xo[2] = t1; if (a3xw3) xo[3] = t2; if (a3xw4) xo[4] = t3; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosDI (0x1ad6d0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1; VU0 in c0st, out c0st h2)
inline uint64_t iosDI(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint32_t s0 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    sp = sp - 0x10u;
    { A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = DIntr(ram, rt, c0st, a7c); a0 = a7c[0]; }
    { const bool c = (int64_t)v0 == 0; s0 = 0x1E0000u; if (c) goto L_1ad710; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 3044))));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) goto L_1ad728; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 7112)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3, t0, t1, t2, t3, sp, h2)); }
    v1 = s0 + 0xBE4u;
    goto L_1ad72c;
L_1ad710:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 3044))));
    { const bool c = (int32_t)v0 > 0; (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) goto L_1ad728; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 7136)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3, t0, t1, t2, t3, sp, h2)); }
L_1ad728:;
    v1 = s0 + 0xBE4u;
L_1ad72c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W32B(v1, 0, (uint32_t)v0);
    { sp = sp + 0x10u; if (xo) { xo[0] = a0; xo[1] = a1; } io_c0st = c0st; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosEI (0x1ad748): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0; VU0 in c0st, out c0st h2)
inline uint64_t iosEI(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t s0 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    sp = sp - 0x10u;
    s0 = 0x1E0000u;
    v1 = s0 + 0xBE4u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { const bool c = (int64_t)v0 != 0; A1_W32B(v1, 0, (uint32_t)v0); if (c) goto L_1ad790; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    a0 = 0x1E0000u; a3xw0 = true;
    { const bool c = (int64_t)v0 == 0; a0 = a0 + 0x1BF8u; a3xw0 = true; if (c) goto L_1ad7a4; }
    { A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, h2)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 3044))));
    goto L_1ad7a8;
L_1ad790:;
    if ((int32_t)v0 >= 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 3044)))); goto L_1ad7a8; }
    a0 = 0x1E0000u; a3xw0 = true;
    { a0 = a0 + 0x1C28u; a3xw0 = true; A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, h2)); }
L_1ad7a4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 3044))));
L_1ad7a8:;
    { sp = sp + 0x10u; if (xo) { if (a3xw0) xo[0] = a1_sx64(a0); } io_c0st = c0st; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001B7590 (0x1b7590): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in h4, out h2)
inline uint64_t kernel_sub_001B7590(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t t8 = 0;
    uint64_t t9 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 28))));
    { const bool c = (int64_t)s0 == 0; (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_1b7770; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 64)))); a3xw1 = true;
    { const bool c = (int32_t)a3 <= 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw2 = true; if (c) goto L_1b7770; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 48))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
L_1b75d0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v1)) ? 1 : 0)); if (c) goto L_1b7760; }
    if ((int64_t)v0 != 0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); a3xw2 = true; goto L_1b7764; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    if ((int64_t)v0 == 0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); a3xw2 = true; goto L_1b7764; }
    if ((int64_t)a0 == 0) goto L_1b7608;
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 56)))); A3_CHARGE(); v0 = a6x::strcpy(ram, rt, a0, (uint32_t)a1, h2, h4); }
L_1b7608:;
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1b7774; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 56))));
    { const bool c = (int32_t)v0 <= 0; (t6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1b76cc; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 60)))); a3xw3 = true;
    (t8 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    (t5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t7 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
L_1b7638:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    if ((int64_t)v0 == (int64_t)s3) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 4)))); goto L_1b7650; }
    if ((int64_t)v0 != (int64_t)t9) { (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 1)))); goto L_1b76bc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 4))));
L_1b7650:;
    { const bool c = (int64_t)v0 == 0; (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw4 = true; if (c) goto L_1b76b8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, (uint32_t)t5)))); a3xw1 = true;
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 40))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 4)))); a3xw5 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 12)))); a3xw0 = true;
L_1b7668:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, (uint32_t)v0)))); a3xw2 = true;
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)t0, 14)));
    if ((int64_t)v1 != 0) { (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1)))); a3xw4 = true; goto L_1b76ac; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 48))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    if ((int64_t)v0 == (int64_t)s1) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 44)))); goto L_1b76e8; }
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1)))); a3xw4 = true;
L_1b76ac:;
    (v0 = (uint64_t)(((uint64_t)RC_U64X(t2) < (uint64_t)t3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 8)))); a3xw0 = true; if (c) { A3_BACK(); goto L_1b7668; } }
L_1b76b8:;
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 1))));
L_1b76bc:;
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 28))));
    (v0 = (uint64_t)(((int64_t)(int64_t)t6 < (int64_t)(int64_t)t8) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 28)))); a3xw3 = true; if (c) { A3_BACK(); goto L_1b7638; } }
L_1b76cc:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1b76d0:;
    if ((int64_t)a1 == 0) goto L_1b76f8;
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a6x::strcpy(ram, rt, a0, (uint32_t)a1, h2, h4); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1b7774;
L_1b76e8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    { A3_BACK(); goto L_1b76d0; }
L_1b76f8:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 10128))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 35); uint32_t aligned_addr = addr & ~3u; uint32_t offset = addr & 3u; uint32_t mem = A1_R32(aligned_addr); uint32_t shift = (3u - offset) << 3; uint32_t keepMask = (shift == 0) ? 0u : ((1u << shift) - 1u); uint32_t merged = ((uint32_t)v1 & keepMask) | (mem << shift); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)merged))); }
    { uint32_t addr = ADD32((uint32_t)a1, 32); uint32_t aligned_addr = addr & ~3u; uint32_t offset = addr & 3u; uint32_t mem = A1_R32(aligned_addr); uint32_t shift = offset << 3; uint32_t keepMask = (offset == 0) ? 0u : (0xFFFFFFFFu << ((4u - offset) << 3)); uint32_t merged32 = ((uint32_t)v1 & keepMask) | (mem >> shift); uint64_t merged64 = (RC_U64X(v1) & 0xFFFFFFFF00000000ull) | (uint64_t)merged32; if (offset == 0) merged64 = (uint64_t)(int64_t)(int32_t)merged32; (v1 = (uint64_t)(merged64)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)a1, 36))));
    { uint32_t addr = ADD32((uint32_t)s2, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 35); uint32_t aligned_addr = addr & ~3u; uint32_t offset = addr & 3u; uint32_t shift = (3u - offset) << 3; uint32_t mask = 0xFFFFFFFFu >> shift; uint32_t old_data = A1_R32(aligned_addr); uint32_t val = (uint32_t)v1; uint32_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W32(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 32); uint32_t aligned_addr = addr & ~3u; uint32_t offset = addr & 3u; uint32_t shift = offset << 3; uint32_t mask = 0xFFFFFFFFu << shift; uint32_t old_data = A1_R32(aligned_addr); uint32_t val = (uint32_t)v1; uint32_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W32(aligned_addr, new_data); }
    A1_W8B((uint32_t)s2, 36, (uint8_t)(uint32_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    goto L_1b7774;
L_1b7760:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); a3xw2 = true;
L_1b7764:;
    (v0 = (uint64_t)(((int64_t)(int64_t)t0 < (int64_t)(int64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 32)))); if (c) { A3_BACK(); goto L_1b75d0; } }
L_1b7770:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1b7774:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); if (xo) { if (a3xw0) xo[0] = a2; if (a3xw1) xo[1] = a3; if (a3xw2) xo[2] = t0; if (a3xw3) xo[3] = t1; if (a3xw4) xo[4] = t2; if (a3xw5) xo[5] = t3; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// Sg2CtrlEntryTickProc (0x1cc030): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2CtrlEntryTickProc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st)
{
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)29 << 16))));
    v1 = 0x270000u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294950952))));
    v1 = v1 + 0x3D40u;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1cc074;
L_1cc070:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
L_1cc074:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)16) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1cc098;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 != (int64_t)a1; v1 = v1 + 8u; if (c) { A3_BACK(); goto L_1cc070; } }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = Sg2CtrlEntryTickProcById(ram, rt, a0, (uint32_t)a1); }
L_1cc098:;
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1cc0ac; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_1cc0ac:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2CtrlRemoveTickProc (0x1cc108): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2CtrlRemoveTickProc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st)
{
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    a3 = 0x270000u;
    (t0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    v1 = a3 + 0x3D40u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 != (int64_t)s0; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1cc158; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)29 << 16))));
    A1_W32B(v1, 4, 0u);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294950952))));
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W32B(v1, 0, (uint32_t)v0);
    goto L_1cc198;
L_1cc158:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
L_1cc15c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)16) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 3)))); if (c) goto L_1cc198; }
    a2 = a3 + 0x3D40u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    if ((int64_t)v0 != (int64_t)s0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); { A3_BACK(); goto L_1cc15c; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)29 << 16))));
    v1 = a2 + v1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294950952))));
    A1_W32B(v1, 4, 0u);
    A1_W32B((uint32_t)a0, 0, (uint32_t)v0);
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1cc198:;
    { const bool c = (int64_t)t0 == 0; (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1cc1ac; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_1cc1ac:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2DspEffectSetEndAdr (0x1cc320): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2DspEffectSetEndAdr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 15816))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8))));
    v1 = v1 << 3;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == 0; A1_W32B(v1, 0, (uint32_t)s1); if (c) goto L_1cc36c; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1cc36c:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2DspEffectSetType (0x1cc388): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2DspEffectSetType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 15816))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4))));
    v1 = v1 << 3;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == 0; A1_W32B(v1, 0, (uint32_t)s1); if (c) goto L_1cc3d4; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1cc3d4:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2DspEffectSetDepth (0x1cc3f0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2DspEffectSetDepth(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { s0 = a1 << 16; A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 1))));
    s0 = (uint32_t)((int32_t)s0 >> 16);
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    v1 = v1 << 3;
    at = 0x270000u;
    at = at + 0x3DC8u;
    v1 = at + v1;
    (s1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 12))));
    A1_W16B(v1, 14, (uint16_t)s0);
    { const bool c = (int64_t)v0 == 0; A1_W16B(v1, 12, (uint16_t)s0); if (c) goto L_1cc440; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1cc440:;
    (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2DspEffectSetDepth2 (0x1cc458): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2DspEffectSetDepth2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    s0 = a2 << 16;
    { s1 = a1 << 16; A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s2, 1))));
    s0 = (uint32_t)((int32_t)s0 >> 16);
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s2))));
    s1 = (uint32_t)((int32_t)s1 >> 16);
    v1 = v1 << 3;
    at = 0x270000u;
    at = at + 0x3DC8u;
    v1 = at + v1;
    A1_W16B(v1, 14, (uint16_t)s0);
    s0 = (uint32_t)(int16_t)A1_R16B(v1, 12);
    { const bool c = (int64_t)v0 == 0; A1_W16B(v1, 12, (uint16_t)s1); if (c) goto L_1cc4b4; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1cc4b4:;
    (v0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2DspEffectSetDelay (0x1cc4d0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2DspEffectSetDelay(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    v1 = v1 << 3;
    at = 0x270000u;
    at = at + 0x3DD8u;
    v1 = at + v1;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == 0; A1_W32B(v1, 0, (uint32_t)s1); if (c) goto L_1cc518; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1cc518:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2DspEffectSetFeedBack (0x1cc530): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2DspEffectSetFeedBack(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    v1 = v1 << 3;
    at = 0x270000u;
    at = at + 0x3DDCu;
    v1 = at + v1;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == 0; A1_W32B(v1, 0, (uint32_t)s1); if (c) goto L_1cc578; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1cc578:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ModMidiout1 (0x1ce2c8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2ModMidiout1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t a1, uint32_t &io_c0st, uint64_t *xo)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { s1 = a1 & 0xFFu; A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    a1 = v1 << 5;
    a1 = a1 - v1;
    a1 = a1 << 4;
    at = 0x270000u;
    at = at + 0x3F3Cu;
    a1 = at + a1;
    v1 = A1_R32B(a1, 512);
    v1 = a1 + v1;
    A1_W8(v1, (uint8_t)s1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 512))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    (a0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)511));
    { const bool c = (int64_t)v0 == 0; A1_W32B(a1, 512, (uint32_t)a0); if (c) goto L_1ce338; }
    { if (xo) { xo[0] = a1_sx64(a1); } const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1ce338:;
    { if (xo) { xo[0] = a1_sx64(a1); } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ModMidiout1Exclusive (0x1ce350): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2ModMidiout1Exclusive(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    a3 = 0xF7u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16188))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
L_1ce398:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 512))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, v1))));
    A1_W8B(v1, 0, (uint8_t)(uint32_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 512))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)511));
    A1_W32B((uint32_t)a1, 512, (uint32_t)v0);
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 0)));
    { const bool c = v1 != a3; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1)))); if (c) { A3_BACK(); goto L_1ce398; } }
    if ((int64_t)a2 == 0) goto L_1ce3e0;
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1ce3e0:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ModMidiout (0x1ce3f0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3; VU0 in c0st h4, out c0st h2, frame 64 bytes, 0 slot accesses)
inline uint64_t Sg2ModMidiout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint32_t &io_c0st, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s5 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    a3 = 0x200u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 5))));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16188))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 512))));
    s1 = a3 - v1;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s1, (uint32_t)s2))));
    { const bool c = (int32_t)v0 < 0; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0)))); if (c) goto L_1ce488; }
    { A3_CHARGE(); v0 = a6x::memcpy(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 512))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s2))));
    v1 = v1 & 0x1FFu;
    A1_W32B((uint32_t)s0, 512, v1);
    goto L_1ce4a4;
L_1ce488:;
    { (a2 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL)); A3_CHARGE(); v0 = a6x::memcpy(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, s1))));
    { (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a6x::memcpy(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    A1_W32B((uint32_t)s0, 512, (uint32_t)s4);
L_1ce4a4:;
    if ((int64_t)s5 == 0) goto L_1ce4d0;
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a1_sx64(a3); } const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; io_h2 = h2; return a4_t; }
L_1ce4d0:;
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a1_sx64(a3); } io_c0st = c0st; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// Sg2ModOpenBgm (0x1ce898): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in c0st h4 h5, out c0st h2 h3, frame 80 bytes, 0 slot accesses)
inline uint64_t Sg2ModOpenBgm(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t io_h5, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s7 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = vab_check_alive(ram, rt, a0); }
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1ce900; }
    goto L_1cea3c;
L_1ce8e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { A1_W32B((uint32_t)t2, 0, (uint32_t)v0); A3_CHARGE(); v0 = Sg2SlotGetSlotClassIdx(ram, rt); }
    v1 = s1 + s0;
    A1_W32B(v1, 4, (uint32_t)v0);
    goto L_1ce97c;
L_1ce900:;
    { s3 = 0x270000u; A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true;
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)40 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 29912)))); a3xw4 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0)))); a3xw2 = true;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32(s3, 16088))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw3 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a3))));
L_1ce930:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 31))));
    (v1 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if (a1_sx64(v1) != 0) do { const a4::q128 _v = (a4::mk128(h4, a0)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (s0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    v1 = s0 + s1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == (int64_t)t1; (a2 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)32) ? 1 : 0)); a3xw0 = true; if (c) { A3_BACK(); goto L_1ce8e0; } }
    { const bool c = (int64_t)a2 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a3)))); if (c) { A3_BACK(); goto L_1ce930; } }
L_1ce97c:;
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1ce990; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1ce990:;
    { const bool c = (int64_t)s4 == (int64_t)v0; (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); if (c) goto L_1cea34; }
    { (s3 = (uint32_t)((int32_t)((int32_t)ADD32(s3, 16088)))); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = kernel_sub_001CDDE8_x14(ram, rt, a0, h3, h5, a7c); t1 = a7c[2]; t2 = a7c[3]; t3 = a7c[4]; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s4))));
    s5 = s3 + 8u;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    s0 = s3 + 0xCu;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(v1, (uint32_t)v0))));
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    s1 = v1 << 4;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, s5)))); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, s0))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, 80))));
    A1_W32B((uint32_t)v0, 16, (uint32_t)s7);
    { (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, s0)))); A3_CHARGE(); v0 = vab_get_info_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 84))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, s3))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 88)))); a3xw0 = true;
    A1_W32B((uint32_t)v0, 0, (uint32_t)s6);
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, (uint32_t)a1))));
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, (uint32_t)a2)))); a3xw0 = true; A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = a1_sx64(vab_get_lfotbl_param_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a7c)); a3 = a7c[0]; t0 = a7c[1]; a3xw1 = true; a3xw2 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(s3, (uint32_t)s2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    { const bool c = (int32_t)a0 >= 0; A1_W32B(s0, 0, (uint32_t)v0); if (c) goto L_1cea1c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, s5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 5800))));
    A1_W32B(v1, 0, (uint32_t)v0);
L_1cea1c:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 76))));
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, (uint32_t)a1)))); A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = a1_sx64(vab_get_veltbl_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, a7c)); a2 = a7c[0]; a3xw0 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, s3))));
    A1_W32B(v1, 0, (uint32_t)v0);
L_1cea34:;
    (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
L_1cea3c:;
    { if (xo) { if (a3xw0) xo[0] = a2; if (a3xw1) xo[1] = a3; if (a3xw2) xo[2] = t0; if (a3xw3) xo[3] = t1; if (a3xw4) xo[4] = t2; if (a3xw5) xo[5] = t3; } io_c0st = c0st; io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// Sg2ModOpenSe (0x1cea68): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in c0st h4, out c0st h2, frame 80 bytes, 0 slot accesses)
inline uint64_t Sg2ModOpenSe(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s7 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = vab_check_alive(ram, rt, a0); }
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1cead0; }
    goto L_1cec0c;
L_1ceab0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { A1_W32B((uint32_t)t2, 0, (uint32_t)v0); A3_CHARGE(); v0 = Sg2SlotGetSlotClassIdx(ram, rt); }
    v1 = s1 + s0;
    A1_W32B(v1, 4, (uint32_t)v0);
    goto L_1ceb4c;
L_1cead0:;
    { s3 = 0x270000u; A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw2 = true;
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)40 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 29912)))); a3xw5 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0)))); a3xw3 = true;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32(s3, 16088))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw4 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a3)))); a3xw0 = true;
L_1ceb00:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); a3xw2 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 31))));
    (v1 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if (a1_sx64(v1) != 0) do { const a4::q128 _v = (a4::mk128(h4, a0)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (s0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    v1 = s0 + s1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == (int64_t)t1; (a2 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)32) ? 1 : 0)); a3xw1 = true; if (c) { A3_BACK(); goto L_1ceab0; } }
    { const bool c = (int64_t)a2 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a3)))); a3xw0 = true; if (c) { A3_BACK(); goto L_1ceb00; } }
L_1ceb4c:;
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1ceb60; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1ceb60:;
    { const bool c = (int64_t)s4 == (int64_t)v0; (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); if (c) goto L_1cec04; }
    { (s3 = (uint32_t)((int32_t)((int32_t)ADD32(s3, 16088)))); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = a1_sx64(kernel_sub_001CDF38_x14(ram, rt, a0, a7c)); t1 = a7c[2]; t2 = a7c[3]; t3 = a7c[4]; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s4))));
    s5 = s3 + 8u;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    s0 = s3 + 0xCu;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(v1, (uint32_t)v0))));
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    s1 = v1 << 4;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, s5)))); a3xw1 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, s0)))); a3xw0 = true;
    (v0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, 80))));
    A1_W32B((uint32_t)v0, 16, (uint32_t)s7);
    { (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, s0)))); A3_CHARGE(); v0 = vab_get_info_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 84)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, s3))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 88)))); a3xw1 = true;
    A1_W32B((uint32_t)v0, 0, (uint32_t)s6);
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, (uint32_t)a1)))); a3xw0 = true;
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, (uint32_t)a2)))); a3xw1 = true; A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = a1_sx64(vab_get_lfotbl_param_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a7c)); a3 = a7c[0]; t0 = a7c[1]; a3xw2 = true; a3xw3 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(s3, (uint32_t)s2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    { const bool c = (int32_t)a0 >= 0; A1_W32B(s0, 0, (uint32_t)v0); if (c) goto L_1cebec; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, s5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 5800))));
    A1_W32B(v1, 0, (uint32_t)v0);
L_1cebec:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 76)))); a3xw0 = true;
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s1, (uint32_t)a1)))); a3xw0 = true; A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = a1_sx64(vab_get_veltbl_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, a7c)); a2 = a7c[0]; a3xw1 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, s3))));
    A1_W32B(v1, 0, (uint32_t)v0);
L_1cec04:;
    (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
L_1cec0c:;
    { if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a3; if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = t2; if (a3xw6) xo[6] = t3; } io_c0st = c0st; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001D0E50 (0x1d0e50): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in -, out -)
inline uint64_t kernel_sub_001D0E50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    sp = sp - 0xE0u;
    a1 = 0x270000u;
    s2 = 0x270000u;
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 16076))));
    a3 = A1_R32B(a1, 16080);
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (a1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2)));
    v1 = v1 & 0xFu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 3))));
    (a2 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a3))));
    A1_W32B((uint32_t)v0, 764, a1);
    { a1 = A1_R32B(a3, 4); A3_CHARGE(); v0 = Sg2SlotFindSlotClassCh(ram, rt, (uint32_t)a0, a1_sx64(a1), a2); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { const bool c = (int32_t)s0 < 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2)))); if (c) goto L_1d0ed4; }
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, sp))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 16076))));
L_1d0eb8:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 0))));
    s1 = s1 - 4u;
    { (a1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2))); A3_CHARGE(); v0 = Sg2SlotSetLfoPitchSpeed(ram, rt, a0, a1_sx64(a1)); }
    { const bool c = (int32_t)s0 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 16076)))); if (c) { A3_BACK(); goto L_1d0eb8; } }
L_1d0ed4:;
    { sp = sp + 0xE0u; return v0; }
    __builtin_unreachable();
}

// Sg2RemoteSendDmaCom (0x1d20a0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in c0st h4, out c0st h3, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2RemoteSendDmaCom(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint32_t a3, uint32_t &io_c0st, uint64_t &io_h3, uint64_t io_h4, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    s0 = 0x290000u;
    s0 = s0 - 0x48D8u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294948672))));
    t3 = 0xFFFFFFFFu;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (t4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (t2 = (uint64_t)((uint64_t)a1_sx64(t4) + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)RC_U64X(t2) + (uint64_t)0ULL));
    A1_W32B(t0, 16, t3);
    A1_W32B((uint32_t)t1, 0, a0);
    A1_W32B((uint32_t)t2, 4, a1);
    A1_W32B((uint32_t)v1, 8, (uint32_t)a2);
    { A1_W32B(t4, 12, a3); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    a0 = A1_R32(s0);
    a1 = a0 + 1u;
    a0 = a0 + 0x20u;
    (a2 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a1) < (int64_t)(int32_t)0) ? 1 : 0));
    (v1 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL));
    if (RC_U64X(a2) != 0) do { const a4::q128 _v = (a4::mk128(h4, a1_sx64(a0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    (a1 = (uint32_t)((int32_t)((int32_t)SUB32(a1, (uint32_t)v1))));
    { const bool c = (int64_t)v0 == 0; A1_W32(s0, a1); if (c) goto L_1d2134; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1d2134:;
    (v0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; xo[2] = a1_sx64(t0); xo[3] = t1; xo[4] = t2; xo[5] = a1_sx64(t3); } io_c0st = c0st; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetDumperOff (0x1d6d90): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st h3, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetDumperOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t &io_h3)
{
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (t0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952440))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294952496))));
    t2 = 0xFFFFFFF7u;
    t1 = 4u;
    a3 = 1u;
L_1d6dd0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)8)); if (c) goto L_1d6e38; }
    if ((int64_t)v1 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6e3c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    if ((int64_t)v0 != (int64_t)s1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6e3c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 20))));
    if ((int64_t)v0 != (int64_t)s0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6e3c; }
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(t2)));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)24) ? 1 : 0));
    (v1 = (uint64_t)((uint64_t)a1_sx64(t1) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a1, 0, (uint32_t)a0);
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, t3))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); if (c) goto L_1d6e28; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a3, (uint32_t)a2 & 0x1F))));
    goto L_1d6e30;
L_1d6e28:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967272))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a3, (uint32_t)v0 & 0x1F))));
L_1d6e30:;
    (v0 = (uint64_t)(RC_U64X(a0) | RC_U64X(v0)));
    A1_W32B((uint32_t)v1, 0, (uint32_t)v0);
L_1d6e38:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_1d6e3c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 224)))); if (c) { A3_BACK(); goto L_1d6dd0; } }
    if ((int64_t)t0 == 0) goto L_1d6e60;
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; io_h3 = h3; return a4_t; }
L_1d6e60:;
    { io_c0st = c0st; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// Sg2SlotModKeyOff (0x1d6e70): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st h3, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotModKeyOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t &io_c0st, uint64_t &io_h3)
{
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (t3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (t6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294952440))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t5 = 0xFFFFFFF7u;
    t4 = 4u;
    t1 = 1u;
    (a3 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d6ec0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6f3c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 12))));
    if ((int64_t)v0 != (int64_t)s2) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6f3c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    if ((int64_t)v1 != (int64_t)s1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6f3c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    if ((int64_t)v0 != (int64_t)s0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d6f3c; }
    (a0 = (uint64_t)(RC_U64X(a0) & a1_sx64(t5)));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)24) ? 1 : 0));
    (v1 = (uint64_t)((uint64_t)a1_sx64(t4) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a3, 0, (uint32_t)a0);
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, t6))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); if (c) goto L_1d6f28; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t1, (uint32_t)a2 & 0x1F))));
    goto L_1d6f30;
L_1d6f28:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967272))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t1, (uint32_t)v0 & 0x1F))));
L_1d6f30:;
    (v0 = (uint64_t)(RC_U64X(a0) | RC_U64X(v0)));
    A1_W32B((uint32_t)v1, 0, (uint32_t)v0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_1d6f3c:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 224))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 224)))); if (c) { A3_BACK(); goto L_1d6ec0; } }
    if ((int64_t)t3 == 0) goto L_1d6f68;
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; io_h3 = h3; return a4_t; }
L_1d6f68:;
    { io_c0st = c0st; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// Sg2SlotVabKeyOff (0x1d6f80): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st h3, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotVabKeyOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t &io_c0st, uint64_t &io_h3)
{
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (t3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (t6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294952440))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t5 = 0xFFFFFFF7u;
    t4 = 4u;
    t1 = 1u;
    (a3 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d6fd0:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d704c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 16))));
    { const bool c = (int64_t)v0 != (int64_t)s2; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 16)))); if (c) goto L_1d7048; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = (int64_t)v1 != (int64_t)s1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0)))); if (c) goto L_1d7048; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    if ((int64_t)v0 != (int64_t)s0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1d704c; }
    (a0 = (uint64_t)(RC_U64X(a1) & a1_sx64(t5)));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)24) ? 1 : 0));
    (v1 = (uint64_t)((uint64_t)a1_sx64(t4) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a3, 0, (uint32_t)a0);
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, t6))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); if (c) goto L_1d7038; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t1, (uint32_t)a2 & 0x1F))));
    goto L_1d7040;
L_1d7038:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967272))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t1, (uint32_t)v0 & 0x1F))));
L_1d7040:;
    (v0 = (uint64_t)(a0 | RC_U64X(v0)));
    A1_W32B((uint32_t)v1, 0, (uint32_t)v0);
L_1d7048:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_1d704c:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 224))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 224)))); if (c) { A3_BACK(); goto L_1d6fd0; } }
    if ((int64_t)t3 == 0) goto L_1d7078;
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; io_h3 = h3; return a4_t; }
L_1d7078:;
    { io_c0st = c0st; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetKeyOnBit (0x1d71d8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetKeyOnBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    at = 0x290000u;
    at = at - 0x3A10u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)s0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) | s1));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 0, (uint32_t)v1); if (c) goto L_1d7228; }
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1d7228:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetKeyOffBit (0x1d7240): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetKeyOffBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    at = 0x290000u;
    at = at - 0x3A08u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)s0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) | s1));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 0, (uint32_t)v1); if (c) goto L_1d7290; }
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1d7290:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetReverbOnBit (0x1d72a8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetReverbOnBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    at = 0x290000u;
    at = at - 0x39F0u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)s0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) | s1));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 0, (uint32_t)v1); if (c) goto L_1d72f8; }
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1d72f8:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetReverbOffBit (0x1d7310): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetReverbOffBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    at = 0x290000u;
    at = at - 0x39F0u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)s0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (s1 = (uint64_t)(~(0ULL | s1)));
    (v1 = (uint64_t)(RC_U64X(v1) & s1));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 0, (uint32_t)v1); if (c) goto L_1d7368; }
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1d7368:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetNoiseOnBit (0x1d7380): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetNoiseOnBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    at = 0x290000u;
    at = at - 0x3A00u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)s0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) | s1));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 0, (uint32_t)v1); if (c) goto L_1d73d0; }
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1d73d0:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetNoiseOffBit (0x1d73e8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetNoiseOffBit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t &io_c0st)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    at = 0x290000u;
    at = at - 0x3A00u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)s0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (s1 = (uint64_t)(~(0ULL | s1)));
    (v1 = (uint64_t)(RC_U64X(v1) & s1));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 0, (uint32_t)v1); if (c) goto L_1d7440; }
    { const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1d7440:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DB088 (0x1db088): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f20 xo[5]=f21 xo[6]=f22 xo[7]=f23; VU0 in c0st, out c0st, frame 80 bytes, 12 slot accesses)
inline uint64_t kernel_sub_001DB088(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t a3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t fcr31 = 0;
    uint32_t c0st = io_c0st;
    uint8_t a2fr[80];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, (uint32_t)v0))));
    a2::frSt<uint32_t>(a2fr, 64, a1_bits(f23));
    s1 = a0 + 0x14u;
    a2::frSt<uint32_t>(a2fr, 56, a1_bits(f22));
    a2::frSt<uint32_t>(a2fr, 48, a1_bits(f21));
    f21 = ((float)(f13));
    a2::frSt<uint32_t>(a2fr, 40, a1_bits(f20));
    f20 = ((float)(f12));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    f22 = a1_flt(A1_R32B(s1, 100));
    { f23 = a1_flt(A1_R32B(s1, 96)); A3_CHARGE(); v0 = a1_sx64(Sg2CtrlGetTickMode(ram, rt)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    at = 0x447A0000u;
    f0 = a1_flt(a7_opq(at));
    { uint32_t bits = (uint32_t)s0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    f3 = a1_flt(a7_opq(at));
    f2 = a1f::div(fcr31, f0, f2);
    f2 = a1f::div(fcr31, f1, f2);
    fcr31 = (a1f::lt(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f2 = ((float)(f3)); goto L_1db134; }
L_1db134:;
    if ((int64_t)s2 == 0) goto L_1db14c;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 4))));
    A1_W32B(s1, 8, (uint32_t)s2);
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)4));
    A1_W32B(s1, 4, (uint32_t)v0);
L_1db14c:;
    { int32_t tmp = a1f::cvtw(f20); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    at = 0xC47A0000u;
    f1 = a1_flt(a7_opq(at));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    fcr31 = (a1f::eq(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1db194;
    f0 = a1f::sub(f20, f22);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 4))));
    A1_W32B(s1, 12, a1_bits(f20));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)1));
    A1_W32B(s1, 4, (uint32_t)v0);
    f0 = a1f::div(fcr31, f0, f2);
    A1_W32B(s1, 20, a1_bits(f0));
L_1db194:;
    { int32_t tmp = a1f::cvtw(f21); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    fcr31 = (a1f::eq(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 4)))); if (c) goto L_1db1d4; }
    f0 = a1f::sub(f21, f23);
    A1_W32B(s1, 16, a1_bits(f21));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)2));
    A1_W32B(s1, 4, (uint32_t)v0);
    f0 = a1f::div(fcr31, f0, f2);
    A1_W32B(s1, 24, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 4))));
L_1db1d4:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1db1e0; }
    A1_W32B(s1, 0, (uint32_t)v0);
L_1db1e0:;
    if ((int64_t)s3 == 0) goto L_1db210;
    f23 = a1_flt(a2::frLd<uint32_t>(a2fr, 64));
    f22 = a1_flt(a2::frLd<uint32_t>(a2fr, 56));
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 48));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 40));
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f20); xo[5] = a1_bits(f21); xo[6] = a1_bits(f22); xo[7] = a1_bits(f23); } const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1db210:;
    f23 = a1_flt(a2::frLd<uint32_t>(a2fr, 64));
    f22 = a1_flt(a2::frLd<uint32_t>(a2fr, 56));
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 48));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 40));
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f20); xo[5] = a1_bits(f21); xo[6] = a1_bits(f22); xo[7] = a1_bits(f23); } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ExPcmInputCtrl (0x1db950): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2ExPcmInputCtrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t &io_c0st, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t c0st = io_c0st;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v0 = (uint64_t)(((uint64_t)s0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)(s1 & (uint64_t)(uint16_t)3)); if (c) goto L_1db9d0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    if ((int64_t)v1 == (int64_t)v0) { goto L_1db9d4; }
    { A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    a2 = 0x290000u; a3xw1 = true;
    (t0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw3 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
    { int64_t result = (int64_t)(int32_t)s0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a3 = (uint32_t)((int32_t)((int32_t)result))); } a3xw2 = true;
    (v0 = (uint64_t)(s1 & (uint64_t)(uint16_t)1));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, 2008)))); a3xw0 = true;
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((uint64_t)a1_sx64(a3) + (uint64_t)0ULL)); if (c) goto L_1db9b8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    A1_W32B((uint32_t)v0, 20, 0u);
    A1_W32B((uint32_t)v1, 464, 0u);
L_1db9b8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, 2008))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, (uint32_t)v0))));
    { const bool c = (int64_t)t0 == 0; A1_W32B((uint32_t)v0, 0, (uint32_t)s1); if (c) goto L_1db9d0; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1db9d0:;
L_1db9d4:;
    { if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a1_sx64(a2); if (a3xw2) xo[2] = a1_sx64(a3); if (a3xw3) xo[3] = t0; } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// manager_sub_0115C2C0 (0x115c2c0): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a2; VU0 in -, out h2, jump tables 1, relocated calls 1, frame 48 bytes, 0 slot accesses)
inline uint64_t manager_sub_0115C2C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    { const bool c = (int32_t)a0 <= 0; (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8)))); if (c) goto L_115c384; }
L_115c2f0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B(s0, 0))));
    (v1 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)s1, 0))));
    if ((int64_t)(int32_t)v1 != (int64_t)v0) { (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 1)))); goto L_115c378; }
    (v0 = (uint64_t)(((uint64_t)a1_sx64(v1) < (uint64_t)(int64_t)(int32_t)5) ? 1 : 0));
    if ((int64_t)v0 == 0) { (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 1)))); goto L_115c378; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    v1 = 0x1190000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    v1 = A1_R32B(v1, 26464);
    { const uint32_t a5_jt = v1; switch (a5_jt) { case 0x115C328u: goto L_115c328; case 0x115C340u: goto L_115c340; case 0x115C358u: goto L_115c358; default: A5_BADJUMP(0x115C31Cu, a5_jt); } }
L_115c328:;
    (v1 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)s1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(s0, 4))));
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 1)))); if (c) goto L_115c378; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    goto L_115c3a8;
L_115c340:;
    (v1 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)s1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(s0, 4))));
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 1)))); if (c) goto L_115c378; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    goto L_115c398;
L_115c358:;
    A1_W16B((uint32_t)s1, 4, (uint16_t)0u);
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { a2 = 8u; a3xw0 = true; A3_CHARGE(); v0 = a1_sx64(a6x::memcmp(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, h2)); }
    { const bool c = (int64_t)v0 == 0; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 1)))); if (c) goto L_115c390; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
L_115c378:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s3 < (int64_t)(int64_t)a0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s0 = s0 + 0x10u; if (c) { A3_BACK(); goto L_115c2f0; } }
L_115c384:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    goto L_115c3a8;
L_115c390:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    goto L_115c3a8;
L_115c398:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, -496))));
    v1 = v1 << 3;
    v1 = s0 + v1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
L_115c3a8:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a2); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// manager_sub_011641E0 (0x11641e0): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in -, out -)
inline uint64_t manager_sub_011641E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)a0 != (int64_t)v0) goto L_1164250;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)64512));
    (v0 = (uint64_t)(RC_U64X(v0) << 26));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, -428))));
    (a1 = (uint64_t)(a1 & v0));
    { const bool c = (int32_t)v1 <= 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1164250; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, -424))));
L_1164218:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); if (c) goto L_1164240; }
    { A3_CHARGE(); v0 = manager_sub_01164260(ram, rt, a0); }
    goto L_1164258;
L_1164240:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, -428))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, -424)))); if (c) { A3_BACK(); goto L_1164218; } }
L_1164250:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1164258:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); return v0; }
    __builtin_unreachable();
}

// GetAttributeRelationEffectDefByName (0x1167238): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a2 xo[1]=a3; VU0 in h2, out h2, relocated calls 1, frame 64 bytes, 0 slot accesses)
inline uint64_t GetAttributeRelationEffectDefByName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1167288; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_116728c;
L_1167288:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_116728c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1167290u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_11672a8; }
    { const uint32_t _dynInsn = A1_R32(0x1167294u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)(int32_t)v1) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_11672ac; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    goto L_11672ac;
L_11672a8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11672ac:;
    (s2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_1167380; }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0)))); if (c) goto L_11672d0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_11672d4;
L_11672d0:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11672d4:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x11672D8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1167300; }
    { const uint32_t _dynInsn = A1_R32(0x11672DCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a1 == (int64_t)(int32_t)v1) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1167304; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 16))));
    goto L_1167304;
L_11672f0:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)s6, 0, v1);
    goto L_1167384;
L_1167300:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1167304:;
    { const bool c = (int64_t)a0 == 0; (s1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) goto L_1167380; }
    s3 = 0x30000u;
    { const uint32_t _dynInsn = A1_R32(0x1167310u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s4 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    s3 = s3 | 0xFFFFu;
L_1167318:;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 4))));
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(a2) & a1_sx64(s3)));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)SUB32(v1, (uint32_t)v0)))); if (c) goto L_1167334; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
L_1167334:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1167338u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s4, _dynSimm))))); } if (c) goto L_1167360; }
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_1167360; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v1 = v1 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    goto L_1167364;
L_1167360:;
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1167364:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    { (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(a6x::strcmp(ram, rt, (uint32_t)a0, (uint32_t)a1, h2)); }
    { const bool c = (int64_t)v0 == 0; (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295)))); if (c) { A3_BACK(); goto L_11672f0; } }
    { const bool c = (int64_t)s2 != 0; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8)))); if (c) { A3_BACK(); goto L_1167318; } }
L_1167380:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1167384:;
    { if (xo) { if (a3xw0) xo[0] = a2; xo[1] = a1_sx64(a3); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013458A0 (0x13458a0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=a0 xo[7]=a1 xo[8]=a2 xo[9]=a3 xo[10]=t0 xo[11]=t1; VU0 in -, out hi1 lo1, relocated calls 1)
inline uint64_t gamecore_sub_013458A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t s0 = 0;
    uint32_t fcr31 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    bool a3xw0 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    { uint32_t bits = 0u; std::memcpy(&f4, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 22096))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 424); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 416); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    f2 = a1f::sub(f3, f5);
    fcr31 = (a1f::eq(f2, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { uint32_t bits = A1_R32B((uint32_t)v0, 420); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } if (c) goto L_13458d8; }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 420, bits); }
    goto L_134596c;
L_13458d8:;
    { uint32_t bits = A1_R32B((uint32_t)v0, 384); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f4, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f0 = a1f::mul(f1, f0); a3xw0 = true;
    { const bool c = (fcr31 & 0x800000u) == 0u; f2 = a1f::add(f5, f0); if (c) goto L_1345908; }
    fcr31 = (a1f::le(f1, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f2 = ((float)(f3)); goto L_1345920; }
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    goto L_1345918;
L_1345908:;
    fcr31 = (a1f::le(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f2 = ((float)(f3)); goto L_1345920; }
    fcr31 = (a1f::lt(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1345918:;
    if ((fcr31 & 0x800000u) != 0u) { f2 = ((float)(f3)); goto L_1345920; }
L_1345920:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17280 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 22096))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 352)));
    f0 = a1f::mul(f2, f0); a3xw0 = true;
    (v0 = (uint64_t)(A1_R64B((uint32_t)s0, 328)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)s0, 0))); a3xw6 = true;
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)v1));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 416, bits); }
    (a0 = (uint64_t)(a0 << 8)); a3xw6 = true;
    A1_W64B((uint32_t)s0, 352, v0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); } a3xw7 = true;
    (a0 = (uint64_t)(a0 >> 8)); a3xw6 = true;
    { (a1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); a3xw7 = true; A3_CHARGE(); uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = ps2___muldi3(ram, rt, a0, a1, t1, hi1, lo1, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; }
    A1_W64B((uint32_t)s0, 328, 0ULL);
    (v0 = (uint64_t)(v0 >> 8));
    A1_W64B((uint32_t)s0, 344, v0);
L_134596c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f4); xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a0; if (a3xw7) xo[7] = a1; if (a3xw8) xo[8] = a2; if (a3xw9) xo[9] = a3; if (a3xw10) xo[10] = t0; if (a3xw11) xo[11] = t1; } io_hi1 = hi1; io_lo1 = lo1; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01345980 (0x1345980): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f1 xo[1]=at xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=hi xo[9]=lo; VU0 in -, out hi1 lo1, relocated calls 1)
inline a1::RetVF gamecore_sub_01345980(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17280 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 22096))));
    { uint32_t bits = A1_R32B((uint32_t)s0, 416); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s0, 320)));
    f0 = a1f::mul(f0, f1);
    (a2 = (uint64_t)(A1_R64B((uint32_t)s0, 296)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 0)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)s0, 352)));
    (a2 = (uint64_t)((uint64_t)a2 + (uint64_t)a0));
    (v0 = (uint64_t)(A1_R64B((uint32_t)s0, 328)));
    (v1 = (uint64_t)(v1 << 8));
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(v1 >> 8));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)a3));
    A1_W64B((uint32_t)s0, 320, a2);
    A1_W64B((uint32_t)s0, 312, v1);
    (a0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    A1_W64B((uint32_t)s0, 352, v0);
    A1_W64B((uint32_t)s0, 296, 0ULL);
    { (a1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = ps2___muldi3(ram, rt, a0, a1, t1, hi1, lo1, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; hi = a7c[6]; lo = a7c[7]; }
    A1_W64B((uint32_t)s0, 328, 0ULL);
    (v0 = (uint64_t)(v0 >> 8));
    A1_W64B((uint32_t)s0, 344, v0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { xo[0] = a1_bits(f1); xo[1] = at; xo[2] = a0; xo[3] = a1; xo[4] = a2; xo[5] = a3; xo[6] = t0; xo[7] = t1; xo[8] = hi; xo[9] = lo; } io_hi1 = hi1; io_lo1 = lo1; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// ChangeGameAnimeTimeScaleRatio (0x1345db8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out hi1 lo1, relocated calls 1)
inline a1::RetVF ChangeGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, float f12, float f13, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t s0 = 0;
    uint32_t fcr31 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    fcr31 = (a1f::eq(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22096))));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 424, bits); }
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 420, bits); } if (c) goto L_1345e28; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17280 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 352)));
    f0 = a1f::mul(f12, f0);
    (v0 = (uint64_t)(A1_R64B((uint32_t)s0, 328)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)s0, 0)));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)v1));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 416, bits); }
    (a0 = (uint64_t)(a0 << 8));
    A1_W64B((uint32_t)s0, 352, v0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a0 = (uint64_t)(a0 >> 8));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = ps2___muldi3(ram, rt, a0, a1, t1, hi1, lo1, a7c); a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; }
    A1_W64B((uint32_t)s0, 328, 0ULL);
    (v0 = (uint64_t)(v0 >> 8));
    A1_W64B((uint32_t)s0, 344, v0);
L_1345e28:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { if (a3xw0) xo[0] = a2; if (a3xw1) xo[1] = a3; if (a3xw2) xo[2] = t0; if (a3xw3) xo[3] = t1; } io_hi1 = hi1; io_lo1 = lo1; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01346090 (0x1346090): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out hi1 lo1, relocated calls 1)
inline a1::RetVF gamecore_sub_01346090(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t s0 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17280 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 22096))));
    { uint32_t bits = A1_R32B((uint32_t)s0, 416); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 352)));
    f0 = a1f::mul(f0, f1);
    (v0 = (uint64_t)(A1_R64B((uint32_t)s0, 328)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)s0, 0)));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)v1));
    A1_W64B((uint32_t)s0, 352, v0);
    (a0 = (uint64_t)(a0 << 8));
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a0 = (uint64_t)(a0 >> 8));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); A3_CHARGE(); v0 = ps2___muldi3(ram, rt, a0, a1, t1, hi1, lo1); }
    A1_W64B((uint32_t)s0, 328, 0ULL);
    (v0 = (uint64_t)(v0 >> 8));
    A1_W64B((uint32_t)s0, 344, v0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_hi1 = hi1; io_lo1 = lo1; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01349D90 (0x1349d90): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a2; VU0 in -, out -, frame 64 bytes, 0 slot accesses)
inline void gamecore_sub_01349D90(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint32_t s6 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 12))));
    { const bool c = (int32_t)a2 <= 0; (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 16)))); if (c) goto L_1349eb0; }
    s3 = 0x30000u;
    { const uint32_t _dynInsn = A1_R32(0x1349DCCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1349DD0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s6 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    s3 = s3 | 0xFFFFu;
    s5 = 0x303u;
L_1349de0:;
    v1 = A1_R32(s0);
    (v0 = (uint64_t)(a1_sx64(v1) & a1_sx64(s3)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s0, (uint32_t)v0)))); if (c) goto L_1349df8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1349dfc;
L_1349df8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1349dfc:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1349E00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s2, _dynSimm))))); } if (c) goto L_1349e0c; }
    { const bool c = (int64_t)a0 != (int64_t)v0; (a1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_1349e14; }
L_1349e0c:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1349E10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s2, _dynSimm))))); }
L_1349e14:;
    { const uint32_t _dynInsn = A1_R32(0x1349E14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s6, _dynSimm)))); }
    if ((int64_t)v0 == (int64_t)a0) goto L_1349e30;
    { const bool c = (int64_t)a1 == (int64_t)a0; (v0 = (uint64_t)(a1_sx64(v1) & a1_sx64(s3))); if (c) goto L_1349e3c; }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
    goto L_1349ea4;
L_1349e30:;
    if ((int64_t)a1 != 0) { (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1)))); goto L_1349ea4; }
    (v0 = (uint64_t)(a1_sx64(v1) & a1_sx64(s3)));
L_1349e3c:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s0, (uint32_t)v0)))); if (c) goto L_1349e50; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1349e54;
L_1349e50:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1349e54:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1349E58u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s2, _dynSimm))))); } if (c) goto L_1349e80; }
    { const bool c = (int64_t)a0 == (int64_t)v0; v1 = v1 >> 18; if (c) goto L_1349e80; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v1 = v1 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    goto L_1349e84;
L_1349e80:;
    v1 = 0u;
L_1349e84:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    if ((int64_t)v0 != (int64_t)(int32_t)s5) { (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1)))); goto L_1349ea4; }
    A1_W32B(v1, 232, s0);
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 24)))); A3_CHARGE(); v0 = _gameFlagSet(ram, rt, a0); }
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 12))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
L_1349ea4:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int64_t)a1_sx64(a2)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s0 = s0 + 0x14u; if (c) { A3_BACK(); goto L_1349de0; } }
L_1349eb0:;
    { if (xo) { xo[0] = a1_sx64(a2); } return; }
    __builtin_unreachable();
}

// gamecore_sub_01361E88 (0x1361e88): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=t1 xo[1]=t2 xo[2]=t3; VU0 in -, out hi1 lo1 h2 h3 h5 h6, break)
inline a1::RetVF gamecore_sub_01361E88(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)0) ? 1 : 0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 25512))));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 64))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    { uint32_t bits = (uint32_t)t0; std::memcpy(&f4, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f4, sizeof(tmp)); f4 = ((float)(int32_t)(tmp)); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 60))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 52))));
    (t1 = (uint64_t)(RC_U64X(t1) | (uint64_t)(uint16_t)65535));
    f3 = a1f::mul(f2, f3);
    { uint32_t bits = A1_R32B((uint32_t)v1, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)a3; std::memcpy(&f5, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f5, sizeof(tmp)); f5 = ((float)(int32_t)(tmp)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    f2 = a1f::div(fcr31, f2, f4);
    if ((int64_t)a3 == 0) { A6_BREAK(0x1361EFDu); goto L_1361f00; }
L_1361f00:;
    { uint32_t bits = (uint32_t)t2; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 25516))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 25524))));
    f0 = a1f::mul(f1, f0);
    f1 = a1f::div(fcr31, f1, f5);
    f3 = a1f::div(fcr31, f3, f2);
    f0 = a1f::div(fcr31, f0, f1);
    { int32_t tmp = a1f::cvtw(f3); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    (a0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    if (RC_U64X(a0) != 0) do { const a4::q128 _v = (a4::mk128(h6, a2)); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)t0;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (a0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    if (RC_U64X(a0) != 0) do { const a4::q128 _v = (a4::mk128(h5, a1)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)(lo));
    (a2 = (uint64_t)(hi));
    { int32_t divisor = (int32_t)a3;    int32_t dividend = (int32_t)v0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)a3; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)(lo));
    (a1 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)t2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)s0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(RC_U64X(v1) & t1));
    { const bool c = (int64_t)v0 == 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a1)))); if (c) goto L_1361fc0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1361fc4;
L_1361fc0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1361fc4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1361FC8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1361fe0; }
    { const uint32_t _dynInsn = A1_R32(0x1361FCCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)v1) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1361fe4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    goto L_1361fe4;
L_1361fe0:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1361fe4:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)31)); if (c) goto L_1362008; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 5))));
    (s0 = (uint64_t)(RC_U64X(s0) | RC_U64X(v0)));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = getstagenumber_u14(ram, rt, a0, h3); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 15))));
    (v0 = (uint64_t)(RC_U64X(v0) | s0));
    goto L_136200c;
L_1362008:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_136200c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { xo[0] = t1; xo[1] = t2; xo[2] = t3; } io_hi1 = hi1; io_lo1 = lo1; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h6 = h6; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SetStageManagerAreaPreload (0x1366120): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f1 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=f6 xo[6]=at xo[7]=a0 xo[8]=a1 xo[9]=a2 xo[10]=hi xo[11]=lo; VU0 in -, out h2 h6, break)
inline a1::RetVF SetStageManagerAreaPreload(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, uint64_t &io_h2, uint64_t &io_h6, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); v0 = a1_sx64(PauseCamFinalizer(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 25512))));
    { const uint32_t _dynInsn = A1_R32(0x1366140u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f4, &bits, sizeof(bits)); }
    { const uint32_t _dynInsn = A1_R32(0x136614Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 52))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f6, &bits, sizeof(bits)); }
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)s0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    if ((int64_t)v0 == 0) { A6_BREAK(0x1366171u); goto L_1366174; }
L_1366174:;
    { uint32_t bits = A1_R32B((uint32_t)v1, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    A1_W32B((uint32_t)sp, 4, 0u);
    f2 = a1f::mul(f2, f4);
    f5 = a1f::mul(f3, f4);
    (a2 = (uint64_t)(hi));
    (s0 = (uint64_t)(lo));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = (uint32_t)s0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f0 = a1f::sub(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::addFinite(f0, f5);
    f0 = (-(float)(f0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f6; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 12, bits); }
    f3 = a1f::mul(f0, f4);
    f2 = a1f::mul(f2, f4);
    f1 = a1f::sub(f1, f2);
    f1 = a1f::mul(f1, f0);
    f1 = a1f::addFinite(f1, f3);
    { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 8, bits); } A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = SetCamFinalizerGeometry_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, h2, h6, a7c); a2 = a7c[0]; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (xo) { xo[0] = a1_bits(f1); xo[1] = a1_bits(f2); xo[2] = a1_bits(f3); xo[3] = a1_bits(f4); xo[4] = a1_bits(f5); xo[5] = a1_bits(f6); xo[6] = at; xo[7] = a0; xo[8] = a1; xo[9] = a2; xo[10] = hi; xo[11] = lo; } io_h2 = h2; io_h6 = h6; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SetStageManagerAreaPreloadWithAreaOffset (0x13661f0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f2 xo[1]=f3 xo[2]=f4 xo[3]=f5 xo[4]=f6; VU0 in -, out vf5 vf6 h2 h6, break)
inline a1::RetVF SetStageManagerAreaPreloadWithAreaOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2, uint64_t &io_h6, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(PauseCamFinalizer(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 25512))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f4, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 52))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f6, &bits, sizeof(bits)); }
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)s0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    if ((int64_t)v0 == 0) { A6_BREAK(0x136623Du); goto L_1366240; }
L_1366240:;
    { uint32_t bits = A1_R32B((uint32_t)v1, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    A1_W32B((uint32_t)sp, 4, 0u);
    f2 = a1f::mul(f2, f4);
    f5 = a1f::mul(f3, f4);
    (a0 = (uint64_t)(hi));
    (s0 = (uint64_t)(lo));
    { uint32_t bits = (uint32_t)a0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = (uint32_t)s0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f0 = a1f::sub(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::addFinite(f0, f5);
    f0 = (-(float)(f0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f6; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 12, bits); }
    f3 = a1f::mul(f0, f4);
    f2 = a1f::mul(f2, f4);
    f1 = a1f::sub(f1, f2);
    f1 = a1f::mul(f1, f0);
    f1 = a1f::addFinite(f1, f3);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 8, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x13662B4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { { const uint32_t _dynInsn = A1_R32(0x13662C0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); } A3_CHARGE(); v0 = SetCamFinalizerGeometry(ram, rt, (uint32_t)a0, (uint32_t)a1, h2, h6); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); if (xo) { xo[0] = a1_bits(f2); xo[1] = a1_bits(f3); xo[2] = a1_bits(f4); xo[3] = a1_bits(f5); xo[4] = a1_bits(f6); } io_vf5 = vf5; io_vf6 = vf6; io_h2 = h2; io_h6 = h6; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_0137FD70 (0x137fd70): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=a0 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1; VU0 in vf0 vf7 vf8 vf9 vacc vrr vstat h2 h8, out vf5 vf6 vf7 vf8 vf9 vacc vq vrr vstat h2, relocated calls 2)
inline a1::RetVF gamecore_sub_0137FD70(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vacc, float &io_vq, a4::vec4 &io_vrr, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    a4::vec4 vrr = io_vrr;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    sp = sp - 0x70u;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 48))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 104, bits); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 144))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 852); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::div(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s7, 0), a4v::castps_si128(vf5));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 128))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { A4_W128(ADD32((uint32_t)s2, 80), a4::mk128(h2, v0)); A3_CHARGE(); iosGetDRandomVector(ram, rt, (uint32_t)a0, vf0, vf5, vrr); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); iosGetDRandomVector(ram, rt, (uint32_t)a0, vf0, vf5, vrr); }
    A1_W32B(sp, 4, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15948 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 224))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf7));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 176))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    A1_W32B((uint32_t)s2, 68, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16204 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf9));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15820 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 68, bits); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f20 = a1f::div(fcr31, f20, f0);
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 112))));
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf0));
    { uint32_t bits = A1_R32B((uint32_t)s1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22096))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 160, bits); }
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 80))));
    (t1 = (uint64_t)((uint64_t)RC_U64X(s4) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s6) + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)v0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (a2 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 164, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::mul(f1, f0);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (t0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL)); if (c) goto L_137ff9c; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 160, bits); }
L_137ff9c:;
    { { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelC3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelA3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 16))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f12)}; { const a1::RetVF a2_r = AI_GetAccelB3XYZ_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); }
    { uint32_t bits = A1_R32B(sp, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { sp = sp + 0x70u; if (xo) { xo[0] = a1_bits(f12); xo[1] = a0; xo[2] = a1; xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vacc = vacc; io_vq = vq; io_vrr = vrr; io_vstat = vstat; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SpriteInitStatusClut (0x13997f0): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in -, out -, relocated calls 21, break)
inline uint64_t SpriteInitStatusClut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
    { (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    { (s4 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 3))); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    A1_W8B((uint32_t)v0, 3, (uint8_t)0u);
    A1_W8B((uint32_t)s0, 2, (uint8_t)0u);
    A1_W8B((uint32_t)s1, 1, (uint8_t)0u);
    { const bool c = (int64_t)s4 == 0; A1_W8B((uint32_t)s2, 0, (uint8_t)0u); if (c) goto L_1399a7c; }
L_1399890:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s4 == 0) { A6_BREAK(0x13998E1u); goto L_13998e4; }
L_13998e4:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s5; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    { int32_t divisor = (int32_t)s4;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { A1_W8B((uint32_t)s2, 0, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 1)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s4 == 0) { A6_BREAK(0x1399959u); goto L_139995c; }
L_139995c:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s5; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    { int32_t divisor = (int32_t)s4;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { A1_W8B((uint32_t)s2, 1, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s4 == 0) { A6_BREAK(0x13999D1u); goto L_13999d4; }
L_13999d4:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 2)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s5; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    { int32_t divisor = (int32_t)s4;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { A1_W8B((uint32_t)s2, 2, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 3)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 3)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s4 == 0) { A6_BREAK(0x1399A49u); goto L_1399a4c; }
L_1399a4c:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 3)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s5; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    (a0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int64_t)RC_U64X(s5)) ? 1 : 0));
    { int32_t divisor = (int32_t)s4;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { const bool c = (int64_t)a0 == 0; A1_W8B((uint32_t)s2, 3, (uint8_t)(uint32_t)v0); if (c) { A3_BACK(); goto L_1399890; } }
L_1399a7c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); return v0; }
    __builtin_unreachable();
}

// SpriteSetStatusClut (0x1399aa0): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in -, out -, relocated calls 41, break)
inline a1::RetVF SpriteSetStatusClut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1)))); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (s5 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 3))); if (c) goto L_1399b28; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15395 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)55050));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1f::sub(f1, f0);
    fcr31 = (a1f::le(f1, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 8, bits); } if (c) goto L_1399b28; }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 8, bits); }
    (fp = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    f1 = ((float)(f3));
L_1399b28:;
    fcr31 = (a1f::lt(f1, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1399b5c;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15395 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)55050));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1f::add(f1, f0);
    fcr31 = (a1f::le(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 8, bits); } if (c) goto L_1399b5c; }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 8, bits); }
    (fp = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    f1 = ((float)(f3));
L_1399b5c:;
    { uint32_t bits = (uint32_t)s5; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16320 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f1 = a1f::mul(f1, f0);
    f0 = a1f::mul(f3, f0);
    f1 = a1f::addFinite(f1, f2);
    f0 = a1f::addFinite(f0, f2);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f2, sizeof(bits)); (s6 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (s7 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (s4 = (uint64_t)((uint64_t)RC_U64X(s6) + (uint64_t)0ULL));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int64_t)RC_U64X(s4)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s7) < (int64_t)(int64_t)RC_U64X(s6)) ? 1 : 0)); if (c) goto L_1399c60; }
L_1399ba0:;
    { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 254))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { A1_W8B((uint32_t)s0, 0, (uint8_t)(uint32_t)v1); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 254))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 1)));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { A1_W8B((uint32_t)s0, 1, (uint8_t)(uint32_t)v1); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 254))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2)));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
    { A1_W8B((uint32_t)s0, 2, (uint8_t)(uint32_t)v1); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 254))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 3)));
    (a0 = (uint64_t)(((int64_t)(int64_t)s5 < (int64_t)(int64_t)s4) ? 1 : 0));
    { const bool c = (int64_t)a0 == 0; A1_W8B((uint32_t)s0, 3, (uint8_t)(uint32_t)v1); if (c) { A3_BACK(); goto L_1399ba0; } }
    (v0 = (uint64_t)(((int64_t)(int64_t)s7 < (int64_t)(int64_t)s6) ? 1 : 0));
L_1399c60:;
    { const bool c = (int64_t)v0 == 0; (s4 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); if (c) goto L_1399e60; }
L_1399c68:;
    { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 0)));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399CD1u); goto L_1399cd4; }
L_1399cd4:;
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    { A1_W8B((uint32_t)s2, 0, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 1)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 1)));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399D4Du); goto L_1399d50; }
L_1399d50:;
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    { A1_W8B((uint32_t)s2, 1, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 2)));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399DC9u); goto L_1399dcc; }
L_1399dcc:;
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    { A1_W8B((uint32_t)s2, 2, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 3)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 3)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399E2Du); goto L_1399e30; }
L_1399e30:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 3)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s4) < (int64_t)(int64_t)s6) ? 1 : 0));
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { const bool c = (int64_t)a0 != 0; A1_W8B((uint32_t)s2, 3, (uint8_t)(uint32_t)v0); if (c) { A3_BACK(); goto L_1399c68; } }
L_1399e60:;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s4) < (int64_t)(int64_t)s7) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); if (c) goto L_139a060; }
L_1399e70:;
    { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399EC1u); goto L_1399ec4; }
L_1399ec4:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { A1_W8B((uint32_t)s2, 0, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 1)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399F39u); goto L_1399f3c; }
L_1399f3c:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { A1_W8B((uint32_t)s2, 1, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x1399FB1u); goto L_1399fb4; }
L_1399fb4:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 2)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { A1_W8B((uint32_t)s2, 2, (uint8_t)(uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 253))));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(texGetCSM1Idx(ram, rt, (uint32_t)a0)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s3))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 3)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 3)));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    if ((int64_t)s5 == 0) { A6_BREAK(0x139A029u); goto L_139a02c; }
L_139a02c:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s1, 3)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)s4; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s3))));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s4) < (int64_t)(int64_t)s7) ? 1 : 0));
    { int32_t divisor = (int32_t)s5;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)(lo));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { const bool c = (int64_t)a0 != 0; A1_W8B((uint32_t)s2, 3, (uint8_t)(uint32_t)v0); if (c) { A3_BACK(); goto L_1399e70; } }
    (v0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
L_139a060:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// setEventLineAttr (0x1432950): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a0 xo[1]=a1; VU0 in -, out -, relocated calls 1, frame 64 bytes, 0 slot accesses)
inline uint64_t setEventLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo)
{
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 144))));
    if ((int32_t)v1 <= 0) goto L_1432a14;
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s4, 8))));
L_1432998:;
    s0 = A1_R32(s2);
    if (s0 == 0) { (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1)))); goto L_1432a04; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1432a00; }
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); a3xw1 = true;
L_14329b8:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); v0 = getLineData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 16))));
    if ((int64_t)s3 == (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8)))); goto L_14329e4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 18))));
    if ((int64_t)s3 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_14329f0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
L_14329e4:;
    (v0 = (uint64_t)(RC_U64X(v0) | s5));
    A1_W32B((uint32_t)v1, 8, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
L_14329f0:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw1 = true; if (c) { A3_BACK(); goto L_14329b8; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 144))));
L_1432a00:;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
L_1432a04:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s6 < (int64_t)(int64_t)v1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s2 = s2 + 0x24u; if (c) { A3_BACK(); goto L_1432998; } }
L_1432a14:;
    { if (xo) { if (a3xw0) xo[0] = a0; if (a3xw1) xo[1] = a1; } return v0; }
    __builtin_unreachable();
}

// unsetEventLineAttr (0x1432a38): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in -, out -, relocated calls 1, frame 80 bytes, 0 slot accesses)
inline uint64_t unsetEventLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s7 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    (s5 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 144))));
    if ((int32_t)v1 <= 0) goto L_1432b04;
    (s3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s5, 8))));
L_1432a88:;
    s0 = A1_R32(s3);
    if (s0 == 0) { (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1)))); goto L_1432af4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1432af0; }
    (s2 = (uint64_t)(~(0ULL | s7)));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
L_1432aa8:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = getLineData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 16))));
    if ((int64_t)s4 == (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8)))); goto L_1432ad4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 18))));
    if ((int64_t)s4 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_1432ae0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
L_1432ad4:;
    (v0 = (uint64_t)(RC_U64X(v0) & s2));
    A1_W32B((uint32_t)v1, 8, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
L_1432ae0:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1432aa8; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 144))));
L_1432af0:;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
L_1432af4:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s6 < (int64_t)(int64_t)v1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s3 = s3 + 0x24u; if (c) { A3_BACK(); goto L_1432a88; } }
L_1432b04:;
    { return v0; }
    __builtin_unreachable();
}

// resetEventLineAttr (0x1432b30): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in -, out -, relocated calls 1, frame 64 bytes, 0 slot accesses)
inline uint64_t resetEventLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1)
{
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 144))));
    if ((int32_t)v1 <= 0) goto L_1432be8;
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s4, 8))));
L_1432b70:;
    s0 = A1_R32(s2);
    if (s0 == 0) { (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1)))); goto L_1432bd8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1432bd4; }
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
L_1432b90:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = getLineData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 16))));
    if ((int64_t)s3 == (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); goto L_1432bbc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 18))));
    if ((int64_t)s3 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_1432bc4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
L_1432bbc:;
    A1_W32B((uint32_t)v1, 8, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
L_1432bc4:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1432b90; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 144))));
L_1432bd4:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
L_1432bd8:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s5 < (int64_t)(int64_t)v1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s2 = s2 + 0x24u; if (c) { A3_BACK(); goto L_1432b70; } }
L_1432be8:;
    { return v0; }
    __builtin_unreachable();
}

// allResetLineAttr (0x1432c08): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a0; VU0 in -, out -, relocated calls 1, frame 48 bytes, 0 slot accesses)
inline uint64_t allResetLineAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t a0, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    bool a3xw0 = false;
    { (s4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, a0)); }
    (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 144))));
    if ((int32_t)v1 <= 0) goto L_1432ca4;
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s3, 8))));
L_1432c40:;
    s0 = A1_R32(s2);
    if (s0 == 0) { (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1)))); goto L_1432c94; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1432c90; }
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
L_1432c60:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    { a0 = s0; a3xw0 = true; A3_CHARGE(); v0 = getLineData(ram, rt, a1_sx64(a0), a1); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    (a0 = (uint32_t)((uint16_t)A1_R16B((uint32_t)v0, 12))); a3xw0 = true;
    A1_W32B((uint32_t)v0, 8, v1);
    A1_W16B((uint32_t)v0, 14, (uint16_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1432c60; } }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 144))));
L_1432c90:;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
L_1432c94:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int64_t)a1_sx64(v1)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s2 = s2 + 0x24u; if (c) { A3_BACK(); goto L_1432c40; } }
L_1432ca4:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// setEventLinePrio (0x1432cc0): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a0; VU0 in -, out -, relocated calls 1, frame 64 bytes, 0 slot accesses)
inline uint64_t setEventLinePrio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo)
{
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    bool a3xw0 = false;
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 144))));
    if ((int32_t)v1 <= 0) goto L_1432d7c;
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s4, 8))));
L_1432d08:;
    s0 = A1_R32(s2);
    if (s0 == 0) { (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1)))); goto L_1432d6c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1432d68; }
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
L_1432d28:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); v0 = getLineData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 16))));
    if ((int64_t)s3 == (int64_t)v0) { A1_W16B((uint32_t)v1, 14, (uint16_t)(uint32_t)s5); goto L_1432d54; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 18))));
    if ((int64_t)s3 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_1432d58; }
    A1_W16B((uint32_t)v1, 14, (uint16_t)(uint32_t)s5);
L_1432d54:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
L_1432d58:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1432d28; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 144))));
L_1432d68:;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
L_1432d6c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s6 < (int64_t)(int64_t)v1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s2 = s2 + 0x24u; if (c) { A3_BACK(); goto L_1432d08; } }
L_1432d7c:;
    { if (xo) { if (a3xw0) xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// getPointDataFromEvent (0x14338e8): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in -, out -, relocated calls 1, frame 64 bytes, 0 slot accesses)
inline uint64_t getPointDataFromEvent(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    { const bool c = (int64_t)s4 == 0; (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_1433928; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    A1_W32B((uint32_t)s4, 0, (uint32_t)v0);
L_1433928:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 144))));
    { const bool c = (int32_t)v1 <= 0; (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_14339ac; }
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s3, 4))));
L_1433938:;
    s0 = A1_R32(s2);
    if (s0 == 0) { (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1)))); goto L_14339a0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; s1 = 0u; if (c) goto L_143399c; }
    (a1 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL));
L_1433958:;
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    if ((int64_t)v1 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_1433988; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 28))));
    if ((int64_t)s6 != (int64_t)v1) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_1433988; }
    if ((int64_t)s4 != 0) { A1_W32B((uint32_t)s4, 0, s1); goto L_14339b0; }
    goto L_14339b4;
L_1433988:;
    s1 = s1 + 1u;
    (v0 = (uint64_t)(((uint64_t)a1_sx64(s1) < (uint64_t)v0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1433958; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 144))));
L_143399c:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
L_14339a0:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s5 < (int64_t)(int64_t)v1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s2 = s2 + 0x24u; if (c) { A3_BACK(); goto L_1433938; } }
L_14339ac:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14339b0:;
L_14339b4:;
    { return v0; }
    __builtin_unreachable();
}

// getPointPosFromEvent (0x14339d8): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in -, out h3, relocated calls 1, frame 64 bytes, 0 slot accesses)
inline uint64_t getPointPosFromEvent(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h3)
{
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t h3 = io_h3;
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 144))));
    { const bool c = (int32_t)v1 <= 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1433a8c; }
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s3, 4))));
L_1433a20:;
    s0 = A1_R32(s2);
    if (s0 == 0) { (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1)))); goto L_1433a7c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1433a78; }
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
L_1433a40:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    if ((int64_t)v1 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_1433a68; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 28))));
    { const bool c = (int64_t)s4 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL)); if (c) goto L_1433a8c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
L_1433a68:;
    (v0 = (uint64_t)(((uint64_t)s1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1433a40; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 144))));
L_1433a78:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
L_1433a7c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s5 < (int64_t)(int64_t)v1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; s2 = s2 + 0x24u; if (c) { A3_BACK(); goto L_1433a20; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1433a8c:;
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1433aa0; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A4_W128(ADD32((uint32_t)s6, 0), a4::mk128(h3, v1));
L_1433aa0:;
    { io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// getRootPointIdxForGroupID (0x1433ac8): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a3 xo[1]=t0; VU0 in -, out -, relocated calls 2, frame 64 bytes, 0 slot accesses)
inline uint64_t getRootPointIdxForGroupID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 15440))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    s3 = 0x1200000u;
    (s3 = (uint32_t)((int32_t)((int32_t)ADD32(s3, 4296))));
    (s3 = (uint32_t)((int32_t)((int32_t)ADD32(s3, (uint32_t)v0))));
    { const bool c = (int32_t)a2 >= 0; (s6 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (c) goto L_1433ba0; }
    v1 = A1_R32B(s3, 144);
    { const bool c = (int32_t)v1 <= 0; (s4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1433c0c; }
    (s1 = (uint64_t)((uint64_t)a1_sx64(s3) + (uint64_t)0ULL));
L_1433b30:;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 12))));
    { const bool c = a0 == 0; (s2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1433b88; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    { const bool c = (int64_t)v0 == 0; (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1433b88; }
L_1433b48:;
    { (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getRootPointData(ram, rt, a1_sx64(a0), a1); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    if ((int64_t)s5 != (int64_t)(int32_t)v1) { (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 12)))); goto L_1433b70; }
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    v1 = A1_R32B(a0, 28);
    goto L_1433c14;
L_1433b70:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(s0) < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    if ((int64_t)v0 != 0) { A3_BACK(); goto L_1433b48; }
    v1 = A1_R32B(s3, 144);
L_1433b88:;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s4) < (int64_t)(int64_t)a1_sx64(v1)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 36)))); if (c) { A3_BACK(); goto L_1433b30; } }
    v1 = 0xFFFFFFFFu;
    goto L_1433c10;
L_1433ba0:;
    { (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[2] = {0u, t0}; v0 = getPathDataAddr_x14(ram, rt, a0, a1, a7c); a3 = a7c[0]; t0 = a7c[1]; a3xw0 = true; a3xw1 = true; }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)s1 == 0; v1 = 0xFFFFFFFFu; if (c) goto L_1433c10; }
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 12))));
    { const bool c = a0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1433c14; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    { const bool c = (int64_t)v0 == 0; (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1433c10; }
L_1433bd0:;
    { (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getRootPointData(ram, rt, a1_sx64(a0), a1); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    if ((int64_t)s5 != (int64_t)(int32_t)v1) { (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 12)))); goto L_1433bf8; }
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    v1 = A1_R32B(a0, 28);
    goto L_1433c14;
L_1433bf8:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(s0) < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    if ((int64_t)v0 != 0) { A3_BACK(); goto L_1433bd0; }
L_1433c0c:;
    v1 = 0xFFFFFFFFu;
L_1433c10:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1433c14:;
    A1_W32B((uint32_t)s6, 0, v1);
    { if (xo) { if (a3xw0) xo[0] = a3; if (a3xw1) xo[1] = t0; } return v0; }
    __builtin_unreachable();
}

// iosPushVu0Registers (0x1b8ed0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1; VU0 in vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf19 vf20 vf21 vf22 vf23 vf24 vf25 vf26 vf27 vf28 vf29 vf30 vf31 c0st, out c0st h2, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t iosPushVu0Registers(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, const a4::vec4 &io_vf5, const a4::vec4 &io_vf6, const a4::vec4 &io_vf7, const a4::vec4 &io_vf8, const a4::vec4 &io_vf9, const a4::vec4 &io_vf10, const a4::vec4 &io_vf11, const a4::vec4 &io_vf12, const a4::vec4 &io_vf13, const a4::vec4 &io_vf14, const a4::vec4 &io_vf15, const a4::vec4 &io_vf16, const a4::vec4 &io_vf17, const a4::vec4 &io_vf18, const a4::vec4 &io_vf19, const a4::vec4 &io_vf20, const a4::vec4 &io_vf21, const a4::vec4 &io_vf22, const a4::vec4 &io_vf23, const a4::vec4 &io_vf24, const a4::vec4 &io_vf25, const a4::vec4 &io_vf26, const a4::vec4 &io_vf27, const a4::vec4 &io_vf28, const a4::vec4 &io_vf29, const a4::vec4 &io_vf30, const a4::vec4 &io_vf31, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf19 = io_vf19;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vf22 = io_vf22;
    a4::vec4 vf23 = io_vf23;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    { A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = iosDI(ram, rt, v0, a2, a3, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFF0u), f12, f13, f14, f15, f16, f17, f18, f19, c0st, h2, a7c); a0 = a7c[0]; a1 = a7c[1]; }
    v1 = 0x200000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 10416))));
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    A4_W128(ADD32(v1, 16), a4v::castps_si128(vf6));
    A4_W128(ADD32(v1, 32), a4v::castps_si128(vf7));
    A4_W128(ADD32(v1, 48), a4v::castps_si128(vf1));
    A4_W128(ADD32(v1, 64), a4v::castps_si128(vf2));
    A4_W128(ADD32(v1, 80), a4v::castps_si128(vf3));
    A4_W128(ADD32(v1, 96), a4v::castps_si128(vf4));
    A4_W128(ADD32(v1, 112), a4v::castps_si128(vf8));
    A4_W128(ADD32(v1, 128), a4v::castps_si128(vf9));
    A4_W128(ADD32(v1, 144), a4v::castps_si128(vf10));
    A4_W128(ADD32(v1, 160), a4v::castps_si128(vf11));
    A4_W128(ADD32(v1, 176), a4v::castps_si128(vf12));
    A4_W128(ADD32(v1, 192), a4v::castps_si128(vf13));
    A4_W128(ADD32(v1, 208), a4v::castps_si128(vf14));
    A4_W128(ADD32(v1, 224), a4v::castps_si128(vf15));
    A4_W128(ADD32(v1, 240), a4v::castps_si128(vf16));
    A4_W128(ADD32(v1, 256), a4v::castps_si128(vf17));
    A4_W128(ADD32(v1, 272), a4v::castps_si128(vf18));
    A4_W128(ADD32(v1, 288), a4v::castps_si128(vf19));
    A4_W128(ADD32(v1, 304), a4v::castps_si128(vf20));
    A4_W128(ADD32(v1, 320), a4v::castps_si128(vf21));
    A4_W128(ADD32(v1, 336), a4v::castps_si128(vf22));
    A4_W128(ADD32(v1, 352), a4v::castps_si128(vf23));
    A4_W128(ADD32(v1, 368), a4v::castps_si128(vf24));
    A4_W128(ADD32(v1, 384), a4v::castps_si128(vf25));
    A4_W128(ADD32(v1, 400), a4v::castps_si128(vf26));
    A4_W128(ADD32(v1, 416), a4v::castps_si128(vf27));
    A4_W128(ADD32(v1, 432), a4v::castps_si128(vf28));
    A4_W128(ADD32(v1, 448), a4v::castps_si128(vf29));
    A4_W128(ADD32(v1, 464), a4v::castps_si128(vf30));
    A4_W128(ADD32(v1, 480), a4v::castps_si128(vf31));
    v1 = v1 - 0x200u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf1));
    A4_W128(ADD32(v1, 16), a4v::castps_si128(vf2));
    A4_W128(ADD32(v1, 32), a4v::castps_si128(vf3));
    A4_W128(ADD32(v1, 48), a4v::castps_si128(vf4));
    A4_W128(ADD32(v1, 64), a4v::castps_si128(vf5));
    A4_W128(ADD32(v1, 80), a4v::castps_si128(vf6));
    A4_W128(ADD32(v1, 96), a4v::castps_si128(vf7));
    A4_W128(ADD32(v1, 112), a4v::castps_si128(vf8));
    A4_W128(ADD32(v1, 128), a4v::castps_si128(vf9));
    A4_W128(ADD32(v1, 144), a4v::castps_si128(vf10));
    A4_W128(ADD32(v1, 160), a4v::castps_si128(vf11));
    A4_W128(ADD32(v1, 176), a4v::castps_si128(vf12));
    A4_W128(ADD32(v1, 192), a4v::castps_si128(vf13));
    A4_W128(ADD32(v1, 208), a4v::castps_si128(vf14));
    A4_W128(ADD32(v1, 224), a4v::castps_si128(vf15));
    A4_W128(ADD32(v1, 240), a4v::castps_si128(vf16));
    A4_W128(ADD32(v1, 256), a4v::castps_si128(vf17));
    A4_W128(ADD32(v1, 272), a4v::castps_si128(vf18));
    A4_W128(ADD32(v1, 288), a4v::castps_si128(vf19));
    A4_W128(ADD32(v1, 304), a4v::castps_si128(vf20));
    A4_W128(ADD32(v1, 320), a4v::castps_si128(vf21));
    A4_W128(ADD32(v1, 336), a4v::castps_si128(vf22));
    A4_W128(ADD32(v1, 352), a4v::castps_si128(vf23));
    A4_W128(ADD32(v1, 368), a4v::castps_si128(vf24));
    A4_W128(ADD32(v1, 384), a4v::castps_si128(vf25));
    A4_W128(ADD32(v1, 400), a4v::castps_si128(vf26));
    A4_W128(ADD32(v1, 416), a4v::castps_si128(vf27));
    A4_W128(ADD32(v1, 432), a4v::castps_si128(vf28));
    A4_W128(ADD32(v1, 448), a4v::castps_si128(vf29));
    A4_W128(ADD32(v1, 464), a4v::castps_si128(vf30));
    A4_W128(ADD32(v1, 480), a4v::castps_si128(vf31));
    { uint64_t a7c[1] = {a0}; const uint64_t a7_t = iosEI(ram, rt, (uint32_t)a1, a2, a3, t0, t1, t2, t3, (uint32_t)(sp + 0x0u), f12, f13, f14, f15, f16, f17, f18, f19, c0st, h2, a7c); a0 = a7c[0]; if (xo) { xo[0] = a0; xo[1] = a1; } io_c0st = c0st; io_h2 = h2; return a7_t; }
    __builtin_unreachable();
}

// iosPopVu0Registers (0x1b8ff0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1; VU0 in c0st, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf19 vf20 vf21 vf22 vf23 vf24 vf25 vf26 vf27 vf28 vf29 vf30 vf31 c0st h2, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t iosPopVu0Registers(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf19, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vf22, a4::vec4 &io_vf23, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, uint32_t &io_c0st, uint64_t &io_h2, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf19 = io_vf19;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vf22 = io_vf22;
    a4::vec4 vf23 = io_vf23;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    uint32_t c0st = io_c0st;
    uint64_t h2 = io_h2;
    { A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = iosDI(ram, rt, v0, a2, a3, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFF0u), f12, f13, f14, f15, f16, f17, f18, f19, c0st, h2, a7c); a0 = a7c[0]; a1 = a7c[1]; }
    v1 = 0x200000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 10416))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf1 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32(v1, 64)));
    vf3 = a4v::castsi128_ps(A4_R128(ADD32(v1, 80)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(v1, 96)));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(v1, 112)));
    vf9 = a4v::castsi128_ps(A4_R128(ADD32(v1, 128)));
    vf10 = a4v::castsi128_ps(A4_R128(ADD32(v1, 144)));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v1, 160)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(v1, 176)));
    vf13 = a4v::castsi128_ps(A4_R128(ADD32(v1, 192)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 208)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v1, 224)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v1, 240)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v1, 256)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32(v1, 272)));
    vf19 = a4v::castsi128_ps(A4_R128(ADD32(v1, 288)));
    vf20 = a4v::castsi128_ps(A4_R128(ADD32(v1, 304)));
    vf21 = a4v::castsi128_ps(A4_R128(ADD32(v1, 320)));
    vf22 = a4v::castsi128_ps(A4_R128(ADD32(v1, 336)));
    vf23 = a4v::castsi128_ps(A4_R128(ADD32(v1, 352)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(v1, 368)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(v1, 384)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(v1, 400)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(v1, 416)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32(v1, 432)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(v1, 448)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 464)));
    vf31 = a4v::castsi128_ps(A4_R128(ADD32(v1, 480)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 9904))));
    vf1 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf3 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 64)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 80)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 96)));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 112)));
    vf9 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 128)));
    vf10 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 144)));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 160)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 176)));
    vf13 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 192)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 208)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 224)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 240)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 256)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 272)));
    vf19 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 288)));
    vf20 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 304)));
    vf21 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 320)));
    vf22 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 336)));
    vf23 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 352)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 368)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 384)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 400)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 416)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 432)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 448)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 464)));
    vf31 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 480)));
    { uint64_t a7c[1] = {a0}; const uint64_t a7_t = iosEI(ram, rt, (uint32_t)a1, a2, a3, t0, t1, t2, t3, (uint32_t)(sp + 0x0u), f12, f13, f14, f15, f16, f17, f18, f19, c0st, h2, a7c); a0 = a7c[0]; if (xo) { xo[0] = a0; xo[1] = a1; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf19 = vf19; io_vf20 = vf20; io_vf21 = vf21; io_vf22 = vf22; io_vf23 = vf23; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_c0st = c0st; io_h2 = h2; return a7_t; }
    __builtin_unreachable();
}

// Sg2DmaTransVoice (0x1cc2d0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in c0st h4, out c0st h3, frame 16 bytes, 0 slot accesses)
inline uint64_t Sg2DmaTransVoice(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t &io_c0st, uint64_t &io_h3, uint64_t io_h4, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 19))));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; const uint64_t a7_t = Sg2RemoteSendDmaCom(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, (uint32_t)a3, c0st, h3, h4, a7c); a1 = a7c[0]; a2 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; t3 = a7c[5]; if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } io_c0st = c0st; io_h3 = h3; return a7_t; }
    __builtin_unreachable();
}

// Sg2ExStAdpcmTrkDspSw (0x1cd2b8): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in c0st, out c0st)
inline uint64_t Sg2ExStAdpcmTrkDspSw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t c0st = io_c0st;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)16) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1cd3ac; }
    { const bool c = (int64_t)a2 != (int64_t)v0; (v1 = (uint64_t)(A1_R64B((uint32_t)a0, 160))); if (c) goto L_1cd2e4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 128))));
L_1cd2e4:;
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1cd2f0:;
    (a3 = (uint64_t)(t0 << (32 + 0)));
    (a3 = (uint64_t)((int64_t)a3 >> (32 + 0)));
    (v0 = (uint64_t)(t1 << ((uint32_t)a3 & 0x3F)));
    (v0 = (uint64_t)(v1 & v0));
    if ((int64_t)v0 == 0) { (t0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1)); goto L_1cd3a0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    if ((int32_t)a1 >= 0) { (t0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1)); goto L_1cd3a0; }
    if ((int64_t)a2 == 0) { (v1 = (uint64_t)(((uint64_t)t0 < (uint64_t)(int64_t)(int32_t)24) ? 1 : 0)); goto L_1cd330; }
    { const bool c = (int64_t)a2 == (int64_t)t1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1cd368; }
    goto L_1cd3b0;
L_1cd330:;
    (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)1));
    (a0 = (uint64_t)(v0 << (32 + 0)));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    { const bool c = (int64_t)v1 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)a3 & 0x1F)))); if (c) goto L_1cd354; }
    (v0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)4294967272));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, (uint32_t)v0 & 0x1F))));
L_1cd354:;
    { A3_CHARGE(); v0 = Sg2SlotSetReverbOffBit(ram, rt, v0, a0, a1, c0st); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1cd3ac;
L_1cd368:;
    (v1 = (uint64_t)(((uint64_t)t0 < (uint64_t)(int64_t)(int32_t)24) ? 1 : 0));
    (v0 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1));
    (a0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    { const bool c = (int64_t)v1 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, (uint32_t)a3 & 0x1F)))); if (c) goto L_1cd390; }
    (v0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)4294967272));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, (uint32_t)v0 & 0x1F))));
L_1cd390:;
    { A3_CHARGE(); v0 = Sg2SlotSetReverbOnBit(ram, rt, v0, a0, a1, c0st); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1cd3ac;
L_1cd3a0:;
    (v0 = (uint64_t)(((uint64_t)t0 < (uint64_t)(int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) { A3_BACK(); goto L_1cd2f0; } }
L_1cd3ac:;
L_1cd3b0:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// all_keyoff (0x1ce068): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in c0st, out c0st)
inline uint64_t all_keyoff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t sp, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t c0st = io_c0st;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    sp = sp - 0xE0u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    a1 = 0x270000u;
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, (uint32_t)v0))));
    a1 = A1_R32B(a1, 16092);
    { (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); v0 = Sg2SlotFindSlotClass(ram, rt, (uint32_t)a0, a1_sx64(a1)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { const bool c = (int32_t)s0 < 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2)))); if (c) goto L_1ce0ec; }
    s2 = 1u;
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, sp))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 0))));
L_1ce0c0:;
    s1 = s1 - 4u;
    (v1 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)24) ? 1 : 0));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294967272))));
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32(s2, (uint32_t)v0 & 0x1F))));
    { const bool c = (int64_t)v1 != 0; (a0 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1)); if (c) goto L_1ce0dc; }
    a1 = s2 << (a2 & 31);
L_1ce0dc:;
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295)))); A3_CHARGE(); v0 = Sg2SlotSetKeyOffBit(ram, rt, v0, a0, a1_sx64(a1), c0st); }
    if ((int32_t)s0 >= 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 0)))); { A3_BACK(); goto L_1ce0c0; } }
L_1ce0ec:;
    { sp = sp + 0xE0u; io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001CFB28 (0x1cfb28): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st h3, frame 16 bytes, 0 slot accesses)
inline uint64_t kernel_sub_001CFB28(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    v0 = 0x270000u;
    v1 = 0x270000u;
    a3 = A1_R32B(v0, 16076);
    a2 = A1_R32B(v1, 16080);
    a1 = A1_R8(a3);
    a0 = A1_R32B(a2, 4);
    a1 = a1 & 0xFu;
    a2 = A1_R8B(a3, 1);
    { const uint64_t a4_t = Sg2SlotModKeyOff(ram, rt, a1_sx64(v0), a1_sx64(a0), a1_sx64(a1), a1_sx64(a2), c0st, h3); io_c0st = c0st; io_h3 = h3; return a4_t; }
    __builtin_unreachable();
}

// kernel_sub_001D1360 (0x1d1360): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st h3, frame 16 bytes, 0 slot accesses)
inline uint64_t kernel_sub_001D1360(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h3 = io_h3;
    v0 = 0x270000u;
    a0 = A1_R32B(v0, 16076);
    v1 = A1_R8B(a0, 2);
    v0 = A1_R8(a0);
    v1 = v1 < 0x7Eu ? 1u : 0u;
    { const bool c = v1 == 0; a3 = v0 & 0xFu; if (c) goto L_1d13b8; }
    v0 = 0x270000u;
    v1 = a3 << 3;
    a2 = A1_R32B(v0, 16080);
    v1 = v1 - a3;
    v1 = v1 << 4;
    v1 = v1 + a2;
    (a1 = (uint64_t)((uint64_t)a1_sx64(a3) + (uint64_t)0ULL));
    A1_W32B(v1, 736, 0u);
    a0 = A1_R32B(a2, 4);
    { const uint64_t a4_t = Sg2SlotSetDumperOff(ram, rt, a1_sx64(v0), a1_sx64(a0), a1, c0st, h3); io_c0st = c0st; io_h3 = h3; return a4_t; }
L_1d13b8:;
    v1 = 0x270000u;
    v0 = a3 << 3;
    a0 = A1_R32B(v1, 16080);
    v0 = v0 - a3;
    v0 = v0 << 4;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    v0 = v0 + a0;
    A1_W32B(v0, 736, (uint32_t)a1);
    { io_c0st = c0st; io_h3 = h3; return a1_sx64(v0); }
    __builtin_unreachable();
}

// kernel_sub_001D1778 (0x1d1778): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in c0st, out c0st)
inline uint64_t kernel_sub_001D1778(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t c0st = io_c0st;
    sp = sp - 0xE0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 16076))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16080))));
    a2 = A1_R8(a1);
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    { a2 = a2 & 0xFu; A3_CHARGE(); v0 = Sg2SlotFindSlotClassCh(ram, rt, (uint32_t)a0, a1_sx64(a1), a1_sx64(a2)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { const bool c = (int32_t)s0 < 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2)))); if (c) goto L_1d17f4; }
    s2 = 1u;
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, sp))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 0))));
L_1d17c8:;
    s1 = s1 - 4u;
    (v1 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)24) ? 1 : 0));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294967272))));
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32(s2, (uint32_t)v0 & 0x1F))));
    { const bool c = (int64_t)v1 != 0; (a0 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1)); if (c) goto L_1d17e4; }
    a1 = s2 << (a2 & 31);
L_1d17e4:;
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295)))); A3_CHARGE(); v0 = Sg2SlotSetKeyOffBit(ram, rt, v0, a0, a1_sx64(a1), c0st); }
    if ((int32_t)s0 >= 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 0)))); { A3_BACK(); goto L_1d17c8; } }
L_1d17f4:;
    { sp = sp + 0xE0u; io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SepStopSndOff (0x1d28a0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t Sg2SepStopSndOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t &io_c0st, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t s3 = 0;
    uint32_t c0st = io_c0st;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    if ((int64_t)v1 == 0) goto L_1d2938;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    s0 = s0 << 4;
    (s3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294949312))));
    s2 = s0 + s3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 0))));
    if ((int32_t)v0 <= 0) { goto L_1d293c; }
    { s0 = s0 + 0x10u; A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    A1_W32B(s2, 12, (uint32_t)v0);
    A1_W32B(s2, 0, (uint32_t)v1);
    s0 = s0 + s3;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255)))); a3xw0 = true;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); A3_CHARGE(); v0 = Sg2ModMidiout1(ram, rt, v0, a0, (uint32_t)a1, c0st); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 65)))); a3xw0 = true; A3_CHARGE(); v0 = Sg2ModMidiout1(ram, rt, v0, a0, (uint32_t)a1, c0st); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 247)))); a3xw0 = true; A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = Sg2ModMidiout1(ram, rt, v0, a0, (uint32_t)a1, c0st, a7c); a1 = a7c[0]; a3xw0 = true; }
    if ((int64_t)s1 == 0) { goto L_1d293c; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1d2938:;
L_1d293c:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { if (xo) { if (a3xw0) xo[0] = a1; } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetBasicParam (0x1d6118): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=t2 xo[1]=t3 xo[2]=t6; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t Sg2SlotSetBasicParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t &io_c0st, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t t8 = 0;
    uint32_t c0st = io_c0st;
    t3 = 0x290000u;
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s2, 3))));
    t3 = t3 - 0x39E0u;
    (s3 = (uint64_t)(((int64_t)(int64_t)s2 < (int64_t)(int32_t)24) ? 1 : 0));
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(v1, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    v1 = v1 << 5;
    t7 = A1_R32(t3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    t2 = v1 + 0x10u;
    (t4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    s1 = v1 + t4;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(t2, (uint32_t)a1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)a1))));
    (t5 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    t4 = t2 + t4;
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32(t2, (uint32_t)v0))));
    t8 = 1u;
    t6 = t7 + 1u;
    (v0 = (uint64_t)(RC_U64X(s3) ^ (uint64_t)(uint16_t)1));
    A1_W32(t3, t6);
    A1_W32(t5, t8);
    (t3 = (uint32_t)((int32_t)((int32_t)SLL32(t8, (uint32_t)s2 & 0x1F))));
    A1_W32B((uint32_t)a1, 0, (uint32_t)a0);
    (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    A1_W32B(v1, 12, a2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967272))));
    A1_W32(t2, a3);
    A1_W32(s0, t0);
    A1_W32(t4, t1);
    { const bool c = (int64_t)s3 != 0; A1_W32(s1, t7); if (c) goto L_1d61c0; }
    (t3 = (uint32_t)((int32_t)((int32_t)SLL32(t8, (uint32_t)v0 & 0x1F))));
L_1d61c0:;
    { (a1 = (uint64_t)((uint64_t)a1_sx64(t3) + (uint64_t)0ULL)); A3_CHARGE(); v0 = Sg2SlotSetReverbOffBit(ram, rt, v0, a0, a1, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 4294967272))));
    { const bool c = (int64_t)s3 == 0; (a0 = (uint64_t)(s3 ^ (uint64_t)(uint16_t)1)); if (c) goto L_1d61e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, (uint32_t)s2 & 0x1F))));
    goto L_1d61e8;
L_1d61e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, v1 & 0x1F))));
L_1d61e8:;
    { if (xo) { xo[0] = a1_sx64(t2); xo[1] = a1_sx64(t3); xo[2] = a1_sx64(t6); } const uint64_t a4_t = Sg2SlotSetNoiseOffBit(ram, rt, v0, a0, a1, c0st); io_c0st = c0st; return a4_t; }
    __builtin_unreachable();
}

// kernel_sub_001DAC50 (0x1dac50): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f7 xo[8]=f8 xo[9]=f9 xo[10]=f10 xo[11]=f11 xo[12]=f12 xo[13]=f13 xo[14]=f14 xo[15]=a3 xo[16]=t0 xo[17]=t1 xo[18]=t2 xo[19]=t3; VU0 in h2 h3 h8, out hi1 lo1 h2 h5 h6 h10, break)
inline uint64_t kernel_sub_001DAC50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h5, uint64_t &io_h6, uint64_t io_h8, uint64_t &io_h10, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    uint64_t h10 = io_h10;
    bool a3xw15 = false;
    bool a3xw16 = false;
    bool a3xw17 = false;
    bool a3xw18 = false;
    bool a3xw19 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 416))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)2));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 16)))); if (c) goto L_1daca0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(a0)) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420)))); goto L_1daca4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
    A1_W32B((uint32_t)s0, 0, (uint32_t)v0);
L_1daca0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420))));
L_1daca4:;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1dacc0; }
    if ((int64_t)v1 == (int64_t)v0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 456)))); a3xw16 = true; goto L_1dad20; }
    goto L_1dad70;
L_1dacc0:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 456)))); a3xw16 = true;
    { const bool c = (int64_t)t0 != 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 460)))); a3xw17 = true; if (c) goto L_1dacf8; }
    { const bool c = (int64_t)t1 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 31)))); if (c) goto L_1dacfc; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 440))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 444)))); a3xw15 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 1))));
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[6] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), t0, t1, 0u, 0u}; v0 = kernel_sub_001DA790(ram, rt, a0, a1, a2, a3, hi1, lo1, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; t3 = a7c[5]; a3xw16 = true; a3xw17 = true; a3xw18 = true; a3xw19 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420))));
    goto L_1dad6c;
L_1dacf8:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 31))));
L_1dacfc:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 440))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 444)))); a3xw15 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 1))));
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), 0u, t3}; kernel_sub_001DA850(ram, rt, a0, a1, a2, a3, t0, t1, hi1, lo1, h8, h10, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); t2 = a7c[2]; t3 = a7c[3]; a3xw18 = true; a3xw19 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420))));
    goto L_1dad6c;
L_1dad20:;
    { const bool c = (int64_t)t0 != 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 460)))); a3xw17 = true; if (c) goto L_1dad38; }
    { const bool c = (int64_t)t1 == 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 440)))); if (c) goto L_1dad58; }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    goto L_1dad40;
L_1dad38:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 440))));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_1dad40:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 444)))); a3xw15 = true;
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), 0u, t3}; kernel_sub_001DA9C8(ram, rt, a0, a1, a2, a3, t0, t1, sp, h3, h8, h10, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); t2 = a7c[2]; t3 = a7c[3]; a3xw18 = true; a3xw19 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420))));
    goto L_1dad6c;
L_1dad58:;
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 444)))); a3xw15 = true;
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {(uint64_t)a1_bits(f0), 0u, t1, t2, t3}; v0 = kernel_sub_001DA8F8(ram, rt, a0, a1, a2, a3, a7c); f0 = a1_flt((uint32_t)a7c[0]); t0 = a7c[1]; t1 = a7c[2]; t2 = a7c[3]; t3 = a7c[4]; a3xw16 = true; a3xw17 = true; a3xw18 = true; a3xw19 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420))));
L_1dad6c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1dad70:;
    { const bool c = (int64_t)v1 != (int64_t)v0; (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_1dad90; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 424))));
    if ((int64_t)v0 != 0) goto L_1dad90;
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), 0u, 0u}; kernel_sub_001DAAB0_x14(ram, rt, a0, a1, h2, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); a3 = a7c[3]; t0 = a7c[4]; a3xw15 = true; a3xw16 = true; }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_1dad90:;
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[15] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f7), (uint64_t)a1_bits(f8), (uint64_t)a1_bits(f9), (uint64_t)a1_bits(f10), (uint64_t)a1_bits(f11), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f13), (uint64_t)a1_bits(f14)}; v0 = kernel_sub_001DAB10_x14(ram, rt, a0, a1, (uint32_t)sp, h2, h3, h5, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f7 = a1_flt((uint32_t)a7c[7]); f8 = a1_flt((uint32_t)a7c[8]); f9 = a1_flt((uint32_t)a7c[9]); f10 = a1_flt((uint32_t)a7c[10]); f11 = a1_flt((uint32_t)a7c[11]); f12 = a1_flt((uint32_t)a7c[12]); f13 = a1_flt((uint32_t)a7c[13]); f14 = a1_flt((uint32_t)a7c[14]); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 452))));
    if ((int64_t)v1 == 0) { goto L_1dade4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 420))));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 464)))); goto L_1dadc0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 31))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
    (s1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 464))));
L_1dadc0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    { uint32_t divisor = (uint32_t)v1; if (divisor != 0) { lo = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 / divisor); hi = (uint64_t)(int64_t)(int32_t)((uint32_t)v0 % divisor); } else { lo = 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)(int32_t)(uint32_t)v0; } }
    A1_W32B((uint32_t)s0, 464, (uint32_t)v0);
    if ((int64_t)v1 == 0) { A6_BREAK(0x1DADD5u); goto L_1dadd8; }
L_1dadd8:;
    (a0 = (uint64_t)(hi));
    A1_W32B((uint32_t)s0, 464, (uint32_t)a0);
L_1dade4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f4); xo[5] = a1_bits(f5); xo[6] = a1_bits(f6); xo[7] = a1_bits(f7); xo[8] = a1_bits(f8); xo[9] = a1_bits(f9); xo[10] = a1_bits(f10); xo[11] = a1_bits(f11); xo[12] = a1_bits(f12); xo[13] = a1_bits(f13); xo[14] = a1_bits(f14); if (a3xw15) xo[15] = a3; if (a3xw16) xo[16] = t0; if (a3xw17) xo[17] = t1; if (a3xw18) xo[18] = t2; if (a3xw19) xo[19] = t3; } io_hi1 = hi1; io_lo1 = lo1; io_h2 = h2; io_h5 = h5; io_h6 = h6; io_h10 = h10; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DB238 (0x1db238): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f12 xo[5]=f13 xo[6]=f20 xo[7]=f21 xo[8]=f22 xo[9]=f23 xo[10]=a2 xo[11]=a3 xo[12]=t0; VU0 in c0st, out c0st, frame 48 bytes, 0 slot accesses)
inline uint64_t kernel_sub_001DB238(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, float f1, float f2, float f3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t *xo)
{
    A14_REC(1);
    float f0 = 0.0f;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(((int64_t)(int64_t)s1 < (int64_t)(int32_t)4) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468)))); if (c) goto L_1db288; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 1576))));
    goto L_1db298;
L_1db288:;
    { int64_t result = (int64_t)(int32_t)s1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    v1 = 0x290000u;
    v1 = v1 + 0x7DCu;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
L_1db298:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 12))));
    { const bool c = (int64_t)a3 == (int64_t)v0; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1db2c0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    { const bool c = (int64_t)a3 == (int64_t)v0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)6) ? 1 : 0)); if (c) goto L_1db2d8; }
    goto L_1db328;
L_1db2c0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 20))));
    A1_W32B(v1, 64, (uint32_t)s0);
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    A1_W32B(v1, 68, (uint32_t)v0);
    goto L_1db30c;
L_1db2d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 68))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { const bool c = (int64_t)v0 != 0; A1_W32B(v1, 68, (uint32_t)v0); if (c) goto L_1db2f8; }
    A1_W32B(v1, 64, 0u);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 20))));
    goto L_1db338;
L_1db2f8:;
    if ((int32_t)v0 >= 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 64)))); goto L_1db30c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    A1_W32B(v1, 68, (uint32_t)v0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 64))));
L_1db30c:;
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[13] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = kernel_sub_001DB238(ram, rt, v0, a0, a1, a2, a3, t0, f1, f2, f3, f12, f13, f20, f21, f22, f23, c0st, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f12 = a1_flt((uint32_t)a7c[4]); f13 = a1_flt((uint32_t)a7c[5]); f20 = a1_flt((uint32_t)a7c[6]); f21 = a1_flt((uint32_t)a7c[7]); f22 = a1_flt((uint32_t)a7c[8]); f23 = a1_flt((uint32_t)a7c[9]); a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; }
    goto L_1db380;
L_1db328:;
    if ((int64_t)v0 != 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4)))); goto L_1db33c; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967290))));
L_1db338:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
L_1db33c:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32767 << 16))));
    A1_W32B(v1, 52, (uint32_t)s2);
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)65535));
    A1_W32B(v1, 48, (uint32_t)s3);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    A1_W32B(v1, 60, (uint32_t)s0);
    { uint32_t bits = A1_R32B((uint32_t)s0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { const bool c = (int64_t)a0 == (int64_t)t0; { uint32_t bits = A1_R32B((uint32_t)s0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } if (c) goto L_1db36c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    { const bool c = (int64_t)v0 != (int64_t)t0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a0)))); if (c) goto L_1db374; }
L_1db36c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32767 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
L_1db374:;
    A1_W32B(v1, 56, (uint32_t)v0);
    { (a0 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = kernel_sub_001DB088(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3, f12, f13, f20, f21, f22, f23, c0st, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f20 = a1_flt((uint32_t)a7c[4]); f21 = a1_flt((uint32_t)a7c[5]); f22 = a1_flt((uint32_t)a7c[6]); f23 = a1_flt((uint32_t)a7c[7]); }
L_1db380:;
    if ((int64_t)s4 == 0) goto L_1db3a8;
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f12); xo[5] = a1_bits(f13); xo[6] = a1_bits(f20); xo[7] = a1_bits(f21); xo[8] = a1_bits(f22); xo[9] = a1_bits(f23); xo[10] = a2; xo[11] = a3; xo[12] = t0; } const uint64_t a4_t = EIntr(ram, rt, c0st); io_c0st = c0st; return a4_t; }
L_1db3a8:;
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f12); xo[5] = a1_bits(f13); xo[6] = a1_bits(f20); xo[7] = a1_bits(f21); xo[8] = a1_bits(f22); xo[9] = a1_bits(f23); xo[10] = a2; xo[11] = a3; xo[12] = t0; } io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ExPcmInputSetAutoFilterDirect (0x1dbc10): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st h19, out c0st h7, frame 64 bytes, 4 slot accesses)
inline uint64_t Sg2ExPcmInputSetAutoFilterDirect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t &io_h7, uint64_t io_h19)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t c0st = io_c0st;
    uint64_t h7 = io_h7;
    uint64_t h19 = io_h19;
    uint8_t a2fr[64];
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 56, a1_bits(f21));
    f21 = ((float)(f13));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 48, bits); }
    f20 = ((float)(f12));
    if ((int64_t)v0 == 0) goto L_1dbcd8;
    s0 = 0x1D4u;
    { { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)s0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (s0 = (uint32_t)((int32_t)((int32_t)result))); } A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294966296))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)(s3 ^ RC_U64X(v1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    at = 0x290000u;
    at = at + 0x7DCu;
    s0 = at + s0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s0))));
    t1 = s0 + t0;
    if (v1 != 0) do { const a4::q128 _v = (a4::mk128(h19, s3)); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    s0 = t1;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    A1_W32B((uint32_t)v0, 48, 0u);
    A1_W32B(t1, 4, (uint32_t)v1);
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    A1_W32B(s0, 12, 0u);
    (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    f12 = ((float)(f20));
    { f13 = ((float)(f21)); A3_CHARGE(); v0 = kernel_sub_001DB088(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3, f12, f13, f20, f21, f22, f23, c0st); }
    if ((int64_t)s1 == 0) { goto L_1dbcdc; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1dbcd8:;
L_1dbcdc:;
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 56));
    { uint32_t bits = a2::frLd<uint32_t>(a2fr, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { io_c0st = c0st; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// Sg2ExPcmOutputSetAutoFilterDirect (0x1dc098): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st h18, out c0st h7, frame 64 bytes, 4 slot accesses)
inline uint64_t Sg2ExPcmOutputSetAutoFilterDirect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st, uint64_t &io_h7, uint64_t io_h18)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t c0st = io_c0st;
    uint64_t h7 = io_h7;
    uint64_t h18 = io_h18;
    uint8_t a2fr[64];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1576))));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 48, a1_bits(f21));
    f21 = ((float)(f13));
    a2::frSt<uint32_t>(a2fr, 40, a1_bits(f20));
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = DIntr(ram, rt, c0st); }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    v1 = v1 << 4;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    v1 = v1 << 2;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 48))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    A1_W32B(v1, 48, 0u);
    (a0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a1, 4, (uint32_t)a2);
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)a0, 12, 0u); if (c) goto L_1dc118; }
    { A3_CHARGE(); v0 = EIntr(ram, rt, c0st); }
L_1dc118:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294966296))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)(s2 ^ RC_U64X(v0)));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    f12 = ((float)(f20));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 40));
    f13 = ((float)(f21));
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 48));
    if (v0 != 0) do { const a4::q128 _v = (a4::mk128(h18, s2)); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    { const uint64_t a4_t = kernel_sub_001DB088(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3, f12, f13, f20, f21, f22, f23, c0st); io_c0st = c0st; io_h7 = h7; return a4_t; }
    __builtin_unreachable();
}

// clipBySphere (0x1087260): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in vf0 vf26 vacc vclip, out vf26 vf27 vf28 vf29 vf30 vf31 vacc vclip, frame 16 bytes, 0 slot accesses)
inline uint64_t clipBySphere(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint32_t &io_vclip)
{
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint32_t vclip = io_vclip;
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 1440))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 480))));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    { a4::vec4 res = a4v::addK<1, 0>(vf0, a4v::bcast<3>(vf27)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    vf31 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, 0); vf26 = a4v::blendv_ps(vf26, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf28, a4v::bcast<0>(vf26)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf29, a4v::bcast<1>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf30, a4v::bcast<2>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf31, a4v::bcast<3>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    vf28 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf31 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    { a4::vec4 res = a4v::mul(vf28, a4v::bcast<0>(vf27)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf29, a4v::bcast<1>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf30, a4v::bcast<2>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf31, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf27, a4v::bcast<0>(vf26)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    vclip = ((vclip << 6) | ps2VuClipFlags(vf27, vf27)) & 0xFFFFFF;
    (a0 = (uint64_t)((int64_t)(int32_t)(vclip & 0x00FFFFFFu)));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)63));
    { io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vclip = vclip; return v0; }
    __builtin_unreachable();
}

// shadowReInit (0x109df58): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in -, out hi1 lo1 h2 h3, relocated calls 5, guest sp, frame 64 bytes, 0 slot accesses)
inline uint64_t shadowReInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo)
{
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { s5 = 0x1190000u; A3_CHARGE(); v0 = a1_sx64(getDrawScreenWidth(ram, rt)); }
    { (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getDrawScreenHeight(ram, rt)); }
    (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_109df98:;
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4294967295)))); A3_CHARGE(); v0 = a1_sx64(getFrameBufferSize(ram, rt, hi1, lo1)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (s0 = (uint64_t)(RC_U64X(s0) | (uint64_t)(uint16_t)16384));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v0))));
    { (s0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)s0, 11)))); A3_CHARGE(); v0 = a1_sx64(getZBufferAddressFBP(ram, rt)); }
    v1 = A1_R32(s5 - 0x3F74u);
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    a0 = A1_R32B(v1, 28);
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, (uint32_t)s2))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 400)))); A3_CHARGE(); uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = iosSetDCParam(ram, rt, a1_sx64(a0), a1, a2, a3, t0, (uint32_t)(sp + 0xFFFFFFC0u), h2, h3, a7c); a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; t2 = a7c[6]; t3 = a7c[7]; }
    if ((int32_t)s1 >= 0) { A3_BACK(); goto L_109df98; }
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } io_hi1 = hi1; io_lo1 = lo1; io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GetDissolveAnimation (0x10b7710): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1; VU0 in -, out h2, break)
inline a1::RetVF GetDissolveAnimation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, t0, t1, 0u, 0u}; v0 = GetAnimObjResource(ram, rt, a0, t0, t1, (uint32_t)t2, (uint32_t)t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a3 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 != 0) { goto L_10b77a0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 64))));
    if ((int64_t)a1 == 0) { goto L_10b77a0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v0 == (int64_t)v1; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295)))); if (c) goto L_10b7778; }
    { int32_t divisor = (int32_t)a0;    int32_t dividend = (int32_t)s0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    if ((int64_t)a0 == 0) { A6_BREAK(0x10B7761u); goto L_10b7764; }
L_10b7764:;
    (v0 = (uint64_t)(lo));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)s0, (uint32_t)v0))));
    goto L_10b777c;
L_10b7778:;
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b777c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s0))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15232 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)32896));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v1, 0)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f1);
L_10b77a0:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; } io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// manager_sub_0110EF60 (0x110ef60): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f0 xo[1]=f12 xo[2]=f20 xo[3]=f21 xo[4]=a1 xo[5]=a2 xo[6]=a3 xo[7]=t0 xo[8]=t1; VU0 in -, out -, jump tables 1, frame 48 bytes, 2 slot accesses)
inline void manager_sub_0110EF60(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, uint64_t *xo)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint8_t a2fr[48];
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f20));
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 8))));
    { const bool c = (int32_t)a1 <= 0; f20 = ((float)(f12)); if (c) goto L_110f0b0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
L_110ef90:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    (v1 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)6) ? 1 : 0));
    if ((int64_t)v0 == 0) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1)))); goto L_110f0a4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 21216))));
    { const uint32_t a5_jt = (uint32_t)v1; switch (a5_jt) { case 0x110EFD0u: goto L_110efd0; case 0x110F008u: goto L_110f008; case 0x110F020u: goto L_110f020; case 0x110F090u: goto L_110f090; case 0x110F0A0u: goto L_110f0a0; default: A5_BADJUMP(0x110EFC4u, a5_jt); } }
L_110efd0:;
    (v1 = (uint64_t)(a0 & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4)))); goto L_110f034; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 12)))); A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_0110EB10(ram, rt, a0, a1_sx64(a1)); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)2));
    goto L_110f084;
L_110f008:;
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { f12 = ((float)(f20)); a3xw1 = true; A3_CHARGE(); uint64_t a7c[7] = {(uint64_t)a1_bits(f12), 0u, 0u, a2, a3, t0, t1}; { const a1::RetVF a2_r = manager_sub_0110EDC8_x14(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); f21 = a1_flt((uint32_t)a7c[2]); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; a3xw0 = true; a3xw1 = true; a3xw3 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 8))));
    goto L_110f0a0;
L_110f020:;
    (v1 = (uint64_t)(a0 & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 == (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_110f048; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
L_110f034:;
    if ((int64_t)v1 == (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0)))); goto L_110f068; }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
    goto L_110f0a4;
L_110f048:;
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 12)))); A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_0110EB10(ram, rt, a0, a1_sx64(a1)); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)2));
    goto L_110f084;
L_110f068:;
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 12)))); A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_0110EB10(ram, rt, a0, a1_sx64(a1)); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)5));
L_110f084:;
    A1_W32B(s0, 8, (uint32_t)v0);
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 8))));
    goto L_110f0a0;
L_110f090:;
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { f12 = ((float)(f20)); a3xw1 = true; A3_CHARGE(); uint64_t a7c[7] = {(uint64_t)a1_bits(f12), 0u, 0u, a2, a3, t0, t1}; { const a1::RetVF a2_r = manager_sub_0110EDC8_x14(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); f21 = a1_flt((uint32_t)a7c[2]); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; a3xw0 = true; a3xw1 = true; a3xw3 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 8))));
L_110f0a0:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
L_110f0a4:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s2 < (int64_t)(int64_t)a1_sx64(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4)))); if (c) { A3_BACK(); goto L_110ef90; } }
L_110f0b0:;
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f12); xo[2] = a1_bits(f20); if (a3xw3) xo[3] = a1_bits(f21); xo[4] = a1_sx64(a1); if (a3xw5) xo[5] = a2; if (a3xw6) xo[6] = a3; if (a3xw7) xo[7] = t0; if (a3xw8) xo[8] = t1; } return; }
    __builtin_unreachable();
}

// PauseGame (0x1345cb8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f1 xo[1]=at xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=hi xo[9]=lo; VU0 in -, out hi1 lo1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF PauseGame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t1, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v1 = 0x1460000u;
    v1 = v1 + 0x5650u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 428))));
    (v0 = (uint64_t)(RC_U64X(v0) | a0));
    A1_W32B(v1, 428, (uint32_t)v0);
    { uint64_t a7c[10] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; const a1::RetVF a7_t = gamecore_sub_01345980(ram, rt, t1, a1_sx64((uint32_t)(sp + 0x0u)), hi1, lo1, a7c); f1 = a1_flt((uint32_t)a7c[0]); at = a7c[1]; a0 = a7c[2]; a1 = a7c[3]; a2 = a7c[4]; a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; hi = a7c[8]; lo = a7c[9]; if (xo) { xo[0] = a1_bits(f1); xo[1] = at; xo[2] = a0; xo[3] = a1; xo[4] = a2; xo[5] = a3; xo[6] = t0; xo[7] = t1; xo[8] = hi; xo[9] = lo; } io_hi1 = hi1; io_lo1 = lo1; return a7_t; }
    __builtin_unreachable();
}

// ChangeGameTimeScale (0x1345d00): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out hi1 lo1, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF ChangeGameTimeScale(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t w64_t1, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint32_t v0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v0 = 0x1460000u;
    A1_W32B(v0, 22504, a0);
    { const a1::RetVF a4_t = gamecore_sub_01345980(ram, rt, w64_t1, a1_sx64((uint32_t)(sp + 0x0u)), hi1, lo1); io_hi1 = hi1; io_lo1 = lo1; return a4_t; }
    __builtin_unreachable();
}

// ChangeGameTimeScaleRatio (0x1345d20): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out hi1 lo1, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF ChangeGameTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_t1, uint32_t sp, float f12, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint32_t v0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v0 = 0x1460000u;
    A1_W32B(v0, 22508, a1_bits(f12));
    { const a1::RetVF a4_t = gamecore_sub_01345980(ram, rt, w64_t1, a1_sx64((uint32_t)(sp + 0x0u)), hi1, lo1); io_hi1 = hi1; io_lo1 = lo1; return a4_t; }
    __builtin_unreachable();
}

// setGameAnimeTimeScaleRatio (0x1345ea0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out hi1 lo1, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF setGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_t1, uint32_t sp, uint64_t &io_hi1, uint64_t &io_lo1)
{
    float f12 = 0.0f;
    float f13 = 0.0f;
    uint32_t v0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    v0 = 0x1460000u;
    f13 = a1_flt(0u);
    f12 = a1_flt(A1_R32B(v0, 23012));
    { const a1::RetVF a4_t = ChangeGameAnimeTimeScaleRatio(ram, rt, w64_t1, a1_sx64((uint32_t)(sp + 0x0u)), f12, f13, hi1, lo1); io_hi1 = hi1; io_lo1 = lo1; return a4_t; }
    __builtin_unreachable();
}

// setFrameRateMode (0x1345ee0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out hi1 lo1, relocated calls 1, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF setFrameRateMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_t1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint32_t v1 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint32_t s0 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    { s0 = 0x1460000u; A3_CHARGE(); v0 = iosGetTCount(ram, rt); }
    s0 = s0 + 0x59F0u;
    A1_W64B(s0, 0, v0);
    v1 = 0x1460000u;
    A1_W64B(v1, 22096, v0);
    { const a1::RetVF a4_t = gamecore_sub_01346090(ram, rt, w64_t1, a1_sx64((uint32_t)(sp + 0x0u)), hi1, lo1); io_hi1 = hi1; io_lo1 = lo1; return a4_t; }
    __builtin_unreachable();
}

// setDemoBar (0x1346240): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f2 xo[1]=f12 xo[2]=f13 xo[3]=f20 xo[4]=f21 xo[5]=f22 xo[6]=a1 xo[7]=a2 xo[8]=a3 xo[9]=t0 xo[10]=t1; VU0 in -, out hi1 lo1, guest sp, frame 32 bytes, 6 slot accesses)
inline a1::RetVF setDemoBar(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, float f0, float f12, float f13, float f20, float f21, float f22, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t *xo)
{
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t fcr31 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint8_t a2fr[32];
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f20));
    f20 = a1_flt(0u);
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f21));
    f21 = ((float)(f12));
    fcr31 = (a1f::eq(f21, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f22));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    f12 = a1_flt(a7_opq(at));
    { const bool c = (fcr31 & 0x800000u) != 0u; f22 = ((float)(f13)); if (c) goto L_1346298; }
    { A3_CHARGE(); v0 = a1_sx64(SetDefaultGameAnimeTimeScaleRatio(ram, rt, f12)); }
    { A3_CHARGE(); { const a1::RetVF a2_r = GetGameAnimeTimeScaleRatio(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    f12 = ((float)(f0));
    { f13 = ((float)(f20)); A3_CHARGE(); uint64_t a7c[4] = {a2, a3, t0, t1}; { const a1::RetVF a2_r = ChangeGameAnimeTimeScaleRatio(ram, rt, t1, a1_sx64((uint32_t)(sp + 0xFFFFFFE0u)), f12, f13, hi1, lo1, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; }
    f12 = ((float)(f21));
    goto L_13462b8;
L_1346298:;
    { A3_CHARGE(); { const a1::RetVF a2_r = RestoreDefaultGameAnimeTimeScaleRatio(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { A3_CHARGE(); { const a1::RetVF a2_r = GetGameAnimeTimeScaleRatio(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    f12 = ((float)(f0));
    { f13 = ((float)(f20)); A3_CHARGE(); uint64_t a7c[4] = {a2, a3, t0, t1}; { const a1::RetVF a2_r = ChangeGameAnimeTimeScaleRatio(ram, rt, t1, a1_sx64((uint32_t)(sp + 0xFFFFFFE0u)), f12, f13, hi1, lo1, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; }
    f12 = ((float)(f21));
L_13462b8:;
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 16));
    f13 = ((float)(f22));
    f22 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    a0 = 0x1460000u;
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 8));
    a1 = 0x18u;
    a0 = a0 + 0x5A58u;
    { uint64_t a7c[1] = {0u}; const a1::RetVF a7_t = setFadeParam_x14(ram, rt, a0, a1, f12, f13, a7c); f2 = a1_flt((uint32_t)a7c[0]); if (xo) { xo[0] = a1_bits(f2); xo[1] = a1_bits(f12); xo[2] = a1_bits(f13); xo[3] = a1_bits(f20); xo[4] = a1_bits(f21); xo[5] = a1_bits(f22); xo[6] = a1_sx64(a1); xo[7] = a2; xo[8] = a3; xo[9] = t0; xo[10] = t1; } io_hi1 = hi1; io_lo1 = lo1; return a7_t; }
    __builtin_unreachable();
}

// gamecore_sub_01364CD8 (0x1364cd8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=t1 xo[1]=t2 xo[2]=t3; VU0 in -, out hi1 lo1 h2 h3 h5 h6, break)
inline a1::RetVF gamecore_sub_01364CD8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h6, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f6, &bits, sizeof(bits)); }
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 22928))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)fp, 25512))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f4, sizeof(tmp)); f4 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f4 = a1f::mul(f0, f4);
    f1 = a1f::mul(f1, f3);
    f0 = ((float)(f4) * (float)(f6));
    f1 = ((float)(f1) * (float)(f6));
    f0 = a1f::sub(f0, f5);
    f2 = a1f::add(f2, f1);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t tmp = a1f::cvtw(f2); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { const bool c = (int32_t)a1 < 0; (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1364da8; }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    if ((fcr31 & 0x800000u) == 0u) { (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); goto L_1364db0; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::subFinite(f4, f0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    goto L_1364db0;
L_1364da8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
L_1364db0:;
    { const bool c = (int32_t)v1 < 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)fp, 25512)))); if (c) goto L_1364e08; }
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { int32_t tmp = a1f::cvtw(f2); std::memcpy(&f3, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f3, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    f1 = a1f::mul(f0, f1);
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)fp, 25512)))); goto L_1364e0c; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::subFinite(f1, f0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)fp, 25512))));
    goto L_1364e0c;
L_1364e00:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1364ee0;
L_1364e08:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1364e0c:;
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 0, 0u);
    { uint32_t bits = A1_R32B((uint32_t)v0, 60); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::div(fcr31, f0, f3);
    f1 = a1f::div(fcr31, f1, f2);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f2, sizeof(bits)); (s3 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)s3;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(hi));
    (s0 = (uint64_t)(lo));
    A1_W32B((uint32_t)sp, 4, (uint32_t)v0);
    if ((int64_t)s3 == 0) { A6_BREAK(0x1364E61u); goto L_1364e64; }
L_1364e64:;
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (s4 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (s7 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { int32_t divisor = (int32_t)s4;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (s1 = (uint64_t)(lo));
    (v0 = (uint64_t)(hi));
    (s6 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 8, (uint32_t)v0);
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { A3_CHARGE(); { const a1::RetVF a2_r = gamecore_sub_01361E88(ram, rt, a1, a2, sp, hi1, lo1, h2, h3, h5, h6); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v1 < 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1364eec; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 0))));
    if ((int32_t)v0 < 0) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1364ee0; }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
L_1364eb8:;
    { const bool c = (int64_t)v0 == (int64_t)v1; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4)))); if (c) { A3_BACK(); goto L_1364e00; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    if ((int32_t)v0 >= 0) { A3_BACK(); goto L_1364eb8; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1364ee0:;
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1364eec; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1364eec:;
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_1364fcc; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)fp, 25512))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 52))));
    (a0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 5))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 27))));
    (a0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)1023));
    if ((int64_t)v0 == 0) { A6_BREAK(0x1364F11u); goto L_1364f14; }
L_1364f14:;
    { int32_t divisor = (int32_t)v0;    int32_t dividend = (int32_t)a0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 60))));
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 27))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 64))));
    (a0 = (uint64_t)(lo));
    (a2 = (uint64_t)(hi));
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo1 = (uint64_t)(int64_t)(int32_t)result; hi1 = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint64_t)(lo));
    (t0 = (uint64_t)(hi));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t0))));
    { const bool c = (int32_t)a2 >= 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_1364f80; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; { const a1::RetVF a2_r = gamecore_sub_01361E88(ram, rt, a1, a2, sp, hi1, lo1, h2, h3, h5, h6, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } t1 = a7c[0]; t2 = a7c[1]; t3 = a7c[2]; }
    if ((int32_t)v0 < 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1364f6c; }
    A1_W32B((uint32_t)s2, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1364f6c:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_136509c; }
    A1_W32B((uint32_t)sp, 0, (uint32_t)v0);
    goto L_136509c;
L_1364f80:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)s0, (uint32_t)a2))));
    if ((int32_t)v0 < 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)s0)))); goto L_1364f8c; }
L_1364f8c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_1364fcc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)s1, (uint32_t)a0))));
    { const bool c = (int32_t)v0 < 0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)2) ? 1 : 0)); if (c) goto L_1364fb8; }
    if ((int64_t)v0 == 0) { (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); goto L_1364fd0; }
    (v0 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int64_t)a2) ? 1 : 0));
    goto L_1364ff8;
L_1364fb8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)s1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int64_t)a2) ? 1 : 0)); if (c) goto L_1364ff8; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_1364fcc:;
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_1364fd0:;
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; { const a1::RetVF a2_r = gamecore_sub_01361E88(ram, rt, a1, a2, sp, hi1, lo1, h2, h3, h5, h6, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } t1 = a7c[0]; t2 = a7c[1]; t3 = a7c[2]; }
    if ((int32_t)v0 < 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1364fe8; }
    A1_W32B((uint32_t)s2, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1364fe8:;
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_136509c; }
    A1_W32B((uint32_t)sp, 0, (uint32_t)v1);
    goto L_136509c;
L_1364ff8:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)s0) ? 1 : 0)); if (c) goto L_1365020; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967276))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)s0) ? 1 : 0)); if (c) goto L_1365020; }
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1))));
    goto L_1365034;
L_1365020:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)s1 < (int64_t)(int64_t)a0) ? 1 : 0)); if (c) goto L_1365038; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)20) ? 1 : 0));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)s0, (uint32_t)v0))));
L_1365034:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s1 < (int64_t)(int64_t)a0) ? 1 : 0));
L_1365038:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int64_t)s1) ? 1 : 0)); if (c) goto L_1365060; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 4294967276))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int64_t)s1) ? 1 : 0)); if (c) goto L_1365060; }
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    goto L_1365074;
L_1365060:;
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); if (c) goto L_1365078; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)20) ? 1 : 0));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)s1, (uint32_t)v0))));
L_1365074:;
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
L_1365078:;
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; { const a1::RetVF a2_r = gamecore_sub_01361E88(ram, rt, a1, a2, sp, hi1, lo1, h2, h3, h5, h6, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } t1 = a7c[0]; t2 = a7c[1]; t3 = a7c[2]; }
    if ((int32_t)v0 < 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1365094; }
    A1_W32B((uint32_t)s2, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1365094:;
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    A1_W32B((uint32_t)sp, 0, (uint32_t)v0);
L_136509c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    A1_W32B((uint32_t)v0, 0, (uint32_t)v1);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); if (xo) { xo[0] = t1; xo[1] = t2; xo[2] = t3; } io_hi1 = hi1; io_lo1 = lo1; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h6 = h6; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetStageContext (0x1365230): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=a1 xo[6]=a2 xo[7]=a3 xo[8]=t0 xo[9]=t1 xo[10]=t2 xo[11]=t3; VU0 in -, out h2 h4 h5 h6 h7, relocated calls 1, break)
inline uint64_t GetStageContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw7 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294935104))));
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 2)));
    { const uint32_t _dynInsn = A1_R32(0x136524Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a2 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)v0, 0)));
    { const uint32_t _dynInsn = A1_R32(0x1365254u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    A1_W8B((uint32_t)t0, 2, (uint8_t)(uint32_t)a1);
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25512))));
    A1_W16B((uint32_t)t0, 0, (uint16_t)(uint32_t)a2);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)t0, 48), a4::mk128(h2, v0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16)))); if (c) goto L_1365440; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 25516))));
    { const bool c = (int64_t)v1 == 0; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw11 = true; if (c) goto L_1365440; }
    { uint32_t bits = A1_R32B((uint32_t)a0, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); } a3xw1 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f5, &bits, sizeof(bits)); } a3xw4 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 4)))); a3xw10 = true;
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)t2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = ((float)(f0) * (float)(f5)); a3xw0 = true;
    f0 = a1f::sub(f0, f1); a3xw0 = true;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    A1_W32B((uint32_t)t0, 8, (uint32_t)a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)t2, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); } a3xw1 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f0 = ((float)(f0) * (float)(f5)); a3xw0 = true;
    f1 = a1f::add(f1, f0);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a2 = (uint64_t)((int64_t)(int32_t)(bits))); }
    A1_W32B((uint32_t)t0, 12, (uint32_t)a2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)a0;    int32_t dividend = (int32_t)a1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    A1_W32B((uint32_t)t0, 16, (uint32_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    A1_W32B((uint32_t)t0, 20, (uint32_t)v1);
    if ((int64_t)a0 == 0) { A6_BREAK(0x1365315u); goto L_1365318; }
L_1365318:;
    (a1 = (uint64_t)(lo));
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)a2;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    A1_W32B((uint32_t)t0, 24, (uint32_t)a1);
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    A1_W32B((uint32_t)t0, 24, (uint32_t)a1);
    (a2 = (uint64_t)(lo));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    A1_W32B((uint32_t)t0, 28, (uint32_t)a2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 52)))); a3xw7 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw7 = true;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)RC_U64X(a3)) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h5, a1)); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0); a3xw7 = true;
    A1_W32B((uint32_t)t0, 24, (uint32_t)a3);
    { uint32_t bits = (uint32_t)a3; std::memcpy(&f3, &bits, sizeof(bits)); } a3xw2 = true;
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = ((float)(int32_t)(tmp)); } a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 56))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)RC_U64X(a0)) ? 1 : 0));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h6, a2)); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    A1_W32B((uint32_t)t0, 28, (uint32_t)a0);
    { uint32_t bits = (uint32_t)a0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; } a3xw3 = true;
    { int32_t tmp; std::memcpy(&tmp, &f4, sizeof(tmp)); f4 = ((float)(int32_t)(tmp)); } a3xw3 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f2 = a1f::mul(f0, f5); a3xw1 = true;
    f3 = a1f::mul(f3, f0); a3xw2 = true;
    f0 = a1f::mul(f0, f4); a3xw0 = true;
    f3 = a1f::addFinite(f3, f2); a3xw2 = true;
    f0 = a1f::mul(f0, f5); a3xw0 = true;
    f0 = a1f::subFinite(f0, f3); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t0, 48, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; } a3xw2 = true;
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = ((float)(int32_t)(tmp)); } a3xw2 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f1 = a1f::mul(f1, f0);
    f2 = a1f::mul(f0, f5); a3xw1 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f1 = a1f::addFinite(f1, f2);
    f0 = a1f::mul(f0, f5); a3xw0 = true;
    f1 = a1f::subFinite(f1, f0);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t0, 56, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 52))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    A1_W32B((uint32_t)t0, 32, (uint32_t)a0);
    { uint32_t bits = A1_R32B((uint32_t)t2, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (t3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw11 = true; goto L_13653fc; }
L_13653fc:;
    { uint32_t bits = A1_R32B((uint32_t)t2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1365414; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1365414:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t3))));
    (a0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)t0, 36, (uint32_t)v0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294935112))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 28)))); a3xw7 = true;
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 65)))); A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = a1_sx64(iosSPrintf(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3, t0, t1, t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c)); a2 = a7c[0]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1365444;
L_1365440:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1365444:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f2); if (a3xw2) xo[2] = a1_bits(f3); if (a3xw3) xo[3] = a1_bits(f4); if (a3xw4) xo[4] = a1_bits(f5); xo[5] = a1; xo[6] = a2; if (a3xw7) xo[7] = a3; xo[8] = t0; xo[9] = t1; if (a3xw10) xo[10] = t2; if (a3xw11) xo[11] = t3; } io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// SCRFuncBirdSetLandingPos (0x136eb78): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat h2 h3)
inline uint64_t SCRFuncBirdSetLandingPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointPosFromEvent(ram, rt, v0, a0, a1, a2, h3); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = (int64_t)v0 == 0; (a2 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); if (c) goto L_136ec20; }
    { A3_CHARGE(); v0 = getPointPosFromEvent(ram, rt, v0, a0, a1, a2, h3); }
    { const bool c = (int64_t)v0 == 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (c) goto L_136ec20; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf7));
    A1_W32B((uint32_t)sp, 36, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 44, bits); } A3_CHARGE(); v0 = setAttainmentPosAndDir(ram, rt, v0, a0, a1, a2, f12, f13, f14, f15, f16, f17, f18, f19, h2, h3); }
L_136ec20:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SCRCerberusEventLineCut (0x137b488): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, 64-bit inputs a0, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRCerberusEventLineCut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, w64_a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    a2 = 4u;
    { const uint64_t a4_t = setEventLineAttr(ram, rt, v0, a1_sx64(a0), a1, a1_sx64(a2)); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// kernel_sub_001D24B0 (0x1d24b0): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in c0st, out c0st)
inline uint64_t kernel_sub_001D24B0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint32_t sp, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t s0 = 0;
    uint32_t c0st = io_c0st;
    sp = sp - 0x1A0u;
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    s0 = 0x290000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, 4294949312))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
L_1d24d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, -4))));
    if ((int64_t)v0 != (int64_t)a0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d2514; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    if ((int64_t)v0 != (int64_t)a1) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d2514; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
    if ((int64_t)v0 != (int64_t)a2) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d2514; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, -8))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    A1_W32B((uint32_t)t1, 0, (uint32_t)t0);
    A1_W32B((uint32_t)t1, 4, (uint32_t)v0);
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 8))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
L_1d2514:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int32_t)32) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0x50u; if (c) { A3_BACK(); goto L_1d24d8; } }
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)t2) ? 1 : 0));
    if ((int64_t)v0 == 0) { goto L_1d25ac; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    v1 = A1_R32B(sp, 4);
    (a0 = (uint64_t)(((uint64_t)RC_U64X(t0) < (uint64_t)t2) ? 1 : 0));
    A1_W32B(sp, 384, (uint32_t)v0);
    { const bool c = (int64_t)a0 == 0; A1_W32B(sp, 388, v1); if (c) goto L_1d2580; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 12))));
L_1d2550:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v1 = A1_R32B(sp, 388);
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)a1_sx64(v1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); if (c) goto L_1d2574; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, -4))));
    A1_W32B(sp, 384, (uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    A1_W32B(sp, 388, v1);
L_1d2574:;
    (v0 = (uint64_t)(((uint64_t)RC_U64X(t0) < (uint64_t)t2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8)))); if (c) { A3_BACK(); goto L_1d2550; } }
L_1d2580:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 384)))); A3_CHARGE(); v0 = Sg2SepStopSndOff(ram, rt, a0, c0st); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, 4294949312))));
    v1 = A1_R32B(sp, 384);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    A1_W32B((uint32_t)v0, 20, (uint32_t)a0);
L_1d25ac:;
    { sp = sp + 0x1A0u; io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SepVabStopSndOff (0x1d2ae8): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2SepVabStopSndOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t &io_c0st)
{
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    { (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = vab_check_alive(ram, rt, a0); }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16)))); if (c) goto L_1d2b38; }
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294949312))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 12))));
L_1d2b18:;
    { const bool c = (int64_t)v0 != (int64_t)s2; s1 = s1 + 0x50u; if (c) goto L_1d2b28; }
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = Sg2SepStopSndOff(ram, rt, a0, c0st); }
L_1d2b28:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s0) < (int64_t)(int32_t)32) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 12)))); { A3_BACK(); goto L_1d2b18; } }
L_1d2b38:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2SepAllStopSndOff (0x1d2b58): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in c0st, out c0st, frame 16 bytes, 0 slot accesses)
inline uint64_t Sg2SepAllStopSndOff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t s0 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
L_1d2b70:;
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1)))); A3_CHARGE(); v0 = Sg2SepStopSndOff(ram, rt, a0, c0st); }
    (v1 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int32_t)32) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1d2b70; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ExPcmInputSetAutoFilter (0x1dbd00): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, 64-bit inputs t0, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2ExPcmInputSetAutoFilter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t w64_t0, float f1, float f2, float f3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t t0 = (uint32_t)w64_t0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t c0st = io_c0st;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v1 = (uint64_t)(((uint64_t)s2 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 == 0; (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_1dbd74; }
    { A3_CHARGE(); v0 = a1_sx64(kernel_sub_001DB3C8(ram, rt, (uint32_t)a0)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == (int64_t)v1; (v1 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int64_t)a0) ? 1 : 0)); if (c) goto L_1dbd74; }
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294)))); if (c) goto L_1dbd74; }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = kernel_sub_001DB418(ram, rt, (uint32_t)a0, (uint32_t)a1); }
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = kernel_sub_001DB238(ram, rt, v0, a0, a1, a2, a3, w64_t0, f1, f2, f3, f12, f13, f20, f21, f22, f23, c0st); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1dbd74:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// Sg2ExPcmOutputSetAutoFilter (0x1dc160): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in c0st, out c0st, 64-bit inputs t0, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2ExPcmOutputSetAutoFilter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t w64_t0, float f1, float f2, float f3, float f12, float f13, float f20, float f21, float f22, float f23, uint32_t &io_c0st)
{
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t t0 = (uint32_t)w64_t0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t c0st = io_c0st;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(kernel_sub_001DB3C8(ram, rt, (uint32_t)a0)); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const bool c = (int64_t)v1 == (int64_t)a0; (a1 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int64_t)v1) ? 1 : 0)); if (c) goto L_1dc1b8; }
    { const bool c = (int64_t)a1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294)))); if (c) goto L_1dc1b8; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = kernel_sub_001DB418(ram, rt, (uint32_t)a0, (uint32_t)a1); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = kernel_sub_001DB238(ram, rt, v0, a0, a1, a2, a3, w64_t0, f1, f2, f3, f12, f13, f20, f21, f22, f23, c0st); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1dc1b8:;
    { io_c0st = c0st; return v0; }
    __builtin_unreachable();
}

// manager_sub_0113B1D8 (0x113b1d8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf20 vf21 vf26 vacc vclip h2, out vf1 vf2 vf4 vf5 vf6 vf7 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf20 vf21 vf24 vf25 vf26 vf27 vf28 vf29 vf30 vf31 vacc vclip)
inline a1::RetVF manager_sub_0113B1D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint32_t &io_vclip, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint32_t vclip = io_vclip;
    uint64_t h2 = io_h2;
    sp = sp - 0xE0u;
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 48))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 32); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 16))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf5));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 != (int64_t)v0; a3 = 2u; if (c) goto L_113b250; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { A1_W32B(sp, 12, a1_bits(f0)); A3_CHARGE(); v0 = clipBySphere(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf26, vf27, vf28, vf29, vf30, vf31, vacc, vclip); }
    if ((int64_t)v0 != 0) { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_113b3f0; }
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    goto L_113b3f4;
L_113b250:;
    { const bool c = (int64_t)v1 != (int64_t)(int32_t)a3; (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_113b3f4; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 116); float f; std::memcpy(&f, &bits, sizeof(f)); f6 = f; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 32))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    (a1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    f1 = (-(float)(f5));
    at = 0x3F000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    f2 = (-(float)(f4));
    A1_W32B(sp, 60, 0u);
    f3 = (-(float)(f6));
    A1_W32B(sp, 76, 0u);
    f6 = a1f::mul(f6, f0);
    f1 = a1f::mul(f1, f0);
    f2 = a1f::mul(f2, f0);
    f3 = a1f::mul(f3, f0);
    f5 = a1f::mul(f5, f0);
    { float f = f6; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 72, bits); }
    f4 = a1f::mul(f4, f0);
    A1_W32B(sp, 48, a1_bits(f1));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 52, bits); }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 56, bits); }
    (v0 = (uint64_t)(A1_R64B(sp, 48)));
    (v1 = (uint64_t)(A1_R64B(sp, 56)));
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 64, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 68, bits); }
    A1_W64B(sp, 32, v0);
    A1_W64B(sp, 40, v1);
    (v0 = (uint64_t)(A1_R64B(sp, 64)));
    (v1 = (uint64_t)(A1_R64B(sp, 72)));
    A1_W64B(sp, 48, v0);
    A1_W64B(sp, 56, v1);
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf11, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf11, vf10); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf10, vf10); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf13, vf13); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf13, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf12, vf17); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf17 = a4v::blendv_ps(vf17, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf17, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, vf17, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)a0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 48), a4v::castps_si128(vf4));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { A1_W32B(sp, 140, a1_bits(f0)); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1440))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 144))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf24)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf24)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf24)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf24)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf24 = a4v::blendv_ps(vf24, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf25)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf26)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf27)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)a2, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)a2, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)a2, 48), a4v::castps_si128(vf27));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48)))); A3_CHARGE(); v0 = a1_sx64(clipByMinMaxCube(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf1, vf2, vf4, vf5, vf6, vf7, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf20, vf21, vacc, vclip)); }
    (s0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
L_113b3f0:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_113b3f4:;
    { sp = sp + 0xE0u; io_vf1 = vf1; io_vf2 = vf2; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf20 = vf20; io_vf21 = vf21; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vclip = vclip; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013524D8 (0x13524d8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out hi1 lo1, relocated calls 1, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_013524D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_t1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint64_t a0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    { A3_CHARGE(); v0 = a1_sx64(isysGetThisSubThread(ram, rt)); }
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); scrInit(ram, rt); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const a1::RetVF a4_t = PauseGame(ram, rt, a0, w64_t1, (uint32_t)(sp + 0x0u), hi1, lo1); io_hi1 = hi1; io_lo1 = lo1; return a4_t; }
    __builtin_unreachable();
}

// gamecore_sub_01352528 (0x1352528): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out hi1 lo1, relocated calls 1, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_01352528(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_t1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_hi1, uint64_t &io_lo1)
{
    uint64_t a0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    { A3_CHARGE(); v0 = a1_sx64(isysGetThisSubThread(ram, rt)); }
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); scrInit(ram, rt); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const a1::RetVF a4_t = PauseGame(ram, rt, a0, w64_t1, (uint32_t)(sp + 0x0u), hi1, lo1); io_hi1 = hi1; io_lo1 = lo1; return a4_t; }
    __builtin_unreachable();
}

// GetStageContextByPos (0x1365cc0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out h2 h4 h5 h6 h7, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t GetStageContextByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { A1_W32B((uint32_t)s0, 4, (uint32_t)a1); A3_CHARGE(); uint64_t a7c[12] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, a3, 0u, 0u, t2, t3}; v0 = GetStageContext(ram, rt, a0, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); a1 = a7c[5]; a2 = a7c[6]; a3 = a7c[7]; t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; }
    v1 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25512))));
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1365cf8; }
    v1 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25516))));
    if ((int64_t)a0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 32)))); goto L_1365cf8; }
L_1365cf8:;
    { if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// GetStageCenter (0x1365f98): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in -, out h2 h3 h4 h5 h6 h7)
inline uint64_t GetStageCenter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    sp = sp - 0x50u;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { A1_W32B(sp, 4, a1); A3_CHARGE(); uint64_t a7c[12] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), 0u, 0u, a3, 0u, 0u, t2, t3}; v0 = GetStageContext(ram, rt, a0, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); f0 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); a3 = a7c[7]; t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; }
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 48))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h3, v1));
    { sp = sp + 0x50u; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f2); xo[2] = a1_bits(f3); xo[3] = a1_bits(f4); xo[4] = a1_bits(f5); xo[5] = a3; xo[6] = t0; xo[7] = t1; xo[8] = t2; xo[9] = t3; } io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// GetStageName (0x1365fd0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in h4, out h2 h4 h5 h6 h7, relocated calls 1)
inline uint64_t GetStageName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t a1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22928))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    { A1_W32B((uint32_t)sp, 4, (uint32_t)v0); A3_CHARGE(); uint64_t a7c[12] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, a3, 0u, 0u, t2, t3}; v0 = GetStageContext(ram, rt, a0, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); a3 = a7c[7]; t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; }
    (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a6x::strcpy(ram, rt, a0, (uint32_t)a1, h2, h4); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; } io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// GetStageAreaNumber (0x1366010): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f3 xo[2]=f4 xo[3]=f5 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out h2 h4 h5 h6 h7)
inline uint64_t GetStageAreaNumber(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    sp = sp - 0x50u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22928))));
    { A1_W32B(sp, 4, (uint32_t)v0); A3_CHARGE(); uint64_t a7c[12] = {(uint64_t)a1_bits(f0), 0u, (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), 0u, 0u, 0u, 0u, 0u, t2, t3}; v0 = GetStageContext(ram, rt, a0, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); f0 = a1_flt((uint32_t)a7c[0]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; }
    v1 = 0x1460000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25512))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_136604c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 25516))));
    if (v1 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 32)))); goto L_136604c; }
L_136604c:;
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    { sp = sp + 0x50u; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f3); xo[2] = a1_bits(f4); xo[3] = a1_bits(f5); xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// GetStageAreaNumberByPos (0x1366060): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=a0 xo[6]=a1 xo[7]=a2 xo[8]=a3 xo[9]=t0 xo[10]=t1 xo[11]=t2 xo[12]=t3; VU0 in -, out h2 h4 h5 h6 h7)
inline uint64_t GetStageAreaNumberByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    sp = sp - 0x50u;
    A1_W32B(sp, 4, (uint32_t)a0);
    { (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[12] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), 0u, 0u, a3, 0u, 0u, t2, t3}; v0 = GetStageContext(ram, rt, a0, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); f0 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); a1 = a7c[5]; a2 = a7c[6]; a3 = a7c[7]; t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; }
    v1 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25512))));
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1366094; }
    v1 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25516))));
    if ((int64_t)a0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 32)))); goto L_1366094; }
L_1366094:;
    { sp = sp + 0x50u; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f2); xo[2] = a1_bits(f3); xo[3] = a1_bits(f4); xo[4] = a1_bits(f5); xo[5] = a0; xo[6] = a1; xo[7] = a2; xo[8] = a3; xo[9] = t0; xo[10] = t1; xo[11] = t2; xo[12] = t3; } io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// GetStageNumberByPos (0x13660a0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out h2 h3 h4 h5 h6 h7)
inline uint64_t GetStageNumberByPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    sp = sp - 0x50u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22928))));
    { A1_W32B(sp, 4, (uint32_t)v0); A3_CHARGE(); v0 = GetStageContext(ram, rt, a0, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7); }
    v1 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25512))));
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1366114; }
    v1 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25516))));
    if ((int64_t)a0 == 0) goto L_1366118;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 32))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 36))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (a0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)31));
    { (a0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0))); A3_CHARGE(); v0 = getstagenumber_u14(ram, rt, a0, h3); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 15))));
    v1 = A1_R32B(sp, 36);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    v1 = v1 & 0x1Fu;
    (a0 = (uint64_t)(RC_U64X(a0) | a1_sx64(v1)));
    { (a0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0))); A3_CHARGE(); v0 = getstagenumber_u14(ram, rt, a0, h3); }
L_1366114:;
L_1366118:;
    { sp = sp + 0x50u; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// GetStageNumberNumber (0x1366670): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=a1 xo[6]=a2 xo[7]=a3 xo[8]=t0 xo[9]=t1 xo[10]=t2 xo[11]=t3; VU0 in -, out h2 h3 h4 h5 h6 h7)
inline uint64_t GetStageNumberNumber(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    sp = sp - 0x50u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22928))));
    { A1_W32B(sp, 4, (uint32_t)v0); A3_CHARGE(); uint64_t a7c[12] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), 0u, 0u, a3, 0u, 0u, t2, t3}; v0 = GetStageContext(ram, rt, a0, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); f0 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); a1 = a7c[5]; a2 = a7c[6]; a3 = a7c[7]; t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; }
    v1 = 0x1460000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 25512))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_13666e4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 25516))));
    { const bool c = v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 32)))); if (c) goto L_13666e4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 36))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (a0 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)31));
    { (a0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0))); A3_CHARGE(); v0 = getstagenumber_x14(ram, rt, a0, h3); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 15))));
    v1 = A1_R32B(sp, 36);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    v1 = v1 & 0x1Fu;
    (a0 = (uint64_t)(RC_U64X(a0) | a1_sx64(v1)));
    { (a0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0))); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; v0 = getstagenumber_x14(ram, rt, a0, h3, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_13666e4:;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { sp = sp + 0x50u; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f2); xo[2] = a1_bits(f3); xo[3] = a1_bits(f4); xo[4] = a1_bits(f5); xo[5] = a1; xo[6] = a2; xo[7] = a3; xo[8] = t0; xo[9] = t1; xo[10] = t2; xo[11] = t3; } io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// SCRFuncBirdSetAttainPosForPathData (0x136e8c8): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat h2, relocated calls 4)
inline uint64_t SCRFuncBirdSetAttainPosForPathData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967136))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { (fp = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getPathInfo(ram, rt, (uint32_t)a0)); }
    (s7 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)a1_sx64(s7) + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointDataFromEvent(ram, rt, v0, a0, a1, a2); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 20))));
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { A1_W32B((uint32_t)sp, 0, (uint32_t)v0); A3_CHARGE(); v0 = getPointDataFromEvent(ram, rt, v0, a0, a1, a2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    { const bool c = (int64_t)a1 == 0; A1_W32B((uint32_t)sp, 4, (uint32_t)v0); if (c) goto L_136ea98; }
    if ((int64_t)v0 == 0) goto L_136ea7c;
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (s4 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
L_136e970:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s5, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 4))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s7, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0)))); A3_CHARGE(); v0 = a1_sx64(getConnectLineArray(ram, rt, a0, a1)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 8))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v0, 0)))); A3_CHARGE(); v0 = getLineData(ram, rt, a0, a1); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    if ((int64_t)v1 == 0) { goto L_136ea9c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0))));
    if ((int64_t)v0 == (int64_t)a1) { (a1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 2)))); goto L_136e9ac; }
L_136e9ac:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 4)))); A3_CHARGE(); v0 = getPointData(ram, rt, a0, a1); }
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)fp, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf7));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 32, bits); }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 4)))); if (c) { A3_BACK(); goto L_136e970; } }
    { uint32_t bits = A1_R32B((uint32_t)sp, 32); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 36); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); if (c) goto L_136ea90; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
L_136ea7c:;
    { (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    goto L_136ea9c;
L_136ea90:;
    { (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
L_136ea98:;
L_136ea9c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// SCRFuncBirdSetAttainNearPosForPathData (0x136efb8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf7, out vf5 vf6 vf7 h2, relocated calls 1)
inline uint64_t SCRFuncBirdSetAttainNearPosForPathData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2)
{
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    sp = sp - 0x50u;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointDataFromEvent(ram, rt, v0, a0, a1, a2); }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 20))));
    { A1_W32B(sp, 0, (uint32_t)v0); A3_CHARGE(); v0 = getPointDataFromEvent(ram, rt, v0, a0, a1, a2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a1 == 0; A1_W32B(sp, 4, (uint32_t)a2); if (c) goto L_136f0a0; }
    if ((int64_t)a2 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 308)))); goto L_136f040; }
    { (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    goto L_136f0a4;
L_136f040:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_136f098; }
    { A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    goto L_136f0a4;
L_136f098:;
    { (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
L_136f0a0:;
L_136f0a4:;
    { sp = sp + 0x50u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// SCRFuncBirdSetAttainFarPosForPathData (0x136f130): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf7, out vf5 vf6 vf7 h2, relocated calls 1)
inline uint64_t SCRFuncBirdSetAttainFarPosForPathData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2)
{
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    sp = sp - 0x50u;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointDataFromEvent(ram, rt, v0, a0, a1, a2); }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 20))));
    { A1_W32B(sp, 0, (uint32_t)v0); A3_CHARGE(); v0 = getPointDataFromEvent(ram, rt, v0, a0, a1, a2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a1 == 0; A1_W32B(sp, 4, (uint32_t)a2); if (c) goto L_136f218; }
    if ((int64_t)a2 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 308)))); goto L_136f1b8; }
    { (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    goto L_136f21c;
L_136f1b8:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_136f210; }
    { A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    goto L_136f21c;
L_136f210:;
    { (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
L_136f218:;
L_136f21c:;
    { sp = sp + 0x50u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013813A0 (0x13813a0): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f0 xo[1]=f12 xo[2]=f13 xo[3]=f20 xo[4]=f21 xo[5]=f22 xo[6]=a0 xo[7]=a1 xo[8]=a2 xo[9]=a3 xo[10]=t0 xo[11]=t1 xo[12]=t2 xo[13]=t3; VU0 in vf0 vf2 vf7 vf8 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vrr vstat h2 h3 h4 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vrr vstat h2 h3 h4 h8 h9 h10 h11 h12 h13 h14 h15, jump tables 1, relocated calls 1)
inline uint64_t gamecore_sub_013813A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t0, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, a4::vec4 &io_vrr, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a3 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    a4::vec4 vrr = io_vrr;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    sp = sp - 0x70u;
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 104, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 56))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 60))));
    { const bool c = (int64_t)v1 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295)))); if (c) goto L_13817ec; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s3, 72, 0u); if (c) goto L_13817ec; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, -31280))));
    { const uint32_t a5_jt = (uint32_t)v1; switch (a5_jt) { case 0x1381408u: goto L_1381408; case 0x1381600u: goto L_1381600; case 0x1381620u: goto L_1381620; case 0x1381660u: goto L_1381660; case 0x13816C0u: goto L_13816c0; case 0x1381720u: goto L_1381720; case 0x1381738u: goto L_1381738; case 0x13817D8u: goto L_13817d8; case 0x13817E0u: goto L_13817e0; case 0x13817ECu: goto L_13817ec; default: A5_BADJUMP(0x13813FCu, a5_jt); } }
L_1381408:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s3, 152, 0u);
    A1_W32B((uint32_t)s3, 140, (uint32_t)v0);
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 256))));
    A1_W32B((uint32_t)s3, 156, 0u);
    (a0 = (uint64_t)((uint64_t)RC_U64X(s4) + (uint64_t)0ULL)); a3xw6 = true;
    A1_W32B((uint32_t)s3, 148, 0u);
    A1_W32B((uint32_t)s3, 144, 0u);
    { A1_W32B((uint32_t)s3, 136, 0u); A3_CHARGE(); { const a1::RetVF a2_r = gamecore_sub_0137EA00(ram, rt, (uint32_t)a0, vf0); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    A1_W32B((uint32_t)s3, 76, 0u);
    { uint32_t bits = A1_R32B((uint32_t)s3, 124); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15820 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 192))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    f0 = (-(float)(f0)); a3xw0 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16544 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)s3, 128, (uint32_t)v1);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 852, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 848, bits); }
    A1_W32B((uint32_t)s3, 856, 0u);
    A1_W32B((uint32_t)s3, 860, 0u);
    A1_W32B((uint32_t)s3, 864, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 112))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 116)))); a3xw6 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf5));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 176))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); a3xw7 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    A4_W128(ADD32((uint32_t)v1, 0), a4::mk128(h2, v0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v1, 12, bits); }
    goto L_13814cc;
L_13814b0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967280))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf7));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v1, 12, bits); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); a3xw7 = true;
L_13814cc:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 132))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 16)))); a3xw6 = true; if (c) { A3_BACK(); goto L_13814b0; } }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 192))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 368))));
    A4_W128(ADD32((uint32_t)s4, 48), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 176))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s4, 80), a4::mk128(h2, v0));
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf0));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 352))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw6 = true;
    (a1 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL)); a3xw7 = true;
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16)))); a3xw8 = true; A3_CHARGE(); uint64_t a7c[1] = {0u}; { const a1::RetVF a2_r = gamecore_sub_0137F508(ram, rt, a0, a1, a2, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, vf0, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, h4, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f21 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; a3xw4 = true; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 320))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf5));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 176))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x138153Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1381540u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16)))); a3xw6 = true;
    A4_W128(ADD32((uint32_t)s4, 128), a4::mk128(h2, v0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 22096)))); a3xw6 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s4, 144), a4::mk128(h2, v0));
    { uint32_t bits = A1_R32B((uint32_t)s3, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 160, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s3, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 164, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f1 = a1f::mul(f1, f0);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 160, bits); } goto L_1381588; }
L_1381588:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 288))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 304))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 336))));
    { uint32_t bits = A1_R32B((uint32_t)s4, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } a3xw1 = true;
    (t1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); a3xw11 = true;
    (a0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); a3xw6 = true;
    (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); a3xw7 = true;
    (a2 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL)); a3xw8 = true;
    (a3 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); a3xw9 = true;
    { (t0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelC3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw6 = true;
    { uint32_t bits = A1_R32B((uint32_t)s4, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } a3xw1 = true;
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw7 = true;
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw8 = true;
    (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw9 = true;
    (t0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); a3xw10 = true;
    { (t1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw11 = true; A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelA3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 272)))); a3xw6 = true;
    { uint32_t bits = A1_R32B((uint32_t)s4, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } a3xw1 = true;
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw7 = true;
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw8 = true;
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw9 = true;
    (t0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw10 = true;
    { (t1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw11 = true; A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f12)}; { const a1::RetVF a2_r = AI_GetAccelB3XYZ_x14(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; a3xw1 = true; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 384))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s3, 784), a4::mk128(h2, v0));
    goto L_13817ec;
L_1381600:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16544 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw6 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 256)))); a3xw7 = true;
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); } A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = gamecore_sub_0137FD70(ram, rt, a0, a1, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf5, vf6, vf7, vf8, vf9, vacc, vq, vrr, vstat, h2, h8, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); a0 = a7c[1]; a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; a3xw0 = true; a3xw1 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; }
    goto L_13817f0;
L_1381620:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 140))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v1 == 0; A1_W32B((uint32_t)s3, 136, (uint32_t)v0); if (c) goto L_1381638; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 832))));
    A1_W32B((uint32_t)s3, 144, (uint32_t)v0);
L_1381638:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16448 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw6 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16544 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 256)))); a3xw7 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 860, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 848, bits); }
    goto L_1381754;
L_1381660:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16448 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 256))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 8)))); a3xw6 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 384))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 848, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 860, bits); }
    { A4_W128(ADD32(sp, 0), a4::mk128(h2, v0)); A3_CHARGE(); v0 = GetLayoutClass_u14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw7 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw1 = true;
    f13 = ((float)(f20)); a3xw2 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16)))); a3xw8 = true;
    { A4_W128(ADD32(sp, 16), a4::mk128(h2, v0)); A3_CHARGE(); uint64_t a7c[12] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = gamecore_sub_0137F850_x14(ram, rt, a0, a1, a2, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf28, vf29, vacc, vq, vstat, h2, h3, h4, h8, h9, h10, h11, h12, h13, h14, h15, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); f21 = a1_flt((uint32_t)a7c[2]); f22 = a1_flt((uint32_t)a7c[3]); a0 = a7c[4]; a1 = a7c[5]; a2 = a7c[6]; a3 = a7c[7]; t0 = a7c[8]; t1 = a7c[9]; t2 = a7c[10]; t3 = a7c[11]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 384))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
    goto L_13817ec;
L_13816c0:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 816))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16544 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 848, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
    A1_W32B((uint32_t)s3, 140, 0u);
    A1_W32B((uint32_t)s3, 864, 0u);
    A1_W32B((uint32_t)s3, 860, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s3, 852); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 856, bits); }
    goto L_13817ec;
L_1381720:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s3, 416); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 848, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
    goto L_13817ec;
L_1381738:;
    { uint32_t bits = A1_R32B((uint32_t)s3, 416); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw6 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 256)))); a3xw7 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 848, bits); }
L_1381754:;
    { A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; { const a1::RetVF a2_r = gamecore_sub_0137EB38_x14(ram, rt, a0, a1, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vacc, vq, vstat, h3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; a3xw0 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 432)))); a3xw6 = true;
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 448))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 464))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 480))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 496))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 512))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 528))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 544))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 560))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 576))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15624 << 16))));
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)43909));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    A1_W32B((uint32_t)a0, 160, 0u);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 164, bits); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 816))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 800))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s3, 400), a4::mk128(h2, v0));
    A4_W128(ADD32((uint32_t)s3, 384), a4::mk128(h3, v1));
    goto L_13817ec;
L_13817d8:;
    A1_W32B((uint32_t)s3, 68, 0u);
    goto L_13817ec;
L_13817e0:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16448 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 68, bits); }
L_13817ec:;
L_13817f0:;
    { uint32_t bits = A1_R32B(sp, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { sp = sp + 0x70u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f12); if (a3xw2) xo[2] = a1_bits(f13); xo[3] = a1_bits(f20); if (a3xw4) xo[4] = a1_bits(f21); if (a3xw5) xo[5] = a1_bits(f22); if (a3xw6) xo[6] = a0; if (a3xw7) xo[7] = a1; if (a3xw8) xo[8] = a2; if (a3xw9) xo[9] = a3; if (a3xw10) xo[10] = t0; if (a3xw11) xo[11] = t1; if (a3xw12) xo[12] = t2; if (a3xw13) xo[13] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vrr = vrr; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// SCRConditionHorseNeighPermission (0x1396438): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out h2, break)
inline a1::RetVF SCRConditionHorseNeighPermission(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 8, bits); }
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16672 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 16548); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f20 = a1f::mul(f20, f1);
    f0 = a1f::mul(f0, f1);
    { int32_t tmp = a1f::cvtw(f20); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    if ((int64_t)v1 == 0) { A6_BREAK(0x1396475u); goto L_1396478; }
L_1396478:;
    { int32_t divisor = (int32_t)v1;    int32_t dividend = (int32_t)a0;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    { uint32_t bits = A1_R32B((uint32_t)sp, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    (v0 = (uint64_t)(hi));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SCRConditionKirinIntervalOfMovement (0x139cdb8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out h2, break)
inline a1::RetVF SCRConditionKirinIntervalOfMovement(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    if ((int64_t)s0 == 0) { A6_BREAK(0x139CDD9u); goto L_139cddc; }
L_139cddc:;
    { uint32_t bits = A1_R32B((uint32_t)v0, 16548); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)s0;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(hi));
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int64_t)s1) ? 1 : 0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SCRConditionKameNextTime (0x1404ff0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out h2, break)
inline a1::RetVF SCRConditionKameNextTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 16548); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    if ((int64_t)s0 == 0) { A6_BREAK(0x140500Du); goto L_1405010; }
L_1405010:;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int32_t divisor = (int32_t)s0;    int32_t dividend = (int32_t)v1;    if (divisor != 0) {        if (divisor == -1 && dividend == INT32_MIN) {            lo = (uint64_t)(int64_t)INT32_MIN; hi = 0;        } else {            lo = (uint64_t)(int64_t)(dividend / divisor);            hi = (uint64_t)(int64_t)(dividend % divisor);        }    } else {        lo = (dividend < 0) ? 1ull : 0xFFFFFFFFFFFFFFFFull; hi = (uint64_t)(int64_t)dividend;    } }
    (v0 = (uint64_t)(hi));
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)1) ? 1 : 0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01426370 (0x1426370): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f2 xo[1]=f12 xo[2]=f13 xo[3]=f14 xo[4]=f20 xo[5]=f21 xo[6]=f22 xo[7]=a3 xo[8]=t0 xo[9]=t1; VU0 in h5, out hi1 lo1 h2 h3, 64-bit inputs a2, guest sp, frame 32 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_01426370(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t w64_a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, float f0, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5, uint64_t *xo)
{
    float f2 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint32_t a2 = (uint32_t)w64_a2;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    { uint32_t bits = 0u; std::memcpy(&f12, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f13, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[11] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = setDemoBar(ram, rt, w64_a2, a3, t0, t1, (uint32_t)(sp + 0xFFFFFFE0u), f0, f12, f13, f20, f21, f22, hi1, lo1, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[1]); f13 = a1_flt((uint32_t)a7c[2]); f20 = a1_flt((uint32_t)a7c[3]); f21 = a1_flt((uint32_t)a7c[4]); f22 = a1_flt((uint32_t)a7c[5]); t1 = a7c[10]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    if ((int64_t)v1 == 0) goto L_14263e8;
L_14263a8:;
    if ((int32_t)v1 <= 0) goto L_14263c8;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, t1}; gameModeCls_x14(ram, rt, a0, a7c); t1 = a7c[2]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    goto L_14263d8;
L_14263c8:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); v0 = gameModeSet(ram, rt, a0); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
L_14263d8:;
    A1_W32B((uint32_t)s0, 128, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_14263a8; } }
L_14263e8:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 572)))); A3_CHARGE(); v0 = a1_sx64(CAM_get_target_obj(ram, rt, (uint32_t)a0)); }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = nico_playerSetAttentionHeadActFlag(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2, h5); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; const a1::RetVF a7_t = gamecore_sub_01423410_x14(ram, rt, a0, f20, f21, h2, h3, a7c); f2 = a1_flt((uint32_t)a7c[0]); f12 = a1_flt((uint32_t)a7c[1]); f13 = a1_flt((uint32_t)a7c[2]); f14 = a1_flt((uint32_t)a7c[3]); f20 = a1_flt((uint32_t)a7c[4]); f21 = a1_flt((uint32_t)a7c[5]); a3 = a7c[6]; t0 = a7c[7]; if (xo) { xo[0] = a1_bits(f2); xo[1] = a1_bits(f12); xo[2] = a1_bits(f13); xo[3] = a1_bits(f14); xo[4] = a1_bits(f20); xo[5] = a1_bits(f21); xo[6] = a1_bits(f22); xo[7] = a3; xo[8] = t0; xo[9] = t1; } io_hi1 = hi1; io_lo1 = lo1; io_h2 = h2; io_h3 = h3; return a7_t; }
    __builtin_unreachable();
}

// gamecore_sub_01426418 (0x1426418): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f2 xo[1]=f12 xo[2]=f13 xo[3]=f14 xo[4]=f20 xo[5]=f21 xo[6]=f22 xo[7]=a3 xo[8]=t0 xo[9]=t1; VU0 in h5, out hi1 lo1 h2 h3, 64-bit inputs a2, guest sp, frame 32 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_01426418(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t w64_a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, float f0, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, uint64_t &io_hi1, uint64_t &io_lo1, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5, uint64_t *xo)
{
    float f2 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint32_t a2 = (uint32_t)w64_a2;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t hi1 = io_hi1;
    uint64_t lo1 = io_lo1;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    { uint32_t bits = 0u; std::memcpy(&f12, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f13, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[11] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = setDemoBar(ram, rt, w64_a2, a3, t0, t1, (uint32_t)(sp + 0xFFFFFFE0u), f0, f12, f13, f20, f21, f22, hi1, lo1, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[1]); f13 = a1_flt((uint32_t)a7c[2]); f20 = a1_flt((uint32_t)a7c[3]); f21 = a1_flt((uint32_t)a7c[4]); f22 = a1_flt((uint32_t)a7c[5]); t1 = a7c[10]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    if ((int64_t)v1 == 0) goto L_1426490;
L_1426450:;
    if ((int32_t)v1 <= 0) goto L_1426470;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, t1}; gameModeCls_x14(ram, rt, a0, a7c); t1 = a7c[2]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    goto L_1426480;
L_1426470:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); v0 = gameModeSet(ram, rt, a0); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
L_1426480:;
    A1_W32B((uint32_t)s0, 128, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 128))));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1426450; } }
L_1426490:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 572)))); A3_CHARGE(); v0 = a1_sx64(CAM_get_target_obj(ram, rt, (uint32_t)a0)); }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = nico_playerSetAttentionHeadActFlag(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2, h5); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; const a1::RetVF a7_t = gamecore_sub_01423410_x14(ram, rt, a0, f20, f21, h2, h3, a7c); f2 = a1_flt((uint32_t)a7c[0]); f12 = a1_flt((uint32_t)a7c[1]); f13 = a1_flt((uint32_t)a7c[2]); f14 = a1_flt((uint32_t)a7c[3]); f20 = a1_flt((uint32_t)a7c[4]); f21 = a1_flt((uint32_t)a7c[5]); a3 = a7c[6]; t0 = a7c[7]; if (xo) { xo[0] = a1_bits(f2); xo[1] = a1_bits(f12); xo[2] = a1_bits(f13); xo[3] = a1_bits(f14); xo[4] = a1_bits(f20); xo[5] = a1_bits(f21); xo[6] = a1_bits(f22); xo[7] = a3; xo[8] = t0; xo[9] = t1; } io_hi1 = hi1; io_lo1 = lo1; io_h2 = h2; io_h3 = h3; return a7_t; }
    __builtin_unreachable();
}

// SCRFuncLineCut (0x144f918): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRFuncLineCut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    a2 = 4u;
    { const uint64_t a4_t = setEventLineAttr(ram, rt, v0, a0, a1, a1_sx64(a2)); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// SCRFuncLineConnect (0x144f948): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRFuncLineConnect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    a2 = 4u;
    { const uint64_t a4_t = unsetEventLineAttr(ram, rt, v0, a0, a1, a1_sx64(a2)); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// SCRFuncLineSetRoute (0x144f978): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRFuncLineSetRoute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    a2 = 2u;
    { const uint64_t a4_t = setEventLineAttr(ram, rt, v0, a0, a1, a1_sx64(a2)); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// SCRFuncLineUnsetRoute (0x144f9a8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRFuncLineUnsetRoute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    a2 = 2u;
    { const uint64_t a4_t = unsetEventLineAttr(ram, rt, v0, a0, a1, a1_sx64(a2)); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// SCRFuncLineSetForcePrio (0x144f9d8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRFuncLineSetForcePrio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    a2 = 0x81u;
    { const uint64_t a4_t = setEventLinePrio(ram, rt, v0, a0, a1, a1_sx64(a2)); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// SCRFuncLineAllReset (0x144fa08): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRFuncLineAllReset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint64_t h2 = io_h2;
    { A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const uint64_t a4_t = allResetLineAttr(ram, rt, v0, (uint32_t)a0); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// SCRFuncSetAttainPointPosForLanding (0x144fe48): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out h2 h3)
inline uint64_t SCRFuncSetAttainPointPosForLanding(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3)
{
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 16528))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; a3 = 4u; if (c) goto L_144fe88; }
    if (v1 != a3) goto L_144fea4;
L_144fe88:;
    { A3_CHARGE(); v0 = getPointPosFromEvent(ram, rt, v0, a0, a1, a2, h3); }
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_144fea0; }
    { (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
L_144fea0:;
L_144fea4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SCRFuncSetAttainPointPosForEventID (0x1452198): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out h2 h3)
inline uint64_t SCRFuncSetAttainPointPosForEventID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPointPosFromEvent(ram, rt, v0, a0, a1, a2, h3); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_14521d8; }
    { A3_CHARGE(); v0 = setAttainmentPos(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
L_14521d8:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_add_cur_stage_ofst_position (0x13433b8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in -, out vf5 vf6 h2 h3 h4 h5 h6 h7, relocated calls 1)
inline uint64_t GAMEUTIL_get_add_cur_stage_ofst_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(get_main_cameraman_camera_obj(ram, rt)); }
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(get_camera_position(ram, rt, (uint32_t)a0)); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[10] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetStageCenter(ram, rt, a0, (uint32_t)a1, a3, t2, t3, (uint32_t)sp, f0, f2, f3, f4, f5, f12, f13, f14, f15, f16, f17, f18, f19, h2, h3, h4, h5, h6, h7, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); if (xo) { xo[0] = a1_bits(f0); xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// resetUserPathWork (0x1431da8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in vf0 vf5 vf6 vf7 vf8 h5, out vf4 vf5 vf6 vf7 vf8 h2 h4 h5 h6 h7, relocated calls 2, guest sp, frame 32 bytes, 0 slot accesses)
inline uint64_t resetUserPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a3, uint64_t t2, uint64_t t3, uint32_t sp, float f2, float f3, float f4, float f5, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h6, uint64_t &io_h7, uint64_t *xo)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { s1 = 0xFFFFFFFFu; A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 164))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 152))));
    A1_W32B((uint32_t)s0, 896, 0u);
    A1_W32B((uint32_t)s0, 144, 0u);
    A1_W32B((uint32_t)s0, 168, 0u);
    A1_W32B((uint32_t)s0, 172, 0u);
    A1_W32B((uint32_t)s0, 128, s1);
    A1_W32B((uint32_t)s0, 132, s1);
    A1_W32B((uint32_t)s0, 136, s1);
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)s0, 140, s1); if (c) goto L_1431e00; }
    { A1_W32B((uint32_t)s0, 164, 0u); A3_CHARGE(); v0 = a1_sx64(DeleteTwoWayLink(ram, rt, (uint32_t)a0)); }
L_1431e00:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16457 << 16))));
    at = at | 0xFDAu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    A1_W32B((uint32_t)s0, 932, s1);
    A1_W32B((uint32_t)s0, 148, 0u);
    A1_W32B((uint32_t)s0, 1032, 0u);
    A1_W32B((uint32_t)s0, 1036, 0u);
    A1_W32B((uint32_t)s0, 1040, 0u);
    A1_W32B((uint32_t)s0, 1052, s1);
    A1_W32B((uint32_t)s0, 1056, s1);
    A1_W32B((uint32_t)s0, 1060, s1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 228, bits); }
    A1_W32B((uint32_t)s0, 1064, s1);
    A1_W32B((uint32_t)s0, 1068, s1);
    A1_W32B((uint32_t)s0, 1072, s1);
    A1_W32B((uint32_t)s0, 224, 0u);
    A1_W32B((uint32_t)s0, 928, s1);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 308))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h2, v0));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 308))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 16), a4::mk128(h2, v0));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 316))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf6, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf8 = a4v::blend(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<3>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf8, a4v::bcast<1>(vf8)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf6));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308)))); A3_CHARGE(); uint64_t a7c[13] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetStageAreaNumberByPos(ram, rt, a0, a3, t2, t3, (uint32_t)(sp + 0xFFFFFFE0u), f0, f2, f3, f4, f5, f12, f13, f14, f15, f16, f17, f18, f19, h2, h4, h5, h6, h7, a7c); f0 = a1_flt((uint32_t)a7c[0]); a2 = a7c[7]; a3 = a7c[8]; t0 = a7c[9]; t1 = a7c[10]; t2 = a7c[11]; t3 = a7c[12]; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)s0, 940, (uint32_t)v0);
    A1_W32B((uint32_t)s0, 936, (uint32_t)v0);
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } const uint32_t a4_t = setDataHeaderFromAreaIdx(ram, rt); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h6 = h6; io_h7 = h7; return a1_sx64(a4_t); }
    __builtin_unreachable();
}

// get_apply_camera_region (0x1163ef0): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f0 xo[1]=f21 xo[2]=f22 xo[3]=f23; VU0 in vf0 vf2 vf7 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc h2 h3 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15)
inline uint64_t get_apply_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint32_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 104, bits); }
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 120, bits); }
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 112, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 96, bits); }
    { { uint32_t bits = A1_R32B((uint32_t)v0, 27384); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; } A3_CHARGE(); v0 = GetLayoutClass_u14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0)); A3_CHARGE(); v0 = manager_sub_011641E0(ram, rt, a0, a1, sp); }
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)s2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)291 << 16)))); if (c) goto L_1163f80; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)291 << 16))));
L_1163f80:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f23, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f22, &bits, sizeof(bits)); }
    (s5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294948752))));
    s4 = 0x31u;
L_1163f98:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s5, 0))));
    if ((int64_t)s0 == 0) { s4 = s4 - 1u; goto L_116404c; }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16)))); A3_CHARGE(); v0 = GetLayoutClass_u14(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int64_t)s3) ? 1 : 0));
    if ((int64_t)v0 != 0) { s4 = s4 - 1u; goto L_116404c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    (v0 = (uint64_t)(((int64_t)(int64_t)s3 < (int64_t)(int64_t)RC_U64X(a0)) ? 1 : 0));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { const bool c = (int64_t)s0 != (int64_t)s6; (t0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); if (c) goto L_1164018; }
    f20 = a1f::sub(f20, f23);
    fcr31 = (a1f::lt(f20, f22)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1164018;
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
L_1164018:;
    if ((int64_t)v0 != 0) goto L_116402c;
    fcr31 = (a1f::lt(f20, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { s4 = s4 - 1u; goto L_116404c; }
L_116402c:;
    { A3_CHARGE(); uint64_t a7c[14] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f13), (uint64_t)a1_bits(f14), (uint64_t)a1_bits(f20), 0u, 0u, 0u, 0u, t1, t2, t3}; v0 = is_clear_camera_region_condition(ram, rt, v0, a0, a1, a2, a3, t0, t1, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, h2, h3, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f12 = a1_flt((uint32_t)a7c[3]); f13 = a1_flt((uint32_t)a7c[4]); f14 = a1_flt((uint32_t)a7c[5]); f20 = a1_flt((uint32_t)a7c[6]); t1 = a7c[11]; t2 = a7c[12]; t3 = a7c[13]; a3xw0 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v0 != (int64_t)v1; s4 = s4 - 1u; if (c) goto L_116404c; }
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    (s2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    f21 = ((float)(f20));
L_116404c:;
    { const bool c = (int32_t)s4 >= 0; s5 = s5 + 4u; if (c) { A3_BACK(); goto L_1163f98; } }
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)sp, 120); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 96); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f21); xo[2] = a1_bits(f22); xo[3] = a1_bits(f23); } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}
