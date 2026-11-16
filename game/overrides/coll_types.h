// Round S2: the collision / physics subsystem's guest data as C++ types (semantic decomp).
//
// The collision library (GAMECORE 0x01111000-0x01136F00) and the characters' root dynamics (0x010D4000-0x010EBA00) keep
// their state in main RAM in the layouts below. docs/collision_map.md ("Data") says where each field comes from;
// docs/collision_s2.md says what S2 keeps host-side. These are views on guest memory (guest RAM is host memory,
// little-endian: a `const coll::Part *` at rdram + address reads the record in place); the layouts are pinned with
// static_asserts. Field names marked "(guess)" come from access patterns only.
//
// Who outside the library reads them (why they stay in guest RAM): the root dynamics and every gameplay client hold
// surfaces by id (SurfaceId) and read the hit record and the finder's id list; the per-part caches (ModifiedManager)
// are the game's own per-frame caches, flushed by FlushCollisionWorkByWorkStruct when an object moves. The views are
// may_alias: any host structure built from them must copy into host memory the compiler can see is separate (S1's
// lesson) before a hot loop.
#pragma once

#include <cstddef>
#include <cstdint>

namespace coll
{
    using f32 = float __attribute__((may_alias));
    using u32 = uint32_t __attribute__((may_alias));
    using s32 = int32_t __attribute__((may_alias));
    using u16 = uint16_t __attribute__((may_alias));
    using u8 = uint8_t __attribute__((may_alias));

    struct Vec4
    {
        f32 x, y, z, w;
    };

    // ---- Surface id (8 bytes) -------------------------------------------------
    // How every client names a surface (hit records, finder lists, keep results, geometry pointers).
    struct SurfaceId
    {
        u32 object; // +0 the layout object (layout id)
        u32 word;   // +4 bit 31: an independent surface (vertices through 01114C70, hit kind 2); clear: a mesh /
                    //    skinned surface (vertices through 01116108, hit kind 3); bits 24-30 the part; bits 0-23 the surface
    };
    static_assert(sizeof(SurfaceId) == 8, "SurfaceId layout");
    inline bool independent(uint32_t word) { return (word & 0x80000000u) != 0u; }
    inline uint32_t partOf(uint32_t word) { return (word >> 24) & 0x7Fu; }
    inline uint32_t surfaceOf(uint32_t word) { return word & 0x00FFFFFFu; }
    inline uint64_t key(uint32_t object, uint32_t word) { return (static_cast<uint64_t>(word) << 32) | object; }

    // ---- Layout object and its layout class -----------------------------------
    // Object +8 the type byte; GetLayoutClass (0x10F1A20): types 9-22 index a table at 0x1194070 (0x10F1A58 -> object
    // + 0x20, 0x10F1A98 -> object + 0x90, 0x10F1AA0 -> none; anything else is computed by a call).
    namespace object
    {
        constexpr uint32_t kType = 0x08;     // u8
        constexpr uint32_t kWalkCount = 0x09; // u8
        constexpr uint32_t kFlags = 0x0A;    // u8, bit 0 hidden (isysGroupForNext skips it)
    }
    namespace layout // the layout class's collision fields
    {
        constexpr uint32_t kScale = 0x90;          // f32: the bound radius multiplier (uniform scale)
        constexpr uint32_t kScaleVec = 0xA0;       // vec4: the local hit point's scale
        constexpr uint32_t kPosition = 0x134;      // -> vec4 the object's position (the bound cylinder's centre)
        constexpr uint32_t kPartToTree = 0x158;    // -> 0x40 a part: the matrices into the tree's space
        constexpr uint32_t kPartWorld = 0x15C;     // -> 0x40 a part: world matrices
        constexpr uint32_t kPartInverse = 0x164;   // -> 0x40 a part: world-to-local matrices
        constexpr uint32_t kPartMove0 = 0x16C;     // -> part matrices (movedPoint)
        constexpr uint32_t kPartMove1 = 0x170;
        constexpr uint32_t kWork = 0x174;          // -> CollisionWork
        constexpr uint32_t kCloth = 0x180;
        constexpr uint32_t kRecord = 0x184;        // -> collision record (+0x20 the DB)
        constexpr uint32_t kParent = 0x194;        // the parent class
        constexpr uint32_t kEnableA = 0x1B8;       // s8: both non-zero = the object takes part in segment tests
        constexpr uint32_t kEnableB = 0x1BB;
        constexpr uint32_t kRecordDB = 0x20;       // record +0x20 -> DB
    }

    // ---- Collision DB (class +0x184 -> +0x20) ---------------------------------
    struct DB
    {
        s32 kind;        // +0x00 < 3: the old 8-byte tree (0x112B890); the ground probe reads +0x3C as ids when < 4
        s32 partCount;   // +0x04
        u32 parts;       // +0x08 -> Part[partCount]
        u32 w0C;         // +0x0C
        u32 w10;         // +0x10
        u32 blend;       // +0x14 blend lists (01116108)
        u32 faces;       // +0x18 skinned face records, 0x40 each (+0x3C the attribute index)
        u32 tree;        // +0x1C -> Tree (0: none)
        u32 w20, w24, w28;
        f32 radius;      // +0x2C the bounding radius (x layout::kScale); 0: always near
        u32 w30;
        u32 attributes;  // +0x34 16-byte attribute records (a 64-bit word read by the id filters)
        s32 waterCount;  // +0x38
        u32 water;       // +0x3C water triangles, 0x20 each (+4 height, +8 / +0xC origin xz, +0x10..+0x1C two more xz)
    };
    static_assert(offsetof(DB, tree) == 0x1C && offsetof(DB, radius) == 0x2C && offsetof(DB, attributes) == 0x34 && offsetof(DB, water) == 0x3C,
                  "DB offsets");

    // ---- Part record (0x90 bytes) ---------------------------------------------
    struct Part
    {
        u32 w00;
        u32 vertices;      // +0x04 (0: no surfaces)
        u8 shortIds;       // +0x08 non-zero: 16-bit ids (0x14-byte surfaces), else 0x34-byte surfaces
        u8 packed;         // +0x09 non-zero: packed int16 vertices (x 2^-15 x scale + offset)
        u8 b0A, b0B;
        u32 scaleOffset;   // +0x0C -> +0 offset, +0x10 scale (packed vertices)
        s32 surfaceCount;  // +0x10
        u32 surfaces;      // +0x14 the surface list
        u32 w18;
        f32 sphereRadius;  // +0x1C the part's sphere (x layout::kScale)
        Vec4 sphereCentre; // +0x20 in the part's space (through layout::kPartWorld)
        u32 w30[8];
        s32 skinCount;     // +0x50 skinned triangles
        u32 skinIndex;     // +0x54 -> u32 indices into DB::faces
        u32 skinSpheres;   // +0x58 -> 16 bytes a triangle (centre, radius)
        f32 skinRadius;    // +0x5C the skinned group's sphere
        Vec4 skinCentre;   // +0x60
        u32 w70[8];
    };
    static_assert(sizeof(Part) == 0x90, "Part layout");
    static_assert(offsetof(Part, surfaceCount) == 0x10 && offsetof(Part, sphereRadius) == 0x1C && offsetof(Part, skinCount) == 0x50 &&
                      offsetof(Part, skinCentre) == 0x60,
                  "Part offsets");

    // Surface records in Part::surfaces.
    struct SurfaceShort // 0x14 bytes
    {
        u16 attribute;   // +0x00 index into DB::attributes
        u16 v[3];        // +0x02 vertex ids
        u16 links[6];    // +0x08 three edge link pairs
    };
    static_assert(sizeof(SurfaceShort) == 0x14, "SurfaceShort layout");
    struct EdgeLong
    {
        u32 link, surface, part;
    };
    struct SurfaceLong // 0x34 bytes
    {
        u32 v[3];        // +0x00 vertex ids
        EdgeLong edge[3]; // +0x0C
        u32 attribute;   // +0x30
    };
    static_assert(sizeof(SurfaceLong) == 0x34, "SurfaceLong layout");

    // ---- Tree (DB +0x1C) and its 8-byte node ----------------------------------
    struct Tree
    {
        u32 ids;      // +0x00 -> u16 surface ids
        u32 leaves;   // +0x04 -> u32: count in the low byte, the first id's index above it
        u32 nodes;    // +0x08 -> Node
        u32 root;     // +0x0C the root's index
        u32 oldNodes; // +0x10 non-zero: the old 16-byte node format (0x112ACD0)
    };
    struct Node
    {
        f32 split;    // x the split scale
        u32 word;     // bits 0-1 axis; bits 2-15 low child (leaf when bit 16); bits 17-30 high child (leaf when bit 31)
    };
    static_assert(sizeof(Node) == 8, "Node layout");
    inline uint32_t nodeAxis(uint32_t w) { return w & 3u; }
    inline uint32_t nodeLow(uint32_t w) { return (w >> 2) & 0x3FFFu; }
    inline bool nodeLowLeaf(uint32_t w) { return (w & 0x10000u) != 0u; }
    inline uint32_t nodeHigh(uint32_t w) { return (w >> 17) & 0x3FFFu; }
    inline bool nodeHighLeaf(uint32_t w) { return (w & 0x80000000u) != 0u; }

    // ---- Hit record (0x50 bytes): a segment test's result ---------------------
    struct Hit
    {
        SurfaceId id;  // +0x00
        f32 u, v;      // +0x08 barycentric
        Vec4 local;    // +0x10 the local hit point of an independent surface (class +0x164, x class +0xA0)
        Vec4 w20;
        s32 kind;      // +0x30 2 independent, 3 mesh
        u32 w34[3];
        Vec4 point;    // +0x40 the hit point (the segment's new end)
    };
    static_assert(sizeof(Hit) == 0x50 && offsetof(Hit, kind) == 0x30 && offsetof(Hit, point) == 0x40, "Hit layout");

    // ---- Modified manager: the game's per-frame cache of one kind -------------
    // EntryModifiedManager appends an index and sets its flag byte; FlushModifiedManager clears the listed flags.
    struct ModifiedManager
    {
        s32 count;     // +0x00 listed entries
        u32 w04;
        u32 flags;     // +0x08 -> u8 a slot: set = made since the last flush
        s32 indexSize; // +0x0C 1 / 2 / 4
        u32 list;      // +0x10 -> the listed indices
        u32 values;    // +0x14 -> 16 bytes a slot: a vertex, a plane (n.xyz, w = -n.v0) or a boundary (centre, r^2)
    };
    static_assert(sizeof(ModifiedManager) == 0x18, "ModifiedManager layout");

    // ---- Collision work (class +0x174) ----------------------------------------
    namespace work
    {
        constexpr uint32_t kFinderLinks = 0x10; // head of the finder entries' two-way links
        constexpr uint32_t kFlushFlag = 0x1C;   // cleared by the flush
        constexpr uint32_t kParts = 0x24;       // -> per part 0x60 bytes: +0x00 vertices, +0x18 a fourth list, +0x30 planes, +0x48 boundaries
        constexpr uint32_t kSkinVertices = 0x28;
        constexpr uint32_t kSkinPlanes = 0x58;
        constexpr uint32_t kSkinBoundaries = 0x70;
        constexpr uint32_t kPartStride = 0x60, kPartVertices = 0x00, kPartPlanes = 0x30, kPartBoundaries = 0x48;
    }

    // ---- Finder: a contact list (the ground fill's output) --------------------
    struct Finder
    {
        s32 count;     // +0x00
        s32 capacity;  // +0x04 (the smaller of count / capacity bounds the list)
        s32 keepIndex; // +0x08 (GetCollisionDBCallBack)
        u32 ids;       // +0x0C -> SurfaceId[capacity]
        u32 links;     // +0x10 -> 12 bytes each: next, prev, the id's address
    };
    static_assert(sizeof(Finder) == 0x14, "Finder layout");

    // The finder leaf's query (a0 of 0x1115798 / 0x1115A50).
    struct FinderQuery
    {
        u32 finder;     // +0x00
        u32 object;     // +0x04
        u32 layout;     // +0x08
        u32 w0C;        // +0x0C the filters' word
        u32 w10, w14;
        f32 radius;     // +0x18
        u32 w1C;
        u32 centre;     // +0x20 (address)
        u32 w24;
        u32 boundaryFilter; // +0x28
        u32 idFilter;   // +0x2C
        u32 w30;        // +0x30 the filters' second word
        s32 firstNew;   // +0x34
    };
    static_assert(offsetof(FinderQuery, radius) == 0x18 && offsetof(FinderQuery, firstNew) == 0x34, "FinderQuery layout");

    // ---- The segment test (ClipCollisionDirectCallBack 0x112D590) -------------
    // a0 the hit record, a1 / a2 the segment's ends, a3 the isys group, t0 object filter (object, user), t1 part
    // filter (centre, matrix, inverse, user), t2 surface filter (centre / matrix / user), t3 id filter (id, user);
    // the caller's +0 the user word (the judge data's six words: count, then objects to leave out).
    namespace segment
    {
        constexpr uint32_t kFrame = 0x270;  // the object loop's frame
        constexpr uint32_t kLeafParams = 0xD0; // the leaf's t1 (object, class, length, tree matrix, cylinder matrix, split scale, side, filters, user)
    }

    // ---- isys group link (0x21B900, 24 bytes a link) ---------------------------
    struct GroupLink
    {
        u32 object;   // +0x00
        u32 w04, w08;
        u8 deleting;  // +0x0C
        u8 b0D;
        u16 next;     // +0x0E the next link's index (0 ends)
        u32 w10, w14;
    };
    static_assert(sizeof(GroupLink) == 24, "GroupLink layout");
    constexpr uint32_t kGroupLinks = 0x0021B900u;
}
