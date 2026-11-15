// Round S1: the particle subsystem's guest data as C++ types (semantic decomp).
//
// The particle / wind module (GAMECORE 0x01139CA0-0x01147530) and ptclman
// (0x01099E00-0x0109A4D0) keep their state in main RAM in the layouts below;
// docs/particles_s1.md says where each field comes from, who writes it and
// who else reads it. These are views on guest memory (guest RAM is host
// memory, little-endian: a `const Particle *` at rdram + address reads the
// record in place); the layouts are pinned with static_asserts. Field names
// marked "(guess)" come from access patterns only.
//
// Outside readers (why the records stay in guest RAM for now): the effect
// users (layout objects, boss effects, scripts) hold emitters by handle and
// write the emitter's blend ratio / generate flag / stop through module
// entries; nothing outside the module reads a Particle. The draw chain's
// output (the 48-byte DrawRecord) is read only by the renderer's job.
#pragma once

#include <cstddef>
#include <cstdint>

namespace ptcl
{
    using f32 = float __attribute__((may_alias));
    using u32 = uint32_t __attribute__((may_alias));
    using s32 = int32_t __attribute__((may_alias));

    struct Vec4
    {
        f32 x, y, z, w;
    };

    // ---- Particle (0xB0 bytes): one record of the module's particle pool -----
    // Free records are on the manager's free list (Manager::freeList), live
    // ones on their emitter's list (Emitter::live), both linked by `next`.
    struct Particle
    {
        Vec4 pos;          // +0x00 position (world, or the emitter's local space for space 2 / 3)
        Vec4 vel;          // +0x10 velocity (per second of step)
        Vec4 acc;          // +0x20 acceleration
        Vec4 axis;         // +0x30 shape 2's fourth vector / shape 4's random axis (x, y; z = w = 0)
        f32 life;          // +0x40 lifetime; 0 = dead (skipped by the step and the draws)
        f32 age;           // +0x44 age, += dt each step
        f32 attach;        // +0x48 how much of the emitter's move it follows (space 1 / 3)
        f32 size;          // +0x4C size (>= 0)
        f32 growth;        // +0x50 size speed
        f32 alpha;         // +0x54 alpha (x the floor / ceiling bounce's factor)
        f32 fadeInEnd;     // +0x58 age where the fade-in ends
        f32 fadeOutStart;  // +0x5C age where the fade-out starts
        f32 fadeInRate;    // +0x60 fade-in: alpha x age x this
        f32 fadeOutRate;   // +0x64 fade-out: alpha x (life - age) x this
        f32 r, rSpeed;     // +0x68 / +0x6C colour channel 0 (0..1) and its speed
        f32 g, gSpeed;     // +0x70 / +0x74 channel 1
        f32 b, bSpeed;     // +0x78 / +0x7C channel 2
        f32 spinRate;      // +0x80 turn about the emitter's spin axis (spin != 0)
        f32 w84;           // +0x84 (guess: unused by the step)
        f32 drift;         // +0x88 how much of the list's drift vector it takes
        f32 sink;          // +0x8C subtracted from the y acceleration
        f32 frame;         // +0x90 rotation / animation frame (animated drawers: frame / length)
        f32 frameSpeed;    // +0x94 its speed
        f32 angle;         // +0x98 the sprite's angle (shapes 1 / 4), or shape 2's length
        f32 angleSpeed;    // +0x9C its speed
        f32 turnBase;      // +0xA0 x the parameters' turn factor -> the angle speed's change
        u32 hit;           // +0xA4 collision: has hit (collision mode != 0 steps only hit particles natively)
        u32 next;          // +0xA8 next record (guest address; 0 ends)
        u32 wAC;           // +0xAC
    };
    static_assert(sizeof(Particle) == 0xB0, "Particle layout");
    static_assert(offsetof(Particle, life) == 0x40 && offsetof(Particle, size) == 0x4C && offsetof(Particle, r) == 0x68 &&
                      offsetof(Particle, frame) == 0x90 && offsetof(Particle, angle) == 0x98 && offsetof(Particle, next) == 0xA8,
                  "Particle offsets");

    // A list head: count, first, last (the free list and each emitter's live list).
    struct List
    {
        s32 count;
        u32 first, last;
    };

    // ---- Node / Emitter ------------------------------------------------------
    // A particle geodisp's class (+0xE8) heads a list of nodes; a node is the
    // sheet record pointer (+0x00) followed by the emitter E (node + 0x10); the
    // next node is at node + 0x1B0 (= E + 0x1A0). The emitter id the effect
    // users hold is the node's address (get_particle_emitter_from_emitter_id:
    // id + 0x10). Offsets below are E's. Only the fields S1 and the documented
    // functions use are named.
    namespace node
    {
        constexpr uint32_t kSheet = 0x00;   // the sheet record: +0x28 layer 0..4, +0x34 draw key, +0x48 main effect set, +0x4C sub (blend partner)
        constexpr uint32_t kEmitter = 0x10; // the emitter E
        constexpr uint32_t kNext = 0x1B0;   // the next node of the geodisp's list
    }

    // ---- Sheet record (0x54 bytes; node +0x00). Facts from RafaelScharf (PRs #4-#6),
    // checked against the recompiled code. The "effect id" the effect users hold
    // is the record's address (get_particle_effect_id_from_sheet returns its
    // argument). get_particle_effect_id_by_name (0113F8F8; a0 a resource handle
    // whose word 0 & 0x3FFFF is the distance back to its owner, a1 the name)
    // walks owner +0x10 (the records, this stride) for owner +0x0C entries,
    // strcmp on +0x24; an owner of 0 or equal to the "no owner" global (lui 0x1e
    // + 0xED4 in the code: a relocation placeholder, read the real address live)
    // gives 0.
    namespace sheet
    {
        constexpr uint32_t kStride = 0x54;    // records are packed at this stride
        constexpr uint32_t kName = 0x24;      // -> the effect's name (get_particle_name_from_effect_id; 0 for a null id)
        constexpr uint32_t kLayer = 0x28;     // layer 0..4
        constexpr uint32_t kDrawKey = 0x34;   // 0113ACB8 groups runs of equal keys (the depth sort runs inside a run)
        constexpr uint32_t kMainSet = 0x48;   // the main effect set Mr (get_main_particle_effect_lod_from_sheet)
        constexpr uint32_t kSubSet = 0x4C;    // the blend partner's effect set
    }
    namespace emitter
    {
        constexpr uint32_t kWind = 0x00;         // vec4: the wind at the emitter (GetWindVector), x the particle's +0x88
        constexpr uint32_t kPos = 0x10;          // vec4: position (from the geometry pointer +0xD0 when attached, +0xC4 == 1)
        constexpr uint32_t kTurn = 0x20;         // vec4: orientation (quaternion)
        constexpr uint32_t kDir = 0x30;          // vec4: direction (spin axis; x the parameters' +4 = this frame's move)
        constexpr uint32_t kPrevPos = 0x40;      // vec4: last frame's +0x10 (+0x50 / +0x60: last frame's +0x20 / +0x30)
        constexpr uint32_t kColourScale = 0x70;  // vec4: r, g, b scales for the draw (fog-blended by 0113B408 once a frame, +0x110 its flag)
        constexpr uint32_t kViewPos = 0x80;      // qword: position copy for gluGetBrightness (0113B408)
        constexpr uint32_t kLight = 0x90;        // vec4: unit view direction (0113B408): the lit draw's light
        constexpr uint32_t kDone = 0x114;        // 1: done generating (only the live particles step)
        constexpr uint32_t kLod = 0x118;         // the LOD record's index 0..2 (01141150)
        constexpr uint32_t kCarry = 0x11C;       // the emission's carried fraction
        constexpr uint32_t kSpawned = 0x120;     // particles made so far
        constexpr uint32_t kLive = 0x124;        // List: the live particles (count, first, last)
        constexpr uint32_t kDistance = 0x130;    // distance to the camera
        constexpr uint32_t kDepth = 0x134;       // |depth| along the camera's axis; the sort key of 0113ACB8's runs:
                                                 // 01140EA8 (qsort compare on node pointers, node +0x144) returns 1 when
                                                 // a's depth < b's, else -1 (never 0): the farthest first, back to front
        constexpr uint32_t kBaseAlpha = 0x138;   // base alpha (init)
        constexpr uint32_t kDistFade = 0x13C;    // distance fade (011410D0)
        constexpr uint32_t kAlphaScale = 0x140;  // draw alpha = +0x138 x +0x13C (0 hides the emitter)
        constexpr uint32_t kSizeScale = 0x144;   // draw size scale
        constexpr uint32_t kFloor = 0x150;       // the bounce planes' y: particles are put back when y >= this - margin
        constexpr uint32_t kCeiling = 0x154;     // ... and when y <= this + margin (defaults +-1e7)
        constexpr uint32_t kEndCallback = 0x158; // mode (1) / +0x15C function / +0x160 argument, called at death
        constexpr uint32_t kShown = 0x164;       // the clip pass kept it (1)
        constexpr uint32_t kGenerate = 0x168;    // set_generate_particle_flag
        constexpr uint32_t kBlendRatio = 0x16C;  // set_npe_blend_ratio_particle_emitter's ratio
        constexpr uint32_t kFinder = 0x170;      // collision finder (released at death)
        constexpr uint32_t kFirstFrame = 0x190;  // forces one particle on the first generating frame
        constexpr uint32_t kStepped = 0x198;     // display flag: 0 at each step, set per layer (011408E0); drawn only when 1
    }

    // ---- Effect set Mr (sheet +0x48 / +0x4C; a2 of the drawers, a1 of the step) ----
    // 0x140 bytes: create_particle_effect_lod (0113FBF0) allocates 0x140, zeroes
    // +0xD0..+0xDC and sets a unit matrix at +0xE0 (RafaelScharf, PRs #4-#6).
    namespace effect
    {
        constexpr uint32_t kSize = 0x140;
        constexpr uint32_t kLodMode = 0x08;       // LOD by screen radius (+0x0C..+0x18 thresholds)
        constexpr uint32_t kMaxParticles = 0x1C;  // the live cap; also the display-list room asked for
        constexpr uint32_t kFadeOut = 0x24;       // the draws' fade: 1 - this
        constexpr uint32_t kScaleX = 0x50;        // polygon scale: the lit draw's environment scale x
        constexpr uint32_t kScaleY = 0x54;        // ... y
        constexpr uint32_t kTexExtentU = 0x58;    // the sprite's texture extent (u, v): the env packet's vec
        constexpr uint32_t kTexExtentV = 0x5C;
        // The light colour (0113B408, once a frame into E +0x70): +0x64 its source (0 / >= 6 the
        // default colour; 1 the class's +0xB0 vector; 2..5 the mean of the class's +0x80/+0x90/+0xA0
        // plus +0xB0), x +0x6C, clamped to 0..1, blended with the default by +0x68. The light-need
        // test 01140E60 (a0 E, a1 Mr, a2 Mr's LOD record 0): 0 when E +0x110 is set (done this
        // frame), else 1 when +0x64 != 0, Mr +0xB0 == 1 or the record's +0x164 == 2.
        // calc_light_particle_geodisp (0113F600) asks it for every node (01140C80, stops at the
        // first 1); on a 1 it copies the first node's E +0x10 into the class's +0x10 (w = 1),
        // clears bit 0 of the class's +0xC0 and tail-calls SetGeoDispClassLightColMan (RafaelScharf, PRs #4-#6).
        constexpr uint32_t kLightMode = 0x64;
        constexpr uint32_t kLightBlend = 0x68;
        constexpr uint32_t kLightScale = 0x6C;
        constexpr uint32_t kViewFacing = 0xB0;    // 1: view-facing (particleDraw, the light direction written)
        constexpr uint32_t kShadeK = 0xB4;        // the lit draw's shade w
        constexpr uint32_t kBrightness = 0xB8;    // x gluGetBrightness, clamped 0..1: the lit draw's shade
        constexpr uint32_t kCameraFollow = 0xBC;  // camera-follow attribute (get_particle_camera_follow_attr_from_effect_id:
                                                  // sheet +0x48 -> this; -1 for a null id; read by updateParticleGeo)
        constexpr uint32_t kPlanes = 0xCC;        // 1: the floor / ceiling planes on
        constexpr uint32_t kLodCount = 0xD0;      // LOD records: count
        constexpr uint32_t kLodRecords = 0xD4;    // LOD records: array, 0x1BC each, indexed by emitter::kLod
        constexpr uint32_t kInfo = 0xD8;          // -> +0x24 the effect's name, +0x44 the damage test's handle
                                                  // (guess: the sheet record itself, whose name is at +0x24 too)
        constexpr uint32_t kMatrix = 0xE0;        // 4x4 matrix, set to unit by create_particle_effect_lod
        constexpr uint32_t kFrames = 0x120;       // animation frames (>= 2: the animated drawer)
        constexpr uint32_t kFrameLength = 0x128;  // animated drawers: frames per texture cell
        constexpr uint32_t kCellU = 0x12C;        // the texture cell's size (u, v); 0113DBF8 mode 7's extent
        constexpr uint32_t kCellV = 0x130;
        constexpr uint32_t kMargin = 0x134;       // the bounce planes' margin, x the particle's size
    }

    // ---- LOD record R (0x1BC bytes; Mr +0xD4 [E +0x118]; a1 of the drawers, a2 "K" of the step) ----
    namespace lod
    {
        constexpr uint32_t kSpace = 0x00;    // 1 / 3: follow the emitter's move; 2 / 3: local matrix
        constexpr uint32_t kTotal = 0x04;    // particles to make in all
        constexpr uint32_t kShape = 0x0C;    // 0 trap, 1 sprites (drawer by +0x18C / Mr +0xB0 / Mr +0x120), 2 streaks, 4 lit puffs
        constexpr uint32_t kGenMode = 0x10;  // 0: counts E +0x120; 1 in record 0 = a looping effect (is_loop_particle)
        constexpr uint32_t kBlend = 0x14;    // material: 1 adds, 2 subtracts, else blends
        constexpr uint32_t kSpin = 0xC8;     // orbit about the emitter's axis
        constexpr uint32_t kCollision = 0x104;
        constexpr uint32_t kJitter = 0x11C;
        constexpr uint32_t kAim = 0x164;     // 2: aim at the camera
        constexpr uint32_t kRandomAxis = 0x174;
        constexpr uint32_t kQuads = 0x18C;
        constexpr uint32_t kSpanVariant = 0x1B4;
    }

    // ---- DrawRecord (48 bytes): what every particle drawer writes per live
    // particle and the renderer's job reads (ptclVtxDraw).
    struct DrawRecord
    {
        f32 x, y, z, size;     // position, size x the emitter's size scale
        f32 r, g, b, a;        // colour x the emitter's scales, alpha x scale x fade, clamped to 0..1
        f32 s, c, u, v;        // sin / cos pair of angle + pi/4 (iosGetSinCosf); type 7: the cell's texture offset
    };
    static_assert(sizeof(DrawRecord) == 48, "DrawRecord layout");

    // ---- Manager globals (main RAM) ------------------------------------------
    namespace manager
    {
        constexpr uint32_t kStepPtr = 0x0118CFE8u;    // -> the frame step: +0 dt, +4 dt2, +0xC VBlanks this frame
        constexpr uint32_t kRendererOn = 0x0118CFF0u; // the particle renderer's switch (0: no draws)
        constexpr uint32_t kFreeList = 0x01216DF0u;   // List of free Particle records
        constexpr uint32_t kViewRows = 0x01216E40u;   // the particles' view matrix (four rows; the lit draw's environment)
        constexpr uint32_t kLayerSwitch = 0x01216ED0u;
        // ptclman's packet state (RafaelScharf, PRs #4-#6): +0 the packet's base (ptclmanBegin: the
        // packet allocator's current pointer), +4 qwords used (ptclmanBegin zeroes it, ptclmanPut adds;
        // ptclmanGet = base + used x 16). ptclmanEnd appends the qword 0x60000000 0 0 0x13000000
        // (a DMA ret tag, VIF FLUSHA) before closing the packet.
        constexpr uint32_t kPtclmanBase = 0x01215BE8u;
        constexpr uint32_t kPtclmanUsed = 0x01215BECu;
    }
}
