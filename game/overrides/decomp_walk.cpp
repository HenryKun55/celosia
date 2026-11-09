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
#include "runtime/ps2_diag.h"
#include "sotc_worker.h"
#include "lines67.h"
#include "sotc_prof.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/native_tuning.h"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

namespace decomp_prefetch
{
    void aheadClip(const uint8_t *rdram, uint32_t object);
    void aheadDisp(const uint8_t *rdram, uint32_t object);
    void aheadWalkClip(const uint8_t *rdram, uint32_t object);
    void aheadWalkDisp(const uint8_t *rdram, uint32_t object);
}

namespace sotc_native
{
    bool nativeDraws(); // round 103: the model hook draws natively (no PS2 lists, no GS raster)
    // round 128 (imm128): an immediate primitive's pairs straight to the 2D assembler; its check's records
    void immFeed128(uint8_t *rdram, const uint64_t *pairs, uint32_t count, uint32_t layer, bool haze);
    void immRecord128(std::vector<uint64_t> &rec, const uint64_t *pairs, uint32_t count, uint32_t layer, bool haze);
    void immArm128(std::vector<uint64_t> *rec);
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
        uint8_t *p(uint32_t a) const
        {
            lines67::touch(a);
            return rdram + (a & PS2_RAM_MASK);
        }
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
        { fn(rdram, ctx, runtime); runtime->checkTransfer(); }
        while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
        {
            PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
            if (!next)
                break;
            { next(rdram, ctx, runtime); runtime->checkTransfer(); }
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
            { fn(rdram, ctx, runtime); runtime->checkTransfer(); }
        }
    };

    // GetLayoutClass (decomp_layout.cpp) for its plain cases; a guest call for the others.
    // Round 49: the plain cases inline at the callers (it was a call with G
    // copied through the stack, ~25 instructions, ~700 times a frame), the
    // guest call out of line. Same reads, same result: no switch.
    __attribute__((noinline)) uint32_t layoutClassCall(const G &g, uint32_t object, uint32_t site, uint32_t sp)
    {
        static sotc_decomp::NativeCount count("layoutClassCall");
        count.hit();
        g.a(4, object);
        g.call(kGetLayoutClass, site, sp);
        return g.v0();
    }
    inline __attribute__((always_inline)) uint32_t layoutClass(const G &g, uint32_t object, uint32_t site, uint32_t sp)
    {
        const uint32_t index = static_cast<uint32_t>(g.m.u8(object + 8u)) - 9u;
        if (index >= 0xEu)
            return 0u;
        const uint32_t target = g.m.u32(kClassTable + index * 4u);
        if (__builtin_expect(target == 0x010F1A58u, 1))
            return object + 0x20u;
        if (target == 0x010F1A98u)
            return object + 0x90u;
        if (target == 0x010F1AA0u)
            return 0u;
        return layoutClassCall(g, object, site, sp);
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

    // ---- Round 67 (SOTC_WALK67): the walk's own lines asked for ahead ----
    // Only reads and PLDs: nothing here is written or called, and every
    // decision the walk makes reads the same words as before.
    inline void ask(const Ram m, uint32_t a)
    {
        lines67::pref(a);
        __builtin_prefetch(m.rdram + (a & PS2_RAM_MASK));
    }
    // GetLayoutClass's plain cases without a call (0 for the others).
    inline uint32_t layoutClassHint(const Ram m, uint32_t object)
    {
        const uint32_t index = static_cast<uint32_t>(m.u8(object + 8u)) - 9u;
        if (index >= 0xEu)
            return 0u;
        const uint32_t target = m.u32(kClassTable + index * 4u);
        return target == 0x010F1A58u ? object + 0x20u : target == 0x010F1A98u ? object + 0x90u : 0u;
    }
    // A sort list's nodes in GetNextSortObj's order, read without its
    // writes (the bucket index at +0x10 is kept here): the passes over it
    // ask for their next nodes' lines from this copy.
    void sortOrder(const Ram m, uint32_t list, std::vector<uint32_t> &out)
    {
        out.clear();
        const int32_t count = m.s32(list + 4u);
        const uint32_t buckets = m.u32(list);
        if (count <= 0 || count > 0x10000 || !Ram::ok(buckets, static_cast<uint32_t>(count) * 4u))
            return;
        for (int32_t index = 0; index < count && out.size() < 4096u; ++index)
            for (uint32_t node = m.u32(buckets + static_cast<uint32_t>(index) * 4u); node != 0u && out.size() < 4096u; node = m.u32(node + 8u))
            {
                if (!Ram::ok(node, 0x10u))
                    break;
                out.push_back(node);
            }
    }
    // The stages ahead of node k of a pass over `order`: the node's line 6
    // on, its object's type and class lines 3 on (the node read: asked for 3
    // turns ago), the class's +0x144 target's +0x260 word and the draw's
    // class words 1 on (the object's lines asked for 2 turns ago).
    void sortAhead(const Ram m, const std::vector<uint32_t> &order, size_t k, bool draw)
    {
        if (k == 0)
        {
            for (size_t i = 0; i < 6u && i < order.size(); ++i)
                ask(m, order[i]);
        }
        if (k + 6u < order.size())
            ask(m, order[k + 6u]);
        if (k + 3u < order.size())
        {
            const uint32_t obj = m.u32(order[k + 3u]);
            if (Ram::ok(obj, 0x200u))
            {
                ask(m, obj + 8u);
                ask(m, obj + 0x160u);
                ask(m, obj + 0x1D0u);
            }
        }
        if (k + 1u < order.size())
        {
            const uint32_t obj = m.u32(order[k + 1u]);
            if (Ram::ok(obj, 0x200u))
                if (const uint32_t lc = layoutClassHint(m, obj))
                {
                    const uint32_t geo = m.u32(lc + 0x144u);
                    if (Ram::ok(geo, 0x268u))
                        ask(m, geo + 0x260u);
                    if (draw)
                    {
                        const uint32_t cls = m.u32(lc + 0x140u);
                        if (Ram::ok(cls, 0xE8u))
                            ask(m, cls + 0xD0u);
                    }
                }
        }
    }
    // The group chain kAhead entries on (isysGroupForNext's entries: +0xc's
    // high half the next index, +0 the object).
    struct GroupRunner
    {
        uint32_t entry = 0u;
        void step(const Ram m)
        {
            if (!entry)
                return;
            const uint32_t index = static_cast<uint16_t>(m.u32(entry + 0xCu) >> 16);
            entry = index ? kEntries + index * 24u : 0u;
            if (!entry || !Ram::ok(entry, 0x18u))
            {
                entry = 0u;
                return;
            }
            ask(m, entry);
            ask(m, entry + 0x17u);
        }
    };


    // ---- Round 79's measurement (SOTC_WALK79_LOG=1): the walk's time by step and callee ----
    namespace wprof
    {
        inline bool on()
        {
            static const bool v = ps2_diag::env("SOTC_WALK79_LOG") && std::atoi(ps2_diag::env("SOTC_WALK79_LOG")) != 0;
            return v;
        }
        struct Slot
        {
            char kind;
            uint32_t target;
            uint64_t t, n;
        };
        static Slot g_slots[256];
        static int g_used = 0;
        static uint64_t g_phase[8], g_cnt[8], g_frames = 0;
        inline void add(char kind, uint32_t target, uint64_t dt)
        {
            for (int i = 0; i < g_used; ++i)
                if (g_slots[i].kind == kind && g_slots[i].target == target)
                {
                    g_slots[i].t += dt;
                    ++g_slots[i].n;
                    return;
                }
            if (g_used < 256)
                g_slots[g_used++] = {kind, target, dt, 1u};
        }
        inline uint64_t now() { return on() ? sotc_prof::ticks() : 0u; }
        void report()
        {
            if (++g_frames % 120u)
                return;
#ifdef PLATFORM_VITA
            const double k = 1.0 / 1000.0 / 120.0;
#else
            const double k = 1.0 / 1e6 / 120.0;
#endif
            std::fprintf(stderr, "[walk79] frames %llu: all %.2f grp %.2f copy %.2f light %.2f draw %.2f ms | objs %.1f layout %.1f geo %.1f shown %.1f drawn %.1f\n",
                         (unsigned long long)g_frames, g_phase[0] * k, g_phase[1] * k, g_phase[2] * k, g_phase[3] * k, g_phase[4] * k,
                         g_cnt[0] / 120.0, g_cnt[1] / 120.0, g_cnt[2] / 120.0, g_cnt[3] / 120.0, g_cnt[4] / 120.0);
            std::sort(g_slots, g_slots + g_used, [](const Slot &a, const Slot &b) { return a.t > b.t; });
            std::string line = "[walk79] sites:";
            for (int i = 0; i < g_used && i < 40; ++i)
            {
                char b[64];
                std::snprintf(b, sizeof b, " %c%08x %.2f/x%.1f", g_slots[i].kind, g_slots[i].target, g_slots[i].t * k, g_slots[i].n / 120.0);
                line += b;
            }
            std::fprintf(stderr, "%s\n", line.c_str());
            g_used = 0;
            for (auto &v : g_phase)
                v = 0;
            for (auto &v : g_cnt)
                v = 0;
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
    // Round 49 (SOTC_FASTNATIVE): ps2VuClipFlags(p, p) on the vector side,
    // one NEON->ARM move: the same integer compares of x, y, z (and their
    // sign-flipped patterns) against |w|'s pattern (0x7fffff when w's
    // exponent is 0), the six bits ORed into one lane. Exact on every host.
    inline __attribute__((always_inline)) uint32_t clipFlagsVec(__m128 p)
    {
#if defined(__ARM_NEON)
        const int32x4_t b = vreinterpretq_s32_f32(vreinterpretq_f32_m128(p));
        const int32x4_t w = vdupq_lane_s32(vget_high_s32(b), 1);
        const uint32x4_t hasExp = vtstq_s32(w, vdupq_n_s32(0x7F800000));
        const int32x4_t limit = vbslq_s32(hasExp, vandq_s32(w, vdupq_n_s32(0x7FFFFFFF)), vdupq_n_s32(0x007FFFFF));
        const uint32x4_t gp = vcgtq_s32(b, limit);
        const uint32x4_t gn = vcgtq_s32(veorq_s32(b, vdupq_n_s32(static_cast<int32_t>(0x80000000u))), limit);
        const uint32x4_t kp = {1u, 4u, 16u, 0u}, kn = {2u, 8u, 32u, 0u};
        const uint32x4_t bits = vorrq_u32(vandq_u32(gp, kp), vandq_u32(gn, kn));
        uint32x2_t h = vorr_u32(vget_low_u32(bits), vget_high_u32(bits));
        h = vorr_u32(h, vrev64_u32(h));
        return vget_lane_u32(h, 0);
#else
        return ps2VuClipFlags(p, p);
#endif
    }

    // The fast clipByMinMaxCube: the same operations on the same values, each
    // operand's clamp (PS2X_VU_IN) done once (the matrix rows, the box's two
    // corners, vf0.w) and left out where the operand is an op's result (it
    // maps those to themselves: ps2VuMulK / AddK with K = 1); the corners'
    // lanes as broadcasts of min / max, c3 x w once (the same product every
    // corner), the CLIP in NEON. The registers written back as the old code.
    uint32_t clipByMinMaxCubeFast(R5900Context *ctx, const Ram m, uint32_t lo, uint32_t hi, uint32_t mat)
    {
        __m128 *vf = ctx->vu0_vf;
        const __m128 vf0 = vf[0];
        const __m128 vf1 = sel(m.q(lo), vf0, kW), vf2 = sel(m.q(hi), vf0, kW);
        const __m128 c[4] = {m.q(mat), m.q(mat + 0x10u), m.q(mat + 0x20u), m.q(mat + 0x30u)};
        const __m128 n0 = PS2X_VU_IN(c[0]), n1 = PS2X_VU_IN(c[1]), n2 = PS2X_VU_IN(c[2]);
        const __m128 lo1 = PS2X_VU_IN(vf1), hi1 = PS2X_VU_IN(vf2);
        const __m128 lx = lane<0>(lo1), ly = lane<1>(lo1), lz = lane<2>(lo1), hx = lane<0>(hi1), hy = lane<1>(hi1), hz = lane<2>(hi1);
        const __m128 c3w = PS2_VMUL(c[3], lane<3>(vf0));
        __m128 acc = ctx->vu0_acc, out[2] = {vf[20], vf[21]};
        uint32_t flags = ctx->vu0_clip_flags, v0 = 0x3Fu;
        auto corner = [&](int k, __m128 x, __m128 y, __m128 z) __attribute__((always_inline)) -> bool
        {
            acc = PS2_VMUL_K(1, 1, n0, x);
            acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n1, y));
            acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n2, z));
            const __m128 p = PS2_VADD_K(1, 1, acc, c3w);
            out[k & 1] = p;
            const uint32_t j = clipFlagsVec(p);
            flags = ((flags << 6) | j) & 0xFFFFFFu;
            sotc_decomp::clipShift(ctx, j); // round 61: decided on its own judgements only (v0 starts at 0x3f)
            v0 &= flags;
            return v0 == 0u;
        };
        // vf10..vf17's x, y, z: min / max as clipByMinMaxCube's selects pick them.
        (void)(corner(0, lx, ly, lz) || corner(1, lx, hy, hz) || corner(2, hx, ly, hz) || corner(3, hx, hy, lz) ||
               corner(4, lx, ly, hz) || corner(5, lx, hy, lz) || corner(6, hx, ly, lz) || corner(7, hx, hy, hz));
        const __m128 kXy = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, -1)), kXz = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, -1)),
                     kYz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, 0));
        vf[10] = sel(vf[10], vf1, kXyz);
        vf[11] = sel(sel(vf[11], vf1, kX), vf2, kYz);
        vf[12] = sel(sel(vf[12], vf1, kY), vf2, kXz);
        vf[13] = sel(sel(vf[13], vf1, kZ), vf2, kXy);
        vf[14] = sel(sel(vf[14], vf1, kXy), vf2, kZ);
        vf[15] = sel(sel(vf[15], vf2, kY), vf1, kXz);
        vf[16] = sel(sel(vf[16], vf2, kX), vf1, kYz);
        vf[17] = sel(vf[17], vf2, kXyz);
        vf[1] = vf1;
        vf[2] = vf2;
        for (int i = 0; i < 4; ++i)
            vf[4 + i] = c[i];
        vf[20] = out[0];
        vf[21] = out[1];
        ctx->vu0_acc = acc;
        ctx->vu0_clip_flags = flags;
        return v0;
    }

    uint32_t clipByMinMaxCube(R5900Context *ctx, const Ram m, uint32_t lo, uint32_t hi, uint32_t mat)
    {
        static sotc_decomp::NativeCount count("clipByMinMaxCube");
        count.hit();
        if (sotc_decomp::fastNativeOn())
            return clipByMinMaxCubeFast(ctx, m, lo, hi, mat);
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
            const uint32_t j = ps2VuClipFlags(p, p);
            flags = ((flags << 6) | j) & 0xFFFFFFu;
            sotc_decomp::clipShift(ctx, j); // round 61
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
    // Round 49: Fast (SOTC_FASTNATIVE) the lambdas inline (finish was an
    // out-of-line call reading every capture through a closure built on the
    // stack each call) and the root box's camera x model product with each
    // operand clamped once (ps2VuMulK / AddK, see clipByMinMaxCubeFast); the
    // same operations, values and stores otherwise.
    template <bool Fast>
    void modelClipSolidT(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xB0u;
        const uint32_t s2 = GPR_U32(ctx, 4), s3 = GPR_U32(ctx, 5);
        if (!Ram::ok(sp, 0xB0u) || !Ram::ok(s2, 0x260u) || !Ram::ok(s3, 0x40u) || !Ram::ok(m.u32(s2), 0x24u))
            return fallback(rdram, ctx, runtime, kClipSolid);
        // Round 28: the model matrix's two lines, read last (the root box's
        // product), asked for now: they missed the cache there.
        lines67::pref(s3), lines67::pref(s3 + 0x20u);
        __builtin_prefetch(rdram + s3);
        __builtin_prefetch(rdram + s3 + 0x20u);
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
        auto finish = [&](float f0, uint64_t v0) __attribute__((always_inline))
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
        auto distance = [&](__m128 vf5, __m128 vf6) __attribute__((always_inline)) -> float
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
        // Round 36, "Detalhe reduzido" (Tuning::sacLod): the level picked as
        // if the model were 1.6x as far, for records that keep their last
        // level past their range (+0x28 clear: nothing vanishes sooner,
        // terrain included; it is drawn coarser).
        if (native_renderer::tuning().sacLod && m.s32(a0 + 0x28u) == 0)
            f1 = f1 * 1.6f;
        // Round 65, "Shorter draw distance" (Tuning::sacDrawDist): for the
        // records that drop the model past their last level (+0x28 set:
        // never terrain, whose records keep their last level) the level is
        // picked as if 1.5x as far, so the model goes at 2/3 of its range.
        if (native_renderer::tuning().sacDrawDist && m.s32(a0 + 0x28u) != 0)
            f1 = f1 * 1.5f;
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
        if constexpr (Fast)
        {
            // apply()'s operations, each operand's clamp once: the camera rows
            // and each model row (its lanes), the products' and sums' results
            // as they are. Each row read before the previous product's store,
            // as the old loop does.
            const __m128 n0 = PS2X_VU_IN(cam[0]), n1 = PS2X_VU_IN(cam[1]), n2 = PS2X_VU_IN(cam[2]), n3 = PS2X_VU_IN(cam[3]);
            for (int i = 0; i < 4; ++i)
            {
                const __m128 r = PS2X_VU_IN(m.q(s3 + static_cast<uint32_t>(i) * 0x10u));
                acc = PS2_VMUL_K(1, 1, n0, lane<0>(r));
                acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n1, lane<1>(r)));
                acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n2, lane<2>(r)));
                out[i] = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n3, lane<3>(r)));
                m.putq(sp + 0x30u + static_cast<uint32_t>(i) * 0x10u, out[i]);
            }
        }
        else
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
    void modelClipSolid(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("modelClipSolid");
        count.hit();
        const lines67::Scope l67(lines67::kClip, false, GPR_U32(ctx, 29));
        if (sotc_decomp::fastNativeOn())
            return modelClipSolidT<true>(rdram, ctx, runtime);
        modelClipSolidT<false>(rdram, ctx, runtime);
    }


    // ---- Round 83 (SOTC_WALK83): ClipLayoutObjModel for a solid model as plain A9 code ----
    // The same decisions as ClipLayoutObjModel -> modelClipSolid ->
    // modelClipPoint / clipByMinMaxCube above, without their guest calls
    // (the camera context read from its globals, no guest frame, no VU0
    // registers kept, plain float maths: the results may differ from the
    // PS2's in the last bits). The outputs the walk and the draw read: the
    // sort record's level (+0x30, -1 none, -2 culled), fade (+0x34) and depth
    // keys (+0x20..+0x28), the instance's LOD scale (+0x250), level bits
    // (bits 40-42 of +0x250) and view depths (+0x230..+0x238); v0 the
    // instance, f0 its +0x234. Not here (the code above): skinned models, a
    // model that picks its own level (+0xc of its header), an unknown level
    // mode, anything outside main RAM. SOTC_WALK83_CHECK=2 runs the code above
    // after it and compares ([walk83c] lines).
    namespace clip83
    {
        struct V4
        {
            float x, y, z, w;
        };
        inline const uint8_t *host(const uint8_t *rdram, uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a ? rdram + a : nullptr; }
        inline float hf(const uint8_t *p, uint32_t o)
        {
            float v;
            std::memcpy(&v, p + o, 4);
            return v;
        }
        inline uint32_t hu(const uint8_t *p, uint32_t o)
        {
            uint32_t v;
            std::memcpy(&v, p + o, 4);
            return v;
        }
        inline V4 hq(const uint8_t *p, uint32_t o)
        {
            V4 v;
            std::memcpy(&v, p + (o & ~0xFu), 16);
            return v;
        }
        // r0 x v.x + r1 x v.y + r2 x v.z + r3 x w (the rows as the VU's vmula chain takes them).
        inline V4 rows(const V4 r[4], float x, float y, float z, float w)
        {
#if defined(__ARM_NEON)
            float32x4_t a = vmulq_n_f32(vld1q_f32(&r[3].x), w);
            a = vmlaq_n_f32(a, vld1q_f32(&r[0].x), x);
            a = vmlaq_n_f32(a, vld1q_f32(&r[1].x), y);
            a = vmlaq_n_f32(a, vld1q_f32(&r[2].x), z);
            V4 o;
            vst1q_f32(&o.x, a);
            return o;
#else
            return {r[0].x * x + r[1].x * y + r[2].x * z + r[3].x * w, r[0].y * x + r[1].y * y + r[2].y * z + r[3].y * w,
                    r[0].z * x + r[1].z * y + r[2].z * z + r[3].z * w, r[0].w * x + r[1].w * y + r[2].w * z + r[3].w * w};
#endif
        }
        inline void load4(const uint8_t *p, uint32_t o, V4 r[4])
        {
            std::memcpy(r, p + o, 64);
        }
        // The VU CLIP judgement's six bits (+x, -x, +y, -y, +z, -z against |w|).
        inline uint32_t clipBits(const V4 &p)
        {
            const float w = std::fabs(p.w);
            return (p.x > w ? 1u : 0u) | (p.x < -w ? 2u : 0u) | (p.y > w ? 4u : 0u) | (p.y < -w ? 8u : 0u) | (p.z > w ? 16u : 0u) | (p.z < -w ? 32u : 0u);
        }
        // The root box's eight corners through PM: true when all of them are
        // out by one same plane (the AND of their CLIP bits is not 0).
        inline bool boxOut(const V4 PM[4], const V4 &lo, const V4 &hi)
        {
#if defined(__ARM_NEON)
            // Corners k = 0..3 have x = lo.x, k = 4..7 x = hi.x; y = (k & 2) ? hi : lo, z = (k & 1) ? hi : lo.
            const float32x4_t ys = {lo.y, lo.y, hi.y, hi.y}, zs = {lo.z, hi.z, lo.z, hi.z};
            float32x4_t cA[4], cB[4];
            const float *pm = &PM[0].x;
            for (int c = 0; c < 4; ++c)
            {
                float32x4_t t = vdupq_n_f32(pm[12 + c]);
                t = vmlaq_n_f32(t, ys, pm[4 + c]);
                t = vmlaq_n_f32(t, zs, pm[8 + c]);
                cA[c] = vmlaq_n_f32(t, vdupq_n_f32(lo.x), pm[c]);
                cB[c] = vmlaq_n_f32(t, vdupq_n_f32(hi.x), pm[c]);
            }
            const float32x4_t wA = vabsq_f32(cA[3]), wB = vabsq_f32(cB[3]);
            const float32x4_t nwA = vnegq_f32(wA), nwB = vnegq_f32(wB);
            auto all = [](uint32x4_t a, uint32x4_t b)
            {
                const uint32x4_t m = vandq_u32(a, b);
                uint32x2_t h = vand_u32(vget_low_u32(m), vget_high_u32(m));
                h = vpmin_u32(h, h);
                return vget_lane_u32(h, 0) != 0u;
            };
            for (int c = 0; c < 3; ++c)
                if (all(vcgtq_f32(cA[c], wA), vcgtq_f32(cB[c], wB)) || all(vcltq_f32(cA[c], nwA), vcltq_f32(cB[c], nwB)))
                    return true;
            return false;
#else
            uint32_t a = 0x3Fu;
            for (int k = 0; k < 8 && a; ++k)
                a &= clipBits(rows(PM, (k & 4) ? hi.x : lo.x, (k & 2) ? hi.y : lo.y, (k & 1) ? hi.z : lo.z, 1.0f));
            return a != 0u;
#endif
        }
        uint64_t g_calls = 0, g_fast = 0, g_checks = 0, g_bad = 0, g_frames = 0;
        int checkMode()
        {
            static const int v = ps2_diag::env("SOTC_WALK83_CHECK") ? std::atoi(ps2_diag::env("SOTC_WALK83_CHECK")) : 0;
            return v;
        }
        struct Out
        {
            uint32_t object, geo;
            float f0;
        };
        // True when done (out set); false: the code above, nothing written.
        bool clip(uint8_t *rdram, uint32_t object, Out &out)
        {
            ++g_calls;
            const uint8_t *ob = host(rdram, object, 0x10u);
            if (!ob)
                return false;
            const uint32_t index = static_cast<uint32_t>(ob[8]) - 9u;
            if (index >= 0xEu)
                return false;
            const uint32_t target = hu(rdram, kClassTable + index * 4u);
            const uint32_t s0 = target == 0x010F1A58u ? object + 0x20u : target == 0x010F1A98u ? object + 0x90u : 0u;
            const uint8_t *c0 = s0 ? host(rdram, s0, 0x170u) : nullptr;
            if (!c0)
                return false;
            const uint32_t s1 = hu(c0, 0x140u), geo = hu(c0, 0x144u);
            uint8_t *rec = const_cast<uint8_t *>(host(rdram, s1, 0x40u));
            if (!rec)
                return false;
            if (geo == 0u)
            {
                const float m1 = -1.0f;
                std::memcpy(rec + 0x30u, &m1, 4);
                out = {object, 0u, -1.0f};
                return true;
            }
            uint8_t *gp = const_cast<uint8_t *>(host(rdram, geo, 0x260u));
            if (!gp)
                return false;
            const uint8_t *model = host(rdram, hu(gp, 0u), 0x24u);
            const uint8_t *hd = model ? host(rdram, hu(model, 0x20u), 0x60u) : nullptr;
            if (!hd || (hu(hd, 0x18u) & 1u) != 0u || hu(hd, 0xCu) != 0u)
                return false; // skinned, or a model that picks its own level
            const uint8_t *lod = host(rdram, hu(hd, 8u), 0x40u);
            const uint8_t *mx = host(rdram, hu(c0, 0x15Cu), 0x40u);
            const uint32_t cam = hu(rdram, 0x0122A8F0u + (hu(rdram, 0x0118FE4Cu) << 2));
            const uint8_t *cp = host(rdram, cam, 0x5E0u);
            if (hu(hd, 8u) == 0u || !lod || !mx || !cp)
                return false;
            const int32_t count = static_cast<int32_t>(hu(hd, 0x1Cu));
            if (count <= 0 || count > 8)
                return false;
            const uint32_t mode = hu(lod, 0x14u);
            uint32_t modeTarget = 0u;
            if (mode < 5u)
            {
                modeTarget = hu(rdram, kLodModes + mode * 4u);
                if (modeTarget != 0x01098968u && modeTarget != 0x01098970u && modeTarget != 0x01098978u && modeTarget != 0x01098980u && modeTarget != 0x010989D0u)
                    return false;
            }
            ++g_fast;
            // ClipLayoutObjModel.
            const float m1 = -1.0f;
            std::memcpy(rec + 0x30u, &m1, 4);
            const float fade = hf(gp, 0x244u);
            std::memcpy(rec + 0x34u, &fade, 4);
            if (!(fade < 1.0f))
            {
                out = {object, geo, fade};
                return true;
            }
            float lens = 1.0f;
            if (hu(lod, 0x3Cu) != 0u && cam != 0u)
                lens = 500.0f / hf(cp, 0xCu);
            const float lodScale = hf(c0, 0x9Cu) * lens;
            std::memcpy(gp + 0x250u, &lodScale, 4);
            // modelClipSolid.
            float level;
            const V4 eye = hq(cp, 0x140u);
            V4 M[4];
            load4(mx, 0u, M);
            auto dist3 = [](float x, float y, float z) { return std::sqrt(x * x + y * y + z * z); };
            bool done = false;
            if ((hu(hd, 0x18u) >> 24) & 1u)
            {
                level = -2.0f;
                done = true;
            }
            if (!done && hu(rdram, kLodOn) != 0u)
            {
                const float far = hf(lod, 0x30u);
                if (0.0f < far && dist3(M[3].x - eye.x, M[3].y - eye.y, M[3].z - eye.z) < far)
                {
                    level = -2.0f;
                    done = true;
                }
            }
            if (!done)
            {
                // modelClipPoint: the sphere's centre in the world, its view depths, the CLIP of the
                // centre with w grown by the radius's projected size.
                const V4 c = hq(hd, 0x20u);
                const float r = c.w * lodScale;
                const V4 world = rows(M, c.x, c.y, c.z, 1.0f);
                V4 P[4], VP[4], V[4];
                load4(cp, 0x1E0u, P);
                load4(cp, 0x5A0u, VP);
                load4(cp, 0x1A0u, V);
                const V4 pr = rows(P, r, 0.0f, 0.0f, 1.0f);
                V4 clipP = rows(VP, world.x, world.y, world.z, 1.0f);
                clipP.w += pr.x;
                const bool crosses = clipBits(clipP) != 0u;
                const V4 vc = rows(V, world.x, world.y, world.z, world.w); // V x (M x c): the centre in view space
                const float keys[4] = {vc.z + r, vc.z, vc.z - r, vc.w};
                std::memcpy(gp + 0x230u, keys, 16);
                const float f21 = crosses ? dist3(world.x - eye.x, world.y - eye.y, world.z - eye.z) : -1.0f;
                float d = 0.0f;
                switch (modeTarget)
                {
                case 0x01098968u: d = keys[1]; break;
                case 0x01098970u: d = keys[2]; break;
                case 0x01098978u: d = keys[0]; break;
                case 0x01098980u: d = dist3(world.x - eye.x, 0.0f, world.z - eye.z); break;
                case 0x010989D0u: d = dist3(world.x - eye.x, world.y - eye.y, world.z - eye.z); break;
                default: d = 0.0f; break;
                }
                const bool keepsLast = static_cast<int32_t>(hu(lod, 0x28u)) == 0;
                if (native_renderer::tuning().sacLod && keepsLast)
                    d *= 1.6f;
                if (native_renderer::tuning().sacDrawDist && !keepsLast)
                    d *= 1.5f;
                uint64_t w250;
                std::memcpy(&w250, gp + 0x250u, 8);
                const Ram m{rdram};
                float f12 = lodByDistance(m, hu(hd, 8u), d, static_cast<int32_t>(static_cast<int64_t>(w250 << 21) >> 61));
                const int32_t lv = static_cast<int32_t>(f12);
                float f20;
                if (static_cast<uint64_t>(static_cast<int64_t>(lv)) < static_cast<uint64_t>(static_cast<int64_t>(count)))
                    f20 = f12;
                else
                    f20 = keepsLast ? static_cast<float>(count - 1) : -1.0f;
                if (f20 < 0.0f)
                    level = -1.0f;
                else if (f21 < 0.0f)
                    level = f20;
                else
                {
                    // The level's root box through camera x model: culled when every corner is out by one same plane.
                    const uint32_t table = hu(hd, 0x30u + static_cast<uint32_t>(static_cast<int32_t>(f20)) * 4u);
                    const uint8_t *tb = host(rdram, table, 0x20u);
                    level = f20;
                    if (tb)
                    {
                        V4 PM[4];
                        for (int i = 0; i < 4; ++i)
                            PM[i] = rows(VP, M[i].x, M[i].y, M[i].z, M[i].w);
                        if (boxOut(PM, hq(tb, 0u), hq(tb, 0x10u)))
                            level = -2.0f;
                    }
                }
            }
            // The level into bits 40-42 of +0x250, the sort record's keys.
            uint64_t w250;
            std::memcpy(&w250, gp + 0x250u, 8);
            const int32_t lvl = static_cast<int32_t>(level);
            w250 = (w250 & 0xFFFFF8FFFFFFFFFFull) | (static_cast<uint64_t>(static_cast<uint32_t>(lvl) & 7u) << 40);
            std::memcpy(gp + 0x250u, &w250, 8);
            const float lf = static_cast<float>(static_cast<int32_t>(static_cast<int64_t>(w250 << 21) >> 61));
            std::memcpy(rec + 0x30u, &lf, 4);
            std::memcpy(rec + 0x20u, gp + 0x230u, 4);
            std::memcpy(rec + 0x28u, gp + 0x238u, 4);
            std::memcpy(rec + 0x24u, gp + 0x234u, 4);
            out = {object, geo, hf(gp, 0x234u)};
            return true;
        }
    }

    void clipLayoutObjModelOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // ---- ClipLayoutObjModel (0x10f4890; a0 the object) ---------------------
    void clipLayoutObjModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const bool clipPart83 = !(std::getenv("SOTC_WALK83_CLIP") && std::getenv("SOTC_WALK83_CLIP")[0] == '0');
        if (clipPart83 && sotc_decomp::walk83On())
        {
            const uint32_t object = GPR_U32(ctx, 4);
            const int check = clip83::checkMode();
            // (checking: the inputs the old code reads back, as they were)
            uint8_t keep[0x28];
            const uint32_t s0c = object + 0x20u;
            uint32_t geoc = 0u, recc = 0u;
            if (check == 2 && Ram::ok(s0c, 0x170u))
            {
                geoc = Ram{rdram}.u32(s0c + 0x144u);
                recc = Ram{rdram}.u32(s0c + 0x140u);
                if (Ram::ok(geoc, 0x260u))
                    std::memcpy(keep, rdram + geoc + 0x230u, 0x28u);
                else
                    geoc = 0u;
            }
            clip83::Out o;
            if (clip83::clip(rdram, object, o))
            {
                if (check == 2 && geoc != 0u && geoc == o.geo && Ram::ok(recc, 0x40u))
                {
                    uint8_t fastGeo[0x28], fastRec[0x18];
                    std::memcpy(fastGeo, rdram + geoc + 0x230u, 0x28u);
                    std::memcpy(fastRec, rdram + recc + 0x20u, 0x18u);
                    std::memcpy(rdram + geoc + 0x230u, keep, 0x28u);
                    clipLayoutObjModelOld(rdram, ctx, runtime);
                    ++clip83::g_checks;
                    float a[6], b[6];
                    std::memcpy(a, fastRec, 0x18u);
                    std::memcpy(b, rdram + recc + 0x20u, 0x18u);
                    uint64_t fa, fb2;
                    std::memcpy(&fa, fastGeo + 0x20u, 8);
                    std::memcpy(&fb2, rdram + geoc + 0x250u, 8);
                    bool bad = a[4] != b[4] || ((fa >> 40) & 7u) != ((fb2 >> 40) & 7u);
                    for (int i = 0; i < 3; ++i)
                        bad |= !(std::fabs(a[i] - b[i]) <= 1e-3f * (1.0f + std::fabs(b[i])));
                    if (bad)
                    {
                        if (++clip83::g_bad <= 40u)
                            std::fprintf(stderr, "[walk83c] object %08x fast level %.0f keys %.4g %.4g %.4g | old level %.0f keys %.4g %.4g %.4g\n", object, a[4], a[0], a[1], a[2], b[4], b[0], b[1], b[2]);
                    }
                    if (clip83::g_checks % 20000u == 0u)
                        std::fprintf(stderr, "[walk83c] checks %llu bad %llu (fast %llu of %llu)\n", (unsigned long long)clip83::g_checks, (unsigned long long)clip83::g_bad,
                                     (unsigned long long)clip83::g_fast, (unsigned long long)clip83::g_calls);
                    return;
                }
                ctx->f[0] = o.f0;
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.geo));
                returnTo(ctx);
                return;
            }
        }
        clipLayoutObjModelOld(rdram, ctx, runtime);
    }
    void clipLayoutObjModelOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x20u;
        const uint32_t object = GPR_U32(ctx, 4);
        if (!Ram::ok(sp, 0x20u) || !Ram::ok(object, 0x10u))
            return fallback(rdram, ctx, runtime, kClip);
        const lines67::Scope l67(lines67::kClip, true);
        lines67::base(object);
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
        const lines67::Scope l67(lines67::kDisp, false, GPR_U32(ctx, 29));
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
        // Round 103 (SOTC_SOLID103, A/B `solid103`, `r103all`): with the
        // native renderer drawing, the parts' matrix blocks (the camera's
        // +0x1a0 x each part's matrix, into the instance packet's +0x104
        // block) are the PS2's VU1 input only: the model hook draws from the
        // parts' own matrices (a1) and the camera's +0x5a0, and nothing
        // native reads the blocks. Left out (with their camera lookup) for a
        // model without a shadow (shadowDisp's rigid matrix comes from its
        // packets) whose LOD record asks for no lens scale (which rescales
        // the blocks). Everything else as before.
        const auto lensScaled = [&]
        {
            const uint32_t lodRecord = m.u32(m.u32(m.u32(m.u32(s1 + 0x144u)) + 0x20u) + 8u);
            return lodRecord != 0u && m.u32(lodRecord + 0x3Cu) != 0u;
        };
        if (s0 == 0u && sotc_decomp::solid103On() && sotc_native::nativeDraws() && m.u32(m.u32(s1 + 0x144u) + 0x280u) == 0u && !lensScaled())
        {
            const int32_t s2 = m.s16(s1 + 0x1B4u);
            SET_GPR_S32(ctx, 18, s2);
            ctx->f[0] = 1.0f;
            // SOTC_SOLID103_POISON=1 (checking): the skipped blocks filled with
            // NaNs, so any reader left would show (a model gone or broken).
            static const bool poison = std::getenv("SOTC_SOLID103_POISON") && std::atoi(std::getenv("SOTC_SOLID103_POISON")) != 0;
            if (poison)
            {
                const __m128 nan = _mm_set1_ps(std::numeric_limits<float>::quiet_NaN());
                for (int32_t i = 0; i < s2 && i < 256; ++i)
                    for (uint32_t k = 0; k < 4u; ++k)
                        m.putq(block(i) + k * 0x10u, nan);
            }
        }
        else if (s0 == 0u)
        {
            const int32_t s2 = m.s16(s1 + 0x1B4u);
            SET_GPR_S32(ctx, 18, s2);
            g.call(kGetCameraContext, 0x010F3A2Cu, sp);
            const uint32_t t0 = g.v0() + 0x1A0u;
            uint32_t a3 = m.u32(s1 + 0x15Cu);
            if (0 < s2)
            {
                // Round 67 (SOTC_WALK67): the parts' source matrices (64
                // bytes each, in a row) and their blocks asked for four parts
                // ahead of the products.
                const bool w67 = sotc_decomp::walk67On();
                auto askPart = [&](int32_t i, uint32_t src)
                {
                    ask(m, src);
                    ask(m, src + 0x20u);
                    const uint32_t d = block(i);
                    ask(m, d);
                    ask(m, d + 0x20u);
                };
                if (w67)
                    for (int32_t i = 0; i < 4 && i < s2; ++i)
                        askPart(i, a3 + static_cast<uint32_t>(i) * 0x40u);
                for (int32_t i = 0; i < s2; ++i, a3 += 0x40u)
                {
                    if (w67 && i + 4 < s2)
                        askPart(i + 4, a3 + 0x100u);
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
    // Round 116 (SOTC_GLV116_LOG=1, diagnostic): the kick's callers (its return
    // address) and the gl* layer, counted; a [glv116] line every 30 000 kicks.
    void glv116Note(uint32_t ra)
    {
        static std::unordered_map<uint32_t, uint64_t> calls;
        static uint64_t total = 0;
        ++calls[ra];
        if (++total % 30000u != 0u)
            return;
        std::vector<std::pair<uint64_t, uint32_t>> v;
        for (const auto &kv : calls)
            v.push_back({kv.second, kv.first});
        std::sort(v.rbegin(), v.rend());
        std::string line = "[glv116] kicks " + std::to_string(total) + " by caller:";
        for (size_t i = 0; i < v.size() && i < 12u; ++i)
        {
            char b[48];
            std::snprintf(b, sizeof b, " %08x %llu", v[i].second, (unsigned long long)v[i].first);
            line += b;
        }
        std::fprintf(stderr, "%s\n", line.c_str());
    }
    void glVertexKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const bool glv116 = ps2_diag::env("SOTC_GLV116_LOG") != nullptr;
        if (glv116)
            glv116Note(GPR_U32(ctx, 31));
        sotc_decomp::clipNeeded(ctx); // round 61: it reads the whole CLIP register (left in s0)
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
            // Round 67 (SOTC_WALK67): the GIF packet's next lines (a vertex
            // writes up to 48 bytes there) asked for a vertex or two ahead of
            // their stores.
            if (sotc_decomp::walk67On())
            {
                const uint32_t out = m.u32(kGl + 4u);
                ask(m, out + 0x40u);
                ask(m, out + 0x60u);
            }
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

    // ---- Round 117 (SOTC_GLV117, default on; A/B `glv117`, `r117all`): the
    // strips' four vertices between a glBegin and its glEnd in one pass.
    // Each vertex is glColor4f, glTexCoord2f, glVertex3fv's own part and the
    // kick (0108D730), as above and in their order, writing what they write
    // (the colour / ST words, the ring slot, the state words, the GIF pairs,
    // the CLIP history, +0x1b4, the ADC byte, the ring index); but the three
    // matrices (the view on top of the stack, the camera's clip +0x5e0 and
    // screen +0x660) are read and clamped once for the four (the VU0 add /
    // mul clamp is idempotent: the products and sums are the same bits as
    // the kick's, which clamps every operand), the kick's frame and saved
    // registers are not written (dead stack: the kick restores every
    // s-register it uses), and the VU0 registers, Q, f0 - f2, v0 / a0 / ra
    // are set once, as the fourth kick leaves them. False (nothing written)
    // when the kick would take another path for any of them (no camera, an
    // untransformed vertex, UV texturing, an unknown primitive, a full
    // ring): then the vertices go through the calls as before.
    // SOTC_GLV117_CHECK=<n>: every n-th quad run both ways from the same
    // state and compared ([glv117c]: the gl block, the packet, VU0, f0 - f2).
    struct Vert117
    {
        float alpha, u, v;
        uint32_t pos, site;
        bool colour = true; // false: no glColor4f per vertex (glColor4fv once before: 01142350's quads)
    };
    // host (round 128, imm128): the pairs into host[] (value, register; *hostCount of them) instead of the GIF packet.
    bool quad117(const G g, uint32_t sp, const Vert117 *vs, int n, uint64_t *host = nullptr, uint32_t *hostCount = nullptr)
    {
        const Ram m = g.m;
        R5900Context *ctx = g.ctx;
        const uint32_t camera = m.u32(kGl);
        const uint32_t index0 = m.u32(kGl + 0x41F0u);
        static int said = 0;
        auto no = [&](int why)
        {
            if (said++ < 8)
                std::fprintf(stderr, "[glv117] quad by the calls: reason %d (camera %08x index %u view %u prim %u texturing %u mode %u)\n", why, camera, index0,
                             m.u32(kViewIndex), m.u32(kGl + 0x10u), m.u32(kGl + 0x18u), m.u32(kGl + 0x2Cu));
            return false;
        };
        if (camera == 0u || !Ram::ok(camera, 0x6A0u) || index0 + static_cast<uint32_t>(n) >= 0x100u || m.u32(kViewIndex) >= 0x20u)
            return no(1);
        // UV texturing (mode 1): iosFTOI4Vector on the slot's ST (0x1be700, a relocated jal: checked).
        const uint32_t uvMode = m.u32(kGl + 0x18u) != 0u ? m.u32(kGl + 0x2Cu) : 0u;
        if (uvMode > 1u || (uvMode == 1u && sotc_decomp::jalTarget(g.rdram, 0x0108D9B0u) != 0x001BE700u))
            return no(2);
        const uint32_t prim = m.u32(kGl + 0x10u);
        const uint32_t target = prim < 7u ? m.u32(0x01191730u + prim * 4u) : 0x0108DC48u;
        if (target != 0x0108DAE0u && target != 0x0108DAF8u && target != 0x0108DB58u && target != 0x0108DB78u && target != 0x0108DC48u)
            return no(3);
        for (int i = 0; i < n; ++i)
            if (!Ram::ok(vs[i].pos, 0x10u))
                return no(4);
        sotc_decomp::clipNeeded(ctx);
        static const bool glv116 = ps2_diag::env("SOTC_GLV116_LOG") != nullptr;
        __m128 *vf = ctx->vu0_vf;
        const __m128 vf0 = vf[0];
        const uint32_t view = kViewStack + (m.u32(kViewIndex) << 6);
        const __m128 viewM[4] = {m.q(view), m.q(view + 0x10u), m.q(view + 0x20u), m.q(view + 0x30u)};
        const __m128 clipM[4] = {m.q(camera + 0x5E0u), m.q(camera + 0x5F0u), m.q(camera + 0x600u), m.q(camera + 0x610u)};
        const __m128 screenM[4] = {m.q(camera + 0x660u), m.q(camera + 0x670u), m.q(camera + 0x680u), m.q(camera + 0x690u)};
        __m128 viewN[4], clipN[4], screenN[4];
        for (int i = 0; i < 4; ++i)
        {
            viewN[i] = PS2X_VU_IN(viewM[i]);
            clipN[i] = PS2X_VU_IN(clipM[i]);
            screenN[i] = PS2X_VU_IN(screenM[i]);
        }
        // apply() on clamped rows and a clamped vector: each product / sum the kick's.
        auto applyN = [](const __m128 c[4], __m128 v, __m128 &acc) -> __m128
        {
            const __m128 vn = PS2X_VU_IN(v);
            acc = PS2_VMUL_K(1, 1, c[0], lane<0>(vn));
            acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, c[1], lane<1>(vn)));
            acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, c[2], lane<2>(vn)));
            return PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, c[3], lane<3>(vn)));
        };
        const bool perspective = m.u32(kGl + 0x5Cu) != 0u;
        const bool texturing = m.u32(kGl + 0x18u) != 0u;
        __m128 vf5 = vf[5], vf6 = vf[6];
        __m128 vf10 = vf[10], vf23 = vf[23], vf24 = vf[24], vf25 = vf[25], vf26 = vf[26], vf27 = vf[27], acc = ctx->vu0_acc;
        float f0 = 0.0f, f1 = 0.0f, f2 = 0.0f;
        for (int i = 0; i < n; ++i)
        {
            const Vert117 &vx = vs[i];
            if (glv116)
                glv116Note(vx.site + 8u);
            if (vx.colour)
                glColor4f(ctx, m, 0.5f, 0.5f, 0.5f, vx.alpha);
            glTexCoord2f(ctx, m, vx.u, vx.v); // (glTexCoord2fv writes the same words)
            // glVertex3fv's own part (the lit colour never: bit 0 is cleared first).
            const uint64_t state = (m.u64(kGl + 0x1B0u) & ~9ull) | 8u;
            m.put64(kGl + 0x1B0u, state);
            const uint32_t index = m.u32(kGl + 0x41F0u);
            const uint32_t slot = kGl + (index << 6);
            m.putqi(slot + 0x220u, m.qi(kGl + 0x90u));
            if (texturing)
            {
                m.putf(slot + 0x210u, m.f32(kGl + 0xA0u));
                m.putf(slot + 0x214u, m.f32(kGl + 0xA4u));
            }
            m.put64(kGl + 0x1B0u, m.u64(kGl + 0x1B0u) & ~1ull);
            m.putqi(slot + 0x200u, m.qi(vx.pos));
            // The kick (transformed: bit 3 was just set).
            vf23 = m.q(slot + 0x200u);
            vf24 = m.q(slot + 0x210u);
            vf25 = m.q(slot + 0x220u);
            vf10 = applyN(viewN, vf23, acc);
            vf27 = applyN(screenN, vf10, acc);
            vf26 = applyN(clipN, vf10, acc);
            ctx->vu0_q = ps2VuDivide(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf27)));
            vf25 = ftoi(vf25, 1.0f);
            const uint32_t flags = ((ctx->vu0_clip_flags << 6) | ps2VuClipFlags(vf26, vf26)) & 0xFFFFFFu;
            ctx->vu0_clip_flags = flags;
            const __m128 q = _mm_set1_ps(ctx->vu0_q);
            vf27 = sel(vf27, PS2_VMUL(vf27, q), kXyz);
            vf27 = sel(vf27, PS2_VMUL(vf0, q), kW);
            if (perspective)
                vf24 = sel(vf24, PS2_VMUL(vf24, q), kXyz);
            const uint32_t s0 = flags;
            vf27 = sel(vf27, ftoi(vf27, 16.0f), kXyz);
            m.putq(slot + 0x1F0u, vf26);
            uint32_t w24[4], w25[4], w27[4];
            std::memcpy(w24, &vf24, 16);
            std::memcpy(w25, &vf25, 16);
            std::memcpy(w27, &vf27, 16);
            auto emit = [&](uint64_t value, uint64_t reg)
            {
                if (host)
                {
                    host[2u * *hostCount] = value;
                    host[2u * *hostCount + 1u] = reg;
                    ++*hostCount;
                    return;
                }
                uint32_t at = m.u32(kGl + 4u);
                m.put64(at, value);
                at += 8u;
                m.put32(kGl + 4u, at);
                m.put64(at, reg);
                m.put32(kGl + 4u, at + 8u);
            };
            auto sx = [](uint32_t v) -> uint64_t { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); };
            if (texturing && uvMode == 0u)
                emit(static_cast<uint64_t>(w24[0]) | (static_cast<uint64_t>(w24[1]) << 32), 2u); // ST
            else if (texturing)
            {
                // UV: iosFTOI4Vector(the frame, the slot's ST): vftoi4 of the raw ST.
                vf5 = m.q(slot + 0x210u);
                vf6 = ftoi(vf5, 16.0f);
                uint32_t uv[4];
                std::memcpy(uv, &vf6, 16);
                emit(static_cast<uint64_t>(uv[0]) | (static_cast<uint64_t>(uv[1]) << 16), 3u);
            }
            {
                const uint64_t q32 = perspective ? (sx(w27[3]) << 32) : 0x3F80000000000000ull;
                const uint64_t value = (sx(w25[0]) | q32) | ((sx(w25[2]) << 16) | (sx(w25[1]) << 8)) | (sx(w25[3]) << 24);
                emit(value, 1u);
            }
            uint32_t s2 = 0u, s5 = 0u;
            const int32_t count = static_cast<int32_t>(index);
            auto pairs = [&](const uint32_t *masks, int k) -> bool
            {
                for (int j = 0; j < k; ++j)
                    if ((s0 & masks[j]) == masks[j])
                        return true;
                return false;
            };
            switch (target)
            {
            case 0x0108DAE0u:
                if (s0 & 0x3Fu)
                    s2 = 1u;
                break;
            case 0x0108DAF8u:
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
            case 0x0108DB58u:
                if (count > 0 && (s0 & 0xFFFu) != 0u)
                    s2 = 1u;
                break;
            case 0x0108DB78u:
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
            default: // 0x0108DC48: triangles / fans
            {
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
            const int32_t pending = m.s32(kGl + 0x1B4u);
            s2 |= (0 < pending) ? 1u : 0u;
            uint64_t reg = 4u;
            if (s2 != 0u)
            {
                if (pending > 0)
                    m.put32(kGl + 0x1B4u, static_cast<uint32_t>(pending - 1));
                reg = 0xCu;
            }
            f1 = m.f32(kGl + 0x41F4u);
            f2 = 2147483648.0f;
            f0 = FPU_MUL_S(FPU_CVT_S_W(static_cast<int32_t>(w27[2]) >> 4), f1);
            uint64_t z;
            if (FPU_C_OLE_S(f2, f0))
            {
                f0 = FPU_SUB_S(f0, f2);
                z = static_cast<uint64_t>(static_cast<int64_t>(FPU_CVT_W_S(f0))) | 0xFFFFFFFF80000000ull;
            }
            else
                z = static_cast<uint64_t>(static_cast<int64_t>(FPU_CVT_W_S(f0)));
            {
                const int32_t zi = static_cast<int32_t>(z);
                std::memcpy(&f1, &zi, 4);
            }
            const uint64_t a3v = sx(w27[0]) | (static_cast<uint64_t>(m.u32(kGl + 0x60u)) << 56);
            const uint64_t xyz = a3v | ((z << 32) | (sx(w27[1]) << 16));
            emit(xyz, reg);
            m.put8(kGl + 0x41F8u + index, static_cast<uint8_t>(s5));
            m.put32(kGl + 0x41F0u, index + 1u);
        }
        // What the last kick leaves.
        vf[5] = vf5;
        vf[6] = vf6;
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
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        const uint32_t last = m.u32(kGl + 0x41F0u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(last));
        SET_GPR_U32(ctx, 4, last - 1u);
        SET_GPR_U32(ctx, 31, vs[n - 1].site + 8u);
        SET_GPR_U32(ctx, 29, sp);
        return true;
    }

    // SOTC_GLV117_CHECK=<n>: every n-th quad both ways (glv117c).
    struct Glv117Check
    {
        int every = -1;
        uint64_t quads = 0, checks = 0, differ = 0;
    };
    Glv117Check g_glv117Check;

    // quad117, or the calls when it does not take them; SOTC_GLV117_CHECK=<n>:
    // every n-th quad both ways from the same state (the gl block 0x5300 bytes,
    // the packet's next 0x100, VU0 / Q / CLIP, f0 - f2), the calls' result kept.
    template <class Calls>
    void quadRun117(const G g, uint32_t sp, const Vert117 *vs, int n, const Calls &calls)
    {
        const Ram m = g.m;
        R5900Context *ctx = g.ctx;
        Glv117Check &c = g_glv117Check;
        if (c.every < 0)
            c.every = ps2_diag::env("SOTC_GLV117_CHECK") ? std::atoi(ps2_diag::env("SOTC_GLV117_CHECK")) : 0;
        const uint32_t pkt = m.u32(kGl + 4u);
        if (!(c.every > 0 && ++c.quads % static_cast<uint64_t>(c.every) == 0u) || !Ram::ok(pkt, 0x100u))
        {
            if (!quad117(g, sp, vs, n))
                calls();
            return;
        }
        // (static: nothing 16-aligned on the stack)
        static std::vector<uint8_t> blk0(0x5300u), pk0(0x100u), blkA(0x5300u), pkA(0x100u), ctx0(sizeof(R5900Context)), ctxA(sizeof(R5900Context));
        std::memcpy(blk0.data(), m.p(kGl), 0x5300u);
        std::memcpy(pk0.data(), m.p(pkt), 0x100u);
        std::memcpy(ctx0.data(), ctx, sizeof(R5900Context));
        if (!quad117(g, sp, vs, n))
        {
            calls();
            return;
        }
        std::memcpy(blkA.data(), m.p(kGl), 0x5300u);
        std::memcpy(pkA.data(), m.p(pkt), 0x100u);
        std::memcpy(ctxA.data(), ctx, sizeof(R5900Context));
        std::memcpy(m.p(kGl), blk0.data(), 0x5300u);
        std::memcpy(m.p(pkt), pk0.data(), 0x100u);
        std::memcpy(ctx, ctx0.data(), sizeof(R5900Context));
        calls();
        ++c.checks;
        const R5900Context *A = reinterpret_cast<const R5900Context *>(ctxA.data());
        const bool blkSame = std::memcmp(blkA.data(), m.p(kGl), 0x5300u) == 0, pkSame = std::memcmp(pkA.data(), m.p(pkt), 0x100u) == 0;
        const bool vuSame = std::memcmp(A->vu0_vf, ctx->vu0_vf, sizeof(ctx->vu0_vf)) == 0 && std::memcmp(&A->vu0_acc, &ctx->vu0_acc, 16) == 0 &&
                            std::memcmp(&A->vu0_q, &ctx->vu0_q, 4) == 0 && A->vu0_clip_flags == ctx->vu0_clip_flags;
        const bool fSame = std::memcmp(A->f, ctx->f, 3 * sizeof(float)) == 0;
        if (!(blkSame && pkSame && vuSame && fSame) && c.differ++ < 20u)
        {
            uint32_t first = 0x5300u;
            for (uint32_t i = 0; i < 0x5300u; ++i)
                if (blkA[i] != m.p(kGl)[i])
                {
                    first = i;
                    break;
                }
            std::fprintf(stderr, "[glv117c] DIFFERS (site %08x): gl block %d (first +%x) packet %d vu0 %d f %d\n", vs[0].site, blkSame ? 0 : 1, first, pkSame ? 0 : 1,
                         vuSame ? 0 : 1, fSame ? 0 : 1);
        }
        if (c.checks % 500u == 0u)
            std::fprintf(stderr, "[glv117c] %llu quads checked, %llu differ\n", (unsigned long long)c.checks, (unsigned long long)c.differ);
    }

    // ---- Round 128 (SOTC_IMM128, default on; A/B `imm128`, `r128all`): an
    // immediate-mode quad (glBegin, the four vertices, glEnd) without the
    // guest GIF packet when the native renderer draws. glEnd's hook only
    // reads the packet's pairs back into the 2D assembler; the rest of glEnd
    // (the GIF / DMA tags' counts, iosSetPacketPtrPath1, the chain into the
    // layer's list, the pop of glBegin's push) feeds the PS2's GS, which
    // draws nothing with the native renderer (GS::skipRaster: the chains are
    // walked for SIGNAL / FINISH / LABEL only, and this A+D packet has none;
    // round 103's ptcl103 leaves its packets out the same way). So: glBegin's
    // state words (begin128, below), quad117's pairs into host memory, the
    // pairs fed as glEndHook feeds them (the layer glBegin would have pushed,
    // the haze's screen copy; sotc_native::immFeed128). glEnd's 0108CD80
    // (the software clip of the primitives the kicks flagged, ADC bytes at
    // +0x41f8; its own glBegin .. glEnd) is called as glEnd calls it, after
    // the feed, when a flag is set; with none it does nothing. Not taken (the
    // calls as before): another renderer, a packet already open (glBegin
    // would return -1), an unknown primitive table entry, quad117 not taking
    // the vertices. SOTC_IMM128_CHECK=<n>: every n-th quad both ways from the
    // same state (the new path dry), the 2D assembler's input compared (the
    // layer, the texture header, every pair), the gl block (but its packet
    // pointers +4 / +8), VU0 and f0 - f2; the old path's result kept.
    struct Imm128Stats
    {
        int every = -1;
        uint64_t quads = 0, fast = 0, clipped = 0, byCalls = 0, checks = 0, differ = 0;
    };
    Imm128Stats g_imm128;

    // glBegin(prim) (0x108e530 -> glBeginWithDlLayerNo(prim, iosDlGetLayer()))
    // as its writes to the gl block, without the packet; pairs[0..3] its TEST
    // and PRIM pairs, *layer the layer it would push. False (nothing
    // written): a packet open, or a primitive / table entry not known.
    bool begin128(const Ram m, uint32_t prim, uint64_t *pairs, uint32_t *layer)
    {
        if (prim >= 8u || m.u32(kGl + 4u) != 0u)
            return false;
        uint32_t s4;
        switch (m.u32(0x011916E0u + prim * 4u))
        {
        case 0x0108BB98u: s4 = m.u32(kGl + 0x20u); break;
        case 0x0108BBA4u: case 0x0108BBA8u: s4 = m.u32(kGl + 0x24u); break;
        case 0x0108BBB0u: s4 = 0u; break;
        default: return false;
        }
        const uint32_t gsPrim = m.u32(0x011916C0u + prim * 4u);
        if (gsPrim >= 8u)
            return false;
        *layer = m.u32(0x002132D8u + 0x1Cu); // iosDlGetLayer: the display list's current layer
        m.put32(kGl + 0xCu, prim);
        m.put32(kGl + 0x41F0u, 0u);
        m.put32(kGl + 0x5220u, 0u);
        m.put32(kGl + 0x14u, 1u);
        m.put32(kGl + 0x10u, gsPrim);
        m.put32(kGl + 0x1B8u, 0u);
        m.put32(kGl + 0x1B4u, m.u32(0x011916A0u + gsPrim * 4u) - 1u);
        m.put64(kGl + 0x1B0u, m.u64(kGl + 0x1B0u) & ~0x10ull);
        auto lw = [&](uint32_t off) -> int64_t { return static_cast<int64_t>(m.s32(kGl + off)); };
        const int64_t test = (static_cast<int64_t>(static_cast<int32_t>(m.u32(kGl + 0x34u) + 1u)) << 17) | (lw(0x38u) != 0 ? 0x1164A : 0x11001);
        pairs[0] = static_cast<uint64_t>(test);
        pairs[1] = lw(0x30u) != 0 ? 0x48u : 0x47u;
        const int64_t primValue = lw(0x10u) | (lw(0x30u) << 9) | (lw(0x28u) << 5) | (static_cast<int64_t>(static_cast<int32_t>(s4)) << 7) | (lw(0x14u) << 3) |
                                  (lw(0x18u) << 4) | (lw(0x1Cu) << 6) | (lw(0x2Cu) << 8);
        pairs[2] = static_cast<uint64_t>(primValue);
        pairs[3] = 0u;
        return true;
    }

    // The quad without the packet (above); false: nothing written, take the calls.
    // dry (the check): the pairs made into *rec, nothing fed or clipped.
    bool imm128(const G g, uint32_t sp, const Vert117 *vs, int n, uint32_t prim, bool haze, std::vector<uint64_t> *rec)
    {
        const Ram m = g.m;
        uint64_t pairs[2u * (2u + 3u * 4u)];
        uint32_t layer = 0, count = 0;
        if (n > 4 || !begin128(m, prim, pairs, &layer))
            return false;
        if (!quad117(g, sp, vs, n, pairs + 4, &count))
            return false; // (begin128's words are glBegin's own: the calls write them again)
        count += 2u;
        if (rec)
        {
            sotc_native::immRecord128(*rec, pairs, count, layer, haze);
            return true;
        }
        sotc_native::immFeed128(g.rdram, pairs, count, layer, haze);
        bool clipped = false;
        for (int i = 0; i < n; ++i)
            clipped |= m.u8(kGl + 0x41F8u + static_cast<uint32_t>(i)) != 0u;
        if (clipped)
        {
            // glEnd's 0108CD80 (it sets +4 to 0 first: not open here either).
            ++g_imm128.clipped;
            g.call(0x0108CD80u, 0x0108D128u, sp);
        }
        SET_GPR_U32(g.ctx, 2, 0u);
        return true;
    }

    // glBegin(prim) at beginSite, body() (the vertices), glEnd at endSite: as
    // imm128 when it takes them, else as the calls.
    template <class Body>
    void prim128(const G g, uint32_t sp, const Vert117 *vs, int n, uint32_t prim, uint32_t beginSite, uint32_t endSite, bool haze, const Body &body)
    {
        Imm128Stats &st = g_imm128;
        if (st.every < 0)
            st.every = ps2_diag::env("SOTC_IMM128_CHECK") ? std::atoi(ps2_diag::env("SOTC_IMM128_CHECK")) : 0;
        const bool on = sotc_decomp::imm128On() && sotc_decomp::glv117On() && sotc_native::nativeDraws();
        ++st.quads;
        auto calls = [&]
        {
            g.a(4, prim);
            g.call(kGlBegin, beginSite, sp);
            body();
            g.call(kGlEnd, endSite, sp);
        };
        if (on && st.every > 0 && st.quads % static_cast<uint64_t>(st.every) == 0u)
        {
            const Ram m = g.m;
            R5900Context *ctx = g.ctx;
            static std::vector<uint8_t> blk0(0x5300u), blkN(0x5300u), ctx0(sizeof(R5900Context)), ctxN(sizeof(R5900Context));
            static std::vector<uint64_t> recN, recO;
            recN.clear();
            recO.clear();
            std::memcpy(blk0.data(), m.p(kGl), 0x5300u);
            std::memcpy(ctx0.data(), ctx, sizeof(R5900Context));
            const bool took = imm128(g, sp, vs, n, prim, haze, &recN);
            bool clipped = false;
            for (int i = 0; took && i < n; ++i)
                clipped |= m.u8(kGl + 0x41F8u + static_cast<uint32_t>(i)) != 0u;
            std::memcpy(blkN.data(), m.p(kGl), 0x5300u);
            std::memcpy(ctxN.data(), ctx, sizeof(R5900Context));
            std::memcpy(m.p(kGl), blk0.data(), 0x5300u);
            std::memcpy(ctx, ctx0.data(), sizeof(R5900Context));
            sotc_native::immArm128(&recO);
            calls();
            sotc_native::immArm128(nullptr);
            if (!took)
            {
                ++st.byCalls;
                return;
            }
            ++st.checks;
            // The gl block but its packet pointers (+4, +8): the old path leaves +8 at its packet.
            std::memcpy(blkN.data() + 4u, m.p(kGl) + 4u, 8u);
            const bool recSame = recN == recO;
            // With a clip flag 0108CD80 ran in the old path only (its work area, VU0): the record alone.
            const bool blkSame = clipped || std::memcmp(blkN.data(), m.p(kGl), 0x5300u) == 0;
            const R5900Context *N = reinterpret_cast<const R5900Context *>(ctxN.data());
            const bool vuSame = clipped || (std::memcmp(N->vu0_vf, ctx->vu0_vf, sizeof(ctx->vu0_vf)) == 0 && std::memcmp(&N->vu0_acc, &ctx->vu0_acc, 16) == 0 &&
                                            std::memcmp(&N->vu0_q, &ctx->vu0_q, 4) == 0 && N->vu0_clip_flags == ctx->vu0_clip_flags &&
                                            std::memcmp(N->f, ctx->f, 3 * sizeof(float)) == 0);
            if (!(recSame && blkSame && vuSame) && st.differ++ < 20u)
            {
                uint32_t first = 0x5300u;
                for (uint32_t i = 0; i < 0x5300u; ++i)
                    if (blkN[i] != m.p(kGl)[i])
                    {
                        first = i;
                        break;
                    }
                size_t at = 0;
                while (at < recN.size() && at < recO.size() && recN[at] == recO[at])
                    ++at;
                std::fprintf(stderr, "[imm128c] DIFFERS (site %08x): record %d (sizes %u / %u, first word %u: %016llx / %016llx) gl block %d (first +%x) vu0/f %d clipped %d\n",
                             beginSite, recSame ? 0 : 1, (unsigned)recN.size(), (unsigned)recO.size(), (unsigned)at, (unsigned long long)(at < recN.size() ? recN[at] : 0),
                             (unsigned long long)(at < recO.size() ? recO[at] : 0), blkSame ? 0 : 1, first, vuSame ? 0 : 1, clipped ? 1 : 0);
            }
            if (st.checks % 500u == 0u)
                std::fprintf(stderr, "[imm128c] %llu quads checked, %llu differ (quads %llu fast %llu clipped %llu by the calls %llu)\n", (unsigned long long)st.checks,
                             (unsigned long long)st.differ, (unsigned long long)st.quads, (unsigned long long)st.fast, (unsigned long long)st.clipped,
                             (unsigned long long)st.byCalls);
            return;
        }
        if (on && imm128(g, sp, vs, n, prim, haze, nullptr))
        {
            ++st.fast;
            static const bool log = ps2_diag::env("SOTC_IMM128_LOG") != nullptr;
            if (log && st.fast % 20000u == 0u)
                std::fprintf(stderr, "[imm128] quads %llu fast %llu clipped %llu\n", (unsigned long long)st.quads, (unsigned long long)st.fast,
                             (unsigned long long)st.clipped);
            return;
        }
        if (on)
            ++st.byCalls;
        calls();
    }

    // ---- Round 117 (glv117): manager_sub_01142350, one particle as a quad of
    // the gl* immediate API (the quads drawer 01141D20, a particle a call;
    // a0 its 48-byte record, a1 the size pair, a2 the world matrix). The
    // frame: the constant corner block (0x1195900) at sp, memset(sp + 0x80,
    // 0, 0x40) and the size pair into its corners, copied to sp + 0x40 (the
    // corners' ST); iosRotMatrixAxis(sp + 0xc0, the record's axis +0x30,
    // 1, the angle +0x40) (a guest call); the VU0 block (the corners scaled
    // by the record's size +0xc, rotated, x the world matrix, + the position,
    // into sp + 0x80: the recompiled statements, generated by
    // r117/gen_1142350.py); glBegin(5) (a guest call), glColor4fv(+0x10),
    // the four corners through quad117 (glTexCoord2fv + glVertex4fv + the
    // kick each), glEnd (a guest call; its hook draws). Off (or quad117 not
    // taking them): the four vertices as the calls (the recompiled code from
    // 0x11425a0, the registers as there).
    void quad1142350(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        constexpr uint32_t kSelf = 0x01142350u;
        G g(rdram, ctx, runtime);
        const Ram m = g.m;
        const uint32_t s0 = GPR_U32(ctx, 4), s1in = GPR_U32(ctx, 5), s3 = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x130u;
        if (!sotc_decomp::glv117On() || !Ram::ok(sp, 0x130u) || !Ram::ok(s0, 0x50u) || !Ram::ok(s1in, 8u) || !Ram::ok(s3, 0x40u) ||
            sotc_decomp::jalTarget(rdram, 0x0114240Cu) != 0x001283D0u)
            return fallback(rdram, ctx, runtime, kSelf);
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20);
        m.put64(sp + 0x120u, e20);
        m.put64(sp + 0x100u, e16);
        m.put64(sp + 0x108u, e17);
        m.put64(sp + 0x118u, e19);
        m.put64(sp + 0x110u, e18);
        m.put64(sp + 0x128u, ra0);
        const uint32_t s4 = sp + 0x80u, s2 = sp + 0xC0u;
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 16, s0);
        SET_GPR_U32(ctx, 17, s1in);
        SET_GPR_U32(ctx, 19, s3);
        SET_GPR_U32(ctx, 20, s4);
        for (uint32_t i = 0; i < 0x40u; i += 8u)
            m.put64(sp + i, m.u64(0x01195900u + i));
        std::memset(m.p(s4), 0, 0x40u); // memset (0x1283d0, checked above): the same bytes
        {
            const float f0 = m.f32(s1in), f1 = m.f32(s1in + 4u);
            m.putf(sp + 0xB0u, f0);
            m.putf(sp + 0x90u, f0);
            m.putf(sp + 0xB4u, f1);
            m.putf(sp + 0xA4u, f1);
            for (uint32_t i = 0; i < 0x40u; i += 8u)
                m.put64(sp + 0x40u + i, m.u64(sp + 0x80u + i));
        }
        SET_GPR_U32(ctx, 18, s2);
        g.a(4, s2);
        g.a(5, s0 + 0x30u);
        g.a(6, 1u);
        ctx->f[12] = m.f32(s0 + 0x40u);
        g.call(0x01133748u, 0x0114247Cu, sp); // iosRotMatrixAxis
        {
            const float f0 = m.f32(s0 + 0xCu);
            ctx->f[0] = f0;
            uint32_t bits;
            std::memcpy(&bits, &f0, 4);
            SET_GPR_U32(ctx, 8, bits);
        }
        ctx->vu0_vf[14] = _mm_castsi128_ps(m.qi(s2 + 0u));
        ctx->vu0_vf[15] = _mm_castsi128_ps(m.qi(s2 + 16u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(m.qi(s2 + 32u));
        // (lwc1 f0, 0xc(s0); mfc1 t0, f0; qmtc2.ni t0, vf10: below)
        ctx->vu0_vf[10] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[17] = _mm_castsi128_ps(m.qi(s2 + 48u));
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[26] = _mm_blendv_ps(ctx->vu0_vf[26], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[24] = _mm_blendv_ps(ctx->vu0_vf[24], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[24], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[24] = PS2_VBLEND(ctx->vu0_vf[24], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[26] = _mm_blendv_ps(ctx->vu0_vf[26], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[24] = _mm_blendv_ps(ctx->vu0_vf[24], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[14] = _mm_castsi128_ps(m.qi(s3 + 0u));
        ctx->vu0_vf[15] = _mm_castsi128_ps(m.qi(s3 + 16u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(m.qi(s3 + 32u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(m.qi(s3 + 48u));
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[24], ctx->vu0_vf[24], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[24] = _mm_blendv_ps(ctx->vu0_vf[24], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[26], ctx->vu0_vf[26], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[26] = _mm_blendv_ps(ctx->vu0_vf[26], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[27] = _mm_castsi128_ps(m.qi(s0 + 0u));
        ctx->vu0_vf[20] = _mm_castsi128_ps(m.qi(sp + 0u));
        ctx->vu0_vf[21] = _mm_castsi128_ps(m.qi(sp + 16u));
        ctx->vu0_vf[22] = _mm_castsi128_ps(m.qi(sp + 32u));
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[27] = _mm_blendv_ps(ctx->vu0_vf[27], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[23] = _mm_castsi128_ps(m.qi(sp + 48u));
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[20], ctx->vu0_vf[20], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[20], ctx->vu0_vf[20], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[20], ctx->vu0_vf[20], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[27], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[20] = _mm_blendv_ps(ctx->vu0_vf[20], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[21], ctx->vu0_vf[21], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[21], ctx->vu0_vf[21], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[21], ctx->vu0_vf[21], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[27], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[21] = _mm_blendv_ps(ctx->vu0_vf[21], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[22], ctx->vu0_vf[22], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[22], ctx->vu0_vf[22], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[22], ctx->vu0_vf[22], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[27], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[22] = _mm_blendv_ps(ctx->vu0_vf[22], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[23], ctx->vu0_vf[23], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[23], ctx->vu0_vf[23], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[23], ctx->vu0_vf[23], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[27], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[23] = _mm_blendv_ps(ctx->vu0_vf[23], res, _mm_castsi128_ps(mask)); }
        m.putqi(s4 + 0u, _mm_castps_si128(ctx->vu0_vf[20]));
        m.putqi(s4 + 16u, _mm_castps_si128(ctx->vu0_vf[21]));
        m.putqi(s4 + 32u, _mm_castps_si128(ctx->vu0_vf[22]));
        m.putqi(s4 + 48u, _mm_castps_si128(ctx->vu0_vf[23]));

        SET_GPR_U32(ctx, 17, 0u);
        // (glBegin below, in prim128: it writes nothing glColor4fv or the corners read)
        // glColor4fv(+0x10): glColor4f's words and clamp; f1 - f5 as it leaves them.
        {
            const uint32_t c = s0 + 0x10u;
            glColor4f(ctx, m, m.f32(c), m.f32(c + 4u), m.f32(c + 8u), m.f32(c + 0xCu));
            ctx->f[1] = FPU_MUL_S(m.f32(c + 8u), 255.0f);
            ctx->f[2] = 255.0f;
            ctx->f[3] = 128.0f;
            ctx->f[4] = 255.0f;
            ctx->f[5] = 0.0f;
        }
        const uint32_t st = sp + 0x40u;
        Vert117 vs[4];
        for (int i = 0; i < 4; ++i)
        {
            vs[i].alpha = 0.0f;
            vs[i].u = m.f32(st + i * 16u);
            vs[i].v = m.f32(st + i * 16u + 4u);
            vs[i].pos = s4 + i * 16u;
            vs[i].site = 0x011425B4u;
            vs[i].colour = false;
        }
        auto calls = [&]
        {
            // The loop as the code has it: glTexCoord2fv(sp + 0x40 + 16 i), glVertex4fv(sp + 0x80 + 16 i) (guest calls).
            for (uint32_t i = 0; i < 4u; ++i)
            {
                SET_GPR_U32(ctx, 16, i << 4);
                SET_GPR_U32(ctx, 17, i + 1u);
                g.a(4, st + (i << 4));
                g.call(0x0108E9F0u, 0x011425ACu, sp);
                g.a(4, s4 + (i << 4));
                g.call(0x0108ECA0u, 0x011425B4u, sp);
            }
        };
        // Round 128 (imm128): glBegin(5), the corners, glEnd without the GIF packet (prim128), else as the calls.
        prim128(g, sp, vs, 4, 5u, 0x01142590u, 0x011425C8u, false,
                [&]
                {
                    SET_GPR_U32(ctx, 18, st);
                    SET_GPR_U32(ctx, 20, s4);
                    quadRun117(g, sp, vs, 4, calls);
                    SET_GPR_U32(ctx, 16, 0x30u);
                    SET_GPR_U32(ctx, 17, 4u);
                });
        SET_GPR_U64(ctx, 16, e16);
        SET_GPR_U64(ctx, 17, e17);
        SET_GPR_U64(ctx, 18, e18);
        SET_GPR_U64(ctx, 19, e19);
        SET_GPR_U64(ctx, 20, e20);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
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
                    // Round 117 (glv117): each strip's four vertices in one pass (quad117), else as the calls.
                    auto four = [&](const Vert117 *vs)
                    {
                        auto calls = [&]
                        {
                            for (int i = 0; i < 4; ++i)
                                vertex(vs[i].alpha, vs[i].u, vs[i].v, vs[i].pos, vs[i].site);
                        };
                        if (!sotc_decomp::glv117On())
                            return calls();
                        quadRun117(g, sp, vs, 4, calls);
                    };
                    // Round 128 (imm128): each glBegin .. glEnd without the GIF packet (prim128), else as the calls.
                    {
                        const Vert117 vs[4] = {{f20, m.f32(s6), m.f32(sp + 0xE4u), sp + 0xA0u, 0x010F4690u},
                                               {f21, m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0xB0u, 0x010F46BCu},
                                               {f20, m.f32(s6), m.f32(sp + 0xE4u), sp + 0x120u, 0x010F46E8u},
                                               {f21, m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0x130u, 0x010F4714u}};
                        prim128(g, sp, vs, 4, 5u, 0x010F4664u, 0x010F471Cu, true, [&] { four(vs); });
                    }
                    {
                        const Vert117 vs[4] = {{f20, m.f32(s6), m.f32(sp + 0xE4u), sp + 0x120u, 0x010F4750u},
                                               {f21, m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0x130u, 0x010F477Cu},
                                               {FPU_MUL_S(f20, f28), m.f32(s6), m.f32(sp + 0xE4u), sp + 0x100u, 0x010F47A8u},
                                               {FPU_MUL_S(f21, f28), m.f32(sp + 0xF0u), m.f32(sp + 0xF4u), sp + 0x110u, 0x010F47D4u}};
                        prim128(g, sp, vs, 4, 5u, 0x010F4724u, 0x010F47DCu, true, [&] { four(vs); });
                    }
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
        const lines67::Scope l67(lines67::kDisp, true);
        lines67::base(object);
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

    // ---- Round 95 (join95): the particle geodisps held back by the group pass
    // while the guest worker steps them, joined inside the light pass (after
    // the layout objects' light callbacks: decomp_light calls late95 there
    // through sotc_decomp::g_lightMid95), or here when the light pass is not
    // reached. Each one as the group pass's geodisp branch would have done it
    // (its clip, unless the job ran it; the shown test; the key), then
    // straight into the second list, where the copy loop puts every geodisp
    // (no layout class); the first list is left without them (its chains
    // already relinked into the second: nothing reads it after the copy).
    // `nodes` gets the sort nodes added, for their light callbacks. Their
    // order among equal keys can differ (added last).
    namespace late95
    {
        std::vector<uint32_t> g_deferred;
        uint64_t g_walks = 0, g_inLight = 0, g_objects = 0, g_waitUs = 0, g_early = 0, g_forced = 0, g_atEnd = 0, g_inDraw = 0, g_earlyWalks = 0;
        // draw95: each drawn particle geodisp's sort key in the walk before
        // (the turn its draw is predicted to take in this one: the join is
        // made no later than that bucket).
        std::unordered_map<uint32_t, float> g_lastKey;
        // The bucket AddSortObj (decomp_dl) puts a key in.
        int32_t bucketOf(const Ram m, uint32_t list, float key)
        {
            const int32_t count = m.s32(list + 4u);
            const int32_t word = FPU_CVT_W_S(FPU_MUL_S(key, m.f32(list + 8u)));
            int32_t index = word < count ? (word < 0 ? 0 : word) : count - 1;
            if (m.s32(list + 0xCu) == 1)
                index = count - index - 1;
            return index;
        }
        int32_t predictedFirst(const Ram m, uint32_t list)
        {
            int32_t first = 0x7FFFFFFF;
            for (const uint32_t obj : g_deferred)
                if (const auto it = g_lastKey.find(obj); it != g_lastKey.end())
                    first = std::min(first, bucketOf(m, list, it->second));
            return first;
        }
        void run(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, std::vector<uint32_t> &nodes)
        {
            const Ram m{rdram};
            const auto t0 = std::chrono::steady_clock::now();
            sotc_worker::join(sotc_worker::kSiteWalk);
            g_waitUs += static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count());
            static const int invert79 = std::getenv("SOTC_PTCL79_INVERT") ? std::atoi(std::getenv("SOTC_PTCL79_INVERT")) : 0;
            for (const uint32_t obj : g_deferred)
            {
                if (sotc_decomp::ptcl91Freed(obj))
                    continue;
                ++g_objects;
                const uint32_t gdc = geoDispClass(m, obj);
                if (gdc == 0u)
                    continue;
                if (const uint32_t update = m.u32(gdc + 0xD8u))
                    if (!sotc_decomp::ptcl95Clipped(obj))
                    {
                        SET_GPR_S32(ctx, 4, static_cast<int32_t>(obj));
                        sotc_decomp::callGuest(rdram, ctx, runtime, update, 0x010F55E0u, sp);
                    }
                const float f1 = m.f32(gdc + 0x20u);
                if (!FPU_C_OLT_S(0.0f, f1))
                    continue;
                const float f0 = m.f32(gdc + 0x30u);
                ctx->f[0] = f0;
                const bool culled79 = sotc_decomp::ptcl79Culled(obj);
                const bool draw79 = invert79 == 0 ? !culled79 : invert79 == 1 ? culled79 : false;
                if (!(FPU_C_OLE_S(0.0f, f0) && draw79))
                    continue;
                m.putf(gdc + 4u, f1);
                // Straight into the second list: the first one's chains were
                // relinked into the second by its copy loop (adding to it now
                // would walk the second list's links and could link the node
                // twice: a cycle, the hang p1 met).
                SET_GPR_S32(ctx, 5, static_cast<int32_t>(gdc));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(m.u32(kSortList2)));
                sotc_decomp::callGuest(rdram, ctx, runtime, kAddSortObj, 0x010F56B0u, sp);
                nodes.push_back(gdc);
                if (g_lastKey.size() > 4096u)
                    g_lastKey.clear();
                g_lastKey[obj] = f1;
            }
            g_deferred.clear();
        }
        std::vector<uint32_t> g_lightNodes; // SOTC_DRAW95_CHECK: the nodes lit inside the light pass
        uint64_t g_checked = 0, g_differ = 0;
        void inLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, std::vector<uint32_t> &nodes)
        {
            ++g_inLight;
            run(rdram, ctx, runtime, sp, nodes);
            g_lightNodes = nodes;
        }
        bool checkOn()
        {
            static const bool on = ps2_diag::env("SOTC_DRAW95_CHECK") && std::atoi(ps2_diag::env("SOTC_DRAW95_CHECK")) != 0;
            return on;
        }
        void report()
        {
            if (++g_walks % 600u != 0u)
                return;
            std::fprintf(stderr, "[early95] %llu walks: after an early hand-over %.0f%%\n", (unsigned long long)g_walks, 100.0 * g_earlyWalks / 600.0);
            g_earlyWalks = 0;
            std::fprintf(stderr, "[join95] %llu walks: joined in the light pass %.0f%%, in the draw loop %.0f%% (at its turn, not done: %.0f%%; at the loop's end %.0f%%), "
                                 "%.1f particle geodisps a walk, %.2f drawn out of turn, join wait %.3f ms a walk\n",
                         (unsigned long long)g_walks, 100.0 * g_inLight / 600.0, 100.0 * g_inDraw / 600.0, 100.0 * g_forced / 600.0, 100.0 * g_atEnd / 600.0,
                         g_objects / 600.0, g_early / 600.0, g_waitUs / 600.0 / 1000.0);
            g_inLight = g_objects = g_waitUs = g_early = g_forced = g_atEnd = g_inDraw = 0;
        }
    }

    // ---- DispDispObjGroupModel (0x10f52c8; a0 the camera object) -------------
    void dispDispObjGroupModel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const lines67::Scope l67(lines67::kGroup, false, GPR_U32(ctx, 29));
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
        const bool wp = wprof::on(); uint64_t wt0 = wp ? sotc_prof::ticks() : 0u;
        const uint64_t wstart = wt0;
        set(30, 0x01190000u);
        g.callRel(0x010F5310u, sp); // isysObjWaitSema
        set(19, 0u);
        g.call(kGraphFsEffectReset, 0x010F5318u, sp);
        const bool w67 = sotc_decomp::walk67On();
        if (w67)
        {
            // Round 67: the bucket array the clear writes, asked for ahead of
            // its stores (~4 KB, one miss a line otherwise).
            const uint32_t list = m.u32(kSortList);
            const int32_t count = m.s32(list + 4u);
            const uint32_t buckets = m.u32(list);
            if (count > 0 && count <= 0x10000 && Ram::ok(buckets, static_cast<uint32_t>(count) * 4u))
                for (uint32_t a = buckets & ~31u, end = buckets + static_cast<uint32_t>(count) * 4u; a < end; a += 32u)
                    ask(m, a);
        }
        clearSortList(m, m.u32(kSortList));
        g.a(4, m.u32(sp + 0x20u));
        SET_GPR_U64(ctx, 5, 0u);
        g.callRel(0x010F532Cu, sp); // isysGroupForFirst
        uint32_t s4 = g.v0(), s3 = 0u;
        set(20, s4);
        QuatProduct qp{};
        const bool walk83 = sotc_decomp::walk83On();
        bool s6Known = false;
        uint32_t s6Class = 0u;
        GroupRunner runner;
        if (w67 && Ram::ok(s4, 0x18u))
        {
            runner.entry = s4;
            for (int i = 0; i < 4; ++i)
                runner.step(m);
        }
        // A geodisp object of the group (the walk's else branch: its class's
        // update callback, the visible test, the sort list).
        bool joined91 = false; // round 95: the job's results readable (after the walk's join)
        auto geodispObject = [&](const uint32_t s3)
        {
            const uint32_t gdc = geoDispClass(m, s3);
            set(16, gdc);
            if (const uint32_t update = m.u32(gdc + 0xD8u); update != 0u && !(joined91 && sotc_decomp::ptcl95Clipped(s3)))
            {
                g.a(4, s3);
                {
                    const uint64_t t0_ = wprof::now();
                    g.call(update, 0x010F55E0u, sp);
                    if (wp) wprof::add('u', update, sotc_prof::ticks() - t0_);
                }
            }
            const float f1 = m.f32(gdc + 0x20u);
            if (FPU_C_OLT_S(0.0f, f1))
            {
                const float f0 = m.f32(gdc + 0x30u);
                ctx->f[0] = f0;
                // Round 79: a particle system wholly outside the view is not drawn.
                // SOTC_PTCL79_INVERT=1 (checking): only those are drawn; =2: no particle system.
                static const int invert79 = std::getenv("SOTC_PTCL79_INVERT") ? std::atoi(std::getenv("SOTC_PTCL79_INVERT")) : 0;
                const bool culled79 = sotc_decomp::ptcl79Culled(s3);
                const bool draw79 = invert79 == 0 ? !culled79 : invert79 == 1 ? culled79 : false;
                if (FPU_C_OLE_S(0.0f, f0) && draw79)
                {
                    m.putf(gdc + 4u, f1);
                    g.a(5, gdc);
                    g.a(4, m.u32(kSortList));
                    g.call(kAddSortObj, 0x010F5614u, sp);
                    if (wp) ++wprof::g_cnt[3];
                }
            }
        };
        // Round 91 (ptcl91, sotc_worker.cpp): while the particle step runs on
        // the guest worker, the particle geodisps (their class's update in the
        // particle module: the clip from their particles) wait for the join
        // after the group pass, then go through the same branch in the group's
        // order; one the job's auto-end check freed is left out (the original
        // freed it before the walk). Their sort keys put them where they would
        // have gone (AddSortObj sorts by key; only equal keys can differ).
        const bool defer91 = sotc_worker::busy();
        static std::vector<uint32_t> deferred91;
        deferred91.clear();
        auto flush91 = [&]()
        {
            if (!defer91)
                return;
            sotc_worker::join(sotc_worker::kSiteWalk);
            joined91 = true;
            uint64_t saved[8];
            for (int r = 0; r < 8; ++r)
                saved[r] = GPR_U64(ctx, 16 + r);
            for (const uint32_t obj : deferred91)
                if (!sotc_decomp::ptcl91Freed(obj))
                {
                    set(19, obj);
                    geodispObject(obj);
                }
            deferred91.clear();
            for (int r = 0; r < 8; ++r)
                SET_GPR_U64(ctx, 16 + r, saved[r]);
        };
        while (s4 != 0u)
        {
            if (!Ram::ok(s4, 0x10u) || !Ram::ok(m.u32(s4), 0x10u))
            {
                flush91();
                set(19, Ram::ok(s4, 4u) ? m.u32(s4) : s3);
                set(20, s4);
                return resume(0x010F5348u);
            }
            s3 = m.u32(s4);
            if (w67)
            {
                runner.step(m);
                decomp_prefetch::aheadWalkClip(rdram, s3);
            }
            set(19, s3);
            const uint32_t s0 = layoutClass(g, s3, 0x010F5348u, sp);
            set(16, s0);
            if (wp) { ++wprof::g_cnt[0]; ++wprof::g_cnt[s0 ? 1 : 2]; }
            if (s0 != 0u)
            {
                set(21, 1u);
                // Round 83 (SOTC_WALK83): the walk's own object's layout class
                // (GetLayoutClass(a0), a lookup) once a walk, not once an object.
                uint32_t s1;
                if (walk83)
                {
                    if (!s6Known)
                    {
                        s6Class = layoutClass(g, s6, 0x010F5360u, sp);
                        s6Known = true;
                    }
                    s1 = s6Class;
                }
                else
                    s1 = layoutClass(g, s6, 0x010F5360u, sp);
                set(17, s1);
                if (!Ram::ok(s0, 0x1C0u) || !Ram::ok(s1, 0x160u) || !Ram::ok(m.u32(s0 + 0x140u), 0x40u) || !Ram::ok(m.u32(s0 + 0x144u), 0x268u))
                {
                    flush91();
                    return resume(0x010F5348u);
                }
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
                    {
                        const uint64_t t0_ = wprof::now();
                        g.call(kUpdateSimple, 0x010F54E4u, sp);
                        if (wp) wprof::add('S', kUpdateSimple, sotc_prof::ticks() - t0_);
                    }
                }
                if (m.s8(s0 + 0x1B8u) != 0)
                {
                    if (const uint32_t update = m.u32(s2 + 0xD8u))
                    {
                        g.a(4, s3);
                        {
                            const uint64_t t0_ = wprof::now();
                            g.call(update, 0x010F5504u, sp);
                            if (wp) wprof::add('U', update, sotc_prof::ticks() - t0_);
                        }
                    }
                    if (const uint32_t clip = m.u32(s2 + 0xDCu))
                    {
                        g.a(4, s3);
                        const uint64_t t0_ = wprof::now();
                        if (clip == kClip)
                            g.callNative(clipLayoutObjModel, kClip, 0x010F5518u, sp);
                        else
                            g.call(clip, 0x010F5518u, sp);
                        if (wp) wprof::add('C', clip, sotc_prof::ticks() - t0_);
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
                            {
                                const uint64_t t0_ = wprof::now();
                                g.call(told, 0x010F5580u, sp);
                                if (wp) wprof::add('T', told, sotc_prof::ticks() - t0_);
                            }
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
                    if (wp) ++wprof::g_cnt[3];
                }
                const uint32_t geo = m.u32(s0 + 0x144u);
                m.put64(geo + 0x260u, (m.u64(geo + 0x260u) & ~0x100ull) | (s5 << 8));
            }
            else
            {
                const uint32_t gdc91 = geoDispClass(m, s3);
                const uint32_t update91 = m.u32(gdc91 + 0xD8u);
                if (defer91 && update91 >= 0x01139CA0u && update91 < 0x01147530u)
                {
                    set(16, gdc91);
                    deferred91.push_back(s3);
                }
                else
                    geodispObject(s3);
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
        // Round 91: the particle step's join, the particle geodisps; round 95
        // (join95): held until the light pass (late95 above).
        // Not after an early hand-over (early95): the job started a camera before and is
        // mostly done or waiting for its kept calls here; holding the geodisps longer only
        // costs (p5: riding, join95 + draw95 on top of early95 1.4 ms slower than without).
        const bool early95 = sotc_decomp::early95Kicked();
        const bool late95 = defer91 && !deferred91.empty() && sotc_decomp::join95On() && !early95;
        late95::g_earlyWalks += early95 ? 1u : 0u;
        sotc_worker::noteWalk(); // round 95: a camera manager call after this in the frame makes no early hand-over
        if (late95)
        {
            late95::g_deferred.assign(deferred91.begin(), deferred91.end());
            deferred91.clear();
        }
        else
            flush91();
        if (defer91)
            late95::report();
        if (wp) { const uint64_t t = sotc_prof::ticks(); wprof::g_phase[1] += t - wt0; wt0 = t; }
        // The second list: the first one without the objects with bit 4 of +0x260.
        clearSortList(m, m.u32(kSortList2));
        static std::vector<uint32_t> order;
        size_t turn = 0;
        if (w67)
            sortOrder(m, m.u32(kSortList), order);
        uint32_t s0 = firstSortObj(m, m.u32(kSortList));
        while (s0 != 0u)
        {
            if (w67)
            {
                if (turn < order.size() && order[turn] == s0)
                    sortAhead(m, order, turn, false);
                ++turn;
            }
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
        // Round 95: no light pass ahead (nothing in the second list): the held
        // particle geodisps now, so the light pass below sees them as before.
        std::vector<uint32_t> nodes95;
        if (late95 && firstSortObj(m, m.u32(kSortList2)) == 0u)
            late95::run(rdram, ctx, runtime, sp, nodes95);
        set(16, kSortList2);
        if (wp) { const uint64_t t = sotc_prof::ticks(); wprof::g_phase[2] += t - wt0; wt0 = t; }
        if (firstSortObj(m, m.u32(kSortList2)) != 0u)
        {
            g.call(kSetEnviroment, 0x010F56D8u, sp);
            g.a(4, m.u32(kSortList2));
            g.a(6, m.u32(sp + 0x20u));
            g.a(5, s6);
            // Round 95 (draw95): the held particle geodisps wait for the draw
            // loop (below); otherwise (join95) the light pass joins them.
            bool pending95 = !late95::g_deferred.empty() && sotc_decomp::draw95On();
            if (!late95::g_deferred.empty() && !pending95)
                sotc_decomp::g_lightMid95 = late95::inLight;
            g.call(kLightPass, 0x010F56E8u, sp);
            // A particle geodisp's light callback (the light pass's geodisp branch).
            auto light95 = [&](uint32_t node)
            {
                const uint32_t id = m.u32(node), cls = geoDispClass(m, id);
                if (const uint32_t light = cls ? m.u32(cls + 0xE0u) : 0u)
                {
                    g.a(4, id);
                    g.a(5, node);
                    g.call(light, 0x010F5074u, sp);
                }
            };
            // SOTC_DRAW95_CHECK=1 (with draw95 off): each particle geodisp lit
            // inside the light pass lit again after it (as draw95 does), its
            // class's bytes compared, then put back as the pass left them.
            if (late95::checkOn() && !late95::g_lightNodes.empty() && sotc_decomp::g_lightMid95 == nullptr)
            {
                for (const uint32_t node : late95::g_lightNodes)
                {
                    const uint32_t id = m.u32(node);
                    if (!Ram::ok(id, 0x140u))
                        continue;
                    uint8_t before[0x120], after[0x120];
                    std::memcpy(before, rdram + id + 0x20u, sizeof(before));
                    light95(node);
                    std::memcpy(after, rdram + id + 0x20u, sizeof(after));
                    ++late95::g_checked;
                    if (std::memcmp(before, after, sizeof(before)) != 0)
                    {
                        if (++late95::g_differ <= 8u)
                            for (uint32_t i = 0; i < sizeof(before); i += 4)
                                if (std::memcmp(before + i, after + i, 4) != 0)
                                {
                                    uint32_t b, a2;
                                    std::memcpy(&b, before + i, 4);
                                    std::memcpy(&a2, after + i, 4);
                                    std::fprintf(stderr, "[draw95chk] geodisp %08x class +%03x: in the pass %08x, after it %08x\n", id, i, b, a2);
                                }
                        std::memcpy(rdram + id + 0x20u, before, sizeof(before));
                    }
                }
                late95::g_lightNodes.clear();
                if (late95::g_checked % 2000u < 40u)
                    std::fprintf(stderr, "[draw95chk] %llu lit again after the light pass, %llu differ\n", (unsigned long long)late95::g_checked, (unsigned long long)late95::g_differ);
            }
            if (!pending95 && (sotc_decomp::g_lightMid95 != nullptr || !late95::g_deferred.empty()))
            {
                // The light pass went to the recompiled code: the held ones
                // after it, their light callbacks then.
                sotc_decomp::g_lightMid95 = nullptr;
                nodes95.clear();
                late95::run(rdram, ctx, runtime, sp, nodes95);
                for (const uint32_t node : nodes95)
                    light95(node);
            }
            if (wp) { const uint64_t t = sotc_prof::ticks(); wprof::g_phase[3] += t - wt0; wt0 = t; }
            const uint32_t list = m.u32(kSortList2);
            set(19, list);
            if (w67)
                sortOrder(m, list, order);
            turn = 0;
            // Round 95 (draw95): a held particle geodisp's draw (the draw loop's geodisp branch).
            auto drawGeo95 = [&](uint32_t node)
            {
                const uint32_t obj = m.u32(node), cls = geoDispClass(m, obj);
                set(16, obj);
                set(17, cls);
                if (obj != 0u)
                {
                    g.a(4, obj);
                    g.callRel(0x010F577Cu, sp); // iosDlLockObj
                }
                g.a(5, obj);
                g.a(4, s6);
                g.call(m.u32(cls + 0xD0u), 0x010F578Cu, sp);
            };
            // The join, before the draw of `next` (0: the loop's end): the held
            // ones into both lists (late95::run) and their light callbacks; those
            // the loop will not reach from `next` any more (their bucket passed)
            // drawn now, in the list's order; the rest in their turn.
            // SOTC_DRAW95_ORDER=1: also join (waiting) when the loop reaches the
            // first bucket the held geodisps' keys of the walk before predict, so
            // they keep their turn; default 0: no wait in the loop (p3 measured
            // the turn coming early: most of the wait stayed), those whose turn
            // has passed are drawn when the job is done or at the loop's end.
            static const bool order95 = std::getenv("SOTC_DRAW95_ORDER") && std::atoi(std::getenv("SOTC_DRAW95_ORDER")) != 0;
            const int32_t first95 = pending95 && order95 ? late95::predictedFirst(m, list) : 0x7FFFFFFF;
            auto insert95 = [&](uint32_t next)
            {
                pending95 = false;
                ++late95::g_inDraw;
                nodes95.clear();
                late95::run(rdram, ctx, runtime, sp, nodes95);
                for (const uint32_t node : nodes95)
                    light95(node);
                if (nodes95.empty())
                    return;
                const uint32_t index = m.u32(list + 16u);
                static std::vector<uint32_t> reach, all;
                reach.clear();
                all.clear();
                for (uint32_t x = next; x != 0u && reach.size() < 4096u; x = nextSortObj(m, list, x))
                    reach.push_back(x);
                for (uint32_t x = firstSortObj(m, list); x != 0u && all.size() < 4096u; x = nextSortObj(m, list, x))
                    all.push_back(x);
                m.put32(list + 16u, index);
                for (const uint32_t x : all)
                    if (std::find(nodes95.begin(), nodes95.end(), x) != nodes95.end() && std::find(reach.begin(), reach.end(), x) == reach.end())
                    {
                        drawGeo95(x);
                        ++late95::g_early;
                    }
                set(18, next);
            };
            uint32_t s2 = firstSortObj(m, list);
            set(18, s2);
            while (s2 != 0u)
            {
                if (!Ram::ok(s2, 0x10u))
                {
                    if (pending95)
                    {
                        pending95 = false;
                        nodes95.clear();
                        late95::run(rdram, ctx, runtime, sp, nodes95);
                        for (const uint32_t node : nodes95)
                            light95(node);
                        set(18, s2);
                    }
                    return resume(0x010F5710u);
                }
                if (pending95 && (static_cast<int32_t>(m.u32(list + 16u)) >= first95 || sotc_worker::done()))
                {
                    late95::g_forced += sotc_worker::done() ? 0u : 1u;
                    insert95(s2);
                }
                if (w67)
                {
                    if (turn < order.size() && order[turn] == s2)
                        sortAhead(m, order, turn, true);
                    ++turn;
                    decomp_prefetch::aheadWalkDisp(rdram, m.u32(s2));
                }
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
                        const uint64_t t0_ = wprof::now();
                        g.callRel(0x010F5728u, sp); // iosDlLockObj
                        if (wp) wprof::add('K', 0u, sotc_prof::ticks() - t0_);
                    }
                    if (const uint32_t lights = m.u32(cls + 0xD4u))
                    {
                        g.a(6, m.u32(cls + 0xE4u));
                        g.a(4, s6);
                        g.a(5, obj);
                        {
                            const uint64_t t0_ = wprof::now();
                            g.call(lights, 0x010F5744u, sp);
                            if (wp) wprof::add('L', lights, sotc_prof::ticks() - t0_);
                        }
                    }
                    const uint32_t draw = m.u32(cls + 0xD0u);
                    g.a(5, obj);
                    g.a(4, s6);
                    const uint64_t t0_ = wprof::now();
                    if (draw == kDisp)
                        g.callNative(dispLayoutObjModel, kDisp, 0x010F5754u, sp);
                    else
                        g.call(draw, 0x010F5754u, sp);
                    if (wp) { wprof::add('D', draw, sotc_prof::ticks() - t0_); ++wprof::g_cnt[4]; }
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
                    {
                        const uint64_t t0_ = wprof::now();
                        g.call(draw, 0x010F578Cu, sp); ++wprof::g_cnt[4];
                        if (wp) wprof::add('d', draw, sotc_prof::ticks() - t0_);
                    }
                }
                s2 = nextSortObj(m, list, s2);
                set(18, s2);
            }
            if (pending95)
            {
                ++late95::g_atEnd;
                insert95(0u);
            }
        }
        if (wp) { const uint64_t t = sotc_prof::ticks(); wprof::g_phase[4] += t - wt0; wprof::g_phase[0] += t - wstart; wprof::report(); }
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
SOTC_DECOMP_FUNCTION(0x01142350u, particleQuad_01142350, decomp_walk::quad1142350);
SOTC_DECOMP_AB_FUNCTION(0x0108D730u, glVertexKick_0108D730, decomp_walk::glVertexKick);
SOTC_DECOMP_AB_FUNCTION(0x0108EBC0u, glVertex3fv, decomp_walk::glVertex3fvEntry);
