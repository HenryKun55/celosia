// Round S3: the characters' skeleton / motion data as C++ types (semantic decomp).
//
// A character (object types 9 and 0x16) keeps its frame state in its character class (object + 0x90, 0x1210 bytes),
// its pose buffers (pointers in the class), the shared read-only skeleton and the motion resources. docs/characters_map.md
// section 4 says where each field comes from (file:line of the natives that read them); docs/characters_s3.md says what S3
// keeps host-side. As coll_types.h / ptcl_types.h: views on guest memory (guest RAM is host memory, little-endian), with
// may_alias members, pinned by static_asserts. Field names marked "(guess)" come from access patterns only.
//
// Who reads them outside the characters' code (why the guest copies stay the truth): the camera and the managers read
// the pose (+0x154 / +0x158) and the node positions within 0.04-0.2 ms of a character's geometry; the display walk reads
// the skinning matrices (+0x160) into the models' palettes; the scripts read the motion record and the control status;
// collision reads the layout outputs (+0x15C / +0x164). S3's host structures are caches of guest data made from these
// views and validated against them (docs/characters_s3.md), never a replacement.
#pragma once

#include <cstddef>
#include <cstdint>

namespace chara
{
    using f32 = float __attribute__((may_alias));
    using u32 = uint32_t __attribute__((may_alias));
    using s32 = int32_t __attribute__((may_alias));
    using u16 = uint16_t __attribute__((may_alias));
    using s16 = int16_t __attribute__((may_alias));
    using u8 = uint8_t __attribute__((may_alias));

    struct Vec4
    {
        f32 x, y, z, w;
    };
    static_assert(sizeof(Vec4) == 16, "Vec4");
    // A quaternion is (x, y, z, w), w the scalar part (MultiCurrentQuaternion's order).
    using Quat = Vec4;
    // A bone matrix: four columns of four floats (c[3] the translation, w 1), as the VU0 code keeps them.
    struct Mat4
    {
        Vec4 c[4];
    };
    static_assert(sizeof(Mat4) == 0x40, "Mat4");

    constexpr uint32_t kNone = 0xFFFFFFFFu; // "no bone" in the skeleton's links

    // ---- The object ------------------------------------------------------------------------------------------
    namespace object
    {
        constexpr uint32_t kType = 0x08;      // u8: 9 or 0x16 = a character
        constexpr uint32_t kClass = 0x90;     // the character class lives here (GetCharacterClass 0x10C0A70)
        constexpr uint32_t kPosition = 0x170; // vec4 (-x, y, -z of the teleport numbers; the player distance tests)
        constexpr uint32_t kAnimation = 0x250; // the animation class (GetAnimationClass 0x10BA7A8)
        constexpr uint8_t kChara = 9, kChara2 = 0x16;
        inline bool isCharacter(uint8_t type) { return type == kChara || type == kChara2; }
    }

    // ---- The skeleton (class +0x358) --------------------------------------------------------------------------
    // Shared by every character of a kind, read-only during the frame.
    struct Bone
    {
        u32 w00[4];   // +0x00 (unused by the pose)
        Vec4 offset;  // +0x10 the offset from the parent (x the class scale)
        Quat base;    // +0x20 the base rotation (GetMotionNodeBaseQuaternion)
        u32 child;    // +0x30 the first child, kNone: none (pose82 / skeletonPose; motion.cpp reads it as "first animated
                      //       ancestor" for option nodes: not reconciled, docs/characters_map.md section 4)
        u32 sibling;  // +0x34 the next sibling, kNone: none
        s32 parent;   // +0x38 the parent, -1 for a root (CalcNodeInterp, GetMotionNodeBasePos)
        u32 w3C;
    };
    static_assert(sizeof(Bone) == 0x40 && offsetof(Bone, offset) == 0x10 && offsetof(Bone, child) == 0x30 && offsetof(Bone, sibling) == 0x34 &&
                      offsetof(Bone, parent) == 0x38,
                  "Bone layout");
    struct Skeleton
    {
        u32 w00;
        s32 weights;     // +0x04 the IK weight count (class +0x320 / +0x324 entries)
        u32 bones;       // +0x08 -> Bone[]
        u32 w0C, w10, w14, w18, w1C;
        s32 ikRecords;   // +0x20 IK node records (class +0x31C, 0xD0 each)
        u32 optionNodes; // +0x24 -> 0x40 each: +0x10 position, +0x20 quaternion, +0x30 parent
    };
    static_assert(offsetof(Skeleton, bones) == 0x08 && offsetof(Skeleton, ikRecords) == 0x20 && offsetof(Skeleton, optionNodes) == 0x24,
                  "Skeleton layout");
    constexpr uint32_t kMaxPoseBones = 512u, kMaxPoseDepth = 96u; // pose82's limits (beyond them: the older paths)

    // ---- Motion resources (shared, read-only while sampled) ---------------------------------------------------
    // A handle: low 18 bits a self-relative offset to the owning table's word, bits 18-29 the index; the element is
    // table + 0x10 + index x stride (table +8 -> +0xC). A plain motion's element +0xC is its resource; a blend's
    // element +0xC is a chain of 12-byte handles, +0x50 the last motion (getBlendLastMotion 0x10B28E0).
    inline uint32_t handleIndex(uint32_t h) { return (h >> 18) & 0xFFFu; }
    inline uint32_t handleOffset(uint32_t h) { return h & 0x3FFFFu; }

    struct MotionHeader // resource + 0x20 (GetMotionVersion 0x10B39B0 ...); GetMotionFloatFrameNoCaribration's a1 points here
    {
        s16 version;       // +0x00
        s16 h02;
        s16 frames;        // +0x04 the frame count
        s16 h06;
        u32 formats;       // +0x08 -> u8 per channel: the curve format (0-14, the jump table 0x1193230)
        u32 w0C;
        u32 channels;      // +0x10 -> u32 per channel: the Channel record
        s32 channelCount;  // +0x14
        s32 optionNodes;   // +0x18 the option-node count
        u32 w1C, w20, w24;
        u32 optionIds;     // +0x28 -> u8 per option node: its physical node
    };
    static_assert(offsetof(MotionHeader, formats) == 0x08 && offsetof(MotionHeader, channels) == 0x10 &&
                      offsetof(MotionHeader, channelCount) == 0x14 && offsetof(MotionHeader, optionIds) == 0x28,
                  "MotionHeader layout");

    // One channel's curve (decodeKey, decomp_char122.cpp; curveEval 0x10AC6C8). The packed formats (3-6) are 9/9/9/4
    // bits a key word with the sign of w in a fixed bit; w = sqrt(1 - |xyz|^2) (+ the 4-bit offset term).
    struct Channel
    {
        f32 scale[8];    // +0x00 offset / scale pairs (x, y, z, the 4-bit term); format 13: +0 offset, +4 scale, +8..+0x10 axis
        u32 keys32;      // +0x20 -> u32 keys (formats 3-6)
    };
    static_assert(offsetof(Channel, keys32) == 0x20, "Channel layout");
    namespace channel
    {
        constexpr uint32_t kAngles = 0x14; // -> s16 angle per key (format 13)
        constexpr uint32_t kS16 = 0x18;    // -> 3 x s16 per key (formats 12 / 14)
    }
    // The curve evaluators by their jump-table targets (0x1193230 + 4 x format, relocated at run time).
    namespace curve
    {
        constexpr uint32_t kTable = 0x01193230u;
        constexpr uint32_t kQword = 0x010AD028u;  // the channel's own qword (a constant key)
        constexpr uint32_t kNone = 0x010AC700u;   // vf0: the unit quaternion
        constexpr uint32_t kPacked0 = 0x010AC978u, kPacked1 = 0x010ACAB0u, kPacked2 = 0x010ACBE8u, kPacked3 = 0x010ACD20u;
        constexpr uint32_t kS16xyz = 0x010AC8A8u; // s16 x, y, z; w from |xyz|^2, its sign in x's low bit
        constexpr uint32_t kAngle = 0x010ACFA0u;  // an angle about a fixed axis
    }

    // ---- The motion record (class +0x1C0, 0x40 bytes a slot) -------------------------------------------------
    struct MotionSlot
    {
        u32 handle;      // +0x00 the motion (plain or blend)
        u32 w04;
        f32 time;        // +0x08 the frame (float)
        u32 w0C[4];
        f32 speed;       // +0x1C
        u32 w20[2];
        f32 speed2;      // +0x28
        u32 w2C;
        u32 ikSet;       // +0x30 the IK set's handle (CalcNodeQuaternionWithIK's a1 + 0x30)
        u32 w34;
    };
    static_assert(offsetof(MotionSlot, time) == 0x08 && offsetof(MotionSlot, speed) == 0x1C && offsetof(MotionSlot, ikSet) == 0x30, "MotionSlot");

    // ---- IK (class +0x31C: 0xD0 bytes a record, Skeleton::ikRecords of them) ----------------------------------
    struct IkRecord
    {
        u32 w00[16];
        Vec4 root;        // +0x40 = class +0x460 (reset on the no-IK path)
        u32 w50[20];
        Quat unit;        // +0xA0 = the unit quaternion 0x1E0F40
        f32 b0, b4;       // +0xB0 / +0xB4 = 0
        f32 weight;       // +0xB8 = 1.0 (guess: a blend weight)
        f32 limit;        // +0xBC = 100.0 (guess)
        f32 c0;           // +0xC0 = 0
        u8 c4, c5, c6, c7; // +0xC4 / +0xC5 = 0
        u32 wC8[2];
    };
    static_assert(sizeof(IkRecord) == 0xD0 && offsetof(IkRecord, root) == 0x40 && offsetof(IkRecord, unit) == 0xA0 &&
                      offsetof(IkRecord, weight) == 0xB8 && offsetof(IkRecord, c4) == 0xC4,
                  "IkRecord layout");

    // ---- Gaze (class +0x334: 16 bytes each, +0x41C of them) ----------------------------------------------------
    struct Gaze
    {
        f32 x, y;      // the gaze direction's components (the angle picks two of eight keys, the length the amount)
        u32 resource;  // the gaze motion's resource
        u32 w0C;
    };
    static_assert(sizeof(Gaze) == 16, "Gaze");

    // ---- The character class (object + 0x90) -------------------------------------------------------------------
    // Offsets, not a struct: 0x1210 bytes of which the natives name about a hundred fields.
    namespace cls
    {
        constexpr uint32_t kSize = 0x1210;
        constexpr uint32_t kVelocity = 0x30;       // vec4
        constexpr uint32_t kFlags38 = 0x38;        // bit 3 root slerp, bit 4 gaze
        constexpr uint32_t kAnimeDistance = 0x3C;  // f32: the anime tables run when ExecCharaGeometry's a1 is within this
        constexpr uint32_t kRootPos = 0x40;        // vec4 (CalcNodeInterp's root position; IK t0)
        constexpr uint32_t kRootRot = 0x80;        // quat
        constexpr uint32_t kScale = 0x90;          // vec4 (x, y, z): the pose's t1
        constexpr uint32_t kPositionPtr = 0x134;   // -> vec4 the position (the pose's t0)
        constexpr uint32_t kPastPositionPtr = 0x138;
        constexpr uint32_t kRootQuatPtr = 0x13C;   // -> quat
        constexpr uint32_t kPoseQuats = 0x154;     // -> Quat[bones]: the pose's quaternions (the pose's a1)
        constexpr uint32_t kPoseMats = 0x158;      // -> Mat4[bones]: the pose's matrices (the pose's a0)
        constexpr uint32_t kScaledMats = 0x15C;    // -> Mat4[bones]: the layout's scaled bone matrices
        constexpr uint32_t kSkinMats = 0x160;      // -> Mat4[bones]: = kScaledMats x kInvBind (bone products, flag bit 3)
        constexpr uint32_t kLayout164 = 0x164, kLayout16C = 0x16C, kLayout170 = 0x170; // layout outputs (rigid inverses, products)
        constexpr uint32_t kBoneCount = 0x1B4;     // s16 the nodes / bones (also the +0x2E8 count)
        constexpr uint32_t kLayoutFlags = 0x1BC;   // u8: bit 3 bone products, bit 0 +0x450, 0x02 / 0x04 / 0x10 / 0x40 / 0x80 layout parts
        constexpr uint32_t kMotion = 0x1C0;        // MotionSlot (0x40 each)
        constexpr uint32_t kFrameF = 0x1C8, kFrameF2 = 0x1CC;
        constexpr uint32_t kAnimeTable = 0x1F0;
        constexpr uint32_t kMotionRot = 0x2D0;     // -> the motion's node rotations (CalcNodeInterp input)
        constexpr uint32_t kInterpOut = 0x2DC;     // -> the blended node rotations
        constexpr uint32_t kPrevPose = 0x2E0;      // -> the previous pose (node interpolation)
        constexpr uint32_t kOptionQuats = 0x2E4;
        constexpr uint32_t kLocals = 0x2E8;        // -> Quat[kBoneCount]: node local rotations (the pose's a3, normalised in place)
        constexpr uint32_t kInvBind = 0x2D8;       // -> Mat4[bones]: inverse bind (the bone products' b)
        constexpr uint32_t kBufA = 0x2F4, kBufB = 0x2F8, kBufCurA = 0x34C, kBufCurB = 0x350; // the per-frame buffer pair
        constexpr uint32_t kAnimeSlots = 0x30C, kAnimeSlotCount = 0x3FC; // 0x80-byte slots
        constexpr uint32_t kMatPairA = 0x310, kMatPairB = 0x314, kMatCurA = 0x344, kMatCurB = 0x348;
        constexpr uint32_t kIkRecords = 0x31C;     // -> IkRecord[Skeleton::ikRecords]
        constexpr uint32_t kIkWeights = 0x320, kIkRates = 0x324, kIkTargets = 0x328;
        constexpr uint32_t kGazes = 0x334, kGazeCount = 0x41C;
        constexpr uint32_t kSkeleton = 0x358;      // -> Skeleton
        constexpr uint32_t kRootSphere = 0x3B0, kCollSphere = 0xE00, kCollSphereBlend = 0xE10;
        constexpr uint32_t kRoot460 = 0x460, kRoot470 = 0x470, kRoot480 = 0x480, kRoot490 = 0x490;
        constexpr uint32_t kClothCatchUp = 0x52A, kIkEnable = 0x52C, kSubGeo = 0x52E, kPoseMade = 0x533, kAreaExists = 0x539;
        constexpr uint32_t kSkipDyn = 0x53B, kSkipCloth = 0x53C, kQueued = 0x53D, kQueuedCallbacks = 0x550;
        constexpr uint32_t kGazeResult = 0x590;
        constexpr uint32_t kInterpWeight = 0x724;  // f32 >= 1: the motion's rotation as is
        constexpr uint32_t kCtrlDef = 0x750, kCtrlStatus = 0x760;
        constexpr uint32_t kScriptMask = 0x920;
        constexpr uint32_t kPartner = 0x950, kRider = 0x960;
        constexpr uint32_t kGeometryPending = 0xCEC; // set by Preprocess on update frames; cleared by ExecCharaGeometry
        constexpr uint32_t kPastGeometry = 0x10A0;
        constexpr uint32_t kTransitions = 0x10E8, kLinks = 0x10EC, kTransitions2 = 0x10F0;
        constexpr uint32_t kFlip = 0x1130;
        constexpr uint32_t kClassType = 0x1140;
        constexpr uint32_t kGathered = 0x1164;
        constexpr uint32_t kSmallAnimalAi = 0x1200;
    }

    // ---- Per-character frame work (what ExecCharaGeometry's phases leave per character) -----------------------
    // The order inside one character's frame (docs/characters_map.md section 1): root dynamics and fitting (collision),
    // node interpolation, IK and gaze into kLocals, then the pose (kLocals -> kPoseMats / kPoseQuats), the layout and
    // the skinning matrices, the cloths (the VU0 R stream), the anime tables. The pose's inputs are final once the gaze
    // motions have run; its outputs are read at once by the sub-geometries of the matrices and the layout.
    struct PoseCall // GetMotionMatrix 0x10B1D60's arguments
    {
        uint32_t mats;    // a0 -> Mat4[]
        uint32_t quats;   // a1 -> Quat[] (0: the quaternion-less form, never S3's)
        uint32_t skel;    // a2 -> Skeleton
        uint32_t locals;  // a3 -> Quat[] (normalised in place)
        uint32_t pos;     // t0 -> vec4
        uint32_t scale;   // t1 -> vec4 (x, y, z)
    };

    // ---- Fixed guest globals the area uses ------------------------------------------------------------------------
    namespace global
    {
        constexpr uint32_t kQuatTop = 0x001E0F60u, kQuatStack = 0x00202AC0u; // the quaternion stack (64 entries)
        constexpr uint32_t kUnitQuat = 0x001E0F40u;
        constexpr uint32_t kRunningObject = 0x001E1550u;
        constexpr uint32_t kWorkStack = 0x011F7140u;
        constexpr uint32_t kExecGeoPoint = 0x014659C0u;
        constexpr uint32_t kFootIkScratch = 0x011E5DC0u; // .. 0x011E5E10
        constexpr uint32_t kPlayerIndex = 0x01465900u, kLinkTable = 0x0021B900u;
        constexpr uint32_t kActiveColossus = 0x01465AD4u;
    }
}
