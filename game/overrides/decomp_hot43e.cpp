// gamecore_sub_013EF378 (0x13ef378, round 43): the stage's small-object
// manager's frame (updateSmallManager 0x13eefa0 / gamecore_sub_013EF210 call
// it; 0.3% of the Vita's game thread recompiled). a0 the manager m, a1 the
// player record (+0x134: the position qword).
//
// The type table: *0x1b7b70 (boot ELF, relocated), 0x34 bytes a type: +0xc
// a flag table in use, +0x10 the stage the type belongs to, +0x18 / +0x1c a
// placement callback and "needs its result", +0x20 a dead-boss limit, +0x30
// the stage's load count.
//
// The near list at m + 0x11c (count, then 10 records of 0x1c bytes: +0 the
// layout object's id, +4 the point index, +8 the squared distance, +0xc the
// layout point data, +0x10 / +0x14 / +0x18 the record's spawned objects:
// count, head, tail, linked through +0x58); last frame's list at m + 0.
//   1. The list is cleared (ids -1, distances *0x146b4c4) and refilled from
//      every layout object of group 0x1465944 (isysGroupForFirst / Next /
//      Exit): each point (getLayoutPointData) of the current stage (type
//      +0x10 == m + 0x9d4) nearer than its radius (+0x24 squared, else
//      150^2) goes in, sorted by distance, the farthest dropped at 10.
//   2. Each of last frame's records found again (same id and point) keeps
//      its objects; one gone gives its idle objects (+0x54 == 0) back to the
//      free list m + 0x238 and, holding any still, takes a free slot (a new
//      one, or the first whose objects are all idle: 013F1330).
//   3. Two passes over the list with the budget 013F1370 gives: pass 0
//      trims each record to its share (the flag table's cleared bits
//      (gexsFlagChk / 013F12F0), the dead-boss count, m + 0x9d0 in all),
//      pass 1 spawns one object a frame for the nearest record short of it
//      (013F13E0 / 013F1430 give the template; it comes off the free list
//      or the spare list m + 0x244), copies the point into it, notes its
//      flag bit (tailsFlagChk) and asks for its stage (0x11f8510: one spawn
//      a frame). After pass 1 with 0x11f8518 clear and free objects left:
//      013F0888 frees them.
//   4. Unless 0x11f8520: each record's first object with no model (+0 == 0)
//      whose stage is loaded and with more than ~1.2 MB of heap left
//      (iosGetMallocRootPtnRemainSize, else m + 0x9d8 = 1) gets its model
//      parts loaded (loadObj per part, at most 2; the placement callback or
//      getStageLayoutGeometryPointer, GetQuaternionByDegEulerYXZ for the
//      part's rotation); one load a frame (0x11f8514).
//   5. The new list is copied over last frame's (0x11c bytes).
//
// The recompiled code's operations in its order: the FPU compares (fcr31's
// condition bit), FPU_MUL_S / ADD_S / CVT_S_W / NEG_S and the distance's
// PS2_VSUB / VMUL_K / VADD_K into vf5..vf7 as it does them; f0 kept in the
// context at each write (the guest calls leave theirs); its frame written
// (the saved registers, the spill slots, the structs the callees get
// pointers to); the guest calls (callGuest) with the original's ra and frame
// sp and its argument registers (t0 / t1 / t2 too for the placement
// callback and loadObj); HI / LO at each mult; v0 / v1 as it leaves them
// (the copy's last two loads). The boot ELF's references (the type table,
// the null handle 0x1e0ed4, the callback structs 0x1e0fd0 / 0x1e0c88) and
// its jal sites are read from the live words, as the recompiled code does.
// The list unlinks are written once (the original's seven copies differ
// only in when they reload a field nothing in between writes).
//
// SOTC_HOTLIST=0 or SOTC_DECOMP_SKIP=gamecore_sub_013EF378: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43e
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    constexpr uint32_t kSelf = 0x013EF378u;
    // GAMECORE's own globals and functions (fixed: the module is not relocated).
    constexpr uint32_t kNoLoads = 0x011F8520u, kSpawned = 0x011F8510u, kLoaded = 0x011F8514u, kFreed = 0x011F8518u;
    constexpr uint32_t kLayoutGroup = 0x01465944u, kFarDistance = 0x0146B4C4u, kLoadName = 0x014658D0u;
    constexpr uint32_t kFreeSlot = 0x013F1330u, kBudget = 0x013F1370u, kGexsFlag = 0x0134F938u, kFlagTaken = 0x013F12F0u;
    constexpr uint32_t kDeadBosses = 0x0134A388u, kTemplateByFlag = 0x013F13E0u, kTemplate = 0x013F1430u, kTailsFlag = 0x0134F820u;
    constexpr uint32_t kStageName = 0x013F1290u, kStageNumber = 0x013663D0u, kLoadStage = 0x01366BF0u, kStageLoaded = 0x013668F0u;
    constexpr uint32_t kFreeAll = 0x013F0888u, kGeometry = 0x01356630u, kLoadObj = 0x01354B78u;
    constexpr uint32_t kMask18 = 0x3FFFFu;

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp = 0;  // the frame
        uint32_t m = 0;   // a0: the manager
        uint32_t tbl = 0; // the type table

        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        int32_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint32_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put16(uint32_t a, uint32_t v) const
        {
            const uint16_t h = static_cast<uint16_t>(v);
            std::memcpy(rdram + (a & PS2_RAM_MASK), &h, 2);
        }
        void putf(uint32_t a, float v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
        // n bytes read first, then written (the ldl / ldr, sdl / sdr runs).
        void copy(uint32_t dst, uint32_t src, uint32_t n) const
        {
            uint8_t t[0x20];
            for (uint32_t i = 0; i < n; ++i)
                t[i] = rdram[(src + i) & PS2_RAM_MASK];
            for (uint32_t i = 0; i < n; ++i)
                rdram[(dst + i) & PS2_RAM_MASK] = t[i];
        }
        void frame(uint32_t off, uint32_t v) const { put32(sp + off, v); }
        uint32_t frameWord(uint32_t off) const { return u32(sp + off); }

        uint32_t type(int32_t index) const { return tbl + static_cast<uint32_t>(index) * 0x34u; }

        // ---- registers --------------------------------------------------------
        void a(int reg, uint32_t v) const { SET_GPR_S32(ctx, reg, static_cast<int32_t>(v)); }
        void setF0(float v) const { ctx->f[0] = v; }
        // c.lt.s: the condition bit, as the original leaves it.
        bool lt(float x, float y) const
        {
            const bool c = FPU_C_OLT_S(x, y);
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        }
        // mult rd, rs, rt: HI / LO and rd.
        int32_t mult(int32_t x, int32_t y) const
        {
            const int64_t r = static_cast<int64_t>(x) * static_cast<int64_t>(y);
            ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r)));
            ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r >> 32)));
            return static_cast<int32_t>(r);
        }
        // A jal (ra its return address) from this frame; v0.
        uint32_t call(uint32_t target, uint32_t ra) const
        {
            sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
            return GPR_U32(ctx, 2);
        }
        uint32_t call1(uint32_t target, uint32_t ra, uint32_t a0) const
        {
            a(4, a0);
            return call(target, ra);
        }
        uint32_t call2(uint32_t target, uint32_t ra, uint32_t a0, uint32_t a1) const
        {
            a(4, a0);
            a(5, a1);
            return call(target, ra);
        }

        // The squared distance of two points (xyz), as the code's vsub /
        // vmul / vaddy / vaddz into vf7 (vf5 / vf6 the points).
        float distance2(uint32_t p, uint32_t q) const
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(p));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(q));
            {
                __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]);
                __m128i mask = _mm_set_epi32(-1, -1, -1, -1);
                ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
            }
            {
                __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[7], ctx->vu0_vf[7]);
                __m128i mask = _mm_set_epi32(0, -1, -1, -1);
                ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
            }
            {
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1, 1, 1, 1)));
                __m128i mask = _mm_set_epi32(0, 0, 0, -1);
                ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
            }
            {
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2, 2, 2, 2)));
                __m128i mask = _mm_set_epi32(0, 0, 0, -1);
                ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
            }
            float d;
            std::memcpy(&d, &ctx->vu0_vf[7], 4);
            return d;
        }

        // ---- object lists: +0 count, +4 head, +8 tail, linked through +0x58 ----
        // Takes node out of the list; false when it is not in it.
        bool unlink(uint32_t list, uint32_t node) const
        {
            uint32_t prev = 0, cur = u32(list + 4u);
            if (!cur)
                return false;
            while (cur != node)
            {
                prev = cur;
                cur = u32(cur + 0x58u);
                if (!cur)
                    return false;
            }
            if (prev)
                put32(prev + 0x58u, u32(node + 0x58u));
            if (u32(list + 4u) == node)
                put32(list + 4u, u32(node + 0x58u));
            if (u32(list + 8u) == node)
                put32(list + 8u, prev);
            put32(list, u32(list) - 1u);
            return true;
        }
        void append(uint32_t list, uint32_t node) const
        {
            const uint32_t tail = u32(list + 8u);
            if (tail == 0)
                put32(list + 4u, node);
            else
                put32(tail + 0x58u, node);
            put32(node + 0x58u, 0);
            put32(list + 8u, node);
            put32(list, u32(list) + 1u);
        }

        // A handle word at `at`: its low 18 bits back to the owner record
        // (0: none). The address it names: the record's +0x10 plus
        // (bits 18..29) x the record's element size (+8 -> +0xc), or 0 for
        // no record or the null record (*nullAt).
        uint32_t owner(uint32_t at, uint32_t word) const
        {
            const uint32_t back = word & kMask18;
            return back ? u32(at - back) : 0u;
        }
        uint32_t element(uint32_t at, uint32_t nullAt) const
        {
            const uint32_t word = u32(at);
            const uint32_t rec = owner(at, word);
            if (rec == 0 || rec == u32(nullAt))
                return 0;
            const uint32_t size = u32(u32(rec + 8u) + 0xCu);
            const uint32_t base = u32(rec + 0x10u);
            return base + static_cast<uint32_t>(mult(static_cast<int32_t>((word >> 18) & 0xFFFu), static_cast<int32_t>(size)));
        }
        // The owner record's field at +off (0 for none or the null record).
        uint32_t ownerField(uint32_t at, uint32_t word, uint32_t nullAt, uint32_t off) const
        {
            const uint32_t rec = owner(at, word);
            if (rec == 0 || rec == u32(nullAt))
                return 0;
            return u32(rec + off);
        }

        void nearList(uint32_t player);
        void merge(uint32_t player);
        void passes();
        void loads();
    };

    // ---- 1. the near list ------------------------------------------------------
    void G::nearList(uint32_t player)
    {
        const uint32_t list = m + 0x11Cu;
        uint32_t layout = 0; // s3
        uint32_t node = call2(jalTarget(rdram, 0x013EF438u), 0x013EF440u, kLayoutGroup, 0);
        while (node)
        {
            layout = u32(node);
            uint32_t points = u32(layout + 0x58u);
            if (points && u32(points) != 0)
            {
                for (uint32_t index = 0;;)
                {
                    const uint32_t point = call2(jalTarget(rdram, 0x013EF470u), 0x013EF478u, points, index);
                    const float d = distance2(point, u32(player + 0x134u));
                    const uint32_t ty = type(s16(point + 0x14u));
                    if (u32(ty + 0x10u) == u32(m + 0x9D4u))
                    {
                        const float radius = f32(point + 0x24u);
                        setF0(0.0f);
                        float limit;
                        {
                            const uint32_t bits = 0x46AFC800u; // 150^2
                            std::memcpy(&limit, &bits, 4);
                        }
                        if (lt(0.0f, radius))
                            limit = FPU_MUL_S(radius, radius);
                        if (lt(d, limit))
                        {
                            int32_t count = s32(list);
                            bool insert = true;
                            uint32_t at = list + 4u; // the record written
                            if (count == 0)
                            {
                                const uint32_t id = u32(u32(layout + 0x38u) + 0x1Cu);
                                putf(at + 8u, d);
                                put32(at, id);
                                put32(at + 0xCu, point);
                                put32(at + 4u, index);
                                put32(list, u32(list) + 1u);
                                insert = false;
                            }
                            else if (count == 10)
                            {
                                const float last = f32(list + 0x108u);
                                setF0(last);
                                if (lt(last, d))
                                    insert = false;
                                else
                                {
                                    put32(list, 9);
                                    count = 9;
                                }
                            }
                            if (insert && count >= 0)
                            {
                                // Walk to the first record farther than d (or the end).
                                int32_t hi = count, pos = 0;
                                bool past = false;
                                for (;;)
                                {
                                    const float f = f32(at + 8u);
                                    setF0(f);
                                    if (lt(d, f))
                                    {
                                        --hi;
                                        break;
                                    }
                                    if (pos == hi)
                                    {
                                        --hi;
                                        break;
                                    }
                                    ++pos;
                                    past = count < pos;
                                    at += 0x1Cu;
                                    if (past)
                                        break;
                                }
                                if (!past)
                                {
                                    // Records pos..hi one down.
                                    if (!(hi < pos))
                                        for (uint32_t dst = list + 0x20u + static_cast<uint32_t>(hi) * 0x1Cu;; dst -= 0x1Cu)
                                        {
                                            --hi;
                                            copy(dst, dst - 0x1Cu, 0x1Cu);
                                            if (hi < pos)
                                                break;
                                        }
                                    const uint32_t id = u32(u32(layout + 0x38u) + 0x1Cu);
                                    put32(at + 0xCu, point);
                                    put32(at, id);
                                    putf(at + 8u, d);
                                    put32(at + 4u, index);
                                    put32(list, u32(list) + 1u);
                                }
                            }
                        }
                    }
                    points = u32(layout + 0x58u);
                    ++index;
                    if (!(index < u32(points)))
                        break;
                }
            }
            node = call2(jalTarget(rdram, 0x013EF640u), 0x013EF648u, node, 0);
        }
        if (layout)
            call2(jalTarget(rdram, 0x013EF65Cu), 0x013EF664u, layout, 0);
    }

    // ---- 2. last frame's records into the new list -----------------------------
    void G::merge(uint32_t player)
    {
        const uint32_t list = m + 0x11Cu;
        uint32_t old = m + 0x14u; // the old record's +0x10 (its objects' list)
        for (int32_t i = 0; i < s32(m); ++i, old += 0x1Cu)
        {
            const uint32_t id = u32(old - 0x10u);
            const int32_t count = s32(list);
            bool found = false;
            for (int32_t k = 0; k < count; ++k)
            {
                const uint32_t r = list + 4u + static_cast<uint32_t>(k) * 0x1Cu;
                if (u32(r) == id && u32(r + 4u) == u32(old - 0xCu))
                {
                    copy(r + 0x10u, old, 12);
                    found = true;
                    break;
                }
            }
            if (found)
                continue;
            // Gone: its idle objects back to the free list.
            const int32_t objects = s32(old);
            if (objects > 0)
            {
                uint32_t obj = u32(old + 4u);
                for (int32_t left = objects;;)
                {
                    const uint32_t busy = u32(obj + 0x54u), next = u32(obj + 0x58u);
                    if (busy == 0 && obj != 0 && unlink(old, obj))
                        append(m + 0x238u, obj);
                    obj = next;
                    if (--left == 0)
                        break;
                }
            }
            if (s32(old) <= 0)
                continue;
            // Still holding some: a slot in the new list.
            int32_t n = s32(list);
            uint32_t slot = 0;
            if (!(n < 10))
            {
                uint32_t r = list + 4u;
                for (int32_t k = 0;; r += 0x1Cu)
                {
                    const bool more = k < n;
                    ++k;
                    if (!more)
                        break;
                    if (call1(kFreeSlot, 0x013EF850u, r) == 0)
                    {
                        slot = r;
                        break;
                    }
                    n = s32(list);
                }
            }
            else
            {
                put32(list, static_cast<uint32_t>(n + 1));
                slot = list + 4u + static_cast<uint32_t>(n) * 0x1Cu;
            }
            if (slot)
            {
                copy(slot, old - 0x10u, 0x1Cu);
                const uint32_t p = u32(slot + 0x14u) + 0x20u;
                put32(slot + 0xCu, p);
                const float d = distance2(p, u32(player + 0x134u));
                setF0(d);
                putf(slot + 8u, d);
            }
        }
    }

    // ---- 3. trim (pass 0) and spawn (pass 1) -----------------------------------
    void G::passes()
    {
        const uint32_t list = m + 0x11Cu;
        frame(0xE8u, 0);
        frame(0xECu, call1(kBudget, 0x013EF910u, list));
        for (int32_t pass = 0; pass < 2;)
        {
            uint32_t budget = frameWord(0xECu); // s6
            frame(0xE4u, 0);
            if (s32(list) > 0)
            {
                uint32_t s1 = list;
                for (int32_t index = 0;;)
                {
                    const uint32_t rec = s1 + 4u;
                    const uint32_t point = u32(s1 + 0x10u);
                    frame(0xF0u, rec);
                    setF0(0.0f);
                    const float radius = f32(point + 0x24u);
                    float limit;
                    {
                        const uint32_t bits = 0x46AFC800u;
                        std::memcpy(&limit, &bits, 4);
                    }
                    int32_t want = 0; // s5
                    if (lt(0.0f, radius))
                        limit = FPU_MUL_S(radius, radius);
                    uint32_t typeHalf = u16(point + 0x14u);
                    if (u32(type(s16(point + 0x14u)) + 0xCu) == 0)
                        want = s16(point + 0x16u);
                    else
                    {
                        const int32_t bits = s16(point + 0x18u);
                        uint32_t bitsHalf = u16(point + 0x18u);
                        if (bits > 0)
                        {
                            for (int32_t b = 0;;)
                            {
                                const int32_t bit = s16(point + 0x1Au) + b;
                                if (bit < 0x40)
                                {
                                    bool counts = true;
                                    if (call2(kGexsFlag, 0x013EF9BCu, u32(s1 + 4u), static_cast<uint32_t>(bit)) != 0 &&
                                        call2(kFlagTaken, 0x013EF9D0u, rec, static_cast<uint32_t>(s16(point + 0x1Au) + b)) == 0)
                                    {
                                        bitsHalf = u16(point + 0x18u);
                                        counts = false;
                                    }
                                    if (counts)
                                    {
                                        const int32_t most = s16(point + 0x16u);
                                        bitsHalf = u16(point + 0x18u);
                                        want += want < most ? 1 : 0;
                                    }
                                }
                                ++b;
                                if (!(b < static_cast<int16_t>(bitsHalf)))
                                    break;
                            }
                            typeHalf = u16(point + 0x14u);
                        }
                    }
                    if (u32(type(static_cast<int16_t>(typeHalf)) + 0x20u) != 0)
                    {
                        a(4, typeHalf);
                        if (static_cast<int32_t>(call(kDeadBosses, 0x013EFA44u)) < want)
                            want = static_cast<int32_t>(call(kDeadBosses, 0x013EFA58u));
                    }
                    {
                        const int32_t total = s32(m + 0x9D0u);
                        if (total < static_cast<int32_t>(static_cast<uint32_t>(want) + budget))
                            want = static_cast<int32_t>(static_cast<uint32_t>(total) - budget);
                    }
                    if (frameWord(0xE8u) == 0)
                    {
                        // Pass 0: objects past `want` back to the free list (the head last).
                        int32_t count = s32(s1 + 0x14u);
                        const int32_t first = count;
                        if (want < count)
                        {
                            uint32_t obj = u32(s1 + 0x18u);
                            bool more;
                            if (obj == 0)
                                more = want < count;
                            else
                            {
                                obj = u32(obj + 0x58u);
                                bool stopped = false;
                                if (1 < count)
                                    for (int32_t k = 1;;)
                                    {
                                        const uint32_t busy = u32(obj + 0x54u), next = u32(obj + 0x58u);
                                        if (busy == 0 && obj != 0)
                                        {
                                            if (unlink(s1 + 0x14u, obj))
                                                append(m + 0x238u, obj);
                                            count = s32(s1 + 0x14u);
                                        }
                                        if (!(want < count))
                                        {
                                            want = count;
                                            stopped = true;
                                            break;
                                        }
                                        ++k;
                                        obj = next;
                                        if (!(k < first))
                                            break;
                                    }
                                more = !stopped && want < count;
                                if (stopped)
                                    goto next;
                            }
                            want = count;
                            if (more)
                            {
                                const uint32_t head = u32(s1 + 0x18u);
                                if (u32(head + 0x54u) == 0 && head != 0)
                                {
                                    if (unlink(s1 + 0x14u, head))
                                        append(m + 0x238u, head);
                                    want = s32(s1 + 0x14u);
                                }
                            }
                        }
                    }
                    else if (u32(kNoLoads) == 0 && s32(s1 + 0x14u) < want && u32(kSpawned) == 0)
                    {
                        // Pass 1: one object for the nearest record short of its share.
                        const float dist = f32(s1 + 0xCu);
                        setF0(dist);
                        if (lt(dist, limit))
                        {
                            int32_t flag = -1; // fp
                            uint32_t obj = 0;  // s0
                            bool haveTemplate;
                            if (u32(type(s16(point + 0x14u)) + 0xCu) != 0)
                            {
                                const int32_t bits = s16(point + 0x18u);
                                uint32_t bitsHalf = u16(point + 0x18u);
                                if (bits > 0)
                                    for (int32_t b = 0;;)
                                    {
                                        const int32_t bit = s16(point + 0x1Au) + b;
                                        if (bit < 0x40)
                                        {
                                            if (call2(kGexsFlag, 0x013EFCF4u, u32(s1 + 4u), static_cast<uint32_t>(bit)) == 0 &&
                                                call2(kFlagTaken, 0x013EFD08u, rec, static_cast<uint32_t>(s16(point + 0x1Au) + b)) == 0)
                                            {
                                                flag = s16(point + 0x1Au) + b;
                                                break;
                                            }
                                            bitsHalf = u16(point + 0x18u);
                                        }
                                        ++b;
                                        if (!(b < static_cast<int16_t>(bitsHalf)))
                                            break;
                                    }
                                if (flag == -1)
                                    goto spawned;
                                obj = call2(kTemplateByFlag, 0x013EFD50u, m, static_cast<uint32_t>(flag));
                            }
                            else
                            {
                                a(6, u32(s1 + 4u));
                                obj = call2(kTemplate, 0x013EFDE0u, m, u32(s1 + 8u));
                            }
                            haveTemplate = obj != 0;
                            if (haveTemplate)
                            {
                                // Off the free list (appended to the record's list either way).
                                unlink(m + 0x238u, obj);
                                append(s1 + 0x14u, obj);
                            }
                            else
                            {
                                // The spare list's head.
                                const uint32_t spare = u32(m + 0x248u);
                                if (spare && unlink(m + 0x244u, spare))
                                {
                                    append(s1 + 0x14u, spare);
                                    obj = spare;
                                }
                            }
                        spawned:
                            if (obj)
                            {
                                for (uint32_t q = 0; q < 0x30u; q += 8u)
                                    put64(obj + 0x20u + q, u64(point + q));
                                const uint32_t ty = type(s16(obj + 0x34u));
                                const uint32_t idHalf = u16(s1 + 4u);
                                put16(obj + 0x50u, idHalf);
                                const uint32_t indexHalf = u16(s1 + 8u);
                                put32(obj + 0xCu, static_cast<uint32_t>(flag));
                                put16(obj + 0x52u, indexHalf);
                                if (u32(ty + 0xCu) != 0 &&
                                    call2(kTailsFlag, 0x013EFFD4u, static_cast<uint32_t>(static_cast<int16_t>(idHalf)), static_cast<uint32_t>(flag)) != 0)
                                    put32(obj + 0x10u, 1);
                                if (u32(obj + 8u) == 0)
                                {
                                    put32(obj + 8u, 1);
                                    uint32_t stage = u16(obj + 0x34u);
                                    if (u32(type(s16(obj + 0x34u)) + 0x30u) == 0)
                                    {
                                        const uint32_t name = call1(kStageName, 0x013F0028u, obj);
                                        const uint32_t number = call1(kStageNumber, 0x013F0030u, name);
                                        call1(kLoadStage, 0x013F0038u, number);
                                        stage = u16(obj + 0x34u);
                                    }
                                    const uint32_t loads = type(static_cast<int16_t>(stage)) + 0x30u;
                                    put32(loads, u32(loads) + 1u);
                                }
                                put32(kSpawned, 1);
                            }
                        }
                    }
                next:
                    budget += static_cast<uint32_t>(want);
                    {
                        const int32_t done = static_cast<int32_t>(frameWord(0xE4u)) + 1;
                        s1 += 0x1Cu;
                        frame(0xE4u, static_cast<uint32_t>(done));
                        budget -= call1(kFreeSlot, 0x013F0098u, frameWord(0xF0u));
                        index = done;
                        if (!(index < s32(list)))
                            break;
                    }
                }
            }
            if (u32(kFreed) == 0 && frameWord(0xE8u) == 1 && u32(m + 0x23Cu) != 0)
            {
                a(4, m);
                a(5, m + 0x238u);
                a(6, 0);
                a(7, 1);
                put32(kFreed, call(kFreeAll, 0x013F00ECu));
            }
            pass = static_cast<int32_t>(frameWord(0xE8u)) + 1;
            frame(0xE8u, static_cast<uint32_t>(pass));
        }
    }

    // ---- 4. one model load a frame -----------------------------------------------
    void G::loads()
    {
        const uint32_t list = m + 0x11Cu;
        int32_t count = s32(list); // a1
        frame(0xE4u, 0);
        if (count <= 0)
            return;
        frame(0x104u, list);
        for (;;)
        {
            const uint32_t at = frameWord(0x104u);
            uint32_t obj = u32(at + 0x18u); // s1
            frame(0xF8u, 0);
            frame(0xF4u, u32(at + 0x10u));
            if (obj)
            {
                for (uint32_t o = obj; o;)
                {
                    const uint32_t n = frameWord(0xF8u);
                    const uint32_t model = u32(o);
                    o = u32(o + 0x58u);
                    frame(0xF8u, model ? n + 1u : n);
                }
                // The boot ELF's null handle record (lui at 0x13f0170 + each lw's offset).
                auto nullAt = [&](uint32_t site) { return relocated(rdram, 0x013F0170u, site); };
                for (;;)
                {
                    frame(0xFCu, 0);
                    if (u32(kLoaded) != 0 || u32(obj) != 0 || call1(kStageLoaded, 0x013F01ACu, call1(kStageName, 0x013F01A4u, obj)) == 0)
                    {
                        obj = u32(obj + 0x58u);
                        if (obj)
                            continue;
                        count = s32(list);
                        break;
                    }
                    const uint32_t left = call(jalTarget(rdram, 0x013F01B4u), 0x013F01BCu);
                    float heap;
                    if (static_cast<int32_t>(left) >= 0)
                        heap = FPU_CVT_S_W(static_cast<int32_t>(left));
                    else
                    {
                        heap = FPU_CVT_S_W(static_cast<int32_t>((left & 1u) | (left >> 1)));
                        heap = FPU_ADD_S(heap, heap);
                    }
                    float enough;
                    {
                        const uint32_t bits = 0x49999999u;
                        std::memcpy(&enough, &bits, 4);
                    }
                    setF0(enough);
                    bool stop = false; // FC
                    if (!lt(enough, heap))
                        put32(m + 0x9D8u, 1);
                    else
                    {
                        // The object's model: its stage data -> the model record -> its parts.
                        const uint32_t h = call1(kStageName, 0x013F020Cu, obj);
                        const uint32_t modelAt = element(h, nullAt(0x013F0230u));
                        const uint32_t partsAt = modelAt + 4u;
                        const uint32_t parts = ownerField(partsAt, u32(partsAt), nullAt(0x013F0280u), 0x10u);
                        if (parts != 0)
                        {
                            const uint32_t word = u32(parts);
                            if (ownerField(parts, word, nullAt(0x013F02C8u), 0x10u) != 0)
                            {
                                uint32_t part = ownerField(parts, word, nullAt(0x013F0308u), 0x10u);
                                int32_t partCount = static_cast<int32_t>(ownerField(parts, word, nullAt(0x013F0348u), 0xCu));
                                if (!(partCount < 3))
                                    partCount = 2;
                                bool all = true; // t2: every part's mesh there
                                if (partCount > 0)
                                {
                                    const uint32_t nullRec = relocated(rdram, 0x013F0384u, 0x013F0388u);
                                    for (int32_t k = 0;;)
                                    {
                                        const uint32_t meshAt = part + 4u;
                                        part += 0x4Cu;
                                        if (element(meshAt, nullRec) == 0)
                                        {
                                            all = false;
                                            break;
                                        }
                                        ++k;
                                        if (!(k < partCount))
                                            break;
                                    }
                                }
                                if (all)
                                {
                                    part = ownerField(parts, word, nullAt(0x013F0430u), 0x10u);
                                    if (partCount > 0)
                                    {
                                        float one;
                                        {
                                            const uint32_t bits = 0x3F800000u;
                                            std::memcpy(&one, &bits, 4);
                                        }
                                        const uint32_t fp = sp + 0x50u;
                                        frame(0x100u, kMask18);
                                        for (int32_t k = 0;;)
                                        {
                                            const uint32_t mesh = element(part + 4u, relocated(rdram, 0x013F04A0u, 0x013F04A4u)); // s6
                                            const uint32_t pointData = frameWord(0xF4u);
                                            uint32_t geometry = 0; // s2
                                            const uint32_t layoutObj = u32(obj);
                                            {
                                                uint8_t q[16];
                                                std::memcpy(q, rdram + (pointData & PS2_RAM_MASK & ~0xFu), 16);
                                                std::memcpy(rdram + ((sp + 0x40u) & PS2_RAM_MASK), q, 16);
                                                std::memcpy(rdram + (fp & PS2_RAM_MASK), &ctx->vu0_vf[0], 16);
                                            }
                                            if (layoutObj != 0)
                                            {
                                                a(4, sp);
                                                a(5, layoutObj);
                                                a(6, part + 0x10u);
                                                call(kGeometry, 0x013F0500u);
                                                geometry = sp;
                                            }
                                            else
                                            {
                                                const uint32_t placement = u32(type(s16(pointData + 0x14u)) + 0x18u);
                                                if (placement != 0)
                                                {
                                                    a(4, sp + 0x40u);
                                                    a(5, fp);
                                                    a(6, sp);
                                                    a(7, pointData);
                                                    a(8, frameWord(0xF8u));
                                                    if (k == 0)
                                                        a(9, kMask18);
                                                    a(10, kMask18);
                                                    geometry = call(placement, 0x013F0548u);
                                                    if (u32(type(s16(frameWord(0xF4u) + 0x14u)) + 0x1Cu) != 0 && geometry == 0)
                                                    {
                                                        frame(0xFCu, 1);
                                                        stop = true;
                                                        break;
                                                    }
                                                }
                                            }
                                            // The part's position (x, y, z, 1) and rotation (-x, y, -z, 1), its quaternion.
                                            float f = f32(part + 0x30u);
                                            putf(sp + 0xD0u, f);
                                            putf(sp + 0xD4u, f32(part + 0x34u));
                                            f = f32(part + 0x38u);
                                            putf(sp + 0xDCu, one);
                                            putf(sp + 0xD8u, f);
                                            put64(sp + 0xC0u, u64(sp + 0xD0u));
                                            put64(sp + 0xC8u, u64(sp + 0xD8u));
                                            f = FPU_NEG_S(f32(part + 0x18u));
                                            putf(sp + 0xA0u, f);
                                            putf(sp + 0xA4u, f32(part + 0x1Cu));
                                            f = f32(part + 0x20u);
                                            putf(sp + 0xACu, one);
                                            f = FPU_NEG_S(f);
                                            putf(sp + 0xA8u, f);
                                            setF0(f);
                                            ctx->f[13] = FPU_NEG_S(f32(part + 0x28u));
                                            ctx->f[12] = f32(part + 0x24u);
                                            ctx->f[14] = f32(part + 0x2Cu);
                                            a(4, sp + 0xB0u);
                                            call(jalTarget(rdram, 0x013F05E8u), 0x013F05F0u);
                                            // loadObj's description (sp + 0x60).
                                            put32(sp + 0x6Cu, sp + 0xC0u);
                                            put32(sp + 0x64u, sp + 0xA0u);
                                            put32(sp + 0x80u, geometry);
                                            put32(sp + 0x60u, 0);
                                            put32(sp + 0x68u, sp + 0xB0u);
                                            put32(sp + 0x70u, u32(part + 8u));
                                            put32(sp + 0x74u, element(part + 0x3Cu, relocated(rdram, 0x013F0640u, 0x013F0644u)));
                                            {
                                                const float g = f32(part + 0x40u);
                                                setF0(g);
                                                putf(sp + 0x78u, g);
                                            }
                                            put32(sp + 0x84u, 0);
                                            put32(sp + 0x7Cu, u32(part + 0x44u));
                                            put32(sp + 0x88u, 0);
                                            put32(sp + 0x8Cu, 1);
                                            put32(sp + 0x94u, 0);
                                            put32(sp + 0x90u, u32(part + 0x14u));
                                            put32(sp + 0x98u, 0);
                                            if (k == 0)
                                            {
                                                put32(sp + 0x64u, sp + 0x40u);
                                                put32(sp + 0x60u, relocated(rdram, 0x013F06ACu, 0x013F06B4u));
                                                put32(sp + 0x68u, fp);
                                            }
                                            a(4, mesh);
                                            a(5, sp + 0x60u);
                                            a(6, relocated(rdram, 0x013F06C4u, 0x013F06D4u));
                                            a(7, kLoadName);
                                            a(8, 0);
                                            a(9, 4);
                                            const uint32_t loaded = call(kLoadObj, 0x013F06E8u);
                                            if (k != 0)
                                                put32(obj + 4u, loaded);
                                            else
                                            {
                                                put32(obj, loaded);
                                                if (loaded == 0)
                                                    break;
                                            }
                                            ++k;
                                            part += 0x4Cu;
                                            if (!(k < partCount))
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (!stop)
                        put32(kLoaded, 1);
                    count = s32(list);
                    break;
                }
            }
            const int32_t done = static_cast<int32_t>(frameWord(0xE4u)) + 1;
            frame(0xE4u, static_cast<uint32_t>(done));
            frame(0x104u, at + 0x1Cu);
            if (!(done < count))
                break;
        }
    }

    void smallManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        G g{rdram, ctx, runtime};
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint64_t entryRa = GPR_U64(ctx, 31);
        g.sp = entrySp - 0x170u;
        g.m = GPR_U32(ctx, 4);
        const uint32_t player = GPR_U32(ctx, 5);
        // The frame: the saved registers.
        g.put64(g.sp + 0x130u, GPR_U64(ctx, 20));
        g.put64(g.sp + 0x110u, GPR_U64(ctx, 16));
        g.put64(g.sp + 0x140u, GPR_U64(ctx, 22));
        g.put64(g.sp + 0x118u, GPR_U64(ctx, 17));
        g.put64(g.sp + 0x120u, GPR_U64(ctx, 18));
        g.put64(g.sp + 0x128u, GPR_U64(ctx, 19));
        g.put64(g.sp + 0x138u, GPR_U64(ctx, 21));
        g.put64(g.sp + 0x148u, GPR_U64(ctx, 23));
        g.put64(g.sp + 0x150u, GPR_U64(ctx, 30));
        g.put64(g.sp + 0x158u, entryRa);
        g.putf(g.sp + 0x160u, ctx->f[20]);
        g.tbl = g.u32(relocated(rdram, 0x013EF37Cu, 0x013EF388u));
        g.frame(0xE0u, g.tbl);
        // The new list cleared.
        const float far = g.f32(kFarDistance);
        g.setF0(far);
        g.put32(g.m + 0x9D8u, 0);
        g.put32(g.m + 0x11Cu, 0);
        for (uint32_t r = g.m + 0x120u; r < g.m + 0x120u + 10u * 0x1Cu; r += 0x1Cu)
        {
            g.put32(r, 0xFFFFFFFFu);
            g.put32(r + 4u, 0xFFFFFFFFu);
            g.putf(r + 8u, far);
            g.put32(r + 0xCu, 0);
            g.put32(r + 0x10u, 0);
            g.put32(r + 0x14u, 0);
            g.put32(r + 0x18u, 0);
        }
        g.nearList(player);
        g.merge(player);
        g.passes();
        if (g.u32(kNoLoads) == 0)
            g.loads();
        // The new list over last frame's; v0 / v1 the copy's last two dwords.
        for (uint32_t i = 0; i < 0x11Cu; i += 4u)
            g.put32(g.m + i, g.u32(g.m + 0x11Cu + i));
        SET_GPR_U64(ctx, 2, g.u64(g.m + 0x21Cu));
        SET_GPR_U64(ctx, 3, g.u64(g.m + 0x224u));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        SET_GPR_U64(ctx, 31, entryRa);
        ctx->pc = static_cast<uint32_t>(entryRa);
    }
}

SOTC_DECOMP_FUNCTION(0x013EF378u, gamecore_sub_013EF378, decomp_hot43e::smallManager);
