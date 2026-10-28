// The display walk natively from its top (round 7): DispDispObjGroupModel
// (0x10f52c8, the gamecore display task's call each frame) with the layout
// objects' clip and draw callbacks it calls for the common class.
//
// DispDispObjGroupModel walks the layout display group: for each object its
// layout class (GetLayoutClass) or, without one, its geometry display class
// (GetGeoDispClass); a billboard turn (+0x1b6: the camera's quaternion made
// into two axes, GetQuaternionByAxisDirZY, times the object's own) and a
// link to the camera object (+0x1b7: a point through the camera's matrix,
// the quaternions multiplied), UpdateLayoutClassSimpleGeometry after either;
// then the class's +0xd8 (update) and +0xdc (clip) callbacks, the visible
// flag (+0x260 bit 8 of the geometry, the character's +0xcc0 told when it
// changes) and the object into the sort list (AddSortObj, its distance key
// at +4). The sort list is copied into the second list without the objects
// with bit 4 of +0x260, the light pass runs (decomp_light.cpp) and each
// object is drawn: iosDlLockObj, the class's +0xd4 (lights) and +0xd0
// (draw) callbacks.
//
// The callbacks of the common layout class natively, called straight from
// here (no function table, no guest frame):
//  - ClipLayoutObjModel (0x10f4890, +0xdc): the instance's fade (+0x244),
//    the LOD scale (+0x250: the class's +0x9c over the camera's lens factor
//    +0xc / 500 when the model's LOD record asks), modelClipSolid (or
//    modelClipSkin, a guest call, for skinned models), the LOD level it
//    returns into bits 40-42 of +0x250, the geometry class's sort keys.
//  - modelClipSolid (0x1098840): a model's far cull (+0x30 of its LOD
//    record against the camera distance), the bounding sphere through
//    modelClipPoint (decomp_modelclip.cpp, a guest call), the LOD level by
//    the record's mode (distance to the camera in 3D or on the ground, or
//    the instance's +0x230..0x238) with its hysteresis (+0x24), 01097F88
//    for a model that picks its own (a guest call), the LOD's root box
//    through camera x model against the frustum (clipByMinMaxCube's VU0
//    CLIP test inline).
//  - DispLayoutObjModel (0x10f5920, +0xd0): 010F3A08 for a solid model,
//    the skin palette (010F3CF0, decomp_skinpal.cpp) and the heat haze
//    strips (010F3EB8) for a skinned one as guest calls, DispCloth inline
//    (clothMesh, decomp_cloth.cpp, a guest call per cloth).
//  - 010F3A08: the parts' matrices (the camera's +0x1a0 x the object's,
//    then scaled by the lens factor when the model asks, iosGetScaleMatrix
//    a guest call), or a parent's matrix block taken (+0x194); modelDisp
//    (sotc_native.cpp's hook) and shadowDisp (hooked too) as guest calls.
//  - 010F3EB8 / 010F3FB0 (the heat haze behind the bird, DispLayoutObjModel's
//    skinned path): see strips() below; with it 0108D730 (glVertex's kick,
//    the gl* immediate API's vertex into the GIF packet) and glVertex3fv,
//    registered for every other gl* user too.
//
// The display walk's draws reach the native renderer only through
// sotc_native.cpp's hooks, all called through the function table here (the
// hooks are never replaced; glEnd's tells the haze by its return address).
//
// Every other callee is a guest call through the function table from the
// original's frame (ra its jal site + 8, the frame at sp), with the
// callee-saved registers and the prologue's saves in the context as the
// recompiled code has them there: a guest callee that blocks (a semaphore)
// unwinds to the scheduler, and the thread goes on in the recompiled code
// at that call's return. The relocated jals into the boot ELF read their
// targets from the code (sotc_decomp::jalTarget). GetLayoutClass's plain
// cases, GetCharacterClass, GetGeoDispClass, GetLayoutDispObjGroup and the
// sort lists' ClearSortList / GetFirstSortObj / GetNextSortObj inline.
//
// The recompiled code's operations in its order (FPU_* / PS2_V* macros),
// v0 and f0 as the original leaves them (the callers read v0 of the clip
// callback). An object or pointer outside main RAM goes to the recompiled
// code: DispDispObjGroupModel from its loop head (0x10f5348 / 0x10f5710)
// with the registers it has there, the callbacks from their entry before
// anything is written.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_prefetch
{
    void aheadClip(const uint8_t *rdram, uint32_t object);
    void aheadDisp(const uint8_t *rdram, uint32_t object);
}

namespace decomp_walk
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kWalk = 0x010F52C8u, kClip = 0x010F4890u, kDisp = 0x010F5920u, kSolidDisp = 0x010F3A08u, kClipSolid = 0x01098840u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kUpdateSimple = 0x010EED58u, kAddSortObj = 0x0110DC20u;
    constexpr uint32_t kGraphFsEffectReset = 0x0108FCC8u, kSetEnviroment = 0x010998B8u, kLightPass = 0x010F4FB8u;
    constexpr uint32_t kGetCameraContext = 0x0108F518u, kClipSkin = 0x01098450u, kClipPoint = 0x01099C50u, kModelLod = 0x01097F88u;
    constexpr uint32_t kSkinPalette = 0x010F3CF0u, kStrips = 0x010F3EB8u, kClothMesh = 0x01137F78u, kModelDisp = 0x01097118u, kShadowDisp = 0x0109E0E8u;
    constexpr uint32_t kClassTable = 0x01194070u;
    // The display's globals (lui 0x119 - ...).
    constexpr uint32_t kGroupBase = 0x0118CB20u, kWalkOn = 0x0118CB00u, kSortList = 0x0118CB08u, kSortList2 = 0x0118CB0Cu;
    constexpr uint32_t kStripsOff = 0x0118CAF8u, kClothOn = 0x0118CAF4u, kLodOn = 0x0118BFD4u, kLodModes = 0x01191FE0u;

    struct Ram
    {
        uint8_t *rdram;
        static bool ok(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - a; }
        uint8_t *p(uint32_t a) const { return rdram + (a & PS2_RAM_MASK); }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, p(a), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        int8_t s8(uint32_t a) const { return static_cast<int8_t>(*p(a)); }
        uint8_t u8(uint32_t a) const { return *p(a); }
        int16_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, p(a), 2);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, p(a), 8);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, p(a), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, p(a & ~0xFu), 16);
            return v;
        }
        __m128i qi(uint32_t a) const
        {
            __m128i v;
            std::memcpy(&v, p(a & ~0xFu), 16);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(p(a), &v, 4); }
        void put8(uint32_t a, uint8_t v) const { *p(a) = v; }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(p(a), &v, 8); }
        void putf(uint32_t a, float v) const { std::memcpy(p(a), &v, 4); }
        void putq(uint32_t a, __m128 v) const { std::memcpy(p(a & ~0xFu), &v, 16); }
        void putqi(uint32_t a, __m128i v) const { std::memcpy(p(a & ~0xFu), &v, 16); }
    };

    // VU0 helpers, as the generated code spells the macro instructions.
    const __m128 kAll = _mm_castsi128_ps(_mm_set1_epi32(-1)), kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)),
                 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), kY = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)),
                 kZ = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)), kW = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
    template <int L>
    inline __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    inline __m128 sel(__m128 old, __m128 v, __m128 mask) { return _mm_blendv_ps(old, v, mask); }
    inline __m128 mr32(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1)); }
    inline __m128 yzx(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    inline __m128 zxy(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }

    // vmulax / vmadday / vmaddaz / vmaddw.xyzw: c0..c3 weighted by v's x, y, z, w (acc: the third sum).
    inline __m128 apply(const __m128 c[4], __m128 v, __m128 &acc)
    {
        acc = PS2_VMUL(c[0], lane<0>(v));
        acc = PS2_VADD(acc, PS2_VMUL(c[1], lane<1>(v)));
        acc = PS2_VADD(acc, PS2_VMUL(c[2], lane<2>(v)));
        return PS2_VADD(acc, PS2_VMUL(c[3], lane<3>(v)));
    }

    // The quaternion product block (DispDispObjGroupModel's 0x10f5424..0x10f5448):
    // vf13 = vf11 x vf12 (w: the dot product's complement), ACC / vf14..16 left as the original does.
    struct QuatProduct
    {
        __m128 vf13, vf14, vf15, vf16, acc;
    };
    inline void quatProduct(__m128 vf11, __m128 vf12, QuatProduct &r)
    {
        __m128 vf13 = PS2_VMUL(vf11, vf12);
        vf13 = sel(vf13, PS2_VADD(vf13, lane<1>(vf13)), kX);
        vf13 = sel(vf13, PS2_VADD(vf13, lane<2>(vf13)), kX);
        vf13 = sel(vf13, PS2_VSUB(vf13, lane<0>(vf13)), kW);
        r.vf14 = sel(r.vf14, PS2_VMUL(vf12, lane<3>(vf11)), kXyz);
        r.vf15 = sel(r.vf15, PS2_VMUL(vf11, lane<3>(vf12)), kXyz);
        r.acc = sel(r.acc, PS2_VMUL(yzx(vf12), zxy(vf11)), kXyz);
        r.vf16 = sel(r.vf16, PS2_VSUB(r.acc, PS2_VMUL(yzx(vf11), zxy(vf12))), kXyz);
        vf13 = sel(vf13, PS2_VADD(r.vf14, r.vf15), kXyz);
        vf13 = sel(vf13, PS2_VADD(vf13, r.vf16), kXyz);
        r.vf13 = vf13;
    }

    // Runs recompiled code until it returns to ra with sp (a fallback, or a
    // guest call): what sotc_decomp::callGuest does, from any pc.
    void runToReturn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction fn, uint32_t ra, uint32_t sp)
    {
        fn(rdram, ctx, runtime);
        while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
        {
            PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
            if (!next)
                break;
            next(rdram, ctx, runtime);
        }
    }

    // A function's recompiled code from its entry, run to its return (the
    // registers as at the entry): the rare cases of a native called straight
    // from another native.
    void fallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address)
    {
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        ctx->pc = address;
        runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(address), ra, sp);
    }

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        Ram m;
        G(uint8_t *r, R5900Context *c, PS2Runtime *rt) : rdram(r), ctx(c), runtime(rt), m{r} {}

        void a(int reg, uint32_t v) const { SET_GPR_S32(ctx, reg, static_cast<int32_t>(v)); }
        uint32_t v0() const { return GPR_U32(ctx, 2); }
        // A jal / jalr at `site`: the callee from the frame at sp, run to its return.
        void call(uint32_t target, uint32_t site, uint32_t sp) const { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp); }
        // A jal into the boot ELF (relocated: its target read from the code).
        void callRel(uint32_t site, uint32_t sp) const { call(sotc_decomp::jalTarget(rdram, site), site, sp); }
        // A call to one of this file's natives: straight to it when the table
        // holds it (not under a difftest check, SOTC_DECOMP_SKIP, SOTC_AB's old
        // half or a profiler probe), else through the table.
        void callNative(PS2Runtime::RecompiledFunction fn, uint32_t target, uint32_t site, uint32_t sp) const
        {
            if (runtime->guestFunctionAt(target) != fn)
                return call(target, site, sp);
            SET_GPR_U32(ctx, 31, site + 8u);
            SET_GPR_U32(ctx, 29, sp);
            ctx->pc = target;
            fn(rdram, ctx, runtime);
        }
    };

    // GetLayoutClass (decomp_layout.cpp) for its plain cases; a guest call for the others.
    uint32_t layoutClass(const G g, uint32_t object, uint32_t site, uint32_t sp)
    {
        const uint32_t index = static_cast<uint32_t>(g.m.u8(object + 8u)) - 9u;
        if (index >= 0xEu)
            return 0u;
        const uint32_t target = g.m.u32(kClassTable + index * 4u);
        if (target == 0x010F1A58u)
            return object + 0x20u;
        if (target == 0x010F1A98u)
            return object + 0x90u;
        if (target == 0x010F1AA0u)
            return 0u;
        g.a(4, object);
        g.call(kGetLayoutClass, site, sp);
        return g.v0();
    }
    // GetCharacterClass (0x10c0a70): types 9 and 0x16.
    inline uint32_t characterClass(const Ram m, uint32_t object)
    {
        const uint8_t type = m.u8(object + 8u);
        return (type == 9u || type == 0x16u) ? object + 0x90u : 0u;
    }
    // GetGeoDispClass (0x10ed688): type 0x24.
    inline uint32_t geoDispClass(const Ram m, uint32_t object) { return m.u8(object + 8u) == 0x24u ? object + 0x20u : 0u; }

    // The sort lists (decomp_sortlist.cpp's GetNextSortObj / ClearSortList, GetFirstSortObj).
    void clearSortList(const Ram m, uint32_t list)
    {
        if (0 < m.s32(list + 4u))
            for (int32_t i = 0; i < m.s32(list + 4u); ++i)
                m.put32(m.u32(list) + static_cast<uint32_t>(i) * 4u, 0u);
        m.put32(list + 0x10u, 0u);
    }
    uint32_t firstSortObj(const Ram m, uint32_t list)
    {
        const int32_t count = m.s32(list + 4u);
        if (count <= 0)
            return 0u;
        m.put32(list + 0x10u, 0u);
        const uint32_t buckets = m.u32(list);
        uint32_t result = m.u32(buckets);
        for (int32_t index = 1; result == 0u; ++index)
        {
            if (!(index < count))
                return 0u;
            m.put32(list + 0x10u, static_cast<uint32_t>(index));
            result = m.u32(buckets + static_cast<uint32_t>(index) * 4u);
        }
        return result;
    }
    uint32_t nextSortObj(const Ram m, uint32_t list, uint32_t current)
    {
        if (current == 0u)
            return firstSortObj(m, list);
        uint32_t result = m.u32(current + 8u);
        if (result != 0u)
            return result;
        const int32_t count = m.s32(list + 4u);
        const uint32_t buckets = m.u32(list);
        int32_t index = m.s32(list + 0x10u);
        for (;;)
        {
            ++index;
            if (index == count)
                break;
            result = m.u32(buckets + static_cast<uint32_t>(index) * 4u);
            if (result != 0u)
                break;
        }
        m.put32(list + 0x10u, static_cast<uint32_t>(index));
        return result;
    }

    // isysGroupForNext (decomp_group.cpp): the next entry of a group chain
    // not being deleted whose object is not hidden, the iteration counts
    // moved on; false when something is outside main RAM (nothing written).
    constexpr uint32_t kEntries = 0x0021B900u;
    bool groupNext(const Ram m, uint32_t entry, uint32_t &result)
    {
        if (!Ram::ok(entry, 0x10u))
            return false;
        const uint32_t current = m.u32(entry);
        uint32_t index = static_cast<uint16_t>(m.u32(entry + 0xCu) >> 16);
        for (uint32_t steps = 0;; ++steps)
        {
            if (index == 0u)
            {
                result = 0u;
                return true;
            }
            const uint32_t e = kEntries + index * 24u;
            if (!Ram::ok(e, 0x10u) || steps > 0x10000u)
                return false;
            if (m.u8(e + 0xCu) != 0u)
            {
                index = static_cast<uint16_t>(m.u32(e + 0xCu) >> 16);
                continue;
            }
            const uint32_t object = m.u32(e);
            if (!Ram::ok(object, 0xCu) || !Ram::ok(current, 0xAu))
                return false;
            if ((m.u8(object + 0xAu) & 1u) != 0u)
            {
                index = static_cast<uint16_t>(m.u32(e + 0xCu) >> 16);
                continue;
            }
            m.put8(current + 9u, static_cast<uint8_t>(m.u8(current + 9u) - 1u));
            m.put8(object + 9u, static_cast<uint8_t>(m.u8(object + 9u) + 1u));
            result = e;
            return true;
        }
    }

    void clipLayoutObjModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void dispLayoutObjModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void solidDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void modelClipSolid(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void strips(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void glVertexKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    inline float bitsf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    // ---- clipByMinMaxCube (0x1087300): a box (a0 min, a1 max) through a
    // matrix (a2) against the frustum, VU0 CLIP of its eight corners: the
    // AND of their flags (non-zero: every corner out past one plane). The
    // original tests the AND after each corner's CLIP and stops at 0; the
    // corners and the registers are as it leaves them when it runs to the
    // end (when it stops early, VU0 holds more corners than it would: VU0 is
    // scratch to its callers).
    uint32_t clipByMinMaxCube(R5900Context *ctx, const Ram m, uint32_t lo, uint32_t hi, uint32_t mat)
    {
        __m128 *vf = ctx->vu0_vf;
        const __m128 vf0 = vf[0];
        const __m128 vf1 = sel(m.q(lo), vf0, kW), vf2 = sel(m.q(hi), vf0, kW);
        const __m128 c[4] = {m.q(mat), m.q(mat + 0x10u), m.q(mat + 0x20u), m.q(mat + 0x30u)};
        const __m128 kXy = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, -1)), kXz = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, -1)),
                     kYz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, 0));
        // vf10..vf17 as the original moves them in (min / max per axis).
        const __m128 corner[8] = {
            sel(vf[10], vf1, kXyz),
            sel(sel(vf[11], vf1, kX), vf2, kYz),
            sel(sel(vf[12], vf1, kY), vf2, kXz),
            sel(sel(vf[13], vf1, kZ), vf2, kXy),
            sel(sel(vf[14], vf1, kXy), vf2, kZ),
            sel(sel(vf[15], vf2, kY), vf1, kXz),
            sel(sel(vf[16], vf2, kX), vf1, kYz),
            sel(vf[17], vf2, kXyz),
        };
        const __m128 w = lane<3>(vf0);
        __m128 acc = ctx->vu0_acc, out[2] = {vf[20], vf[21]};
        uint32_t flags = ctx->vu0_clip_flags, v0 = 0x3Fu;
        // Corner k into vf20 (even k) / vf21 (odd), its CLIP, the AND.
        for (int k = 0; k < 8; ++k)
        {
            acc = PS2_VMUL(c[0], lane<0>(corner[k]));
            acc = PS2_VADD(acc, PS2_VMUL(c[1], lane<1>(corner[k])));
            acc = PS2_VADD(acc, PS2_VMUL(c[2], lane<2>(corner[k])));
            const __m128 p = PS2_VADD(acc, PS2_VMUL(c[3], w));
            out[k & 1] = p;
            flags = ((flags << 6) | ps2VuClipFlags(p, p)) & 0xFFFFFFu;
            v0 &= flags;
            if (v0 == 0u)
                break;
        }
        vf[1] = vf1;
        vf[2] = vf2;
        for (int i = 0; i < 4; ++i)
            vf[4 + i] = c[i];
        for (int i = 0; i < 8; ++i)
            vf[10 + i] = corner[i];
        vf[20] = out[0];
        vf[21] = out[1];
        ctx->vu0_acc = acc;
        ctx->vu0_clip_flags = flags;
        return v0;
    }

    // modelClipSolid's level by distance (0x1098a14..0x1098b48): the LOD
    // record's thresholds of levels 1..3 (+0x18 / +0x1c / +0x20) against
    // the distance, then with the hysteresis +0x24 around the current level:
    // a coarser level only past its threshold + h, a finer one only below
    // the threshold - h.
    float lodByDistance(const Ram m, uint32_t record, float d, int32_t current)
    {
        const float t1 = m.f32(record + 0x18u), t2 = m.f32(record + 0x1Cu), t3 = m.f32(record + 0x20u), h = m.f32(record + 0x24u);
        float f12;
        if (FPU_C_OLT_S(t3, d))
            f12 = 3.0f;
        else if (FPU_C_OLT_S(t2, d))
            f12 = 2.0f;
        else if (FPU_C_OLT_S(t1, d))
            f12 = 1.0f;
        else
            f12 = 0.0f;
        if (!FPU_C_OLT_S(0.0f, h) || !FPU_C_OLE_S(0.0f, f12))
            return f12;
        const float now = FPU_CVT_S_W(current);
        if (FPU_C_OLT_S(now, f12))
        {
            if (FPU_C_OLT_S(FPU_ADD_S(t3, h), d))
                return 3.0f;
            if (FPU_C_OLT_S(FPU_ADD_S(t2, h), d))
                return 2.0f;
            if (FPU_C_OLT_S(FPU_ADD_S(t1, h), d))
                return 1.0f;
            return f12;
        }
        if (!FPU_C_OLT_S(f12, now))
            return f12;
        if (FPU_C_OLT_S(d, FPU_SUB_S(t1, h)))
            return 0.0f;
        if (FPU_C_OLT_S(d, FPU_SUB_S(t2, h)))
            return 1.0f;
        if (FPU_C_OLT_S(d, FPU_SUB_S(t3, h)))
            return 2.0f;
        return f12;
    }

    // ---- modelClipSolid (0x1098840; a0 the instance, a1 its matrix): the LOD
    // level to draw (f0), -2 culled, -1 none.
    void modelClipSolid(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xB0u;
        const uint32_t s2 = GPR_U32(ctx, 4), s3 = GPR_U32(ctx, 5);
        if (!Ram::ok(sp, 0xB0u) || !Ram::ok(s2, 0x260u) || !Ram::ok(s3, 0x40u) || !Ram::ok(m.u32(s2), 0x24u))
            return fallback(rdram, ctx, runtime, kClipSolid);
        const uint64_t in16 = GPR_U64(ctx, 16), in17 = GPR_U64(ctx, 17), in18 = GPR_U64(ctx, 18), in19 = GPR_U64(ctx, 19), in20 = GPR_U64(ctx, 20);
        m.put64(sp + 0x80u, in18);
        m.put64(sp + 0x78u, in17);
        m.put64(sp + 0x88u, in19);
        m.put64(sp + 0x90u, in20);
        m.put64(sp + 0x70u, in16);
        m.put64(sp + 0x98u, GPR_U64(ctx, 31));
        m.putf(sp + 0xA8u, ctx->f[21]);
        m.putf(sp + 0xA0u, ctx->f[20]);
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 4));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 5));
        auto finish = [&](float f0, uint64_t v0)
        {
            ctx->f[0] = f0;
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 16, in16);
            SET_GPR_U64(ctx, 17, in17);
            SET_GPR_U64(ctx, 18, in18);
            SET_GPR_U64(ctx, 19, in19);
            SET_GPR_U64(ctx, 20, in20);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        g.call(kGetCameraContext, 0x01098868u, sp);
        const uint32_t s4 = g.v0();
        SET_GPR_S32(ctx, 20, static_cast<int32_t>(s4));
        const uint32_t s1 = m.u32(m.u32(s2) + 0x20u);
        SET_GPR_S32(ctx, 17, static_cast<int32_t>(s1));
        if (!Ram::ok(s1, 0x60u) || !Ram::ok(s4, 0x5E0u) || !Ram::ok(m.u32(s1 + 8u), 0x30u))
        {
            // Back to the recompiled code after its first call (0x1098870),
            // the frame and registers as it has them there.
            SET_GPR_U32(ctx, 29, sp);
            SET_GPR_U32(ctx, 31, 0x01098870u);
            ctx->pc = 0x01098870u;
            return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kClipSolid), ra0, sp0);
        }
        // bit 24 of the header's +0x18: never drawn.
        if (static_cast<int32_t>(static_cast<int64_t>(m.u64(s1 + 0x18u) << 8) >> 32) & 1)
            return finish(-2.0f, 0x01190000ull);
        float f0, f1;
        const __m128 vf0 = ctx->vu0_vf[0];
        // vsub / vmul.xyz / the sum into w / vsqrt: |a - b| in Q, the registers as the original leaves them.
        auto distance = [&](__m128 vf5, __m128 vf6) -> float
        {
            __m128 vf7 = PS2_VSUB(vf5, vf6);
            vf7 = sel(vf7, PS2_VMUL(vf7, vf7), kXyz);
            __m128 acc = sel(ctx->vu0_acc, PS2_VMUL(vf0, lane<0>(vf7)), kW);
            acc = sel(acc, PS2_VADD(acc, PS2_VMUL(vf0, lane<1>(vf7))), kW);
            vf7 = sel(vf7, PS2_VADD(acc, PS2_VMUL(vf0, lane<2>(vf7))), kW);
            ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf7)));
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_acc = acc;
            return ctx->vu0_q;
        };
        auto qbits = [&]() -> uint64_t
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->vu0_q, 4);
            return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(bits)));
        };
        if (m.u32(kLodOn) != 0u)
        {
            const uint32_t lodRecord = m.u32(s1 + 8u);
            f0 = 0.0f;
            f1 = m.f32(lodRecord + 0x30u);
            if (FPU_C_OLT_S(f0, f1))
            {
                f0 = distance(m.q(s3 + 0x30u), m.q(s4 + 0x140u));
                if (FPU_C_OLT_S(f0, f1))
                    return finish(-2.0f, qbits());
            }
        }
        // The bounding sphere (header +0x20, its radius x the LOD scale) through modelClipPoint.
        m.putqi(sp + 0x10u, m.qi(s1 + 0x20u));
        m.putf(sp + 0x1Cu, FPU_MUL_S(m.f32(sp + 0x1Cu), m.f32(s2 + 0x250u)));
        g.a(4, sp);
        g.a(5, s2);
        g.a(6, sp + 0x10u);
        g.a(7, s3);
        g.a(8, s4);
        g.call(kClipPoint, 0x0109892Cu, sp);
        const float f21 = ctx->f[0];
        uint32_t a0 = m.u32(s1 + 8u);
        const uint32_t mode = m.u32(a0 + 0x14u);
        if (mode < 5u)
        {
            const uint32_t target = m.u32(kLodModes + mode * 4u);
            switch (target)
            {
            case 0x01098968u: f1 = m.f32(s2 + 0x234u); break;
            case 0x01098970u: f1 = m.f32(s2 + 0x238u); break;
            case 0x01098978u: f1 = m.f32(s2 + 0x230u); break;
            case 0x01098980u:
            {
                // On the ground: the point and the camera with y = 0.
                g.call(kGetCameraContext, 0x01098980u, sp);
                m.putqi(sp + 0x20u, m.qi(g.v0() + 0x140u));
                m.put32(sp + 4u, 0u);
                m.put32(sp + 0x24u, 0u);
                f1 = distance(m.q(sp), m.q(sp + 0x20u));
                a0 = m.u32(s1 + 8u);
                break;
            }
            case 0x010989D0u:
                g.call(kGetCameraContext, 0x010989D0u, sp);
                f1 = distance(m.q(sp), m.q(g.v0() + 0x140u));
                a0 = m.u32(s1 + 8u);
                break;
            default:
            {
                // A mode this code does not know: the recompiled code from
                // modelClipPoint's return (0x1098934), f0 its result.
                ctx->f[0] = f21;
                SET_GPR_U32(ctx, 29, sp);
                ctx->pc = 0x01098934u;
                return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kClipSolid), ra0, sp0);
            }
            }
        }
        else
            f1 = 0.0f;
        // The level by distance, then against the model's count (+0x1c):
        // past it the last level, or none when the record says so (+0x28);
        // 01097F88 when the model picks its own level (+0xc).
        float f12 = lodByDistance(m, a0, f1, static_cast<int32_t>(static_cast<int64_t>(m.u64(s2 + 0x250u) << 21) >> 61));
        uint64_t v0;
        float f20 = -1.0f;
        bool none = false, past = false;
        const int32_t level = FPU_CVT_W_S(f12);
        const int32_t count = m.s32(s1 + 0x1Cu);
        if (static_cast<uint64_t>(static_cast<int64_t>(level)) < static_cast<uint64_t>(static_cast<int64_t>(count)))
            v0 = static_cast<uint64_t>(static_cast<int64_t>(m.s32(s1 + 0xCu)));
        else
        {
            const int32_t last = static_cast<int32_t>(static_cast<uint32_t>(count) - 1u);
            v0 = static_cast<uint64_t>(static_cast<int64_t>(last));
            past = true;
            if (m.s32(a0 + 0x28u) != 0)
                none = true;
            else
            {
                if (last < 0)
                {
                    const uint32_t half = static_cast<uint32_t>(last) >> 1;
                    f12 = FPU_CVT_S_W(static_cast<int32_t>((static_cast<uint32_t>(last) & 1u) | half));
                    f12 = FPU_ADD_S(f12, f12);
                }
                else
                    f12 = FPU_CVT_S_W(last);
                v0 = static_cast<uint64_t>(static_cast<int64_t>(m.s32(s1 + 0xCu)));
            }
        }
        if (!none)
        {
            if (v0 != 0u)
            {
                g.a(4, s1);
                g.a(5, s2);
                g.a(6, s3);
                ctx->f[12] = f12;
                if (past)
                    ctx->f[20] = -1.0f; // (else the caller's, as the original has it there)
                ctx->f[21] = f21;
                g.call(kModelLod, 0x01098BB4u, sp);
                ctx->f[20] = m.f32(sp + 0xA0u);
                ctx->f[21] = m.f32(sp + 0xA8u);
                f12 = ctx->f[0];
                v0 = GPR_U64(ctx, 2);
            }
            f20 = f12;
        }
        if (FPU_C_OLT_S(f20, 0.0f))
            return finish(-1.0f, v0);
        if (!FPU_C_OLE_S(0.0f, f21))
            return finish(f20, v0);
        v0 = static_cast<uint64_t>(static_cast<int64_t>(-0x30));
        if (s1 == 0xFFFFFFD0u)
            return finish(f20, v0);
        // The level's root box (its part table) through camera (+0x5a0) x
        // model, the product into the frame at +0x30.
        const int32_t drawn = FPU_CVT_W_S(f20);
        const uint32_t table = m.u32(static_cast<uint32_t>(drawn) * 4u + s1 + 0x30u);
        const __m128 cam[4] = {m.q(s4 + 0x5A0u), m.q(s4 + 0x5B0u), m.q(s4 + 0x5C0u), m.q(s4 + 0x5D0u)};
        __m128 acc = ctx->vu0_acc, out[4];
        for (int i = 0; i < 4; ++i)
        {
            out[i] = apply(cam, m.q(s3 + static_cast<uint32_t>(i) * 0x10u), acc);
            m.putq(sp + 0x30u + static_cast<uint32_t>(i) * 0x10u, out[i]);
        }
        ctx->vu0_acc = acc;
        for (int i = 0; i < 4; ++i)
        {
            ctx->vu0_vf[14 + i] = cam[i];
            ctx->vu0_vf[24 + i] = out[i];
        }
        if (!Ram::ok(table, 0x20u))
        {
            g.a(4, table);
            g.a(5, table + 0x10u);
            g.a(6, sp + 0x30u);
            g.call(0x01087300u, 0x01098C7Cu, sp);
            v0 = GPR_U64(ctx, 2);
        }
        else
            v0 = clipByMinMaxCube(ctx, m, table, table + 0x10u, sp + 0x30u);
        finish(v0 != 0u ? -2.0f : f20, v0);
    }

    // ---- ClipLayoutObjModel (0x10f4890; a0 the object) ---------------------
    void clipLayoutObjModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x20u;
        const uint32_t object = GPR_U32(ctx, 4);
        if (!Ram::ok(sp, 0x20u) || !Ram::ok(object, 0x10u))
            return fallback(rdram, ctx, runtime, kClip);
        decomp_prefetch::aheadClip(rdram, object);
        const uint64_t in16 = GPR_U64(ctx, 16), in17 = GPR_U64(ctx, 17);
        m.put64(sp, in16);
        m.put64(sp + 8u, in17);
        m.put64(sp + 0x10u, GPR_U64(ctx, 31));
        m.putf(sp + 0x18u, ctx->f[20]);
        const uint32_t s0 = layoutClass(g, object, 0x010F48A0u, sp);
        auto finish = [&](uint32_t v0)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            SET_GPR_U64(ctx, 16, in16);
            SET_GPR_U64(ctx, 17, in17);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        if (!Ram::ok(s0, 0x170u) || !Ram::ok(m.u32(s0 + 0x140u), 0x40u))
        {
            // From after GetLayoutClass (0x10f48a8), s0 its result.
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(s0));
            SET_GPR_U32(ctx, 29, sp);
            SET_GPR_U32(ctx, 31, 0x010F48A8u);
            ctx->pc = 0x010F48A8u;
            return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kClip), ra0, sp0);
        }
        const uint32_t s1 = m.u32(s0 + 0x140u);
        m.putf(s1 + 0x30u, -1.0f);
        ctx->f[0] = -1.0f;
        uint32_t geo = m.u32(s0 + 0x144u);
        if (geo == 0u)
            return finish(0u);
        if (!Ram::ok(geo, 0x260u) || !Ram::ok(m.u32(geo), 0x24u) || !Ram::ok(m.u32(m.u32(geo) + 0x20u), 0x20u))
        {
            // The rest by the recompiled code, from 0x10f48a8 again (nothing written it would redo differently).
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(s0));
            SET_GPR_U32(ctx, 29, sp);
            SET_GPR_U32(ctx, 31, 0x010F48A8u);
            ctx->pc = 0x010F48A8u;
            return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kClip), ra0, sp0);
        }
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
        SET_GPR_S32(ctx, 17, static_cast<int32_t>(s1));
        float f0 = m.f32(geo + 0x244u);
        m.putf(s1 + 0x34u, f0);
        ctx->f[0] = f0;
        if (!FPU_C_OLT_S(f0, 1.0f))
            return finish(geo);
        const uint32_t header = m.u32(m.u32(geo) + 0x20u);
        uint32_t a1;
        if (m.u32(header + 0x18u) & 1u)
        {
            m.putf(geo + 0x250u, m.f32(s0 + 0x9Cu));
            g.a(4, m.u32(s0 + 0x144u));
            g.a(5, m.u32(s0 + 0x160u));
            ctx->f[20] = 1.0f;
            g.call(kClipSkin, 0x010F4908u, sp);
            ctx->f[20] = m.f32(sp + 0x18u);
            f0 = ctx->f[0];
            a1 = m.u32(s0 + 0x144u);
        }
        else
        {
            float f1 = 1.0f;
            const uint32_t lodRecord = m.u32(header + 8u);
            if (lodRecord != 0u && m.u32(lodRecord + 0x3Cu) != 0u)
            {
                ctx->f[20] = 1.0f;
                g.call(kGetCameraContext, 0x010F492Cu, sp);
                ctx->f[20] = m.f32(sp + 0x18u);
                const uint32_t camera = g.v0();
                geo = m.u32(s0 + 0x144u);
                if (camera != 0u)
                {
                    f0 = ps2FpuDivide(ctx->fcr31, m.f32(camera + 0xCu), 500.0f);
                    f1 = ps2FpuDivide(ctx->fcr31, 1.0f, f0);
                }
            }
            f0 = FPU_MUL_S(m.f32(s0 + 0x9Cu), f1);
            m.putf(geo + 0x250u, f0);
            g.a(4, m.u32(s0 + 0x144u));
            g.a(5, m.u32(s0 + 0x15Cu));
            ctx->f[0] = f0;
            ctx->f[20] = 1.0f;
            g.callNative(modelClipSolid, kClipSolid, 0x010F4980u, sp);
            ctx->f[20] = m.f32(sp + 0x18u);
            f0 = ctx->f[0];
            a1 = m.u32(s0 + 0x144u);
        }
        // The level into bits 40-42 of +0x250; the geometry class's keys.
        {
            const int32_t level = FPU_CVT_W_S(f0);
            const uint64_t word = (m.u64(a1 + 0x250u) & 0xFFFFF8FFFFFFFFFFull) | (static_cast<uint64_t>(static_cast<uint32_t>(level) & 7u) << 40);
            m.put64(a1 + 0x250u, word);
        }
        {
            const uint32_t g144 = m.u32(s0 + 0x144u);
            const int32_t level = static_cast<int32_t>(static_cast<int64_t>(m.u64(g144 + 0x250u) << 21) >> 61);
            m.putf(s1 + 0x30u, FPU_CVT_S_W(level));
        }
        m.putf(s1 + 0x20u, m.f32(m.u32(s0 + 0x144u) + 0x230u));
        m.putf(s1 + 0x28u, m.f32(m.u32(s0 + 0x144u) + 0x238u));
        const uint32_t last = m.u32(s0 + 0x144u);
        f0 = m.f32(last + 0x234u);
        m.putf(s1 + 0x24u, f0);
        ctx->f[0] = f0;
        finish(last);
    }

    // ---- 010F3A08 (a0 the camera object, a1 the layout class, a2 its character class)
    void solidDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x60u;
        const uint32_t s1 = GPR_U32(ctx, 5);
        if (!Ram::ok(sp, 0x60u) || !Ram::ok(s1, 0x1B8u) || !Ram::ok(m.u32(s1 + 0x144u), 0x290u))
            return fallback(rdram, ctx, runtime, kSolidDisp);
        const uint64_t in16 = GPR_U64(ctx, 16), in17 = GPR_U64(ctx, 17), in18 = GPR_U64(ctx, 18);
        m.put64(sp + 0x48u, in17);
        m.put64(sp + 0x40u, in16);
        m.put64(sp + 0x50u, in18);
        m.put64(sp + 0x58u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5));
        const uint32_t s0 = m.u32(s1 + 0x194u);
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
        __m128 acc = ctx->vu0_acc;
        bool vuTouched = false;
        __m128 lastC[4], lastOut[4];
        // A part's matrix block: word(word(geo + 0xc) + 0x104) + ((i * 4 + (i + 1) / 64) << 4).
        auto block = [&](int32_t i) -> uint32_t
        {
            const uint32_t geo = m.u32(s1 + 0x144u);
            const uint32_t base = m.u32(m.u32(geo + 0xCu) + 0x104u);
            const int32_t index = (i << 2) + ((i + 1) >> 6);
            return static_cast<uint32_t>(index << 4) + base;
        };
        if (s0 == 0u)
        {
            const int32_t s2 = m.s16(s1 + 0x1B4u);
            SET_GPR_S32(ctx, 18, s2);
            g.call(kGetCameraContext, 0x010F3A2Cu, sp);
            const uint32_t t0 = g.v0() + 0x1A0u;
            uint32_t a3 = m.u32(s1 + 0x15Cu);
            if (0 < s2)
            {
                for (int32_t i = 0; i < s2; ++i, a3 += 0x40u)
                {
                    const uint32_t dst = block(i);
                    // (the camera matrix read again each part, as the original does)
                    const __m128 cc[4] = {m.q(t0), m.q(t0 + 0x10u), m.q(t0 + 0x20u), m.q(t0 + 0x30u)};
                    const __m128 src[4] = {m.q(a3), m.q(a3 + 0x10u), m.q(a3 + 0x20u), m.q(a3 + 0x30u)};
                    for (int k = 0; k < 4; ++k)
                        lastOut[k] = apply(cc, src[k], acc);
                    for (int k = 0; k < 4; ++k)
                    {
                        m.putq(dst + static_cast<uint32_t>(k) * 0x10u, lastOut[k]);
                        lastC[k] = cc[k];
                    }
                    vuTouched = true;
                }
            }
            // The lens factor: the camera's +0xc / 500, when the LOD record asks (+0x3c).
            const uint32_t header = m.u32(m.u32(m.u32(s1 + 0x144u)) + 0x20u);
            float f12 = 1.0f;
            const uint32_t lodRecord = m.u32(header + 8u);
            if (lodRecord != 0u && m.u32(lodRecord + 0x3Cu) != 0u)
            {
                if (vuTouched)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        ctx->vu0_vf[14 + k] = lastC[k];
                        ctx->vu0_vf[24 + k] = lastOut[k];
                    }
                    ctx->vu0_acc = acc;
                    vuTouched = false;
                }
                g.call(kGetCameraContext, 0x010F3B10u, sp);
                const uint32_t camera = g.v0();
                if (camera != 0u)
                {
                    float f0 = m.f32(camera + 0xCu);
                    f0 = ps2FpuDivide(ctx->fcr31, f0, 500.0f);
                    ctx->f[0] = f0;
                    f12 = ps2FpuDivide(ctx->fcr31, 1.0f, f0);
                }
            }
            ctx->f[0] = 1.0f;
            if (!FPU_C_EQ_S(f12, 1.0f))
            {
                if (vuTouched)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        ctx->vu0_vf[14 + k] = lastC[k];
                        ctx->vu0_vf[24 + k] = lastOut[k];
                    }
                    ctx->vu0_acc = acc;
                    vuTouched = false;
                }
                // iosGetScaleMatrix(sp, f12, f12, f12), then each part's block x that.
                const int32_t count = m.s16(s1 + 0x1B4u);
                SET_GPR_S32(ctx, 16, count);
                g.a(4, sp);
                ctx->f[12] = f12;
                ctx->f[13] = f12;
                ctx->f[14] = f12;
                g.callRel(0x010F3B7Cu, sp);
                acc = ctx->vu0_acc;
                for (int32_t i = 0; i < count; ++i)
                {
                    const uint32_t dst = block(i);
                    const __m128 c[4] = {m.q(dst), m.q(dst + 0x10u), m.q(dst + 0x20u), m.q(dst + 0x30u)};
                    const __m128 sc[4] = {m.q(sp), m.q(sp + 0x10u), m.q(sp + 0x20u), m.q(sp + 0x30u)};
                    for (int k = 0; k < 4; ++k)
                        lastOut[k] = apply(c, sc[k], acc);
                    for (int k = 0; k < 4; ++k)
                    {
                        m.putq(dst + static_cast<uint32_t>(k) * 0x10u, lastOut[k]);
                        lastC[k] = c[k];
                    }
                    vuTouched = true;
                }
            }
        }
        else
        {
            // A parent's matrix block (+0x194: its object) taken as this one's.
            const uint32_t parent = layoutClass(g, s0, 0x010F3C38u, sp);
            m.put32(m.u32(s1 + 0x144u) + 0xCu, m.u32(m.u32(parent + 0x144u) + 0xCu));
        }
        if (vuTouched)
        {
            for (int k = 0; k < 4; ++k)
            {
                ctx->vu0_vf[14 + k] = lastC[k];
                ctx->vu0_vf[24 + k] = lastOut[k];
            }
            ctx->vu0_acc = acc;
        }
        auto finish = [&](uint64_t v0)
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 16, in16);
            SET_GPR_U64(ctx, 17, in17);
            SET_GPR_U64(ctx, 18, in18);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        uint32_t geo = m.u32(s1 + 0x144u);
        uint64_t v0 = m.u64(geo + 0x260u) & 0x20u;
        if (v0 == 0u)
        {
            g.a(4, geo);
            g.a(5, m.u32(s1 + 0x15Cu));
            g.call(kModelDisp, 0x010F3C64u, sp);
        }
        v0 = static_cast<uint64_t>(static_cast<int64_t>(m.s32(s1 + 0x194u)));
        if (v0 != 0u)
            return finish(v0);
        geo = m.u32(s1 + 0x144u);
        v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(geo)));
        const uint32_t shadow = m.u32(geo + 0x280u);
        if (shadow == 0u)
            return finish(v0);
        g.a(4, shadow);
        g.callRel(0x010F3C88u, sp);
        geo = m.u32(s1 + 0x144u);
        v0 = m.u64(geo + 0x260u) & 0x40u;
        if (v0 != 0u)
            return finish(v0);
        // shadowDisp(geo's +0xc, shadow +0x20, +0x134, the matrix, the part count, 0, 0, 0.0)
        const uint32_t record = m.u32(geo + 0x280u);
        const uint32_t a1 = m.u32(record + 0x20u);
        g.a(6, m.u32(s1 + 0x134u));
        g.a(5, a1);
        g.a(4, m.u32(geo + 0xCu));
        g.a(7, a1 == 0u ? m.u32(s1 + 0x15Cu) : m.u32(s1 + 0x160u));
        SET_GPR_S32(ctx, 8, m.s16(s1 + 0x1B4u));
        SET_GPR_U64(ctx, 9, 0u);
        ctx->f[12] = 0.0f;
        SET_GPR_U64(ctx, 10, 0u);
        g.call(kShadowDisp, 0x010F3CD0u, sp);
        finish(GPR_U64(ctx, 2));
    }


    // ---- The heat haze strips (010F3EB8, falling into 010F3FB0; a0 the
    // camera's layout class, a1 the character class, a2 the layout class):
    // for each of the character's haze segments (+0x1140's record, 0x34
    // bytes each: two bone points with radii, a width) the two points
    // through their bones (the inverse of one table's bone, then the
    // other's and the view matrix), skipped when either radius over the
    // distance is below 0.001 or either point is off the 256 x 256 screen
    // copy; then two triangle strips of four vertices with the gl* immediate
    // API in layer 0x17 over 0x10f37c8's copy of the screen (the native
    // renderer samples it: glEnd's hook, by return address). glColor4f,
    // glTexCoord2f, glVertex3fv's own part (0108D358: the vertex colour),
    // getDrawScreenWidth / Height and iosgluGetViewCoordPosition inline;
    // glBegin, glEnd, the vertex's 0108D730 (clipping and the GIF packet),
    // the enables and the layer as guest calls.
    constexpr uint32_t kGl = 0x01186390u, kViewStack = 0x01214EE0u, kViewIndex = 0x012156E0u;
    constexpr uint32_t kGlBegin = 0x0108E530u, kGlEnd = 0x0108D060u, kGlVertex = 0x0108D730u, kGlEnable = 0x0108E358u, kGlDisable = 0x0108E448u;
    constexpr uint32_t kGlLoadMatrixf = 0x0108ED78u, kScreenCopy = 0x010F37C8u, kGlZOffset = 0x0108F3B0u;

    // glColor4f (0x108e8b8): r, g, b x 255 and a x 128, each clamped to 0..255.
    void glColor4f(R5900Context *ctx, const Ram m, float r, float gr, float b, float a)
    {
        const float f2 = 255.0f;
        m.putf(kGl + 0x90u, FPU_MUL_S(r, 255.0f));
        m.putf(kGl + 0x9Cu, FPU_MUL_S(a, 128.0f));
        m.putf(kGl + 0x94u, FPU_MUL_S(gr, 255.0f));
        m.putf(kGl + 0x98u, FPU_MUL_S(b, 255.0f));
        float f0 = 0.0f;
        for (uint32_t i = 0; i < 4u; ++i)
        {
            const uint32_t at = kGl + 0x90u + i * 4u;
            f0 = m.f32(at);
            if (FPU_C_OLT_S(f2, f0))
            {
                m.putf(at, f2);
                f0 = f2;
            }
            if (FPU_C_OLT_S(f0, 0.0f))
                m.put32(at, 0u);
        }
        ctx->f[0] = f0;
    }
    // glTexCoord2f (0x108e9c8): (s, t, 1, 0).
    void glTexCoord2f(R5900Context *ctx, const Ram m, float s, float t)
    {
        m.putf(kGl + 0xA0u, s);
        m.putf(kGl + 0xA4u, t);
        m.putf(kGl + 0xA8u, 1.0f);
        m.put32(kGl + 0xACu, 0u);
        ctx->f[0] = 1.0f;
    }
    // glVertex3fv (0x108ebc0): 0108D358(1) (the vertex's colour, lit when
    // bits 0, 1 and 3 of the state say so; its texture coordinates), the
    // position into the vertex ring, then 0108D730 (a guest call: the tail
    // jump, from our frame).
    void glVertex3fv(const G g, uint32_t vertex, uint32_t site, uint32_t sp)
    {
        const Ram m = g.m;
        R5900Context *ctx = g.ctx;
        uint64_t state = (m.u64(kGl + 0x1B0u) & ~9ull) | 8u;
        m.put64(kGl + 0x1B0u, state);
        const uint32_t index = m.u32(kGl + 0x41F0u);
        const uint32_t dst = kGl + 0x220u + (index << 6);
        if ((state & 0xBu) == 0xBu)
        {
            __m128 *vf = ctx->vu0_vf;
            const __m128 vf0 = vf[0];
            const __m128 vf24 = m.q(kGl + 0x90u), vf25 = m.q(kGl + 0x80u);
            const __m128 c1[4] = {m.q(kGl + 0x130u), m.q(kGl + 0x140u), m.q(kGl + 0x150u), m.q(kGl + 0x160u)};
            ctx->f[1] = 255.0f;
            // mfc1 a3, f1 (255.0), qmtc2 a3, vf30: the whole 128-bit register.
            SET_GPR_U64(ctx, 7, 0x437F0000ull);
            const __m128 vf30 = _mm_castsi128_ps(GPR_VEC(ctx, 7));
            __m128 acc = PS2_VMUL(c1[0], lane<0>(vf25));
            acc = PS2_VADD(acc, PS2_VMUL(c1[1], lane<1>(vf25)));
            acc = PS2_VADD(acc, PS2_VMUL(c1[2], lane<2>(vf25)));
            __m128 vf23 = PS2_VADD(acc, PS2_VMUL(c1[3], lane<3>(vf0)));
            const __m128 c2[4] = {m.q(kGl + 0x170u), m.q(kGl + 0x180u), m.q(kGl + 0x190u), m.q(kGl + 0x1A0u)};
            vf23 = _mm_max_ps(vf23, lane<0>(vf0));
            acc = PS2_VMUL(c2[0], lane<0>(vf23));
            acc = PS2_VADD(acc, PS2_VMUL(c2[1], lane<1>(vf23)));
            acc = PS2_VADD(acc, PS2_VMUL(c2[2], lane<2>(vf23)));
            vf23 = PS2_VADD(acc, PS2_VMUL(c2[3], lane<3>(vf0)));
            __m128 out = sel(vf24, PS2_VMUL(vf24, vf23), kXyz);
            out = _mm_min_ps(out, lane<0>(vf30));
            m.putq(dst, out);
            vf[23] = vf23;
            vf[24] = out;
            vf[25] = vf25;
            for (int i = 0; i < 4; ++i)
                vf[26 + i] = c2[i];
            vf[30] = vf30;
            ctx->vu0_acc = acc;
        }
        else
            m.putqi(dst, m.qi(kGl + 0x90u));
        if (m.u32(kGl + 0x18u) != 0u)
        {
            const uint32_t at = kGl + (m.u32(kGl + 0x41F0u) << 6);
            m.putf(at + 0x210u, m.f32(kGl + 0xA0u));
            ctx->f[0] = m.f32(kGl + 0xA4u);
            m.putf(kGl + (m.u32(kGl + 0x41F0u) << 6) + 0x214u, ctx->f[0]);
        }
        m.put64(kGl + 0x1B0u, m.u64(kGl + 0x1B0u) & ~1ull);
        m.putqi(kGl + (m.u32(kGl + 0x41F0u) << 6) + 0x200u, m.qi(vertex));
        g.a(4, m.u32(kGl + 0x41F0u));
        SET_GPR_U32(ctx, 2, kGl + (m.u32(kGl + 0x41F0u) << 6));
        g.callNative(glVertexKick, kGlVertex, site, sp);
    }


    // vftoi0 / vftoi4 as the generated code has them (truncation, positive
    // overflow to 0x7fffffff by the xor).
    inline __m128 ftoi(__m128 v, float scale)
    {
        const __m128 src = _mm_mul_ps(v, _mm_set1_ps(scale));
        const __m128i positiveOverflow =
            _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f))));
        return _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow));
    }

    // ---- 0108D730 (glVertex's kick; a0 the vertex's ring index): the
    // vertex (position +0x200, ST +0x210, colour +0x220 of its 64-byte ring
    // slot at 0x1186390) through the view (the stack at 0x1214ee0) and the
    // camera (+0x660 for the screen, +0x5e0 for the clip test), its CLIP
    // flags, the colour (RGBAQ: Q = 1/w with perspective texturing (+0x5c),
    // else 1), ST / UV (+0x18 texturing, +0x2c 1 for UV through 01BE700)
    // and XYZ2 / XYZ3 (the Z scaled by +0x41f4, fog in the top byte) into
    // the GIF packet at *(0x1186394). By the primitive (+0x10) the flags of
    // this vertex and the last ones (the 24-bit CLIP history) say whether a
    // clipped primitive is being drawn (s2: +0x1b4 counts the vertices to
    // send as XYZ3 after it, and the vertex's own ADC bit at +0x41f8 + index).
    // The vertex ring wraps at 256: glEnd, then glBeginWithDlLayerNo again
    // (guest calls). An untransformed vertex (bit 3 of +0x1b0 clear) is
    // 0108D478's, a guest call.
    void glVertexKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x90u;
        const uint32_t index = GPR_U32(ctx, 4);
        const uint32_t camera = m.u32(kGl);
        if (!Ram::ok(sp, 0x90u) || (camera != 0u && !Ram::ok(camera, 0x6A0u)) || index >= 0x100u)
            return fallback(rdram, ctx, runtime, kGlVertex);
        static const int saved[] = {19, 16, 17, 18, 20, 21, 31};
        static const uint32_t slot[] = {0x68u, 0x50u, 0x58u, 0x60u, 0x70u, 0x78u, 0x80u};
        uint64_t in[7];
        for (int i = 0; i < 7; ++i)
        {
            in[i] = GPR_U64(ctx, saved[i]);
            m.put64(sp + slot[i], in[i]);
        }
        m.put32(sp, index);
        auto finish = [&]()
        {
            for (int i = 0; i < 7; ++i)
                SET_GPR_U64(ctx, saved[i], in[i]);
            SET_GPR_U32(ctx, 29, sp0);
            returnTo(ctx);
        };
        if (camera == 0u)
            return finish();
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 19, 0x01180000u);
        const uint32_t a0 = index << 6;
        if (((m.u64(kGl + 0x1B0u) >> 3) & 1u) == 0u)
        {
            // Untransformed: 0108D478(a0 clip out, a1 position, a2 ST, a3 colour; v0 the frame).
            g.a(4, kGl + 0x1F0u + a0);
            g.a(5, kGl + 0x200u + a0);
            g.a(6, kGl + 0x210u + a0);
            g.a(7, kGl + 0x220u + a0);
            g.a(2, sp);
            g.call(0x0108D478u, 0x0108DE24u, sp);
        }
        else
        {
            const uint32_t t2 = kGl + 0x220u + a0, t1 = kGl + 0x1F0u + a0, t0 = kGl + 0x200u + a0, a3 = kGl + 0x210u + a0;
            __m128 *vf = ctx->vu0_vf;
            const __m128 vf0 = vf[0];
            const __m128 clipM[4] = {m.q(camera + 0x5E0u), m.q(camera + 0x5F0u), m.q(camera + 0x600u), m.q(camera + 0x610u)};
            const bool perspective = m.u32(kGl + 0x5Cu) != 0u;
            const uint32_t view = kViewStack + (m.u32(kViewIndex) << 6);
            const __m128 viewM[4] = {m.q(view), m.q(view + 0x10u), m.q(view + 0x20u), m.q(view + 0x30u)};
            const __m128 screenM[4] = {m.q(camera + 0x660u), m.q(camera + 0x670u), m.q(camera + 0x680u), m.q(camera + 0x690u)};
            const __m128 vf23 = m.q(t0);
            __m128 vf24 = m.q(a3), vf25 = m.q(t2);
            __m128 acc;
            const __m128 vf10 = apply(viewM, vf23, acc);
            __m128 vf27 = apply(screenM, vf10, acc);
            const __m128 vf26 = apply(clipM, vf10, acc);
            ctx->vu0_q = ps2VuDivide(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf27)));
            vf25 = ftoi(vf25, 1.0f);
            const uint32_t flags = ((ctx->vu0_clip_flags << 6) | ps2VuClipFlags(vf26, vf26)) & 0xFFFFFFu;
            ctx->vu0_clip_flags = flags;
            m.putq(sp + 0x30u, vf25);
            const __m128 q = _mm_set1_ps(ctx->vu0_q);
            vf27 = sel(vf27, PS2_VMUL(vf27, q), kXyz);
            vf27 = sel(vf27, PS2_VMUL(vf0, q), kW);
            if (perspective)
                vf24 = sel(vf24, PS2_VMUL(vf24, q), kXyz);
            const uint32_t s0 = flags;
            m.putq(sp + 0x40u, vf24);
            vf27 = sel(vf27, ftoi(vf27, 16.0f), kXyz);
            m.putq(t1, vf26);
            m.putq(sp + 0x20u, vf27);
            vf[10] = vf10;
            for (int i = 0; i < 4; ++i)
            {
                vf[11 + i] = viewM[i];
                vf[15 + i] = clipM[i];
                vf[19 + i] = screenM[i];
            }
            vf[23] = vf23;
            vf[24] = vf24;
            vf[25] = vf25;
            vf[26] = vf26;
            vf[27] = vf27;
            ctx->vu0_acc = acc;
            SET_GPR_U32(ctx, 16, s0);
            SET_GPR_U32(ctx, 17, kGl);
            SET_GPR_U32(ctx, 20, sp);
            uint64_t v0 = 0;
            // A packet pair at *(G + 4): the value, then the register.
            auto emit = [&](uint64_t value, uint64_t reg)
            {
                uint32_t at = m.u32(kGl + 4u);
                m.put64(at, value);
                at += 8u;
                m.put32(kGl + 4u, at);
                m.put64(at, reg);
                m.put32(kGl + 4u, at + 8u);
            };
            auto lw = [&](uint32_t a) -> uint64_t { return static_cast<uint64_t>(static_cast<int64_t>(m.s32(a))); };
            if (m.u32(kGl + 0x18u) != 0u)
            {
                const uint32_t mode = m.u32(kGl + 0x2Cu);
                if (mode == 0u)
                    emit(static_cast<uint64_t>(m.u32(sp + 0x40u)) | (static_cast<uint64_t>(m.u32(sp + 0x44u)) << 32), 2u); // ST
                else if (mode == 1u)
                {
                    // UV: 01BE700(sp + 0x40, the ST slot).
                    g.a(4, sp + 0x40u);
                    g.a(5, a3);
                    g.callRel(0x0108D9B0u, sp);
                    emit(static_cast<uint64_t>(m.u32(sp + 0x40u)) | (static_cast<uint64_t>(m.u32(sp + 0x44u)) << 16), 3u);
                }
            }
            {
                // RGBAQ: r | g << 8 | b << 16 | a << 24 | Q << 32 (Q 1.0 without perspective).
                const uint64_t q32 = perspective ? (lw(sp + 0x2Cu) << 32) : 0x3F80000000000000ull;
                const uint64_t value = (lw(sp + 0x30u) | q32) | ((lw(sp + 0x38u) << 16) | (lw(sp + 0x34u) << 8)) | (lw(sp + 0x3Cu) << 24);
                emit(value, 1u);
            }
            // The primitive's clip state.
            uint32_t s2 = 0u, s5 = 0u;
            const uint32_t prim = m.u32(kGl + 0x10u);
            uint32_t target = prim < 7u ? m.u32(0x01191730u + prim * 4u) : 0x0108DC48u;
            const int32_t count = m.s32(sp);
            auto pairs = [&](const uint32_t *masks, int n) -> bool
            {
                for (int i = 0; i < n; ++i)
                    if ((s0 & masks[i]) == masks[i])
                        return true;
                return false;
            };
            switch (target)
            {
            case 0x0108DAE0u: // points
                if (s0 & 0x3Fu)
                    s2 = 1u;
                break;
            case 0x0108DAF8u: // lines
            {
                static const uint32_t two[] = {0x41u, 0x82u, 0x104u, 0x208u, 0x410u};
                if (count > 0 && (s0 & 0xFFFu) != 0u)
                {
                    if (!pairs(two, 5))
                        s5 = ((s0 & 0x820u) ^ 0x820u) != 0u ? 1u : 0u;
                    s2 = 1u;
                }
                break;
            }
            case 0x0108DB58u: // line strip
                if (count > 0 && (s0 & 0xFFFu) != 0u)
                    s2 = 1u;
                break;
            case 0x0108DB78u: // triangle strip (the first vertex starts a clipped run)
            {
                static const uint32_t two[] = {0x41u, 0x82u, 0x104u, 0x208u, 0x410u};
                if (count == 0 && (s0 & 0x3Fu) != 0u)
                {
                    m.put64(kGl + 0x1B0u, m.u64(kGl + 0x1B0u) | 0x10u);
                    s2 = 1u;
                }
                const bool run = ((m.u64(kGl + 0x1B0u) >> 4) & 1u) != 0u;
                if (count < 2)
                {
                    if (run)
                    {
                        s5 = 1u;
                        s2 = 1u;
                    }
                }
                else if ((s0 & 0xFFFu) == 0u)
                {
                    if (run)
                    {
                        s5 = 1u;
                        s2 = 1u;
                    }
                }
                else
                {
                    if (pairs(two, 5) || (s0 & 0x820u) == 0x820u)
                        s5 = run ? 0u : 1u;
                    else
                        s5 = 1u;
                    s2 = 1u;
                }
                break;
            }
            case 0x0108DC48u: // triangles / fans
            default:
            {
                if (target != 0x0108DC48u)
                {
                    // A table entry this code does not know: the recompiled
                    // code from its jump (0x108dad4), the registers as there.
                    SET_GPR_S32(ctx, 3, static_cast<int32_t>(target));
                    ctx->pc = 0x0108DAD4u;
                    return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kGlVertex), ra0, sp0);
                }
                static const uint32_t three[] = {0x1041u, 0x2082u, 0x4104u, 0x8208u, 0x10410u};
                if (!(count < 2) && (s0 & 0x3FFFFu) != 0u)
                {
                    if (!pairs(three, 5))
                        s5 = ((s0 & 0x20820u) ^ 0x20820u) != 0u ? 1u : 0u;
                    s2 = 1u;
                }
                break;
            }
            }
            // XYZ2, or XYZ3 (no kick) while +0x1b4 counts down after a clipped primitive.
            const int32_t pending = m.s32(kGl + 0x1B4u);
            s2 |= (0 < pending) ? 1u : 0u;
            uint64_t reg = 4u;
            if (s2 != 0u)
            {
                if (pending > 0)
                    m.put32(kGl + 0x1B4u, static_cast<uint32_t>(pending - 1));
                reg = 0xCu;
            }
            const float f1 = m.f32(kGl + 0x41F4u), f2 = 2147483648.0f;
            float f0 = FPU_MUL_S(FPU_CVT_S_W(m.s32(sp + 0x28u) >> 4), f1);
            uint64_t z;
            if (FPU_C_OLE_S(f2, f0))
            {
                f0 = FPU_SUB_S(f0, f2);
                z = static_cast<uint64_t>(static_cast<int64_t>(FPU_CVT_W_S(f0))) | 0xFFFFFFFF80000000ull;
            }
            else
                z = static_cast<uint64_t>(static_cast<int64_t>(FPU_CVT_W_S(f0)));
            {
                int32_t zi = static_cast<int32_t>(z);
                std::memcpy(&ctx->f[1], &zi, 4);
            }
            ctx->f[0] = f0;
            ctx->f[2] = f2;
            const uint64_t a3v = lw(sp + 0x20u) | (static_cast<uint64_t>(m.u32(kGl + 0x60u)) << 56);
            const uint64_t xyz = a3v | ((z << 32) | (lw(sp + 0x24u) << 16));
            emit(xyz, reg);
            m.put8(kGl + 0x41F8u + index, static_cast<uint8_t>(s5));
            SET_GPR_U32(ctx, 18, s2);
            SET_GPR_U32(ctx, 21, s5);
            (void)v0;
        }
        // The ring: past 256 vertices glEnd, then a new glBeginWithDlLayerNo.
        const uint32_t next = m.u32(kGl + 0x41F0u) + 1u;
        m.put32(kGl + 0x41F0u, next);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(next));
        if (static_cast<int32_t>(next) >= 0x100)
        {
            g.call(kGlEnd, 0x0108DE44u, sp);
            const uint32_t prim = m.u32(kGl + 0xCu);
            g.callRel(0x0108DE4Cu, sp); // 01C0198
            g.a(5, g.v0());
            g.a(4, prim);
            g.call(0x0108BB38u, 0x0108DE58u, sp);
        }
        finish();
    }

    // glVertex3fv (0x108ebc0; a0 the position): as above, the kick native.
    void glVertex3fvEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29), vertex = GPR_U32(ctx, 4);
        if (!Ram::ok(sp - 0x10u, 0x10u) || !Ram::ok(vertex, 0x10u))
            return sotc_decomp::recompiled(0x0108EBC0u)(rdram, ctx, runtime);
        g.m.put64(sp - 0x10u, GPR_U64(ctx, 16));
        g.m.put64(sp - 8u, GPR_U64(ctx, 31));
        glVertex3fv(g, vertex, GPR_U32(ctx, 31) - 8u, sp);
    }

    // iosgluGetViewCoordPosition (0x108f678; out, point): the point through
    // the current view (the stack at 0x1214ee0) and the camera's +0x660,
    // divided by w, x / y less 2048.
    void viewCoord(R5900Context *ctx, const Ram m, uint32_t out, uint32_t point)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        const uint32_t view = kViewStack + (m.u32(kViewIndex) << 6), cam = m.u32(kGl) + 0x660u;
        const __m128 a[4] = {m.q(view), m.q(view + 0x10u), m.q(view + 0x20u), m.q(view + 0x30u)};
        const __m128 b[4] = {m.q(cam), m.q(cam + 0x10u), m.q(cam + 0x20u), m.q(cam + 0x30u)};
        const __m128 vf10 = m.q(point);
        __m128 acc = PS2_VMUL(a[0], lane<0>(vf10));
        acc = PS2_VADD(acc, PS2_VMUL(a[1], lane<1>(vf10)));
        acc = PS2_VADD(acc, PS2_VMUL(a[2], lane<2>(vf10)));
        const __m128 vf19 = PS2_VADD(acc, PS2_VMUL(a[3], lane<3>(vf0)));
        __m128 vf20 = apply(b, vf19, acc);
        ctx->vu0_q = ps2VuDivide(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf20)));
        vf20 = sel(vf20, PS2_VMUL(vf20, _mm_set1_ps(ctx->vu0_q)), kXyz);
        vf20 = sel(vf20, PS2_VMUL(vf0, _mm_set1_ps(ctx->vu0_q)), kW);
        m.putq(out, vf20);
        const float x = FPU_SUB_S(m.f32(out), 2048.0f), y = FPU_SUB_S(m.f32(out + 4u), 2048.0f);
        m.putf(out, x);
        m.putf(out + 4u, y);
        __m128 *vf = ctx->vu0_vf;
        vf[10] = vf10;
        for (int i = 0; i < 4; ++i)
        {
            vf[11 + i] = a[i];
            vf[15 + i] = b[i];
        }
        vf[19] = vf19;
        vf[20] = vf20;
        ctx->vu0_acc = acc;
        ctx->f[0] = x;
        ctx->f[1] = y;
        ctx->f[2] = 2048.0f;
    }

    void strips(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x1F0u;
        const uint32_t cameraClass = GPR_U32(ctx, 4), chara = GPR_U32(ctx, 5), lc = GPR_U32(ctx, 6);
        if (!Ram::ok(sp, 0x1F0u) || !Ram::ok(chara, 0x1144u) || !Ram::ok(lc, 0x170u) || !Ram::ok(cameraClass, 0x170u) ||
            !Ram::ok(m.u32(chara + 0x1140u), 0x14u))
            return fallback(rdram, ctx, runtime, kStrips);
        static const int saved[] = {16, 17, 18, 19, 20, 21, 22, 23, 30, 31};
        static const uint32_t slot[] = {0x150u, 0x158u, 0x160u, 0x168u, 0x170u, 0x178u, 0x180u, 0x188u, 0x190u, 0x198u};
        uint64_t in[10];
        for (int i = 0; i < 10; ++i)
        {
            in[i] = GPR_U64(ctx, saved[i]);
            m.put64(sp + slot[i], in[i]);
        }
        float fin[9];
        for (int i = 0; i < 9; ++i)
        {
            fin[i] = ctx->f[20 + i];
            m.putf(sp + 0x1A0u + static_cast<uint32_t>(i) * 8u, fin[i]);
        }
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 6));
        // The segment table: a handle at the character's +0x1140 (+0x10: its
        // offset back to the record in the low 18 bits, the index in bits 18-29).
        const uint32_t handle = m.u32(chara + 0x1140u) + 0x10u;
        const uint32_t word = m.u32(handle), back = word & 0x3FFFFu;
        const uint32_t sentinel = m.u32(sotc_decomp::relocated(rdram, 0x010F3F40u, 0x010F3F44u));
        const uint32_t record = back ? m.u32(handle - back) : 0u;
        const uint32_t count = (record == 0u || record == sentinel) ? 0u : m.u32(record + 0xCu);
        uint32_t s1 = 0u;
        if (record != 0u && record != sentinel)
        {
            if (!Ram::ok(record, 0x14u) || !Ram::ok(m.u32(record + 8u), 0x10u))
                return fallback(rdram, ctx, runtime, kStrips);
            const uint32_t index = (word >> 18) & 0xFFFu;
            s1 = m.u32(record + 0x10u) + static_cast<uint32_t>(static_cast<int32_t>(index) * m.s32(m.u32(record + 8u) + 0xCu));
        }
        const uint32_t fp = m.u32(cameraClass + 0x16Cu);
        const uint32_t s7 = lc + 0x90u;
        auto sreg = [&](int r, uint32_t v) { SET_GPR_S32(ctx, r, static_cast<int32_t>(v)); };
        sreg(16, count);
        sreg(17, s1);
        sreg(23, s7);
        sreg(30, fp);
        auto enable = [&](uint32_t target, uint32_t cap, uint32_t site)
        {
            g.a(4, cap);
            g.call(target, site, sp);
        };
        enable(kGlEnable, 0u, 0x010F3FBCu);
        enable(kGlEnable, 0x14u, 0x010F3FC8u);
        enable(kGlDisable, 2u, 0x010F3FD0u);
        enable(kGlEnable, 3u, 0x010F3FD8u);
        enable(kGlDisable, 8u, 0x010F3FE0u);
        g.a(4, sotc_decomp::relocated(rdram, 0x010F3FE8u, 0x010F3FF0u));
        m.put32(sp + 0x140u, count - 1u);
        g.call(kGlLoadMatrixf, 0x010F3FF4u, sp);
        g.a(4, 0x17u);
        g.callRel(0x010F3FFCu, sp); // iosDlPushLayer (sotc_native's hook)
        g.call(kScreenCopy, 0x010F4004u, sp);
        ctx->f[12] = bitsf(0x3F866666u);
        g.call(kGlZOffset, 0x010F4018u, sp);
        const uint32_t matrices = m.u32(lc + 0x15Cu), bones = m.u32(lc + 0x168u);
        m.put32(sp + 0x144u, matrices);
        m.put32(sp + 0x148u, bones);
        if (count != 0u)
        {
            const float f25 = 1.0f, f27 = bitsf(0x3A83126Eu), f28 = 0.0f, f26 = 0.5f;
            const uint32_t s3 = sp + 0xC0u, s2 = sp + 0xD0u, s4 = sp + 0xA0u, s5 = sp + 0xB0u, s6 = sp + 0xE0u;
            sreg(19, s3);
            sreg(18, s2);
            __m128 *vf = ctx->vu0_vf;
            const __m128 vf0 = vf[0];
            __m128 vf28 = vf[28];
            float f20 = ctx->f[20], f21 = ctx->f[21], f22 = ctx->f[22], f23 = ctx->f[23], f24 = ctx->f[24];
            auto syncFloats = [&]()
            {
                ctx->f[20] = f20;
                ctx->f[21] = f21;
                ctx->f[22] = f22;
                ctx->f[23] = f23;
                ctx->f[24] = f24;
                ctx->f[25] = f25;
                ctx->f[26] = f26;
                ctx->f[27] = f27;
                ctx->f[28] = f28;
            };
            // A point (w 1) x the scale (+0x90), through the inverse of its
            // bone (the bone table +0x168) and the view matrix.
            auto boneInverse = [&](uint32_t bone, uint32_t out)
            {
                __m128 vf24 = m.q(bone + 0x30u);
                const __m128 vf25x = PS2_VSUB(vf0, lane<3>(vf0));
                vf28 = sel(vf28, PS2_VMUL(vf24, lane<0>(vf25x)), kXyz);
                vf28 = sel(vf28, PS2_VSUB(vf28, vf28), kW);
                const __m128i t0 = m.qi(bone), t1 = m.qi(bone + 0x10u), t2 = m.qi(bone + 0x20u), t3 = _mm_castps_si128(vf0);
                const __m128i t4 = PS2_PEXTLW(t1, t0), t5 = PS2_PEXTUW(t1, t0), t6 = PS2_PEXTLW(t3, t2), t7 = PS2_PEXTUW(t3, t2);
                const __m128 r0 = _mm_castsi128_ps(PS2_PCPYLD(t6, t4)), r1 = _mm_castsi128_ps(_mm_unpackhi_epi64(t4, t6)),
                             r2 = _mm_castsi128_ps(PS2_PCPYLD(t7, t5));
                __m128 acc = PS2_VMUL(r0, lane<0>(vf28));
                acc = PS2_VADD(acc, PS2_VMUL(r1, lane<1>(vf28)));
                acc = PS2_VADD(acc, PS2_VMUL(r2, lane<2>(vf28)));
                __m128 vf29 = PS2_VADD(acc, PS2_VMUL(vf0, lane<3>(vf28)));
                vf29 = sel(vf29, PS2_VSUB(vf29, vf29), kW);
                vf29 = sel(vf29, PS2_VADD(vf29, vf0), kW);
                m.putq(out, r0);
                m.putq(out + 0x10u, r1);
                m.putq(out + 0x20u, r2);
                m.putq(out + 0x30u, vf29);
                vf[24] = r0;
                vf[25] = sel(vf[25], vf25x, kX);
                vf[25] = r1;
                vf[26] = r2;
                vf[29] = vf29;
                ctx->vu0_acc = acc;
            };
            auto through = [&](uint32_t mat, uint32_t point, uint32_t out)
            {
                const __m128 c[4] = {m.q(mat), m.q(mat + 0x10u), m.q(mat + 0x20u), m.q(mat + 0x30u)};
                __m128 acc;
                const __m128 vf8 = m.q(point);
                const __m128 r = apply(c, vf8, acc);
                m.putq(out, r);
                vf[8] = vf8;
                vf[10] = r;
                for (int i = 0; i < 4; ++i)
                    vf[14 + i] = c[i];
                ctx->vu0_acc = acc;
            };
            // |a - b|^2 (x of vsub, vmul.xyz, x + y + z).
            auto dist2 = [&](uint32_t a, uint32_t b) -> float
            {
                const __m128 vf5 = m.q(a), vf6 = m.q(b);
                __m128 vf7 = PS2_VSUB(vf5, vf6);
                vf7 = sel(vf7, PS2_VMUL(vf7, vf7), kXyz);
                vf7 = sel(vf7, PS2_VADD(vf7, lane<1>(vf7)), kX);
                vf7 = sel(vf7, PS2_VADD(vf7, lane<2>(vf7)), kX);
                vf[5] = vf5;
                vf[6] = vf6;
                vf[7] = vf7;
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf7));
                return _mm_cvtss_f32(vf7);
            };
            const uint32_t width = 0x00213818u, height = 0x0021381Cu; // getDrawScreenWidth / Height
            // The point's screen copy coordinates (256 x 256 over the draw screen); true when off it.
            auto screen = [&](uint32_t out, uint32_t point, float &x, float &y) -> bool
            {
                static const bool debug = std::getenv("SOTC_STRIPS_DEBUG") != nullptr;
                if (debug)
                {
                    g.a(4, out);
                    g.a(5, point);
                    g.call(0x0108F678u, 0x010F43B8u, sp);
                    const __m128i guest = m.qi(out);
                    viewCoord(ctx, m, out, point);
                    const __m128i mine = m.qi(out);
                    if (std::memcmp(&guest, &mine, 16) != 0)
                    {
                        float a[4], b[4];
                        std::memcpy(a, &guest, 16);
                        std::memcpy(b, &mine, 16);
                        std::fprintf(stderr, "[strips] viewCoord guest %g %g %g %g native %g %g %g %g\n", a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);
                    }
                }
                else
                    viewCoord(ctx, m, out, point);
                const int32_t w = m.s32(width), h = m.s32(height);
                const int32_t hw = static_cast<int32_t>(static_cast<uint32_t>(w) + (static_cast<uint32_t>(w) >> 31)) >> 1;
                const int32_t hh = static_cast<int32_t>(static_cast<uint32_t>(h) + (static_cast<uint32_t>(h) >> 31)) >> 1;
                x = ps2FpuDivide(ctx->fcr31, FPU_MUL_S(FPU_ADD_S(m.f32(out), FPU_CVT_S_W(hw)), 256.0f), FPU_CVT_S_W(w));
                m.putf(out, x);
                y = ps2FpuDivide(ctx->fcr31, FPU_MUL_S(FPU_ADD_S(m.f32(out + 4u), FPU_CVT_S_W(hh)), 256.0f), FPU_CVT_S_W(h));
                m.putf(out + 4u, y);
                ctx->f[0] = y;
                SET_GPR_S32(ctx, 2, h);
                return FPU_C_OLT_S(x, 0.0f) || FPU_C_OLT_S(y, 0.0f) || FPU_C_OLE_S(256.0f, x) || FPU_C_OLE_S(256.0f, y);
            };
            // (a lerp of two points, xyz: a x t + b x (1 - t))
            auto lerp = [&](uint32_t a, uint32_t b, float t, uint32_t out)
            {
                const __m128 vf7 = _mm_set1_ps(t);
                const __m128 vf5 = m.q(a), vf6 = m.q(b);
                const __m128 vf8 = sel(vf[8], PS2_VSUB(vf0, vf7), kW);
                const __m128 acc = sel(ctx->vu0_acc, PS2_VMUL(vf5, vf7), kXyz);
                const __m128 r = sel(vf5, PS2_VADD(acc, PS2_VMUL(vf6, lane<3>(vf8))), kXyz);
                m.putq(out, r);
                vf[5] = r;
                vf[6] = vf6;
                vf[8] = vf8;
                ctx->vu0_acc = acc;
            };
            uint32_t counter = count - 1u;
            for (;;)
            {
                if (!Ram::ok(s1, 0x34u) || !Ram::ok(bones, 0x40u) || !Ram::ok(matrices, 0x40u) || !Ram::ok(fp, 0x40u))
                {
                    // The recompiled code from its loop head (0x10f4068).
                    sreg(17, s1);
                    syncFloats();
                    ctx->pc = 0x010F4068u;
                    return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kStrips), ra0, sp0);
                }
                sreg(17, s1);
                const int32_t a = m.s16(s1 + 8u), b = m.s16(s1 + 0x20u);
                m.putf(sp + 0x80u, m.f32(s1 + 0xCu));
                m.putf(sp + 0x84u, m.f32(s1 + 0x10u));
                m.putf(sp + 0x88u, m.f32(s1 + 0x14u));
                m.putf(sp + 0x8Cu, f25);
                m.putf(sp + 0x90u, m.f32(s1 + 0x24u));
                m.putf(sp + 0x94u, m.f32(s1 + 0x28u));
                m.putf(sp + 0x98u, m.f32(s1 + 0x2Cu));
                m.putf(sp + 0x9Cu, f25);
                m.putqi(sp + 0xA0u, m.qi(sp + 0x90u));
                {
                    const __m128 scale = m.q(s7);
                    const __m128 pa = m.q(sp + 0x80u), pb = m.q(sp + 0x90u);
                    m.putq(s3, sel(pa, PS2_VMUL(pa, scale), kXyz));
                    const __m128 qb = sel(pb, PS2_VMUL(pb, scale), kXyz);
                    m.putq(s2, qb);
                    vf[5] = qb;
                    vf[6] = scale;
                }
                boneInverse(bones + static_cast<uint32_t>(a << 6), sp);
                through(sp, s3, s3);
                through(fp, s3, s3);
                boneInverse(bones + static_cast<uint32_t>(b << 6), sp + 0x40u);
                through(sp + 0x40u, s2, s2);
                through(fp, s2, s2);
                through(matrices + static_cast<uint32_t>(a << 6), sp + 0x80u, sp + 0xA0u);
                through(matrices + static_cast<uint32_t>(b << 6), sp + 0x90u, sp + 0xB0u);
                const float f2 = dist2(sp + 0xA0u, s3);
                f24 = ps2FpuDivide(ctx->fcr31, FPU_MUL_S(m.f32(s1), m.f32(s1)), f2);
                const float f1 = dist2(sp + 0xB0u, s2);
                const float rb2 = FPU_MUL_S(m.f32(s1 + 0x18u), m.f32(s1 + 0x18u));
                f23 = ps2FpuDivide(ctx->fcr31, rb2, f1);
                ctx->f[0] = rb2;
                bool skip = (FPU_C_OLT_S(f24, f27) && FPU_C_OLT_S(f23, f27)) || (FPU_C_OLT_S(f2, f27) && FPU_C_OLT_S(f1, f27));
                float ax = 0, ay = 0, bx = 0, by = 0;
                if (!skip)
                {
                    sreg(20, s4);
                    sreg(22, s6);
                    f22 = m.f32(s1 + 0x30u);
                    skip = screen(s6, s4, ax, ay);
                    f20 = ax;
                    f21 = 256.0f;
                    sreg(21, s5);
                }
                if (!skip)
                {
                    skip = screen(sp + 0xF0u, sp + 0xB0u, bx, by);
                    f20 = bx;
                }
                if (!skip)
                {
                    const float inv = ps2FpuDivide(ctx->fcr31, f25, f22);
                    float fa = FPU_MUL_S(ps2FpuSqrt(ctx->fcr31, f24), inv);
                    float fb = FPU_MUL_S(ps2FpuSqrt(ctx->fcr31, f23), inv);
                    {
                        // vf7 from t0 (mfc1 f22, qmtc2: the 128-bit register, x the width)
                        uint32_t bits;
                        std::memcpy(&bits, &f22, 4);
                        SET_GPR_S32(ctx, 8, static_cast<int32_t>(bits));
                    }
                    const float t = f22;
                    lerp(s4, s3, t, sp + 0x100u);
                    lerp(s5, s2, t, sp + 0x110u);
                    lerp(s4, sp + 0x100u, f26, sp + 0x120u);
                    lerp(s5, sp + 0x110u, f26, sp + 0x130u);
                    vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                    f20 = fa;
                    if (!FPU_C_OLE_S(f20, f25))
                        f20 = 1.0f;
                    float f15 = fb;
                    if (!FPU_C_OLE_S(f15, f25))
                        f15 = 1.0f;
                    f21 = f15;
                    syncFloats();
                    ctx->f[15] = f15;
                    auto vertex = [&](float alpha, float u, float v, uint32_t pos, uint32_t site)
                    {
                        glColor4f(ctx, m, 0.5f, 0.5f, 0.5f, alpha);
                        glTexCoord2f(ctx, m, u, v);
                        glVertex3fv(g, pos, site, sp);
                    };
                    g.a(4, 5u);
                    g.call(kGlBegin, 0x010F4664u, sp);
                    vertex(f20, m.f32(s6), m.f32(sp + 0xE4u), sp + 0xA0u, 0x010F4690u);
                    vertex(f21, m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0xB0u, 0x010F46BCu);
                    vertex(f20, m.f32(s6), m.f32(sp + 0xE4u), sp + 0x120u, 0x010F46E8u);
                    vertex(f21, m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0x130u, 0x010F4714u);
                    g.call(kGlEnd, 0x010F471Cu, sp);
                    g.a(4, 5u);
                    g.call(kGlBegin, 0x010F4724u, sp);
                    vertex(f20, m.f32(s6), m.f32(sp + 0xE4u), sp + 0x120u, 0x010F4750u);
                    vertex(f21, m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0x130u, 0x010F477Cu);
                    vertex(FPU_MUL_S(f20, f28), m.f32(s6), m.f32(sp + 0xE4u), sp + 0x100u, 0x010F47A8u);
                    vertex(FPU_MUL_S(f21, f28), m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0x110u, 0x010F47D4u);
                    g.call(kGlEnd, 0x010F47DCu, sp);
                }
                (void)ax;
                (void)ay;
                (void)bx;
                (void)by;
                s1 += 0x34u;
                const uint32_t left = counter - 1u;
                m.put32(sp + 0x140u, left);
                counter = left;
                if (left == 0xFFFFFFFFu)
                    break;
            }
            vf[28] = vf28;
            ctx->f[20] = f20;
            ctx->f[21] = f21;
            ctx->f[22] = f22;
        }
        ctx->f[12] = 1.0f;
        g.call(kGlZOffset, 0x010F4804u, sp);
        enable(kGlDisable, 0u, 0x010F480Cu);
        enable(kGlDisable, 0x14u, 0x010F4814u);
        enable(kGlEnable, 2u, 0x010F481Cu);
        enable(kGlEnable, 3u, 0x010F4824u);
        enable(kGlDisable, 8u, 0x010F482Cu);
        g.callRel(0x010F4834u, sp); // iosDlPopLayer (sotc_native's hook)
        for (int i = 0; i < 10; ++i)
            SET_GPR_U64(ctx, saved[i], in[i]);
        for (int i = 0; i < 9; ++i)
            ctx->f[20 + i] = fin[i];
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    // ---- DispLayoutObjModel (0x10f5920; a0 the camera object, a1 the object)
    void dispLayoutObjModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x20u;
        const uint32_t object = GPR_U32(ctx, 5), camera = GPR_U32(ctx, 4);
        if (!Ram::ok(sp, 0x20u) || !Ram::ok(object, 0x10u))
            return fallback(rdram, ctx, runtime, kDisp);
        decomp_prefetch::aheadDisp(rdram, object);
        const uint64_t in16 = GPR_U64(ctx, 16), in17 = GPR_U64(ctx, 17), in18 = GPR_U64(ctx, 18);
        m.put64(sp, in16);
        m.put64(sp + 0x10u, in18);
        m.put64(sp + 8u, in17);
        m.put64(sp + 0x18u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 5));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 4));
        const uint32_t s1 = characterClass(m, object);
        SET_GPR_S32(ctx, 17, static_cast<int32_t>(s1));
        const uint32_t s0 = layoutClass(g, object, 0x010F5948u, sp);
        if (!Ram::ok(s0, 0x188u) || !Ram::ok(m.u32(s0 + 0x144u), 0x268u) || !Ram::ok(m.u32(m.u32(s0 + 0x144u)), 0x24u) ||
            !Ram::ok(m.u32(m.u32(m.u32(s0 + 0x144u)) + 0x20u), 0x1Cu))
        {
            // From after GetLayoutClass (0x10f5950).
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(s0));
            SET_GPR_U32(ctx, 29, sp);
            SET_GPR_U32(ctx, 31, 0x010F5950u);
            ctx->pc = 0x010F5950u;
            return runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kDisp), ra0, sp0);
        }
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
        auto finish = [&](uint64_t v0)
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 16, in16);
            SET_GPR_U64(ctx, 17, in17);
            SET_GPR_U64(ctx, 18, in18);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        const uint32_t header = m.u32(m.u32(m.u32(s0 + 0x144u)) + 0x20u);
        if ((m.u32(header + 0x18u) & 1u) == 0u)
        {
            g.a(4, camera);
            g.a(5, s0);
            g.a(6, s1);
            g.callNative(solidDisp, kSolidDisp, 0x010F59C4u, sp);
        }
        else
        {
            g.a(4, camera);
            g.a(5, s0);
            g.a(6, s1);
            g.call(kSkinPalette, 0x010F5974u, sp);
            if (s1 != 0u && m.s8(s1 + 0x10A5u) != 0 && m.u32(kStripsOff) == 0u)
            {
                const uint32_t cameraClass = layoutClass(g, camera, 0x010F599Cu, sp);
                g.a(5, s1);
                g.a(4, cameraClass);
                g.a(6, s0);
                g.callNative(strips, kStrips, 0x010F59ACu, sp);
            }
        }
        uint64_t v0 = 0x01190000u;
        if (m.u32(kClothOn) == 0u)
            return finish(v0);
        v0 = static_cast<uint64_t>(static_cast<int64_t>(m.s32(s0 + 0x180u)));
        if (v0 == 0u)
            return finish(v0);
        v0 = m.u64(m.u32(s0 + 0x144u) + 0x260u) & 0x80u;
        if (v0 != 0u)
            return finish(v0);
        // The tail jump to DispCloth (0x11392c0) with its own frame at our caller's sp - 0x20.
        const uint32_t cloths = m.u32(s0 + 0x178u);
        SET_GPR_U64(ctx, 16, in16);
        SET_GPR_U64(ctx, 17, in17);
        SET_GPR_U64(ctx, 18, in18);
        const uint32_t csp = sp0 - 0x20u;
        m.put64(csp, in16);
        m.put64(csp + 8u, in17);
        m.put64(csp + 0x10u, ra0);
        const int16_t count = m.s16(cloths + 4u);
        uint32_t s0c = m.u32(cloths);
        if (count > 0)
        {
            int32_t left = count;
            do
            {
                if (m.s8(s0c + 0x81u) != 0)
                    m.put8(s0c + 0x81u, 0u);
                else
                {
                    SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0c));
                    SET_GPR_S32(ctx, 17, left);
                    g.a(4, s0c);
                    g.call(kClothMesh, 0x011392ECu, csp);
                }
                --left;
                s0c += 0x90u;
            } while (left != 0);
        }
        SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_U64(ctx, 16, in16);
        SET_GPR_U64(ctx, 17, in17);
        SET_GPR_U64(ctx, 18, in18);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    // ---- DispDispObjGroupModel (0x10f52c8; a0 the camera object) -------------
    void dispDispObjGroupModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x80u;
        if (!Ram::ok(sp, 0x80u))
            return sotc_decomp::recompiled(kWalk)(rdram, ctx, runtime);
        // The prologue's saves (the recompiled code's epilogue reads them when it goes on from a label).
        uint64_t in[32];
        for (int r : {16, 17, 18, 19, 20, 21, 22, 23, 30, 31})
            in[r] = GPR_U64(ctx, r);
        m.put64(sp + 0x60u, in[22]);
        m.put64(sp + 0x30u, in[16]);
        m.put64(sp + 0x38u, in[17]);
        m.put64(sp + 0x40u, in[18]);
        m.put64(sp + 0x48u, in[19]);
        m.put64(sp + 0x50u, in[20]);
        m.put64(sp + 0x58u, in[21]);
        m.put64(sp + 0x68u, in[23]);
        m.put64(sp + 0x70u, in[30]);
        m.put64(sp + 0x78u, in[31]);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 22, GPR_U64(ctx, 4));
        const uint32_t s6 = GPR_U32(ctx, 4);
        auto set = [&](int r, uint32_t v) { SET_GPR_S32(ctx, r, static_cast<int32_t>(v)); };
        auto finish = [&]()
        {
            SET_GPR_U64(ctx, 2, 0x01190000u);
            for (int r : {16, 17, 18, 19, 20, 21, 22, 23, 30, 31})
                SET_GPR_U64(ctx, r, in[r]);
            SET_GPR_U32(ctx, 29, sp0);
            returnTo(ctx);
        };
        // The recompiled code from one of its loop heads with the registers it has there.
        auto resume = [&](uint32_t label)
        {
            SET_GPR_U32(ctx, 29, sp);
            ctx->pc = label;
            runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kWalk), ra0, sp0);
        };
        const uint32_t group = m.u32(kGroupBase) + 0x10u; // GetLayoutDispObjGroup
        m.put32(sp + 0x20u, group);
        if (m.u32(kWalkOn) == 0u)
            return finish();
        set(30, 0x01190000u);
        g.callRel(0x010F5310u, sp); // isysObjWaitSema
        set(19, 0u);
        g.call(kGraphFsEffectReset, 0x010F5318u, sp);
        clearSortList(m, m.u32(kSortList));
        g.a(4, m.u32(sp + 0x20u));
        SET_GPR_U64(ctx, 5, 0u);
        g.callRel(0x010F532Cu, sp); // isysGroupForFirst
        uint32_t s4 = g.v0(), s3 = 0u;
        set(20, s4);
        QuatProduct qp{};
        while (s4 != 0u)
        {
            if (!Ram::ok(s4, 0x10u) || !Ram::ok(m.u32(s4), 0x10u))
            {
                set(19, Ram::ok(s4, 4u) ? m.u32(s4) : s3);
                set(20, s4);
                return resume(0x010F5348u);
            }
            s3 = m.u32(s4);
            set(19, s3);
            const uint32_t s0 = layoutClass(g, s3, 0x010F5348u, sp);
            set(16, s0);
            if (s0 != 0u)
            {
                set(21, 1u);
                const uint32_t s1 = layoutClass(g, s6, 0x010F5360u, sp);
                set(17, s1);
                if (!Ram::ok(s0, 0x1C0u) || !Ram::ok(s1, 0x160u) || !Ram::ok(m.u32(s0 + 0x140u), 0x40u) || !Ram::ok(m.u32(s0 + 0x144u), 0x268u))
                    return resume(0x010F5348u);
                const uint32_t s2 = m.u32(s0 + 0x140u);
                set(18, s2);
                uint64_t s5 = 1u, s7 = 1u;
                set(23, 1u);
                bool simple = false;
                const __m128 vf0 = ctx->vu0_vf[0];
                if (m.s8(s0 + 0x1B6u) != 0)
                {
                    // The billboard: the camera's quaternion as its z and y axes
                    // (into the frame at +0x10 and +0), the turn between them,
                    // times the object's own quaternion (+0x80) into its +0x13c.
                    __m128 vf4 = m.q(m.u32(s1 + 0x13Cu));
                    __m128 vf5 = PS2_VMUL(vf4, vf4);
                    __m128 vf6 = mr32(vf4);
                    __m128 vf7 = PS2_VMUL(vf6, vf4);
                    vf6 = mr32(vf6);
                    __m128 vf8 = PS2_VMUL(vf6, vf4);
                    vf6 = sel(vf6, PS2_VSUB(vf0, lane<0>(vf5)), kZ);
                    vf6 = sel(vf6, PS2_VSUB(vf6, lane<1>(vf5)), kZ);
                    vf6 = sel(vf6, PS2_VADD(vf7, lane<3>(vf7)), kY);
                    vf6 = sel(vf6, PS2_VSUB(vf8, lane<1>(vf8)), kX);
                    vf6 = sel(vf6, PS2_VSUB(vf0, vf0), kW);
                    vf5 = sel(vf5, PS2_VADD(vf0, vf0), kW);
                    vf6 = sel(vf6, PS2_VMUL(vf6, lane<3>(vf5)), kXyz);
                    vf6 = sel(vf6, PS2_VADD(vf6, lane<3>(vf0)), kZ);
                    m.putq(sp + 0x10u, vf6);
                    vf4 = m.q(m.u32(s1 + 0x13Cu));
                    vf5 = PS2_VMUL(vf4, vf4);
                    vf6 = mr32(vf4);
                    vf7 = PS2_VMUL(vf6, vf4);
                    vf6 = sel(vf6, PS2_VADD(vf0, lane<1>(vf7)), kZ);
                    vf6 = sel(vf6, PS2_VSUB(vf6, lane<3>(vf7)), kZ);
                    vf6 = sel(vf6, PS2_VSUB(vf0, lane<0>(vf5)), kY);
                    vf6 = sel(vf6, PS2_VSUB(vf6, lane<2>(vf5)), kY);
                    vf6 = sel(vf6, PS2_VADD(vf7, lane<2>(vf7)), kX);
                    vf6 = sel(vf6, PS2_VSUB(vf0, vf0), kW);
                    vf5 = sel(vf5, PS2_VADD(vf0, vf0), kW);
                    vf6 = sel(vf6, PS2_VMUL(vf6, lane<3>(vf5)), kXyz);
                    vf6 = sel(vf6, PS2_VADD(vf6, lane<3>(vf0)), kY);
                    m.putq(sp, vf6);
                    ctx->vu0_vf[4] = vf4;
                    ctx->vu0_vf[5] = vf5;
                    ctx->vu0_vf[6] = vf6;
                    ctx->vu0_vf[7] = vf7;
                    ctx->vu0_vf[8] = vf8;
                    g.a(4, m.u32(s0 + 0x13Cu));
                    g.a(5, sp + 0x10u);
                    g.a(6, sp);
                    g.callRel(0x010F540Cu, sp); // GetQuaternionByAxisDirZY
                    const uint32_t q = m.u32(s0 + 0x13Cu);
                    qp.vf14 = ctx->vu0_vf[14];
                    qp.vf15 = ctx->vu0_vf[15];
                    qp.vf16 = ctx->vu0_vf[16];
                    qp.acc = ctx->vu0_acc;
                    const __m128 vf11 = m.q(q), vf12 = m.q(s0 + 0x80u);
                    quatProduct(vf11, vf12, qp);
                    m.putq(q, qp.vf13);
                    ctx->vu0_vf[11] = vf11;
                    ctx->vu0_vf[12] = vf12;
                    ctx->vu0_vf[13] = qp.vf13;
                    ctx->vu0_vf[14] = qp.vf14;
                    ctx->vu0_vf[15] = qp.vf15;
                    ctx->vu0_vf[16] = qp.vf16;
                    ctx->vu0_acc = qp.acc;
                    simple = true;
                }
                if (m.s8(s0 + 0x1B7u) != 0)
                {
                    // Linked to the camera object: its matrix x our point (+0x40)
                    // into +0x134, its quaternion x ours (+0x80) into +0x13c.
                    const uint32_t out = m.u32(s0 + 0x134u), mat = m.u32(s1 + 0x158u);
                    const __m128 vf8 = m.q(s0 + 0x40u);
                    const __m128 c[4] = {m.q(mat), m.q(mat + 0x10u), m.q(mat + 0x20u), m.q(mat + 0x30u)};
                    __m128 acc;
                    const __m128 vf10 = apply(c, vf8, acc);
                    m.putq(out, vf10);
                    const uint32_t q = m.u32(s0 + 0x13Cu);
                    const __m128 vf11 = m.q(m.u32(s1 + 0x13Cu)), vf12 = m.q(s0 + 0x80u);
                    qp.vf14 = c[0];
                    qp.vf15 = c[1];
                    qp.vf16 = c[2];
                    qp.acc = acc;
                    quatProduct(vf11, vf12, qp);
                    m.putq(q, qp.vf13);
                    ctx->vu0_vf[8] = vf8;
                    ctx->vu0_vf[10] = vf10;
                    ctx->vu0_vf[11] = vf11;
                    ctx->vu0_vf[12] = vf12;
                    ctx->vu0_vf[13] = qp.vf13;
                    ctx->vu0_vf[14] = qp.vf14;
                    ctx->vu0_vf[15] = qp.vf15;
                    ctx->vu0_vf[16] = qp.vf16;
                    ctx->vu0_vf[17] = c[3];
                    ctx->vu0_acc = qp.acc;
                    simple = true;
                }
                if (simple)
                {
                    ctx->f[12] = 0.0f;
                    g.a(4, s0);
                    g.call(kUpdateSimple, 0x010F54E4u, sp);
                }
                if (m.s8(s0 + 0x1B8u) != 0)
                {
                    if (const uint32_t update = m.u32(s2 + 0xD8u))
                    {
                        g.a(4, s3);
                        g.call(update, 0x010F5504u, sp);
                    }
                    if (const uint32_t clip = m.u32(s2 + 0xDCu))
                    {
                        g.a(4, s3);
                        if (clip == kClip)
                            g.callNative(clipLayoutObjModel, kClip, 0x010F5518u, sp);
                        else
                            g.call(clip, 0x010F5518u, sp);
                        s7 = GPR_U64(ctx, 2);
                        SET_GPR_U64(ctx, 23, s7);
                    }
                    float f0 = m.f32(s2 + 0x20u);
                    ctx->f[0] = f0;
                    if (FPU_C_OLT_S(0.0f, f0))
                    {
                        f0 = m.f32(s2 + 0x30u);
                        ctx->f[0] = f0;
                        if (FPU_C_OLE_S(0.0f, f0))
                        {
                            s5 = s7 < 1u ? 1u : 0u;
                            SET_GPR_U64(ctx, 21, s5);
                        }
                    }
                }
                const uint64_t shown = (m.u64(m.u32(s0 + 0x144u) + 0x260u) >> 8) & 1u;
                if (s5 != shown)
                {
                    const uint32_t cc = characterClass(m, s3);
                    if (cc != 0u)
                        if (const uint32_t told = m.u32(cc + 0xCC0u))
                        {
                            g.a(4, s3);
                            SET_GPR_U64(ctx, 5, s5);
                            g.call(told, 0x010F5580u, sp);
                        }
                }
                if (s5 == 0u)
                {
                    const float key = m.f32(s2 + 0x20u);
                    ctx->f[0] = key;
                    m.putf(s2 + 4u, key);
                    g.a(5, s2);
                    g.a(4, m.u32(kSortList));
                    g.call(kAddSortObj, 0x010F559Cu, sp);
                }
                const uint32_t geo = m.u32(s0 + 0x144u);
                m.put64(geo + 0x260u, (m.u64(geo + 0x260u) & ~0x100ull) | (s5 << 8));
            }
            else
            {
                const uint32_t gdc = geoDispClass(m, s3);
                set(16, gdc);
                if (const uint32_t update = m.u32(gdc + 0xD8u))
                {
                    g.a(4, s3);
                    g.call(update, 0x010F55E0u, sp);
                }
                const float f1 = m.f32(gdc + 0x20u);
                if (FPU_C_OLT_S(0.0f, f1))
                {
                    const float f0 = m.f32(gdc + 0x30u);
                    ctx->f[0] = f0;
                    if (FPU_C_OLE_S(0.0f, f0))
                    {
                        m.putf(gdc + 4u, f1);
                        g.a(5, gdc);
                        g.a(4, m.u32(kSortList));
                        g.call(kAddSortObj, 0x010F5614u, sp);
                    }
                }
            }
            uint32_t next;
            if (groupNext(m, s4, next))
            {
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(next));
            }
            else
            {
                g.a(4, s4);
                SET_GPR_U64(ctx, 5, 0u);
                g.callRel(0x010F5620u, sp); // isysGroupForNext
                next = g.v0();
            }
            s4 = next;
            set(20, s4);
        }
        set(18, 0x01190000u);
        if (s3 != 0u)
        {
            g.a(4, s3);
            SET_GPR_U64(ctx, 5, 0u);
            g.callRel(0x010F5640u, sp); // isysGroupForExit
        }
        // The second list: the first one without the objects with bit 4 of +0x260.
        clearSortList(m, m.u32(kSortList2));
        uint32_t s0 = firstSortObj(m, m.u32(kSortList));
        while (s0 != 0u)
        {
            set(16, s0);
            const uint32_t s1 = nextSortObj(m, m.u32(kSortList), s0);
            set(17, s1);
            const uint32_t lc = layoutClass(g, m.u32(s0), 0x010F567Cu, sp);
            if (lc == 0u)
            {
                g.a(4, m.u32(kSortList2));
                g.a(5, s0);
                g.call(kAddSortObj, 0x010F56B0u, sp);
            }
            else if ((m.u64(m.u32(lc + 0x144u) + 0x260u) & 0x10u) == 0u)
            {
                g.a(4, m.u32(kSortList2));
                g.a(5, s0);
                g.call(kAddSortObj, 0x010F56A0u, sp);
            }
            s0 = s1;
        }
        set(16, kSortList2);
        if (firstSortObj(m, m.u32(kSortList2)) != 0u)
        {
            g.call(kSetEnviroment, 0x010F56D8u, sp);
            g.a(4, m.u32(kSortList2));
            g.a(6, m.u32(sp + 0x20u));
            g.a(5, s6);
            g.call(kLightPass, 0x010F56E8u, sp);
            const uint32_t list = m.u32(kSortList2);
            set(19, list);
            uint32_t s2 = firstSortObj(m, list);
            set(18, s2);
            while (s2 != 0u)
            {
                if (!Ram::ok(s2, 0x10u))
                    return resume(0x010F5710u);
                const uint32_t obj = m.u32(s2);
                set(16, obj);
                const uint32_t lc = layoutClass(g, obj, 0x010F5710u, sp);
                uint32_t cls;
                if (lc != 0u)
                {
                    cls = m.u32(lc + 0x140u);
                    set(17, cls);
                    if (obj != 0u)
                    {
                        g.a(4, obj);
                        g.callRel(0x010F5728u, sp); // iosDlLockObj
                    }
                    if (const uint32_t lights = m.u32(cls + 0xD4u))
                    {
                        g.a(6, m.u32(cls + 0xE4u));
                        g.a(4, s6);
                        g.a(5, obj);
                        g.call(lights, 0x010F5744u, sp);
                    }
                    const uint32_t draw = m.u32(cls + 0xD0u);
                    g.a(5, obj);
                    g.a(4, s6);
                    if (draw == kDisp)
                        g.callNative(dispLayoutObjModel, kDisp, 0x010F5754u, sp);
                    else
                        g.call(draw, 0x010F5754u, sp);
                }
                else
                {
                    cls = geoDispClass(m, obj);
                    set(17, cls);
                    if (obj != 0u)
                    {
                        g.a(4, obj);
                        g.callRel(0x010F577Cu, sp); // iosDlLockObj
                    }
                    const uint32_t draw = m.u32(cls + 0xD0u);
                    g.a(5, obj);
                    g.a(4, s6);
                    g.call(draw, 0x010F578Cu, sp);
                }
                s2 = nextSortObj(m, list, s2);
                set(18, s2);
            }
        }
        g.callRel(0x010F57ACu, sp); // isysObjSignalSema
        m.put32(kStripsOff, 0u);
        finish();
    }
}

SOTC_DECOMP_AB_FUNCTION(0x010F52C8u, DispDispObjGroupModel, decomp_walk::dispDispObjGroupModel);
SOTC_DECOMP_AB_FUNCTION(0x010F4890u, ClipLayoutObjModel, decomp_walk::clipLayoutObjModel);
SOTC_DECOMP_AB_FUNCTION(0x010F5920u, DispLayoutObjModel, decomp_walk::dispLayoutObjModel);
SOTC_DECOMP_AB_FUNCTION(0x010F3A08u, solidDisp_010F3A08, decomp_walk::solidDisp);
SOTC_DECOMP_FLOAT_FUNCTION(0x01098840u, modelClipSolid, decomp_walk::modelClipSolid);
SOTC_DECOMP_AB_FUNCTION(0x010F3EB8u, hazeStrips_010F3EB8, decomp_walk::strips);
SOTC_DECOMP_AB_FUNCTION(0x0108D730u, glVertexKick_0108D730, decomp_walk::glVertexKick);
SOTC_DECOMP_AB_FUNCTION(0x0108EBC0u, glVertex3fv, decomp_walk::glVertex3fvEntry);
