#include <tuple>
// Shadow of the Colossus: models drawn by the native renderer
// (runtime/native_renderer.h) straight from the game's own model data, read
// at modelDisp (docs/native_renderer_map.md). SOTC_NATIVE=1 shows that
// picture instead of the emulated one; the emulated drawing still runs.
//
// What is read: the model header (packets at +0x60/+0x64), the instance's
// LOD (bits 40-42 of the 64-bit word at instance + 0x250), the part
// visibility the clip left (LOD table at header + 0x30, part x 0x30 + 0x2c),
// and per packet its world batches (VIF: GIF tag, V3-32 positions, V2-16 ST,
// V4-8 colours, each batch ending in MSCNT; triangle strips). The matrix is
// camera (camera context + 0x5a0) x model (modelDisp's second argument).
// Textures: the surface's index (+0x2c) into the model's table (+0x40,
// 0x20 each; +0x10 the texture object, whose +0x20 is the "NTO2" header:
// +0x14 pixels, +0x18 CLUT, +0x1c format | log2 width << 16 | log2 height
// << 20; byte +0x1f bit 0: level 0 uploaded as PSMCT32, W/2 x H/2 (8-bit)
// or W/2 x H/4 (4-bit), so the pixels are in GS block order).
// Skinned parts (characters) are posed on the CPU every frame from the bone
// palette in their instance chain (readPalette).
// Lit batches (normals) are lit per vertex as the VU1's lit microcode does,
// from the lights in the instance's own packet.
// Not yet: the second texture and mip levels.

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/native_tuning.h"
#include "runtime/native_renderer.h"
#include "runtime/gs/ps2_gs_psmct32.h"
#include "runtime/gs/ps2_gs_psmt4.h"
#include "runtime/gs/ps2_gs_psmt8.h"
#include "runtime/ps2_stats.h"

#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace sotc_native
{
    constexpr uint32_t kModelDisp = 0x01097118u, kGetCameraContext = 0x0108F518u;
    PS2Runtime::RecompiledFunction g_modelDisp = nullptr, g_getCameraContext = nullptr;
    uint32_t g_cameraContext = 0;
    uint64_t g_frame = 0;
    uint32_t g_frameDraws = 0;
    uint32_t g_modelDraws = 0; // 3D draws this frame

    const uint8_t *g_ram = nullptr;
    uint32_t word(uint32_t addr)
    {
        uint32_t v;
        std::memcpy(&v, g_ram + (addr & 0x1FFFFFCu), 4);
        return v;
    }
    float real(uint32_t addr)
    {
        float v;
        std::memcpy(&v, g_ram + (addr & 0x1FFFFFCu), 4);
        return v;
    }

    // A vertex as the batch gives it: model space, or bind pose with up to
    // three bones (palette matrices) and weights.
    struct BatchVertex
    {
        float x, y, z;
        float u, v;
        float u2, v2;     // the second texture's (two-texture surfaces)
        float nx, ny, nz; // lit batches
        uint8_t r, g, b, a;
        uint8_t bones;
        uint8_t bone[3];
        float weight[3];
        uint32_t skin[4]; // the skin qword as it came (shadow models read it their own way)
    };
    using Strip = std::vector<BatchVertex>;

    // The packet's batches, one triangle strip each (they end in MSCNT).
    // STCYCL n+1 interleaves n fields per vertex; each UNPACK's address is
    // its field: 0 GIF tag, 1 position (V3-32), then by format
    //   V2-16 ST, V4-8 colour (unlit)
    //   V4-16 two ST pairs (two textures), V4-8 colour
    //   V3-32 normal, V2-16 ST, V4-8 colour (lit)
    //   V4-16 normal, V2-16 ST, V4-8 colour, V4-32 skin (skinned)
    // Skin: w = bone count, x/y/z = bone index + weight (the fraction; one
    // bone: full weight); the bone is palette matrix n (VU1 qwords 4n..4n+3).
    void readBatches(uint32_t addr, uint32_t size, std::vector<Strip> &out)
    {
        const uint32_t end = addr + size;
        uint32_t p = addr;
        uint32_t positions = 0, normals = 0, colours = 0, uvs = 0, pairs = 0, skin = 0, count = 0;
        while (p < end)
        {
            const uint32_t code = word(p);
            p += 4;
            const uint32_t cmd = (code >> 24) & 0x7Fu, num = (code >> 16) & 0xFFu, field = code & 0x3FFu;
            uint32_t words = 0;
            if ((cmd & 0x60u) == 0x60u)
            {
                const uint32_t vn = (cmd >> 2) & 3u, vl = cmd & 3u, n = num ? num : 256u;
                words = (n * (vn + 1u) * (32u >> vl) + 31u) / 32u;
                switch (cmd & 0x6Fu)
                {
                case 0x68u: // V3-32: position at field 1 (a normal elsewhere)
                    if (field == 1u)
                    {
                        positions = p;
                        count = n;
                    }
                    else
                        normals = p;
                    break;
                case 0x6Eu:
                    colours = p;
                    break;
                case 0x65u: // ST, 12-bit fraction (VIF ROW taken as 0)
                    uvs = p;
                    break;
                case 0x6Du: // two ST pairs, or a normal
                    pairs = p;
                    break;
                case 0x6Cu:
                    if (field != 0u)
                        skin = p;
                    break;
                }
            }
            else if (cmd == 0x20u)
                words = 1;
            else if (cmd == 0x30u || cmd == 0x31u)
                words = 4;
            else if (cmd == 0x4Au)
                words = (num ? num : 256u) * 2u;
            else if (cmd == 0x50u || cmd == 0x51u)
                words = ((code & 0xFFFFu) ? (code & 0xFFFFu) : 65536u) * 4u;
            else if (cmd == 0x14u || cmd == 0x15u || cmd == 0x17u) // the batch goes to VU1
            {
                if (positions)
                {
                    Strip strip(count);
                    for (uint32_t i = 0; i < count; ++i)
                    {
                        BatchVertex &v = strip[i];
                        v = {real(positions + i * 12u), real(positions + i * 12u + 4u), real(positions + i * 12u + 8u), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 128, 128, 128, 128, 0, {0, 0, 0}, {0.0f, 0.0f, 0.0f}};
                        // V4-16 is the normal when the ST comes apart (V2-16), else two ST pairs.
                        const uint32_t st = uvs ? word(uvs + i * 4u) : (pairs ? word(pairs + i * 8u) : 0u);
                        if (pairs && !uvs) // the second texture's pair
                        {
                            const uint32_t st2 = word(pairs + i * 8u + 4u);
                            v.u2 = static_cast<int16_t>(st2 & 0xFFFFu) / 4096.0f;
                            v.v2 = static_cast<int16_t>(st2 >> 16) / 4096.0f;
                        }
                        if (normals)
                        {
                            v.nx = real(normals + i * 12u);
                            v.ny = real(normals + i * 12u + 4u);
                            v.nz = real(normals + i * 12u + 8u);
                        }
                        else if (pairs && uvs)
                        {
                            const uint32_t xy = word(pairs + i * 8u), z = word(pairs + i * 8u + 4u);
                            v.nx = static_cast<int16_t>(xy & 0xFFFFu) / 4096.0f;
                            v.ny = static_cast<int16_t>(xy >> 16) / 4096.0f;
                            v.nz = static_cast<int16_t>(z & 0xFFFFu) / 4096.0f;
                        }
                        v.u = static_cast<int16_t>(st & 0xFFFFu) / 4096.0f;
                        v.v = static_cast<int16_t>(st >> 16) / 4096.0f;
                        if (colours)
                        {
                            const uint32_t c = word(colours + i * 4u);
                            v.r = static_cast<uint8_t>(c);
                            v.g = static_cast<uint8_t>(c >> 8);
                            v.b = static_cast<uint8_t>(c >> 16);
                            v.a = static_cast<uint8_t>(c >> 24);
                        }
                        if (skin)
                        {
                            const uint32_t q = skin + i * 16u;
                            for (uint32_t k = 0; k < 4u; ++k)
                                v.skin[k] = word(q + k * 4u);
                            v.bones = static_cast<uint8_t>(std::min(word(q + 12u), 3u));
                            for (uint32_t k = 0; k < v.bones; ++k)
                            {
                                const float packed = real(q + k * 4u);
                                const float bone = std::floor(packed);
                                v.bone[k] = static_cast<uint8_t>(std::clamp(bone, 0.0f, 255.0f));
                                v.weight[k] = v.bones == 1 ? 1.0f : packed - bone;
                            }
                        }
                    }
                    out.push_back(std::move(strip));
                }
                positions = normals = colours = uvs = pairs = skin = count = 0;
            }
            p += words * 4u;
        }
    }

    bool lit(const std::vector<Strip> &strips)
    {
        for (const Strip &strip : strips)
            if (!strip.empty() && (strip[0].nx != 0.0f || strip[0].ny != 0.0f || strip[0].nz != 0.0f))
                return true;
        return false;
    }

    // Strips as a triangle list; `place` maps a batch vertex (position,
    // normal) to the mesh's space. Unlit strips get face normals.
    template <typename Place>
    void triangulate(const std::vector<Strip> &strips, Place place, std::vector<native_renderer::Vertex> &out, bool secondTexture = false)
    {
        std::vector<native_renderer::Vertex> placed;
        for (const Strip &strip : strips)
        {
            placed.resize(strip.size());
            for (size_t i = 0; i < strip.size(); ++i)
            {
                const BatchVertex &v = strip[i];
                native_renderer::Vertex &o = placed[i];
                place(v, o);
                o.u = secondTexture ? v.u2 : v.u;
                o.v = secondTexture ? v.v2 : v.v;
                o.r = v.r;
                o.g = v.g;
                o.b = v.b;
                o.a = v.a;
            }
            for (size_t i = 2; i < placed.size(); ++i)
            {
                native_renderer::Vertex t[3] = {placed[i - 2], placed[i - 1], placed[i]};
                const float ux = t[1].x - t[0].x, uy = t[1].y - t[0].y, uz = t[1].z - t[0].z;
                const float vx = t[2].x - t[0].x, vy = t[2].y - t[0].y, vz = t[2].z - t[0].z;
                float nx = uy * vz - uz * vy, ny = uz * vx - ux * vz, nz = ux * vy - uy * vx;
                const float len = std::sqrt(nx * nx + ny * ny + nz * nz);
                if (!(len > 0.0f))
                    continue; // degenerate (strip joins)
                for (native_renderer::Vertex &v : t)
                {
                    if (v.nx == 0.0f && v.ny == 0.0f && v.nz == 0.0f)
                    {
                        v.nx = nx / len;
                        v.ny = ny / len;
                        v.nz = nz / len;
                    }
                    out.push_back(v);
                }
            }
        }
    }

    bool skinned(const std::vector<Strip> &strips)
    {
        for (const Strip &strip : strips)
            if (!strip.empty() && strip[0].bones)
                return true;
        return false;
    }

    // An NTO2 texture (level 0) as RGBA8, on the GPU once under its header address.
    // The game streams data in and reuses memory when the area changes: what
    // is cached under an address is also keyed by a sample of its contents.
    uint64_t sample(uint32_t addr, uint32_t bytes, uint32_t samples)
    {
        uint64_t h = 0xcbf29ce484222325ull ^ addr;
        const uint32_t step = std::max<uint32_t>((bytes / samples) & ~3u, 4u);
        for (uint32_t off = 0; off + 4u <= bytes; off += step)
            h = (h ^ word(addr + off)) * 0x100000001b3ull;
        return h;
    }

    uint64_t texture(uint32_t header)
    {
        if (!header || word(header) != 0x324F544Eu) // "NTO2"
            return 0;
        const uint32_t format = word(header + 0x1Cu), psm = format & 0x3Fu;
        const uint32_t width = 1u << ((format >> 16) & 0xFu), height = 1u << ((format >> 20) & 0xFu);
        const uint32_t pixelBytes = psm == 0x14u ? width * height / 2u : width * height;
        const uint64_t key = sample(header + word(header + 0x14u), pixelBytes, 64u) ^
                             sample(header + word(header + 0x18u), psm == 0x14u ? 64u : 1024u, 16u) * 31u ^ format;
        if (native_renderer::hasTexture(key))
            return key;
        if ((psm != 0x00u && psm != 0x13u && psm != 0x14u) || width > 1024u || height > 1024u)
        {
            static const bool log = std::getenv("SOTC_NATIVE_LOG") != nullptr;
            static std::set<uint32_t> seen;
            if (log && seen.insert(header).second)
                std::fprintf(stderr, "[native-tex] header %08x psm %02x %ux%u format %08x swizzled %u\n", header, psm, width, height, format,
                             g_ram[(header + 0x1Fu) & 0x1FFFFFFu] & 1u);
            return 0;
        }
        const uint32_t clut = header + word(header + 0x18u);
        const uint32_t entries = psm == 0x14u ? 16u : 256u;
        uint32_t palette[256];
        if (psm != 0x00u)
            for (uint32_t i = 0; i < entries; ++i)
            {
                // CSM1: of every 32 entries, 8-15 and 16-23 are swapped.
                const uint32_t from = entries == 256u ? ((i & ~0x18u) | ((i & 8u) << 1) | ((i & 16u) >> 1)) : i;
                palette[i] = word(clut + from * 4u);
            }
        // Mip levels (format bits 12-15, texTransResolve): each half the last,
        // stored one after the other; byte +0x1f has a swizzle bit per level.
        static const bool noMips = std::getenv("SOTC_NATIVE_NOMIPS") != nullptr;
        const uint32_t levels = noMips ? 1u : std::clamp((format >> 12) & 0xFu, 1u, 7u);
        const uint8_t swizzleBits = g_ram[(header + 0x1Fu) & 0x1FFFFFFu];
        std::vector<uint8_t> rgba;
        std::vector<uint8_t> scratch;
        uint32_t pixels = header + word(header + 0x14u), w = width, h = height, decoded = 0;
        for (uint32_t level = 0; level < levels && w >= 1u && h >= 1u; ++level)
        {
            const size_t base = rgba.size();
            rgba.resize(base + static_cast<size_t>(w) * h * 4u);
            uint8_t *out = &rgba[base];
            const bool swizzled = ((swizzleBits >> level) & 1u) != 0u;
            if (psm == 0x00u)
            {
                // PSMCT32: the pixels themselves (alpha 0x80 = opaque).
                for (uint32_t i = 0; i < w * h; ++i)
                    std::memcpy(out + i * 4u, g_ram + ((pixels + i * 4u) & 0x1FFFFFFu), 4);
            }
            else
            {
                if (swizzled)
                {
                    // Uploaded as PSMCT32 and read back as 8/4-bit: replay that.
                    const uint32_t cw = std::max(w / 2u, 1u), ch = std::max(psm == 0x13u ? h / 2u : h / 4u, 1u);
                    scratch.assign(static_cast<size_t>(std::max(w, 64u)) * std::max(h, 64u) + 0x4000u, 0);
                    const uint32_t bw32 = std::max<uint32_t>(cw / 64u, 1u);
                    for (uint32_t i = 0; i < cw * ch; ++i)
                    {
                        const uint32_t a = GSPSMCT32::addrPSMCT32(0, bw32, i % cw, i / cw);
                        if (a + 4u <= scratch.size())
                            std::memcpy(&scratch[a], g_ram + ((pixels + i * 4u) & 0x1FFFFFFu), 4);
                    }
                }
                const uint32_t tbw = std::max<uint32_t>(w / 64u, 2u);
                for (uint32_t y = 0; y < h; ++y)
                    for (uint32_t x = 0; x < w; ++x)
                    {
                        uint32_t index;
                        if (swizzled && psm == 0x14u)
                        {
                            const uint32_t n = GSPSMT4::addrPSMT4(0, tbw, x, y);
                            const uint8_t b = (n >> 1) < scratch.size() ? scratch[n >> 1] : 0;
                            index = (n & 1u) ? (b >> 4) : (b & 15u);
                        }
                        else if (swizzled)
                        {
                            const uint32_t a = GSPSMT8::addrPSMT8(0, tbw, x, y);
                            index = a < scratch.size() ? scratch[a] : 0u;
                        }
                        else if (psm == 0x14u)
                        {
                            const uint8_t b = g_ram[(pixels + (y * w + x) / 2u) & 0x1FFFFFFu];
                            index = (x & 1u) ? (b >> 4) : (b & 15u);
                        }
                        else
                            index = g_ram[(pixels + y * w + x) & 0x1FFFFFFu];
                        std::memcpy(out + (static_cast<size_t>(y) * w + x) * 4u, &palette[index & (entries - 1u)], 4);
                    }
            }
            ++decoded;
            pixels += psm == 0x00u ? w * h * 4u : (psm == 0x13u ? w * h : w * h / 2u);
            w /= 2u;
            h /= 2u;
        }
        // Metal wants the whole chain down to the smallest it was given; stop
        // where a level would be under 1 pixel (kept as decoded).
        // SOTC_TEXDUMP_DIR=<dir> (debugging): level 0 of every decoded texture, raw RGBA.
        if (const char *dir = std::getenv("SOTC_TEXDUMP_DIR"))
        {
            char path[512];
            std::snprintf(path, sizeof(path), "%s/nat_%08x_psm%02x_%ux%u.rgba", dir, header, psm, width, height);
            if (FILE *f = std::fopen(path, "wb"))
            {
                std::fwrite(rgba.data(), 1, static_cast<size_t>(width) * height * 4u, f);
                std::fclose(f);
            }
        }
        native_renderer::uploadTexture(key, width, height, rgba.data(), decoded);
        return key;
    }

    // The surface's texture: +0x2c, or +0x3c for the second (texture mode 2).
    uint64_t surfaceTexture(uint32_t header, uint32_t surface, uint32_t field = 0x2Cu)
    {
        const uint32_t s = word(header + 0x50u) + surface * 0x120u;
        const uint32_t index = word(s + field);
        if (index >= word(header + 0x44u))
            return 0;
        const uint32_t object = word(word(header + 0x40u) + index * 0x20u + 0x10u);
        return object ? texture(word(object + 0x20u)) : 0;
    }

    // modelGetDlLayer (0x1096a80): the display list layer of a surface; the
    // layers go to the GS in increasing order, so it is the draw order.
    uint32_t surfaceLayer(uint32_t header, uint32_t s, bool fading)
    {
        uint64_t flags;
        std::memcpy(&flags, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
        const uint32_t owner = word(header + 8u);
        const bool second = owner && word(owner + 0x38u) == 1u;
        const auto bit = [flags](int n) { return ((flags >> n) & 1u) != 0; };
        if (flags & 0x1020000u)
            return 8;
        if (bit(23))
            return 10;
        if (bit(25))
            return 16;
        if (bit(9))
            return 17;
        if (bit(10))
            return (bit(8) || fading) ? 13 : 12;
        if (bit(14))
            return 1;
        if (bit(15))
            return 18;
        if (bit(8) || fading)
            return second ? 6 : 3;
        return second ? 5 : 2;
    }

    // modelDisp's skips: bit 29 never drawn; bit 17 (boss effect) and bit 24
    // (shadow effect) only while getNowBossEffect (0x1186290) has bit 0 / 1;
    // bit 26 not while the underwater fog (getFogUnderWater, 0x1186328) is on.
    bool surfaceDrawn(uint32_t s)
    {
        uint64_t flags;
        std::memcpy(&flags, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
        const uint32_t boss = word(0x1186290u);
        if ((flags >> 29) & 1u)
            return false;
        if ((flags >> 17) & 1u)
            return (boss & 1u) != 0;
        if (((flags >> 24) & 1u) && !(boss & 2u))
            return false;
        return !(((flags >> 26) & 1u) && word(0x1186328u) != 0u);
    }

    // Texture scrolling: texTransResolve puts a VIF STROW in each texture's
    // transfer packet, the row the 16-bit values at *(NTO2 header +0xc) and
    // +4, which the game moves every frame. The ST unpacks use STMOD 1, so
    // the row adds to S and T (12-bit fraction).
    bool surfaceRow(uint32_t header, uint32_t surface, float offset[2], uint32_t field = 0x2Cu)
    {
        const uint32_t s = word(header + 0x50u) + surface * 0x120u;
        const uint32_t index = word(s + field);
        if (index >= word(header + 0x44u))
            return false;
        const uint32_t object = word(word(header + 0x40u) + index * 0x20u + 0x10u);
        const uint32_t nto2 = object ? word(object + 0x20u) : 0u;
        const uint32_t scroll = nto2 ? word(nto2 + 0xCu) : 0u;
        if (!scroll)
            return false;
        uint32_t pair;
        std::memcpy(&pair, g_ram + (scroll & 0x1FFFFFCu), 4);
        const int32_t r0 = static_cast<int32_t>(pair & 0xFFFFu), r1 = static_cast<int32_t>(word(scroll + 4u) & 0xFFFFu);
        offset[0] = static_cast<float>(r0) / 4096.0f;
        offset[1] = static_cast<float>(r1) / 4096.0f;
        return r0 != 0 || r1 != 0;
    }

    // Mip selection as texTransResolve sets TEX1 from the NTO2 header: MXL =
    // levels - 1, MMIN +0x78 (2-5: mipmapped), L +0x94, K = +0x90 x
    // *0x118c180 (1/16 steps); the GS level is log2(1/Q) x 2^L + K, Q = 1/w.
    void surfaceLod(uint32_t header, uint32_t surface, native_renderer::DrawState &state, uint32_t field = 0x2Cu)
    {
        const uint32_t s = word(header + 0x50u) + surface * 0x120u;
        const uint32_t index = word(s + field);
        if (index >= word(header + 0x44u))
            return;
        const uint32_t object = word(word(header + 0x40u) + index * 0x20u + 0x10u);
        const uint32_t nto2 = object ? word(object + 0x20u) : 0u;
        if (!nto2 || word(nto2) != 0x324F544Eu)
            return;
        const uint32_t levels = std::clamp((word(nto2 + 0x1Cu) >> 12) & 0xFu, 1u, 7u);
        if (levels < 2u || word(nto2 + 0x78u) < 2u)
            return;
        state.maxLod = static_cast<float>(levels - 1u);
        state.lodScale = static_cast<float>(1u << (word(nto2 + 0x94u) & 3u));
        const int32_t k = static_cast<int32_t>(static_cast<float>(static_cast<int32_t>(word(nto2 + 0x90u))) * real(0x0118C180u));
        state.lodK = static_cast<float>((k << 20) >> 20) / 16.0f; // 12-bit signed, 7.4
    }

    // ALPHA_1 (0x42, first pass) or ALPHA_2 (0x43, the second texture's
    // pass) in the surface's A+D packet (+0x60): 0x44 blends, 0x48 adds,
    // 0x42 subtracts (Cd - Cs x As).
    // CLAMP_1 (0x08, first pass) or CLAMP_2 (0x09, the second texture's) in
    // the surface's A+D packet: WMS / WMT 1 (clamp) or 2 (region clamp)
    // keep the coordinate inside the texture; repeating there pulls the far
    // edge into the border under bilinear filtering (light seams).
    uint8_t surfaceClamp(uint32_t s, uint32_t reg = 0x08u)
    {
        for (uint32_t q = s + 0x60u; q < s + 0xC0u; q += 16u)
            if (word(q + 8u) == reg)
            {
                const uint32_t v = word(q), wms = v & 3u, wmt = (v >> 2) & 3u;
                return static_cast<uint8_t>(((wms == 1u || wms == 2u) ? 1u : 0u) | ((wmt == 1u || wmt == 2u) ? 2u : 0u));
            }
        return 0;
    }

    native_renderer::Blend surfaceBlend(uint32_t s, uint32_t reg = 0x42u)
    {
        // SOTC_LOG_SURFACE_REGS=1: the distinct GS registers of the surfaces' A+D packets.
        static const bool logRegs = std::getenv("SOTC_LOG_SURFACE_REGS") != nullptr;
        if (logRegs && reg == 0x42u)
        {
            static std::set<std::pair<uint32_t, uint64_t>> seen;
            for (uint32_t q = s + 0x60u; q < s + 0xC0u; q += 16u)
            {
                const uint32_t r = word(q + 8u);
                const uint64_t v = (static_cast<uint64_t>(word(q + 4u)) << 32) | word(q);
                if (r < 0x80u && seen.insert({r, v}).second && seen.size() < 400)
                    std::fprintf(stderr, "[surface] reg %02x = %016llx\n", r, static_cast<unsigned long long>(v));
            }
        }
        for (uint32_t q = s + 0x60u; q < s + 0xC0u; q += 16u)
            if (word(q + 8u) == reg)
                switch (word(q) & 0xFFu)
                {
                case 0x48u: return native_renderer::Blend::Additive;
                case 0x42u: return native_renderer::Blend::Subtract;
                default: return native_renderer::Blend::Normal;
                }
        return native_renderer::Blend::Normal;
    }

    // Skinned parts: their instance chain (instance + 0xc: a NEXT tag past
    // the instance's own 15 qwords) ends in a RET packet that unpacks the
    // bone palette to VU1 qword 0 (modelSetMatrixTransTag), each matrix
    // bone x view (camera space, the projection comes after). Rigid parts
    // upload one matrix there (5 qwords). False when there is no palette.
    bool readChainPalette(uint32_t own, std::vector<float> &vu);
    bool readPalette(uint32_t instance, std::vector<float> &vu)
    {
        return readChainPalette(word(instance + 0xCu), vu);
    }
    bool readChainPalette(uint32_t own, std::vector<float> &vu)
    {
        if (((word(own) >> 28) & 7u) != 2u)
            return false;
        const uint32_t ret = word(own + 4u), qwc = word(ret) & 0xFFFFu;
        if (qwc <= 5u)
            return false;
        vu.assign(1024u * 4u, 0.0f);
        const uint32_t end = ret + 16u + qwc * 16u;
        for (uint32_t p = ret + 8u; p < end;)
        {
            const uint32_t code = word(p);
            p += 4;
            const uint32_t cmd = (code >> 24) & 0x7Fu;
            if ((cmd & 0x6Fu) != 0x6Cu)
                continue; // NOP / STCYCL / MSCAL: one word
            const uint32_t n = ((code >> 16) & 0xFFu) ? ((code >> 16) & 0xFFu) : 256u, at = code & 0x3FFu;
            for (uint32_t q = 0; q < n && at + q < 1024u; ++q)
                for (uint32_t k = 0; k < 4u; ++k)
                    vu[(at + q) * 4u + k] = real(p + q * 16u + k * 4u);
            p += n * 16u;
        }
        return true;
    }

    struct SkinnedPacket
    {
        std::vector<Strip> strips; // bind pose
        uint64_t used = 0;         // the last frame (g_frame) that drew it
    };
    std::unordered_map<uint64_t, SkinnedPacket> g_skinnedPackets; // by packet
    std::unordered_set<uint64_t> g_litPackets;                        // packets with normals
    uint64_t g_skinnedDraws = 0;                                       // this frame's skinned meshes

    void flush2D();

    // zFog (0x10ac0a8, layer 4): after the opaque world (layers 2-3) it
    // writes a fog amount into the frame's alpha by depth, in N + 1 slices
    // (globals at 0x118c46c: N, width, margin; 0x118c480: far) from
    // far - margin - width to far - margin (view depth), and bgDraw then
    // blends the background buffer (the sky, layer 1) over the scene by it.
    constexpr uint32_t kZFog = 0x010AC0A8u;
    PS2Runtime::RecompiledFunction g_zFog = nullptr;
    native_renderer::Fog g_fog;

    // The bloom chain (0x109e2a0, layer 0x14; it runs in pieces, so it is
    // seen through gsReg): a mask of what lies beyond GS Z 0x118c094 (0:
    // only empty pixels, the sky) in colour 0x118c0d0, reduced, blurred with
    // 12 shifted sprites (weights at 0x118c0f0/100/110), lerped with the last
    // frame's by 0x118c12c / 128 and added over the frame in colour
    // 0x118c140 (alpha 0x118c14c). The native renderer does the same with
    // its own passes; the values are read when the chain ran.
    bool g_bloomRan = false;

    native_renderer::Bloom bloomParams()
    {
        native_renderer::Bloom b;
        const native_renderer::Tuning &tune = native_renderer::tuning();
        b.enabled = g_cameraContext != 0 && tune.bloom;
        b.order = 20;
        b.threshold = static_cast<float>(word(0x118C094u));
        b.zA = real(g_cameraContext + 0x4A0u + 10u * 4u);
        b.zB = real(g_cameraContext + 0x4A0u + 14u * 4u);
        for (int i = 0; i < 3; ++i)
            b.maskColor[i] = static_cast<float>(word(0x118C0D0u + i * 4u) & 0xFFu) / 255.0f;
        for (int i = 0; i < 3; ++i)
            b.color[i] = static_cast<float>(word(0x118C140u + i * 4u) & 0xFFu) / 128.0f;
        // The add is Cs x As + Cd, As the composite colour's alpha (the GS
        // chain: mask in 0x118c0d0, a lerp blur of 12 shifted copies by alpha
        // 0x40 / 0x20 / 0x10, the temporal lerp, then this add at full
        // strength; SOTC_BLOOM_SCALE overrides).
        const float scale = tune.bloomStrength;
        b.color[3] = static_cast<float>(word(0x118C14Cu) & 0xFFu) / 128.0f * scale;
        const uint32_t temporal = word(0x118C12Cu) & 0xFFu;
        b.temporal = temporal ? static_cast<float>(temporal) / 128.0f : 1.0f;
        return b;
    }

    void zFogHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_ram = rdram;
        const int32_t slices = static_cast<int32_t>(word(0x118C46Cu));
        const float width = real(0x118C470u), margin = real(0x118C474u), farDistance = real(0x118C480u);
        static const bool log = std::getenv("SOTC_NATIVE_LOG") != nullptr;
        static uint64_t logged = 0;
        if (log && g_frame >= logged + 300u)
        {
            logged = g_frame;
            std::fprintf(stderr, "[native-fog] frame %llu slices %d width %g margin %g far %g c478 %g c47c %g near %g\n", (unsigned long long)g_frame, slices, width, margin,
                         farDistance, real(0x118C478u), real(0x118C47Cu), real(0x118C484u));
        }
        // The native renderer redoes it as one pass over the picture by depth
        // (native_renderer::Fog); SOTC_NATIVE_ZFOG=0 leaves it out.
        const bool zFogOn = native_renderer::tuning().zFog;
        if (zFogOn && slices > 0 && width > 0.0f)
        {
            g_fog.enabled = true;
            g_fog.width = width;
            g_fog.start = farDistance - margin - width;
            g_fog.steps = slices;
        }
        // SOTC_SKIP_ZFOG=1 (debugging): the game's own zFog does not run.
        static const bool skip = std::getenv("SOTC_SKIP_ZFOG") != nullptr;
        if (!skip)
            g_zFog(rdram, ctx, runtime);
    }

    void endFrame()
    {
        if (g_frame == 0)
            return;
        flush2D();
        // The GS vertex fog (every world batch has FGE): F = base + scale / w
        // from getFogFactor0/1 (0x1089b70/bb0), the colour as
        // graphSetFogParam last set FOGCOL (0x118b8dc/e0/e4).
        {
            const float nearD = real(0x12147DCu), farD = real(0x12147E0u), nearR = real(0x12147E4u), farR = real(0x12147E8u), k = real(0x12147ECu);
            const native_renderer::Tuning &tune = native_renderer::tuning();
            if (tune.vertexFog && farD != nearD)
            {
                static const float baseScale = std::getenv("SOTC_FOG_BASE_SCALE") ? static_cast<float>(std::atof(std::getenv("SOTC_FOG_BASE_SCALE"))) : 1.0f;
                g_fog.vertexFog = true;
                g_fog.scale = farD * nearD * (nearR - farR) / (farD - nearD);
                // The camera packet of every layer (0x1098e20) carries these
                // two values for the microcode: F = factor1 + factor0 / w.
                g_fog.base = ((farR - nearR) * (farD + nearD) / (farD - nearD) + farR + nearR) * k * baseScale;
                // The tuning's amount scales the fog's share: F' = 255 - (255 - F) x amount.
                const float amount = std::max(0.0f, tune.vertexFogAmount);
                g_fog.base = 255.0f - (255.0f - g_fog.base) * amount;
                g_fog.scale *= amount;
                for (int i = 0; i < 3; ++i)
                    g_fog.colour[i] = static_cast<float>(word(0x118B8DCu + i * 4u) & 0xFFu) / 255.0f;
            }
            static const bool log = std::getenv("SOTC_NATIVE_LOG") != nullptr;
            static uint64_t logged = 0;
            if (log && g_frame >= logged + 300u)
            {
                logged = g_frame;
                std::fprintf(stderr, "[native-vfog] frame %llu near %g far %g rates %g %g k %g -> base %g scale %g colour %u %u %u\n", (unsigned long long)g_frame,
                             nearD, farD, nearR, farR, k, g_fog.base, g_fog.scale, word(0x118B8DCu), word(0x118B8E0u), word(0x118B8E4u));
            }
        }
        if (std::getenv("SOTC_NATIVE_LOG") && g_frame % 60u == 0u)
            std::fprintf(stderr, "[bossfx] frame %llu flags %08x a0 %d b0 %d b4 %d b8 %d bc %d c0 %d c4 %d 298 %g\n", (unsigned long long)g_frame, word(0x1186290u),
                         static_cast<int32_t>(word(0x11862A0u)), static_cast<int32_t>(word(0x11862B0u)), static_cast<int32_t>(word(0x11862B4u)), static_cast<int32_t>(word(0x11862B8u)),
                         static_cast<int32_t>(word(0x11862BCu)), static_cast<int32_t>(word(0x11862C0u)), static_cast<int32_t>(word(0x11862C4u)), real(0x1186298u));
        if (std::getenv("SOTC_NATIVE_LOG") && g_frame % 120u == 0u)
            std::fprintf(stderr, "[dof] frame %llu on %u rounds %d slices %d focus %g colour %08x %08x %08x %08x\n", (unsigned long long)g_frame, word(0x11862D0u),
                         static_cast<int32_t>(word(0x11862D4u)), static_cast<int32_t>(word(0x11862D8u)), static_cast<float>(static_cast<int32_t>(word(0x11862DCu))),
                         word(0x11862E0u), word(0x11862E4u), word(0x11862E8u), word(0x11862ECu));
        if (std::getenv("SOTC_NATIVE_LOG") && g_frame % 60u == 0u)
        {
            std::fprintf(stderr, "[bloomcol] %llu:", (unsigned long long)g_frame);
            for (uint32_t a = 0x118C0C0u; a < 0x118C150u; a += 4u)
                std::fprintf(stderr, " %x", word(a));
            std::fprintf(stderr, "\n");
        }
        if (std::getenv("SOTC_NATIVE_LOG") && g_frame % 60u == 0u)
            std::fprintf(stderr, "[native-frame] %llu bloomZ %u ran %d zfog %d start %g width %g vfog %d base %g scale %g col %g %g %g models %u\n", (unsigned long long)g_frame, word(0x118C094u), g_bloomRan ? 1 : 0, g_fog.enabled ? 1 : 0,
                         g_fog.start, g_fog.width, g_fog.vertexFog ? 1 : 0, g_fog.base, g_fog.scale, g_fog.colour[0], g_fog.colour[1], g_fog.colour[2], g_modelDraws);
        if (std::getenv("SOTC_NATIVE_LOG") && g_frame % 120u == 0u)
        {
            const uint32_t table = word(0x118BFCCu);
            for (uint32_t layer : {1u, 2u, 3u})
            {
                const uint32_t pk = table ? word(table + layer * 4u) : 0u;
                if (pk)
                    std::fprintf(stderr, "[layerfog] frame %llu layer %u q22 %g %g q23 %g %g q24 %g %g q25 %g %g\n", (unsigned long long)g_frame, layer,
                                 real(pk + 0x110u), real(pk + 0x114u), real(pk + 0x118u), real(pk + 0x11Cu), real(pk + 0x120u), real(pk + 0x124u),
                                 real(pk + 0x128u), real(pk + 0x12Cu));
            }
        }
        {
            // bossDeathEffect (on while the word 0x1186290 is set; most of the
            // game): layer 8 is drawn apart and laid over the picture sharp
            // (alpha 0x11862c4) and blurred (0x11862c0, 0x11862a0 rounds), in the
            // tint 0x11862b0..b8 (0x80 = 1). SOTC_NATIVE_NOGLOW=1 draws it plainly.
            native_renderer::GlowComposite gc;
            gc.enabled = native_renderer::tuning().glow && word(0x1186290u) != 0u && g_modelDraws != 0u;
            gc.order = 8;
            gc.rounds = std::clamp(static_cast<int32_t>(word(0x11862A0u)), 0, 8);
            for (int i = 0; i < 3; ++i)
                gc.tint[i] = static_cast<float>(static_cast<int32_t>(word(0x11862B0u + i * 4u))) / 128.0f;
            gc.amountA = static_cast<float>(static_cast<int32_t>(word(0x11862C4u))) / 128.0f;
            gc.amountB = static_cast<float>(static_cast<int32_t>(word(0x11862C0u))) / 128.0f;
            native_renderer::setGlowComposite(gc);
        }
        {
            // depthOfField (0x10874e8, on while 0x11862d0 is set): focus
            // distance 0x11862dc taken to GS Z by the camera's 0x4a0 matrix,
            // slices 0x11862d8, blur rounds 0x11862d4.
            native_renderer::DepthOfField dof;
            if (native_renderer::tuning().depthOfField && g_cameraContext && word(0x11862D0u) != 0u && g_modelDraws != 0u)
            {
                const float focus = static_cast<float>(static_cast<int32_t>(word(0x11862DCu)));
                const uint32_t m = g_cameraContext + 0x4A0u;
                const float w = real(m + 11u * 4u) * focus + real(m + 15u * 4u);
                dof.enabled = w != 0.0f;
                dof.zA = real(m + 10u * 4u);
                dof.zB = real(m + 14u * 4u);
                dof.focusZ = dof.enabled ? static_cast<uint32_t>(std::max(0.0f, (real(m + 10u * 4u) * focus + real(m + 14u * 4u)) / w)) : 0u;
                dof.slices = std::clamp(static_cast<int32_t>(word(0x11862D8u)), 0, 8);
                dof.rounds = std::clamp(static_cast<int32_t>(word(0x11862D4u)), 0, 4);
            }
            native_renderer::setDepthOfField(dof);
        }
        native_renderer::setFog(g_fog);
        g_fog = native_renderer::Fog{};
        // Without 3D this frame (loading, menus over black) the bloom mask would
        // take the empty screen for sky and glow all over it.
        native_renderer::setBloom(g_bloomRan && g_modelDraws ? bloomParams() : native_renderer::Bloom{});
        g_modelDraws = 0;
        g_bloomRan = false;
        // Drawn larger than the PS2's 512 x 448 (F10 / SOTC_RENDER_SCALE): the
        // geometry has no resolution; the effects measure in PS2 pixels.
        const float scale = std::clamp(native_renderer::tuning().renderScale, 0.5f, 4.0f);
        native_renderer::endFrame(static_cast<uint32_t>(512.0f * scale + 0.5f), static_cast<uint32_t>(448.0f * scale + 0.5f));
        static const bool show = std::atoi(std::getenv("SOTC_NATIVE")) != 2; // 2: draw, keep the emulated picture
        native_renderer::setShown(show && native_renderer::tuning().showNative && g_frameDraws != 0);
    }

    void getCameraContextHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_getCameraContext(rdram, ctx, runtime);
        g_cameraContext = GPR_U32(ctx, 2);
    }

    // A frame of the game ends when the display list goes to the VIF1.
    void syncFrame(uint8_t *rdram)
    {
        g_ram = rdram;

        const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        if (frame != g_frame)
        {
            endFrame();
            native_renderer::beginFrame();
            g_frame = frame;
            g_frameDraws = 0;
            g_skinnedDraws = 0;
            // A packet's id hashes its content: another area loaded in its
            // place makes new ids, the old ones never come back (the title's
            // demo filled the Vita's heap). Gone after ~30 s undrawn.
            static const bool cacheLog = std::getenv("SOTC_GL_CACHE_LOG") != nullptr;
            if (cacheLog && frame % 600u == 0u)
            {
                size_t strips = 0;
                for (const auto &p : g_skinnedPackets)
                    strips += p.second.strips.size();
                std::fprintf(stderr, "[skinned:cache] frame %llu packets %zu strips %zu lit %zu\n", (unsigned long long)frame,
                             g_skinnedPackets.size(), strips, g_litPackets.size());
            }
            if (frame % 256u == 0u)
                for (auto it = g_skinnedPackets.begin(); it != g_skinnedPackets.end();)
                    if (it->second.used + 1800u < frame)
                    {
                        g_litPackets.erase(it->first);
                        it = g_skinnedPackets.erase(it);
                    }
                    else
                        ++it;
        }
    }

    // ---- 2D ---------------------------------------------------------------
    //
    // SotC's 2D functions write GS registers with gsReg(reg, value)
    // (0x1c5b00): PRIM, RGBAQ, UV or ST, then XYZ2 / XYZF2 per vertex, plus
    // ALPHA. They are read as they are written and assembled into
    // primitives here. The writes are told apart by gsReg's return address
    // (the recompiled functions run in pieces the dispatcher continues after
    // a call returns: a hook around a call sees none of its writes). The
    // texture is the last texTransRequest(resource) (the 2D functions ask for
    // theirs just before drawing; the resource's +0x20 is the NTO2 header),
    // the layer the current iosDlPushLayer. Screen effects (bloom, blur,
    // reflection, fog, boss death) draw into other buffers and are not
    // listed. Positions are 12.4 on a 512 x 448 screen centred on 2048;
    // ALPHA with C = FIX (fades) blends by FIX.
    constexpr uint32_t kGsReg = 0x001C5B00u, kTexTransRequest = 0x010AAD48u;
    constexpr uint32_t kPushLayer = 0x001C0210u, kPopLayer = 0x001BF8C0u;
    PS2Runtime::RecompiledFunction g_gsReg = nullptr, g_texTransRequest = nullptr, g_pushLayer = nullptr, g_popLayer = nullptr;

    struct Range
    {
        uint32_t start, end;
    };
    constexpr Range k2D[] = {
        {0x010A24C0u, 0x010A5270u}, // spriteDrawSpriteTexNoSet .. spriteDrawSpriteST2 (menus, title, map)
        {0x0108A288u, 0x0108A758u}, // drawFrontFog, fadeScreen, drawDemoBar (cinema bars)
        {0x0134B490u, 0x0134B6F8u}, // subtitles (pre-rendered lines, a textured sprite)
        {0x013F6B20u, 0x013F78A8u}, // HUD gauges (fans and strips)
        {0x014033F0u, 0x01403558u}, // title menu backdrop
        {0x001C2DD0u, 0x001C30D0u}, // loading indicator (lines)
        {0x0114FD00u, 0x01150278u}, // spriteDraw*, windowPrintf (windows)
        {0x01155B98u, 0x01155ED8u}, // windowSpriteNoTex, windowSprite
    };

    std::vector<uint32_t> g_layers; // iosDlPushLayer stack
    constexpr uint32_t kScreenCopy = 1u; // a 2D "header" for 0x10f37c8's screen copy
    constexpr uint32_t kPreviousFrame = 2u; // and for feedbackBlur's display buffer (the last frame shown)
    uint32_t g_requested = 0;       // NTO2 header of the last texTransRequest

    struct Assembler
    {
        uint64_t prim = 0, rgbaq = 0, alpha = 0x44;
        float s = 0.0f, t = 0.0f;
        uint32_t u = 0, v = 0;
        uint32_t header = 0, layer = 0;
        std::vector<native_renderer::Vertex> strip; // this primitive's vertices
        std::vector<native_renderer::Vertex> batch; // triangles under one state
        uint64_t batchKey = ~0ull;
        uint64_t batchPrim = 0, batchAlpha = 0;      // that state
        uint32_t batchHeader = 0, batchLayer = 0;
    } g_2d;

    void flush2D()
    {
        Assembler &a = g_2d;
        if (a.batch.empty())
            return;
        static const bool idColours = std::getenv("SOTC_NATIVE_IDCOLOR") != nullptr; // only the models then
        // Blending with A = B and D = Cd leaves the frame as it was ((Cs - Cs) x C + Cd).
        const uint64_t blendBits = a.batchAlpha & 0xFFu;
        const bool unchanged = (a.batchPrim & 0x40u) && (blendBits & 3u) == ((blendBits >> 2) & 3u) && ((blendBits >> 6) & 3u) == 1u;
        if (idColours || unchanged)
        {
            a.batch.clear();
            a.batchKey = ~0ull;
            return;
        }
        const uint64_t mesh = (1ull << 62) | g_skinnedDraws++;
        native_renderer::uploadMesh(mesh, a.batch.data(), static_cast<uint32_t>(a.batch.size()), true);
        native_renderer::DrawState state;
        state.screen = true;
        state.screenCopy = a.batchHeader == kScreenCopy && (a.batchPrim & 0x10u);
        state.previousFrame = a.batchHeader == kPreviousFrame && (a.batchPrim & 0x10u);
        state.texture = (a.batchPrim & 0x10u) && !state.screenCopy && !state.previousFrame ? texture(a.batchHeader) : 0;
        const uint32_t mode = static_cast<uint32_t>(a.batchAlpha & 0xFFu);
        // SOTC_LOG_2D_ALPHA=1: the distinct ALPHA / PRIM pairs of the 2D draws.
        static const bool logAlpha = std::getenv("SOTC_LOG_2D_ALPHA") != nullptr;
        if (logAlpha)
        {
            static std::set<std::tuple<uint64_t, uint64_t, uint32_t>> seen;
            if (seen.insert({a.batchAlpha, a.batchPrim & 0x7F7u, a.batchLayer}).second && seen.size() < 200)
                std::fprintf(stderr, "[2d-alpha] alpha %016llx prim %03llx layer %u header %08x verts %zu rgba %02x%02x%02x%02x\n",
                             static_cast<unsigned long long>(a.batchAlpha), static_cast<unsigned long long>(a.batchPrim & 0x7F7u), a.batchLayer, a.batchHeader,
                             a.batch.size(), a.batch[0].r, a.batch[0].g, a.batch[0].b, a.batch[0].a);
        }
        state.blend = !(a.batchPrim & 0x40u) ? native_renderer::Blend::Opaque
                      : (mode == 0x48u ? native_renderer::Blend::Additive : native_renderer::Blend::Normal);
        state.order = a.batchLayer;
        static const float identity[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        native_renderer::draw(mesh, identity, state);
        ++g_frameDraws;
        a.batch.clear();
        a.batchKey = ~0ull;
    }

    void emit2D(const native_renderer::Vertex &v0, const native_renderer::Vertex &v1, const native_renderer::Vertex &v2)
    {
        Assembler &a = g_2d;
        const uint64_t key = (a.prim & 0x150u) ^ ((a.alpha & 0xFFu) << 12) ^ (static_cast<uint64_t>(a.header) << 20) ^ (static_cast<uint64_t>(a.layer) << 56);
        if (key != a.batchKey)
        {
            flush2D();
            a.batchKey = key;
            a.batchPrim = a.prim;
            a.batchAlpha = a.alpha;
            a.batchHeader = a.header;
            a.batchLayer = a.layer;
        }
        a.batch.push_back(v0);
        a.batch.push_back(v1);
        a.batch.push_back(v2);
    }

    void vertex2D(uint64_t xyz, bool kick)
    {
        Assembler &a = g_2d;
        native_renderer::Vertex o{};
        o.x = ((static_cast<float>(xyz & 0xFFFFu) / 16.0f) - 2048.0f) / 256.0f;
        o.y = -((static_cast<float>((xyz >> 16) & 0xFFFFu) / 16.0f) - 2048.0f) / 224.0f;
        o.nz = 1.0f;
        o.r = static_cast<uint8_t>(a.rgbaq);
        o.g = static_cast<uint8_t>(a.rgbaq >> 8);
        o.b = static_cast<uint8_t>(a.rgbaq >> 16);
        o.a = static_cast<uint8_t>(a.rgbaq >> 24);
        if (((a.alpha >> 4) & 3u) == 2u) // C = FIX
            o.a = static_cast<uint8_t>(std::min<uint64_t>((a.alpha >> 32) & 0xFFu, 0x80u));
        // kScreenCopy: 0x10f37c8's 256 x 256 copy of the screen.
        const uint32_t format = a.header == kScreenCopy ? 0x00880000u : (a.header ? word(a.header + 0x1Cu) : 0u);
        if ((a.prim & 0x100u) && a.header == kPreviousFrame) // the display buffer, 512 x 448
        {
            o.u = (static_cast<float>(a.u) / 16.0f) / 512.0f;
            o.v = (static_cast<float>(a.v) / 16.0f) / 448.0f;
        }
        else if (a.prim & 0x100u) // FST: UV in texels, 12.4
        {
            o.u = (static_cast<float>(a.u) / 16.0f) / static_cast<float>(1u << ((format >> 16) & 0xFu));
            o.v = (static_cast<float>(a.v) / 16.0f) / static_cast<float>(1u << ((format >> 20) & 0xFu));
        }
        else
        {
            // ST / Q (RGBAQ's high word; the sprites give 1).
            float q;
            const uint32_t qBits = static_cast<uint32_t>(a.rgbaq >> 32);
            std::memcpy(&q, &qBits, 4);
            const float k = (q != 0.0f && std::isfinite(q)) ? 1.0f / q : 1.0f;
            o.u = a.s * k;
            o.v = a.t * k;
        }
        std::vector<native_renderer::Vertex> &v = a.strip;
        v.push_back(o);
        if (!kick)
            return;
        const size_t n = v.size();
        switch (a.prim & 7u)
        {
        case 1: case 2: // line, line strip: one pixel wide
            if (n >= 2)
            {
                native_renderer::Vertex p0 = v[n - 2], p1 = v[n - 1];
                float dx = (p1.x - p0.x) * 256.0f, dy = (p1.y - p0.y) * 224.0f;
                const float len = std::sqrt(dx * dx + dy * dy);
                if (len > 0.0f)
                {
                    const float nx = -dy / len * 0.5f / 256.0f, ny = dx / len * 0.5f / 224.0f;
                    native_renderer::Vertex q[4] = {p0, p0, p1, p1};
                    q[0].x += nx; q[0].y += ny; q[1].x -= nx; q[1].y -= ny;
                    q[2].x += nx; q[2].y += ny; q[3].x -= nx; q[3].y -= ny;
                    emit2D(q[0], q[1], q[2]);
                    emit2D(q[1], q[3], q[2]);
                }
                if ((a.prim & 7u) == 1u)
                    v.clear();
            }
            break;
        case 3: // triangles
            if (n == 3)
            {
                emit2D(v[0], v[1], v[2]);
                v.clear();
            }
            break;
        case 4: // triangle strip
            if (n >= 3)
                emit2D(v[n - 3], v[n - 2], v[n - 1]);
            break;
        case 5: // triangle fan
            if (n >= 3)
                emit2D(v[0], v[n - 2], v[n - 1]);
            break;
        case 6: // sprite: two opposite corners
            if (n == 2)
            {
                native_renderer::Vertex q[4] = {v[0], v[1], v[1], v[1]};
                q[1].y = v[0].y; q[1].v = v[0].v; // (x1, y0)
                q[2].x = v[0].x; q[2].u = v[0].u; // (x0, y1)
                emit2D(q[0], q[1], q[2]);
                emit2D(q[1], q[3], q[2]);
                v.clear();
            }
            break;
        default:
            v.clear();
        }
    }

    // SOTC_GSREG_CENSUS=1: gsReg calls by return address, every 120 frames
    // (resolve with recomp/game/symbols.txt): who draws 2D through gsReg.
    void gsRegCensus(uint32_t ra)
    {
        static const bool on = std::getenv("SOTC_GSREG_CENSUS") != nullptr;
        if (!on)
            return;
        static std::unordered_map<uint32_t, uint32_t> calls;
        static uint64_t since = 0;
        ++calls[ra];
        if (g_frame < since + 120u)
            return;
        std::vector<std::pair<uint32_t, uint32_t>> top(calls.begin(), calls.end());
        std::sort(top.begin(), top.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
        std::string line = "[gsreg] frame=" + std::to_string(g_frame);
        char item[32];
        for (size_t i = 0; i < top.size() && i < 200u; ++i)
        {
            std::snprintf(item, sizeof(item), " %08x:%u", top[i].first, top[i].second);
            line += item;
        }
        std::fprintf(stderr, "%s\n", line.c_str());
        calls.clear();
        since = g_frame;
    }

    // One GS register write of a 2D draw, as gsReg or an A+D packet gives it.
    void feed2D(uint32_t reg, uint64_t value)
    {
        Assembler &a = g_2d;
        a.layer = g_layers.empty() ? 25u : g_layers.back();
        a.header = g_requested;
        switch (reg)
        {
        case 0x00u: a.prim = value; a.strip.clear(); break;
        case 0x01u: a.rgbaq = value; break;
        case 0x02u:
            std::memcpy(&a.s, &value, 4);
            std::memcpy(&a.t, reinterpret_cast<const uint8_t *>(&value) + 4, 4);
            break;
        case 0x03u: a.u = value & 0x3FFFu; a.v = (value >> 16) & 0x3FFFu; break;
        case 0x04u: case 0x05u: vertex2D(value, true); break;   // XYZF2, XYZ2
        case 0x0Cu: case 0x0Du: vertex2D(value, false); break;  // XYZF3, XYZ3: no drawing kick
        case 0x42u: a.alpha = value; break;
        }
    }

    // The gl* immediate API (glBeginWithDlLayerNo .. glEnd) builds a GIF
    // packet on the CPU: at *0x1186398 a DMA tag, a GIF tag (+0x10, A+D),
    // then (value, register) pairs up to *0x1186394: TEST, PRIM, and per
    // vertex RGBAQ, ST or UV, XYZ2 already in screen space. glEnd chains it
    // in the layer glBegin pushed; here the pairs go through the 2D path.
    constexpr uint32_t kGlEnd = 0x0108D060u;
    PS2Runtime::RecompiledFunction g_glEnd = nullptr;

    void glEndHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        static const bool off = std::getenv("SOTC_NATIVE_NOGL") != nullptr;
        const uint32_t start = word(0x01186398u), end = word(0x01186394u);
        // Not drawn here: bossDeathEffectBegin's fan goes to an off-screen buffer.
        const uint32_t ra = GPR_U32(ctx, 31);
        const bool offScreen = ra >= 0x01085640u && ra < 0x01085E40u;
        // 0x10f3fb0's strips (the heat haze behind the bird) sample the copy
        // of the screen 0x10f37c8 made in their layer (UV in its 256 x 256 texels).
        const bool haze = ra >= 0x010F3FB0u && ra < 0x010F4890u;
        const uint32_t requested = g_requested;
        if (haze)
            g_requested = kScreenCopy;
        if (!off && !offScreen && start && end > start + 0x20u && end - start < 0x100000u && word(0x0118B5B0u) == 0u)
        {
            static const bool log = std::getenv("SOTC_NATIVE_LOG") != nullptr;
            if (log)
            {
                static std::set<uint64_t> seen;
                const uint32_t layer = g_layers.empty() ? 25u : g_layers.back();
                if (seen.insert((static_cast<uint64_t>(GPR_U32(ctx, 31)) << 8) | layer).second)
                    std::fprintf(stderr, "[gl] ra %08x layer %u qwords %u\n", GPR_U32(ctx, 31), layer, (end - start) / 16u - 2u);
            }
            for (uint32_t q = start + 0x20u; q + 16u <= end; q += 16u)
            {
                uint64_t value;
                std::memcpy(&value, g_ram + (q & 0x1FFFFF0u), 8);
                feed2D(word(q + 8u) & 0xFFu, value);
            }
        }
        g_requested = requested;
        g_glEnd(rdram, ctx, runtime);
    }

    void gsRegHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31);
        gsRegCensus(ra);
        // SOTC_LOG_GSREG=<reg>: who writes that register, in which layer, with what (debugging).
        static const int logReg = std::getenv("SOTC_LOG_GSREG") ? static_cast<int>(std::strtol(std::getenv("SOTC_LOG_GSREG"), nullptr, 0)) : -1;
        if (logReg >= 0 && (GPR_U32(ctx, 4) & 0xFFu) == static_cast<uint32_t>(logReg))
        {
            static std::set<std::pair<uint32_t, uint64_t>> seen;
            if (seen.insert({ra, GPR_U64(ctx, 5)}).second && seen.size() < 300)
                std::fprintf(stderr, "[gsreg-log] ra %08x layer %u value %016llx\n", ra, g_layers.empty() ? 99u : g_layers.back(),
                             static_cast<unsigned long long>(GPR_U64(ctx, 5)));
        }
        if (ra >= 0x0109E2A0u && ra < 0x0109FB08u)
            g_bloomRan = true;
        if (ra >= 0x01089BF8u && ra < 0x0108A0E0u)
        {
            // feedbackBlur / feedbackBlur2 (layer 29): the display buffer, the
            // last frame shown, laid over the new one (FIX or vertex alpha).
            syncFrame(rdram);
            if (!native_renderer::tuning().feedback)
            {
                g_gsReg(rdram, ctx, runtime);
                return;
            }
            const uint32_t requested = g_requested;
            g_requested = kPreviousFrame;
            feed2D(GPR_U32(ctx, 4) & 0xFFu, GPR_U64(ctx, 5));
            g_requested = requested;
            g_gsReg(rdram, ctx, runtime);
            return;
        }
        if (ra >= 0x0109C610u && ra < 0x0109CD28u)
        {
            // refractionEnd under water (0x109c610): a grid of strips over the
            // screen at layer 19, its ST waving over refractionBegin's 256 x 256
            // copy of the picture so far.
            syncFrame(rdram);
            const uint32_t requested = g_requested;
            g_requested = kScreenCopy;
            feed2D(GPR_U32(ctx, 4) & 0xFFu, GPR_U64(ctx, 5));
            g_requested = requested;
            g_gsReg(rdram, ctx, runtime);
            return;
        }
        for (const Range &r : k2D)
        {
            if (ra < r.start || ra >= r.end)
                continue;
            syncFrame(rdram);
            feed2D(GPR_U32(ctx, 4) & 0xFFu, GPR_U64(ctx, 5));
            break;
        }
        g_gsReg(rdram, ctx, runtime);
    }

    void texTransRequestHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_ram = rdram;
        const uint32_t resource = GPR_U32(ctx, 4);
        g_requested = resource ? word(resource + 0x20u) : 0u;
        g_texTransRequest(rdram, ctx, runtime);
    }

    void pushLayerHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // SOTC_LOG_LAYER=<n> (debugging): who pushes display list layer n (caller, count).
        static const int logLayer = std::getenv("SOTC_LOG_LAYER") ? std::atoi(std::getenv("SOTC_LOG_LAYER")) : -1;
        if (logLayer >= 0 && GPR_U32(ctx, 4) == static_cast<uint32_t>(logLayer))
        {
            static std::map<uint32_t, uint64_t> callers;
            const uint64_t n = ++callers[GPR_U32(ctx, 31)];
            if (n == 1u || n % 600u == 0u)
                std::fprintf(stderr, "[layer %d] pushed from %08x (%llu)\n", logLayer, GPR_U32(ctx, 31), static_cast<unsigned long long>(n));
            if (GPR_U32(ctx, 31) == 0x010860F8u && (n < 40u || n % 60u == 0u))
                std::fprintf(stderr, "[bossfx] frame %u flags %x radius %g rounds %d tint %08x %08x %08x sharp %08x blur %08x clear %08x %08x %08x %08x fade %08x %08x %08x %08x\n", g_frame,
                             word(0x01186290u), real(0x01186298u), static_cast<int>(word(0x011862A0u)), word(0x011862B0u), word(0x011862B4u), word(0x011862B8u), word(0x011862C4u), word(0x011862C0u),
                             word(0x01186280u), word(0x01186284u), word(0x01186288u), word(0x0118628Cu), word(0x01186260u), word(0x01186264u), word(0x01186268u), word(0x0118626Cu));
        }
        // Testing: refractionEnd's underwater waves on any scene (the flag
        // getFogUnderWater reads, set just before refractionEnd tests it).
        static const bool underwater = std::getenv("SOTC_FORCE_UNDERWATER") != nullptr;
        if (underwater && GPR_U32(ctx, 31) == 0x0109D018u)
        {
            const uint32_t one = 1u;
            std::memcpy(rdram + 0x1186328u, &one, 4);
        }
        // Testing: feedbackBlur2's trail at this alpha (graphFrameFinish reads
        // 0x118b740 after refractionEnd).
        static const int feedback = std::getenv("SOTC_FORCE_FEEDBACK") ? std::atoi(std::getenv("SOTC_FORCE_FEEDBACK")) : 0;
        if (feedback && GPR_U32(ctx, 31) == 0x0109D018u)
            std::memcpy(rdram + 0x118B740u, &feedback, 4);
        // feedbackBlur2 sets no ALPHA: its sprite blends by the layer's root
        // context, ALPHA 0x60 ((Cs - Cs) x FIX + Cd: no change), as the GS shows.
        if (GPR_U32(ctx, 31) >= 0x01089E50u && GPR_U32(ctx, 31) < 0x0108A0E0u)
            g_2d.alpha = 0x60u;
        // Testing: bossDeathEffectBegin's fan (bit 0 of 0x1186290) at this radius.
        static const float fan = std::getenv("SOTC_FORCE_BOSSFAN") ? static_cast<float>(std::atof(std::getenv("SOTC_FORCE_BOSSFAN"))) : 0.0f;
        if (fan > 0.0f && GPR_U32(ctx, 31) == 0x01085910u)
        {
            uint32_t flags;
            std::memcpy(&flags, rdram + 0x1186290u, 4);
            flags |= 1u;
            std::memcpy(rdram + 0x1186290u, &flags, 4);
            std::memcpy(rdram + 0x1186298u, &fan, 4);
            const float none = 0.0f;
            std::memcpy(rdram + 0x118629Cu, &none, 4);
        }
        g_layers.push_back(GPR_U32(ctx, 4));
        g_pushLayer(rdram, ctx, runtime);
    }

    void popLayerHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!g_layers.empty())
            g_layers.pop_back();
        g_popLayer(rdram, ctx, runtime);
    }

    // ---- Cloth and hair: meshDisp -------------------------------------------
    //
    // meshCreate builds a grid of rows x columns simulated on the CPU (two
    // buffers, the current one at +0x10 of the "#" object's +0x20 block:
    // [0]/[1] buffers, [2] rows, [3] columns, [4] current). A buffer holds
    // the view matrix (qwords 4-7), then per row an UNPACK and `columns`
    // vertices of 4 qwords: world position, normal, ST(Q), colour (floats,
    // 0x80 = 1); triangle strips join each row to the next. meshDisp (0x109ab80)
    // chains the current buffer; the caller has asked for the texture.
    // Lights in camera space: those of the last skinned part drawn (the
    // character the cloth hangs from).
    float g_viewLights[32];
    bool g_haveViewLights = false;
    constexpr uint32_t kMeshDisp = 0x0109AB80u;
    PS2Runtime::RecompiledFunction g_meshDisp = nullptr;

    void meshDispHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        const uint32_t object = GPR_U32(ctx, 4);
        const uint32_t header = g_requested;
        const uint32_t layer = g_layers.empty() ? 2u : g_layers.back();
        g_meshDisp(rdram, ctx, runtime);
        if (!g_cameraContext || g_ram[(object + 8u) & 0x1FFFFFFu] != '#')
            return;
        const uint32_t base = object + 0x20u;
        const uint32_t rows = word(base + 8u), columns = word(base + 12u);
        const uint32_t buffer = word(base + (word(base + 16u) & 1u) * 4u);
        if (rows < 2u || rows > 64u || columns < 2u || columns > 64u || !buffer)
            return;
        float view[16]; // the buffer's view matrix: normals to camera space
        for (int i = 0; i < 16; ++i)
            view[i] = real(buffer + 0x40u + i * 4u);
        auto vertexAt = [&](uint32_t r, uint32_t c)
        {
            const uint32_t q = buffer + 0xB0u + r * (columns * 0x40u + 0x10u) + c * 0x40u;
            native_renderer::Vertex v{};
            v.x = real(q);
            v.y = real(q + 4u);
            v.z = real(q + 8u);
            const float nx = real(q + 16u), ny = real(q + 20u), nz = real(q + 24u);
            v.nx = view[0] * nx + view[4] * ny + view[8] * nz;
            v.ny = view[1] * nx + view[5] * ny + view[9] * nz;
            v.nz = view[2] * nx + view[6] * ny + view[10] * nz;
            v.u = real(q + 32u);
            v.v = real(q + 36u);
            v.r = static_cast<uint8_t>(std::clamp(real(q + 48u), 0.0f, 255.0f));
            v.g = static_cast<uint8_t>(std::clamp(real(q + 52u), 0.0f, 255.0f));
            v.b = static_cast<uint8_t>(std::clamp(real(q + 56u), 0.0f, 255.0f));
            v.a = static_cast<uint8_t>(std::clamp(real(q + 60u), 0.0f, 255.0f));
            return v;
        };
        static std::vector<native_renderer::Vertex> tris;
        tris.clear();
        for (uint32_t r = 0; r + 1u < rows; ++r)
            for (uint32_t c = 0; c + 1u < columns; ++c)
            {
                const native_renderer::Vertex a = vertexAt(r, c), b = vertexAt(r, c + 1u), d = vertexAt(r + 1u, c), e = vertexAt(r + 1u, c + 1u);
                tris.insert(tris.end(), {a, b, d, b, e, d});
            }
        const uint64_t mesh = (1ull << 60) | g_skinnedDraws++;
        native_renderer::uploadMesh(mesh, tris.data(), static_cast<uint32_t>(tris.size()), true);
        float camera[16];
        for (int i = 0; i < 16; ++i)
            camera[i] = real(g_cameraContext + 0x5A0u + i * 4u);
        native_renderer::DrawState state;
        state.texture = texture(header);
        state.order = layer;
        state.fogged = layer == 2u || layer == 3u;
        if (g_haveViewLights)
        {
            state.lit = true;
            std::memcpy(state.light, g_viewLights, sizeof(state.light));
        }
        native_renderer::draw(mesh, camera, state);
        ++g_frameDraws;
    }

    // ---- Shadows: shadowDisp -------------------------------------------------
    constexpr uint32_t kShadowDisp = 0x0109E0E8u;
    PS2Runtime::RecompiledFunction g_shadowDisp = nullptr;

    // shadowDisp(instance chain, shadow model) (0x109e0e8): microcode 8 takes
    // the shadow model's triangles (V3-32 strips, the REF at model + 0x1c)
    // to camera space by the instance's matrix (its chain's RET packet),
    // extrudes each vertex by the vector in the w of the instance's light
    // rows (VU 984-986), and draws each triangle's prism, its faces counted
    // up or down by their facing on a buffer the game then darkens
    // (shadowDraw; ALPHA 0x64 FIX 0x40: half). Skinned shadow models
    // (shadowDisp's t2 set: microcode 5 loads the instance's bone palette,
    // microcode 9 draws) carry a second qword per vertex: x/y/z = bone index
    // + weight (the fraction), w = n; the vertex is the sum over the first
    // n + 1 components of weight x (palette matrix index, camera space).
    std::vector<native_renderer::Vertex> g_volume;
    constexpr float kShadowNear = 0.1f;

    void shadowDispHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        const uint32_t chain = GPR_U32(ctx, 4), model = GPR_U32(ctx, 5);
        const bool skinnedShadow = GPR_U32(ctx, 10) != 0u;
        g_shadowDisp(rdram, ctx, runtime);
        const bool shadows = native_renderer::tuning().shadows;
        static const bool idColours = std::getenv("SOTC_NATIVE_IDCOLOR") != nullptr;
        if (!shadows || idColours || !model || !g_cameraContext || ((word(chain) >> 28) & 7u) != 2u)
            return;
        const uint32_t ret = word(chain + 4u);
        static std::vector<float> palette;
        float m[16];
        if (skinnedShadow)
        {
            if (!readChainPalette(chain, palette))
                return;
        }
        else
        {
            if ((word(ret) & 0xFFFFu) != 5u) // one matrix: rigid
                return;
            for (int i = 0; i < 16; ++i)
                m[i] = real(ret + 16u + i * 4u);
        }
        const float d[3] = {real(chain + 16u + 12u), real(chain + 32u + 12u), real(chain + 48u + 12u)};
        const uint32_t tag = word(model + 0x1Cu);
        static std::vector<Strip> strips;
        strips.clear();
        readBatches(word(tag + 4u), (word(tag) & 0xFFFFu) * 16u, strips);
        auto toCamera = [&](const BatchVertex &v, float out[3])
        {
            if (!skinnedShadow)
            {
                for (int r = 0; r < 3; ++r)
                    out[r] = m[r] * v.x + m[4 + r] * v.y + m[8 + r] * v.z + m[12 + r];
                return;
            }
            out[0] = out[1] = out[2] = 0.0f;
            const uint32_t n = std::min(v.skin[3] & 0xFFFFu, 3u);
            for (uint32_t k = 0; k <= n; ++k)
            {
                float packed;
                std::memcpy(&packed, &v.skin[k], 4);
                const float index = std::trunc(packed);
                const float *b = &palette[(static_cast<uint32_t>(static_cast<int32_t>(index)) * 4u % 1024u) * 4u];
                const float w = packed - index;
                for (int r = 0; r < 3; ++r)
                    out[r] += w * (b[r] * v.x + b[4 + r] * v.y + b[8 + r] * v.z + b[12 + r]);
            }
        };
        auto put = [&](const float *p)
        {
            native_renderer::Vertex v{};
            v.x = p[0];
            v.y = p[1];
            v.z = p[2];
            v.r = v.g = v.b = v.a = 0x80; // the shared fragment stage drops alpha 0
            g_volume.push_back(v);
        };
        g_volume.clear();
        for (const Strip &strip : strips)
            for (size_t i = 2; i < strip.size(); ++i)
            {
                float a[3], b[3], c[3];
                toCamera(strip[i - 2], a);
                toCamera(strip[i - 1], b);
                toCamera(strip[i], c);
                const float u[3] = {b[0] - a[0], b[1] - a[1], b[2] - a[2]}, w[3] = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};
                const float n[3] = {u[1] * w[2] - u[2] * w[1], u[2] * w[0] - u[0] * w[2], u[0] * w[1] - u[1] * w[0]};
                const float facing = n[0] * d[0] + n[1] * d[1] + n[2] * d[2];
                if (facing == 0.0f)
                    continue; // degenerate (strip joins) or edge-on
                if (facing > 0.0f)
                    std::swap(b, c); // the near face looks away from the extrusion
                // Each extrusion edge cut at z = 0.1 as microcode 8 does, so
                // the far cap stays in front of the camera; a triangle with
                // an edge wholly behind it is dropped.
                float *near[3] = {a, b, c};
                float far[3][3];
                bool behind = false;
                for (int k = 0; k < 3; ++k)
                {
                    for (int r = 0; r < 3; ++r)
                        far[k][r] = near[k][r] + d[r];
                    const float zn = near[k][2], zf = far[k][2];
                    if (zn <= kShadowNear && zf <= kShadowNear)
                        behind = true;
                    else if (zf < kShadowNear || zn < kShadowNear)
                    {
                        float *out = zf < kShadowNear ? far[k] : near[k];
                        const float *in = zf < kShadowNear ? near[k] : far[k];
                        const float t = (in[2] - kShadowNear) / (in[2] - out[2]);
                        for (int r = 0; r < 3; ++r)
                            out[r] = in[r] + (out[r] - in[r]) * t;
                    }
                }
                if (behind)
                    continue;
                // Closed prism, faces outwards: near (a, b, c), far reversed, sides.
                put(a), put(b), put(c);
                put(far[0]), put(far[2]), put(far[1]);
                for (int k = 0; k < 3; ++k)
                {
                    const int l = (k + 1) % 3;
                    put(near[k]), put(far[k]), put(far[l]);
                    put(near[k]), put(far[l]), put(near[l]);
                }
            }
        if (g_volume.empty())
            return;
        float projection[16];
        for (int i = 0; i < 16; ++i)
            projection[i] = real(g_cameraContext + 0x40u + i * 4u);
        const uint64_t mesh = (1ull << 59) | g_skinnedDraws++;
        native_renderer::uploadMesh(mesh, g_volume.data(), static_cast<uint32_t>(g_volume.size()), true);
        native_renderer::DrawState state;
        if (std::getenv("SOTC_SHADOW_SHOW")) // debugging: the volumes in red
        {
            state.debugColour = 0x0000FFu;
            state.order = 15;
            native_renderer::draw(mesh, projection, state);
            return;
        }
        state.shadowVolume = true;
        static const float amount = std::getenv("SOTC_SHADOW_AMOUNT") ? std::strtof(std::getenv("SOTC_SHADOW_AMOUNT"), nullptr) : 0.5f;
        state.shadow = amount;
        state.order = 7; // after the opaque world, before the transparent layers
        native_renderer::draw(mesh, projection, state);
    }

    // ---- Particles (ptclman*) ------------------------------------------------
    constexpr uint32_t kPtclEnv = 0x0109A280u, kPtclMat = 0x01099E00u, kPtclVtx = 0x01099F38u;
    PS2Runtime::RecompiledFunction g_ptclEnv = nullptr, g_ptclMat = nullptr, g_ptclVtx = nullptr;

    // The particle microcode (VU1 microcode 7) expands each particle (48
    // bytes: position + size, colour 0..1, sin/cos of its rotation) into a
    // rotated square: s = size x 0.7071, A = (k.x sin, k.y cos) s,
    // B = (-cos, k.y sin) s with k = VU23 (0x118bfe0); type 1 orients A/B by
    // the billboard axes (VU 0..2 from *0x118bff4, transposed, x k), types
    // 2/4/5 by the environment matrix (ptclmanBuildEnvPacket's); corners
    // p+B, p+A, p-A, p-B with ST (0,0) (vec.x,0) (0,vec.y) (vec.x,vec.y),
    // vec = the env packet's (VU24). Type 4 is a fan around p (a lit puff).
    // Colour x255, alpha x128 (0x80 = 1). The blend is the material
    // packet's: 1 adds, 2 subtracts, else blends.
    struct ParticleEnv
    {
        uint32_t type = 0;
        float m[16];
        float vec[2] = {1, 1};
        float k[3] = {1, 1, 1};
        float axes[3][3];
        float fade = 1.0f;
        float light[4] = {0, 0, 0, 0}, shade[4] = {0.75f, 0, 0, 0}; // VU 26 / 27 (type 4's lighting)
    } g_ptclEnvState;
    uint32_t g_ptclBlend = 0;

    void ptclMatHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_ptclBlend = GPR_U32(ctx, 4);
        g_ptclMat(rdram, ctx, runtime);
    }

    void ptclEnvHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        ParticleEnv &e = g_ptclEnvState;
        const uint32_t vec = GPR_U32(ctx, 4), mat = GPR_U32(ctx, 6);
        e.type = GPR_U32(ctx, 5);
        e.fade = ctx->f[12];
        for (int i = 0; i < 16; ++i)
            e.m[i] = real(mat + i * 4u);
        e.vec[0] = real(vec);
        e.vec[1] = real(vec + 4u);
        const uint32_t lightAt = GPR_U32(ctx, 7), shadeAt = GPR_U32(ctx, 8);
        for (int i = 0; i < 4; ++i)
        {
            if (lightAt)
                e.light[i] = real(lightAt + i * 4u);
            if (shadeAt)
                e.shade[i] = real(shadeAt + i * 4u);
        }
        for (int i = 0; i < 3; ++i)
            e.k[i] = real(0x118BFE0u + i * 4u);
        const uint32_t consts = word(0x118BFF4u);
        // vf9..11: row i of the constants' first three qwords, transposed, times k.
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                e.axes[c][r] = consts ? e.k[c] * real(consts + r * 16u + c * 4u) : 0.0f;
        g_ptclEnv(rdram, ctx, runtime);
    }

    void ptclVtxHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        const uint32_t a = GPR_U32(ctx, 4), count = GPR_U32(ctx, 5);
        const uint32_t header = g_requested, layer = g_layers.empty() ? 18u : g_layers.back();
        g_ptclVtx(rdram, ctx, runtime);
        const bool off = !native_renderer::tuning().particles;
        const ParticleEnv &e = g_ptclEnvState;
        if (off || !g_cameraContext || count == 0u || count > 4096u || (!(e.type >= 1u && e.type <= 5u) && e.type != 7u))
            return;
        static std::vector<native_renderer::Vertex> tris;
        tris.clear();
        auto mul3 = [&](const float *dir, float x, float y, float out[3])
        {
            for (int r = 0; r < 3; ++r)
                out[r] = dir == nullptr ? 0.0f : 0.0f;
            if (e.type == 1u)
                for (int r = 0; r < 3; ++r)
                    out[r] = e.axes[0][r] * x + e.axes[1][r] * y;
            else
                for (int r = 0; r < 3; ++r)
                    out[r] = e.m[r] * x + e.m[4 + r] * y + ((e.type == 2u || e.type == 4u || e.type == 7u) ? e.m[12 + r] : 0.0f); // types 2, 4, 7 add the matrix's translation row (5 is a streak)
        };
        // Type 3 (streaks) side vectors: (p - eye) x (p1 - p2), half the width
        // long; the eye is VU 16, the 17th qword of the constants at *0x118bff4.
        const uint32_t consts = word(0x118BFF4u) & 0x0FFFFFFFu;
        const float eye[3] = {consts ? real(consts + 0x100u) : 0.0f, consts ? real(consts + 0x104u) : 0.0f, consts ? real(consts + 0x108u) : 0.0f};
        auto side = [&](const float *p, const float *d, float half, float out[3])
        {
            const float r[3] = {p[0] - eye[0], p[1] - eye[1], p[2] - eye[2]};
            out[0] = r[1] * d[2] - r[2] * d[1];
            out[1] = r[2] * d[0] - r[0] * d[2];
            out[2] = r[0] * d[1] - r[1] * d[0];
            const float len = std::sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
            const float k = len > 0.0f ? half / len : 0.0f;
            for (int i = 0; i < 3; ++i)
                out[i] *= k;
        };
        for (uint32_t i = 0; i < count; ++i)
        {
            const uint32_t q = a + i * 48u;
            const float p[3] = {real(q), real(q + 4u), real(q + 8u)};
            const float sz = real(q + 12u) * 0.70710677f;
            const float sn = real(q + 32u), cs = real(q + 36u);
            float A[3], B[3];
            mul3(A, e.k[0] * sn * sz, e.k[1] * cs * sz, A);
            mul3(B, -cs * sz, e.k[1] * sn * sz, B);
            native_renderer::Vertex v{};
            v.nz = 1.0f;
            auto col = [&](native_renderer::Vertex &o, float scale)
            {
                o.r = static_cast<uint8_t>(std::clamp(real(q + 16u) * 255.0f * scale, 0.0f, 255.0f));
                o.g = static_cast<uint8_t>(std::clamp(real(q + 20u) * 255.0f * scale, 0.0f, 255.0f));
                o.b = static_cast<uint8_t>(std::clamp(real(q + 24u) * 255.0f * scale, 0.0f, 255.0f));
                o.a = static_cast<uint8_t>(std::clamp(real(q + 28u) * 128.0f, 0.0f, 255.0f));
            };
            auto at = [&](const float *o, float sign, float u, float vv)
            {
                native_renderer::Vertex c = v;
                c.x = p[0] + sign * o[0];
                c.y = p[1] + sign * o[1];
                c.z = p[2] + sign * o[2];
                c.u = u;
                c.v = vv;
                col(c, 1.0f);
                return c;
            };
            if (e.type == 3u || e.type == 5u)
            {
                // A streak from p (q0, width in w) to p2 (q2): corners p +- a, p2 +- b.
                // Type 5 (microcode 7 at 0xba8) is the same streak drawn as type
                // 4's lit fan (routine 0xe48) around the corners' mean.
                const float p2[3] = {real(q + 32u), real(q + 36u), real(q + 40u)};
                const float d[3] = {p[0] - p2[0], p[1] - p2[1], p[2] - p2[2]};
                const float half = real(q + 12u) * 0.5f;
                float sa[3], sb[3];
                side(p, d, half, sa);
                side(p2, d, half, sb);
                auto corner = [&](const float *base, const float *o, float sign, float u, float vv)
                {
                    native_renderer::Vertex c = v;
                    c.x = base[0] + sign * o[0];
                    c.y = base[1] + sign * o[1];
                    c.z = base[2] + sign * o[2];
                    c.u = u;
                    c.v = vv;
                    col(c, 1.0f);
                    return c;
                };
                native_renderer::Vertex c16 = corner(p, sa, 1, 0, 0), c17 = corner(p, sa, -1, e.vec[0], 0), c18 = corner(p2, sb, 1, 0, e.vec[1]),
                                        c19 = corner(p2, sb, -1, e.vec[0], e.vec[1]);
                if (e.type == 3u)
                {
                    tris.insert(tris.end(), {c16, c17, c18, c17, c19, c18});
                    continue;
                }
                native_renderer::Vertex c = v;
                c.x = (c16.x + c17.x + c18.x + c19.x) * 0.25f;
                c.y = (c16.y + c17.y + c18.y + c19.y) * 0.25f;
                c.z = (c16.z + c17.z + c18.z + c19.z) * 0.25f;
                c.u = e.vec[0] * 0.5f;
                c.v = e.vec[1] * 0.5f;
                auto shade = [&](native_renderer::Vertex &o, const float *n)
                {
                    const float dd = n[0] * e.light[0] + n[1] * e.light[1] + e.shade[3] * n[2] * e.light[2] + e.shade[3];
                    float t = std::min(dd * 0.5f, 1.0f);
                    t *= t;
                    col(o, t * e.shade[1] + e.shade[0]);
                };
                shade(c, e.axes[2]);
                for (native_renderer::Vertex *o : {&c16, &c17, &c18, &c19})
                {
                    float n[3] = {c.x - o->x, c.y - o->y, c.z - o->z};
                    const float len = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
                    for (int k = 0; k < 3; ++k)
                        n[k] = len > 0.0f ? n[k] / len : 0.0f;
                    shade(*o, n);
                }
                tris.insert(tris.end(), {c, c16, c17, c, c17, c19, c, c19, c18, c, c18, c16});
                continue;
            }
            // Type 7: each particle's own texture cell offset (q2 z / w).
            const float u0 = e.type == 7u ? real(q + 40u) : 0.0f, v0 = e.type == 7u ? real(q + 44u) : 0.0f;
            const native_renderer::Vertex v16 = at(B, 1, u0, v0), v17 = at(A, 1, u0 + e.vec[0], v0), v18 = at(A, -1, u0, v0 + e.vec[1]),
                                          v19 = at(B, -1, u0 + e.vec[0], v0 + e.vec[1]);
            if (e.type == 4u)
            {
                // A lit fan (routine 0xe48): the centre's normal is the camera's
                // third axis, a corner's the way to the centre; with L = VU 26
                // and k = VU 27: s = min((n.x L.x + n.y L.y + k.w n.z L.z + k.w) / 2,
                // 1)^2, the colour times s k.y + k.x.
                auto lit = [&](native_renderer::Vertex &o, const float *n)
                {
                    const float d = n[0] * e.light[0] + n[1] * e.light[1] + e.shade[3] * n[2] * e.light[2] + e.shade[3];
                    float t = std::min(d * 0.5f, 1.0f);
                    t *= t;
                    col(o, t * e.shade[1] + e.shade[0]);
                };
                auto towardCentre = [&](const native_renderer::Vertex &o, float n[3])
                {
                    n[0] = p[0] - o.x;
                    n[1] = p[1] - o.y;
                    n[2] = p[2] - o.z;
                    const float len = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
                    for (int k = 0; k < 3; ++k)
                        n[k] = len > 0.0f ? n[k] / len : 0.0f;
                };
                native_renderer::Vertex c = v, f16 = v16, f17 = v17, f18 = v18, f19 = v19;
                c.x = p[0];
                c.y = p[1];
                c.z = p[2];
                c.u = e.vec[0] * 0.5f;
                c.v = e.vec[1] * 0.5f;
                lit(c, e.axes[2]);
                float n[3];
                for (native_renderer::Vertex *o : {&f16, &f17, &f18, &f19})
                {
                    towardCentre(*o, n);
                    lit(*o, n);
                }
                tris.insert(tris.end(), {c, f16, f17, c, f17, f19, c, f19, f18, c, f18, f16});
            }
            else
                tris.insert(tris.end(), {v16, v17, v18, v17, v19, v18});
        }
        if (tris.empty())
            return;
        float camera[16];
        for (int i = 0; i < 16; ++i)
            camera[i] = real(g_cameraContext + 0x5A0u + i * 4u);
        const uint64_t mesh = (1ull << 58) | g_skinnedDraws++;
        native_renderer::uploadMesh(mesh, tris.data(), static_cast<uint32_t>(tris.size()), true);
        native_renderer::DrawState state;
        state.texture = texture(header);
        state.blend = g_ptclBlend == 1u ? native_renderer::Blend::Additive : (g_ptclBlend == 2u ? native_renderer::Blend::Subtract : native_renderer::Blend::Normal);
        state.order = layer;
        state.fogged = false;
        native_renderer::draw(mesh, camera, state);
        ++g_frameDraws;
    }

    // ---- screenBlur (0x108a780) -------------------------------------------
    constexpr uint32_t kScreenBlur = 0x0108A780u;
    PS2Runtime::RecompiledFunction g_screenBlur = nullptr;
    void screenBlurHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        static int n = 0;
        if (std::getenv("SOTC_NATIVE_LOG") && n++ < 30)
            std::fprintf(stderr, "[sblur] frame %llu angle %g scale %g layer %u w %d a1 %d steps %d a3 %d t0 %d\n", (unsigned long long)g_frame, ctx->f[12], ctx->f[13],
                         GPR_U32(ctx, 4), static_cast<int32_t>(GPR_U32(ctx, 5)), static_cast<int32_t>(GPR_U32(ctx, 6)), static_cast<int32_t>(GPR_U32(ctx, 7)),
                         static_cast<int32_t>(GPR_U32(ctx, 8)), static_cast<int32_t>(GPR_U32(ctx, 9)));
        native_renderer::ScreenBlur b;
        b.angle = ctx->f[12];
        b.scale = ctx->f[13];
        b.order = GPR_U32(ctx, 4);
        b.steps = static_cast<int32_t>(GPR_U32(ctx, 7));
        b.dx = static_cast<float>(static_cast<int32_t>(GPR_U32(ctx, 8)));
        b.dy = static_cast<float>(static_cast<int32_t>(GPR_U32(ctx, 9)));
        if (native_renderer::tuning().screenBlur && b.steps > 0 && b.steps < 64)
            native_renderer::addScreenBlur(b);
        g_screenBlur(rdram, ctx, runtime);
    }

    // A 4 x 4 inverse (column-major); false when singular.
    bool invert4(const float *m, float *out)
    {
        float inv[16];
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        const float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        if (det == 0.0f || !std::isfinite(det))
            return false;
        for (int i = 0; i < 16; ++i)
            out[i] = inv[i] / det;
        return true;
    }

    void modelDispHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        const uint32_t instance = GPR_U32(ctx, 4), matrix = GPR_U32(ctx, 5);
        float model[16];
        for (int i = 0; i < 16; ++i)
            model[i] = real(matrix + i * 4u);
        g_modelDisp(rdram, ctx, runtime); // the emulated drawing goes on as before
        if (!g_cameraContext)
            return;

        float camera[16], clip[16];
        for (int i = 0; i < 16; ++i)
            camera[i] = real(g_cameraContext + 0x5A0u + i * 4u);
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                    sum += camera[k * 4 + r] * model[c * 4 + k];
                clip[c * 4 + r] = sum;
            }

        const uint32_t header = word(word(instance) + 0x20u);
        static const uint32_t skipHeader = std::getenv("SOTC_NATIVE_SKIP") ? static_cast<uint32_t>(std::strtoul(std::getenv("SOTC_NATIVE_SKIP"), nullptr, 16)) : 0u; // debugging
        if (header == skipHeader)
            return;
        const bool fading = real(instance + 0x244u) > 0.0f; // modelDisp: param_1[0x91]
        // modelDisp's instance opacity (the microcode's, at its packet + 0xdc).
        float opacity = 1.0f - real(instance + 0x244u);
        if (word(header + 0xCu) != 0u)
            opacity *= 1.0f - real(instance + 0x248u);
        if (!(opacity > 0.0f))
        {
            static const bool log = std::getenv("SOTC_NATIVE_LOG") != nullptr;
            static uint64_t logged = 0;
            if (log && g_frame >= logged + 120u)
            {
                logged = g_frame;
                std::fprintf(stderr, "[native-hidden] frame %llu header %08x f91 %g f92 %g hc %08x\n", (unsigned long long)g_frame, header,
                             real(instance + 0x244u), real(instance + 0x248u), word(header + 0xCu));
            }
            return;
        }
        opacity = std::min(opacity, 1.0f);
        uint64_t lodWord;
        std::memcpy(&lodWord, rdram + ((instance + 0x250u) & 0x1FFFFF8u), 8);
        uint32_t lod = static_cast<uint32_t>((lodWord >> 40) & 7u);
        if ((word(header + 0x18u) & 1u) && lod >= word(header + 0x1Cu) && word(header + 0x1Cu) != 0u)
            lod = word(header + 0x1Cu) - 1u;
        const uint32_t table = word(header + 0x30u + lod * 4u);
        // modelDisp (0x1097118) tests the parts against the camera only when
        // the model has bit 0 of +0x18 clear and +0x54 != 1; otherwise every
        // packet of the LOD is drawn. The test is made here again rather than
        // read from the parts' +0x2c: the recompiled modelDisp can come back
        // to this hook before its test has run (a checkpoint inside
        // manager_sub_01099A08 returns with pc there, the rest of modelDisp
        // runs after), so +0x2c still held another instance's or an earlier
        // frame's result (on-screen parts skipped, off-screen ones drawn).
        const bool partCulled = (word(header + 0x18u) & 1u) == 0u && word(header + 0x54u) != 1u;
        if (partCulled && table == 0u)
            return; // modelDisp returns too (bVar4 stays set): nothing drawn
        static std::vector<uint8_t> partVisible;
        if (partCulled)
        {
            partVisible.clear();
            // The box (node +0 min, +0x10 max) through camera x model, VCLIP
            // against |w|: 1 every corner out past one plane, 2 all inside, 0 partly.
            const auto test = [&](uint32_t node) -> uint32_t
            {
                uint32_t all = 0x3Fu, any = 0u;
                for (int c = 0; c < 8; ++c)
                {
                    const float x = real(node + ((c & 1) ? 0x10u : 0u)), y = real(node + ((c & 2) ? 0x14u : 4u)), z = real(node + ((c & 4) ? 0x18u : 8u));
                    float v[4];
                    for (int r = 0; r < 4; ++r)
                        v[r] = clip[r] * x + clip[4 + r] * y + clip[8 + r] * z + clip[12 + r];
                    const float w = std::fabs(v[3]);
                    const uint32_t flags = (v[0] > w ? 1u : 0u) | (v[0] < -w ? 2u : 0u) | (v[1] > w ? 4u : 0u) | (v[1] < -w ? 8u : 0u) | (v[2] > w ? 16u : 0u) |
                                           (v[2] < -w ? 32u : 0u);
                    all &= flags;
                    any |= flags;
                }
                return all ? 1u : (any ? 0u : 2u);
            };
            // manager_sub_01099A08: a node takes its parent's result unless
            // that is 0 (partly inside), then it is tested; +0x20 / +0x24 the
            // two nodes under it.
            const auto visit = [&](auto &&self, uint32_t index, uint32_t inherited, int depth) -> void
            {
                if (index >= 4096u || depth > 64)
                    return;
                const uint32_t node = table + index * 0x30u;
                const uint32_t state = inherited ? inherited : test(node);
                if (partVisible.size() <= index)
                    partVisible.resize(index + 1u, 0u); // not reached: drawn
                partVisible[index] = static_cast<uint8_t>(state);
                for (uint32_t link : {0x20u, 0x24u})
                    if (word(node + link) != 0xFFFFFFFFu)
                        self(self, word(node + link), state, depth + 1);
            };
            const uint32_t root = test(table);
            if (root == 1u)
                return; // modelDisp returns: nothing of the model is drawn
            partVisible.assign(1u, static_cast<uint8_t>(root));
            for (uint32_t link : {0x20u, 0x24u})
                if (word(table + link) != 0xFFFFFFFFu)
                    visit(visit, word(table + link), root, 1);
        }
        // Its packet list ends at a packet of size 0 (32 bytes each), not at a count.
        const uint32_t packets = word(header + 0x60u), count = 0x10000u;
        static std::vector<native_renderer::Vertex> vertices;
        static std::vector<Strip> strips;
        static std::vector<float> palette;
        const bool hasPalette = readPalette(instance, palette);
        float projection[16];
        for (int i = 0; i < 16; ++i)
            projection[i] = real(g_cameraContext + 0x40u + i * 4u);
        for (uint32_t k = 0; k < count; ++k)
        {
            const uint32_t pk = packets + k * 32u;
            if (word(pk) == 0u)
                break;
            if (word(pk + 4u) != lod)
                continue;
            const uint32_t part = word(pk + 12u);
            if (partCulled && part != 0xFFFFFFFFu && part < partVisible.size() && partVisible[part] == 1u)
                continue;
            const uint32_t surface = word(pk + 8u), s = word(header + 0x50u) + surface * 0x120u;
            if (!surfaceDrawn(s))
                continue;
            const uint32_t size = word(pk), data = word(pk + 16u);
            const uint64_t id = ((static_cast<uint64_t>(size) << 32) | data) ^ (sample(data, size, 32u) & 0x7FFF000000000000ull);
            auto skinnedPacket = g_skinnedPackets.find(id);
            const bool twoPasses = word(s + 0x1Cu) == 2u;
            if (skinnedPacket == g_skinnedPackets.end() &&
                (!native_renderer::hasMesh(id) || (twoPasses && !native_renderer::hasMesh(id ^ (1ull << 61)))))
            {
                strips.clear();
                readBatches(data, size, strips);
                if (skinned(strips) && lit(strips))
                    g_litPackets.insert(id);
                if (skinned(strips))
                    skinnedPacket = g_skinnedPackets.emplace(id, SkinnedPacket{std::move(strips), 0}).first;
                else
                {
                    vertices.clear();
                    if (lit(strips))
                        g_litPackets.insert(id);
                    const auto place = [](const BatchVertex &v, native_renderer::Vertex &o)
                    {
                        o.x = v.x;
                        o.y = v.y;
                        o.z = v.z;
                        o.nx = v.nx;
                        o.ny = v.ny;
                        o.nz = v.nz;
                    };
                    triangulate(strips, place, vertices);
                    native_renderer::uploadMesh(id, vertices.data(), static_cast<uint32_t>(vertices.size()));
                    if (twoPasses) // two textures: the second pass's mesh, with the second coordinates
                    {
                        vertices.clear();
                        triangulate(strips, place, vertices, true);
                        native_renderer::uploadMesh(id ^ (1ull << 61), vertices.data(), static_cast<uint32_t>(vertices.size()));
                    }
                }
            }
            uint64_t mesh = id;
            const float *transform = clip;
            if (skinnedPacket != g_skinnedPackets.end())
            {
                skinnedPacket->second.used = g_frame;
                if (!hasPalette)
                    continue;
                // Posed on the CPU (camera space), one mesh per draw this frame.
                vertices.clear();
                triangulate(skinnedPacket->second.strips, [](const BatchVertex &v, native_renderer::Vertex &o)
                {
                    o.x = o.y = o.z = o.nx = o.ny = o.nz = 0.0f;
                    for (uint32_t b = 0; b < v.bones; ++b)
                    {
                        const float *m = &palette[(v.bone[b] * 4u % 1024u) * 4u];
                        const float w = v.weight[b];
                        o.x += w * (m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12]);
                        o.y += w * (m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13]);
                        o.z += w * (m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14]);
                        o.nx += w * (m[0] * v.nx + m[4] * v.ny + m[8] * v.nz);
                        o.ny += w * (m[1] * v.nx + m[5] * v.ny + m[9] * v.nz);
                        o.nz += w * (m[2] * v.nx + m[6] * v.ny + m[10] * v.nz);
                    }
                }, vertices);
                mesh = (1ull << 63) | g_skinnedDraws++;
                native_renderer::uploadMesh(mesh, vertices.data(), static_cast<uint32_t>(vertices.size()), true);
                transform = projection;
            }
            static const bool logPasses = std::getenv("SOTC_NATIVE_LOG") != nullptr;
            if (logPasses)
            {
                uint64_t f;
                std::memcpy(&f, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
                static std::set<uint64_t> seen;
                if ((f & 0x2000200u) && seen.insert((static_cast<uint64_t>(header) << 32) | surface).second)
                {
                    uint32_t alphaReg = 0;
                    for (uint32_t q = s + 0x60u; q < s + 0xC0u; q += 16u)
                        if (word(q + 8u) == 0x42u)
                            alphaReg = word(q);
                    std::fprintf(stderr, "[native-pass] header %08x surface %u flags %016llx alpha %02x layer %u texmode %u opacity %g\n", header, surface,
                                 (unsigned long long)f, alphaReg & 0xFFu, surfaceLayer(header, s, fading), word(s + 0x1Cu), opacity);
                }
            }
            static const uint32_t skipLayers = [] // debugging: SOTC_NATIVE_SKIPLAYERS=12,18
            {
                uint32_t mask = 0;
                if (const char *env = std::getenv("SOTC_NATIVE_SKIPLAYERS"))
                    for (const char *p = env; *p;)
                    {
                        mask |= 1u << (std::strtoul(p, const_cast<char **>(&p), 10) & 31u);
                        if (*p)
                            ++p;
                    }
                return mask;
            }();
            // Layers 12/13 are the reflection pass (reflectionBegin): the world
            // seen in a mirror, drawn by the game into a buffer of its own with
            // the camera context's +0x520 matrix and sampled by the reflecting
            // surfaces (layers 14/15). Never on the screen; not rendered yet.
            const uint32_t layer = surfaceLayer(header, s, fading);
            static const int logLayer = std::getenv("SOTC_LOG_LAYER") ? std::atoi(std::getenv("SOTC_LOG_LAYER")) : -1;
            if (static_cast<int>(layer) == logLayer)
            {
                static std::set<uint64_t> told;
                if (told.insert((static_cast<uint64_t>(header) << 16) | surface).second)
                {
                    std::string regs;
                    char item[40];
                    for (uint32_t q = s + 0x20u; q < s + 0x100u; q += 16u)
                    {
                        std::snprintf(item, sizeof(item), " %02x:%08x%08x", word(q + 8u) & 0xFFu, word(q + 4u), word(q));
                        regs += item;
                    }
                    std::fprintf(stderr, "[layer %u model] header %08x surface %u flags %08x texmode %u frame %u regs%s\n", layer, header, surface,
                                 word(s + 0x18u), word(s + 0x1Cu), g_frame, regs.c_str());
                }
            }
            const bool noReflect = !native_renderer::tuning().reflections;
            if ((layer == 12u || layer == 13u) && noReflect)
                continue;
            if ((skipLayers >> (layer & 31u)) & 1u)
                continue;
            native_renderer::DrawState state;
            state.texture = surfaceTexture(header, surface);
            state.blend = surfaceBlend(s);
            state.clamp = surfaceClamp(s);
            state.order = surfaceLayer(header, s, fading);
            state.opacity = opacity;
            const bool noMips = !native_renderer::tuning().mipmaps;
            if (!noMips)
                surfaceLod(header, surface, state);
            if (std::getenv("SOTC_NATIVE_LOG") && state.maxLod > 0.0f)
            {
                static std::set<uint64_t> told;
                if (told.insert((static_cast<uint64_t>(header) << 16) | surface).second && told.size() < 30)
                    std::fprintf(stderr, "[mip] header %08x surface %u levels %g L x%g K %g\n", header, surface, state.maxLod + 1.0f, state.lodScale, state.lodK);
            }
            const bool noRow = !native_renderer::tuning().textureScroll;
            if (!noRow && surfaceRow(header, surface, state.uvOffset) && std::getenv("SOTC_NATIVE_LOG"))
            {
                static std::set<uint64_t> told;
                if (told.insert((static_cast<uint64_t>(header) << 16) | surface).second)
                    std::fprintf(stderr, "[row] header %08x surface %u layer %u offset %g %g\n", header, surface, surfaceLayer(header, s, fading), state.uvOffset[0], state.uvOffset[1]);
            }
            // Debugging: who drew what. SOTC_NATIVE_IDCOLOR=1 every model, =<n> > 1
            // only surfaces in layers >= n (the rest drawn as usual).
            static const int idFrom = std::getenv("SOTC_NATIVE_IDCOLOR") ? std::max(1, std::atoi(std::getenv("SOTC_NATIVE_IDCOLOR"))) : 0;
            if (idFrom && (idFrom == 1 || static_cast<int>(state.order) >= idFrom) && state.blend != native_renderer::Blend::Additive) // glows keep their look
            {
                uint32_t h = (header ^ (surface * 0x9E37u)) * 2654435761u;
                h ^= h >> 13;
                state.debugColour = (h & 0xFFFFFFu) | 0x101010u;
                state.blend = native_renderer::Blend::Opaque;
                static std::set<uint64_t> told;
                if (told.insert((static_cast<uint64_t>(header) << 16) | surface).second)
                    std::fprintf(stderr, "[native-id] %06x header %08x layer %u blend %d tex %08llx surface %u flags %08x\n", state.debugColour & 0xFFFFFFu, header, state.order,
                                 static_cast<int>(surfaceBlend(s)), static_cast<unsigned long long>(state.texture), surface, word(s + 0x18u));
            }
            state.background = state.order == 1u;            // the sky
            static const bool logMat = std::getenv("SOTC_NATIVE_LOGMAT") != nullptr;
            if (logMat && k == 0u)
            {
                static int n = 0;
                const uint32_t own = word(instance + 0xCu), ret = word(own + 4u);
                if (n++ < 200 && (word(ret) & 0xFFFFu) == 5u)
                    std::fprintf(stderr, "[mat] layer %u header %08x mine %g %g %g %g | %g %g %g %g || vu %g %g %g %g | %g %g %g %g\n", state.order, header,
                                 clip[0], clip[1], clip[3], clip[12], clip[5], clip[13], clip[14], clip[15],
                                 real(ret + 16u), real(ret + 20u), real(ret + 28u), real(ret + 64u), real(ret + 36u), real(ret + 68u), real(ret + 72u), real(ret + 76u));
            }
            state.fogged = false; // zFog is a pass over the picture (layer 4), as the game does it
            if (g_litPackets.count(id))
            {
                // The instance's lights (its own 15 qwords, unpacked to VU1
                // 0x3d8): the light matrix, then colours and ambient.
                state.lit = true;
                const uint32_t own = word(instance + 0xCu);
                for (uint32_t i = 0; i < 32u; ++i)
                    state.light[i] = real(own + 0x10u + i * 4u);
                if (mesh != id) // skinned: its lights are in camera space
                {
                    std::memcpy(g_viewLights, state.light, sizeof(g_viewLights));
                    g_haveViewLights = true;
                }
            }
            state.reflectionSource = layer == 12u || layer == 13u;
            // Reflecting surfaces (the water): the world's TEST (0x5140b: AREF 0x40,
            // AFAIL FB_ONLY) keeps their colour but not their Z, their vertex alpha
            // being 0x20; the splash rings drawn after on the surface stay visible.
            if ((word(s + 0x18u) & 0x2200u) == 0x2200u)
                state.noDepthWrite = true;
            const bool twoTextures = word(s + 0x1Cu) == 2u && mesh == id && !state.debugColour;
            if (twoTextures)
                state.writeDepth = true;
            native_renderer::draw(mesh, transform, state);
            ++g_frameDraws;
            ++g_modelDraws;
            if (twoTextures)
            {
                // The microcode draws the batch again with the second texture
                // and coordinates, blended by ALPHA_2 (context 2).
                native_renderer::DrawState second = state;
                second.texture = surfaceTexture(header, surface, 0x3Cu);
                second.uvOffset[0] = second.uvOffset[1] = 0.0f; // the second pair takes ROW z / w (0)
                second.maxLod = 0.0f;
                if (!noMips)
                    surfaceLod(header, surface, second, 0x3Cu);
                second.blend = surfaceBlend(s, 0x43u);
                second.clamp = surfaceClamp(s, 0x09u);
                second.writeDepth = false;
                second.overlay = true;
                native_renderer::draw(id ^ (1ull << 61), transform, second);
                ++g_frameDraws;
            }
            // Reflecting surfaces (flags 0x2200, modelDisp): their pass packet
            // (header +0x14 [1] + s x 0x40 + 0x20, a CALL to the constants
            // unpacked to VU 0x3f3) and their vertices again, in layer 9
            // (microcode 2) over waterRefractionBegin's blurred copy and in
            // layer 15 (microcode 1) over reflectionBegin's rippled mirror;
            // surfaces with bit 25 go to layer 14 instead (unrippled mirror,
            // ALPHA 0x8000000064: written as is).
            uint64_t sflags;
            std::memcpy(&sflags, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
            const bool layer14 = ((sflags >> 25) & 1u) != 0;
            if ((layer14 || (sflags & 0x2200u) == 0x2200u) && !noReflect && !state.debugColour)
            {
                const uint32_t rec = word(word(header + 0x14u) + 4u) + surface * 0x40u + 0x20u;
                const uint32_t data = word(rec + 4u); // CALL target: its VU constants
                native_renderer::DrawState refl = state;
                refl.reflectionSource = false;
                refl.reflectionSample = true;
                refl.overlay = true;
                refl.writeDepth = false;
                refl.lit = false;
                refl.maxLod = 0.0f;
                refl.blend = native_renderer::Blend::Normal;
                refl.order = layer14 ? 14u : 15u;
                // Microcode 1 (VU1 image 98a396b7, routine 0x648): ST from the
                // screen position, RGB the vertex colour as is, alpha = vertex
                // alpha x (VU 0x3f3 w / 128) x (0.2 + 0.8 (1 - |n . v|)), v the
                // normalised camera-space position, n the model's up axis in
                // camera space (VU 972-975, the part's model-view matrix).
                refl.fresnel = true;
                const float *mv = nullptr;
                float modelView[16];
                if (transform == clip && invert4(projection, modelView))
                {
                    float m[16];
                    for (int c = 0; c < 4; ++c)
                        for (int r = 0; r < 4; ++r)
                        {
                            float sum = 0.0f;
                            for (int k = 0; k < 4; ++k)
                                sum += modelView[k * 4 + r] * clip[c * 4 + k];
                            m[c * 4 + r] = sum;
                        }
                    std::memcpy(modelView, m, sizeof(m));
                    mv = modelView;
                }
                static const float identity4[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
                std::memcpy(refl.light, mv ? mv : identity4, 16 * sizeof(float));
                static const float tintAlpha = std::getenv("SOTC_REFLECT_ALPHA") ? static_cast<float>(std::atof(std::getenv("SOTC_REFLECT_ALPHA"))) : 1.0f;
                refl.reflectionTint[0] = refl.reflectionTint[1] = refl.reflectionTint[2] = 1.0f;
                refl.reflectionTint[3] = std::clamp(real(data + 0x3Cu) / 128.0f, 0.0f, 2.0f) * tintAlpha;
                // reflectionBegin's ripple: the mirror copied to fbp + 0x20 in 32
                // bands of 8 rows, band i shifted by (sin(2 (12 i + t) x 24 pi / H)
                // + 1) (0.8 x 8 i / H + 0.2) texels, t = *0x118c030 (+0.1 x the
                // frame step), H = *0x1192090.
                refl.mirrorRipple[0] = real(0x0118C030u);
                refl.mirrorRipple[1] = static_cast<float>(static_cast<int32_t>(word(0x01192090u)));
                if (layer14)
                {
                    refl.mirrorRipple[1] = 0.0f;
                    refl.blend = native_renderer::Blend::Opaque;
                }
                native_renderer::draw(mesh, transform, refl);
                ++g_frameDraws;
                // Microcode 2 in layer 9: the copy's colour as is (RGB 128), the
                // vertex alpha under PABE (below 0x80 not blended): opaque.
                const bool noRefract = !native_renderer::tuning().refraction;
                static const float refractAlpha = std::getenv("SOTC_REFRACT_ALPHA") ? static_cast<float>(std::atof(std::getenv("SOTC_REFRACT_ALPHA"))) : 1.0f;
                if (!noRefract && !layer14)
                {
                    native_renderer::DrawState refr = refl;
                    refr.reflectionSample = false;
                    refr.refractionSample = true;
                    refr.fresnel = false;
                    refr.blend = native_renderer::Blend::Normal;
                    refr.order = 9;
                    refr.reflectionTint[0] = refr.reflectionTint[1] = refr.reflectionTint[2] = 1.0f;
                    refr.reflectionTint[3] = refractAlpha;
                    if (std::getenv("SOTC_REFRACT_SHOW")) // debugging: where it lands, in red
                    {
                        refr.reflectionTint[0] = 4.0f;
                        refr.reflectionTint[1] = refr.reflectionTint[2] = 0.0f;
                    }
                    native_renderer::draw(mesh, transform, refr);
                    ++g_frameDraws;
                }
            }
        }
    }
}

void installSotcNative(PS2Runtime &runtime)
{
    using namespace sotc_native;
    if (!std::getenv("SOTC_NATIVE") || !runtime.hasFunction(kModelDisp) || !runtime.hasFunction(kGetCameraContext))
        return;
    if (!native_renderer::available())
    {
        std::cerr << "[sotc:native] no native renderer on this platform" << std::endl;
        return;
    }
    g_modelDisp = runtime.lookupFunction(kModelDisp);
    runtime.registerFunction(kModelDisp, modelDispHook);
    g_getCameraContext = runtime.lookupFunction(kGetCameraContext);
    runtime.registerFunction(kGetCameraContext, getCameraContextHook);
    if (runtime.hasFunction(kGsReg) && runtime.hasFunction(kTexTransRequest) && runtime.hasFunction(kPushLayer) && runtime.hasFunction(kPopLayer))
    {
        g_gsReg = runtime.lookupFunction(kGsReg);
        runtime.registerFunction(kGsReg, gsRegHook);
        g_texTransRequest = runtime.lookupFunction(kTexTransRequest);
        runtime.registerFunction(kTexTransRequest, texTransRequestHook);
        g_pushLayer = runtime.lookupFunction(kPushLayer);
        runtime.registerFunction(kPushLayer, pushLayerHook);
        g_popLayer = runtime.lookupFunction(kPopLayer);
        runtime.registerFunction(kPopLayer, popLayerHook);
        if (runtime.hasFunction(kGlEnd))
        {
            g_glEnd = runtime.lookupFunction(kGlEnd);
            runtime.registerFunction(kGlEnd, glEndHook);
        }
    }
    if (runtime.hasFunction(kShadowDisp))
    {
        g_shadowDisp = runtime.lookupFunction(kShadowDisp);
        runtime.registerFunction(kShadowDisp, shadowDispHook);
    }
    if (runtime.hasFunction(kPtclEnv) && runtime.hasFunction(kPtclVtx))
    {
        g_ptclEnv = runtime.lookupFunction(kPtclEnv);
        runtime.registerFunction(kPtclEnv, ptclEnvHook);
        g_ptclVtx = runtime.lookupFunction(kPtclVtx);
        runtime.registerFunction(kPtclVtx, ptclVtxHook);
        g_ptclMat = runtime.lookupFunction(kPtclMat);
        runtime.registerFunction(kPtclMat, ptclMatHook);
    }
    if (runtime.hasFunction(kScreenBlur))
    {
        g_screenBlur = runtime.lookupFunction(kScreenBlur);
        runtime.registerFunction(kScreenBlur, screenBlurHook);
    }
    if (runtime.hasFunction(kMeshDisp))
    {
        g_meshDisp = runtime.lookupFunction(kMeshDisp);
        runtime.registerFunction(kMeshDisp, meshDispHook);
    }
    if (runtime.hasFunction(kZFog))
    {
        g_zFog = runtime.lookupFunction(kZFog);
        runtime.registerFunction(kZFog, zFogHook);
    }
    std::cerr << "[sotc:native] models drawn by the native renderer" << std::endl;
}
