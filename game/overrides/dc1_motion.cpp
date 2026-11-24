// Round DC1 (SOTC_DC1MOT, A/B dc1mot): the motion resources' accessors and the option nodes as real C++.
//
// Round 41's natives (decomp_motion.cpp) ran on the guest context: arguments from the R5900 registers, every field
// read through an address helper, the relocated type words decoded from GAMECORE's instructions at each call, the
// VU0 arithmetic on the context's register file (written back as the original leaves it), and every call between
// them through the function table (callGuest). Here the same functions are C++ functions on typed views of the
// guest's data (chara_types.h plus the motion tables below) and host pointers into guest RAM; they call each other
// directly. The guest-signature entries registered in the function table stay only for the callers that are not
// converted yet: they unpack the argument registers, call the C++ function, and put v0 / pc back.
//
// The functions (GAMECORE addresses):
//   getBlendLastMotion 0x10B28E0          blendLast()       a blend's last motion (its handle chain, +0x50)
//   GetMotionVersion 0x10B39B0            motionVersion()   the header's version (s16), -1 if none
//   GetMotionNbOptionNodes 0x10B36C8      nbOptionNodes()   the header's option-node count, -1 if none
//   GetMotionOptionNodePhysicalId 0x10B3810 physicalId()    the option node with that physical id, -1 if none
//   GetOptionNodePhysicalNodeId 0x10C6710 optionNodeId()    version >= 5: physicalId, else the index if below the count
//   GetMotionNodeBasePos 0x10B6E80        basePos()         a node's rest position up its parent chain
//   GetMotionNodeBaseQuaternion 0x10B6F60 baseQuaternion()  a node's rest rotation up its parent chain
//   AddCharacterClassOptionNodeSubGeometries 0x10C6798 subGeometries()  each live anime slot's sub geometry
//   manager_sub_010BC4F0 (ExecCharaGeometry's option nodes) optionNodes()
// and the direct calls from the converted callers: ExecCharaGeometry's option nodes (decomp_charageo.cpp) and the
// aim IK's base positions / quaternions (decomp_hot46e.cpp).
//
// What is still a guest call: the functions of the chain that are not converted (GetMotionOptionNodeFloatFrame
// 0x10B6650, AddCharacterClassOptionNodeSubGeometry 0x10C3650, GetMotionOptionNodeBooleanInfoFloatFrame 0x10B3508):
// their arguments go in the argument registers at the original's frame (sp - 0x70 for the option nodes, sp - 0x40
// for the sub geometries), as a jal would.
//
// Exactness. The integer accessors read the same words and compare the same way. The two base functions are the
// VU0 code's operations in its order (ps2VuMulK / AddK / SubK, PS2_VMUL / PS2_VADD: the recompiled code's clamped
// float operations; the operand clamps where the original clamps, the lanes it masks never computed into the
// outputs), on values, not on the register file: the guest memory they write is the same bits. What the entries no
// longer leave behind: the scratch VU0 registers (vf5 / vf6 / vf8 / vf10..vf17 / ACC) and v0 / v1 of the void
// functions. No caller reads them: the callers are compiled C whose VU0 blocks load their own inputs, and natives
// that reload from memory (each converted caller was read; SOTC_DC1MOT_CHECK counts the leftovers that would have
// differed, as information). The relocated words (the empty table, the plain and blend table types) are decoded once
// a game frame per lane from every site the original reads them at, and must agree; if they ever do not, the old
// natives run (counted).
//
// SOTC_DC1MOT_CHECK=1: every call also runs round 41's native on the same input and compares what it wrote (the
// returned value, the output qword, or for the option nodes the quaternion / position / flag arrays and the frame's
// scratch, after a restore) bit for bit: [dc1mot] ... checks N differ D. The old native's results are the ones kept
// in check mode. SOTC_DC1MOT_LOG=<n>: every n game frames a [dc1mot] line (calls by function, checks, differences).
#include "sotc_decomp.h"
#include "chara_types.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_motion
{
    // Round 41's guest-context natives (the old path: SOTC_DC1MOT=0, and the reference of the check).
    void getBlendLastMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getMotionOptionNodePhysicalId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getMotionVersion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getMotionNodeBasePos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getMotionNodeBaseQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getMotionNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getOptionNodePhysicalNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void addSubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void optionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace dc1mot
{
    using chara::Quat;
    using chara::Vec4;
    using chara::u32;
    using chara::s32;
    using chara::s16;
    using chara::u8;
    using chara::f32;

    // ---- Typed views of the motion tables ------------------------------------------------------------------------
    // A handle is a word whose low 18 bits are the distance back to the owning table's pointer word and whose bits
    // 18-29 are the element's index (chara_types.h). The table's type (+8) says what the elements are (the plain
    // motion type, the blend type: GAMECORE data the module is linked to) and their stride (+0xC).
    struct TableType
    {
        u32 w00, w04, w08;
        u32 stride; // +0x0C
    };
    struct MotionTable
    {
        u32 w00, w04;
        u32 type;     // +0x08 -> TableType
        u32 w0C;
        u32 elements; // +0x10 -> the first element
    };
    struct MotionElement // a plain motion's element; a blend's element is a chain of 12-byte handles from +0xC
    {
        u32 w00, w04, w08;
        u32 resource; // +0x0C -> MotionResource (0: not loaded, the original's iosJumpRecoverPoint)
    };
    constexpr uint32_t kBlendLast = 0x50; // a blend element's last motion handle
    struct MotionResource
    {
        u32 w00[8];
        u32 header; // +0x20 -> chara::MotionHeader
    };
    static_assert(offsetof(MotionTable, type) == 8 && offsetof(MotionTable, elements) == 0x10 && offsetof(TableType, stride) == 0xC &&
                      offsetof(MotionElement, resource) == 0xC && offsetof(MotionResource, header) == 0x20,
                  "motion table layout");
    // The option node record of the skeleton (chara::Skeleton::optionNodes, 0x40 each).
    struct OptionNode
    {
        u32 w00[4];
        Vec4 position; // +0x10
        Quat rotation; // +0x20
        s32 parent;    // +0x30
        u32 w34[3];
    };
    static_assert(sizeof(OptionNode) == 0x40 && offsetof(OptionNode, position) == 0x10 && offsetof(OptionNode, rotation) == 0x20, "OptionNode");
    // One of the character's anime slots (class +0x30C, 0x80 each, class +0x3FC of them).
    struct AnimeSlot
    {
        u32 w00;
        u32 sub[16];  // +0x04 the sub geometry's argument (its address is passed)
        u32 state;    // +0x44 -2: empty
        u32 w48[14];
    };
    static_assert(sizeof(AnimeSlot) == 0x80 && offsetof(AnimeSlot, state) == 0x44, "AnimeSlot");
    constexpr uint32_t kEmptySlot = 0xFFFFFFFEu;

    // The character class fields the option nodes use (chara_types.h cls:: names the rest).
    namespace cls
    {
        using namespace chara::cls;
        constexpr uint32_t kOptionQuats = 0x2EC, kOptionPositions = 0x2F0, kOptionFlags = 0x2FC; // -> per option node: quat, pos, 3 bytes
        constexpr uint32_t kSubGeoMode = 0xD90, kSubGeoD20 = 0xD20, kSubGeoBlend = 0x734;
    }
    // The skeleton's option-node list (chara::Skeleton; its +0x20 is the option nodes' count here, the word
    // chara_types.h names ikRecords from another reader; +0x24 the nodes; +0 above 0: version-5 physical ids).
    constexpr uint32_t kSkelOptionCount = 0x20;

    // ---- Guest RAM as host memory ----------------------------------------------------------------------------------
    struct Ram
    {
        uint8_t *base;
        template <class T>
        T *at(uint32_t a) const { return reinterpret_cast<T *>(base + (a & PS2_RAM_MASK)); }
        template <class T>
        T *atq(uint32_t a) const { return reinterpret_cast<T *>(base + (a & PS2_RAM_MASK & ~0xFu)); }
        uint32_t word(uint32_t a) const { return *at<u32>(a); }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, base + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(base + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
    };

    // ---- The relocated words, decoded once a frame per lane ------------------------------------------------------
    // The empty table's pointer word and the two table types, from every site of the original's code that loads
    // them (lui / addiu or lw pairs): they must all agree.
    struct Links
    {
        uint64_t frame = ~0ull;
        uint32_t empty = 0, plain = 0, blend = 0, base = 0; // base: GetMotionNodeBasePos's starting qword (0x1e0fd0)
        uint32_t optionConst = 0;                           // the option nodes' constant qword (0x1e0fe0)
        bool ok = false;
    };
    Links g_links[2];
    struct Stats
    {
        uint64_t calls[12] = {}, checks = 0, differ = 0, leftovers = 0, linkFail = 0, fallback = 0;
    };
    Stats g_st;
    enum Fn { kFBlend, kFVersion, kFNb, kFPhys, kFOptId, kFBasePos, kFBaseQuat, kFSubGeos, kFOptionNodes, kFDirectOption, kFDirectBase, kFCount };
    const char *const kFnNames[kFCount] = {"blendLast", "version", "nbOption", "physicalId", "optionNodeId", "basePos", "baseQuat", "subGeos", "optionNodes", "directOption", "directBase"};

    int g_check = -1, g_log = -1;
    __attribute__((noinline)) void readEnv()
    {
        const char *c = std::getenv("SOTC_DC1MOT_CHECK");
        const char *l = std::getenv("SOTC_DC1MOT_LOG");
        g_check = c ? std::atoi(c) : 0;
        g_log = l ? std::atoi(l) : 0;
    }
    inline bool checking()
    {
        if (__builtin_expect(g_check < 0, 0))
            readEnv();
        return g_check > 0;
    }

    bool on() { return sotc_decomp::switchOn(sotc_decomp::kDc1Mot); }

    __attribute__((noinline)) void report()
    {
        static uint64_t last = ~0ull;
        if (g_log <= 0 || sotc_decomp::g_gameFrame == last || sotc_decomp::g_gameFrame % static_cast<uint64_t>(g_log) != 0u)
            return;
        last = sotc_decomp::g_gameFrame;
        char buf[512];
        int n = 0;
        for (int i = 0; i < kFCount && n < 400; ++i)
            n += std::snprintf(buf + n, sizeof(buf) - n, " %s %llu", kFnNames[i], (unsigned long long)g_st.calls[i]);
        std::fprintf(stderr, "[dc1mot] frame %llu: calls%s; checks %llu differ %llu (leftover registers that differ, not read: %llu); links failed %llu fallback %llu\n",
                     (unsigned long long)sotc_decomp::g_gameFrame, buf, (unsigned long long)g_st.checks, (unsigned long long)g_st.differ,
                     (unsigned long long)g_st.leftovers, (unsigned long long)g_st.linkFail, (unsigned long long)g_st.fallback);
    }

    __attribute__((noinline)) void decodeLinks(const uint8_t *m, Links &k)
    {
        using sotc_decomp::relocated;
        // The empty table pointer's sites (blendLast x3, and the three accessors' emptyA / emptyB / emptyC).
        static constexpr uint32_t kEmpty[][2] = {{0x010B2908u, 0x010B290Cu}, {0x010B2940u, 0x010B2948u}, {0x010B29ECu, 0x010B29F0u},
                                                 {0x010B3850u, 0x010B3854u}, {0x010B3898u, 0x010B389Cu}, {0x010B3960u, 0x010B3964u},
                                                 {0x010B39E8u, 0x010B39ECu}, {0x010B3A30u, 0x010B3A34u}, {0x010B3AB0u, 0x010B3AB4u},
                                                 {0x010B3700u, 0x010B3704u}, {0x010B3748u, 0x010B374Cu}, {0x010B37C8u, 0x010B37CCu}};
        static constexpr uint32_t kPlain[][2] = {{0x010B381Cu, 0x010B3868u}, {0x010B39BCu, 0x010B3A00u}, {0x010B36D4u, 0x010B3718u}};
        static constexpr uint32_t kBlend[][2] = {{0x010B3974u, 0x010B397Cu}, {0x010B3AC4u, 0x010B3ACCu}, {0x010B37DCu, 0x010B37E4u}};
        bool ok = true;
        const uint32_t e = relocated(m, kEmpty[0][0], kEmpty[0][1]);
        for (const auto &s : kEmpty)
            ok = ok && relocated(m, s[0], s[1]) == e;
        const uint32_t p = relocated(m, kPlain[0][0], kPlain[0][1]);
        for (const auto &s : kPlain)
            ok = ok && relocated(m, s[0], s[1]) == p;
        const uint32_t b = relocated(m, kBlend[0][0], kBlend[0][1]);
        for (const auto &s : kBlend)
            ok = ok && relocated(m, s[0], s[1]) == b;
        // The option nodes' call sites must name the functions this file stands for (as round 41 checks them).
        static constexpr uint32_t kSites[8][2] = {{0x010BC590u, 0x010C6710u}, {0x010BC5B0u, 0x010B6650u}, {0x010BC5F4u, 0x010C6798u},
                                                  {0x010BC640u, 0x010C3650u}, {0x010BC68Cu, 0x010B39B0u}, {0x010BC6A4u, 0x010B3810u},
                                                  {0x010BC6C0u, 0x010B3508u}, {0x010C67F8u, 0x010C3650u}};
        for (const auto &s : kSites)
            ok = ok && sotc_decomp::jalTarget(m, s[0]) == s[1];
        k.empty = e;
        k.plain = p;
        k.blend = b;
        k.base = relocated(m, 0x010B6E80u, 0x010B6E88u);
        k.optionConst = relocated(m, 0x010BC5C4u, 0x010BC5C8u);
        k.ok = ok;
        if (!ok)
            ++g_st.linkFail;
    }
    inline const Links &links(const uint8_t *m, const PS2Runtime *runtime)
    {
        Links &k = g_links[sotc_decomp::lane(runtime)];
        if (__builtin_expect(k.frame != sotc_decomp::g_gameFrame, 0))
        {
            k.frame = sotc_decomp::g_gameFrame;
            decodeLinks(m, k);
            if (g_log > 0)
                report();
        }
        return k;
    }

    // ---- The accessors ---------------------------------------------------------------------------------------------
    // The table a handle word at h belongs to (0: none, or the empty table).
    inline uint32_t tableOf(const Ram r, const Links &k, uint32_t h)
    {
        const uint32_t v = r.word(h), off = chara::handleOffset(v);
        const uint32_t t = off ? r.word(h - off) : 0u;
        return t != 0u && t != r.word(k.empty) ? t : 0u;
    }
    // The element the handle at h names (0: none).
    inline uint32_t elementOf(const Ram r, const Links &k, uint32_t h)
    {
        const uint32_t t = tableOf(r, k, h);
        if (t == 0u)
            return 0u;
        const MotionTable *tab = r.at<MotionTable>(t);
        return tab->elements + chara::handleIndex(r.word(h)) * r.at<TableType>(tab->type)->stride;
    }
    // getBlendLastMotion: the last handle of the blend's chain that names an element; that element's +0x50.
    uint32_t blendLast(const Ram r, const Links &k, uint32_t h)
    {
        uint32_t chain = elementOf(r, k, h);
        while (elementOf(r, k, chain + 0xCu) != 0u)
            chain += 0xCu;
        return elementOf(r, k, chain) + kBlendLast;
    }
    // The plain motion's element a handle leads to through blends (0: neither type). missing: its resource is not
    // loaded (the original longjmps there: the callers go to the recompiled code).
    uint32_t plainMotion(const Ram r, const Links &k, uint32_t h, bool &missing)
    {
        missing = false;
        for (;;)
        {
            const uint32_t t = tableOf(r, k, h);
            const uint32_t type = r.at<MotionTable>(t)->type; // (t 0: word 8 of guest RAM, as the original reads it)
            if (type == k.plain)
            {
                const uint32_t e = elementOf(r, k, h);
                missing = r.at<MotionElement>(e)->resource == 0u;
                return e;
            }
            if (type != k.blend)
                return 0u;
            h = blendLast(r, k, h);
        }
    }
    inline const chara::MotionHeader *headerOf(const Ram r, uint32_t element)
    {
        return r.at<chara::MotionHeader>(r.at<MotionResource>(r.at<MotionElement>(element)->resource)->header);
    }
    int32_t motionVersion(const Ram r, const Links &k, uint32_t h, bool &missing)
    {
        const uint32_t e = plainMotion(r, k, h, missing);
        return e != 0u ? headerOf(r, e)->version : -1;
    }
    int32_t nbOptionNodes(const Ram r, const Links &k, uint32_t h, bool &missing)
    {
        const uint32_t e = plainMotion(r, k, h, missing);
        return e != 0u && !missing ? headerOf(r, e)->optionNodes : -1;
    }
    // The option node whose physical id byte equals id (the whole register compared), -1 if none.
    int32_t physicalId(const Ram r, const Links &k, uint32_t h, uint64_t id, bool &missing)
    {
        const uint32_t e = plainMotion(r, k, h, missing);
        if (e == 0u || missing)
            return -1;
        const chara::MotionHeader *hd = headerOf(r, e);
        const int32_t n = hd->optionNodes;
        const uint32_t ids = hd->optionIds;
        for (int32_t i = 0; i < n; ++i)
            if (*r.at<u8>(ids + static_cast<uint32_t>(i)) == id)
                return i;
        return -1;
    }
    // GetOptionNodePhysicalNodeId(list, motion, index): false = a motion without its resource on the way.
    bool optionNodeId(const Ram r, const Links &k, uint32_t list, uint32_t motion, uint64_t index, int64_t &id)
    {
        bool missing = false;
        if (*r.at<s32>(list) > 0)
        {
            const int32_t version = motionVersion(r, k, motion, missing);
            if (missing)
                return false;
            if (version >= 5)
            {
                id = physicalId(r, k, motion, index, missing);
                return !missing;
            }
        }
        const int32_t n = nbOptionNodes(r, k, motion, missing);
        if (missing)
            return false;
        id = static_cast<int64_t>(index) < n ? static_cast<int64_t>(index) : -1;
        return true;
    }

    // ---- The base position / rotation of a node --------------------------------------------------------------------
    inline __m128 vf0() { return _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f); }
    inline __m128 sel(__m128 d, __m128 s, int x, int y, int z, int w)
    {
        return _mm_blendv_ps(d, s, _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)));
    }
    inline __m128 yzx(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    inline __m128 zxy(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }
    template <int L>
    inline __m128 splat(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    // The point p turned by the unit quaternion q (the original's rows of R(q), w row (0, 0, 0, 1)): the VU0 code's
    // operations in its order. 2x, 2y, 2z from the opmula pairs; the diagonal 1 - 2(..), the off-diagonal sums and
    // differences with w; then p.x row0 + p.y row1 + p.z row2 + p.w (0, 0, 0, 1).
    inline __m128 turnPoint(__m128 q, __m128 p)
    {
        const __m128 z = vf0();
        const __m128 one = ps2VuAddK<1, 1>(z, splat<3>(z));                                      // vaddw.xyz vf12 (1, 1, 1)
        __m128 acc = ps2VuMulK<0, 1>(yzx(q), zxy(one));                                          // vopmula: q.yzx
        const __m128 qq = sel(q, ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(z, z)), 1, 1, 1, 0);      // vmadd vf11 (q.w kept raw)
        const __m128 d2 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(qq, one));                       // vf10 = 2 q.yzx
        const __m128 sq = ps2VuMulK<1, 1>(qq, d2);                                              // vf13: the squares x 2
        acc = ps2VuMulK<1, 1>(yzx(qq), zxy(d2));                                                // the cross terms x 2
        __m128 r15 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(d2, splat<3>(qq)));                   // + w terms
        __m128 r16 = ps2VuSubK<1, 1>(acc, ps2VuMulK<1, 0>(d2, splat<3>(qq)));                   // - w terms
        acc = ps2VuMulK<1, 1>(yzx(sq), zxy(one));
        __m128 r17 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(sq, one));                            // pair sums of squares
        acc = ps2VuMulK<1, 1>(yzx(r15), zxy(one));
        r15 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(z, z));                                       // r15 rotated to y z x
        __m128 r14 = ps2VuSubK<1, 1>(one, r17);                                                 // the diagonal
        // Rows 14..16 with w = 0 (vmr32.w of vf0), then the lane moves of the original (y and z swapped round).
        const __m128 zero = _mm_setzero_ps();
        r14 = sel(r14, zero, 0, 0, 0, 1);
        r15 = sel(r15, zero, 0, 0, 0, 1);
        r16 = sel(r16, zero, 0, 0, 0, 1);
        r17 = sel(r17, r14, 0, 1, 1, 0);
        r14 = sel(r14, r16, 0, 1, 0, 0);
        r16 = sel(r16, r15, 0, 1, 0, 0);
        r15 = sel(r15, r17, 0, 1, 0, 0);
        r14 = sel(r14, r15, 0, 0, 1, 0);
        r15 = sel(r15, r16, 0, 0, 1, 0);
        r16 = sel(r16, r17, 0, 0, 1, 0);
        acc = ps2VuMulK<1, 0>(r14, splat<0>(p));
        acc = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(r15, splat<1>(p)));
        acc = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(r16, splat<2>(p)));
        return ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(z, splat<3>(p)));
    }
    // GetMotionNodeBasePos(out, skel, quats, node): out = the starting qword, then for node up to the root: out turned
    // by the node's quaternion, plus the bone's offset (xyz). node64 all ones: no chain (the original's 64-bit test).
    void basePos(const Ram r, const Links &k, uint32_t out, uint32_t skel, uint32_t quats, uint64_t node64)
    {
        __m128 p = r.q(k.base);
        if (node64 != ~0ull)
        {
            const uint32_t bones = r.at<chara::Skeleton>(skel)->bones;
            uint32_t node = static_cast<uint32_t>(node64);
            do
            {
                const uint32_t bone = bones + (node << 6);
                const __m128 t = turnPoint(r.q(quats + (node << 4)), p);
                p = sel(t, PS2_VADD(t, r.q(bone + offsetof(chara::Bone, offset))), 1, 1, 1, 0);
                node = *r.at<u32>(bone + offsetof(chara::Bone, parent));
            } while (node != 0xFFFFFFFFu);
        }
        r.putq(out, p);
    }
    // The quaternion product q x p of the original (vmul, the w sums, the cross product as opmula / opmsub).
    inline __m128 quatMul(__m128 q, __m128 p)
    {
        __m128 m = PS2_VMUL(q, p);
        m = sel(m, ps2VuAddK<1, 1>(m, splat<1>(m)), 1, 0, 0, 0);
        m = sel(m, ps2VuAddK<1, 1>(m, splat<2>(m)), 1, 0, 0, 0);
        m = sel(m, ps2VuSubK<1, 1>(m, splat<0>(m)), 0, 0, 0, 1);       // w = qw pw - (qx px + qy py + qz pz)
        const __m128 a = PS2_VMUL(p, splat<3>(q));                     // p.xyz qw
        const __m128 b = PS2_VMUL(q, splat<3>(p));                     // q.xyz pw
        const __m128 c = ps2VuSubK<1, 1>(PS2_VMUL(yzx(p), zxy(q)), PS2_VMUL(yzx(q), zxy(p)));
        m = sel(m, ps2VuAddK<1, 1>(a, b), 1, 1, 1, 0);
        return sel(m, ps2VuAddK<1, 1>(m, c), 1, 1, 1, 0);
    }
    // GetMotionNodeBaseQuaternion(out, skel, quats, node): out = (0, 0, 0, 1), then out = quats[node] x out up the chain.
    void baseQuaternion(const Ram r, uint32_t out, uint32_t skel, uint32_t quats, uint64_t node64)
    {
        __m128 o = vf0();
        if (node64 != ~0ull)
        {
            const uint32_t bones = r.at<chara::Skeleton>(skel)->bones;
            uint32_t node = static_cast<uint32_t>(node64);
            do
            {
                o = quatMul(r.q(quats + (node << 4)), o);
                node = *r.at<u32>(bones + (node << 6) + offsetof(chara::Bone, parent));
            } while (node != 0xFFFFFFFFu);
        }
        r.putq(out, o);
    }

    // ---- Guest calls into what is not converted yet --------------------------------------------------------------
    struct Guest
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        void call(uint32_t target, uint32_t site, uint32_t sp) const { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp); }
        void a(int reg, uint32_t v) const { SET_GPR_U32(ctx, reg, v); }
        void a64(int reg, uint64_t v) const { SET_GPR_U64(ctx, reg, v); }
        void s(int reg, int32_t v) const { SET_GPR_S32(ctx, reg, v); }
    };
    constexpr uint32_t kSubGeometry = 0x010C3650u, kOptionFloatFrame = 0x010B6650u, kOptionBoolean = 0x010B3508u;

    // AddCharacterClassOptionNodeSubGeometries(quat, pos, chara, index): for each live anime slot,
    // AddCharacterClassOptionNodeSubGeometry(quat, pos, chara, slot + 4, index) (guest; the count re-read after each).
    void subGeometries(const Ram r, const Guest &g, uint32_t quat, uint32_t pos, uint32_t chara, uint64_t index, uint32_t sp0)
    {
        const uint32_t sp = sp0 - 0x40u;
        int32_t n = *r.at<s32>(chara + cls::kAnimeSlotCount);
        uint32_t slot = *r.at<u32>(chara + cls::kAnimeSlots);
        for (int32_t i = 0; i < n; ++i, slot += sizeof(AnimeSlot))
        {
            if (r.at<AnimeSlot>(slot)->state == kEmptySlot)
                continue;
            g.a(4, quat);
            g.a(5, pos);
            g.a(6, chara);
            g.a(7, slot + offsetof(AnimeSlot, sub));
            g.a64(8, index);
            g.call(kSubGeometry, 0x010C67F8u, sp);
            n = *r.at<s32>(chara + cls::kAnimeSlotCount);
        }
    }

    // manager_sub_010BC4F0(chara): each option node's quaternion / position for the frame (from the motion when the
    // node is in it, else the node's rest values), its sub geometries, the position scaled by the class scale, and
    // its three boolean flags. false: the first node met a motion without its resource before anything was written
    // (the caller runs the original from the start).
    bool optionNodes(const Ram r, const Links &k, const Guest &g, uint32_t chara, uint32_t sp0)
    {
        const uint32_t sp = sp0 - 0x70u;
        uint32_t list = *r.at<u32>(chara + cls::kSkeleton);
        const int32_t count = list != 0u ? *r.at<s32>(list + kSkelOptionCount) : 0;
        uint32_t quat = *r.at<u32>(chara + cls::kOptionQuats), pos = *r.at<u32>(chara + cls::kOptionPositions);
        uint32_t flags = *r.at<u32>(chara + cls::kOptionFlags);
        for (int32_t i = 0; i < count; ++i, pos += 0x10u, quat += 0x10u, flags += 3u)
        {
            if (i > 0)
                list = *r.at<u32>(chara + cls::kSkeleton);
            const uint32_t node = r.at<chara::Skeleton>(list)->optionNodes + static_cast<uint32_t>(i) * sizeof(OptionNode);
            const uint32_t motion = *r.at<u32>(chara + cls::kMotion);
            const uint32_t motionH = motion + 0x50u;
            int64_t id = -1;
            if (motion != 0u && motionH != 0u)
            {
                if (!optionNodeId(r, k, list, motionH, static_cast<uint64_t>(i), id))
                {
                    if (i == 0)
                        return false;
                    g.a(4, list);
                    g.a(5, motionH);
                    g.s(6, i);
                    g.call(0x010C6710u, 0x010BC590u, sp);
                    id = static_cast<int64_t>(GPR_U64(g.ctx, 2));
                }
            }
            if (id >= 0)
            {
                // GetMotionOptionNodeFloatFrame(quat, pos, sp, motion, id, f12 = the frame).
                g.a(4, quat);
                g.a(5, pos);
                g.a(6, sp);
                g.a(7, motionH);
                g.a64(8, static_cast<uint64_t>(id));
                g.ctx->f[12] = *r.at<f32>(chara + cls::kFrameF);
                g.call(kOptionFloatFrame, 0x010BC5B0u, sp);
            }
            else
            {
                r.putq(quat, r.q(node + offsetof(OptionNode, rotation)));
                r.putq(pos, r.q(node + offsetof(OptionNode, position)));
                r.putq(sp, r.q(k.optionConst));
            }
            if (*r.at<chara::u8>(chara + cls::kSubGeo) != 0u)
            {
                subGeometries(r, g, quat, pos, chara, static_cast<uint64_t>(i), sp);
                if (*r.at<u8>(chara + cls::kSubGeoMode) == 2u &&
                    (*r.at<u32>(chara + cls::kSubGeoD20) != 0u || *r.at<f32>(chara + cls::kSubGeoBlend) < 1.0f))
                {
                    g.a(4, quat);
                    g.a(5, pos);
                    g.a(6, chara);
                    g.a(7, chara + cls::kSubGeoD20);
                    g.s(8, i);
                    g.call(kSubGeometry, 0x010BC640u, sp);
                }
            }
            // The position x the class scale (xyz).
            const __m128 p = r.q(pos);
            r.putq(pos, sel(p, PS2_VMUL(p, r.q(chara + cls::kScale)), 1, 1, 1, 0));
            const uint32_t m1 = *r.at<u32>(chara + cls::kMotion);
            const uint32_t m = m1 != 0u ? m1 + 0x50u : 0u;
            const uint32_t list2 = *r.at<u32>(chara + cls::kSkeleton);
            *r.at<u8>(flags + 2u) = 0u;
            *r.at<u8>(flags + 1u) = 0u;
            *r.at<u8>(flags) = 0u;
            if (m != 0u && *r.at<s32>(list2) > 0)
            {
                bool missing = false;
                int32_t version = motionVersion(r, k, m, missing);
                int32_t pid = -1;
                if (!missing && version >= 5)
                    pid = physicalId(r, k, m, static_cast<uint64_t>(i), missing);
                if (missing)
                {
                    // The guest functions for this node, as the original calls them (they longjmp).
                    g.a(4, m);
                    g.call(0x010B39B0u, 0x010BC68Cu, sp);
                    version = static_cast<int32_t>(GPR_U32(g.ctx, 2));
                    pid = -1;
                    if (version >= 5)
                    {
                        g.a(4, m);
                        g.s(5, i);
                        g.call(0x010B3810u, 0x010BC6A4u, sp);
                        pid = static_cast<int32_t>(GPR_U32(g.ctx, 2));
                    }
                }
                if (version >= 5 && pid >= 0)
                {
                    // GetMotionOptionNodeBooleanInfoFloatFrame(flags, motion, pid, f12 / f13 = the frames).
                    g.a(4, flags);
                    g.a(5, m);
                    g.s(6, pid);
                    g.ctx->f[12] = *r.at<f32>(chara + cls::kFrameF);
                    g.ctx->f[13] = *r.at<f32>(chara + cls::kFrameF2);
                    g.call(kOptionBoolean, 0x010BC6C0u, sp);
                }
            }
        }
        return true;
    }

    // ---- The check (SOTC_DC1MOT_CHECK=1) ----------------------------------------------------------------------------
    __attribute__((noinline)) void noteCheck(int fn, bool same, uint32_t a, uint32_t b)
    {
        ++g_st.checks;
        if (!same)
        {
            ++g_st.differ;
            if (g_st.differ <= 20u)
                std::fprintf(stderr, "[dc1chk] frame %llu: %s differs (%08x / %08x)\n", (unsigned long long)sotc_decomp::g_gameFrame, kFnNames[fn], a, b);
        }
    }
    // Round 41's native run as a call from here (a0.. set by the caller; ra / sp as a jal at the entry's own).
    template <class F>
    inline void runOld(F f, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        f(rdram, ctx, runtime);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
    }
    // The leftover registers the old base functions write and the new ones do not (counted, not compared).
    struct Leftover
    {
        __m128 vf[18];
        __m128 acc;
        void save(const R5900Context *c)
        {
            std::memcpy(vf, c->vu0_vf, sizeof(vf));
            acc = c->vu0_acc;
        }
        bool same(const R5900Context *c) const { return std::memcmp(vf, c->vu0_vf, sizeof(vf)) == 0 && std::memcmp(&acc, &c->vu0_acc, 16) == 0; }
    };

    // ---- The guest-signature entries (the function table) -----------------------------------------------------------
    inline void ret(R5900Context *ctx) { ctx->pc = GPR_U32(ctx, 31); }
    inline void retV0(R5900Context *ctx, int64_t v0)
    {
        SET_GPR_U64(ctx, 2, static_cast<uint64_t>(v0));
        ctx->pc = GPR_U32(ctx, 31);
    }
#define DC1_ENTRY_PRELUDE(OLD)                                                                                            \
    if (__builtin_expect(!on(), 0))                                                                                       \
        return decomp_motion::OLD(rdram, ctx, runtime);                                                                   \
    const Links &k = links(rdram, runtime);                                                                               \
    if (__builtin_expect(!k.ok || !sotc_decomp::motionOn(), 0))                                                           \
    {                                                                                                                     \
        ++g_st.fallback;                                                                                                  \
        return decomp_motion::OLD(rdram, ctx, runtime);                                                                   \
    }                                                                                                                     \
    const Ram r{rdram};

    // Integer accessors: value from the C++ function; the check compares v0 with the old native's.
    template <class F>
    inline void intEntry(int fn, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, void (*old)(uint8_t *, R5900Context *, PS2Runtime *), F f)
    {
        ++g_st.calls[fn];
        int64_t v = 0;
        if (!f(v))
            return old(rdram, ctx, runtime);
        if (__builtin_expect(checking(), 0))
        {
            runOld(old, rdram, ctx, runtime);
            noteCheck(fn, static_cast<int64_t>(GPR_U64(ctx, 2)) == v, static_cast<uint32_t>(v), GPR_U32(ctx, 2));
            return ret(ctx);
        }
        retV0(ctx, v);
    }
    void entryBlendLast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getBlendLastMotion)
        intEntry(kFBlend, rdram, ctx, runtime, decomp_motion::getBlendLastMotion, [&](int64_t &v) {
            v = static_cast<int32_t>(blendLast(r, k, GPR_U32(ctx, 4)));
            return true;
        });
    }
    void entryVersion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getMotionVersion)
        intEntry(kFVersion, rdram, ctx, runtime, decomp_motion::getMotionVersion, [&](int64_t &v) {
            bool missing;
            v = motionVersion(r, k, GPR_U32(ctx, 4), missing);
            return !missing;
        });
    }
    void entryNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getMotionNbOptionNodes)
        intEntry(kFNb, rdram, ctx, runtime, decomp_motion::getMotionNbOptionNodes, [&](int64_t &v) {
            bool missing;
            v = nbOptionNodes(r, k, GPR_U32(ctx, 4), missing);
            return !missing;
        });
    }
    void entryPhysicalId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getMotionOptionNodePhysicalId)
        intEntry(kFPhys, rdram, ctx, runtime, decomp_motion::getMotionOptionNodePhysicalId, [&](int64_t &v) {
            bool missing;
            v = physicalId(r, k, GPR_U32(ctx, 4), GPR_U64(ctx, 5), missing);
            return !missing;
        });
    }
    void entryOptionNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getOptionNodePhysicalNodeId)
        intEntry(kFOptId, rdram, ctx, runtime, decomp_motion::getOptionNodePhysicalNodeId, [&](int64_t &v) {
            v = -1;
            return optionNodeId(r, k, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U64(ctx, 6), v);
        });
    }

    // The base functions: the output qword compared with the old native's.
    __attribute__((noinline)) void checkBase(bool pos, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t out, __m128 mine)
    {
        Leftover before;
        before.save(ctx);
        runOld(pos ? decomp_motion::getMotionNodeBasePos : decomp_motion::getMotionNodeBaseQuaternion, rdram, ctx, runtime);
        uint32_t a[4], b[4];
        std::memcpy(a, &mine, 16);
        std::memcpy(b, rdram + (out & PS2_RAM_MASK & ~0xFu), 16);
        noteCheck(pos ? kFBasePos : kFBaseQuat, std::memcmp(a, b, 16) == 0, a[0], b[0]);
        if (!before.same(ctx))
            ++g_st.leftovers;
    }
    void entryBasePos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getMotionNodeBasePos)
        ++g_st.calls[kFBasePos];
        const uint32_t out = GPR_U32(ctx, 4);
        if (__builtin_expect(checking(), 0))
        {
            const __m128 keep = r.q(out);
            basePos(r, k, out, GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7));
            const __m128 mine = r.q(out);
            r.putq(out, keep);
            checkBase(true, rdram, ctx, runtime, out, mine);
            return ret(ctx);
        }
        basePos(r, k, out, GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7));
        ret(ctx);
    }
    void entryBaseQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(getMotionNodeBaseQuaternion)
        ++g_st.calls[kFBaseQuat];
        const uint32_t out = GPR_U32(ctx, 4);
        if (__builtin_expect(checking(), 0))
        {
            const __m128 keep = r.q(out);
            baseQuaternion(r, out, GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7));
            const __m128 mine = r.q(out);
            r.putq(out, keep);
            checkBase(false, rdram, ctx, runtime, out, mine);
            return ret(ctx);
        }
        baseQuaternion(r, out, GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7));
        ret(ctx);
    }

    // The option nodes and the sub geometries write the option arrays and call guest code: the check runs the new
    // path, keeps what it wrote, puts the inputs back, runs the old native and compares.
    struct Snapshot
    {
        enum { kMax = 64 };
        uint8_t quat[kMax * 16], pos[kMax * 16], flags[kMax * 3], frame[0x70];
        uint32_t aq = 0, ap = 0, af = 0, as = 0, n = 0;
        bool take(const Ram r, uint32_t chara, uint32_t sp0, int32_t count)
        {
            if (count < 0 || count > kMax)
                return false;
            n = static_cast<uint32_t>(count);
            aq = *r.at<u32>(chara + cls::kOptionQuats);
            ap = *r.at<u32>(chara + cls::kOptionPositions);
            af = *r.at<u32>(chara + cls::kOptionFlags);
            as = sp0 - 0x70u;
            for (uint32_t i = 0; i < n * 16u; ++i)
                quat[i] = *r.at<uint8_t>(aq + i), pos[i] = *r.at<uint8_t>(ap + i);
            for (uint32_t i = 0; i < n * 3u; ++i)
                flags[i] = *r.at<uint8_t>(af + i);
            for (uint32_t i = 0; i < 0x70u; ++i)
                frame[i] = *r.at<uint8_t>(as + i);
            return true;
        }
        void put(const Ram r) const
        {
            for (uint32_t i = 0; i < n * 16u; ++i)
                *r.at<uint8_t>(aq + i) = quat[i], *r.at<uint8_t>(ap + i) = pos[i];
            for (uint32_t i = 0; i < n * 3u; ++i)
                *r.at<uint8_t>(af + i) = flags[i];
            for (uint32_t i = 0; i < 0x70u; ++i)
                *r.at<uint8_t>(as + i) = frame[i];
        }
        bool same(const Ram r) const
        {
            for (uint32_t i = 0; i < n * 16u; ++i)
                if (*r.at<uint8_t>(aq + i) != quat[i] || *r.at<uint8_t>(ap + i) != pos[i])
                    return false;
            for (uint32_t i = 0; i < n * 3u; ++i)
                if (*r.at<uint8_t>(af + i) != flags[i])
                    return false;
            return true; // (the frame's scratch holds the callees' locals: not an output)
        }
    };
    Snapshot g_before, g_after;

    // The option nodes from a converted caller or the entry: ra / sp are put back as after a jal.
    // false: the original must run from the start (nothing written).
    bool optionNodesRun(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Links &k, uint32_t chara, uint32_t ra, uint32_t sp0, int fn)
    {
        const Ram r{rdram};
        const Guest g{rdram, ctx, runtime};
        ++g_st.calls[fn];
        if (__builtin_expect(checking(), 0) && sotc_decomp::lane(runtime) == 0)
        {
            const uint32_t list = *r.at<u32>(chara + cls::kSkeleton);
            const int32_t count = list != 0u ? *r.at<s32>(list + kSkelOptionCount) : 0;
            if (g_before.take(r, chara, sp0, count))
            {
                if (!optionNodes(r, k, g, chara, sp0))
                {
                    g_before.put(r);
                    return false;
                }
                g_after.take(r, chara, sp0, count);
                g_before.put(r);
                SET_GPR_U32(ctx, 4, chara);
                SET_GPR_U32(ctx, 29, sp0);
                SET_GPR_U32(ctx, 31, ra);
                decomp_motion::optionNodes(rdram, ctx, runtime);
                noteCheck(kFOptionNodes, g_after.same(r), chara, static_cast<uint32_t>(count));
                SET_GPR_U32(ctx, 29, sp0);
                SET_GPR_U32(ctx, 31, ra);
                return true;
            }
        }
        if (!optionNodes(r, k, g, chara, sp0))
            return false;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra);
        return true;
    }
    void entryOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(optionNodes)
        (void)r;
        if (!optionNodesRun(rdram, ctx, runtime, k, GPR_U32(ctx, 4), GPR_U32(ctx, 31), GPR_U32(ctx, 29), kFOptionNodes))
            return sotc_decomp::recompiled(0x010BC4F0u)(rdram, ctx, runtime);
        ret(ctx);
    }
    void entrySubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        DC1_ENTRY_PRELUDE(addSubGeometries)
        ++g_st.calls[kFSubGeos];
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        subGeometries(r, Guest{rdram, ctx, runtime}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7), sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        ret(ctx);
    }
#undef DC1_ENTRY_PRELUDE

    // ---- Direct calls from the converted callers --------------------------------------------------------------------
    // ExecCharaGeometry's option nodes (its jal at 0x10BFC60, frame sp).
    void optionNodesCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t chara, uint32_t sp)
    {
        constexpr uint32_t kSite = 0x010BFC60u;
        if (on() && sotc_decomp::motionOn())
        {
            const Links &k = links(rdram, runtime);
            if (k.ok && optionNodesRun(rdram, ctx, runtime, k, chara, kSite + 8u, sp, kFDirectOption))
                return;
        }
        SET_GPR_U32(ctx, 4, chara);
        sotc_decomp::callGuest(rdram, ctx, runtime, 0x010BC4F0u, kSite + 8u, sp);
    }
    // The aim IK's GetMotionNodeBasePos / GetMotionNodeBaseQuaternion(out, skel, quats, node) (true: done here).
    bool baseCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool pos, uint32_t out, uint32_t skel, uint32_t quats, uint64_t node, uint32_t site, uint32_t sp)
    {
        if (!on() || !sotc_decomp::motionOn())
            return false;
        const Links &k = links(rdram, runtime);
        if (!k.ok)
            return false;
        ++g_st.calls[kFDirectBase];
        const Ram r{rdram};
        if (__builtin_expect(checking(), 0))
        {
            const __m128 keep = r.q(out);
            if (pos)
                basePos(r, k, out, skel, quats, node);
            else
                baseQuaternion(r, out, skel, quats, node);
            const __m128 mine = r.q(out);
            r.putq(out, keep);
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_U32(ctx, 5, skel);
            SET_GPR_U32(ctx, 6, quats);
            SET_GPR_U64(ctx, 7, node);
            SET_GPR_U32(ctx, 31, site + 8u);
            SET_GPR_U32(ctx, 29, sp);
            checkBase(pos, rdram, ctx, runtime, out, mine);
            return true;
        }
        if (pos)
            basePos(r, k, out, skel, quats, node);
        else
            baseQuaternion(r, out, skel, quats, node);
        return true;
    }
}

namespace dc1mot
{
    // Round D1 (d1native): the dispatchers (the D1 bodies at the end of this file).
    void entryBasePosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryOptionNodesSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // Round D2 (d1native)
    void entryBaseQuatSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entrySubGeometriesSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

SOTC_DECOMP_FUNCTION(0x010B28E0u, getBlendLastMotion, dc1mot::entryBlendLast);
SOTC_DECOMP_FUNCTION(0x010B3810u, GetMotionOptionNodePhysicalId, dc1mot::entryPhysicalId);
SOTC_DECOMP_FUNCTION(0x010B39B0u, GetMotionVersion, dc1mot::entryVersion);
SOTC_DECOMP_FUNCTION(0x010B6E80u, GetMotionNodeBasePos, dc1mot::entryBasePosSw);
D1_ALIAS(0x010B6E80u, GetMotionNodeBasePos, dc1mot::entryBasePosSw, dc1mot::entryBasePos);
SOTC_DECOMP_FUNCTION(0x010B6F60u, GetMotionNodeBaseQuaternion, dc1mot::entryBaseQuatSw);
D1_ALIAS(0x010B6F60u, GetMotionNodeBaseQuaternion, dc1mot::entryBaseQuatSw, dc1mot::entryBaseQuat);
SOTC_DECOMP_FUNCTION(0x010B36C8u, GetMotionNbOptionNodes, dc1mot::entryNbOptionNodes);
SOTC_DECOMP_FUNCTION(0x010C6710u, GetOptionNodePhysicalNodeId, dc1mot::entryOptionNodeId);
SOTC_DECOMP_FUNCTION(0x010C6798u, AddCharacterClassOptionNodeSubGeometries, dc1mot::entrySubGeometriesSw);
D1_ALIAS(0x010C6798u, AddCharacterClassOptionNodeSubGeometries, dc1mot::entrySubGeometriesSw, dc1mot::entrySubGeometries);
SOTC_DECOMP_FUNCTION(0x010BC4F0u, manager_sub_010BC4F0, dc1mot::entryOptionNodesSw);
D1_ALIAS(0x010BC4F0u, manager_sub_010BC4F0, dc1mot::entryOptionNodesSw, dc1mot::entryOptionNodes);

// ---- Round D1 (d1native, d1_native.h): GetMotionNodeBasePos and the option nodes as native C++ -----------------------
// GetMotionNodeBasePos: the adapter reads the four arguments and the decoded starting qword, the core basePosD1 is plain
// vec4 math on typed guest memory (the VU0 ops as d1::v* NEON ops, the operand clamps of the old body dropped: finite
// data; the result clamps kept). The old body leaves no register but pc (DC1's entries): neither does this one.
// manager_sub_010BC4F0 (partial): the same walk on typed memory, the scale product a d1::vmul; its guest calls
// (GetMotionOptionNodeFloatFrame, AddCharacterClassOptionNodeSubGeometry, ...) as the old body makes them.
namespace dc1mot
{
    D1_COUNTER(c_d1BasePos, "GetMotionNodeBasePos");
    D1_COUNTER(c_d1OptionNodes, "manager_sub_010BC4F0");

    // A qword at a 16-byte aligned address (the original's lqc2 / sqc2 drop the low four bits).
    PS2_FORCE_INLINE __m128 d1q(const d1::Mem m, uint32_t a) { return m.q(a & ~0xFu); }
    PS2_FORCE_INLINE void d1putq(const d1::Mem m, uint32_t a, __m128 v) { m.putq(a & ~0xFu, v); }
    // Lane moves (vopmula / vmr32's operand orders): a NEON permute, no arithmetic.
    template <int A, int B, int C, int D>
    PS2_FORCE_INLINE __m128 d1perm(__m128 v)
    {
#if defined(__clang__)
        return __builtin_shufflevector(v, v, A, B, C, D);
#else
        typedef int d1v4i __attribute__((vector_size(16)));
        return __builtin_shuffle(v, d1v4i{A, B, C, D});
#endif
    }
    PS2_FORCE_INLINE __m128 d1yzx(__m128 v) { return d1perm<1, 2, 0, 3>(v); }
    PS2_FORCE_INLINE __m128 d1zxy(__m128 v) { return d1perm<2, 0, 1, 3>(v); }

    // turnPoint as d1 vec4 ops, in the same order (the same NEON operations on the same lanes).
    PS2_FORCE_INLINE __m128 d1TurnPoint(__m128 q, __m128 p)
    {
        const __m128 z = d1::vf0();
        const __m128 mXyz = d1::mask(1, 1, 1, 0), mW = d1::mask(0, 0, 0, 1), mY = d1::mask(0, 1, 0, 0), mZ = d1::mask(0, 0, 1, 0);
        const __m128 one = d1::vadd(z, d1::bc<3>(z));
        __m128 acc = d1::vmul(d1yzx(q), d1zxy(one));
        const __m128 qq = d1::vsel(q, d1::vadd(acc, d1::vmul(z, z)), mXyz);
        const __m128 d2 = d1::vadd(acc, d1::vmul(qq, one));
        const __m128 sq = d1::vmul(qq, d2);
        acc = d1::vmul(d1yzx(qq), d1zxy(d2));
        __m128 r15 = d1::vadd(acc, d1::vmul(d2, d1::bc<3>(qq)));
        __m128 r16 = d1::vsub(acc, d1::vmul(d2, d1::bc<3>(qq)));
        acc = d1::vmul(d1yzx(sq), d1zxy(one));
        __m128 r17 = d1::vadd(acc, d1::vmul(sq, one));
        acc = d1::vmul(d1yzx(r15), d1zxy(one));
        r15 = d1::vadd(acc, d1::vmul(z, z));
        __m128 r14 = d1::vsub(one, r17);
        const __m128 zero = d1::zero4();
        r14 = d1::vsel(r14, zero, mW);
        r15 = d1::vsel(r15, zero, mW);
        r16 = d1::vsel(r16, zero, mW);
        r17 = d1::vsel(r17, r14, d1::mask(0, 1, 1, 0));
        r14 = d1::vsel(r14, r16, mY);
        r16 = d1::vsel(r16, r15, mY);
        r15 = d1::vsel(r15, r17, mY);
        r14 = d1::vsel(r14, r15, mZ);
        r15 = d1::vsel(r15, r16, mZ);
        r16 = d1::vsel(r16, r17, mZ);
        acc = d1::vmul(r14, d1::bc<0>(p));
        acc = d1::vadd(acc, d1::vmul(r15, d1::bc<1>(p)));
        acc = d1::vadd(acc, d1::vmul(r16, d1::bc<2>(p)));
        return d1::vadd(acc, d1::vmul(z, d1::bc<3>(p)));
    }

    // GetMotionNodeBasePos(out, skel, quats, node): start = the starting qword's address (0x1e0fd0, relocated).
    void basePosD1(const d1::Mem m, uint32_t start, uint32_t out, uint32_t skel, uint32_t quats, uint64_t node64)
    {
        __m128 p = d1q(m, start);
        if (node64 != ~0ull)
        {
            const uint32_t bones = m.at<chara::Skeleton>(skel)->bones;
            uint32_t node = static_cast<uint32_t>(node64);
            do
            {
                const uint32_t bone = bones + (node << 6);
                const __m128 t = d1TurnPoint(d1q(m, quats + (node << 4)), p);
                p = d1::vsel(t, d1::vadd(t, d1q(m, bone + offsetof(chara::Bone, offset))), d1::mask(1, 1, 1, 0));
                node = *m.at<u32>(bone + offsetof(chara::Bone, parent));
            } while (node != 0xFFFFFFFFu);
        }
        d1putq(m, out, p);
    }

    void entryBasePosD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!on() || checking(), 0))
            return entryBasePos(rdram, ctx, runtime);
        const Links &k = links(rdram, runtime);
        if (__builtin_expect(!k.ok || !sotc_decomp::motionOn(), 0))
            return entryBasePos(rdram, ctx, runtime);
        ++c_d1BasePos.n;
        ++g_st.calls[kFBasePos];
        basePosD1(d1::Mem{rdram}, k.base, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7));
        ret(ctx);
    }
    void entryBasePosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryBasePosD1(rdram, ctx, runtime);
        else
            entryBasePos(rdram, ctx, runtime);
    }

    // optionNodes on typed memory; the guest calls as the old body makes them (g: the context for those only).
    bool optionNodesD1(const d1::Mem m, const Links &k, const Guest &g, uint32_t chara, uint32_t sp0)
    {
        const uint32_t sp = sp0 - 0x70u;
        const Ram r{m.base};
        uint32_t list = *m.at<u32>(chara + cls::kSkeleton);
        const int32_t count = list != 0u ? *m.at<s32>(list + kSkelOptionCount) : 0;
        uint32_t quat = *m.at<u32>(chara + cls::kOptionQuats), pos = *m.at<u32>(chara + cls::kOptionPositions);
        uint32_t flags = *m.at<u32>(chara + cls::kOptionFlags);
        for (int32_t i = 0; i < count; ++i, pos += 0x10u, quat += 0x10u, flags += 3u)
        {
            if (i > 0)
                list = *m.at<u32>(chara + cls::kSkeleton);
            const uint32_t node = m.at<chara::Skeleton>(list)->optionNodes + static_cast<uint32_t>(i) * sizeof(OptionNode);
            const uint32_t motion = *m.at<u32>(chara + cls::kMotion);
            const uint32_t motionH = motion + 0x50u;
            int64_t id = -1;
            if (motion != 0u && motionH != 0u)
            {
                if (!optionNodeId(r, k, list, motionH, static_cast<uint64_t>(i), id))
                {
                    if (i == 0)
                        return false;
                    g.a(4, list);
                    g.a(5, motionH);
                    g.s(6, i);
                    g.call(0x010C6710u, 0x010BC590u, sp);
                    id = static_cast<int64_t>(GPR_U64(g.ctx, 2));
                }
            }
            if (id >= 0)
            {
                g.a(4, quat);
                g.a(5, pos);
                g.a(6, sp);
                g.a(7, motionH);
                g.a64(8, static_cast<uint64_t>(id));
                g.ctx->f[12] = *m.at<f32>(chara + cls::kFrameF);
                g.call(kOptionFloatFrame, 0x010BC5B0u, sp);
            }
            else
            {
                d1putq(m, quat, d1q(m, node + offsetof(OptionNode, rotation)));
                d1putq(m, pos, d1q(m, node + offsetof(OptionNode, position)));
                d1putq(m, sp, d1q(m, k.optionConst));
            }
            if (*m.at<chara::u8>(chara + cls::kSubGeo) != 0u)
            {
                subGeometries(r, g, quat, pos, chara, static_cast<uint64_t>(i), sp);
                if (*m.at<u8>(chara + cls::kSubGeoMode) == 2u &&
                    (*m.at<u32>(chara + cls::kSubGeoD20) != 0u || d1::lt(*m.at<f32>(chara + cls::kSubGeoBlend), 1.0f)))
                {
                    g.a(4, quat);
                    g.a(5, pos);
                    g.a(6, chara);
                    g.a(7, chara + cls::kSubGeoD20);
                    g.s(8, i);
                    g.call(kSubGeometry, 0x010BC640u, sp);
                }
            }
            // The position x the class scale (xyz).
            const __m128 p = d1q(m, pos);
            d1putq(m, pos, d1::vsel(p, d1::vmul(p, d1q(m, chara + cls::kScale)), d1::mask(1, 1, 1, 0)));
            const uint32_t m1 = *m.at<u32>(chara + cls::kMotion);
            const uint32_t mo = m1 != 0u ? m1 + 0x50u : 0u;
            const uint32_t list2 = *m.at<u32>(chara + cls::kSkeleton);
            *m.at<u8>(flags + 2u) = 0u;
            *m.at<u8>(flags + 1u) = 0u;
            *m.at<u8>(flags) = 0u;
            if (mo != 0u && *m.at<s32>(list2) > 0)
            {
                bool missing = false;
                int32_t version = motionVersion(r, k, mo, missing);
                int32_t pid = -1;
                if (!missing && version >= 5)
                    pid = physicalId(r, k, mo, static_cast<uint64_t>(i), missing);
                if (missing)
                {
                    g.a(4, mo);
                    g.call(0x010B39B0u, 0x010BC68Cu, sp);
                    version = static_cast<int32_t>(GPR_U32(g.ctx, 2));
                    pid = -1;
                    if (version >= 5)
                    {
                        g.a(4, mo);
                        g.s(5, i);
                        g.call(0x010B3810u, 0x010BC6A4u, sp);
                        pid = static_cast<int32_t>(GPR_U32(g.ctx, 2));
                    }
                }
                if (version >= 5 && pid >= 0)
                {
                    g.a(4, flags);
                    g.a(5, mo);
                    g.s(6, pid);
                    g.ctx->f[12] = *m.at<f32>(chara + cls::kFrameF);
                    g.ctx->f[13] = *m.at<f32>(chara + cls::kFrameF2);
                    g.call(kOptionBoolean, 0x010BC6C0u, sp);
                }
            }
        }
        return true;
    }

    void entryOptionNodesD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!on() || checking(), 0))
            return entryOptionNodes(rdram, ctx, runtime);
        const Links &k = links(rdram, runtime);
        if (__builtin_expect(!k.ok || !sotc_decomp::motionOn(), 0))
            return entryOptionNodes(rdram, ctx, runtime);
        ++c_d1OptionNodes.n;
        ++g_st.calls[kFOptionNodes];
        const uint32_t chara = GPR_U32(ctx, 4), ra = GPR_U32(ctx, 31), sp0 = GPR_U32(ctx, 29);
        if (!optionNodesD1(d1::Mem{rdram}, k, Guest{rdram, ctx, runtime}, chara, sp0))
            return sotc_decomp::recompiled(0x010BC4F0u)(rdram, ctx, runtime);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra);
        ret(ctx);
    }
    void entryOptionNodesSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryOptionNodesD1(rdram, ctx, runtime);
        else
            entryOptionNodes(rdram, ctx, runtime);
    }
} // namespace dc1mot

// ---- Round D2 (d1native, d1_native.h): GetMotionNodeBaseQuaternion and the option node's sub geometries ------------------
// GetMotionNodeBaseQuaternion (full): the adapter reads the four arguments, the core baseQuatD1 is the chain of quaternion
// products as d1 vec4 ops (the old body's PS2_VMUL / ps2VuAddK / ps2VuSubK in the same order, lane moves as NEON permutes,
// dest masks as d1::vsel; the operand clamps of its PS2_VMULs dropped: the quaternions are the game's rest rotations and
// products of them, every lane written), typed guest memory. The old body leaves no register but pc: neither does this one.
// AddCharacterClassOptionNodeSubGeometries (partial): the anime slot walk on typed memory, the guest call
// (AddCharacterClassOptionNodeSubGeometry) as the old body makes it.
namespace dc1mot
{
    D1_COUNTER(c_d2BaseQuat, "GetMotionNodeBaseQuaternion");
    D1_COUNTER(c_d2SubGeos, "AddCharacterClassOptionNodeSubGeometries");

    // quatMul as d1 vec4 ops: q x p (the w dot as vmul + two adds + a sub, xyz = p q.w + q p.w + p.yzx q.zxy - q.yzx p.zxy).
    PS2_FORCE_INLINE __m128 d1QuatMul(__m128 q, __m128 p)
    {
        const __m128 mX = d1::mask(1, 0, 0, 0), mW = d1::mask(0, 0, 0, 1), mXyz = d1::mask(1, 1, 1, 0);
        __m128 r = d1::vmul(q, p);
        r = d1::vsel(r, d1::vadd(r, d1::bc<1>(r)), mX);
        r = d1::vsel(r, d1::vadd(r, d1::bc<2>(r)), mX);
        r = d1::vsel(r, d1::vsub(r, d1::bc<0>(r)), mW);
        const __m128 a = d1::vmul(p, d1::bc<3>(q));
        const __m128 b = d1::vmul(q, d1::bc<3>(p));
        const __m128 c = d1::vsub(d1::vmul(d1yzx(p), d1zxy(q)), d1::vmul(d1yzx(q), d1zxy(p)));
        r = d1::vsel(r, d1::vadd(a, b), mXyz);
        return d1::vsel(r, d1::vadd(r, c), mXyz);
    }
    // GetMotionNodeBaseQuaternion(out, skel, quats, node): out = (0, 0, 0, 1), then out = quats[node] x out up the chain.
    void baseQuatD1(const d1::Mem m, uint32_t out, uint32_t skel, uint32_t quats, uint64_t node64)
    {
        __m128 o = d1::vf0();
        if (node64 != ~0ull)
        {
            const uint32_t bones = m.at<chara::Skeleton>(skel)->bones;
            uint32_t node = static_cast<uint32_t>(node64);
            do
            {
                o = d1QuatMul(d1q(m, quats + (node << 4)), o);
                node = *m.at<u32>(bones + (node << 6) + offsetof(chara::Bone, parent));
            } while (node != 0xFFFFFFFFu);
        }
        d1putq(m, out, o);
    }
    void entryBaseQuatD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!on() || checking(), 0))
            return entryBaseQuat(rdram, ctx, runtime);
        const Links &k = links(rdram, runtime);
        if (__builtin_expect(!k.ok || !sotc_decomp::motionOn(), 0))
            return entryBaseQuat(rdram, ctx, runtime);
        ++c_d2BaseQuat.n;
        ++g_st.calls[kFBaseQuat];
        baseQuatD1(d1::Mem{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7));
        ret(ctx);
    }
    void entryBaseQuatSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryBaseQuatD1(rdram, ctx, runtime);
        else
            entryBaseQuat(rdram, ctx, runtime);
    }

    // AddCharacterClassOptionNodeSubGeometries(quat, pos, chara, index): each live anime slot's sub geometry (guest), the
    // count re-read after each call.
    void subGeometriesD1(const d1::Mem m, const Guest &g, uint32_t quat, uint32_t pos, uint32_t chara, uint64_t index, uint32_t sp0)
    {
        const uint32_t sp = sp0 - 0x40u;
        int32_t n = *m.at<s32>(chara + cls::kAnimeSlotCount);
        uint32_t slot = *m.at<u32>(chara + cls::kAnimeSlots);
        for (int32_t i = 0; i < n; ++i, slot += sizeof(AnimeSlot))
        {
            if (m.at<AnimeSlot>(slot)->state == kEmptySlot)
                continue;
            g.a(4, quat);
            g.a(5, pos);
            g.a(6, chara);
            g.a(7, slot + offsetof(AnimeSlot, sub));
            g.a64(8, index);
            g.call(kSubGeometry, 0x010C67F8u, sp);
            n = *m.at<s32>(chara + cls::kAnimeSlotCount);
        }
    }
    void entrySubGeometriesD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!on(), 0))
            return entrySubGeometries(rdram, ctx, runtime);
        const Links &k = links(rdram, runtime);
        if (__builtin_expect(!k.ok || !sotc_decomp::motionOn(), 0))
            return entrySubGeometries(rdram, ctx, runtime);
        ++c_d2SubGeos.n;
        ++g_st.calls[kFSubGeos];
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        subGeometriesD1(d1::Mem{rdram}, Guest{rdram, ctx, runtime}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7), sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        ret(ctx);
    }
    void entrySubGeometriesSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entrySubGeometriesD1(rdram, ctx, runtime);
        else
            entrySubGeometries(rdram, ctx, runtime);
    }
} // namespace dc1mot
