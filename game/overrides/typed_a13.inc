// typed_a13.inc: round A13's typed functions (tools/typed_lift2.py, stage 13: hi / lo as extra outputs). Generated: do not edit. Included inside a namespace (typed_a13.h: namespace typed), after typed_a7.inc. First the variants of earlier functions (<name>_x13: the same MIPS typed with extra outputs, called by stage-13 functions only), then stage 13's functions; every one declared first.
// the recursive functions (A13_REC index): 
// every function of the stage declared first (variants of stage-13 functions call stage-13 functions)
inline uint32_t SetCurrentQuaternion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h5, uint64_t *xo = nullptr);
inline uint32_t PopQuaternion_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_h3);
inline uint32_t getBlendLastMotion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr);
inline uint64_t GetCharacterClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo = nullptr);
inline uint64_t GetLightClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t GetScriptAnimationClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t dynamic_cast_to_camera_obj_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t *xo = nullptr);
inline uint64_t PushQuaternion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline void GetStabYRotQuaternion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo = nullptr);
inline uint64_t GetMotionNbOptionNodes_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t GetMotionOptionNodePhysicalId_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t GetMotionVersion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint32_t getAnimObjResource_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t GetScriptLayoutClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr);
inline float manager_sub_010AE128_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t GetMotionOptionNodeIntFrame_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t io_h6, uint64_t *xo = nullptr);
inline a1::RetVF manager_sub_010B8590_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr);
inline uint64_t GetOptionNodePhysicalNodeId_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t GetLayoutClass_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo = nullptr);
inline a1::RetVF GetMotionMatrix_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t *xo = nullptr);
inline uint64_t GetMotionOptionNodeFloatFrame_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t GetCharacterClassNodeQuaternionPos_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t *xo = nullptr);
inline void AddCharacterClassOptionNodeSubGeometry_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline void AddCharacterClassOptionNodeSubGeometries_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t GetCharacterClassOptionNodeGeo_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t manager_sub_010AC4A0_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t iosMvecAllOne(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t hi, uint64_t lo, uint64_t &io_h2);
inline uint64_t iosSWToMmColMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2);
inline uint64_t iosMmColToSWMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2);
inline uint64_t iosITOF15ToMmColMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2);
inline uint64_t iosMmColToITOF15Mvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2);
inline uint64_t gamecore_sub_01386308(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t *xo = nullptr);
inline uint64_t iosFvecToMmColMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2);
inline uint64_t iosMmColToFvecMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2);
inline uint64_t GetAnimObjResource(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline uint64_t gamecore_sub_01386770(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f20, float f21, float f22, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr);
inline a1::RetVF GetMorphAnimation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo = nullptr);
inline a1::RetVF charrootdyn_acceptMotionRootOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr);
inline uint64_t GetMotionNodeQuaternionPosToolCoord(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t updateLightGeo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t w64_t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t update_camera_region_geometry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr);
inline uint64_t nico_player_edgecolfit_HandEffector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8);
inline uint64_t nico_player_colfit_HandEffector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8);
inline uint64_t nico_player_pendulum_HandEffector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t w64_t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8);

// SetCurrentQuaternion_x13 (0x1bb988): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a1; VU0 in -, out h5)
inline uint32_t SetCurrentQuaternion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h5, uint64_t *xo)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t h5 = io_h5;
    v0 = 0x1E0000u;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    v1 = A1_R32B(v0, 3936);
    v1 = v1 << 4;
    at = 0x200000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, 10944))));
    v1 = at + v1;
    { A4_W128(ADD32(v1, 0), a4::mk128(h5, a1)); if (xo) { xo[0] = a1; } io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// PopQuaternion_u13 (0x1bbc08): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h3)
inline uint32_t PopQuaternion_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t h3 = io_h3;
    v0 = 0x1E0000u;
    v0 = v0 + 0xF60u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(a0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A1_W32B(v0, 0, (uint32_t)v1); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// getBlendLastMotion_x13 (0x10b28e0): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=hi xo[4]=lo; VU0 in -, out -)
inline uint32_t getBlendLastMotion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw3 = false;
    bool a3xw4 = false;
    v0 = 0x30000u;
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    { const bool c = v0 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0)))); if (c) goto L_10b2900; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_10b2904;
L_10b2900:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2904:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B2908u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b2938; }
    { const uint32_t _dynInsn = A1_R32(0x10B290Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; v0 = a2 >> 18; if (c) goto L_10b2938; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v0 = v0 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); } a3xw3 = true; a3xw4 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    goto L_10b293c;
L_10b2938:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b293c:;
    t0 = 0x30000u;
    { const uint32_t _dynInsn = A1_R32(0x10B2940u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a3 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x10B2948u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t1 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    t0 = t0 | 0xFFFFu;
L_10b2950:;
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a3, 12))));
    v1 = A1_R32(a2);
    v0 = v1 & t0;
    { const bool c = v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a2, v0)))); if (c) goto L_10b2970; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    goto L_10b2974;
L_10b2970:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2974:;
    { const bool c = (int64_t)a0 == 0; v0 = 0u; if (c) goto L_10b29ac; }
    v0 = A1_R32(t1);
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v0; v1 = v1 >> 18; if (c) goto L_10b29a8; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v1 = v1 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v0 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); } a3xw3 = true; a3xw4 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    goto L_10b29ac;
L_10b29a8:;
    v0 = 0u;
L_10b29ac:;
    if (v0 == 0) { (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0)))); goto L_10b29c0; }
    (a3 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL));
    { A3_BACK(); goto L_10b2950; }
L_10b29c0:;
    v0 = 0x30000u;
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    { const bool c = v0 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a3, v0)))); if (c) goto L_10b29e0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_10b29e4;
L_10b29e0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b29e4:;
    { const bool c = (int64_t)a0 == 0; v0 = 0u; if (c) goto L_10b2a18; }
    { const uint32_t _dynInsn = A1_R32(0x10B29ECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10B29F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)(int32_t)v1) { v0 = 0u; goto L_10b2a18; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v0 = a2 >> 18;
    v0 = v0 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); } a3xw3 = true; a3xw4 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
L_10b2a18:;
    { v0 = v0 + 0x50u; if (xo) { xo[0] = a3; xo[1] = a1_sx64(t0); xo[2] = a1_sx64(t1); if (a3xw3) xo[3] = hi; if (a3xw4) xo[4] = lo; } return v0; }
    __builtin_unreachable();
}

// GetCharacterClass_x13 (0x10c0a70): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a0 xo[1]=a1; VU0 in h5, out h2)
inline uint64_t GetCharacterClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL)); if (c) goto L_10c0a98; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(v1) == 0) do { const a4::q128 _v = (a4::mk128(h5, a1_sx64(a1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_10c0a98:;
    { if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetLightClass_x13 (0x10f93f0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetLightClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo)
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

// GetScriptAnimationClass_x13 (0x110a838): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetScriptAnimationClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo)
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

// dynamic_cast_to_camera_obj_x13 (0x1162e08): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1; VU0 in -, out -, break)
inline uint64_t dynamic_cast_to_camera_obj_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t *xo)
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

// PushQuaternion_x13 (0x1bbb48): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1; VU0 in vf0, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t PushQuaternion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 3936))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    if ((int32_t)v0 >= 0) goto L_1bbb70;
    { A3_CHARGE(); v0 = a1_sx64(InitQuaternionDrive(ram, rt, vf0)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
L_1bbb70:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)64) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; A1_W32B(s0, 0, (uint32_t)v0); if (c) goto L_1bbb88; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 63))));
    A1_W32B(s0, 0, (uint32_t)v0);
L_1bbb88:;
    a0 = A1_R32(s0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 10944))));
    a0 = a0 << 4;
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, a0))));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, (uint32_t)v1))));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 4294967280))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    { if (xo) { xo[0] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetStabYRotQuaternion_x13 (0x1bc968): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2 xo[1]=a3; VU0 in vf0 vf5 vf6 vf7 vf13 vf14 vf15 vf16 vacc vstat, out vf4 vf5 vf6 vf7 vf11 vf12 vf13 vf14 vf15 vf16 vacc vq vstat)
inline void GetStabYRotQuaternion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo)
{
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    sp = sp - 0x40u;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    a2 = sp + 0x10u;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<3>(vf7)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf6));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    a3 = 0x1E0000u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4016))));
    a3 = a3 + 0xFC0u;
    { (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); a9x::diffQuat(ram, rt, (uint32_t)a0, (uint32_t)a1, a2, a3); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
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
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf13));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf15));
    { sp = sp + 0x40u; if (xo) { xo[0] = a1_sx64(a2); xo[1] = a1_sx64(a3); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return; }
    __builtin_unreachable();
}

// GetMotionNbOptionNodes_x13 (0x10b36c8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=hi xo[6]=lo; VU0 in -, out h2, relocated calls 1)
inline uint64_t GetMotionNbOptionNodes_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t s0 = 0;
    uint64_t h2 = io_h2;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    sp = sp - 0x10u;
    { const uint32_t _dynInsn = A1_R32(0x10B36D4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_10b36d8:;
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b36f8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b36fc;
L_10b36f8:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b36fc:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3700u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b3710; }
    { const uint32_t _dynInsn = A1_R32(0x10B3704u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b3714; }
L_10b3710:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3714:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B3718u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, _dynSimm)))); }
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_10b37a8; }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b3740; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b3744;
L_10b3740:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3744:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3748u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b3778; }
    { const uint32_t _dynInsn = A1_R32(0x10B374Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a3, 18)))); if (c) goto L_10b3778; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw5 = true; a3xw6 = true;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b377c;
L_10b3778:;
    s0 = 0u;
L_10b377c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 12))));
    if ((int64_t)v0 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 32)))); goto L_10b37a0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13104)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a3, (uint32_t)t0, (uint32_t)t1, t2, t3, sp, h2)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
L_10b37a0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 24))));
    goto L_10b3800;
L_10b37a8:;
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & v0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b37c0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b37c4;
L_10b37c0:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b37c4:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B37C8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b37d8; }
    { const uint32_t _dynInsn = A1_R32(0x10B37CCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b37dc; }
L_10b37d8:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b37dc:;
    { const uint32_t _dynInsn = A1_R32(0x10B37DCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B37E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10b3800; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, hi, lo}; v0 = a1_sx64(getBlendLastMotion_x13(ram, rt, a0, a7c)); t0 = a7c[1]; t1 = a7c[2]; hi = a7c[3]; lo = a7c[4]; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { A3_BACK(); goto L_10b36d8; }
L_10b3800:;
    { sp = sp + 0x10u; if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a1_sx64(a3); if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = hi; if (a3xw6) xo[6] = lo; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetMotionOptionNodePhysicalId_x13 (0x10b3810): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=hi xo[6]=lo; VU0 in -, out h2, relocated calls 1)
inline uint64_t GetMotionOptionNodePhysicalId_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    sp = sp - 0x20u;
    { const uint32_t _dynInsn = A1_R32(0x10B381Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
L_10b3828:;
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b3848; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b384c;
L_10b3848:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b384c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3850u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b3860; }
    { const uint32_t _dynInsn = A1_R32(0x10B3854u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b3864; }
L_10b3860:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3864:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B3868u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, _dynSimm)))); }
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_10b3940; }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b3890; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b3894;
L_10b3890:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3894:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3898u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b38d0; }
    { const uint32_t _dynInsn = A1_R32(0x10B389Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a3, 18)))); if (c) goto L_10b38d0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw5 = true; a3xw6 = true;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b38d4;
L_10b38c8:;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    goto L_10b3998;
L_10b38d0:;
    s0 = 0u;
L_10b38d4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 12))));
    if ((int64_t)v0 != 0) { v1 = A1_R32B(s0, 12); goto L_10b38f4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13104)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a3, (uint32_t)t0, (uint32_t)t1, t2, t3, sp, h2)); }
    v1 = A1_R32B(s0, 12);
L_10b38f4:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 32))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 24))));
    { const bool c = (int32_t)v1 <= 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 40)))); if (c) goto L_10b3934; }
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 0)));
L_10b3910:;
    { const bool c = (int64_t)v0 == (int64_t)s1; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1)))); if (c) { A3_BACK(); goto L_10b38c8; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int64_t)a1_sx64(v1)) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 0))); { A3_BACK(); goto L_10b3910; } }
L_10b3934:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    goto L_10b3998;
L_10b3940:;
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & v0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b3958; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b395c;
L_10b3958:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b395c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3960u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b3970; }
    { const uint32_t _dynInsn = A1_R32(0x10B3964u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b3974; }
L_10b3970:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3974:;
    { const uint32_t _dynInsn = A1_R32(0x10B3974u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B397Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10b3998; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, hi, lo}; v0 = a1_sx64(getBlendLastMotion_x13(ram, rt, a0, a7c)); t0 = a7c[1]; t1 = a7c[2]; hi = a7c[3]; lo = a7c[4]; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { A3_BACK(); goto L_10b3828; }
L_10b3998:;
    { sp = sp + 0x20u; if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a1_sx64(a3); if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = hi; if (a3xw6) xo[6] = lo; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetMotionVersion_x13 (0x10b39b0): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=hi xo[6]=lo; VU0 in -, out h2, relocated calls 1)
inline uint64_t GetMotionVersion_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t s0 = 0;
    uint64_t h2 = io_h2;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    sp = sp - 0x10u;
    { const uint32_t _dynInsn = A1_R32(0x10B39BCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_10b39c0:;
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b39e0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b39e4;
L_10b39e0:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b39e4:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B39E8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b39f8; }
    { const uint32_t _dynInsn = A1_R32(0x10B39ECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b39fc; }
L_10b39f8:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b39fc:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B3A00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, _dynSimm)))); }
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_10b3a90; }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b3a28; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b3a2c;
L_10b3a28:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3a2c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3A30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b3a60; }
    { const uint32_t _dynInsn = A1_R32(0x10B3A34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a3, 18)))); if (c) goto L_10b3a60; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw5 = true; a3xw6 = true;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b3a64;
L_10b3a60:;
    s0 = 0u;
L_10b3a64:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 12))));
    if ((int64_t)v0 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 32)))); goto L_10b3a88; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13104)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, a3, (uint32_t)t0, (uint32_t)t1, t2, t3, sp, h2)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
L_10b3a88:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 0))));
    goto L_10b3ae8;
L_10b3a90:;
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & v0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b3aa8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b3aac;
L_10b3aa8:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3aac:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B3AB0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b3ac0; }
    { const uint32_t _dynInsn = A1_R32(0x10B3AB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b3ac4; }
L_10b3ac0:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b3ac4:;
    { const uint32_t _dynInsn = A1_R32(0x10B3AC4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B3ACCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10b3ae8; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, hi, lo}; v0 = a1_sx64(getBlendLastMotion_x13(ram, rt, a0, a7c)); t0 = a7c[1]; t1 = a7c[2]; hi = a7c[3]; lo = a7c[4]; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { A3_BACK(); goto L_10b39c0; }
L_10b3ae8:;
    { sp = sp + 0x10u; if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a1_sx64(a3); if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = hi; if (a3xw6) xo[6] = lo; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// getAnimObjResource_x13 (0x10b70b8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=hi xo[1]=lo; VU0 in -, out h2, relocated calls 1)
inline uint32_t getAnimObjResource_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    bool a3xw1 = false;
    sp = sp - 0x10u;
    v0 = 0x30000u;
    v0 = v0 | 0xFFFFu;
    a2 = A1_R32(a0);
    v0 = a2 & v0;
    { const bool c = v0 == 0; a0 = a0 - v0; if (c) goto L_10b70e8; }
    a0 = A1_R32(a0);
    goto L_10b70ec;
L_10b70e8:;
    a0 = 0u;
L_10b70ec:;
    { const bool c = a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B70F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b7120; }
    { const uint32_t _dynInsn = A1_R32(0x10B70F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const bool c = a0 == v1; v0 = a2 >> 18; if (c) goto L_10b7120; }
    a1 = A1_R32B(a0, 8);
    v0 = v0 & 0xFFFu;
    a0 = A1_R32B(a0, 16);
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); } a3xw0 = true; a3xw1 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, v0))));
    goto L_10b7124;
L_10b7120:;
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b7124:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    if (v0 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 12)))); goto L_10b7144; }
    a0 = 0x1190000u;
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { a0 = a0 + 0x3330u; A3_CHARGE(); v0 = a7x::iosJumpRecoverPoint(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, h2); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
L_10b7144:;
    v0 = A1_R32B(v1, 32);
    { sp = sp + 0x10u; if (xo) { if (a3xw0) xo[0] = hi; if (a3xw1) xo[1] = lo; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetScriptLayoutClass_x13 (0x110d058): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t GetScriptLayoutClass_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo)
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
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetScriptAnimationClass_x13(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; xo[2] = a1_sx64(a1); } io_h2 = h2; return a7_t; }
L_110d090:;
    { if (xo) { xo[0] = v1; xo[2] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// manager_sub_010AE128_x13 (0x10ae128): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f1 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=f6 xo[6]=f12 xo[7]=v0 xo[8]=a1 xo[9]=a2 xo[10]=a3 xo[11]=t0 xo[12]=hi xo[13]=lo; VU0 in vf0 vf8 vf9 vacc vstat h2 h6 h8, out vf1 vf5 vf6 vf7 vf8 vf9 vf14 vf15 vacc vq vstat h2 h3 h5)
inline float manager_sub_010AE128_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    { uint32_t bits = (uint32_t)t0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    sp = sp - 0xC0u;
    (s0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { const bool c = (fcr31 & 0x800000u) == 0u; A1_W32B(sp, 184, a1_bits(f20)); if (c) goto L_10ae1b4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); a3xw7 = true;
    if ((int64_t)t0 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 4294967295)))); a3xw7 = true; goto L_10ae188; }
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); } a3xw7 = true;
    goto L_10ae1a8;
L_10ae188:;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::div(fcr31, f12, f0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw7 = true; a3xw12 = true; a3xw13 = true;
L_10ae1a8:;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f12 = a1f::sub(f12, f0); a3xw6 = true;
L_10ae1b4:;
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    s1 = a1_bits(f0);
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = a7_opq(s1); std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f20 = a1f::sub(f12, f0);
    fcr31 = (a1f::eq(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; s4 = s1 + 1u; if (c) goto L_10ae300; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    if ((int64_t)a0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8)))); goto L_10ae2cc; }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { const bool c = (int64_t)v1 == 0; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 12, bits); } if (c) goto L_10ae2b0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw7 = true;
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s1, 1)))); a3xw7 = true; if (c) goto L_10ae228; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s1)))); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2)))); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10ae2c4;
L_10ae228:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s1)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1)))); a3xw7 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14336 << 16))));
    at = at | 0x100u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); } a3xw2 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0)))); a3xw7 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 0, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 2)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 4, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 4)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    goto L_10ae2c4;
L_10ae2b0:;
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10ae2c4:;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 8, bits); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
L_10ae2cc:;
    { const bool c = (int64_t)a2 == 0; (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_10ae2e0; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1))); a3xw8 = true;
    { (a3 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); uint64_t a7c[11] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), a1, 0u, a3}; v0 = manager_sub_010AC6C8(ram, rt, a0, a1, a2, (uint32_t)a3, sp, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf5, vf6, vf7, vq, vstat, h2, h5, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); a1 = a7c[8]; a3 = a7c[10]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw10 = true; }
L_10ae2e0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    { const bool c = (int64_t)a2 == 0; (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_10ae630; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0))); a3xw8 = true;
    { (a3 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), 0u}; { const a1::RetVF a2_r = manager_sub_010ADE08(ram, rt, (uint32_t)a0, a1, (uint32_t)a2, (uint32_t)a3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f1 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); t0 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw7 = true; a3xw11 = true; }
    goto L_10ae634;
L_10ae300:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    if ((int64_t)a0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8)))); goto L_10ae3f4; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    { const bool c = (int64_t)v1 == 0; A1_W32B(sp, 44, a1_bits(f0)); if (c) goto L_10ae3d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw7 = true;
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s1, 1)))); a3xw7 = true; if (c) goto L_10ae350; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s1)))); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2)))); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    A1_W32B(sp, 32, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B(sp, 36, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10ae3ec;
L_10ae350:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s1)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1)))); a3xw7 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14336 << 16))));
    at = at | 0x100u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); } a3xw2 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0)))); a3xw7 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f1);
    A1_W32B(sp, 32, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 2)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    A1_W32B(sp, 36, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 4)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    goto L_10ae3ec;
L_10ae3d8:;
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    A1_W32B(sp, 32, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B(sp, 36, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10ae3ec:;
    A1_W32B(sp, 40, a1_bits(f0));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
L_10ae3f4:;
    { const bool c = (int64_t)a2 == 0; (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); if (c) goto L_10ae408; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1))); a3xw8 = true;
    { (a3 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); uint64_t a7c[11] = {(uint64_t)a1_bits(f0), 0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), 0u, 0u, 0u}; v0 = manager_sub_010AC6C8(ram, rt, a0, a1, a2, (uint32_t)a3, sp, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf5, vf6, vf7, vq, vstat, h2, h5, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; }
L_10ae408:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    { const bool c = (int64_t)a2 == 0; (a3 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL)); a3xw10 = true; if (c) goto L_10ae420; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0))); a3xw8 = true;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64)))); A3_CHARGE(); uint64_t a7c[5] = {0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), 0u}; { const a1::RetVF a2_r = manager_sub_010ADE08(ram, rt, (uint32_t)a0, a1, (uint32_t)a2, (uint32_t)a3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw7 = true; }
L_10ae420:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    if ((int64_t)a0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8)))); goto L_10ae514; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    { const bool c = (int64_t)v1 == 0; A1_W32B(sp, 60, a1_bits(f0)); if (c) goto L_10ae4f8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw7 = true;
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s4, 1)))); a3xw7 = true; if (c) goto L_10ae470; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s4)))); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2)))); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    A1_W32B(sp, 48, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B(sp, 52, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10ae50c;
L_10ae470:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s4)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1)))); a3xw7 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14336 << 16))));
    at = at | 0x100u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); } a3xw2 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 0)))); a3xw7 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f1);
    A1_W32B(sp, 48, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 2)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    A1_W32B(sp, 52, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v1, 4)))); a3xw7 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    goto L_10ae50c;
L_10ae4f8:;
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    A1_W32B(sp, 48, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B(sp, 52, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10ae50c:;
    A1_W32B(sp, 56, a1_bits(f0));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
L_10ae514:;
    { const bool c = (int64_t)a2 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16)))); if (c) goto L_10ae528; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1))); a3xw8 = true;
    { (a3 = (uint64_t)((uint64_t)a1_sx64(s4) + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); uint64_t a7c[11] = {(uint64_t)a1_bits(f0), 0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), 0u, 0u, 0u}; v0 = manager_sub_010AC6C8(ram, rt, a0, a1, a2, (uint32_t)a3, sp, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf5, vf6, vf7, vq, vstat, h2, h5, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; }
L_10ae528:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    { const bool c = (int64_t)a2 == 0; (a3 = (uint64_t)((uint64_t)a1_sx64(s4) + (uint64_t)0ULL)); a3xw10 = true; if (c) goto L_10ae540; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0))); a3xw8 = true;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80)))); A3_CHARGE(); uint64_t a7c[5] = {0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), 0u}; { const a1::RetVF a2_r = manager_sub_010ADE08(ram, rt, (uint32_t)a0, a1, (uint32_t)a2, (uint32_t)a3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw7 = true; }
L_10ae540:;
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16)))); a3xw8 = true;
    A4_W128(ADD32(sp, 96), a4::mk128(h3, v1));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf15)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); a3xw7 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    A1_W32B(sp, 112, a1_bits(f1));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 112)))); a3xw7 = true;
    if ((int32_t)v0 >= 0) goto L_10ae59c;
    at = 0xBF800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
L_10ae59c:;
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); } a3xw11 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf9));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32)))); a3xw7 = true;
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); } a3xw11 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64)))); a3xw7 = true;
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); } a3xw11 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf5));
L_10ae630:;
L_10ae634:;
    f20 = a1_flt(A1_R32B(sp, 184));
    { sp = sp + 0xC0u; if (xo) { xo[0] = a1_bits(f1); if (a3xw1) xo[1] = a1_bits(f2); if (a3xw2) xo[2] = a1_bits(f3); if (a3xw3) xo[3] = a1_bits(f4); if (a3xw4) xo[4] = a1_bits(f5); if (a3xw5) xo[5] = a1_bits(f6); if (a3xw6) xo[6] = a1_bits(f12); if (a3xw7) xo[7] = v0; if (a3xw8) xo[8] = a1; xo[9] = a2; if (a3xw10) xo[10] = a3; if (a3xw11) xo[11] = t0; if (a3xw12) xo[12] = hi; if (a3xw13) xo[13] = lo; } io_vf1 = vf1; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf14 = vf14; io_vf15 = vf15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return f0; }
    __builtin_unreachable();
}

// GetMotionOptionNodeIntFrame_x13 (0x10b4218): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=t2 xo[8]=hi xo[9]=lo; VU0 in vf0 vstat h2 h6, out vf1 vf5 vf6 vf7 vq vstat h2 h5, relocated calls 1)
inline uint64_t GetMotionOptionNodeIntFrame_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t io_h6, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    sp = sp - 0x40u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (t2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)(a1_sx64(t2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_10b4270; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b4274;
L_10b4270:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b4274:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B4278u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b4288; }
    { const uint32_t _dynInsn = A1_R32(0x10B427Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a0 != (int64_t)v0; (a1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_10b428c; }
L_10b4288:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b428c:;
    { const uint32_t _dynInsn = A1_R32(0x10B428Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B4294u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    if ((int64_t)(int32_t)v1 != (int64_t)v0) goto L_10b4454;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(t2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0)))); if (c) goto L_10b42c0; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b42c4;
L_10b42c0:;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b42c4:;
    { const bool c = (int64_t)a3 == 0; { const uint32_t _dynInsn = A1_R32(0x10B42C8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b42f8; }
    { const uint32_t _dynInsn = A1_R32(0x10B42CCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a3 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(t2, 18)))); if (c) goto L_10b42f8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw8 = true; a3xw9 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b42fc;
L_10b42f8:;
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b42fc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12)))); goto L_10b431c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13104)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, t3, sp, h2)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
L_10b431c:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 32))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 28))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    if ((int64_t)a0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8)))); goto L_10b4424; }
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 2)));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { const bool c = v1 == 0; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 12, bits); } if (c) goto L_10b4408; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 1)))); if (c) goto L_10b4380; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    goto L_10b441c;
L_10b4380:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14336 << 16))));
    at = at | 0x100u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); } a3xw3 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 0))));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f0 = a1f::add(f0, f1); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 0, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 2))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw2 = true;
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f0 = a1f::add(f0, f2); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 4, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 4))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)a0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw2 = true;
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f0 = a1f::add(f0, f2); a3xw0 = true;
    goto L_10b441c;
L_10b4408:;
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
L_10b441c:;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 8, bits); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
L_10b4424:;
    { const bool c = (int64_t)a2 == 0; (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); if (c) goto L_10b4438; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 1)));
    { (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[11] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), 0u, 0u, 0u, 0u}; v0 = manager_sub_010AC6C8(ram, rt, a0, a1, a2, (uint32_t)a3, sp, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf5, vf6, vf7, vq, vstat, h2, h5, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
L_10b4438:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    { const bool c = (int64_t)a2 == 0; (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_10b4450; }
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s0, 0)));
    { (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), 0u}; { const a1::RetVF a2_r = manager_sub_010ADE08(ram, rt, (uint32_t)a0, a1, (uint32_t)a2, (uint32_t)a3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f1 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
L_10b4450:;
L_10b4454:;
    { sp = sp + 0x40u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); xo[7] = a1_sx64(t2); if (a3xw8) xo[8] = hi; if (a3xw9) xo[9] = lo; } io_vf1 = vf1; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// manager_sub_010B8590_u13 (0x10b8590): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vq vstat h2 h3, relocated calls 9)
inline a1::RetVF manager_sub_010B8590_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
    (s5 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)fp))));
    A1_W32B((uint32_t)sp, 80, (uint32_t)a3);
    (s1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (s1 = (uint32_t)((int32_t)((int32_t)ADD32(s1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 52))));
    s3 = s1 + 0x10u;
    { A1_W32B((uint32_t)sp, 84, (uint32_t)v0); A3_CHARGE(); v0 = PushQuaternion(ram, rt, vf0, h2); }
    { A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 48))));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(MultiCurrentQuaternion(ram, rt, (uint32_t)a0, vf11, vf12, vf13, vf14, vf15, vf16, vacc)); }
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf15));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); } A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, (uint32_t)a0))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4)))); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s6)))); A3_CHARGE(); v0 = a1_sx64(GetCurrentQuaternion(ram, rt)); }
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf15));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
    (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { const bool c = (int64_t)s4 == (int64_t)s0; (t1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_10b8734; }
    { A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_010B8590(ram, rt, a0, a1, a2, a3, t0, t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
L_10b8734:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    { A3_CHARGE(); v0 = a1_sx64(PopQuaternion_u13(ram, rt, h3)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 84))));
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
    (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == (int64_t)s0; (t1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_10b8768; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8590(ram, rt, a0, a1, a2, a3, t0, t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; }
L_10b8768:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetOptionNodePhysicalNodeId_x13 (0x10c6710): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=t1 xo[3]=hi xo[4]=lo; VU0 in -, out h2, guest sp, frame 32 bytes, 0 slot accesses)
inline uint64_t GetOptionNodePhysicalNodeId_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t h2 = io_h2;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = (int32_t)v0 <= 0; (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_10c6754; }
    { A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionVersion_x13(ram, rt, a0, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFE0u), f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)5) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_10c6770; }
L_10c6754:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, t1, hi, lo}; v0 = GetMotionNbOptionNodes_x13(ram, rt, a0, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFE0u), f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; }
    (v0 = (uint64_t)(((int64_t)(int64_t)s1 < (int64_t)(int64_t)v0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (s2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); goto L_10c677c; }
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    goto L_10c6780;
L_10c6770:;
    { A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, t1, hi, lo}; v0 = GetMotionOptionNodePhysicalId_x13(ram, rt, a0, a1, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFE0u), f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; }
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_10c677c:;
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_10c6780:;
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = t1; xo[3] = hi; xo[4] = lo; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetLayoutClass_u13 (0x10f1a20): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, jump tables 1, frame 16 bytes, 0 slot accesses)
inline uint64_t GetLayoutClass_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo)
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
    { A3_CHARGE(); uint64_t a7c[3] = {0u, a0, 0u}; v0 = GetScriptLayoutClass_x13(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; a1 = a7c[2]; a3xw1 = true; a3xw2 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 112))));
    goto L_10f1aa4;
L_10f1a70:;
    { A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = dynamic_cast_to_camera_obj_x13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, a7c); v1 = a7c[0]; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw1 = true;
    { if (xo) { xo[0] = v1; xo[1] = a0; } const uint32_t a4_t = get_camera_obj_layout_class(ram, rt, (uint32_t)a0); io_h2 = h2; return a1_sx64(a4_t); }
L_10f1a88:;
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetLightClass_x13(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return a7_t; }
L_10f1a98:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    goto L_10f1aa4;
L_10f1aa0:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10f1aa4:;
    { if (xo) { xo[0] = v1; if (a3xw1) xo[1] = a0; if (a3xw2) xo[2] = a1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetMotionMatrix_u13 (0x10b1d60): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vq vstat h2 h3 h5, relocated calls 18)
inline a1::RetVF GetMotionMatrix_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
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
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967040))));
    { const uint32_t _dynInsn = A1_R32(0x10B1D64u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x10B1D78u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    (s5 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    { (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = a1_sx64(SetCurrentQuaternion_x13(ram, rt, (uint32_t)a0, h5, a7c)); a1 = a7c[0]; }
    { A3_CHARGE(); iosUnitCurrentMatrix(ram, rt, vf0, vf1, vf2, vf3, vf4); }
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    { const bool c = (int64_t)s4 == 0; (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10b1f68; }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 8))));
    A1_W32B((uint32_t)sp, 160, (uint32_t)s4);
    (s2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 52))));
    { A1_W32B((uint32_t)sp, 164, (uint32_t)v0); A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = PushQuaternion_x13(ram, rt, vf0, h2, a7c); a1 = a7c[0]; }
    { A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 48))));
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 16)))); A3_CHARGE(); v0 = a1_sx64(MultiCurrentQuaternion(ram, rt, (uint32_t)a0, vf11, vf12, vf13, vf14, vf15, vf16, vacc)); }
    (v1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf15));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); } A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 6))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)a0))));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 4)))); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    { A3_CHARGE(); v0 = a1_sx64(GetCurrentQuaternion(ram, rt)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 160))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v1))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf15));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const bool c = (int64_t)s3 == (int64_t)s0; (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_10b1f2c; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw1 = true;
    (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw2 = true;
    (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw3 = true;
    { (t1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw4 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8590_u13(ram, rt, a0, a1, a2, a3, t0, t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2, h3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
L_10b1f2c:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    { A3_CHARGE(); v0 = a1_sx64(PopQuaternion_u13(ram, rt, h3)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 164))));
    { const bool c = (int64_t)v0 == (int64_t)s0; (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) goto L_10b20b4; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw1 = true;
    (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw2 = true;
    (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw3 = true;
    { (t1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw4 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8590_u13(ram, rt, a0, a1, a2, a3, t0, t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2, h3, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
    goto L_10b20b8;
L_10b1f68:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 8))));
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 52))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    { A1_W32B((uint32_t)sp, 168, (uint32_t)v1); A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 48)))); A3_CHARGE(); v0 = a1_sx64(MultiCurrentQuaternion(ram, rt, (uint32_t)a0, vf11, vf12, vf13, vf14, vf15, vf16, vacc)); }
    (v1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf15));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
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
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 156, bits); } A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, (uint32_t)s1)))); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    { const bool c = (int64_t)s0 == (int64_t)s2; (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_10b208c; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw1 = true;
    (a3 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw2 = true;
    { (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw3 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8798(ram, rt, a0, a1, a2, a3, t0, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; }
L_10b208c:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 168))));
    { const bool c = (int64_t)v1 == (int64_t)s2; (a0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_10b20b4; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw1 = true;
    (a3 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw2 = true;
    { (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw3 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8798(ram, rt, a0, a1, a2, a3, t0, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; }
L_10b20b4:;
L_10b20b8:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 256)))); if (xo) { xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a3; if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetMotionOptionNodeFloatFrame_x13 (0x10b6650): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f12 xo[8]=f20 xo[9]=a1 xo[10]=a2 xo[11]=a3 xo[12]=t0 xo[13]=t1 xo[14]=hi xo[15]=lo; VU0 in vf0 vf8 vf9 vacc vstat h2 h6 h8, out vf1 vf5 vf6 vf7 vf8 vf9 vf14 vf15 vacc vq vstat h2 h3 h5, guest sp, frame 64 bytes, 2 slot accesses)
inline uint64_t GetMotionOptionNodeFloatFrame_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    uint8_t a2fr[64];
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    bool a3xw14 = false;
    bool a3xw15 = false;
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x10B6674u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s3 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 48, bits); }
    f20 = ((float)(f12));
    (a0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
L_10b668c:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b66b0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b66b4;
L_10b66b0:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b66b4:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B66B8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b66c8; }
    { const uint32_t _dynInsn = A1_R32(0x10B66BCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b66cc; }
L_10b66c8:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b66cc:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B66D0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, _dynSimm)))); }
    if ((int64_t)(int32_t)v1 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); goto L_10b6710; }
    { A3_CHARGE(); uint64_t a7c[2] = {hi, lo}; v0 = a1_sx64(getAnimObjResource_x13(ram, rt, (uint32_t)a0, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, t3, (uint32_t)(sp + 0xFFFFFFC0u), f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c)); hi = a7c[0]; lo = a7c[1]; a3xw14 = true; a3xw15 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 28))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)v0, 4)))); a3xw12 = true;
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, v1))));
    (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { f12 = ((float)(f20)); a3xw7 = true; A3_CHARGE(); uint64_t a7c[14] = {0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), v0, a1, 0u, a3, t0, hi, lo}; f0 = manager_sub_010AE128_x13(ram, rt, a0, a1, a2, a3, t0, (uint32_t)(sp + 0xFFFFFFC0u), f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf1, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vacc, vq, vstat, h2, h3, h5, h6, h8, a7c); f1 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); f6 = a1_flt((uint32_t)a7c[5]); f12 = a1_flt((uint32_t)a7c[6]); v0 = a7c[7]; a1 = a7c[8]; a2 = a7c[9]; a3 = a7c[10]; t0 = a7c[11]; hi = a7c[12]; lo = a7c[13]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw12 = true; a3xw14 = true; a3xw15 = true; }
    goto L_10b6768;
L_10b6710:;
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a3 & v0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b6728; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b672c;
L_10b6728:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b672c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B6730u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b6740; }
    { const uint32_t _dynInsn = A1_R32(0x10B6734u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b6744; }
L_10b6740:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b6744:;
    { const uint32_t _dynInsn = A1_R32(0x10B6744u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B674Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    if ((int64_t)(int32_t)v1 != (int64_t)v0) { goto L_10b6768; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, hi, lo}; v0 = a1_sx64(getBlendLastMotion_x13(ram, rt, a0, a7c)); t0 = a7c[1]; t1 = a7c[2]; hi = a7c[3]; lo = a7c[4]; a3xw12 = true; a3xw13 = true; a3xw14 = true; a3xw15 = true; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { A3_BACK(); goto L_10b668c; }
L_10b6768:;
    { uint32_t bits = a2::frLd<uint32_t>(a2fr, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); if (a3xw7) xo[7] = a1_bits(f12); xo[8] = a1_bits(f20); xo[9] = a1; xo[10] = a2; xo[11] = a3; if (a3xw12) xo[12] = t0; if (a3xw13) xo[13] = t1; if (a3xw14) xo[14] = hi; if (a3xw15) xo[15] = lo; } io_vf1 = vf1; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf14 = vf14; io_vf15 = vf15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// GetCharacterClassNodeQuaternionPos_u13 (0x10c1580): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vq vstat h2 h3 h5, guest sp, frame 48 bytes, 0 slot accesses)
inline uint64_t GetCharacterClassNodeQuaternionPos_u13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    { const bool c = (int64_t)t0 == 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6)))); if (c) goto L_10c15d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 340))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    A1_W32B((uint32_t)s2, 0, (uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 344))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2))));
    v1 = v1 + 0x30u;
    A1_W32B((uint32_t)s3, 0, v1);
    goto L_10c162c;
L_10c15d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s0, 1331))));
    if ((int64_t)v0 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 840)))); goto L_10c160c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 836))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 64)))); a3xw4 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 840)))); a3xw1 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 144)))); a3xw5 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 856))));
    { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 832)))); a3xw3 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, a2, a3, t0, t1}; { const a1::RetVF a2_r = GetMotionMatrix_u13(ram, rt, a0, a1, a2, a3, t0, t1, a1_sx64((uint32_t)(sp + 0xFFFFFFD0u)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2, h3, h5, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; a3xw0 = true; a3xw1 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W8B((uint32_t)s0, 1331, (uint8_t)(uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 840))));
L_10c160c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    A1_W32B((uint32_t)s2, 0, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 836))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    A1_W32B((uint32_t)s3, 0, (uint32_t)v0);
L_10c162c:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1; xo[2] = a2; if (a3xw3) xo[3] = a3; if (a3xw4) xo[4] = t0; if (a3xw5) xo[5] = t1; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// AddCharacterClassOptionNodeSubGeometry_x13 (0x10c3650): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f12 xo[8]=f20 xo[9]=a1 xo[10]=a2 xo[11]=a3 xo[12]=t0 xo[13]=t1 xo[14]=t2 xo[15]=hi xo[16]=lo; VU0 in vf0 vf8 vf9 vf13 vf14 vf15 vf16 vacc vstat h2 h6 h8, out vf1 vf5 vf6 vf7 vf8 vf9 vf11 vf12 vf13 vf14 vf15 vf16 vacc vq vstat h2 h3 h5)
inline void AddCharacterClassOptionNodeSubGeometry_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
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
    bool a3xw15 = false;
    bool a3xw16 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 80, (uint32_t)a3);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    { const bool c = (int64_t)s0 == 0; (s7 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_10c38b8; }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 856))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 80))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    { const bool c = (int32_t)v0 <= 0; (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10c36bc; }
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s4) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionVersion_x13(ram, rt, a0, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)5) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); goto L_10c36d8; }
L_10c36bc:;
    { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionNbOptionNodes_x13(ram, rt, a0, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)(((int64_t)(int64_t)s3 < (int64_t)(int64_t)v0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (fp = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); goto L_10c36e4; }
    goto L_10c36e4;
L_10c36d8:;
    { (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw9 = true; A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionOptionNodePhysicalId_x13(ram, rt, a0, a1, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (fp = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_10c36e4:;
    { const bool c = (int32_t)fp < 0; (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 40)))); if (c) goto L_10c38b8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0)))); a3xw10 = true;
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(v1, (uint32_t)v0)))); if (c) goto L_10c3710; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10c3714;
L_10c3710:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c3714:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10C3718u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10c3748; }
    { const uint32_t _dynInsn = A1_R32(0x10C371Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10c3748; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw9 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw15 = true; a3xw16 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10c374c;
L_10c3748:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c374c:;
    if ((int64_t)a0 == 0) { (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); goto L_10c37d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const bool c = (int32_t)v0 <= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 80)))); if (c) goto L_10c3778; }
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionVersion_x13(ram, rt, a0, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)5) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw9 = true; if (c) goto L_10c3798; }
L_10c3778:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionNbOptionNodes_x13(ram, rt, a0, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)(((int64_t)(int64_t)s3 < (int64_t)(int64_t)v0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (s2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); goto L_10c37a4; }
    goto L_10c37a4;
L_10c3798:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionOptionNodePhysicalId_x13(ram, rt, a0, a1, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; hi = a7c[5]; lo = a7c[6]; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_10c37a4:;
    { const bool c = (int32_t)s2 < 0; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (c) goto L_10c38b8; }
    (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw11 = true;
    (t0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw12 = true;
    (a0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(s6) + (uint64_t)0ULL)); a3xw9 = true;
    (a2 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL)); a3xw10 = true;
    { (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw13 = true; A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), 0u, hi, lo}; v0 = GetMotionOptionNodeIntFrame_x13(ram, rt, a0, a1, a2, a3, t0, t1, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf5, vf6, vf7, vq, vstat, h2, h5, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); t2 = a7c[7]; hi = a7c[8]; lo = a7c[9]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
    goto L_10c3800;
L_10c37d8:;
    (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(s6) + (uint64_t)0ULL)); a3xw9 = true;
    (a2 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL)); a3xw10 = true;
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw11 = true;
    (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw12 = true;
    { (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw13 = true; A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), 0u, hi, lo}; v0 = GetMotionOptionNodeIntFrame_x13(ram, rt, a0, a1, a2, a3, t0, t1, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf1, vf5, vf6, vf7, vq, vstat, h2, h5, h6, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); t2 = a7c[7]; hi = a7c[8]; lo = a7c[9]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
L_10c3800:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw10 = true;
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw11 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } a3xw7 = true;
    (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw12 = true;
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); a3xw9 = true; A3_CHARGE(); uint64_t a7c[16] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), 0u, 0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionOptionNodeFloatFrame_x13(ram, rt, a0, a1, a2, a3, t0, t1, (uint32_t)t2, t3, (uint32_t)sp, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, f20, hi, lo, vf0, vf1, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vacc, vq, vstat, h2, h3, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; hi = a7c[14]; lo = a7c[15]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(s6, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf11));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
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
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf13));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s7, 0), a4v::castps_si128(vf5));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf13));
L_10c38b8:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); if (a3xw7) xo[7] = a1_bits(f12); if (a3xw8) xo[8] = a1_bits(f20); if (a3xw9) xo[9] = a1; if (a3xw10) xo[10] = a2; if (a3xw11) xo[11] = a3; if (a3xw12) xo[12] = t0; if (a3xw13) xo[13] = t1; if (a3xw14) xo[14] = t2; if (a3xw15) xo[15] = hi; if (a3xw16) xo[16] = lo; } io_vf1 = vf1; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return; }
    __builtin_unreachable();
}

// AddCharacterClassOptionNodeSubGeometries_x13 (0x10c6798): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f12 xo[8]=f20 xo[9]=a1 xo[10]=a2 xo[11]=a3 xo[12]=t0 xo[13]=t1 xo[14]=t2 xo[15]=hi xo[16]=lo; VU0 in vf0 vf8 vf9 vf13 vf14 vf15 vf16 vacc vstat h2 h6 h8, out vf1 vf5 vf6 vf7 vf8 vf9 vf11 vf12 vf13 vf14 vf15 vf16 vacc vq vstat h2 h3 h5, guest sp, frame 64 bytes, 0 slot accesses)
inline void AddCharacterClassOptionNodeSubGeometries_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t s6 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
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
    bool a3xw15 = false;
    bool a3xw16 = false;
    (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 1020))));
    { const bool c = (int32_t)v1 <= 0; (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 780)))); if (c) goto L_10c6814; }
    s6 = 0xFFFFFFFEu;
L_10c67e0:;
    v0 = A1_R32B(s0, 68);
    { const bool c = v0 == s6; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, 4)))); a3xw11 = true; if (c) goto L_10c6804; }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw9 = true;
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw10 = true;
    { (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw12 = true; A3_CHARGE(); uint64_t a7c[17] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f20), a1, a2, a3, t0, t1, t2, hi, lo}; AddCharacterClassOptionNodeSubGeometry_x13(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64((uint32_t)(sp + 0xFFFFFFC0u)), f0, f1, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, f20, hi, lo, vf0, vf1, vf5, vf6, vf7, vf8, vf9, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat, h2, h3, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 1020))));
L_10c6804:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(s2) < (int64_t)(int64_t)a1_sx64(v1)) ? 1 : 0));
    { const bool c = v0 != 0; s0 = s0 + 0x80u; if (c) { A3_BACK(); goto L_10c67e0; } }
L_10c6814:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); if (a3xw7) xo[7] = a1_bits(f12); if (a3xw8) xo[8] = a1_bits(f20); if (a3xw9) xo[9] = a1; if (a3xw10) xo[10] = a2; if (a3xw11) xo[11] = a3; if (a3xw12) xo[12] = t0; if (a3xw13) xo[13] = t1; if (a3xw14) xo[14] = t2; if (a3xw15) xo[15] = hi; if (a3xw16) xo[16] = lo; } io_vf1 = vf1; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return; }
    __builtin_unreachable();
}

// GetCharacterClassOptionNodeGeo_x13 (0x10bd3f8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f12 xo[8]=f20 xo[9]=a1 xo[10]=a2 xo[11]=a3 xo[12]=t0 xo[13]=t1 xo[14]=t2 xo[15]=hi xo[16]=lo; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5)
inline uint64_t GetCharacterClassOptionNodeGeo_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    bool a3xw14 = false;
    bool a3xw15 = false;
    bool a3xw16 = false;
    sp = sp - 0xA0u;
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 856)))); A3_CHARGE(); v0 = a1_sx64(GetMotionOptionNodeParentId(ram, rt, (uint32_t)a0, (uint32_t)a1)); }
    (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    if ((int32_t)s3 >= 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 752)))); goto L_10bd5f8; }
    { const bool c = (int64_t)s6 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s7, 4)))); if (c) goto L_10bd508; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 844))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 128))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf18 = a4v::blendv_ps(vf18, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
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
    { a4::vec4 mul_res = a4v::mul(vf18, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 740))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s7, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s5, 0), a4::mk128(h3, v1));
    goto L_10bd7e8;
L_10bd508:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 752))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s7, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 748)))); a3xw11 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 128)))); a3xw12 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64)))); a3xw10 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    A4_W128(ADD32(sp, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf18 = a4v::blendv_ps(vf18, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t0, 0)));
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
    { a4::vec4 mul_res = a4v::mul(vf18, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 128))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
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
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf13));
    goto L_10bd7e8;
L_10bd5f8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s7, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 748))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1))));
    A4_W128(ADD32(sp, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const bool c = (int32_t)s3 <= 0; A4_W128(ADD32(sp, 0), a4::mk128(h2, v0)); if (c) goto L_10bd718; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(s3 & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; s0 = sp + 0x20u; if (c) goto L_10bd718; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16)))); a3xw12 = true;
    s1 = sp + 0x30u;
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)65535)); a3xw12 = true;
    (t0 = (uint64_t)(s3 & RC_U64X(t0))); a3xw12 = true;
    (t1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); a3xw13 = true;
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(s1) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw10 = true;
    { (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw11 = true; A3_CHARGE(); uint64_t a7c[17] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f20), 0u, a2, a3, t0, t1, t2, hi, lo}; v0 = GetCharacterClassOptionNodeGeo_x13(ram, rt, v0, a0, a1, a2, a3, t0, t1, t2, t3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32(s1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf18 = a4v::blendv_ps(vf18, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
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
    { a4::vec4 mul_res = a4v::mul(vf18, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
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
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf13));
    goto L_10bd7e8;
L_10bd718:;
    { const bool c = (int64_t)s6 == 0; s0 = sp + 0x40u; if (c) goto L_10bd728; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 340))));
    goto L_10bd75c;
L_10bd728:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s2, 1331))));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 840)))); goto L_10bd75c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 836))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64)))); a3xw12 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 840))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 144)))); a3xw13 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 856)))); a3xw10 = true;
    { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 832)))); a3xw11 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, a2, a3, 0u, t1}; { const a1::RetVF a2_r = GetMotionMatrix_u13(ram, rt, a0, a1, a2, a3, t0, t1, a1_sx64(sp), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2, h3, h5, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t1 = a7c[4]; a3xw0 = true; a3xw10 = true; a3xw11 = true; a3xw13 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W8B((uint32_t)s2, 1331, (uint8_t)(uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 840))));
L_10bd75c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 6)))); a3xw12 = true;
    { const bool c = (int64_t)s6 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10bd778; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 344))));
    goto L_10bd77c;
L_10bd778:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 836))));
L_10bd77c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    A1_W32B(s0, 0, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
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
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf13));
L_10bd7e8:;
    { const bool c = (int64_t)fp == 0; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 448)))); if (c) goto L_10bd8fc; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 856))));
    (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s7, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw12 = true;
    { const bool c = s0 == 0; (s1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1)))); if (c) goto L_10bd828; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, 80))));
    if ((int64_t)a1 == 0) goto L_10bd828;
    { (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = GetOptionNodePhysicalNodeId_x13(ram, rt, a0, a1, a2, t0, t1, (uint32_t)t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a1 = a7c[0]; a2 = a7c[1]; t1 = a7c[2]; hi = a7c[3]; lo = a7c[4]; a3xw10 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (t0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw12 = true;
L_10bd828:;
    { const bool c = (int32_t)t0 < 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, 80)))); a3xw11 = true; if (c) goto L_10bd850; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } a3xw7 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    { (a2 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); a3xw10 = true; A3_CHARGE(); uint64_t a7c[16] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), 0u, 0u, 0u, 0u, t0, t1, hi, lo}; v0 = GetMotionOptionNodeFloatFrame_x13(ram, rt, a0, a1, a2, a3, t0, t1, (uint32_t)t2, t3, sp, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, f20, hi, lo, vf0, vf1, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vacc, vq, vstat, h2, h3, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; hi = a7c[14]; lo = a7c[15]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw15 = true; a3xw16 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s2, 1326))));
    goto L_10bd874;
L_10bd850:;
    do { const a4::q128 _v = (A4_R128(ADD32(s1, 32))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x10BD854u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10BD858u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    A4_W128(ADD32(sp, 32), a4::mk128(h4, a0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32(s1, 16))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)fp, 0), a4::mk128(h3, v1));
    A4_W128(ADD32(sp, 48), a4::mk128(h4, a0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s2, 1326))));
L_10bd874:;
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32)))); if (c) goto L_10bd8d8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw10 = true;
    { (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw11 = true; A3_CHARGE(); uint64_t a7c[17] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f20), a1, a2, a3, t0, t1, t2, hi, lo}; AddCharacterClassOptionNodeSubGeometries_x13(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f0, f1, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, f20, hi, lo, vf0, vf1, vf5, vf6, vf7, vf8, vf9, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat, h2, h3, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s2, 3472)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 144)))); if (c) goto L_10bd8dc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 3360))));
    { const bool c = (int64_t)v0 != 0; (t0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw12 = true; if (c) goto L_10bd8c4; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 1844); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 144)))); if (c) goto L_10bd8dc; }
    (t0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); a3xw12 = true;
L_10bd8c4:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw10 = true;
    { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 3360)))); a3xw11 = true; A3_CHARGE(); uint64_t a7c[17] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f20), a1, a2, a3, t0, t1, t2, hi, lo}; AddCharacterClassOptionNodeSubGeometry_x13(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64(sp), f0, f1, f2, f3, f4, f5, f6, f12, f13, f14, f15, f16, f17, f18, f19, f20, hi, lo, vf0, vf1, vf5, vf6, vf7, vf8, vf9, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat, h2, h3, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
L_10bd8d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 144))));
L_10bd8dc:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)fp, 0), a4v::castps_si128(vf5));
L_10bd8fc:;
    { sp = sp + 0xA0u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); if (a3xw7) xo[7] = a1_bits(f12); if (a3xw8) xo[8] = a1_bits(f20); xo[9] = a1; if (a3xw10) xo[10] = a2; if (a3xw11) xo[11] = a3; if (a3xw12) xo[12] = t0; if (a3xw13) xo[13] = t1; if (a3xw14) xo[14] = t2; if (a3xw15) xo[15] = hi; if (a3xw16) xo[16] = lo; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// manager_sub_010AC4A0_x13 (0x10ac4a0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f12 xo[8]=f20 xo[9]=a1 xo[10]=a2 xo[11]=a3 xo[12]=t0 xo[13]=t1 xo[14]=t2 xo[15]=hi xo[16]=lo; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5, relocated calls 1)
inline uint64_t manager_sub_010AC4A0_x13(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    bool a3xw14 = false;
    bool a3xw15 = false;
    bool a3xw16 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetCharacterClass_x13(ram, rt, a0, h2, h5, a7c); a1 = a7c[1]; }
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    if ((int64_t)a2 == 0) goto L_10ac660;
    { const bool c = (int32_t)s0 <= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294)))); if (c) goto L_10ac53c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(s0 & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); a3xw11 = true; if (c) goto L_10ac538; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16)))); a3xw12 = true;
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)65535)); a3xw12 = true;
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (t0 = (uint64_t)(s0 & RC_U64X(t0))); a3xw12 = true;
    (t1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw13 = true;
    { (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[17] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f20), 0u, a2, a3, t0, t1, t2, hi, lo}; v0 = GetCharacterClassOptionNodeGeo_x13(ram, rt, v0, a0, a1, a2, a3, t0, t1, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; a3xw14 = true; a3xw15 = true; a3xw16 = true; }
    goto L_10ac684;
L_10ac538:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
L_10ac53c:;
    { const bool c = (int64_t)s0 == (int64_t)v0; (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_10ac578; }
    (v0 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int32_t)4294967295) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10ac568; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967293))));
    { const bool c = (int64_t)s0 == (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (c) goto L_10ac5d0; }
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw11 = true;
    goto L_10ac638;
L_10ac568:;
    { const bool c = (int64_t)s0 == (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (c) goto L_10ac598; }
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw11 = true;
    goto L_10ac638;
L_10ac578:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 308))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 316))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0));
    goto L_10ac684;
L_10ac598:;
    { const bool c = (int64_t)s3 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 1120)))); if (c) goto L_10ac5a8; }
    { const uint32_t _dynInsn = A1_R32(0x10AC5A0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10AC5A4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
L_10ac5a8:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x10AC5ACu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10AC5B0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)a2 == 0; A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h3, v1)); if (c) goto L_10ac5c0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 316))));
L_10ac5c0:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0));
    goto L_10ac684;
L_10ac5d0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 308))));
    { const uint32_t _dynInsn = A1_R32(0x10AC5D4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10AC5D8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294934528))));
    A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 1136))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0)); A3_CHARGE(); f0 = a9x::axisRotate(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); a3xw0 = true; }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf13));
    goto L_10ac684;
L_10ac638:;
    (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); a3xw12 = true;
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 20)))); A3_CHARGE(); uint64_t a7c[6] = {(uint64_t)a1_bits(f0), a1, 0u, a3, t0, t1}; v0 = GetCharacterClassNodeQuaternionPos_u13(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, h2, h3, h5, a7c); f0 = a1_flt((uint32_t)a7c[0]); a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; a3xw0 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 16))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h3, v1));
    goto L_10ac684;
L_10ac660:;
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, a1}; v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a1 = a7c[2]; }
    (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 316))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 308))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h2, v0));
L_10ac684:;
    if ((int64_t)s6 == 0) goto L_10ac6a0;
    if ((int64_t)s3 == 0) goto L_10ac6a4;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 144))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s6, 0), a4::mk128(h2, v0));
L_10ac6a0:;
L_10ac6a4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); if (a3xw4) xo[4] = a1_bits(f4); if (a3xw5) xo[5] = a1_bits(f5); if (a3xw6) xo[6] = a1_bits(f6); if (a3xw7) xo[7] = a1_bits(f12); if (a3xw8) xo[8] = a1_bits(f20); xo[9] = a1; xo[10] = a2; if (a3xw11) xo[11] = a3; if (a3xw12) xo[12] = t0; if (a3xw13) xo[13] = t1; if (a3xw14) xo[14] = t2; if (a3xw15) xo[15] = hi; if (a3xw16) xo[16] = lo; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// iosMvecAllOne (0x1b9d98): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h2)
inline uint64_t iosMvecAllOne(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t hi, uint64_t lo, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (a4m::pceqw(a4v::setzero_si128(), a4v::setzero_si128())); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::srli_epi32<31>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosSWToMmColMvec (0x1ba240): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2)
inline uint64_t iosSWToMmColMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    A1_W64B((uint32_t)sp, 8, v0);
    A1_W64B((uint32_t)sp, 0, v1);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::slli_epi32<23>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosMmColToSWMvec (0x1ba268): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2)
inline uint64_t iosMmColToSWMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    A1_W64B((uint32_t)sp, 8, v0);
    A1_W64B((uint32_t)sp, 0, v1);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::srli_epi32<23>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosITOF15ToMmColMvec (0x1ba290): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2)
inline uint64_t iosITOF15ToMmColMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    A1_W64B((uint32_t)sp, 8, v0);
    A1_W64B((uint32_t)sp, 0, v1);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::slli_epi32<8>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosMmColToITOF15Mvec (0x1ba2b8): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2)
inline uint64_t iosMmColToITOF15Mvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    A1_W64B((uint32_t)sp, 8, v0);
    A1_W64B((uint32_t)sp, 0, v1);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::srli_epi32<8>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01386308 (0x1386308): typed by tools/typed_lift2.py (round A6; loop guest frame leaf, extra outputs xo[0]=hi xo[1]=lo; VU0 in -, out -)
inline uint64_t gamecore_sub_01386308(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t *xo)
{
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (t0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    A1_W32B((uint32_t)sp, 0, (uint32_t)v0);
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    A1_W32B((uint32_t)t0, 0, 0u);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    A1_W32B((uint32_t)t0, 4, 0u);
    (t1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a1; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4)))); if (c) goto L_1386364; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386348:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)a0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4)))); { A3_BACK(); goto L_1386348; } }
L_1386364:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 16))));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, -31152))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)32767));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    A1_W32B((uint32_t)t1, 0, (uint32_t)v0);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_13863fc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_13863e0:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4)))); { A3_BACK(); goto L_13863e0; } }
L_13863fc:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v0);
    A1_W32B((uint32_t)t0, 8, (uint32_t)a3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (t1 = (uint64_t)(A1_R64B((uint32_t)a2, 24)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)(t1 & (uint64_t)(uint16_t)48));
    { const bool c = (int64_t)v1 == 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0)))); if (c) goto L_13864e0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_1386494; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386478:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4)))); { A3_BACK(); goto L_1386478; } }
L_1386494:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
L_13864e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 28))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96)))); if (c) goto L_1386570; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_1386524; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386508:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4)))); { A3_BACK(); goto L_1386508; } }
L_1386524:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
L_1386570:;
    (v0 = (uint64_t)(t1 << 30));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96)))); if (c) goto L_138660c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_13865bc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_13865a0:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4)))); { A3_BACK(); goto L_13865a0; } }
L_13865bc:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v0);
    A1_W32B((uint32_t)t0, 0, (uint32_t)a3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
L_138660c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96)))); if (c) goto L_13866ac; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_138665c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386640:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4)))); { A3_BACK(); goto L_1386640; } }
L_138665c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v0);
    A1_W32B((uint32_t)t0, 4, (uint32_t)a3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
L_13866ac:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 28))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); goto L_13866d4; }
    (v0 = (uint64_t)(A1_R64B((uint32_t)a2, 24)));
    (v0 = (uint64_t)(v0 << 27));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (c) goto L_1386760; }
L_13866d4:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_1386714; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4))));
L_13866f8:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4)))); { A3_BACK(); goto L_13866f8; } }
L_1386714:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    A1_W32B((uint32_t)t2, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
L_1386760:;
    (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { xo[0] = hi; xo[1] = lo; } return v0; }
    __builtin_unreachable();
}

// iosFvecToMmColMvec (0x1ba2e0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf6, out vf5 vf6 h2)
inline uint64_t iosFvecToMmColMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); iosFTOI15Vector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf5, vf6); }
    (v0 = (uint64_t)(A1_R64B((uint32_t)s0, 0)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 8)));
    A1_W64B((uint32_t)sp, 0, v0);
    A1_W64B((uint32_t)sp, 8, v1);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::slli_epi32<8>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_vf5 = vf5; io_vf6 = vf6; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosMmColToFvecMvec (0x1ba320): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf6, out vf5 vf6 h2)
inline uint64_t iosMmColToFvecMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t hi, uint64_t lo, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    A1_W64B((uint32_t)sp, 32, v0);
    A1_W64B((uint32_t)sp, 40, v1);
    A1_W64B((uint32_t)sp, 0, v0);
    A1_W64B((uint32_t)sp, 8, v1);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::srli_epi32<8>(a4::mk128(h2, v0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { A4_W128(ADD32((uint32_t)sp, 16), a4::mk128(h2, v0)); A3_CHARGE(); iosITOF15Vector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf5, vf6); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_vf5 = vf5; io_vf6 = vf6; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetAnimObjResource (0x10b2de8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=hi xo[4]=lo; VU0 in -, out h2, relocated calls 1)
inline uint64_t GetAnimObjResource(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    sp = sp - 0x10u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b2e18; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b2e1c;
L_10b2e18:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e1c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B2E20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b2e30; }
    { const uint32_t _dynInsn = A1_R32(0x10B2E24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b2e34; }
L_10b2e30:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e34:;
    { const uint32_t _dynInsn = A1_R32(0x10B2E34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B2E3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_10b2e80; }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a3 & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b2e60; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b2e64;
L_10b2e60:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e64:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B2E68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b2f30; }
    { const uint32_t _dynInsn = A1_R32(0x10B2E6Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 18)))); if (c) goto L_10b2f30; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    goto L_10b2f14;
L_10b2e80:;
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a3 & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b2e98; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b2e9c;
L_10b2e98:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e9c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10B2EA0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b2eb0; }
    { const uint32_t _dynInsn = A1_R32(0x10B2EA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10b2eb4; }
L_10b2eb0:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2eb4:;
    { const uint32_t _dynInsn = A1_R32(0x10B2EB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10B2EBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10b2f58; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, hi, lo}; v0 = a1_sx64(getBlendLastMotion_x13(ram, rt, a0, a7c)); a3 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; hi = a7c[3]; lo = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10b2ef8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b2efc;
L_10b2ef8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2efc:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B2F00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b2f30; }
    { const uint32_t _dynInsn = A1_R32(0x10B2F04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10b2f30; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
L_10b2f14:;
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw3 = true; a3xw4 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b2f34;
L_10b2f30:;
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2f34:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12)))); goto L_10b2f54; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13104)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, t3, sp, h2)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
L_10b2f54:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 32))));
L_10b2f58:;
    { sp = sp + 0x10u; if (xo) { xo[0] = a3; if (a3xw1) xo[1] = t0; if (a3xw2) xo[2] = t1; if (a3xw3) xo[3] = hi; if (a3xw4) xo[4] = lo; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01386770 (0x1386770): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f0 xo[1]=f21 xo[2]=f22; VU0 in vf0 vf4 vf6 vf25 vf26 vacc vstat h3 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf24 vf25 vf26 vf27 vacc vq vstat h2 h8 h9)
inline uint64_t gamecore_sub_01386770(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f20, float f21, float f22, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
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
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t t8 = 0;
    uint64_t t9 = 0;
    uint64_t fp = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967040))));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 240, bits); }
    f22 = ((float)(f13));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 232, bits); }
    f21 = ((float)(f12));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 224, bits); }
    A1_W32B((uint32_t)sp, 116, (uint32_t)a1);
    A1_W32B((uint32_t)sp, 12, (uint32_t)a2);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 324))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 28))));
    A1_W32B((uint32_t)sp, 124, (uint32_t)v0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 80))));
    { const bool c = (int64_t)a0 == 0; A1_W32B((uint32_t)sp, 120, (uint32_t)a2); if (c) goto L_1386b54; }
L_13867e8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 120))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    A1_W32B((uint32_t)sp, 128, (uint32_t)v1);
    (v1 = (uint64_t)(A1_R64B((uint32_t)v1, 24)));
    (v0 = (uint64_t)(v1 << 14));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 != 0) { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32)))); goto L_1386b48; }
    (v0 = (uint64_t)(v1 << 13));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16)))); if (c) goto L_1386848; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 25880); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    goto L_1386850;
L_1386848:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 25880); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
L_1386850:;
    f20 = a1f::mul(f0, f22);
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, (uint32_t)a0))));
    A1_W32B((uint32_t)sp, 132, (uint32_t)a0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 128))));
L_1386868:;
    (a1 = (uint64_t)((uint64_t)t5 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = gamecore_sub_01386308(ram, rt, v0, a0, a1, a2, sp, a7c); hi = a7c[0]; lo = a7c[1]; }
    (t5 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 112))));
    { const bool c = (int64_t)v0 == 0; (t6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1386afc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 25888))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25876))));
    (s2 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)RC_U64X(fp) + (uint64_t)0ULL));
    (t9 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL));
    (t8 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (t7 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13868c0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t6, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 64, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 68, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    A1_W32B((uint32_t)sp, 76, 0u);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 72, bits); }
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 64)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)sp, 72)));
    A1_W64B((uint32_t)sp, 48, v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    A1_W64B((uint32_t)sp, 56, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 96, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 100, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 108, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 104, bits); }
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 96)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)sp, 104)));
    A1_W64B((uint32_t)sp, 80, v0);
    A1_W64B((uint32_t)sp, 88, v1);
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf0));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t8, 0)));
    { const a4::vec4 src = a4v::mul_ps(vf24, a4v::set1_ps(1.0f)); const a4::q128 positiveOverflow = a4v::andnot_si128(a4v::srai_epi32<31>(a4v::castps_si128(src)), a4v::castps_si128(a4v::cmpnlt_ps(src, a4v::set1_ps(2147483648.0f)))); const a4::vec4 res = a4v::castsi128_ps(a4v::xor_si128(a4v::cvttps_epi32(src), positiveOverflow)); vf25 = a4v::dest<1, 1, 1, 1>(vf25, res); }
    { a4::q128 src = a4v::castps_si128(vf25); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(1.0f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf24, vf26); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf24 = a4v::blend(vf24, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)t9, 0), a4v::castps_si128(vf24));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf24 = a4v::blend(vf24, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf25 = a4v::blend(vf25, res, a4v::castsi128_ps(mask)); }
    { const bool c = (int32_t)t4 <= 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13869e4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 116))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    (t2 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 352))));
    (t1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
L_1386988:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t3))));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf1 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf3 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf26 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf27)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf27, a4v::bcast<0>(vf26)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<0, 1>(vf24, vf27); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf24 = a4v::blend(vf24, res, a4v::castsi128_ps(mask)); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)t4) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2)))); if (c) { A3_BACK(); goto L_1386988; } }
L_13869e4:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf24, vf5); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::bcast<0>(vf5)); vq = a4v::sqrt(vstat, ft); }
    { a4::vec4 res = a4v::addK<1, 0>(vf0, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf5)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 112)))); if (c) goto L_1386aec; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25904))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t6, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s6))));
    do { const a4::q128 _v = (a4m::pextlb(a4v::setzero_si128(), a4::mk128(h3, v1))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlh(a4v::setzero_si128(), a4::mk128(h8, t0))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    vf5 = a4v::castsi128_ps(a4::mk128(h9, t1));
    { a4::q128 src = a4v::castps_si128(vf5); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(1.0f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf4));
    { uint32_t bits = A1_R32B((uint32_t)s4, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    f0 = a1f::div(fcr31, f2, f0); a3xw0 = true;
    f0 = a1f::mul(f20, f0); a3xw0 = true;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h9, t1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf6));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 124))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
    A1_W32B((uint32_t)v0, 36, (uint32_t)a0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    { uint32_t bits = A1_R32B((uint32_t)s4, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s6))));
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 32, bits); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25884))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; A1_W32B((uint32_t)a2, 0, (uint32_t)v1); if (c) goto L_1386ae8; }
    A1_W32B((uint32_t)a2, 0, 0u);
L_1386ae8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 112))));
L_1386aec:;
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 1))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(t6) < (uint64_t)v0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, 12)))); if (c) { A3_BACK(); goto L_13868c0; } }
L_1386afc:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t5, 3)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    { const bool c = (int64_t)v0 == (int64_t)v1; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 128)))); if (c) { A3_BACK(); goto L_1386868; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 132))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4))));
L_1386b18:;
    (v0 = (uint64_t)(((uint64_t)t5 < (uint64_t)a0) ? 1 : 0));
    if ((int64_t)v0 == 0) { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32)))); goto L_1386b48; }
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t5, 3)));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    if ((int64_t)v0 != (int64_t)v1) { (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4)))); { A3_BACK(); goto L_1386b18; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 128))));
    { A3_BACK(); goto L_1386868; }
L_1386b48:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_13867e8; } }
L_1386b54:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)sp, 240); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 232); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 224); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 256)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f21); xo[2] = a1_bits(f22); } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// GetMorphAnimation (0x10b7eb8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f20 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out h2, 64-bit inputs a0, guest sp, frame 16 bytes, 2 slot accesses)
inline a1::RetVF GetMorphAnimation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_a0, uint64_t t0, uint64_t t1, uint32_t t2, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t hi, uint64_t lo, uint64_t &io_h2, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint32_t a0 = (uint32_t)w64_a0;
    uint64_t a1 = 0;
    uint64_t a3 = 0;
    uint32_t fcr31 = 0;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f20));
    { f20 = ((float)(f12)); A3_CHARGE(); uint64_t a7c[5] = {0u, t0, t1, 0u, 0u}; v0 = GetAnimObjResource(ram, rt, w64_a0, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFF0u), f12, f13, f14, f15, f16, f17, f18, f19, hi, lo, h2, a7c); a3 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; }
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    f0 = a1_flt(0u);
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)12) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_10b7fa4;
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)a1, 2)));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)2));
    { const bool c = (int64_t)v0 == 0; v1 = 1u; if (c) goto L_10b7fa4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a1, 4))));
    { const bool c = (int64_t)v0 != (int64_t)(int32_t)v1; (a0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295)))); if (c) goto L_10b7f10; }
    { int32_t tmp = a1f::cvtw(f20); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    goto L_10b7f30;
L_10b7f10:;
    f0 = a1_flt(a7_opq(a0));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::div(fcr31, f20, f0);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
L_10b7f30:;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f20 = a1f::sub(f20, f0);
    { int32_t tmp = a1f::cvtw(f20); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 68))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15232 << 16))));
    at = at | 0x8080u;
    f4 = a1_flt(a7_opq(at));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    f3 = a1_flt(a7_opq(at));
    { uint32_t bits = a7_opq(v1); std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 1)));
    (a0 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    f0 = a1_flt(a7_opq(v1));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f2 = a1f::sub(f20, f2);
    f1 = a1_flt(a7_opq(a0));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f4);
    f3 = a1f::subFinite(f3, f2);
    f1 = a1f::mul(f1, f4);
    f0 = ((float)(f0) * (float)(f2));
    f1 = ((float)(f1) * (float)(f3));
    f0 = a1f::addFinite(f0, f1);
L_10b7fa4:;
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 8));
    { if (xo) { xo[0] = a1_bits(f20); xo[1] = a3; xo[2] = t0; xo[3] = t1; } io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// charrootdyn_acceptMotionRootOffset (0x10d3d20): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3 xo[7]=hi xo[8]=lo; VU0 in vf0 vf6 vf10 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h4, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 4)
inline a1::RetVF charrootdyn_acceptMotionRootOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
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
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw1 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967056))));
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 224, bits); }
    f20 = ((float)(f12));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 3200))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::setzero_si128());
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10d41c0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 448))));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32)))); if (c) goto L_10d3f88; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); a3xw1 = true;
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10d3da8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d3dac;
L_10d3da8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3dac:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D3DB0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10d3de0; }
    { const uint32_t _dynInsn = A1_R32(0x10D3DB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10d3de0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw7 = true; a3xw8 = true;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10d3de4;
L_10d3de0:;
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3de4:;
    { const bool c = (int64_t)s1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (c) goto L_10d3f88; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 316))));
    { (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = iosGetStabXZVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf5, vf7, vacc, vq, vstat, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; GetStabYRotQuaternion_x13(ram, rt, a0, a1, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf4, vf5, vf6, vf7, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat, a7c); a2 = a7c[0]; a3 = a7c[1]; a3xw1 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw3 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw4 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw5 = true;
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw6 = true;
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw3 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw4 = true;
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw5 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw6 = true;
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf29));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    f0 = a1f::mul(f0, f20);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h4, a0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_10d3f60; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10d3f60:;
    f0 = a1f::mul(f1, f0);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 184, bits); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 176); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_10d4188; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10d4188;
L_10d3f88:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 4416))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 176))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); a3xw1 = true;
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10d3fb0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d3fb4;
L_10d3fb0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3fb4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D3FB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10d3fe8; }
    { const uint32_t _dynInsn = A1_R32(0x10D3FBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10d3fe8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } a3xw7 = true; a3xw8 = true;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10d3fec;
L_10d3fe8:;
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3fec:;
    { const bool c = (int64_t)s1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (c) goto L_10d41c0; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 316))));
    { (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = iosGetStabXZVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf5, vf7, vacc, vq, vstat, h2); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; GetStabYRotQuaternion_x13(ram, rt, a0, a1, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf4, vf5, vf6, vf7, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat, a7c); a2 = a7c[0]; a3 = a7c[1]; a3xw1 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw3 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw4 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw5 = true;
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw6 = true;
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw3 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw4 = true;
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw5 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw6 = true;
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf29));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    f0 = a1f::mul(f0, f20);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h4, a0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_10d4168; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10d4168:;
    f0 = a1f::mul(f1, f0);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 184, bits); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 176); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_10d4188; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10d4188:;
    f0 = a1f::mul(f1, f0);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 176, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf10));
    goto L_10d41c8;
L_10d41c0:;
    A4_W128(ADD32((uint32_t)s2, 0), a4v::setzero_si128());
L_10d41c8:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 224); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 240)))); if (xo) { xo[0] = a1_bits(f20); if (a3xw1) xo[1] = a2; xo[2] = a3; if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = t2; if (a3xw6) xo[6] = t3; if (a3xw7) xo[7] = hi; if (a3xw8) xo[8] = lo; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetMotionNodeQuaternionPosToolCoord (0x10b63f8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3 xo[4]=f4 xo[5]=f5 xo[6]=f6 xo[7]=f12 xo[8]=f20 xo[9]=a1 xo[10]=a2 xo[11]=a3 xo[12]=t0 xo[13]=t1 xo[14]=t2 xo[15]=hi xo[16]=lo; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5, 64-bit inputs a0, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t GetMotionNodeQuaternionPosToolCoord(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a0 = (uint32_t)w64_a0;
    uint64_t t1 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint64_t a7c[17] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f12), (uint64_t)a1_bits(f20), 0u, 0u, a3, t0, t1, t2, hi, lo}; const uint64_t a7_t = manager_sub_010AC4A0_x13(ram, rt, v0, w64_a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64((uint32_t)(sp + 0x0u)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); a1 = a7c[9]; a2 = a7c[10]; a3 = a7c[11]; t0 = a7c[12]; t1 = a7c[13]; t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); xo[2] = a1_bits(f2); xo[3] = a1_bits(f3); xo[4] = a1_bits(f4); xo[5] = a1_bits(f5); xo[6] = a1_bits(f6); xo[7] = a1_bits(f12); xo[8] = a1_bits(f20); xo[9] = a1; xo[10] = a2; xo[11] = a3; xo[12] = t0; xo[13] = t1; xo[14] = t2; xo[15] = hi; xo[16] = lo; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return a7_t; }
    __builtin_unreachable();
}

// updateLightGeo (0x110b038): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5, 64-bit inputs t2)
inline uint64_t updateLightGeo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t w64_t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint64_t v1 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = (uint32_t)w64_t2;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint32_t s6 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967136))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 16))));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 32)))); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s6 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    { (s3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 316)))); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    t0 = 1u;
    { const bool c = (int32_t)s2 < 0; (s4 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_110b0e0; }
    if ((int32_t)s2 <= 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 348)))); goto L_110b0c4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(s2 & RC_U64X(v0)));
    if ((int64_t)v0 != 0) goto L_110b0e0;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 348))));
L_110b0c4:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 340))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v1))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_110b168;
L_110b0e0:;
    { A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1, a2, a3, a1_sx64(t0), w64_t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[8]); a3xw0 = true; a3xw1 = true; }
    (v1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf4));
L_110b168:;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
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
    A4_W128(ADD32(s3, 0), a4v::castps_si128(vf13));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(s3, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s3, 0), a4v::castps_si128(vf15));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(s5, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s6, 0), a4v::castps_si128(vf10));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// update_camera_region_geometry (0x110b2a0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f12 xo[2]=f20 xo[3]=t2; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5)
inline uint64_t update_camera_region_geometry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8, uint64_t *xo)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint32_t s6 = 0;
    uint64_t s7 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967072))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 20464))));
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (a1 = (uint64_t)(A1_R64B((uint32_t)v0, 8)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)v0, 0)));
    A1_W64B((uint32_t)sp, 40, a1);
    { A1_W64B((uint32_t)sp, 32, v1); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    { const bool c = (int64_t)s3 == 0; (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_110b314; }
    { (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int32_t)43) ? 1 : 0)); goto L_110b318; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 144))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 32), a4::mk128(h2, v0));
L_110b314:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int32_t)43) ? 1 : 0));
L_110b318:;
    if ((int64_t)v0 == 0) { goto L_110b4d4; }
    (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int32_t)39) ? 1 : 0));
    if ((int64_t)v0 != 0) { goto L_110b4d4; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h3, v1));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (s5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 32))));
    (s7 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 308))));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 316)))); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 144))));
    { const bool c = (int32_t)s2 < 0; (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_110b3a8; }
    if ((int32_t)s2 <= 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 348)))); goto L_110b390; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(s2 & RC_U64X(v0)));
    if ((int64_t)v0 != 0) { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); goto L_110b3ac; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 348))));
L_110b390:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 340))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v1))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_110b448;
L_110b3a8:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
L_110b3ac:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1, a2, a3, t0, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); t2 = a7c[14]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    (v1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)a1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)a1, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf4));
L_110b448:;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(s5, 0)));
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
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf13));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf15));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(s6, 0), a4::mk128(h2, v0));
L_110b4d4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f12); if (a3xw2) xo[2] = a1_bits(f20); if (a3xw3) xo[3] = t2; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// nico_player_edgecolfit_HandEffector (0x13ccca0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5, relocated calls 3)
inline uint64_t nico_player_edgecolfit_HandEffector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    a1 = 0x11E0000u;
    a1 = a1 + 0x7090u;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    { (a2 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1_sx64(a1), a2, a1_sx64(a3), t0, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; }
    a1 = 0x11E0000u;
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    a1 = a1 + 0x70A0u;
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1_sx64(a1), a2, a1_sx64(a3), t0, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); }
    (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    (s3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    { (s0 = (uint32_t)(a1_sx64(s0) & RC_U64X(v0))); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 36))));
    s0 = s0 << 6;
    v1 = 0x11E0000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(s0, (uint32_t)a0))));
    v1 = v1 + 0x70B0u;
    s0 = s0 + 0x10u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// nico_player_colfit_HandEffector (0x13ccd60): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5, relocated calls 3)
inline uint64_t nico_player_colfit_HandEffector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t2, uint32_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    a1 = 0x11E0000u;
    a1 = a1 + 0x70A0u;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    { (a2 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1_sx64(a1), a2, a1_sx64(a3), t0, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); t2 = a7c[14]; hi = a7c[15]; lo = a7c[16]; }
    a1 = 0x11E0000u;
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    a1 = a1 + 0x7090u;
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1_sx64(a1), a2, a1_sx64(a3), t0, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); }
    (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    (s3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    { (s0 = (uint32_t)(a1_sx64(s0) & RC_U64X(v0))); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 36))));
    s0 = s0 << 6;
    v1 = 0x11E0000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(s0, (uint32_t)a0))));
    v1 = v1 + 0x70B0u;
    s0 = s0 + 0x10u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// nico_player_pendulum_HandEffector (0x13ccea0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf1 vf2 vf3 vf4 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat h2 h5 h6 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vi15 vacc vq vstat h2 h3 h4 h5, relocated calls 3, 64-bit inputs t2)
inline uint64_t nico_player_pendulum_HandEffector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t w64_t2, uint32_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t hi, uint64_t lo, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h8)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t2 = (uint32_t)w64_t2;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
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
    a4::vec4 vf18 = io_vf18;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    sp = sp - 0x40u;
    t0 = 1u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 28816))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    { (a2 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[17] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetMotionNodeQuaternionPosToolCoord(ram, rt, a0, a1, a2, a1_sx64(a3), a1_sx64(t0), w64_t2, t3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, hi, lo, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf18, vi15, vacc, vq, vstat, h2, h3, h4, h5, h6, h8, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); f4 = a1_flt((uint32_t)a7c[4]); f5 = a1_flt((uint32_t)a7c[5]); f6 = a1_flt((uint32_t)a7c[6]); f12 = a1_flt((uint32_t)a7c[7]); f20 = a1_flt((uint32_t)a7c[8]); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 112))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    { (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 292))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = v1 == 0; (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)65535)); if (c) goto L_13ccf78; }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    { (s0 = (uint64_t)(RC_U64X(s0) & RC_U64X(a1))); A3_CHARGE(); v0 = GetLayoutClass_u13(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 36))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 6))));
    v1 = 0x11E0000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)a0))));
    v1 = v1 + 0x70B0u;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)a0, 28900, (uint32_t)v0);
    goto L_13ccf80;
L_13ccf78:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    A1_W32B((uint32_t)v0, 28900, 0u);
L_13ccf80:;
    { sp = sp + 0x40u; io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; return v0; }
    __builtin_unreachable();
}
