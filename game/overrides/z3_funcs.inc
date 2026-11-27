// Round Z3 (SOTC_Z3=1 with d1native; z3_rootdyn.h / z3_rootdyn.cpp, docs/decomp_z3.md): the characters' root-dynamics
// helpers under xz as host code on this file's helpers. Included at the end of decomp_collision.cpp (G, guestCall,
// layoutClass).
//
// Each function is written from its MIPS (the recompiled code in build/runner): the same guest reads and writes in the
// same order where they can alias, the VU0 operations as the recompiler's own statements on host registers (V[], ACC:
// the same macros, lanes, masks and clamps, so the same bits), the FPU operations as its FPU_* / ps2FpuDivide with the
// context's fcr31. What the caller and the callees can read is as the original leaves it: v0, s0-s7 / fp / sp / ra
// (never changed), f0 and fcr31 where the original writes them, the VU0 registers and ACC (stored into the context
// before each guest call and at the exit), each guest call's argument registers, ra and frame sp. Not written: the
// frames' save slots and the temporaries no callee reads (dead after the return; the check counts the dead stack window
// apart), the scratch GPRs no caller reads.
namespace decomp_collision
{
    namespace z3
    {
        // The VU0 registers the functions use (vf0-vf17, ACC), on the host between two guest calls.
        struct Vu
        {
            __m128 V[18];
            __m128 ACC;
            PS2_FORCE_INLINE void load(const R5900Context *ctx)
            {
                for (int i = 0; i < 18; ++i)
                    V[i] = ctx->vu0_vf[i];
                ACC = ctx->vu0_acc;
            }
            PS2_FORCE_INLINE void store(R5900Context *ctx) const
            {
                for (int i = 1; i < 18; ++i)
                    ctx->vu0_vf[i] = V[i];
                ctx->vu0_acc = ACC;
            }
        };

        PS2_FORCE_INLINE void call(const G g, ::z3rd::Fn f, uint32_t target, uint32_t ra, uint32_t sp)
        {
            ::z3rd::countGuestCall(f);
            guestCall(g.rdram, g.ctx, g.runtime, target, ra, sp);
        }
        PS2_FORCE_INLINE void setA(R5900Context *ctx, int r, uint32_t v) { SET_GPR_U32(ctx, r, v); }
        PS2_FORCE_INLINE int32_t s8(const G g, uint32_t a) { return static_cast<int8_t>(g.u8(a)); }
        PS2_FORCE_INLINE uint64_t u64(const G g, uint32_t a) { return static_cast<uint64_t>(g.u32(a)) | static_cast<uint64_t>(g.u32(a + 4u)) << 32; }
        __attribute__((noinline)) void put8Slow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint8_t v) { WRITE8(a, v); }
        PS2_FORCE_INLINE void put8(const G g, uint32_t a, uint8_t v)
        {
            if (__builtin_expect(G::plain(a, 1u), 1))
                g.rdram[a] = v;
            else
                put8Slow(g.rdram, g.ctx, g.runtime, a, v);
        }

        // GetLayoutClass (0x10f1a20): the plain cases on the host, else the guest call (a0 the object, ra / sp the site).
        PS2_FORCE_INLINE uint32_t layoutClassAt(const G g, ::z3rd::Fn f, uint32_t object, uint32_t ra, uint32_t sp)
        {
            uint32_t cls;
            if (layoutClass(g, object, cls))
                return cls;
            setA(g.ctx, 4, object);
            call(g, f, 0x010F1A20u, ra, sp);
            return GPR_U32(g.ctx, 2);
        }

        // ---- GetGeometryPointerObject (0x11329c0) -----------------------------------------------------------------
        // a0 a geometry pointer: by its type (+0x30, 1..10) through the table at 0x1195700: its object (+0) or 0.
        // False: a table entry this code does not know (the caller makes the guest call).
        PS2_FORCE_INLINE bool geoObjCore(const G g, uint32_t gp, uint32_t &out)
        {
            const uint32_t t = g.u32(gp + 0x30u) - 1u;
            if (t >= 10u)
            {
                out = 0u;
                return true;
            }
            const uint32_t target = g.u32(0x01195700u + t * 4u);
            if (target == 0x011329F0u)
                out = g.u32(gp);
            else if (target == 0x011329F8u)
                out = 0u;
            else
                return false;
            return true;
        }

        // ---- GetCollisionDBAttribute (0x1125a08) ------------------------------------------------------------------
        // a0 the 16-byte attribute out, a1 the DB reference (+0 the object, +4 the part (bits 24-30) and surface (0-23);
        // bit 31 clear: an indexed surface): the object's layout class's collision (+0x184 -> +0x20): the part's
        // surface record (+8 + part x 0x90; byte +8 set: +0x14 + surface x 20 (u16), else +0x14 + surface x 52 + 0x30)
        // or the indexed form (+0x54 list -> +0x18 + index x 64 + 0x3c); the attribute is the collision's +0x34 table
        // entry (x 16). v0 the entry's address. False: the class needs GetLayoutClass's guest call (nothing written;
        // the caller runs the old function).
        PS2_FORCE_INLINE bool dbAttrCore(const G g, uint32_t out, uint32_t db, uint32_t &v0)
        {
            uint32_t cls;
            if (!layoutClass(g, g.u32(db), cls))
                return false;
            const uint32_t w1 = g.u32(db + 4u);
            const uint32_t coll1 = g.u32(cls + 0x184u);
            const uint32_t part = (w1 >> 24) & 0x7Fu, surface = w1 & 0x00FFFFFFu;
            const uint32_t s3 = g.u32(coll1 + 0x20u);
            // the second GetLayoutClass of the same object: the same class (nothing written between)
            const uint32_t coll2 = g.u32(cls + 0x184u);
            const uint32_t w1b = g.u32(db + 4u);
            const uint32_t a2 = part * 144u, a3 = surface << 2;
            const uint32_t a1 = g.u32(coll2 + 0x20u);
            uint32_t idx;
            if (static_cast<int32_t>(w1b) >= 0)
            {
                const uint32_t rec = g.u32(a1 + 8u) + a2;
                const uint32_t at = g.u32(rec + 0x54u) + a3;
                const uint32_t n = g.u32(at);
                idx = g.u32((n << 6) + g.u32(a1 + 0x18u) + 0x3Cu);
            }
            else
            {
                const uint32_t rec = g.u32(a1 + 8u) + a2;
                if (g.u8(rec + 8u) != 0u)
                    idx = g.u16(((a3 + surface) << 2) + g.u32(rec + 0x14u));
                else
                    idx = g.u32(((((surface << 1) + surface) << 2) + surface) * 4u + g.u32(rec + 0x14u) + 0x30u);
            }
            v0 = g.u32(s3 + 0x34u) + (idx << 4);
            g.put(out, g.q(v0));
            return true;
        }

        // ---- UpdateLayoutClassParentGeometry (0x10f22b8) -------------------------------------------------------------
        // a0 the layout class, a1 an offset (or 0), a2 a local point (or 0). The class's parent geometry pointer (+0xc0,
        // when +0x124 is set): its quaternion and position (GetGeometryPointerQuatPos, a guest call: an 11-type switch),
        // the position into +0x134 (plus a1), the parent's quaternion times the class's root quaternion (+0x60) into
        // +0x13c, a2 turned by it added to the position; then the parent object's class: its +0x19c record's +0x8c byte
        // decides whether the position is copied to +0x138 and the class's own +0x19c +0x8c byte set. v0 1 / 0.
        // vu: loaded from the context at the entry; the caller stores it at its exit.
        uint32_t layoutGeoCore(const G g, Vu &vu, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t spIn)
        {
            R5900Context *ctx = g.ctx;
            __m128 *V = vu.V;
            __m128 &ACC = vu.ACC;
            const uint32_t sp = spIn - 0x60u;
            if (const uint32_t r = g.u32(s0 + 0x19Cu))
                put8(g, r + 0x8Cu, 0u);
            if (g.u32(s0 + 0x124u) == 0u)
                return 0u;
            const uint32_t s3 = s0 + 0xC0u;
            setA(ctx, 4, sp);
            setA(ctx, 5, s3);
            call(g, ::z3rd::kLayoutGeo, 0x01132270u, 0x010F2300u, sp); // GetGeometryPointerQuatPos(sp, s3) (VU unchanged before it)
            vu.load(ctx);
            g.put(g.u32(s0 + 0x134u), g.q(sp + 0x10u));
            if (s1 != 0u)
            {
                const uint32_t v0 = g.u32(s0 + 0x134u);
                V[5] = g.q(v0);
                V[6] = g.q(s1);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
                g.put(v0, V[5]);
            }
            {
                const uint32_t v1 = g.u32(s0 + 0x13Cu);
                V[11] = g.q(sp);
                V[12] = g.q(s0 + 0x60u);
// vmul.xyzw $vf13, $vf11, $vf12
{ __m128 res = PS2_VMUL(V[11], V[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); V[13] = PS2_VBLEND(V[13], res, _mm_castsi128_ps(mask)); }
// vaddy.x $vf13, $vf13, $vf13y
{ __m128 res = PS2_VADD_K(1, 1, V[13], _mm_shuffle_ps(V[13], V[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[13] = _mm_blendv_ps(V[13], res, _mm_castsi128_ps(mask)); }
// vaddz.x $vf13, $vf13, $vf13z
{ __m128 res = PS2_VADD_K(1, 1, V[13], _mm_shuffle_ps(V[13], V[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[13] = _mm_blendv_ps(V[13], res, _mm_castsi128_ps(mask)); }
// vsubx.w $vf13, $vf13, $vf13x
{ __m128 res = PS2_VSUB_K(1, 1, V[13], _mm_shuffle_ps(V[13], V[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[13] = _mm_blendv_ps(V[13], res, _mm_castsi128_ps(mask)); }
// vmulw.xyz $vf14, $vf12, $vf11w
{ __m128 res = PS2_VMUL(V[12], _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[14] = _mm_blendv_ps(V[14], res, _mm_castsi128_ps(mask)); }
// vmulw.xyz $vf15, $vf11, $vf12w
{ __m128 res = PS2_VMUL(V[11], _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf12, $vf11
{ __m128 fs_yzx = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vopmsub.xyz $vf16, $vf11, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[16] = _mm_blendv_ps(V[16], res, _mm_castsi128_ps(mask)); }
// vadd.xyz $vf13, $vf14, $vf15
{ __m128 res = PS2_VADD_K(1, 1, V[14], V[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[13] = PS2_VBLEND(V[13], res, _mm_castsi128_ps(mask)); }
// vadd.xyz $vf13, $vf13, $vf16
{ __m128 res = PS2_VADD_K(1, 1, V[13], V[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[13] = PS2_VBLEND(V[13], res, _mm_castsi128_ps(mask)); }
                g.put(v1, V[13]);
            }
            if (s2 != 0u)
            {
                const uint32_t v0 = g.u32(s0 + 0x13Cu);
                V[8] = g.q(s2);
// vaddw.xyz $vf12, $vf0, $vf0w
{ __m128 res = PS2_VADD_K(1, 1, V[0], _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[12] = _mm_blendv_ps(V[12], res, _mm_castsi128_ps(mask)); }
                V[11] = g.q(v0);
// vmr32.w $vf14, $vf0
{ __m128 res = _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[14] = _mm_blendv_ps(V[14], res, _mm_castsi128_ps(mask)); }
// vmr32.w $vf15, $vf0
{ __m128 res = _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vmr32.w $vf16, $vf0
{ __m128 res = _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[16] = _mm_blendv_ps(V[16], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf11, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmadd.xyz $vf11, $vf0, $vf0
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[0], V[0]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[11] = _mm_blendv_ps(V[11], res, _mm_castsi128_ps(mask)); }
// vmadd.xyz $vf10, $vf11, $vf12
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[11], V[12]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[10] = _mm_blendv_ps(V[10], res, _mm_castsi128_ps(mask)); }
// vmul.xyz $vf13, $vf11, $vf10
{ __m128 res = PS2_VMUL_K(1, 1, V[11], V[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[13] = PS2_VBLEND(V[13], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf11, $vf10
{ __m128 fs_yzx = _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[10], V[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmaddw.xyz $vf15, $vf10, $vf11w
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[10], _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vmsubw.xyz $vf16, $vf10, $vf11w
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[10], _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[16] = _mm_blendv_ps(V[16], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf13, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[13], V[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmadd.xyz $vf17, $vf13, $vf12
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[13], V[12]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[17] = _mm_blendv_ps(V[17], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf15, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[15], V[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmadd.xyz $vf15, $vf0, $vf0
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[0], V[0]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vsub.xyz $vf14, $vf12, $vf17
{ __m128 res = PS2_VSUB_K(1, 1, V[12], V[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[14] = PS2_VBLEND(V[14], res, _mm_castsi128_ps(mask)); }
// vmove.yz $vf17, $vf14
{ __m128i mask = _mm_set_epi32(0, -1, -1, 0); V[17] = _mm_blendv_ps(V[17], V[14], _mm_castsi128_ps(mask)); }
// vmove.y $vf14, $vf16
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); V[14] = _mm_blendv_ps(V[14], V[16], _mm_castsi128_ps(mask)); }
// vmove.y $vf16, $vf15
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); V[16] = _mm_blendv_ps(V[16], V[15], _mm_castsi128_ps(mask)); }
// vmove.y $vf15, $vf17
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); V[15] = _mm_blendv_ps(V[15], V[17], _mm_castsi128_ps(mask)); }
// vmove.z $vf14, $vf15
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); V[14] = _mm_blendv_ps(V[14], V[15], _mm_castsi128_ps(mask)); }
// vmove.z $vf15, $vf16
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); V[15] = _mm_blendv_ps(V[15], V[16], _mm_castsi128_ps(mask)); }
// vmove.z $vf16, $vf17
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); V[16] = _mm_blendv_ps(V[16], V[17], _mm_castsi128_ps(mask)); }
// vmulax.xyzw $ACC, $vf14, $vf8x
{ __m128 res = PS2_VMUL_K(1, 0, V[14], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(0,0,0,0))); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
// vmadday.xyzw $ACC, $vf15, $vf8y
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[15], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
// vmaddaz.xyzw $ACC, $vf16, $vf8z
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[16], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
// vmaddw.xyzw $vf10, $vf0, $vf8w
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[0], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); V[10] = _mm_blendv_ps(V[10], res, _mm_castsi128_ps(mask)); }
                const uint32_t p = g.u32(s0 + 0x134u);
                V[5] = g.q(p);
                V[6] = V[10];
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
                g.put(p, V[5]);
            }
            uint32_t obj;
            if (!geoObjCore(g, s3, obj))
            {
                vu.store(ctx);
                setA(ctx, 4, s3);
                call(g, ::z3rd::kLayoutGeo, 0x011329C0u, 0x010F2404u, sp);
                vu.load(ctx);
                obj = GPR_U32(ctx, 2);
            }
            else
                ::z3rd::countInline(::z3rd::kGeoObj);
            if (obj == 0u)
                return 0u;
            uint32_t cls;
            if (!layoutClass(g, obj, cls))
            {
                vu.store(ctx);
                setA(ctx, 4, obj);
                call(g, ::z3rd::kLayoutGeo, 0x010F1A20u, 0x010F2414u, sp);
                vu.load(ctx);
                cls = GPR_U32(ctx, 2);
            }
            if (cls == 0u)
                return 0u;
            const uint32_t rec = g.u32(cls + 0x19Cu);
            if (rec == 0u || s8(g, rec + 0x8Cu) == 0)
                return 0u;
            g.put(g.u32(s0 + 0x138u), g.q(g.u32(s0 + 0x134u)));
            if (const uint32_t own = g.u32(s0 + 0x19Cu))
                put8(g, own + 0x8Cu, 1u);
            return 1u;
        }

        // ---- charrootdyn_updateCharacterClassParentGeometry (0x10df218) --------------------------------------------
        // a0 the character class (a1 / a2 passed on): UpdateLayoutClassParentGeometry (on the host); when it moved the
        // class or the class's +0x10a0 byte is set: the position copied to +0x138, the frame of the root quaternion
        // (+0x13c) and position (+0x134) applied to the local point +0x3c0 into +0x4d0. v0 1 / 0.
        uint32_t parentGeoCore(const G g, Vu &vu, uint32_t s0, uint32_t a1, uint32_t a2, uint32_t spIn)
        {
            __m128 *V = vu.V;
            __m128 &ACC = vu.ACC;
            const uint32_t sp = spIn - 0x50u;
            ::z3rd::countInline(::z3rd::kLayoutGeo);
            const uint32_t moved = layoutGeoCore(g, vu, s0, a1, a2, sp);
            if (moved == 0u && s8(g, s0 + 0x10A0u) == 0)
                return 0u;
            g.put(g.u32(s0 + 0x138u), g.q(g.u32(s0 + 0x134u)));
            const uint32_t v1 = g.u32(s0 + 0x13Cu), a0 = g.u32(s0 + 0x134u);
// vaddw.xyz $vf12, $vf0, $vf0w
{ __m128 res = PS2_VADD_K(1, 1, V[0], _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[12] = _mm_blendv_ps(V[12], res, _mm_castsi128_ps(mask)); }
            V[11] = g.q(v1);
// vmr32.w $vf14, $vf0
{ __m128 res = _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[14] = _mm_blendv_ps(V[14], res, _mm_castsi128_ps(mask)); }
// vmr32.w $vf15, $vf0
{ __m128 res = _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vmr32.w $vf16, $vf0
{ __m128 res = _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[16] = _mm_blendv_ps(V[16], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf11, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmadd.xyz $vf11, $vf0, $vf0
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[0], V[0]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[11] = _mm_blendv_ps(V[11], res, _mm_castsi128_ps(mask)); }
// vmadd.xyz $vf10, $vf11, $vf12
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[11], V[12]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[10] = _mm_blendv_ps(V[10], res, _mm_castsi128_ps(mask)); }
// vmul.xyz $vf13, $vf11, $vf10
{ __m128 res = PS2_VMUL_K(1, 1, V[11], V[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[13] = PS2_VBLEND(V[13], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf11, $vf10
{ __m128 fs_yzx = _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[10], V[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmaddw.xyz $vf15, $vf10, $vf11w
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[10], _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vmsubw.xyz $vf16, $vf10, $vf11w
{ __m128 mul_res = PS2_VMUL_K(1, 0, V[10], _mm_shuffle_ps(V[11], V[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[16] = _mm_blendv_ps(V[16], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf13, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[13], V[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmadd.xyz $vf17, $vf13, $vf12
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[13], V[12]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[17] = _mm_blendv_ps(V[17], res, _mm_castsi128_ps(mask)); }
// vopmula.xyz $ACC, $vf15, $vf12
{ __m128 fs_yzx = _mm_shuffle_ps(V[15], V[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(V[12], V[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
// vmadd.xyz $vf15, $vf0, $vf0
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[0], V[0]); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[15] = _mm_blendv_ps(V[15], res, _mm_castsi128_ps(mask)); }
// vsub.xyz $vf14, $vf12, $vf17
{ __m128 res = PS2_VSUB_K(1, 1, V[12], V[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[14] = PS2_VBLEND(V[14], res, _mm_castsi128_ps(mask)); }
// vmove.yz $vf17, $vf14
{ __m128i mask = _mm_set_epi32(0, -1, -1, 0); V[17] = _mm_blendv_ps(V[17], V[14], _mm_castsi128_ps(mask)); }
// vmove.y $vf14, $vf16
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); V[14] = _mm_blendv_ps(V[14], V[16], _mm_castsi128_ps(mask)); }
// vmove.y $vf16, $vf15
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); V[16] = _mm_blendv_ps(V[16], V[15], _mm_castsi128_ps(mask)); }
// vmove.y $vf15, $vf17
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); V[15] = _mm_blendv_ps(V[15], V[17], _mm_castsi128_ps(mask)); }
// vmove.z $vf14, $vf15
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); V[14] = _mm_blendv_ps(V[14], V[15], _mm_castsi128_ps(mask)); }
// vmove.z $vf15, $vf16
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); V[15] = _mm_blendv_ps(V[15], V[16], _mm_castsi128_ps(mask)); }
// vmove.z $vf16, $vf17
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); V[16] = _mm_blendv_ps(V[16], V[17], _mm_castsi128_ps(mask)); }
            V[4] = g.q(a0);
// vmove.w $vf4, $vf0
{ __m128i mask = _mm_set_epi32(-1, 0, 0, 0); V[4] = _mm_blendv_ps(V[4], V[0], _mm_castsi128_ps(mask)); }
            V[8] = g.q(s0 + 0x3C0u);
            V[17] = V[4]; // the rows through the frame (sp .. sp + 0x30) reloaded as they were stored
// vmulax.xyzw $ACC, $vf14, $vf8x
{ __m128 res = PS2_VMUL(V[14], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(0,0,0,0))); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
// vmadday.xyzw $ACC, $vf15, $vf8y
{ __m128 mul_res = PS2_VMUL(V[15], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
// vmaddaz.xyzw $ACC, $vf16, $vf8z
{ __m128 mul_res = PS2_VMUL(V[16], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
// vmaddw.xyzw $vf10, $vf17, $vf8w
{ __m128 mul_res = PS2_VMUL(V[17], _mm_shuffle_ps(V[8], V[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); V[10] = _mm_blendv_ps(V[10], res, _mm_castsi128_ps(mask)); }
            g.put(s0 + 0x4D0u, V[10]);
            return 1u;
        }

        // mfc1 v0, f0 (the low doubleword: f0's bits sign-extended; the high doubleword still the last qmfc2's) then
        // qmtc2.ni v0, vf6.
        PS2_FORCE_INLINE __m128 viaV0(float f, __m128 hiFrom)
        {
            uint32_t w[4];
            std::memcpy(w, &hiFrom, 16);
            std::memcpy(&w[0], &f, 4);
            w[1] = static_cast<int32_t>(w[0]) < 0 ? 0xFFFFFFFFu : 0u;
            __m128 r;
            std::memcpy(&r, w, 16);
            return r;
        }

        // ---- charrootdyn_slidecollisionCB (0x10d6e30) ----------------------------------------------------------------
        // a0 the result (+0 the end, +0x10 the moved end, +0x20 the hit's object, +0x30 the normal, +0x40.. the kept hit,
        // +0x70 a flag, +0x80 / +0x84 the attribute range), a1 the start, a2 the end, a3 the radius offset, t0 / t1 the
        // filters, t2 the exception callback (or 0), t3 its argument, f12 / f13 the slide's limits. The first hit along
        // start + offset -> end + offset (ClipCollisionFinderWithKeep4DSlopeLimit, or its callback form): none -> the
        // ends written back, v0 0. Else the move cut at the hit plane, the hit object's boss DB info (getBossDBInfo), the
        // slide along the wall (SlideCollisionFinder4D or its callback form): v0 its answer; the end pushed out of the
        // plane when behind it (GetPointDistanceFromPlane), the hit record, the attribute range widened, the kept hit.
        // The VU0 registers are the context's (guest calls between every few operations).
        uint32_t slideCBCore(const G g, uint32_t spIn)
        {
            R5900Context *ctx = g.ctx;
            __m128 *V = ctx->vu0_vf;
            __m128 &ACC = ctx->vu0_acc;
            constexpr ::z3rd::Fn F = ::z3rd::kSlideCB;
            const uint32_t s1 = GPR_U32(ctx, 4), s0 = GPR_U32(ctx, 5), s6 = GPR_U32(ctx, 6), s2in = GPR_U32(ctx, 7);
            const uint32_t s7 = GPR_U32(ctx, 8), fp = GPR_U32(ctx, 9), t2 = GPR_U32(ctx, 10), t3 = GPR_U32(ctx, 11);
            const float f20 = ctx->f[12], f21 = ctx->f[13];
            const uint32_t sp = spIn - 0x1A0u;
            const uint32_t s4 = s1 + 0x40u;
            g.put32(s4 + 0x30u, 0u);
            g.put(sp + 0x70u, g.q(s0));
            g.put(sp + 0x80u, g.q(s6));
            g.put32(sp + 0x130u, t3);
            V[5] = g.q(sp + 0x80u);
            V[6] = g.q(s2in);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
            g.put(sp, V[5]);
            V[5] = g.q(s0);
            V[6] = g.q(s2in);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
            g.put(sp + 0x10u, V[5]);
            setA(ctx, 4, sp + 0x20u);
            setA(ctx, 5, sp + 0x70u);
            setA(ctx, 6, sp);
            setA(ctx, 7, s7);
            setA(ctx, 8, fp);
            SET_GPR_U32(ctx, 2, sp + 0x20u);
            if (t2 != 0u)
            {
                setA(ctx, 9, t2);
                setA(ctx, 10, 0u);
                call(g, F, 0x0111EF10u, 0x010D6EF4u, sp); // ClipCollisionFinderWithKeep4DSlopeLimitCB
            }
            else
                call(g, F, 0x0111ECF0u, 0x010D6F18u, sp); // ClipCollisionFinderWithKeep4DSlopeLimit
            const uint32_t s3 = sp + 0x30u;
            if (GPR_U32(ctx, 2) == 0u)
            {
                g.put(s1 + 0x10u, g.q(sp + 0x10u));
                g.put(s1, g.q(sp));
                return 0u;
            }
            V[5] = g.q(s3);
            V[6] = g.q(s2in);
// vaddw.x $vf7, $vf0, $vf0w
{ __m128 res = PS2_VADD_K(1, 1, V[0], _mm_shuffle_ps(V[0], V[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[7] = _mm_blendv_ps(V[7], res, _mm_castsi128_ps(mask)); }
// vmul.xyz $vf6, $vf5, $vf6
{ __m128 res = PS2_VMUL(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[6] = PS2_VBLEND(V[6], res, _mm_castsi128_ps(mask)); }
// vaddax.x $ACC, $vf0, $vf6x
{ __m128 res = PS2_VADD_K(1, 1, V[0], _mm_shuffle_ps(V[6], V[6], _MM_SHUFFLE(0,0,0,0))); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
// vmadday.x $ACC, $vf7, $vf6y
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[7], _mm_shuffle_ps(V[6], V[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); ACC = _mm_blendv_ps(ACC, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
// vmaddz.x $vf6, $vf7, $vf6z
{ __m128 mul_res = PS2_VMUL_K(1, 1, V[7], _mm_shuffle_ps(V[6], V[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ACC, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[6] = _mm_blendv_ps(V[6], res, _mm_castsi128_ps(mask)); }
            const __m128 v0a = V[6]; // qmfc2.ni v0, vf6; mtc1 v0, f0
            float f0;
            std::memcpy(&f0, &v0a, 4);
            V[7] = g.q(s3);
// vmul.xyz $vf7, $vf7, $vf7
{ __m128 res = PS2_VMUL(V[7], V[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[7] = PS2_VBLEND(V[7], res, _mm_castsi128_ps(mask)); }
// vaddy.x $vf7, $vf7, $vf7y
{ __m128 res = PS2_VADD_K(1, 1, V[7], _mm_shuffle_ps(V[7], V[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[7] = _mm_blendv_ps(V[7], res, _mm_castsi128_ps(mask)); }
// vaddz.x $vf7, $vf7, $vf7z
{ __m128 res = PS2_VADD_K(1, 1, V[7], _mm_shuffle_ps(V[7], V[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[7] = _mm_blendv_ps(V[7], res, _mm_castsi128_ps(mask)); }
            const __m128 v0b = V[7]; // qmfc2.ni v0, vf7; mtc1 v0, f1
            float f1;
            std::memcpy(&f1, &v0b, 4);
            f0 = ps2FpuDivide(ctx->fcr31, f0, f1);
            ctx->f[0] = f0;
            const uint32_t s2 = sp + 0x90u;
            V[5] = g.q(s3);
            V[6] = viaV0(f0, v0b);
// vmulx.xyz $vf5, $vf5, $vf6x
{ __m128 res = PS2_VMUL(V[5], _mm_shuffle_ps(V[6], V[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = _mm_blendv_ps(V[5], res, _mm_castsi128_ps(mask)); }
            g.put(s2, V[5]);
            V[5] = g.q(s6);
            V[6] = g.q(s2);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
            g.put(sp, V[5]);
            V[5] = g.q(s0);
            V[6] = g.q(s2);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
            g.put(sp + 0x10u, V[5]);
            const uint32_t cls = layoutClassAt(g, F, g.u32(sp + 0x40u), 0x010D6FACu, sp);
            {
                const uint32_t v1 = g.u32(cls + 0x174u);
                const uint32_t w = g.u32(v1 + 0x20u);
                g.put32(s1 + 0x84u, w);
                g.put32(s1 + 0x80u, w);
                setA(ctx, 4, s4);
                setA(ctx, 5, sp + 0x20u);
                SET_GPR_U32(ctx, 2, w);
                SET_GPR_U32(ctx, 3, v1);
                call(g, F, 0x01128B28u, 0x010D6FCCu, sp); // getBossDBInfo(s4, sp + 0x20)
            }
            setA(ctx, 10, g.u32(sp + 0x130u));
            ctx->f[12] = f20;
            ctx->f[13] = f21;
            setA(ctx, 7, s7);
            setA(ctx, 8, fp);
            setA(ctx, 4, sp + 0xA0u);
            setA(ctx, 5, s0);
            setA(ctx, 6, sp);
            if (t2 != 0u)
            {
                setA(ctx, 9, t2);
                call(g, F, 0x01126E30u, 0x010D6FF8u, sp); // SlideCollisionFinder4DCB
            }
            else
                call(g, F, 0x01126C10u, 0x010D7020u, sp); // SlideCollisionFinder4D (t1 as the first finder left it)
            const uint32_t s5 = GPR_U32(ctx, 2);
            if (s5 == 0u)
            {
                g.put(s1 + 0x10u, g.q(sp + 0x10u));
                g.put(s1, g.q(sp));
                return 0u;
            }
            setA(ctx, 4, s3);
            setA(ctx, 5, s0);
            call(g, F, 0x01134198u, 0x010D7034u, sp); // GetPointDistanceFromPlane(s3, s0)
            f1 = ctx->f[0];
            f0 = 0.0f;
            ctx->f[0] = f0;
            const bool behind = FPU_C_OLT_S(f1, f0);
            ctx->fcr31 = behind ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            if (behind)
            {
                V[7] = g.q(s3);
// vmul.xyz $vf7, $vf7, $vf7
{ __m128 res = PS2_VMUL(V[7], V[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[7] = PS2_VBLEND(V[7], res, _mm_castsi128_ps(mask)); }
// vaddy.x $vf7, $vf7, $vf7y
{ __m128 res = PS2_VADD_K(1, 1, V[7], _mm_shuffle_ps(V[7], V[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[7] = _mm_blendv_ps(V[7], res, _mm_castsi128_ps(mask)); }
// vaddz.x $vf7, $vf7, $vf7z
{ __m128 res = PS2_VADD_K(1, 1, V[7], _mm_shuffle_ps(V[7], V[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); V[7] = _mm_blendv_ps(V[7], res, _mm_castsi128_ps(mask)); }
                const __m128 v0c = V[7]; // qmfc2.ni v0, vf7; mtc1 v0, f0
                std::memcpy(&f0, &v0c, 4);
                f0 = ps2FpuDivide(ctx->fcr31, f1, f0);
                f0 = FPU_NEG_S(f0);
                ctx->f[0] = f0;
                const uint32_t t = sp + 0x120u;
                V[5] = g.q(s3);
                V[6] = viaV0(f0, v0c);
// vmulx.xyz $vf5, $vf5, $vf6x
{ __m128 res = PS2_VMUL(V[5], _mm_shuffle_ps(V[6], V[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = _mm_blendv_ps(V[5], res, _mm_castsi128_ps(mask)); }
                g.put(t, V[5]);
                const uint32_t e = s1 + 0x10u;
                V[5] = g.q(s0);
                V[6] = g.q(t);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
                g.put(e, V[5]);
                V[5] = g.q(e);
                V[6] = g.q(s2);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
                g.put(e, V[5]);
            }
            else
            {
                V[5] = g.q(s0);
                V[6] = g.q(s2);
// vadd.xyz $vf5, $vf5, $vf6
{ __m128 res = PS2_VADD(V[5], V[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); V[5] = PS2_VBLEND(V[5], res, _mm_castsi128_ps(mask)); }
                g.put(s1 + 0x10u, V[5]);
            }
            {
                const __m128 rec = g.q(sp + 0xA0u);
                const uint32_t o0 = g.u32(sp + 0x40u), o1 = g.u32(sp + 0x44u);
                g.put(s1, rec);
                g.put32(s1 + 0x20u, o0);
                g.put32(s1 + 0x24u, o1);
                const int32_t a3 = static_cast<int32_t>(g.u32(sp + 0x110u)), t0 = static_cast<int32_t>(g.u32(sp + 0x114u));
                int32_t hi = static_cast<int32_t>(g.u32(s1 + 0x84u)), lo = static_cast<int32_t>(g.u32(s1 + 0x80u));
                const __m128 n = g.q(sp + 0x30u);
                if (a3 < hi)
                    hi = a3;
                if (lo < t0)
                    lo = t0;
                g.put32(s1 + 0x84u, static_cast<uint32_t>(hi));
                g.put(s1 + 0x30u, n);
                g.put32(s1 + 0x80u, static_cast<uint32_t>(lo));
            }
            if (g.u32(s4 + 0x30u) == 0u && g.u32(sp + 0x100u) != 0u)
            {
                static constexpr uint32_t kOrder[8] = {0x8u, 0x0u, 0x10u, 0x18u, 0x20u, 0x28u, 0x30u, 0x38u};
                for (uint32_t o : kOrder)
                    g.put64(s4 + o, u64(g, sp + 0xD0u + o));
            }
            return s5;
        }

        // The exit as the epilogue leaves it: ra and sp as at the entry, v0, pc = ra.
        struct Exit
        {
            R5900Context *ctx;
            uint64_t ra, sp;
            explicit Exit(R5900Context *c) : ctx(c), ra(GPR_U64(c, 31)), sp(GPR_U64(c, 29)) {}
            bool done(uint32_t v0) const
            {
                SET_GPR_U64(ctx, 31, ra);
                SET_GPR_U64(ctx, 29, sp);
                SET_GPR_U32(ctx, 2, v0);
                ctx->pc = static_cast<uint32_t>(ra);
                return true;
            }
        };
    }
}

namespace z3rd
{
    using decomp_collision::G;
    namespace z3 = decomp_collision::z3;

    bool geoObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        uint32_t v0;
        if (!z3::geoObjCore(g, GPR_U32(ctx, 4), v0))
            return false;
        SET_GPR_U32(ctx, 2, v0); // a leaf: no frame
        ctx->pc = GPR_U32(ctx, 31);
        return true;
    }

    bool dbAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const z3::Exit x{ctx};
        uint32_t v0;
        if (!z3::dbAttrCore(g, GPR_U32(ctx, 4), GPR_U32(ctx, 5), v0))
            return false;
        return x.done(v0);
    }

    // GetGeometryPointerSurfaceAttribute (0x1132af0): a0 out, a1 the geometry pointer: by its type (+0x30, below 9)
    // through the table at 0x1195760: a collision-DB surface (the tail call to GetCollisionDBAttribute) or none (out
    // zeroed, v0 0).
    bool surfAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const z3::Exit x{ctx};
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5);
        const uint32_t type = g.u32(gp + 0x30u);
        bool zero;
        if (type >= 9u)
            zero = true;
        else
        {
            const uint32_t target = g.u32(0x01195760u + type * 4u);
            if (target == 0x01132B20u)
                zero = true;
            else if (target == 0x01132B24u)
                zero = gp == 0u;
            else
                return false;
        }
        if (zero)
        {
            g.put(out, _mm_setzero_ps());
            return x.done(0u);
        }
        uint32_t v0;
        if (!z3::dbAttrCore(g, out, gp, v0)) // j GetCollisionDBAttribute
            return false;
        countInline(kDBAttr);
        return x.done(v0);
    }

    bool layoutGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const z3::Exit x{ctx};
        z3::Vu vu;
        vu.load(ctx);
        const uint32_t v0 = z3::layoutGeoCore(g, vu, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 29));
        vu.store(ctx);
        return x.done(v0);
    }

    bool parentGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const z3::Exit x{ctx};
        z3::Vu vu;
        vu.load(ctx);
        const uint32_t v0 = z3::parentGeoCore(g, vu, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 29));
        vu.store(ctx);
        return x.done(v0);
    }

    bool slideCB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const z3::Exit x{ctx};
        return x.done(z3::slideCBCore(g, GPR_U32(ctx, 29)));
    }

    // charrootdyn_slidecollision (0x10dc020): slidecollisionCB with the exception callback slideExceptionJudgeCB
    // (0x10dc808) and its two flags (t2's +0x68 bits 11 / 15, at sp / sp + 4) when t2 is set, else with none.
    bool slide(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const z3::Exit x{ctx};
        const uint32_t sp = GPR_U32(ctx, 29) - 0x20u, t2 = GPR_U32(ctx, 10);
        if (t2 != 0u)
        {
            const uint32_t w = g.u32(t2 + 0x68u);
            g.put32(sp, (w >> 11) & 1u);
            g.put32(sp + 4u, (w >> 15) & 1u);
            SET_GPR_U32(ctx, 10, 0x010DC808u);
            SET_GPR_U32(ctx, 11, sp);
        }
        else
            SET_GPR_U32(ctx, 11, 0u);
        countInline(kSlideCB);
        return x.done(z3::slideCBCore(g, sp));
    }
}
