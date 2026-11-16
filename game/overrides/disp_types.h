// Round S4: the display / draw-prep data as C++ types (semantic decomp).
//
// What the game's display code builds each frame and what the native renderer's hooks read from it, as views on guest
// memory (guest RAM is host memory, little-endian), with may_alias members pinned by static_asserts, as chara_types.h /
// coll_types.h. docs/display_map.md section 4 gives where each field comes from (file:line of the natives that read
// them); docs/display_s4.md says what S4 hands the renderer host-side instead of reading it back from PS2 packets. Field
// names marked "(guess)" come from access patterns only.
//
// The second half (namespace disp::host) is S4's own host-side records: what the game-side code hands the native
// renderer directly where the native path used to parse a guest VU1 / VIF packet back.
#pragma once

#include <cstddef>
#include <cstdint>

namespace disp
{
    using f32 = float __attribute__((may_alias));
    using u32 = uint32_t __attribute__((may_alias));
    using s32 = int32_t __attribute__((may_alias));
    using u64 = uint64_t __attribute__((may_alias));
    using u16 = uint16_t __attribute__((may_alias));
    using s16 = int16_t __attribute__((may_alias));
    using u8 = uint8_t __attribute__((may_alias));

    struct Vec4
    {
        f32 x, y, z, w;
    };
    static_assert(sizeof(Vec4) == 16, "Vec4");
    // A VU matrix: four columns of four floats (column 3 the translation), as the VU0 / VU1 code keeps them.
    struct Mat4
    {
        Vec4 c[4];
    };
    static_assert(sizeof(Mat4) == 0x40, "Mat4");

    // ---- DMA / VIF / GIF words the game's packets are made of ------------------------------------------------------
    namespace dma
    {
        // A DMA tag's id (bits 28-30 of its first word).
        enum Id : uint32_t { kRefe = 0, kCnt = 1, kNext = 2, kRef = 3, kRefs = 4, kCall = 5, kRet = 6, kEnd = 7 };
        inline uint32_t id(uint32_t w0) { return (w0 >> 28) & 7u; }
        inline uint32_t qwc(uint32_t w0) { return w0 & 0xFFFFu; }
        struct Tag
        {
            u32 w0;   // qwc (0-15), id (28-30)
            u32 addr; // the address (NEXT / CALL / REF ...)
            u32 vif0, vif1; // two VIFcodes sent with the tag
        };
        static_assert(sizeof(Tag) == 16, "DMA tag");
    }
    namespace vif
    {
        // VIFcode: immediate 0-15, num 16-23, cmd 24-30. UNPACK commands are 0x60-0x7F (cmd & 0x60 == 0x60); the
        // instance chains' palettes and lights are V4-32 UNPACKs (cmd & 0x6F == 0x6C), num qwords at addr (imm & 0x3FF).
        inline uint32_t cmd(uint32_t code) { return (code >> 24) & 0x7Fu; }
        inline bool isUnpackV4_32(uint32_t code) { return (cmd(code) & 0x6Fu) == 0x6Cu; }
        inline uint32_t num(uint32_t code) { return ((code >> 16) & 0xFFu) ? ((code >> 16) & 0xFFu) : 256u; }
        inline uint32_t addr(uint32_t code) { return code & 0x3FFu; }
    }

    // ---- Model header (object +0x20) -------------------------------------------------------------------------------
    struct ModelHeader
    {
        u32 w00[2];
        u32 owner;          // +0x08 set at load
        u32 secondFade;     // +0x0C non-zero: the second fade factor (instance +0x248) applies
        u32 w10;
        u32 loadPackets;    // +0x14 packets built at load ([0] per draw packet 0x30 bytes, [1] per surface 0x40)
        u32 flags;          // +0x18 bit 0: a LOD model
        u32 lodCount;       // +0x1C
        f32 bounds[4];      // +0x20..+0x2C
        u32 partTable[4];   // +0x30 + 4 x lod: the LOD's part table (0x30-byte nodes; +0x2C the clip's visibility)
        u32 textures;       // +0x40 texture table (0x20 each; +0x10 the texture object, whose +0x20 is the NTO2 header)
        u32 textureCount;   // +0x44
        u32 w48[2];
        u32 surfaces;       // +0x50 surfaces (0x120 each)
        u32 surfaceCount;   // +0x54
        u32 w58[2];
        u32 packets;        // +0x60 draw packets (8 words each; the list ends at a packet of size 0)
        u32 packetCount;    // +0x64
    };
    static_assert(offsetof(ModelHeader, secondFade) == 0x0C && offsetof(ModelHeader, flags) == 0x18 && offsetof(ModelHeader, lodCount) == 0x1C &&
                      offsetof(ModelHeader, partTable) == 0x30 && offsetof(ModelHeader, textures) == 0x40 && offsetof(ModelHeader, surfaces) == 0x50 &&
                      offsetof(ModelHeader, surfaceCount) == 0x54 && offsetof(ModelHeader, packets) == 0x60,
                  "ModelHeader layout");

    // A part-table node (0x30 bytes): the clip's camera test result at +0x2C (0 / 2: drawn), children at +0x20 / +0x24.
    struct PartNode
    {
        u32 w00[8];
        u32 child;    // +0x20 (0xFFFFFFFF: none)
        u32 sibling;  // +0x24
        u32 w28;
        u32 visible;  // +0x2C written by modelDispState (round 61's lane) / the clip
    };
    static_assert(sizeof(PartNode) == 0x30 && offsetof(PartNode, child) == 0x20 && offsetof(PartNode, visible) == 0x2C, "PartNode layout");

    // A surface (0x120 bytes).
    struct Surface
    {
        u32 w00[6];
        u64 flags;        // +0x18 bit 29 never drawn, 17 boss effect, 24 shadow effect, 26 hidden under water, 23 / 25 / 9 / 10 /
                          //       14 / 15 / 8 layer selection (modelGetDlLayer), 0x2200 reflection. (+0x1C: texture mode 1 / 2)
        u32 w20[3];
        u32 textureIndex; // +0x2C (also the scroll chain's index, round 127)
        u32 w30[3];
        u32 textureIndex2; // +0x3C
        u32 w40[8];
        u8 registers[0x60]; // +0x60 the GS register packet (A+D: TEX0, ALPHA, TEST ...)
        u8 second[0x4C];    // +0xC0 the second-pass registers
        f32 alpha;          // +0x10C
        u32 w110[4];
    };
    static_assert(sizeof(Surface) == 0x120 && offsetof(Surface, flags) == 0x18 && offsetof(Surface, textureIndex) == 0x2C &&
                      offsetof(Surface, textureIndex2) == 0x3C && offsetof(Surface, registers) == 0x60 && offsetof(Surface, second) == 0xC0 &&
                      offsetof(Surface, alpha) == 0x10C,
                  "Surface layout");

    // A draw packet (8 words) of the header's list.
    struct DrawPacket
    {
        u32 size;      // [0] 0 ends the list
        u32 lod;       // [1]
        u32 surface;   // [2] the surface index
        s32 part;      // [3] -1: always drawn
        u32 vertices;  // [4] the vertex block (world batches: GIF tag, V3-32 positions, V2-16 ST, V4-8 colours, MSCNT each)
        u32 w5[2];
        u32 chain;     // [7] its DMA chain (CALL surface registers, REF vertex block, MSCAL, RET)
    };
    static_assert(sizeof(DrawPacket) == 32 && offsetof(DrawPacket, vertices) == 16 && offsetof(DrawPacket, chain) == 28, "DrawPacket layout");

    // The model instance (the walk's geometry / instance record).
    struct Instance
    {
        u32 object;        // +0x00 (object +0x20 = the header)
        u32 w04[2];
        u32 chain;         // +0x0C the instance chain: a NEXT tag past its 15 qwords of lights / colours to a RET packet whose
                           //       V4-32 UNPACKs carry the VU1 picture (rigid: one matrix; skinned: the palette). Double-buffered.
        u8 w10[0x230 - 0x10];
        f32 depth[3];      // +0x230..+0x238 view depths / screen point (modelClipPoint)
        u32 w23C;
        u32 w240;          // +0x240
        f32 fade;          // +0x244
        f32 fade2;         // +0x248 the second fade
        u32 w24C;          // +0x24C (into the instance packet's +0xD0)
        u64 lod;           // +0x250 the LOD scale; the LOD level in bits 40-42 (signed, as modelDisp's dsll 21 / dsra32 29)
        u32 w258[2];
        u64 drawFlags;     // +0x260 bit 2 skips the opacity write, bit 4 out of the second sort list, bit 8 visible
    };
    static_assert(offsetof(Instance, chain) == 0x0C && offsetof(Instance, depth) == 0x230 && offsetof(Instance, fade) == 0x244 &&
                      offsetof(Instance, fade2) == 0x248 && offsetof(Instance, lod) == 0x250 && offsetof(Instance, drawFlags) == 0x260,
                  "Instance layout");
    inline int32_t lodLevel(uint64_t lodWord) { return static_cast<int32_t>(static_cast<int64_t>(lodWord << 21) >> 61); }

    // A skinned model's display object: object +0x144 -> disp, disp +0xC -> model, model +0x104 -> the palette block in the
    // model's VU1 packet; bone i at palette + ((4i + ((i + 1) >> 6)) << 4) (one more qword after every 64 bones: a VIF
    // UNPACK header). dispSkinned (decomp_skinpal.cpp) is the only writer.
    namespace skinned
    {
        constexpr uint32_t kDisp = 0x144, kModel = 0x0C, kPalette = 0x104, kBones = 0x1B4 /* s16 */, kBoneMatrices = 0x160,
                           kParentBlock = 0x194;
        inline uint32_t boneOffset(uint32_t i) { return (i * 4u + ((i + 1u) >> 6)) << 4; }
    }

    // The instance chain's light picture (the qwords after the chain's first, VU1 0x3D8): L rows 1-4, colours 5-8.
    struct InstanceLights
    {
        Vec4 q0;
        Mat4 light;   // qwords 1-4 the light matrix L
        Mat4 colour;  // qwords 5-8 the colour matrix C
    };
    static_assert(sizeof(InstanceLights) == 0x90, "InstanceLights");

    // ---- The display list (record at 0x2132D8) --------------------------------------------------------------------
    struct DisplayList
    {
        u32 byBuffer[2];  // +0 / +4
        u32 packets[2];   // +8 / +0xC the packet buffers
        u32 work;         // +0x10 the work buffer (its +0x20: the free pointer)
        u32 layerCount;   // +0x14
        u32 buffer;       // +0x18 the buffer index (iosDlGetWorkBufId)
        u32 layer;        // +0x1C the current layer
    };
    static_assert(sizeof(DisplayList) == 0x20 && offsetof(DisplayList, layer) == 0x1C, "DisplayList layout");
    constexpr uint32_t kDisplayList = 0x002132D8u, kLayerDepth = 0x001E10B0u, kSavedLayers = 0x00213308u, kListSema = 0x002132D0u;
    // A layer: a 0x30-byte packet (+0 0x10000000, +0x10 a NEXT to the following, +0x20 / +0x24 self pointers; +0x24 the tail).
    struct LayerPacket
    {
        u32 w0[4];
        dma::Tag next;  // +0x10
        u32 self;       // +0x20
        u32 tail;       // +0x24 the layer's tail
        u32 w28[2];
    };
    static_assert(sizeof(LayerPacket) == 0x30 && offsetof(LayerPacket, tail) == 0x24, "LayerPacket layout");

    // ---- gsReg's packet (*0x1E11A4) and the gl block (0x1186390) ------------------------------------------------
    struct GsRegPacket
    {
        u32 tag;      // +0 DMA tag
        u32 start;    // +4
        u32 giftag;   // +8
        u32 write;    // +0xC the write pointer; a pair is (value, register), 8 bytes each
        u32 counts[3];
        u32 mode;     // +0x1C closed / "no packet"
    };
    static_assert(sizeof(GsRegPacket) == 0x20, "GsRegPacket");
    constexpr uint32_t kGsRegState = 0x001E11A4u, kGlBlock = 0x01186390u;

    // ---- Textures ---------------------------------------------------------------------------------------------------
    // The NTO2 header (a texture resource's +0x20).
    struct Nto2
    {
        u32 magic;    // +0 0x324F544E
        u32 w4[2];
        u32 block;    // +0xC the block B (B +0 / +4: the scroll row the game moves each frame)
        u32 w10;
        u32 pixels;   // +0x14
        u32 clut;     // +0x18
        u32 format;   // +0x1C bits 0-5 PSM, 6-11 CPSM, 12-15 levels, 16-19 / 20-23 log2 width / height (byte +0x1F: a bit a level)
    };
    static_assert(offsetof(Nto2, block) == 0x0C && offsetof(Nto2, format) == 0x1C, "Nto2 layout");
    constexpr uint32_t kNto2Magic = 0x324F544Eu;

    // ---- The cloth grid ("#" object +0x20 block) --------------------------------------------------------------------
    struct ClothBlock
    {
        u32 buffers[2]; // two buffers (0xB0-byte DMA / VIF header with the view matrix at qwords 4-7, then per row an UNPACK and
                        // columns x 4 qwords: world position, normal, ST(Q), colour floats)
        u32 rows;
        u32 columns;
        u32 current;    // [4] the current one
    };
    static_assert(sizeof(ClothBlock) == 20, "ClothBlock");

    // ---- The camera context (*(0x122A8F0 + 4 x *0x118FE4C)) --------------------------------------------------------
    namespace camera
    {
        constexpr uint32_t kTable = 0x0122A8F0u, kIndex = 0x0118FE4Cu;
        constexpr uint32_t kLens = 0x0C, kProjection = 0x40, kEye = 0x140, kView = 0x1A0, kGsZ = 0x4A0, kMirror = 0x520,
                           kViewProjection = 0x5A0, kClipTest = 0x5E0, kScreen = 0x660;
    }

    // ---- S4's host records -------------------------------------------------------------------------------------------
    namespace host
    {
        // A skinned palette as dispSkinned writes it, kept host-side (S4 part 1): the qwords the instance chain's UNPACKs
        // carry to VU1 (lo..hi, VU qword addresses), as readChainPaletteLean read them back from the chain before. Made
        // by dispSkinned's native at the write, handed to the model hook and the kick's copy by the palette's address.
        struct Palette
        {
            uint32_t at = 0;        // the guest palette block (model +0x104)
            uint32_t bones = 0;     // matrices written
            uint64_t frame = ~0ull; // the display frame it was written in
            uint32_t serial = 0;    // bumped at every write (a reader's copy is current when the serial matches)
        };
    }
}
