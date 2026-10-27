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
#include "runtime/gs/gs_frontend.h"
#include "runtime/gs/ps2_gs_psmct32.h"
#include "runtime/gs/ps2_gs_psmt4.h"
#include "runtime/gs/ps2_gs_psmt8.h"
#include "runtime/ps2_stats.h"
#include "ps2x/iop/audio_stats.h"
#include "runtime/ps2_thread_load.h"
#include "sotc_prof.h"
#include "sotc_decomp.h"
#include "lean_hot.h"

#include <algorithm>
#include <map>
#include <memory>
#include <cmath>
#if defined(__arm__) && defined(__ARM_NEON)
#include <arm_neon.h>
#endif
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <iostream>
#include <thread>
#include <set>
#include <string>
#include <sys/stat.h>
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

    // The frame timing log (Debug 'Frame timing log', SOTC_FRAME_LOG): the
    // native hooks' own time (after the recompiled call they wrap).
    struct HookTimer
    {
        uint64_t start = native_renderer::tuning().frameLog ? native_renderer::nowUs() : 0u;
        ~HookTimer()
        {
            if (start)
                native_renderer::frameCounters().hookUs += native_renderer::nowUs() - start;
        }
    };

    const uint8_t *g_ram = nullptr;
    // The display walk's fast paths (2026-10-22 evening), on unless
    // SOTC_NATIVE_R2=0 (no longer alternated by SOTC_AB); set once a frame
    // (syncFrame). Each part has a switch of its own too.
    bool g_r2 = true;
    bool g_quickTextures = true; // decodedTexture: no sample for a texture drawn the frame before (SOTC_TEX_QUICK=0: off)
    bool g_surfaceMemo = true;   // modelDispHook: a surface's state worked out once a frame (SOTC_SURFACE_MEMO=0: off)
    bool g_prefetch = true;      // modelDispState / modelDispHook: guest lines asked for ahead (SOTC_HOOK_PREFETCH=0: off)
    bool g_planOn = true;        // modelDispHook: draw plans kept across frames (SOTC_PLAN=0: off)
    uint32_t g_planVerify = 0;   // SOTC_PLAN_VERIFY=<n>: plan against the packet walk every n-th frame
    // SOTC_R2_CHECK=1 (debugging): the new paths checked against the old ones
    // at every use (the quick texture against its full sample, each surface's
    // kept state against the reads it replaces); counts every 600 frames.
    struct R2Check
    {
        bool on = std::getenv("SOTC_R2_CHECK") != nullptr;
        uint64_t texQuick = 0, texStale = 0, surfaces = 0, surfaceMismatches = 0, packetQuick = 0, packetStale = 0;
        void note(const char *what, bool bad)
        {
            if (!bad)
                return;
            ++surfaceMismatches;
            static int told = 0;
            if (told++ < 40)
                std::fprintf(stderr, "[r2-check] mismatch: %s\n", what);
        }
    };
    // Made at the first use (after the environment is read), then a plain
    // pointer: a function-local static costs a guard check with a barrier
    // (dmb) at every call on the Vita, and this is asked twice a draw.
    R2Check *g_r2Check = nullptr;
    inline R2Check &r2Check()
    {
        if (!g_r2Check)
            g_r2Check = new R2Check;
        return *g_r2Check;
    }
    inline void prefetchGuest(uint32_t addr) { __builtin_prefetch(g_ram + (addr & 0x1FFFFE0u)); }
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

    // SOTC_HOOK_STATS=1 (debugging): what modelDispHook walked, every 120 frames.
    struct HookStats
    {
        bool on = std::getenv("SOTC_HOOK_STATS") != nullptr;
        uint64_t instances = 0, packets = 0, lodPackets = 0, draws = 0, textures = 0, decoded = 0, idSame = 0, idPrev = 0, idQuick = 0, idSampled = 0,
                 idHashed = 0, nodes = 0, surfaces = 0, surfaceHits = 0;
        std::set<uint32_t> headers, surfaceSet, textureSet;
    };
    HookStats *g_hookStats = nullptr; // as r2Check: no guard per call
    inline HookStats &hookStats()
    {
        if (!g_hookStats)
            g_hookStats = new HookStats;
        return *g_hookStats;
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
    template <typename Mem>
    void readBatchesFrom(const Mem &m, uint32_t addr, uint32_t size, std::vector<Strip> &out)
    {
        const uint32_t end = addr + size;
        uint32_t p = addr;
        uint32_t positions = 0, normals = 0, colours = 0, uvs = 0, pairs = 0, skin = 0, count = 0;
        while (p < end)
        {
            const uint32_t code = m.word(p);
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
                        v = {m.real(positions + i * 12u), m.real(positions + i * 12u + 4u), m.real(positions + i * 12u + 8u), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 128, 128, 128, 128, 0, {0, 0, 0}, {0.0f, 0.0f, 0.0f}};
                        // V4-16 is the normal when the ST comes apart (V2-16), else two ST pairs.
                        const uint32_t st = uvs ? m.word(uvs + i * 4u) : (pairs ? m.word(pairs + i * 8u) : 0u);
                        if (pairs && !uvs) // the second texture's pair
                        {
                            const uint32_t st2 = m.word(pairs + i * 8u + 4u);
                            v.u2 = static_cast<int16_t>(st2 & 0xFFFFu) / 4096.0f;
                            v.v2 = static_cast<int16_t>(st2 >> 16) / 4096.0f;
                        }
                        if (normals)
                        {
                            v.nx = m.real(normals + i * 12u);
                            v.ny = m.real(normals + i * 12u + 4u);
                            v.nz = m.real(normals + i * 12u + 8u);
                        }
                        else if (pairs && uvs)
                        {
                            const uint32_t xy = m.word(pairs + i * 8u), z = m.word(pairs + i * 8u + 4u);
                            v.nx = static_cast<int16_t>(xy & 0xFFFFu) / 4096.0f;
                            v.ny = static_cast<int16_t>(xy >> 16) / 4096.0f;
                            v.nz = static_cast<int16_t>(z & 0xFFFFu) / 4096.0f;
                        }
                        v.u = static_cast<int16_t>(st & 0xFFFFu) / 4096.0f;
                        v.v = static_cast<int16_t>(st >> 16) / 4096.0f;
                        if (colours)
                        {
                            const uint32_t c = m.word(colours + i * 4u);
                            v.r = static_cast<uint8_t>(c);
                            v.g = static_cast<uint8_t>(c >> 8);
                            v.b = static_cast<uint8_t>(c >> 16);
                            v.a = static_cast<uint8_t>(c >> 24);
                        }
                        if (skin)
                        {
                            const uint32_t q = skin + i * 16u;
                            for (uint32_t k = 0; k < 4u; ++k)
                                v.skin[k] = m.word(q + k * 4u);
                            v.bones = static_cast<uint8_t>(std::min(m.word(q + 12u), 3u));
                            for (uint32_t k = 0; k < v.bones; ++k)
                            {
                                const float packed = m.real(q + k * 4u);
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

    // Guest memory as readBatches reads it: the game's, or a copy of a
    // packet's bytes (the async worker; past the copy reads 0).
    struct GuestMem
    {
        uint32_t word(uint32_t a) const { return sotc_native::word(a); }
        float real(uint32_t a) const { return sotc_native::real(a); }
    };
    struct CopyMem
    {
        uint32_t base;
        const std::vector<uint8_t> *bytes;
        uint32_t word(uint32_t a) const
        {
            const uint32_t off = (a & ~3u) - base;
            uint32_t v = 0;
            if (off <= bytes->size() && bytes->size() - off >= 4u)
                std::memcpy(&v, bytes->data() + off, 4);
            return v;
        }
        float real(uint32_t a) const
        {
            const uint32_t v = word(a);
            float f;
            std::memcpy(&f, &v, 4);
            return f;
        }
    };
    void readBatches(uint32_t addr, uint32_t size, std::vector<Strip> &out) { readBatchesFrom(GuestMem{}, addr, size, out); }

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

    // A hash of every byte of guest memory from addr (word aligned) on, 8 at a
    // time (content ids: packetId, decodedTexture).
    uint64_t contentHash(uint32_t addr, uint32_t bytes, uint64_t h)
    {
        addr &= 0x1FFFFFCu;
        const auto mix = [&h](uint64_t v)
        {
            h = (h ^ v) * 0x100000001b3ull;
            h ^= h >> 29;
        };
        if (addr + bytes <= 0x2000000u)
        {
            const uint8_t *p = g_ram + addr;
            uint32_t i = 0;
            for (; i + 8u <= bytes; i += 8u)
            {
                uint64_t v;
                std::memcpy(&v, p + i, 8);
                mix(v);
            }
            for (; i < bytes; ++i)
                mix(p[i]);
        }
        else
            for (uint32_t i = 0; i < bytes; ++i)
                mix(g_ram[(addr + i) & 0x1FFFFFFu]);
        return h;
    }

    // Memos by address, four ways a set (a scene draws a few thousand
    // packets: one way a slot made neighbours evict each other every frame,
    // and each time the full hash ran again); the way used longest ago goes.
    template <typename Entry, uint32_t Sets>
    struct WayMemo
    {
        Entry ways[Sets][4];
        Entry &find(uint32_t address, uint32_t hash, bool &found)
        {
            Entry *set = ways[hash & (Sets - 1u)];
            Entry *oldest = &set[0];
            for (int i = 0; i < 4; ++i)
            {
                if (set[i].address == address)
                {
                    found = true;
                    return set[i];
                }
                if (set[i].frame + 1u < oldest->frame + 1u) // ~0 (never used) first
                    oldest = &set[i];
            }
            found = false;
            return *oldest;
        }
    };
    uint64_t decodedTexture(uint32_t header);
    // texture() asked again for a header in the same frame: the key it gave
    // (the contents sampled once a frame; was every draw).
    struct TextureMemo
    {
        uint32_t header = 0;
        uint64_t frame = ~0ull, key = 0;
    };
    TextureMemo g_textureMemo[256];
    uint64_t texture(uint32_t header)
    {
        if (!native_renderer::tuning().fastHooks)
            return decodedTexture(header);
        TextureMemo &memo = g_textureMemo[(header ^ (header >> 9)) & 255u];
        if (HookStats &hs = hookStats(); hs.on)
            ++hs.textures, hs.textureSet.insert(header);
        if (memo.header == header && memo.frame == g_frame)
            return memo.key;
        if (HookStats &hs = hookStats(); hs.on)
            ++hs.decoded;
        const uint64_t key = decodedTexture(header);
        memo = {header, g_frame, key};
        return key;
    }

    // A texture as it is in guest memory, copied out (decodable on another
    // thread): its format, the palette (CSM1 order undone), and the bytes of
    // every level from the pixels' address on (and what a level's swizzled
    // read reaches past them).
    struct TexSource
    {
        uint32_t header = 0, psm = 0, width = 0, height = 0, levels = 1;
        uint8_t swizzleBits = 0;
        uint32_t palette[256] = {};
        std::vector<uint8_t> pixels;
    };

    // GS memory as an 8-bit / 4-bit texture reads it after a PSMCT32 upload
    // (the NTO2 swizzled levels), by page: a page is 8 KB in every format;
    // within one, an 8-bit pixel's byte (128 x 64), a 4-bit pixel's nibble
    // (128 x 128), and which 32-bit pixel (64 x 32) holds a byte. Made once
    // from the GS's own address functions.
    struct SwizzleTables
    {
        uint16_t t8[64 * 128], t4[128 * 128], inv32[2048];
    };
    const SwizzleTables &swizzleTables()
    {
        static const SwizzleTables *tables = []
        {
            auto *t = new SwizzleTables();
            for (uint32_t y = 0; y < 64u; ++y)
                for (uint32_t x = 0; x < 128u; ++x)
                    t->t8[y * 128u + x] = static_cast<uint16_t>(GSPSMT8::addrPSMT8(0, 2, x, y));
            for (uint32_t y = 0; y < 128u; ++y)
                for (uint32_t x = 0; x < 128u; ++x)
                    t->t4[y * 128u + x] = static_cast<uint16_t>(GSPSMT4::addrPSMT4(0, 2, x, y));
            for (uint32_t y = 0; y < 32u; ++y)
                for (uint32_t x = 0; x < 64u; ++x)
                    t->inv32[GSPSMCT32::addrPSMCT32(0, 1, x, y) >> 2] = static_cast<uint16_t>(x | (y << 6));
            return t;
        }();
        return *tables;
    }

    uint32_t log2u(uint32_t v)
    {
        uint32_t n = 0;
        while ((1u << (n + 1u)) <= v)
            ++n;
        return n;
    }

    // The source of a texture (formats checked by the caller); false: no room.
    bool texSource(uint32_t header, TexSource &src)
    {
        const uint32_t format = word(header + 0x1Cu);
        src.header = header;
        src.psm = format & 0x3Fu;
        src.width = 1u << ((format >> 16) & 0xFu);
        src.height = 1u << ((format >> 20) & 0xFu);
        // Mip levels (format bits 12-15, texTransResolve): each half the last,
        // stored one after the other; byte +0x1f has a swizzle bit per level.
        static const bool noMips = std::getenv("SOTC_NATIVE_NOMIPS") != nullptr;
        src.levels = noMips ? 1u : std::clamp((format >> 12) & 0xFu, 1u, 7u);
        src.swizzleBits = g_ram[(header + 0x1Fu) & 0x1FFFFFFu];
        if (src.psm != 0x00u)
        {
            const uint32_t clut = header + word(header + 0x18u);
            const uint32_t entries = src.psm == 0x14u ? 16u : 256u;
            for (uint32_t i = 0; i < entries; ++i)
            {
                // CSM1: of every 32 entries, 8-15 and 16-23 are swapped.
                const uint32_t from = entries == 256u ? ((i & ~0x18u) | ((i & 8u) << 1) | ((i & 16u) >> 1)) : i;
                src.palette[i] = word(clut + from * 4u);
            }
        }
        size_t at = 0, reach = 0;
        for (uint32_t level = 0, w = src.width, h = src.height; level < src.levels && w >= 1u && h >= 1u; ++level, w /= 2u, h /= 2u)
        {
            const size_t bytes = src.psm == 0x00u ? static_cast<size_t>(w) * h * 4u : (src.psm == 0x13u ? static_cast<size_t>(w) * h : static_cast<size_t>(w) * h / 2u);
            size_t read = bytes;
            if (src.psm != 0x00u && ((src.swizzleBits >> level) & 1u))
                read = static_cast<size_t>(std::max(w / 2u, 1u)) * std::max(src.psm == 0x13u ? h / 2u : h / 4u, 1u) * 4u;
            reach = std::max(reach, at + read);
            at += bytes;
        }
        try
        {
            src.pixels.resize(reach);
        }
        catch (const std::bad_alloc &)
        {
            return false;
        }
        const uint32_t pixels = (header + word(header + 0x14u)) & 0x1FFFFFFu;
        if (pixels + reach <= 0x2000000u)
            std::memcpy(src.pixels.data(), g_ram + pixels, reach);
        else
            for (size_t i = 0; i < reach; ++i)
                src.pixels[i] = g_ram[(pixels + i) & 0x1FFFFFFu];
        return true;
    }

    // RGBA8 of every level, one after the other; `decoded` the levels made.
    // Pure: runs on the async worker too.
    bool decodeTexture(const TexSource &src, std::vector<uint8_t> &rgba, uint32_t &decoded)
    {
        const uint32_t psm = src.psm, entries = psm == 0x14u ? 16u : 256u;
        const uint32_t *palette = src.palette;
        size_t total = 0;
        for (uint32_t level = 0, lw = src.width, lh = src.height; level < src.levels && lw >= 1u && lh >= 1u; ++level, lw /= 2u, lh /= 2u)
            total += static_cast<size_t>(lw) * lh * 4u;
        try
        {
            rgba.resize(total);
        }
        catch (const std::bad_alloc &)
        {
            return false;
        }
        const SwizzleTables &tables = swizzleTables();
        const uint8_t *raw = src.pixels.data();
        const size_t rawSize = src.pixels.size();
        size_t at = 0, base = 0;
        decoded = 0;
        for (uint32_t level = 0, w = src.width, h = src.height; level < src.levels && w >= 1u && h >= 1u; ++level, w /= 2u, h /= 2u)
        {
            uint8_t *out = &rgba[base];
            base += static_cast<size_t>(w) * h * 4u;
            const uint8_t *in = raw + at;
            const size_t avail = rawSize - at;
            const bool swizzled = ((src.swizzleBits >> level) & 1u) != 0u;
            if (psm == 0x00u)
                std::memcpy(out, in, std::min(avail, static_cast<size_t>(w) * h * 4u)); // PSMCT32: the pixels themselves (alpha 0x80 = opaque)
            else if (!swizzled)
            {
                uint32_t *o = reinterpret_cast<uint32_t *>(out);
                if (psm == 0x14u)
                    for (uint32_t y = 0; y < h; ++y)
                        for (uint32_t x = 0; x < w; ++x)
                        {
                            const uint8_t b = in[(y * w + x) / 2u];
                            *o++ = palette[(x & 1u) ? (b >> 4) : (b & 15u)];
                        }
                else
                    for (uint32_t i = 0; i < w * h; ++i)
                        o[i] = palette[in[i]];
            }
            else
            {
                // Uploaded as PSMCT32 (cw x ch, bw32 pages a row) and read back
                // as 8 / 4-bit (tbw / 2 pages a row): each pixel's GS byte, then
                // the 32-bit pixel that wrote it. The PS2's scratch area of
                // max(w, 64) x max(h, 64) + 16 KB: bytes past it read 0, as do
                // the bytes no 32-bit pixel wrote.
                const uint32_t cw = std::max(w / 2u, 1u), ch = std::max(psm == 0x13u ? h / 2u : h / 4u, 1u);
                const uint32_t bw32 = std::max<uint32_t>(cw / 64u, 1u), bwShift = log2u(bw32);
                const uint32_t tbw = std::max<uint32_t>(w / 64u, 2u), ppr = tbw >> 1;
                const uint32_t scratch = std::max(w, 64u) * std::max(h, 64u) + 0x4000u;
                const auto byteAt = [&](uint32_t a) -> uint32_t
                {
                    if (a >= scratch)
                        return 0u;
                    const uint32_t page = a >> 13, q = tables.inv32[(a & 8191u) >> 2];
                    const uint32_t cx = ((page & (bw32 - 1u)) << 6) + (q & 63u), cy = ((page >> bwShift) << 5) + (q >> 6);
                    if (cx >= cw || cy >= ch)
                        return 0u;
                    const size_t from = (static_cast<size_t>(cy) * cw + cx) * 4u + (a & 3u);
                    return from < avail ? in[from] : 0u;
                };
                uint32_t *o = reinterpret_cast<uint32_t *>(out);
                if (psm == 0x14u)
                    for (uint32_t y = 0; y < h; ++y)
                    {
                        const uint32_t rowPage = (y >> 7) * ppr;
                        const uint16_t *row = &tables.t4[(y & 127u) * 128u];
                        for (uint32_t x = 0; x < w; ++x)
                        {
                            const uint32_t n = ((rowPage + (x >> 7)) << 14) + row[x & 127u];
                            const uint32_t b = byteAt(n >> 1);
                            *o++ = palette[(n & 1u) ? (b >> 4) : (b & 15u)];
                        }
                    }
                else
                    for (uint32_t y = 0; y < h; ++y)
                    {
                        const uint32_t rowPage = (y >> 6) * ppr;
                        const uint16_t *row = &tables.t8[(y & 63u) * 128u];
                        for (uint32_t x = 0; x < w; ++x)
                            *o++ = palette[byteAt(((rowPage + (x >> 7)) << 13) + row[x & 127u]) & (entries - 1u)];
                    }
            }
            ++decoded;
            at += psm == 0x00u ? static_cast<size_t>(w) * h * 4u : (psm == 0x13u ? static_cast<size_t>(w) * h : static_cast<size_t>(w) * h / 2u);
        }
        return true;
    }

    bool decodeTextureReference(uint32_t header, std::vector<uint8_t> &rgba, uint32_t &decodedOut);

    uint64_t decodedTexture(uint32_t header)
    {
        if (!header || word(header) != 0x324F544Eu) // "NTO2"
            return 0;
        const uint32_t format = word(header + 0x1Cu), psm = format & 0x3Fu;
        const uint32_t width = 1u << ((format >> 16) & 0xFu), height = 1u << ((format >> 20) & 0xFu);
        const uint32_t pixelBytes = psm == 0x14u ? width * height / 2u : width * height;
        const auto sampleNow = [&]
        {
            return sample(header + word(header + 0x14u), pixelBytes, 64u) ^ sample(header + word(header + 0x18u), psm == 0x14u ? 64u : 1024u, 16u) * 31u ^
                   format;
        };
        uint64_t key = 0;
        bool changedInPlace = false;
        if (native_renderer::tuning().fastHooks)
        {
            // Fast hooks: a hash of the texture's bytes (every level, the
            // palette, the format and swizzle bits), kept by header while the
            // sample above stays the same (see packetId: the game moves
            // textures in memory after an area loads).
            struct ContentMemo
            {
                uint32_t address = 0, format = 0;
                uint64_t frame = ~0ull, sampled = 0, key = 0, sampledAt = 0;
                bool rewritten = false; // seen changed in place: sampled at every call
                uint32_t moveSerial = 0; // sotc_decomp::g_guestMoveSerial at the last sample
            };
            static WayMemo<ContentMemo, 512> memos;
            bool found = false;
            ContentMemo &memo = memos.find(header, (header >> 4) ^ (header >> 13), found);
            // The sample is 64 words spread over the pixels and 16 over the
            // palette: ~80 cache misses (the Vita: ~20 us) for each texture
            // every frame. As packetId: a texture drawn the frame before,
            // never seen rewritten in place, is taken as it was; the full
            // sample comes round one frame in 16 (by address).
            if (found && g_quickTextures && !memo.rewritten && memo.moveSerial == sotc_decomp::g_guestMoveSerial && memo.format == format && memo.frame + 1u >= g_frame && g_frame - memo.sampledAt < 16u &&
                ((g_frame + (header >> 5)) & 15u) != 0u)
            {
                key = memo.key;
                memo.frame = g_frame;
                if (R2Check &c = r2Check(); c.on)
                {
                    ++c.texQuick;
                    if (sampleNow() != memo.sampled)
                    {
                        if (c.texStale++ < 20)
                            std::fprintf(stderr, "[r2-check] texture %08x changed, kept its key (frame %llu)\n", header, (unsigned long long)g_frame);
                    }
                }
            }
            else if (const uint64_t sampled = sampleNow(); found && memo.sampled == sampled)
            {
                key = memo.key;
                memo.frame = g_frame;
                memo.sampledAt = g_frame;
                memo.format = format;
                memo.moveSerial = sotc_decomp::g_guestMoveSerial;
            }
            else
            {
                // Drawn in the last two frames and changed: rewritten where it is
                // (animated), decoded at once, not on the worker.
                // (not after a garbage move: then another texture was moved there)
                changedInPlace = found && memo.moveSerial == sotc_decomp::g_guestMoveSerial && memo.frame != ~0ull && memo.frame + 2u >= g_frame;
                uint32_t bytes = 0;
                for (uint32_t level = 0, w = width, h = height, levels = std::clamp((format >> 12) & 0xFu, 1u, 7u); level < levels && w >= 1u && h >= 1u;
                     ++level, w /= 2u, h /= 2u)
                    bytes += psm == 0x00u ? w * h * 4u : (psm == 0x13u ? w * h : w * h / 2u);
                uint64_t h = contentHash(header + word(header + 0x14u), bytes, 0x84222325cbf29ce4ull ^ format);
                h = contentHash(header + word(header + 0x18u), psm == 0x00u ? 0u : (psm == 0x14u ? 64u : 1024u), h ^ g_ram[(header + 0x1Fu) & 0x1FFFFFFu]);
                key = (h & 0x03FFFFFFFFFFFFFFull) | (1ull << 58);
                memo = {header, format, g_frame, sampled, key, g_frame, found && (memo.rewritten || changedInPlace), sotc_decomp::g_guestMoveSerial};
            }
        }
        else
            key = sampleNow();
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
        const uint64_t decodeStart = native_renderer::tuning().frameLog ? native_renderer::nowUs() : 0u;
        auto src = std::make_shared<TexSource>();
        if (!texSource(header, *src))
            return 0; // no room: asked for again at the next draw
        // SOTC_TEX_DECODE_CHECK=1 (debugging): every texture also decoded by
        // the earlier per-pixel code, and the two compared.
        static const bool decodeCheck = std::getenv("SOTC_TEX_DECODE_CHECK") != nullptr;
        if (decodeCheck)
        {
            std::vector<uint8_t> fast, slow;
            uint32_t fastLevels = 0, slowLevels = 0;
            const bool a = decodeTexture(*src, fast, fastLevels), b = decodeTextureReference(header, slow, slowLevels);
            static uint64_t checked = 0, mismatched = 0;
            ++checked;
            if (a != b || fastLevels != slowLevels || fast != slow)
            {
                ++mismatched;
                std::fprintf(stderr, "[tex-check] MISMATCH header %08x psm %02x %ux%u levels %u/%u swizzle %02x\n", header, psm, width, height, fastLevels, slowLevels,
                             src->swizzleBits);
            }
            if (checked % 200u == 0u)
                std::fprintf(stderr, "[tex-check] %llu textures, %llu mismatches\n", (unsigned long long)checked, (unsigned long long)mismatched);
        }
        // SOTC_TEXDUMP_DIR=<dir> (debugging): level 0 of every decoded texture, raw RGBA.
        static const char *dumpDir = std::getenv("SOTC_TEXDUMP_DIR");
        const auto finish = [key, width, height, header, psm](const std::vector<uint8_t> &rgba, uint32_t decoded)
        {
            if (dumpDir)
            {
                char path[512];
                std::snprintf(path, sizeof(path), "%s/nat_%08x_psm%02x_%ux%u.rgba", dumpDir, header, psm, width, height);
                if (FILE *f = std::fopen(path, "wb"))
                {
                    std::fwrite(rgba.data(), 1, static_cast<size_t>(width) * height * 4u, f);
                    std::fclose(f);
                }
            }
            native_renderer::uploadTexture(key, width, height, rgba.data(), decoded);
        };
        // On the worker: decoded there, uploaded at the next frame's start.
        auto out = std::make_shared<std::pair<std::vector<uint8_t>, uint32_t>>();
        auto ok = std::make_shared<bool>(false);
        if (!changedInPlace && native_renderer::buildAsync({key}, [src, out, ok] { *ok = decodeTexture(*src, out->first, out->second); },
                                        [out, ok, finish]
                                        {
                                            if (*ok)
                                                finish(out->first, out->second);
                                        }))
        {
            if (decodeStart)
                native_renderer::frameCounters().decodeUs += native_renderer::nowUs() - decodeStart;
            return key;
        }
        static std::vector<uint8_t> rgba;
        uint32_t decoded = 0;
        if (!decodeTexture(*src, rgba, decoded))
            return 0;
        if (decodeStart)
            native_renderer::frameCounters().decodeUs += native_renderer::nowUs() - decodeStart;
        finish(rgba, decoded);
        return key;
    }

    // The earlier decoder (per pixel through the GS address functions,
    // straight from guest memory): SOTC_TEX_DECODE_CHECK's reference.
    bool decodeTextureReference(uint32_t header, std::vector<uint8_t> &rgba, uint32_t &decodedOut)
    {
        const uint32_t format = word(header + 0x1Cu), psm = format & 0x3Fu;
        const uint32_t width = 1u << ((format >> 16) & 0xFu), height = 1u << ((format >> 20) & 0xFu);
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
        rgba.clear();
        std::vector<uint8_t> scratch;
        // Room for every level at once (growing level by level copied the
        // levels before each time); no room in the heap: no texture this time
        // (asked for again at the next draw), rather than an abort.
        {
            size_t total = 0;
            for (uint32_t level = 0, lw = width, lh = height; level < levels && lw >= 1u && lh >= 1u; ++level, lw /= 2u, lh /= 2u)
                total += static_cast<size_t>(lw) * lh * 4u;
            try
            {
                rgba.reserve(total);
                if (psm != 0x00u)
                    scratch.reserve(static_cast<size_t>(std::max(width, 64u)) * std::max(height, 64u) + 0x4000u);
            }
            catch (const std::bad_alloc &)
            {
                return false;
            }
        }
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
        decodedOut = decoded;
        return true;
    }

    // A model packet's id. Fast hooks: a hash of all its bytes, not of where
    // they are. After an area loads the game moves its model data and
    // textures around in memory (its heap compaction), several times in the
    // first second: ids by address rebuilt the whole scene each time (~2 400
    // packets and ~100 textures, five times in ~20 frames, every one of them
    // the same bytes as before). A memo by address keeps a packet's id while a
    // sample of its contents (32 words) stays the same, so the full hash runs
    // only when the packet is new there; the same packet drawn again in a
    // frame (instances) takes the id it had. Without fast hooks: the address
    // and size, and the sample (another area loaded in its place makes new ids).
    // What a draw of a packet drawn the frame before reads is in its first
    // 32 bytes (one cache line on the Vita).
    struct alignas(64) PacketMemo
    {
        uint32_t address = 0, size = 0;
        uint64_t frame = ~0ull, id = 0;
        uint32_t sampledAt = 0;  // the frame of the last full sample (its low 32 bits)
        bool rewritten = false;  // seen changed in place
        uint16_t moveSerial = 0; // sotc_decomp::g_guestMoveSerial at the last sample (a garbage move since: sampled again)
        uint64_t sample = 0, quick = 0; // 32 words; four words
    };
    // The memo's sets: four ways, the addresses apart from the entries (a set's
    // four tags in 16 bytes: one cache line looked at, not the four entries'
    // 3-4), so the next packet's lookup can be prefetched.
    template <uint32_t Sets>
    struct PacketMemoTable
    {
        struct alignas(16) Tags
        {
            uint32_t address[4] = {};
        };
        Tags tags[Sets];
        PacketMemo entries[Sets][4];
        static uint32_t set(uint32_t address) { return ((address >> 4) ^ (address >> 15)) & (Sets - 1u); }
        PacketMemo &find(uint32_t address, bool &found)
        {
            const uint32_t at = set(address);
            Tags &t = tags[at];
            for (int i = 0; i < 4; ++i)
                if (t.address[i] == address && entries[at][i].address == address)
                {
                    found = true;
                    return entries[at][i];
                }
            found = false;
            int oldest = 0; // the way used longest ago (~0: never used, first)
            for (int i = 1; i < 4; ++i)
                if (entries[at][i].frame + 1u < entries[at][oldest].frame + 1u)
                    oldest = i;
            t.address[oldest] = address; // the caller fills the entry
            return entries[at][oldest];
        }
        // The tag line, and the entry's first line when the tag is there already.
        void prefetch(uint32_t address, bool entry) const
        {
            const uint32_t at = set(address);
            const Tags &t = tags[at];
            __builtin_prefetch(&t);
            if (entry)
                for (int i = 0; i < 4; ++i)
                    if (t.address[i] == address)
                        __builtin_prefetch(&entries[at][i]);
        }
    };
    // Four words of a packet, spread over it (the quick check).
    inline uint64_t quickSample(uint32_t data, uint32_t size)
    {
        const uint32_t step = (size / 5u) & ~3u;
        uint64_t h = 0xcbf29ce484222325ull ^ data;
        for (uint32_t i = 1; i <= 4; ++i)
            h = (h ^ word(data + (size >= 8u ? std::min(step * i, size - 4u) : 0u))) * 0x100000001b3ull;
        return h;
    }
    PacketMemoTable<2048> g_packetMemo;
    // The last packetId whose bytes changed where they are (same address and
    // size, another sample): packets the game rewrites in place (every frame,
    // some of them) are built at once, not on the async worker, which would
    // leave them out of every frame.
    bool g_packetChangedInPlace = false;
    // The last packetId's packet seen rewritten in place at some time (its
    // memo's flag): a draw plan asks for that packet's id every frame.
    bool g_packetRewritten = false;
    uint64_t packetId(uint32_t data, uint32_t size, bool fast)
    {
        g_packetChangedInPlace = false;
        g_packetRewritten = false;
        if (!fast)
            return ((static_cast<uint64_t>(size) << 32) | data) ^ (sample(data, size, 32u) & 0x7FFF000000000000ull);
        bool found = false;
        PacketMemo &memo = g_packetMemo.find(data, found);
        found = found && memo.size == size;
        HookStats &hs = hookStats();
        g_packetRewritten = found && memo.rewritten;
        const uint16_t moveSerial = static_cast<uint16_t>(sotc_decomp::g_guestMoveSerial);
        const bool moved = found && memo.moveSerial != moveSerial;
        if (found && memo.frame == g_frame && !moved)
        {
            hs.idSame += hs.on;
            return memo.id;
        }
        // This round (g_r2): the 32-word sample (32 cache misses) only one
        // frame in 48-63; on the other frames in 16 four words (a packet
        // rewritten in place is still caught within 16 frames).
        const bool stagger = ((g_frame + (data >> 5)) & 15u) == 0u;
        const uint32_t age = static_cast<uint32_t>(g_frame) - memo.sampledAt;
        if (g_r2 && found && !moved && !memo.rewritten && native_renderer::tuning().packetQuickCheck && !(stagger && age >= 48u))
        {
            if ((memo.frame + 1u == g_frame && !stagger) || quickSample(data, size) == memo.quick)
            {
                (memo.frame + 1u == g_frame ? hs.idPrev : hs.idQuick) += hs.on;
                memo.frame = g_frame;
                if (R2Check &c = r2Check(); c.on)
                {
                    ++c.packetQuick;
                    if (sample(data, size, 32u) != memo.sample && c.packetStale++ < 20)
                        std::fprintf(stderr, "[r2-check] packet %08x changed, kept its id (frame %llu)\n", data, (unsigned long long)g_frame);
                }
                return memo.id;
            }
        }
        // Never rewritten in place: four words, the full sample one frame in
        // 16 (by address, so not all on the same frame).
        else if (!g_r2 && found && !moved && !memo.rewritten && native_renderer::tuning().packetQuickCheck && age < 16u && !stagger)
        {
            // Drawn the frame before too: taken as it was (the full sample
            // comes round within 16 frames). Back after a while: four words.
            if (memo.frame + 1u == g_frame || quickSample(data, size) == memo.quick)
            {
                (memo.frame + 1u == g_frame ? hs.idPrev : hs.idQuick) += hs.on;
                memo.frame = g_frame;
                if (R2Check &c = r2Check(); c.on)
                {
                    ++c.packetQuick;
                    if (sample(data, size, 32u) != memo.sample && c.packetStale++ < 20)
                        std::fprintf(stderr, "[r2-check] packet %08x changed, kept its id (frame %llu)\n", data, (unsigned long long)g_frame);
                }
                return memo.id;
            }
        }
        const uint64_t sampled = sample(data, size, 32u);
        hs.idSampled += hs.on;
        if (found && memo.sample == sampled)
        {
            memo.frame = g_frame;
            memo.sampledAt = static_cast<uint32_t>(g_frame);
            memo.moveSerial = moveSerial;
            memo.quick = quickSample(data, size);
            return memo.id;
        }
        // Drawn in the last two frames and changed: rewritten in place
        // (animated). Long unused: another packet loaded there (async is fine).
        // Not after a garbage move: then another packet was moved there.
        g_packetChangedInPlace = !moved && found && memo.frame != ~0ull && memo.frame + 2u >= g_frame;
        hs.idHashed += hs.on;
        // Bit 59 set, 60-63 clear: apart from the per-frame meshes' ids
        // (bits 60, 62, 63 and a count) and the second pass's (bit 61 flipped).
        const uint64_t id = (contentHash(data, size, 0x9E3779B97F4A7C15ull ^ size) & 0x07FFFFFFFFFFFFFFull) | (1ull << 59);
        const bool rewritten = found && (memo.rewritten || g_packetChangedInPlace);
        memo.address = data;
        memo.size = size;
        memo.frame = g_frame;
        memo.id = id;
        memo.sampledAt = static_cast<uint32_t>(g_frame);
        memo.rewritten = rewritten;
        memo.moveSerial = moveSerial;
        g_packetRewritten = rewritten;
        memo.sample = sampled;
        memo.quick = quickSample(data, size);
        return id;
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

    // SOTC_LOG_SURFACE_REGS, read once into a plain flag (no guard / barrier per call).
    bool g_logSurfaceRegs()
    {
        static int8_t state = -1; // game thread only
        if (state < 0)
            state = std::getenv("SOTC_LOG_SURFACE_REGS") != nullptr;
        return state != 0;
    }
    native_renderer::Blend surfaceBlend(uint32_t s, uint32_t reg = 0x42u)
    {
        // SOTC_LOG_SURFACE_REGS=1: the distinct GS registers of the surfaces' A+D packets.
        if (reg == 0x42u && g_logSurfaceRegs())
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

    // A surface's draw state as modelDispHook works it out, kept for the
    // frame (a scene draws ~3 000 packets a frame over ~450 surfaces): the
    // surface record (5 cache lines), the texture table, the texture object
    // and its NTO2 header are read once a frame per surface, not per draw.
    // Read per draw still: the flags' tests against the game's globals
    // (surfaceDrawn), the scroll row's values, the LOD's K scale.
    struct SurfaceInfo
    {
        uint32_t s = 0, header = 0;
        uint64_t frame = ~0ull;
        uint64_t flags = 0;   // +0x18 (64 bits)
        uint32_t texMode = 0; // +0x1c
        bool full = false;    // the rest below made (at the first draw that gets that far)
        uint8_t layer[2] = {}; // surfaceLayer, not fading / fading
        uint8_t clamp = 0, clamp2 = 0;
        native_renderer::Blend blend = native_renderer::Blend::Normal, blend2 = native_renderer::Blend::Normal;
        uint32_t scroll = 0; // the first texture's scroll row (two 16-bit values), 0: none
        uint64_t texture = 0, texture2 = 0;
        struct Lod
        {
            bool on = false;
            float maxLod = 0.0f, lodScale = 1.0f;
            int32_t k = 0; // NTO2 +0x90, times *0x118c180 per draw
        } lod[2];
    };
    SurfaceInfo g_surfaceInfo[1024];
    // surfaceLod's reads, kept (the K product made per draw: applyLod).
    SurfaceInfo::Lod surfaceLodInfo(uint32_t header, uint32_t s, uint32_t field)
    {
        SurfaceInfo::Lod l;
        const uint32_t index = word(s + field);
        if (index >= word(header + 0x44u))
            return l;
        const uint32_t object = word(word(header + 0x40u) + index * 0x20u + 0x10u);
        const uint32_t nto2 = object ? word(object + 0x20u) : 0u;
        if (!nto2 || word(nto2) != 0x324F544Eu)
            return l;
        const uint32_t levels = std::clamp((word(nto2 + 0x1Cu) >> 12) & 0xFu, 1u, 7u);
        if (levels < 2u || word(nto2 + 0x78u) < 2u)
            return l;
        l.on = true;
        l.maxLod = static_cast<float>(levels - 1u);
        l.lodScale = static_cast<float>(1u << (word(nto2 + 0x94u) & 3u));
        l.k = static_cast<int32_t>(word(nto2 + 0x90u));
        return l;
    }
    void applyLod(const SurfaceInfo::Lod &l, native_renderer::DrawState &state)
    {
        if (!l.on)
            return;
        state.maxLod = l.maxLod;
        state.lodScale = l.lodScale;
        const int32_t k = static_cast<int32_t>(static_cast<float>(l.k) * real(0x0118C180u));
        state.lodK = static_cast<float>((k << 20) >> 20) / 16.0f; // 12-bit signed, 7.4
    }
    // surfaceRow's reads: the row pair's address.
    uint32_t surfaceScroll(uint32_t header, uint32_t s)
    {
        const uint32_t index = word(s + 0x2Cu);
        if (index >= word(header + 0x44u))
            return 0;
        const uint32_t object = word(word(header + 0x40u) + index * 0x20u + 0x10u);
        const uint32_t nto2 = object ? word(object + 0x20u) : 0u;
        return nto2 ? word(nto2 + 0xCu) : 0u;
    }
    inline uint32_t surfaceSlot(uint32_t s) { return ((s >> 5) * 0x9E3779B1u) >> 22; }
    // The flags and texture mode (what every packet asks first).
    SurfaceInfo &surfaceInfo(uint32_t header, uint32_t s)
    {
        SurfaceInfo &i = g_surfaceInfo[surfaceSlot(s)];
        if (i.s == s && i.header == header && i.frame == g_frame)
            return i;
        i.s = s;
        i.header = header;
        i.frame = g_frame;
        std::memcpy(&i.flags, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
        i.texMode = word(s + 0x1Cu);
        i.full = false;
        i.layer[0] = static_cast<uint8_t>(surfaceLayer(header, s, false));
        i.layer[1] = static_cast<uint8_t>(surfaceLayer(header, s, true));
        return i;
    }
    // surfaceDrawn from the kept flags.
    bool surfaceDrawnFlags(uint64_t flags)
    {
        const uint32_t boss = word(0x1186290u);
        if ((flags >> 29) & 1u)
            return false;
        if ((flags >> 17) & 1u)
            return (boss & 1u) != 0;
        if (((flags >> 24) & 1u) && !(boss & 2u))
            return false;
        return !(((flags >> 26) & 1u) && word(0x1186328u) != 0u);
    }
    // The rest, at the first draw of the surface this frame.
    const SurfaceInfo &surfaceInfoFull(SurfaceInfo &i, uint32_t surface)
    {
        if (i.full)
            return i;
        i.full = true;
        i.texture = surfaceTexture(i.header, surface, 0x2Cu);
        i.blend = surfaceBlend(i.s, 0x42u);
        i.clamp = surfaceClamp(i.s, 0x08u);
        i.lod[0] = surfaceLodInfo(i.header, i.s, 0x2Cu);
        i.scroll = surfaceScroll(i.header, i.s);
        if (i.texMode == 2u)
        {
            i.texture2 = surfaceTexture(i.header, surface, 0x3Cu);
            i.blend2 = surfaceBlend(i.s, 0x43u);
            i.clamp2 = surfaceClamp(i.s, 0x09u);
            i.lod[1] = surfaceLodInfo(i.header, i.s, 0x3Cu);
        }
        else
        {
            i.texture2 = 0;
            i.lod[1] = {};
        }
        return i;
    }

    // Skinned parts: their instance chain (instance + 0xc: a NEXT tag past
    // the instance's own 15 qwords) ends in a RET packet that unpacks the
    // bone palette to VU1 qword 0 (modelSetMatrixTransTag), each matrix
    // bone x view (camera space, the projection comes after). Rigid parts
    // upload one matrix there (5 qwords). False when there is no palette.
    // VU1 data memory as the palette leaves it (1024 qwords, zero where it
    // wrote nothing). Only the qwords the last palette wrote are cleared
    // again (was the whole 16 KB per skinned instance).
    struct VuPalette
    {
        std::vector<float> vu = std::vector<float>(1024u * 4u, 0.0f);
        uint32_t lo = 0, hi = 0; // qwords written by the last palette
        const float *data() const { return vu.data(); }
        const float &operator[](size_t i) const { return vu[i]; }
    };
    // SOTC_STALE_CHECK=1 (debugging, round 12, the parts desync): the guest
    // memory each model draw read (its matrix, its bone palette) is hashed
    // when the hook reads it and again when the frame's display list goes to
    // the VIF1 (ps2_stats::kickHook, what the PS2's DMA reads) and at the
    // next frame's first hook: a region that changed in between was drawn
    // with other data than the console's.
    struct StaleRegion
    {
        uint32_t at, bytes, instance;
        uint8_t kind; // 0 matrix, 1 palette, 2 cloth buffer read, 3 the cloth's other buffer
        uint64_t hash;
    };
    std::vector<StaleRegion> g_staleRegions, g_stalePrevious; // this frame's, the frame before's
    bool g_staleCheck = std::getenv("SOTC_STALE_CHECK") != nullptr;
    uint64_t staleHash(uint32_t at, uint32_t bytes)
    {
        uint64_t h = 1469598103934665603ull;
        for (uint32_t i = 0; i < bytes; ++i)
            h = (h ^ g_ram[(at + i) & 0x1FFFFFFu]) * 1099511628211ull;
        return h;
    }
    void staleRecord(uint32_t at, uint32_t bytes, uint32_t instance, uint8_t kind)
    {
        if (g_staleRegions.size() < 4096u)
            g_staleRegions.push_back({at, bytes, instance, kind, staleHash(at, bytes)});
    }
    void staleCompare(const char *when, const std::vector<StaleRegion> &regions);
    void staleCompare(const char *when) { staleCompare(when, g_staleRegions); }
    void staleCompare(const char *when, const std::vector<StaleRegion> &regions)
    {
        static const char *kinds[4] = {"matrix", "palette", "cloth", "cloth-other"};
        const int w = when[0] == 'k' ? (when[4] == '2' ? 2 : 0) : 1;
        static uint64_t frames[3] = {}, checked[3][4] = {}, changed[3][4] = {}, framesChanged[3] = {}, told = 0;
        uint32_t now = 0;
        for (const StaleRegion &r : regions)
        {
            ++checked[w][r.kind];
            if (staleHash(r.at, r.bytes) != r.hash)
            {
                ++changed[w][r.kind], ++now;
                if (w != 1 && r.kind != 1 && told++ < 200u)
                    std::fprintf(stderr, "[stale] frame %llu at %s: %s %08x (%u bytes) instance %08x object %08x header %08x\n", (unsigned long long)g_frame, when,
                                 kinds[r.kind], r.at, r.bytes, r.instance, word(r.instance), word(word(r.instance) + 0x20u));
            }
        }
        if (now)
            ++framesChanged[w];
        if (++frames[w] % 600u == 0u)
        {
            std::fprintf(stderr, "[stale] %s: %llu frames, %llu with changes;", when, (unsigned long long)frames[w], (unsigned long long)framesChanged[w]);
            for (int k = 0; k < 4; ++k)
                std::fprintf(stderr, " %s %llu / %llu", kinds[k], (unsigned long long)changed[w][k], (unsigned long long)checked[w][k]);
            std::fprintf(stderr, "\n");
        }
    }
    // SOTC_VIEW_CHECK=1 (debugging, round 12): a draw's matrix as the
    // native path makes it (the camera now x the model) against what the
    // console's VU1 gets (the projection x the matrix the game put in the
    // instance's chain, or in the cloth buffer, when it made them).
    void viewCheck(int kind, const float *mine, const float *view)
    {
        static const bool on = std::getenv("SOTC_VIEW_CHECK") != nullptr;
        if (!on || !g_cameraContext)
            return;
        float projection[16], theirs[16];
        for (int i = 0; i < 16; ++i)
            projection[i] = real(g_cameraContext + 0x40u + i * 4u);
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                    sum += projection[k * 4 + r] * view[c * 4 + k];
                theirs[c * 4 + r] = sum;
            }
        float diff = 0.0f, scale = 1e-6f;
        for (int i = 0; i < 16; ++i)
            diff = std::max(diff, std::fabs(mine[i] - theirs[i])), scale = std::max(scale, std::fabs(theirs[i]));
        const float rel = diff / scale;
        static uint64_t n[2] = {}, over[2] = {}, lastFrame = 0;
        static float worst[2] = {};
        ++n[kind];
        if (rel > 1e-3f)
            ++over[kind];
        worst[kind] = std::max(worst[kind], rel);
        if (g_frame / 100u != lastFrame / 100u)
        {
            std::fprintf(stderr, "[view] frame %llu: rigid %llu (%llu differ, worst %.4f) cloth %llu (%llu differ, worst %.4f)\n", (unsigned long long)g_frame,
                         (unsigned long long)n[0], (unsigned long long)over[0], worst[0], (unsigned long long)n[1], (unsigned long long)over[1], worst[1]);
            n[0] = n[1] = over[0] = over[1] = 0;
            worst[0] = worst[1] = 0.0f;
        }
        lastFrame = g_frame;
    }
    bool readChainPalette(uint32_t own, VuPalette &vu);
    bool readPalette(uint32_t instance, VuPalette &vu)
    {
        return readChainPalette(word(instance + 0xCu), vu);
    }
    bool readChainPalette(uint32_t own, VuPalette &palette)
    {
        if (((word(own) >> 28) & 7u) != 2u)
            return false;
        const uint32_t ret = word(own + 4u), qwc = word(ret) & 0xFFFFu;
        if (qwc <= 5u)
            return false;
        std::vector<float> &vu = palette.vu;
        if (palette.hi > palette.lo)
            std::fill(vu.begin() + palette.lo * 4u, vu.begin() + palette.hi * 4u, 0.0f);
        uint32_t lo = 1024u, hi = 0u;
        const uint32_t end = ret + 16u + qwc * 16u;
        for (uint32_t p = ret + 8u; p < end;)
        {
            const uint32_t code = word(p);
            p += 4;
            const uint32_t cmd = (code >> 24) & 0x7Fu;
            if ((cmd & 0x6Fu) != 0x6Cu)
                continue; // NOP / STCYCL / MSCAL: one word
            const uint32_t n = ((code >> 16) & 0xFFu) ? ((code >> 16) & 0xFFu) : 256u, at = code & 0x3FFu;
            const uint32_t last = std::min(at + n, 1024u);
            for (uint32_t q = at; q < last; ++q)
            {
                const uint32_t from = p + (q - at) * 16u;
                if ((from & 0x1FFFFFCu) + 16u <= 0x2000000u)
                    std::memcpy(&vu[q * 4u], g_ram + (from & 0x1FFFFFCu), 16);
                else
                    for (uint32_t k = 0; k < 4u; ++k)
                        vu[q * 4u + k] = real(from + k * 4u);
            }
            if (last > at)
            {
                lo = std::min(lo, at);
                hi = std::max(hi, last);
            }
            p += n * 16u;
        }
        palette.lo = lo;
        palette.hi = hi;
        return true;
    }


    // A skinned packet's strip vertices as the posing reads them: bind pose,
    // each bone as its matrix's offset in the palette (floats), the weights,
    // and what is copied as is (ST, colour).
    struct SkinBind
    {
        float x, y, z, nx, ny, nz;
        float weight[3];
        uint16_t at[3];
        uint8_t bones;
        float u, v;         // as native_renderer::Vertex has them:
        uint8_t r, g, b, a; // u, v, then the colour
    };
    static_assert(offsetof(SkinBind, r) == offsetof(SkinBind, u) + 8u && offsetof(native_renderer::Vertex, u) == 24u &&
                      offsetof(native_renderer::Vertex, r) == 32u && sizeof(native_renderer::Vertex) == 36u,
                  "poseSkinned copies u, v and the colour as 12 bytes");
    struct SkinnedPacket
    {
        std::vector<Strip> strips;      // bind pose
        std::vector<SkinBind> bind;     // the strips' vertices, one strip after another
        std::vector<uint32_t> stripEnd; // where each strip ends in `bind`
        uint32_t maxVertices = 0;       // 3 x the strips' triangles (joins included)
        // Drawn lit (lit(strips), g_litPackets). Unlit draws do not read the
        // normals (the vertex programs use them only for lighting): they are
        // left at 0 instead of posed.
        bool lit = false;
        // Posed by the GPU when it can be (native_renderer::gpuSkinning): its
        // matrices within kSkinPaletteQwords of each other (qwords lo..hi of
        // the palette), and no lit vertex without a normal (those take each
        // triangle's own normal).
        bool gpu = false;
        uint32_t lo = 0, hi = 0;
        uint64_t used = 0;              // the last frame (g_frame) that drew it
        uint64_t bytes = 0;             // held here (the [gl:mem] log)
    };

    SkinnedPacket makeSkinnedPacket(std::vector<Strip> &&strips)
    {
        SkinnedPacket packet;
        for (const Strip &strip : strips)
        {
            for (const BatchVertex &v : strip)
            {
                SkinBind b{};
                b.x = v.x, b.y = v.y, b.z = v.z;
                b.nx = v.nx, b.ny = v.ny, b.nz = v.nz;
                b.bones = v.bones;
                for (uint32_t k = 0; k < v.bones; ++k)
                {
                    b.weight[k] = v.weight[k];
                    b.at[k] = static_cast<uint16_t>((v.bone[k] * 4u % 1024u) * 4u);
                }
                b.u = v.u, b.v = v.v;
                b.r = v.r, b.g = v.g, b.b = v.b, b.a = v.a;
                packet.bind.push_back(b);
            }
            packet.stripEnd.push_back(static_cast<uint32_t>(packet.bind.size()));
            if (strip.size() > 2u)
                packet.maxVertices += static_cast<uint32_t>(strip.size() - 2u) * 3u;
        }
        packet.lit = lit(strips);
        uint32_t lo = ~0u, hi = 0u;
        bool noNormal = false;
        for (const SkinBind &b : packet.bind)
        {
            for (uint32_t k = 0; k < b.bones; ++k)
            {
                lo = std::min<uint32_t>(lo, b.at[k] / 4u);
                hi = std::max<uint32_t>(hi, b.at[k] / 4u + 4u);
            }
            noNormal = noNormal || (b.nx == 0.0f && b.ny == 0.0f && b.nz == 0.0f);
        }
        packet.lo = lo == ~0u ? 0u : lo;
        packet.hi = std::max(hi, packet.lo);
        packet.gpu = (!packet.lit || !noNormal) && packet.hi - packet.lo <= native_renderer::kSkinPaletteQwords;
        packet.strips = std::move(strips);
        packet.bytes = packet.strips.capacity() * sizeof(Strip) + packet.bind.capacity() * sizeof(SkinBind) + packet.stripEnd.capacity() * 4u;
        for (const Strip &strip : packet.strips)
            packet.bytes += strip.capacity() * sizeof(BatchVertex);
        return packet;
    }

    // A skinned packet's bind pose for the GPU (native_renderer::uploadSkinnedMesh):
    // the strip vertices, and the strips' triangles as indices (those with two
    // corners the same vertex left out: no matrix pulls them apart).
    void uploadSkinned(uint64_t id, const SkinnedPacket &packet)
    {
        static std::vector<native_renderer::SkinVertex> vertices;
        static std::vector<uint32_t> indices;
        vertices.resize(packet.bind.size());
        for (size_t i = 0; i < packet.bind.size(); ++i)
        {
            const SkinBind &b = packet.bind[i];
            native_renderer::SkinVertex &o = vertices[i];
            o = {};
            o.x = b.x, o.y = b.y, o.z = b.z;
            o.nx = b.nx, o.ny = b.ny, o.nz = b.nz;
            o.u = b.u, o.v = b.v;
            o.r = b.r, o.g = b.g, o.b = b.b, o.a = b.a;
            for (uint32_t k = 0; k < b.bones; ++k)
            {
                o.bone[k] = static_cast<float>(b.at[k] / 4u - packet.lo);
                o.weight[k] = b.weight[k];
            }
        }
        const auto same = [&](uint32_t a, uint32_t b) { return std::memcmp(&packet.bind[a], &packet.bind[b], sizeof(SkinBind)) == 0; };
        indices.clear();
        uint32_t first = 0;
        for (const uint32_t end : packet.stripEnd)
        {
            for (uint32_t i = first + 2u; i < end; ++i)
                if (!same(i - 2u, i - 1u) && !same(i - 1u, i) && !same(i - 2u, i))
                    indices.insert(indices.end(), {i - 2u, i - 1u, i});
            first = end;
        }
        native_renderer::uploadSkinnedMesh(id, vertices.data(), static_cast<uint32_t>(vertices.size()), indices.data(), static_cast<uint32_t>(indices.size()));
    }

    // Poses a skinned packet (camera space) into `out` (room for
    // max(bind.size(), maxVertices)): the triangles triangulate() with the
    // posing lambda of modelDispHook gives, each strip vertex posed once.
    // Written as the strip vertices plus indices (a third of the vertices to
    // write and to upload); as the triangle list itself when a lit packet
    // has a posed normal of 0 (that vertex takes each triangle's own face
    // normal, so it cannot be shared). The vertices keep the same values
    // either way: the same picture.
    struct Posed
    {
        float p[3], n[3];
    };
    struct PosedMesh
    {
        uint32_t vertices = 0, indices = 0;
        const uint32_t *index = nullptr; // indexed: the indices (from `base`)
        uint32_t base = 0;
    };
    PosedMesh poseSkinned(const SkinnedPacket &packet, const float *palette, native_renderer::Vertex *out)
    {
        static std::vector<Posed> posed;
        const size_t count = packet.bind.size();
        if (posed.size() < count)
            posed.resize(count);
        const bool lit = packet.lit;
        uint32_t zeroNormals = 0;
        for (size_t i = 0; i < count; ++i)
        {
            const SkinBind &v = packet.bind[i];
            float x = 0.0f, y = 0.0f, z = 0.0f, nx = 0.0f, ny = 0.0f, nz = 0.0f;
            if (v.bones == 1u)
            {
                // One bone, weight 1: 0 + 1 x t is t (the sign of a zero aside).
                const float *m = palette + v.at[0];
                x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
                y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
                z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
                if (lit)
                {
                    nx = m[0] * v.nx + m[4] * v.ny + m[8] * v.nz;
                    ny = m[1] * v.nx + m[5] * v.ny + m[9] * v.nz;
                    nz = m[2] * v.nx + m[6] * v.ny + m[10] * v.nz;
                }
            }
            else
            for (uint32_t b = 0; b < v.bones; ++b)
            {
                const float *m = palette + v.at[b];
                const float w = v.weight[b];
                x += w * (m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12]);
                y += w * (m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13]);
                z += w * (m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14]);
                if (!lit)
                    continue;
                nx += w * (m[0] * v.nx + m[4] * v.ny + m[8] * v.nz);
                ny += w * (m[1] * v.nx + m[5] * v.ny + m[9] * v.nz);
                nz += w * (m[2] * v.nx + m[6] * v.ny + m[10] * v.nz);
            }
            Posed &o = posed[i];
            o.p[0] = x, o.p[1] = y, o.p[2] = z;
            o.n[0] = nx, o.n[1] = ny, o.n[2] = nz;
            if (lit)
                zeroNormals |= static_cast<uint32_t>(nx == 0.0f && ny == 0.0f && nz == 0.0f);
        }
        // A triangle's (doubled) area: degenerate ones (strip joins) are left
        // out, as triangulate() does (its sqrt of this is not > 0 either).
        const auto cross = [](const Posed &a, const Posed &b, const Posed &c, float n[3])
        {
            const float ux = b.p[0] - a.p[0], uy = b.p[1] - a.p[1], uz = b.p[2] - a.p[2];
            const float vx = c.p[0] - a.p[0], vy = c.p[1] - a.p[1], vz = c.p[2] - a.p[2];
            n[0] = uy * vz - uz * vy, n[1] = uz * vx - ux * vz, n[2] = ux * vy - uy * vx;
            return n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
        };
        const auto put = [&](native_renderer::Vertex &o, size_t i, const float *normal)
        {
            const Posed &p = posed[i];
            const SkinBind &src = packet.bind[i];
            o.x = p.p[0], o.y = p.p[1], o.z = p.p[2];
            o.nx = normal[0], o.ny = normal[1], o.nz = normal[2];
            o.u = src.u, o.v = src.v;
            o.r = src.r, o.g = src.g, o.b = src.b, o.a = src.a;
        };
        PosedMesh mesh;
        uint32_t first = 0;
        uint32_t base = 0, *indices = zeroNormals ? nullptr : native_renderer::frameMeshIndices(packet.maxVertices, base);
        if (indices)
        {
            static const float none[3] = {0.0f, 0.0f, 0.0f}; // unlit: not read
            for (size_t i = 0; i < count; ++i)
                put(out[i], i, lit ? posed[i].n : none);
            mesh.vertices = static_cast<uint32_t>(count);
            for (const uint32_t end : packet.stripEnd)
            {
                for (uint32_t i = first + 2u; i < end; ++i)
                {
                    float n[3];
                    if (!(cross(posed[i - 2u], posed[i - 1u], posed[i], n) > 0.0f))
                        continue;
                    indices[mesh.indices++] = base + i - 2u;
                    indices[mesh.indices++] = base + i - 1u;
                    indices[mesh.indices++] = base + i;
                }
                first = end;
            }
            mesh.index = indices;
            mesh.base = base;
            return mesh;
        }
        for (const uint32_t end : packet.stripEnd)
        {
            for (uint32_t i = first + 2u; i < end; ++i)
            {
                float n[3];
                const float squared = cross(posed[i - 2u], posed[i - 1u], posed[i], n);
                if (!(squared > 0.0f))
                    continue;
                float face[3];
                bool haveFace = false;
                for (uint32_t k = i - 2u; k <= i; ++k)
                {
                    const float *normal = posed[k].n;
                    if (normal[0] == 0.0f && normal[1] == 0.0f && normal[2] == 0.0f) // unlit: all 0, so the face's
                    {
                        if (!haveFace)
                        {
                            const float len = std::sqrt(squared);
                            face[0] = n[0] / len, face[1] = n[1] / len, face[2] = n[2] / len;
                            haveFace = true;
                        }
                        normal = face;
                    }
                    put(out[mesh.vertices++], k, normal);
                }
            }
            first = end;
        }
        return mesh;
    }
    std::unordered_map<uint64_t, SkinnedPacket> g_skinnedPackets; // by packet
    uint64_t g_skinnedPacketBytes = 0;                               // what they hold
    native_renderer::IdSet g_litPackets;                              // packets with normals
    native_renderer::IdSet g_skinnedIds;                              // g_skinnedPackets' keys (most packets are not: no map lookup for them)
    uint64_t g_skinnedDraws = 0;                                       // this frame's skinned meshes

    // ---- Skinned parts as the display list's DMA reads them (round 12) -----
    //
    // A skinned instance's chain (instance + 0xc) ends in a CALL to its bone
    // palette's RET packet, and several instances of one character can share
    // that chain (Wander's models: one palette packet rewritten between their
    // draws). The console's DMA reads it when the list goes to the VIF1, after
    // the whole walk: every instance on a shared chain is drawn with the last
    // matrices written there. The hook read them at the instance's draw, so
    // a part drawn before the last write was posed from other matrices than
    // the rest of the character: invisible at 60 fps (the same pose, a frame
    // apart), parts from different poses at 11 fps (the parts desync). Now
    // each skinned draw is recorded with the palette it used, and at the
    // kick (ps2_stats::kickHook) the chains are read again; a draw whose
    // palette changed is posed again (the same per-frame mesh id: the later
    // upload wins) or its GPU palette rewritten. SOTC_SKIN_AT_KICK=0: off.
    struct SkinRecord
    {
        uint32_t own = 0, lo = 0, hi = 0;
        std::vector<float> used; // qwords lo..hi of the palette as posed
    };
    struct SkinDraw
    {
        uint32_t record = 0;
        const SkinnedPacket *packet = nullptr;
        uint64_t mesh = 0;    // posed on the CPU: its per-frame mesh
        uint32_t handle = 0;  // posed by the GPU: its frame palette
        uint32_t lo = 0, count = 0;
    };
    std::vector<SkinRecord> g_skinRecords;
    std::vector<SkinDraw> g_skinDrawList;
    const bool g_skinAtKick = []
    {
        const char *env = std::getenv("SOTC_SKIN_AT_KICK");
        return !(env && std::atoi(env) == 0);
    }();
    struct SkinKickStats
    {
        uint64_t kicks = 0, records = 0, changed = 0, reposed = 0, rewritten = 0;
    } g_skinKickStats;

    void skinAtKick()
    {
        if (g_skinDrawList.empty())
            return;
        static VuPalette now;
        SkinKickStats &st = g_skinKickStats;
        ++st.kicks;
        for (uint32_t r = 0; r < g_skinRecords.size(); ++r)
        {
            SkinRecord &rec = g_skinRecords[r];
            ++st.records;
            if (!readChainPalette(rec.own, now))
                continue;
            if (now.lo == rec.lo && now.hi == rec.hi &&
                std::memcmp(now.data() + rec.lo * 4u, rec.used.data(), rec.used.size() * sizeof(float)) == 0)
                continue;
            ++st.changed;
            rec.lo = now.lo;
            rec.hi = now.hi;
            rec.used.assign(now.data() + now.lo * 4u, now.data() + now.hi * 4u);
            for (const SkinDraw &d : g_skinDrawList)
            {
                if (d.record != r)
                    continue;
                if (d.handle)
                {
                    native_renderer::rewriteFramePalette(d.handle, now.data() + d.lo * 4u, d.count);
                    ++st.rewritten;
                }
                else if (d.mesh)
                {
                    const SkinnedPacket &packet = *d.packet;
                    if (native_renderer::Vertex *out = native_renderer::frameMesh(std::max<uint32_t>(packet.maxVertices, static_cast<uint32_t>(packet.bind.size()))))
                    {
                        const PosedMesh written = poseSkinned(packet, now.data(), out);
                        native_renderer::commitFrameMesh(d.mesh, written.vertices, written.indices);
                        ++st.reposed;
                    }
                }
            }
        }
        static const bool log = std::getenv("SOTC_SKIN_KICK_LOG") != nullptr;
        if (log && st.kicks % 600u == 0u)
            std::fprintf(stderr, "[skin-kick] %llu kicks: %llu instance palettes, %llu changed after their draws: %llu parts posed again, %llu GPU palettes rewritten\n",
                         (unsigned long long)st.kicks, (unsigned long long)st.records, (unsigned long long)st.changed, (unsigned long long)st.reposed,
                         (unsigned long long)st.rewritten);
    }

    // ---- Draw plans (round 8) --------------------------------------------
    //
    // A model's packet list (header +0x60, 32 bytes a packet, every LOD one
    // after the other) walked once, not twice a draw (modelDispState and the
    // hook): for each (header, LOD) the packets of that LOD in their order
    // with their part, surface and data, and what the hook found out about
    // each (its id, skinned or not, its meshes there, lit). Kept across
    // frames. What is still read per draw: the part tree's results, the
    // surfaces' flags / textures / scroll (the per-frame surface memo), the
    // instance's matrix, opacity, lights and palette.
    //
    // Invalidation: each frame a plan is used, the header's packet list and
    // surface table pointers and the list's first packet are compared; one
    // frame in 16 (by header) the whole list (size, LOD, surface, part, data
    // of every packet) is hashed again; a header that moved (an area's heap
    // compaction) or a LOD switch is another plan. A packet's id is taken
    // again from packetId whenever packetId itself would look at the packet
    // (not drawn the frame before, its stagger frame, or a packet seen
    // rewritten in place: every frame), so the ids are packetId's.
    // The meshes' presence is asked again after the backend lets meshes go
    // (meshEvictions) and the skinned packets' after their sweep.
    // SOTC_PLAN=0: off (the packet walk as before). SOTC_PLAN_VERIFY=<n>:
    // every n-th frame each draw's packets go through both paths, the old
    // one dry (hashed by the draw tap, not drawn), and the draw hashes are
    // compared ([plan-verify] lines).
    struct PlanPacket
    {
        uint32_t data = 0, size = 0, part = 0;
        uint32_t surface = 0, s = 0, slot = 0; // the surface: index, record, g_surfaceInfo slot
        uint64_t id = 0, idFrame = ~0ull;      // the packet's id, the frame it was last taken
        SkinnedPacket *skinned = nullptr;
        bool rewritten = false; // packetId saw it rewritten in place: asked every frame
        bool resolved = false;  // skinned / ready / lit known for `id`
        bool ready = false, readyTwo = false, lit = false;
    };
    struct ModelPlan
    {
        uint32_t header = 0, lod = 0, packets = 0, surfaces = 0, count = 0;
        uint32_t first[5] = {};
        uint64_t listHash = 0, checked = ~0ull, used = 0, meshEpoch = 0, skinEpoch = 0;
        std::vector<PlanPacket> pk;
    };
    std::unordered_map<uint64_t, ModelPlan> g_plans;
    int32_t g_planBudget = 0; // packets the frame's new plans may still walk (SOTC_PLAN_BUDGET, default 3000)
    uint64_t g_skinnedEpoch = 0; // g_skinnedPackets lost an entry (their sweep)
    struct PlanStats
    {
        uint64_t builds = 0, rebuilds = 0, uses = 0, asked = 0, trusted = 0, checks = 0, draws = 0, differ = 0, countDiffer = 0, ranges = 0, drawn = 0, merged = 0, deferred = 0;
    } g_planStats;
    // A run of one surface's packets of one draw, drawn as one (drawMulti):
    // the state of its first packet, the meshes of all (round 8). The packets
    // after the first are taken only when their draw would be the same: the
    // same surface record (its flags, texture, layer ... this frame), lit or
    // not alike, a plain mesh already there, one draw a packet (no second
    // texture, no reflection passes). Flushed before any other draw.
    struct PlanRun
    {
        uint32_t n = 0, s = 0;
        bool lit = false;
        const float *transform = nullptr;
        native_renderer::DrawState state;
        std::vector<uint64_t> ids;
        void flush()
        {
            if (!n)
                return;
            native_renderer::drawMulti(ids.data(), n, transform, state);
            g_planStats.ranges += n > 1u;
            g_planStats.merged += n - 1u;
            n = 0;
            ids.clear();
        }
    } g_planRun;
    bool g_planRuns = true; // SOTC_PLAN_RUNS=0: one draw a packet
    bool g_planStatsOn = false; // SOTC_PLAN_STATS=1 (set once a frame: no guard per packet)
    inline bool planStatsOn() { return g_planStatsOn; }
    inline uint64_t planMix(uint64_t h, uint32_t v) { return (h ^ v) * 0x100000001b3ull; }
    // The list's size / LOD / surface / part / data words, every packet.
    uint64_t packetListHash(uint32_t packets, uint32_t &count)
    {
        uint64_t h = 0xcbf29ce484222325ull;
        count = 0;
        for (uint32_t pk = packets; count < 0x10000u; pk += 32u)
        {
            const uint32_t size = word(pk);
            if (size == 0u)
                break;
            ++count;
            for (uint32_t i = 0; i < 20u; i += 4u)
                h = planMix(h, word(pk + i));
        }
        return h;
    }
    void buildPlan(ModelPlan &p, uint32_t header, uint32_t lod)
    {
        p.header = header;
        p.lod = lod;
        p.packets = word(header + 0x60u);
        p.surfaces = word(header + 0x50u);
        p.pk.clear();
        uint64_t h = 0xcbf29ce484222325ull;
        uint32_t count = 0;
        for (uint32_t pk = p.packets; count < 0x10000u; pk += 32u)
        {
            const uint32_t size = word(pk);
            if (size == 0u)
                break;
            ++count;
            for (uint32_t i = 0; i < 20u; i += 4u)
                h = planMix(h, word(pk + i));
            if (word(pk + 4u) != lod)
                continue;
            PlanPacket e;
            e.size = size;
            e.surface = word(pk + 8u);
            e.part = word(pk + 12u);
            e.data = word(pk + 16u);
            e.s = p.surfaces + e.surface * 0x120u;
            e.slot = surfaceSlot(e.s);
            p.pk.push_back(e);
        }
        p.count = count;
        p.listHash = h;
        for (uint32_t i = 0; i < 5u; ++i)
            p.first[i] = word(p.packets + i * 4u);
        p.meshEpoch = native_renderer::meshEvictions();
        p.skinEpoch = g_skinnedEpoch;
    }
    // The plan of (header, LOD), checked once a frame; nullptr: none (LOD out of range).
    ModelPlan *modelPlan(uint32_t header, uint32_t lod)
    {
        if (lod >= 8u)
            return nullptr;
        const auto [it, fresh] = g_plans.try_emplace((static_cast<uint64_t>(header) << 3) | lod);
        ModelPlan &p = it->second;
        p.used = g_frame;
        // Builds spread over frames (an area load brings hundreds of new
        // models at once): past this frame's budget of packets a model is
        // drawn by the packet walk, its plan made at a later frame.
        if ((fresh || p.checked != g_frame) && g_planBudget <= 0)
        {
            if (fresh)
            {
                g_plans.erase(it);
                ++g_planStats.deferred;
                return nullptr;
            }
        }
        if (fresh)
        {
            buildPlan(p, header, lod);
            g_planBudget -= static_cast<int32_t>(p.count) + 16;
            p.checked = g_frame;
            ++g_planStats.builds;
            return &p;
        }
        if (p.checked == g_frame)
            return &p;
        p.checked = g_frame;
        bool ok = p.packets == word(header + 0x60u) && p.surfaces == word(header + 0x50u);
        for (uint32_t i = 0; ok && i < 5u; ++i)
            ok = p.first[i] == word(p.packets + i * 4u);
        if (ok && ((g_frame + (header >> 5)) & 15u) == 0u)
        {
            uint32_t count = 0;
            ok = packetListHash(p.packets, count) == p.listHash && count == p.count;
        }
        if (!ok)
        {
            if (g_planBudget <= 0)
            {
                p.checked = ~0ull; // looked at again next time
                ++g_planStats.deferred;
                return nullptr;
            }
            buildPlan(p, header, lod);
            g_planBudget -= static_cast<int32_t>(p.count) + 16;
            ++g_planStats.rebuilds;
            return &p;
        }
        const uint64_t evictions = native_renderer::meshEvictions();
        if (p.meshEpoch != evictions || p.skinEpoch != g_skinnedEpoch)
        {
            p.meshEpoch = evictions;
            p.skinEpoch = g_skinnedEpoch;
            for (PlanPacket &e : p.pk)
                e.resolved = false;
        }
        return &p;
    }
    // Plans no draw used for 10 s go (every 256 frames).
    void sweepPlans()
    {
        for (auto it = g_plans.begin(); it != g_plans.end();)
            if (it->second.used + 600u < g_frame)
                it = g_plans.erase(it);
            else
                ++it;
    }


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

    // The frame timing log: every 60 frames the average and the slowest
    // frame (ms), split into the game's own work (what is left), the native
    // hooks (decoding and uploads included), the backend's CPU side and the
    // host's swap (with the render thread: that thread's, overlapping the
    // game; "wait" is the game thread's wait for it); and each frame over
    // 50 ms on its own line (hitches).
    void frameLog()
    {
        native_renderer::FrameCounters &c = native_renderer::frameCounters();
        static uint64_t last = 0;
        const uint64_t now = native_renderer::nowUs();
        if (!native_renderer::tuning().frameLog)
        {
            last = 0;
            c = {};
            return;
        }
        struct Sum
        {
            uint64_t frames = 0, period = 0, worst = 0, hooks = 0, decode = 0, tex = 0, mesh = 0, render = 0, present = 0, wait = 0, blocked = 0, threaded = 0;
            uint64_t draws = 0, targets = 0, texUploads = 0, texBytes = 0, meshUploads = 0, meshBytes = 0;
        };
        static Sum sum;
        // The game thread's scheduler waits (idle, VBlank pacing, a guest
        // thread blocked), the disc reads, the guest VBlanks: this frame's.
        struct Mark
        {
            uint64_t wait, cdUs, cdBytes, cdReads, vblanks, sprUs, sprBytes, sprs;
        };
        const Mark mark{ps2_load::waitUs[ps2_load::kGame].load(std::memory_order_relaxed), ps2_stats::cdReadUs.load(std::memory_order_relaxed),
                        ps2_stats::cdReadBytes.load(std::memory_order_relaxed), ps2_stats::cdReads.load(std::memory_order_relaxed),
                        ps2_stats::vblanks.load(std::memory_order_relaxed), ps2_stats::sprDmaUs.load(std::memory_order_relaxed),
                        ps2_stats::sprDmaBytes.load(std::memory_order_relaxed), ps2_stats::sprDmas.load(std::memory_order_relaxed)};
        static Mark lastMark = mark;
        // The game's own step this frame (GAMECORE's frame time, 0x1465650:
        // +0x180 seconds, from the EE timer T0): what its movement, physics
        // and animation advance by (round 10, the clock).
        float gameDt = 0.0f;
        if (g_ram)
            std::memcpy(&gameDt, g_ram + 0x014657D0u, 4);
        if (last)
        {
            const uint64_t period = now - last;
            if (period > 50000u)
                std::fprintf(stderr, "[frame] slow %llu: %.1f ms: hooks %.1f (decode %.1f, %u textures %.0f KB, up %.1f; %u meshes %.0f KB, up %.1f) render %.1f present %.1f wait %.1f draws %u targets %u async %u/%u held %u"
                                     " | sched %.1f cd %.1f (%llu reads %.0f KB) vb %llu dt %.1f spr %.1f (%llu, %.0f KB)\n",
                             (unsigned long long)g_frame, period / 1000.0, c.hookUs / 1000.0, c.decodeUs / 1000.0, c.texUploads, c.texBytes / 1024.0, c.texUs / 1000.0,
                             c.meshUploads, c.meshBytes / 1024.0, c.meshUs / 1000.0, c.renderUs / 1000.0, c.presentUs / 1000.0, c.waitUs / 1000.0, c.draws, c.targets,
                             c.asyncJobs, c.asyncDone, c.heldDraws, (mark.wait - lastMark.wait) / 1000.0, (mark.cdUs - lastMark.cdUs) / 1000.0,
                             (unsigned long long)(mark.cdReads - lastMark.cdReads), (mark.cdBytes - lastMark.cdBytes) / 1024.0,
                             (unsigned long long)(mark.vblanks - lastMark.vblanks), gameDt * 1000.0, (mark.sprUs - lastMark.sprUs) / 1000.0,
                             (unsigned long long)(mark.sprs - lastMark.sprs), (mark.sprBytes - lastMark.sprBytes) / 1024.0);
            // SOTC_FRAME_LOG_ALL=1: every frame on a line of its own (cuts, loads).
            static const bool all = std::getenv("SOTC_FRAME_LOG_ALL") != nullptr;
            if (all && period <= 50000u)
                std::fprintf(stderr, "[frame] each %llu: %.1f ms: hooks %.1f (decode %.1f, %u textures, %u meshes, up %.1f) render %.1f wait %.1f draws %u async %u/%u held %u | sched %.1f cd %.1f (%llu reads %.0f KB) vb %llu dt %.1f spr %.1f (%llu, %.0f KB)\n",
                             (unsigned long long)g_frame, period / 1000.0, c.hookUs / 1000.0, c.decodeUs / 1000.0, c.texUploads, c.meshUploads,
                             (c.texUs + c.meshUs) / 1000.0, c.renderUs / 1000.0, c.waitUs / 1000.0, c.draws, c.asyncJobs, c.asyncDone, c.heldDraws,
                             (mark.wait - lastMark.wait) / 1000.0, (mark.cdUs - lastMark.cdUs) / 1000.0, (unsigned long long)(mark.cdReads - lastMark.cdReads),
                             (mark.cdBytes - lastMark.cdBytes) / 1024.0, (unsigned long long)(mark.vblanks - lastMark.vblanks), gameDt * 1000.0,
                             (mark.sprUs - lastMark.sprUs) / 1000.0, (unsigned long long)(mark.sprs - lastMark.sprs), (mark.sprBytes - lastMark.sprBytes) / 1024.0);
            ++sum.frames;
            sum.period += period;
            sum.worst = std::max(sum.worst, period);
            sum.hooks += c.hookUs;
            sum.decode += c.decodeUs;
            sum.tex += c.texUs;
            sum.mesh += c.meshUs;
            sum.render += c.renderUs;
            sum.present += c.presentUs;
            sum.wait += c.waitUs;
            // A render thread draws (and shows) the frame before while the
            // game runs: the game thread's own time is what it did not wait.
            sum.threaded += c.renderThread ? 1u : 0u;
            // Without one (Metal) render and present were the game thread's.
            sum.blocked += c.waitUs ? c.waitUs : c.renderUs + c.presentUs;
            sum.draws += c.draws;
            sum.targets += c.targets;
            sum.texUploads += c.texUploads;
            sum.texBytes += c.texBytes;
            sum.meshUploads += c.meshUploads;
            sum.meshBytes += c.meshBytes;
            if (sum.frames == 60u)
            {
                const double n = 60.0, game = (static_cast<double>(sum.period) - sum.hooks - sum.blocked) / n / 1000.0;
                std::fprintf(stderr, "[frame] %llu: %.1f ms (%.1f fps, slowest %.1f) | game %.1f hooks %.1f (decode %.2f tex up %.2f mesh up %.2f) render %.1f present %.1f wait %.1f%s | "
                             "draws %.0f targets %.1f textures %.2f (%.0f KB) meshes %.0f (%.0f KB)\n",
                             (unsigned long long)g_frame, sum.period / n / 1000.0, n * 1e6 / static_cast<double>(sum.period), sum.worst / 1000.0, game, sum.hooks / n / 1000.0,
                             sum.decode / n / 1000.0, sum.tex / n / 1000.0, sum.mesh / n / 1000.0, sum.render / n / 1000.0, sum.present / n / 1000.0,
                             sum.wait / n / 1000.0, sum.threaded ? " (render thread)" : "",
                             sum.draws / n, sum.targets / n, sum.texUploads / n, sum.texBytes / n / 1024.0, sum.meshUploads / n, sum.meshBytes / n / 1024.0);
                sum = Sum{};
            }
        }
        last = now;
        lastMark = mark;
        c = {};
    }

    // The sound continuity log (round 12; SOTC_FRAME_LOG=1 or SOTC_AUDIO_LOG=1,
    // ps2x/iop/audio_stats.h): every 5 s of host time one "[audio]" line with
    // the window's counts. Each frame it also looks at the loader's job queues
    // (manager_sub_0115E3E0 takes one job a game frame from five FIFO lists,
    // heads 0x12228b0 + 8 i, links at +0x2c, job type at +0; the sound
    // streams' reads are type 7, queued on list 0 by sheetMgrAdpcmIntLoad;
    // the running job at 0x118fe34): how many are waiting, and how long a
    // stream read waited to start (first seen queued -> gone from the lists).
    void audioLog()
    {
        namespace as = ps2x::iop::audio_stats;
        if (!as::level() || !g_ram)
            return;
        const uint64_t now = native_renderer::nowUs();
        struct Waiting
        {
            uint64_t since;
            bool seen;
        };
        static std::unordered_map<uint32_t, Waiting> waiting; // queued type-7 jobs by node address
        static uint64_t frames = 0, queueMax = 0, adpcmMax = 0, adpcmWaitMaxUs = 0, adpcmWaitUs = 0, adpcmJobs = 0, busyFrames = 0;
        ++frames;
        {
            for (auto &[node, w] : waiting)
                w.seen = false;
            uint64_t queued = 0, adpcm = 0;
            for (uint32_t list = 0; list < 5u; ++list)
            {
                uint32_t node = word(0x12228b0u + list * 8u);
                for (int guard = 0; node != 0u && guard < 256; ++guard)
                {
                    if ((node & 0xFE000003u) != 0u)
                        break;
                    ++queued;
                    if (word(node) == 7u)
                    {
                        ++adpcm;
                        auto [it, fresh] = waiting.try_emplace(node, Waiting{now, true});
                        it->second.seen = true;
                    }
                    node = word(node + 0x2cu);
                }
            }
            for (auto it = waiting.begin(); it != waiting.end();)
            {
                if (!it->second.seen)
                {
                    const uint64_t waited = now - it->second.since;
                    adpcmWaitMaxUs = std::max(adpcmWaitMaxUs, waited);
                    adpcmWaitUs += waited;
                    ++adpcmJobs;
                    it = waiting.erase(it);
                }
                else
                    ++it;
            }
            queueMax = std::max(queueMax, queued);
            adpcmMax = std::max(adpcmMax, adpcm);
            if (word(0x118fe34u) != 0u)
                ++busyFrames;
        }
        struct Totals
        {
            uint64_t status, gaps50, refills, halves, stale, staleSrc, stalls, stallUs, starts, stops, stopsStalled, keyOns, keyOffs, ends, retrig;
            uint64_t pcmStarts, pcmUnder, pcmStale, pcmBlocks, callbacks, cbFrames, cbLate, mixUs, iopReads, iopUs, otherReads, wakeUs, vblanks, cacheHits, cacheBytes;
        };
        auto take = []
        {
            auto l = [](const std::atomic<uint64_t> &a) { return a.load(std::memory_order_relaxed); };
            return Totals{l(as::status), l(as::statusGapsOver50), l(as::refills), l(as::halves), l(as::staleHalves), l(as::staleSources), l(as::stalls),
                          l(as::stallUs), l(as::streamStarts), l(as::streamStops), l(as::stopsWhileStalled), l(as::keyOns), l(as::keyOffs), l(as::dataEnds),
                          l(as::retriggers), l(as::pcmStarts), l(as::pcmUnderFrames), l(as::pcmStaleBlocks), l(as::pcmBlocks), l(as::callbacks),
                          l(as::callbackFrames), l(as::callbacksLate), l(as::mixUs), l(as::iopReads), l(as::iopReadUs), l(as::otherReads), l(as::wakeUs),
                          ps2_stats::vblanks.load(std::memory_order_relaxed), l(as::cacheHits), l(as::cacheBytes)};
        };
        static uint64_t last = now;
        static Totals before = take();
        // The VBlank thread's ticks (getVIntrCnt 0x1e1314) and the sound
        // ticks (soundTickProcCnt 0x1e1564): one each a guest VBlank.
        static uint32_t ticksBefore = word(0x1e1314u), soundBefore = word(0x1e1564u);
        if (now - last < 5000000u)
            return;
        const uint32_t ticks = word(0x1e1314u), sound = word(0x1e1564u);
        {
            static uint64_t v0 = 0, c0 = 0, vb0 = 0, mb0 = 0;
            const uint64_t v = as::voiceUs.load(std::memory_order_relaxed), c = as::coreUs.load(std::memory_order_relaxed),
                           vb = as::voiceBlocks.load(std::memory_order_relaxed), mb = as::mixBlocks.load(std::memory_order_relaxed);
            static uint64_t r0 = 0, l0 = 0;
            const uint64_t r = as::rpcUs.load(std::memory_order_relaxed), l = as::rpcLockUs.load(std::memory_order_relaxed);
            std::fprintf(stderr, "[audio-ticks] vblank thread %u sound %u | mix voices %.1f ms cores %.1f ms, %.1f voices playing | driver %.1f ms (lock wait %.1f)\n",
                         ticks - ticksBefore, sound - soundBefore, (v - v0) / 1000.0, (c - c0) / 1000.0, mb > mb0 ? double(vb - vb0) / double(mb - mb0) : 0.0,
                         (r - r0) / 1000.0, (l - l0) / 1000.0);
            v0 = v; c0 = c; vb0 = vb; mb0 = mb; r0 = r; l0 = l;
        }
        ticksBefore = ticks;
        soundBefore = sound;
        const Totals t = take();
        auto d = [&](uint64_t Totals::*f) { return static_cast<unsigned long long>(t.*f - before.*f); };
        auto mx = [](std::atomic<uint64_t> &a) { return a.exchange(0, std::memory_order_relaxed) / 1000.0; };
        const double secs = (now - last) / 1e6;
        const unsigned long long cb = d(&Totals::callbacks);
        std::fprintf(stderr,
                     "[audio] %.1f s frame %llu: frames %llu vb %llu | status %llu (gap max %.1f ms, %llu over 50) | streams start %llu stop %llu (%llu paused) pause %llu (%.0f ms, max %.0f)"
                     " halves %llu stale %llu src-stale %llu refills %llu (wait max %.1f ms) | voices on %llu off %llu ends %llu retrig %llu | pcm start %llu blocks %llu stale %llu under %llu"
                     " | out cb %llu (%.0f fr) late %llu gap max %.1f ms mix %.2f / max %.2f ms lock max %.2f | cd iop %llu (avg %.1f max %.1f ms) other %llu (max %.1f) wake avg %.2f max %.1f ms cache %llu (%.0f KB)"
                     " | jobs queued max %llu adpcm max %llu, adpcm waits %llu (avg %.0f max %.0f ms), loader busy %llu frames\n",
                     secs, (unsigned long long)g_frame, (unsigned long long)frames, d(&Totals::vblanks), d(&Totals::status), mx(as::statusGapMaxUs),
                     d(&Totals::gaps50), d(&Totals::starts), d(&Totals::stops), d(&Totals::stopsStalled), d(&Totals::stalls), d(&Totals::stallUs) / 1000.0,
                     mx(as::stallMaxUs), d(&Totals::halves), d(&Totals::stale), d(&Totals::staleSrc), d(&Totals::refills), mx(as::refillWaitMaxUs),
                     d(&Totals::keyOns), d(&Totals::keyOffs), d(&Totals::ends), d(&Totals::retrig), d(&Totals::pcmStarts), d(&Totals::pcmBlocks),
                     d(&Totals::pcmStale), d(&Totals::pcmUnder), cb, cb ? double(d(&Totals::cbFrames)) / cb : 0.0, d(&Totals::cbLate), mx(as::callbackGapMaxUs),
                     cb ? d(&Totals::mixUs) / 1000.0 / cb : 0.0, mx(as::mixMaxUs), mx(as::lockWaitMaxUs), d(&Totals::iopReads),
                     d(&Totals::iopReads) ? d(&Totals::iopUs) / 1000.0 / d(&Totals::iopReads) : 0.0, mx(as::iopReadMaxUs), d(&Totals::otherReads),
                     mx(as::otherReadMaxUs), (d(&Totals::iopReads) + d(&Totals::otherReads)) ? d(&Totals::wakeUs) / 1000.0 / (d(&Totals::iopReads) + d(&Totals::otherReads)) : 0.0,
                     mx(as::wakeMaxUs), d(&Totals::cacheHits), d(&Totals::cacheBytes) / 1024.0, (unsigned long long)queueMax, (unsigned long long)adpcmMax,
                     (unsigned long long)adpcmJobs, adpcmJobs ? adpcmWaitUs / 1000.0 / adpcmJobs : 0.0, adpcmWaitMaxUs / 1000.0, (unsigned long long)busyFrames);
        last = now;
        before = t;
        frames = queueMax = adpcmMax = adpcmWaitMaxUs = adpcmWaitUs = adpcmJobs = busyFrames = 0;
    }

    // PGO on the Vita: the VitaSDK's libgcov has no file I/O (built without
    // libc), so a build made with -fprofile-generate -fprofile-info-section=
    // gcov_info (-fno-profile-values -fno-profile-reorder-functions: its
    // value profilers are empty) writes its .gcda files here, through
    // __gcov_info_to_gcda, each to the name compiled in (ux0:...).
    // SOTC_GCOV_DUMP_FRAME=<n>: written at frame n and every n frames after
    // (the counts so far; the last write wins). No-op in other builds.
#ifdef PLATFORM_VITA
    extern "C"
    {
        struct gcov_info;
        extern const gcov_info *const __start_gcov_info[] __attribute__((weak));
        extern const gcov_info *const __stop_gcov_info[] __attribute__((weak));
        void __gcov_info_to_gcda(const gcov_info *, void (*)(const char *, void *), void (*)(const void *, unsigned, void *),
                                 void *(*)(unsigned, void *), void *) __attribute__((weak));
    }
    void gcovDump(uint64_t frame)
    {
        static const uint64_t every = std::getenv("SOTC_GCOV_DUMP_FRAME") ? std::strtoull(std::getenv("SOTC_GCOV_DUMP_FRAME"), nullptr, 10) : 0u;
        if (!every || frame % every != 0u || !__gcov_info_to_gcda || !__start_gcov_info)
            return;
        size_t files = 0;
        for (const gcov_info *const *info = __start_gcov_info; info != __stop_gcov_info; ++info)
        {
            std::FILE *file = nullptr;
            __gcov_info_to_gcda(
                *info,
                [](const char *name, void *arg)
                {
                    std::FILE *&f = *static_cast<std::FILE **>(arg);
                    std::string dir(name);
                    dir.resize(dir.find_last_of('/') == std::string::npos ? 0u : dir.find_last_of('/'));
                    if (!dir.empty())
                        mkdir(dir.c_str(), 0777);
                    f = std::fopen(name, "wb");
                },
                [](const void *data, unsigned size, void *arg)
                {
                    if (std::FILE *f = *static_cast<std::FILE **>(arg))
                        std::fwrite(data, 1, size, f);
                },
                [](unsigned size, void *) -> void * { return std::malloc(size); }, &file);
            if (file)
            {
                std::fclose(file);
                ++files;
            }
        }
        std::fprintf(stderr, "[gcov] frame %llu: %u .gcda files written\n", static_cast<unsigned long long>(frame), static_cast<unsigned>(files));
    }
#else
    void gcovDump(uint64_t) {}
#endif

    // A frame of the game ends when the display list goes to the VIF1.
    void newDrawOrder();
    void syncFrame(uint8_t *rdram)
    {
        g_ram = rdram;

        const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        if (frame != g_frame)
        {
            if (g_staleCheck)
            {
                staleCompare("next frame");
                g_stalePrevious.swap(g_staleRegions);
                g_staleRegions.clear();
            }
            endFrame();
            g_skinRecords.clear();
            g_skinDrawList.clear();
            frameLog();
            audioLog();
            // SOTC_FRAME_STALL_MS=<ms> (measuring, round 10): the game thread
            // sleeps that long at each frame's end from frame
            // SOTC_FRAME_STALL_FROM (default 0) on, so the Mac runs frames as
            // slow as the Vita's (the host clock's catch-up, the game's step).
            static const uint64_t stallMs = std::getenv("SOTC_FRAME_STALL_MS") ? std::strtoull(std::getenv("SOTC_FRAME_STALL_MS"), nullptr, 10) : 0u;
            static const uint64_t stallFrom = std::getenv("SOTC_FRAME_STALL_FROM") ? std::strtoull(std::getenv("SOTC_FRAME_STALL_FROM"), nullptr, 10) : 0u;
            if (stallMs && frame >= stallFrom)
                std::this_thread::sleep_for(std::chrono::milliseconds(stallMs));
            // SOTC_POS_LOG=<n> (measuring, round 10): every n game frames the
            // guest VBlank, the game step and where the player and the camera
            // are ([pos]): runs at other frame rates compared by game time.
            static const uint64_t posEvery = std::getenv("SOTC_POS_LOG") ? std::strtoull(std::getenv("SOTC_POS_LOG"), nullptr, 10) : 0u;
            if (posEvery && frame % posEvery == 0u)
            {
                auto w = [rdram](uint32_t a) { uint32_t v; std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4); return v; };
                auto f = [rdram](uint32_t a) { float v; std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4); return v; };
                float eye[3] = {0, 0, 0}, player[3] = {0, 0, 0};
                if (g_cameraContext)
                {
                    const uint32_t m = g_cameraContext + 0x5A0u;
                    for (int j = 0; j < 3; ++j)
                        eye[j] = -(f(m + 48u) * f(m + j * 16u) + f(m + 52u) * f(m + j * 16u + 4u) + f(m + 56u) * f(m + j * 16u + 8u));
                }
                const uint32_t index = w(0x01465900u) & 0xFFFFu;
                const uint32_t object = index ? w(0x0021B900u + index * 24u) : 0u;
                if (object && object < 0x2000000u) // the player character (type 9): its position at +0x170
                    for (int j = 0; j < 3; ++j)
                        player[j] = f(object + 0x170u + j * 4u);
                float dt;
                std::memcpy(&dt, rdram + 0x014657D0u, 4);
                std::fprintf(stderr, "[pos] frame %llu vb %llu dt %.1f player %.2f %.2f %.2f eye %.2f %.2f %.2f\n", (unsigned long long)frame,
                             (unsigned long long)ps2_stats::vblanks.load(std::memory_order_relaxed), dt * 1000.0f, player[0], player[1], player[2],
                             eye[0], eye[1], eye[2]);
            }
            sotc_prof::frameTick(frame);
            ps2_lean::frameTick(frame);
            sotc_decomp::abTick(frame);
            native_renderer::beginFrame();
            // SOTC_DRAW_DUMP_FRAMES=f,f,... (debugging): those frames' draws to
            // SOTC_DRAW_DUMP_DIR (default shots)/draws_<frame>.txt.
            static const std::vector<uint64_t> dumpFrames = []
            {
                std::vector<uint64_t> out;
                if (const char *env = std::getenv("SOTC_DRAW_DUMP_FRAMES"))
                    for (const char *p = env; *p;)
                    {
                        out.push_back(std::strtoull(p, const_cast<char **>(&p), 10));
                        if (*p)
                            ++p;
                    }
                return out;
            }();
            if (std::find(dumpFrames.begin(), dumpFrames.end(), frame) != dumpFrames.end())
            {
                const char *dir = std::getenv("SOTC_DRAW_DUMP_DIR");
                native_renderer::requestDrawDump(std::string(dir ? dir : "shots") + "/draws_" + std::to_string(frame) + ".txt");
            }
            if (HookStats &hs = hookStats(); hs.on && frame % 120u == 0u)
            {
                std::fprintf(stderr,
                             "[hookstats] frame %llu /120: instances %llu headers %zu packets %llu lod %llu draws %llu tex %llu (distinct %zu) decoded %llu "
                             "id same %llu prev %llu quick %llu sampled %llu hashed %llu nodes %llu surfaces %llu (distinct %zu)\n",
                             (unsigned long long)frame, (unsigned long long)hs.instances, hs.headers.size(), (unsigned long long)hs.packets,
                             (unsigned long long)hs.lodPackets, (unsigned long long)hs.draws, (unsigned long long)hs.textures, hs.textureSet.size(),
                             (unsigned long long)hs.decoded, (unsigned long long)hs.idSame, (unsigned long long)hs.idPrev, (unsigned long long)hs.idQuick,
                             (unsigned long long)hs.idSampled, (unsigned long long)hs.idHashed, (unsigned long long)hs.nodes, (unsigned long long)hs.surfaces,
                             hs.surfaceSet.size());
                const bool on = hs.on;
                hs = HookStats();
                hs.on = on;
            }
            if (HookStats &hs = hookStats(); hs.on)
                hs.headers.clear(), hs.surfaceSet.clear(), hs.textureSet.clear(); // distinct per frame: summed below as sizes
            g_frame = frame;
            newDrawOrder();
            {
                const auto envOn = [](const char *name)
                {
                    const char *v = std::getenv(name);
                    return !v || std::atoi(v) != 0;
                };
                static const bool r2 = envOn("SOTC_NATIVE_R2"), quickTextures = envOn("SOTC_TEX_QUICK"), surfaceMemo = envOn("SOTC_SURFACE_MEMO"),
                                  prefetch = envOn("SOTC_HOOK_PREFETCH"), plans = envOn("SOTC_PLAN"), planRuns = envOn("SOTC_PLAN_RUNS");
                static const uint32_t verify = std::getenv("SOTC_PLAN_VERIFY") ? static_cast<uint32_t>(std::atoi(std::getenv("SOTC_PLAN_VERIFY"))) : 0u;
                g_planOn = plans;
                static const int32_t budget = std::getenv("SOTC_PLAN_BUDGET") ? std::atoi(std::getenv("SOTC_PLAN_BUDGET")) : 3000;
                g_planBudget = budget;
                g_planRuns = planRuns;
                static const bool planStats = std::getenv("SOTC_PLAN_STATS") != nullptr;
                g_planStatsOn = planStats;
                g_planVerify = verify;
                g_r2 = r2;
                g_quickTextures = g_r2 && quickTextures;
                g_surfaceMemo = g_r2 && surfaceMemo;
                g_prefetch = g_r2 && prefetch;
            }
            gcovDump(frame);
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
                        g_skinnedIds.erase(it->first);
                        ++g_skinnedEpoch; // the draw plans find their skinned packets again
                        g_skinnedPacketBytes -= it->second.bytes;
                        it = g_skinnedPackets.erase(it);
                    }
                    else
                        ++it;
            native_renderer::noteFrontendMemory(g_skinnedPacketBytes);
            if (frame % 256u == 0u)
                sweepPlans();
            // SOTC_PLAN_STATS=1 / SOTC_PLAN_VERIFY: the draw plans, every 600 frames.
            static const bool planStats = std::getenv("SOTC_PLAN_STATS") != nullptr;
            if ((planStats || g_planVerify) && frame % 600u == 0u)
            {
                const PlanStats &ps = g_planStats;
                std::fprintf(stderr, "[plan] frame %llu: packets drawn %llu (stats on), %llu runs drawn as one: %llu draws fewer\n", (unsigned long long)frame,
                             (unsigned long long)ps.drawn, (unsigned long long)ps.ranges, (unsigned long long)ps.merged);
                std::fprintf(stderr, "[plan] frame %llu: plans %zu built %llu rebuilt %llu deferred %llu uses %llu ids asked %llu kept %llu; verify %llu checks %llu draws, %llu differ (%llu counts)\n",
                             (unsigned long long)frame, g_plans.size(), (unsigned long long)ps.builds, (unsigned long long)ps.rebuilds, (unsigned long long)ps.deferred,
                             (unsigned long long)ps.uses, (unsigned long long)ps.asked, (unsigned long long)ps.trusted, (unsigned long long)ps.checks,
                             (unsigned long long)ps.draws, (unsigned long long)ps.differ, (unsigned long long)ps.countDiffer);
            }
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
        // (x w, y w, w) -> clip (x w, y w, -, w): the backends take depth from w.
        static const float fromDepth[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
        state.screenDepthTest = state.screenCopy;
        native_renderer::draw(mesh, state.screenDepthTest ? fromDepth : identity, state);
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

    // The view depth of a GS Z (the camera's zA + zB / w, as the bloom's
    // threshold), for 2D draws tested against the picture's depth.
    float viewDepth(uint32_t z)
    {
        if (!g_cameraContext)
            return 1.0f;
        const float zA = real(g_cameraContext + 0x4A0u + 10u * 4u), zB = real(g_cameraContext + 0x4A0u + 14u * 4u);
        const float d = static_cast<float>(z) - zA;
        const float w = d > 0.0f ? zB / d : 1e6f;
        return std::isfinite(w) ? std::clamp(w, 0.11f, 1e6f) : 1e6f;
    }

    // z: the GS Z of XYZ2 / XYZ3 (32 bits) or XYZF2 / XYZF3 (24).
    void vertex2D(uint64_t xyz, bool kick, uint32_t z)
    {
        Assembler &a = g_2d;
        native_renderer::Vertex o{};
        o.x = ((static_cast<float>(xyz & 0xFFFFu) / 16.0f) - 2048.0f) / 256.0f;
        o.y = -((static_cast<float>((xyz >> 16) & 0xFFFFu) / 16.0f) - 2048.0f) / 224.0f;
        o.nz = 1.0f;
        if (a.header == kScreenCopy)
        {
            // The heat haze is depth tested (TEST ZTE, GEQUAL): (x w, y w, w).
            const float w = viewDepth(z);
            o.x *= w;
            o.y *= w;
            o.z = w;
        }
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
        case 0x04u: vertex2D(value, true, static_cast<uint32_t>(value >> 32) & 0xFFFFFFu); break; // XYZF2
        case 0x05u: vertex2D(value, true, static_cast<uint32_t>(value >> 32)); break;             // XYZ2
        case 0x0Cu: vertex2D(value, false, static_cast<uint32_t>(value >> 32) & 0xFFFFFFu); break; // XYZF3: no drawing kick
        case 0x0Du: vertex2D(value, false, static_cast<uint32_t>(value >> 32)); break;             // XYZ3
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
            // SOTC_FEEDBACK_LOG=1 (debugging, round 12): the frames that use it.
            static const bool feedbackLog = std::getenv("SOTC_FEEDBACK_LOG") != nullptr;
            if (feedbackLog)
            {
                static uint64_t lastLogged = ~0ull;
                if (g_frame != lastLogged)
                    std::fprintf(stderr, "[feedback] frame %llu ra %08x a0 %02x a1 %016llx\n", (unsigned long long)g_frame, ra, GPR_U32(ctx, 4) & 0xFFu,
                                 (unsigned long long)GPR_U64(ctx, 5));
                lastLogged = g_frame;
            }
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
        HookTimer hookTimer;
        if (!g_cameraContext || g_ram[(object + 8u) & 0x1FFFFFFu] != '#')
            return;
        const uint32_t base = object + 0x20u;
        const uint32_t rows = word(base + 8u), columns = word(base + 12u);
        const uint32_t buffer = word(base + (word(base + 16u) & 1u) * 4u);
        if (rows < 2u || rows > 64u || columns < 2u || columns > 64u || !buffer)
            return;
        if (g_staleCheck)
        {
            const uint32_t bytes = 0xB0u + rows * (columns * 0x40u + 0x10u);
            staleRecord(buffer, bytes, object, 2);
            if (const uint32_t other = word(base + ((word(base + 16u) & 1u) ^ 1u) * 4u))
                staleRecord(other, bytes, object, 3);
        }
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
        const uint64_t mesh = (1ull << 60) | g_skinnedDraws++;
        if (native_renderer::tuning().fastHooks)
        {
            // Each grid vertex read once, the triangles written in place.
            static std::vector<native_renderer::Vertex> grid;
            grid.resize(rows * columns);
            for (uint32_t r = 0; r < rows; ++r)
                for (uint32_t c = 0; c < columns; ++c)
                    grid[r * columns + c] = vertexAt(r, c);
            const uint32_t total = (rows - 1u) * (columns - 1u) * 6u;
            if (native_renderer::Vertex *out = native_renderer::frameMesh(total))
            {
                for (uint32_t r = 0; r + 1u < rows; ++r)
                    for (uint32_t c = 0; c + 1u < columns; ++c)
                    {
                        const native_renderer::Vertex &a = grid[r * columns + c], &b = grid[r * columns + c + 1u], &d = grid[(r + 1u) * columns + c],
                                                      &e = grid[(r + 1u) * columns + c + 1u];
                        *out++ = a, *out++ = b, *out++ = d, *out++ = b, *out++ = e, *out++ = d;
                    }
                native_renderer::commitFrameMesh(mesh, total);
            }
        }
        else
        {
            tris.clear();
            for (uint32_t r = 0; r + 1u < rows; ++r)
                for (uint32_t c = 0; c + 1u < columns; ++c)
                {
                    const native_renderer::Vertex a = vertexAt(r, c), b = vertexAt(r, c + 1u), d = vertexAt(r + 1u, c), e = vertexAt(r + 1u, c + 1u);
                    tris.insert(tris.end(), {a, b, d, b, e, d});
                }
            native_renderer::uploadMesh(mesh, tris.data(), static_cast<uint32_t>(tris.size()), true);
        }
        float camera[16];
        for (int i = 0; i < 16; ++i)
            camera[i] = real(g_cameraContext + 0x5A0u + i * 4u);
        static const bool viewChecks = std::getenv("SOTC_VIEW_CHECK") != nullptr;
        if (viewChecks)
            viewCheck(1, camera, view);
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
        HookTimer hookTimer;
        const bool shadows = native_renderer::tuning().shadows;
        static const bool idColours = std::getenv("SOTC_NATIVE_IDCOLOR") != nullptr;
        if (!shadows || idColours || !model || !g_cameraContext || ((word(chain) >> 28) & 7u) != 2u)
            return;
        const uint32_t ret = word(chain + 4u);
        static VuPalette palette;
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
        static const bool showVolumes = std::getenv("SOTC_SHADOW_SHOW") != nullptr; // debugging: the volumes in red
        if (showVolumes)
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
        HookTimer hookTimer;
        const bool off = !native_renderer::tuning().particles;
        const ParticleEnv &e = g_ptclEnvState;
        if (off || !g_cameraContext || count == 0u || count > 4096u || (!(e.type >= 1u && e.type <= 5u) && e.type != 7u))
            return;
        // Written straight into the frame's vertex buffer (at most 12 a particle).
        native_renderer::Vertex *room = native_renderer::tuning().fastHooks ? native_renderer::frameMesh(count * 12u) : nullptr;
        uint32_t written = 0;
        static std::vector<native_renderer::Vertex> tris;
        tris.clear();
        const auto put = [&](const auto &...corners)
        {
            if (room)
                ((room[written++] = corners), ...);
            else
                (tris.push_back(corners), ...);
        };
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
            // (uint8_t)clamp(x, 0, 255) without float compares (each a VFP
            // compare + flags transfer on the Vita): truncated to an int (NaN
            // gives 0 either way), then clamped as an int; the same bytes.
            const float cr = real(q + 16u) * 255.0f, cg = real(q + 20u) * 255.0f, cb = real(q + 24u) * 255.0f;
            auto byteOf = [](float x)
            {
#if defined(__arm__) && defined(__ARM_NEON)
                const int32_t i = vget_lane_s32(vcvt_s32_f32(vdup_n_f32(x)), 0); // saturating, NaN -> 0
#else
                const int32_t i = std::isnan(x) ? 0 : (x >= 2147483520.0f ? 255 : (x <= -2147483520.0f ? 0 : static_cast<int32_t>(x)));
#endif
                return static_cast<uint8_t>(i < 0 ? 0 : (i > 255 ? 255 : i));
            };
            const uint8_t ca = byteOf(real(q + 28u) * 128.0f);
            auto col = [&](native_renderer::Vertex &o, float scale)
            {
                o.r = byteOf(cr * scale);
                o.g = byteOf(cg * scale);
                o.b = byteOf(cb * scale);
                o.a = ca;
            };
            col(v, 1.0f); // every corner's colour (the lit ones set their own)
            auto at = [&](const float *o, float sign, float u, float vv)
            {
                native_renderer::Vertex c = v;
                c.x = p[0] + sign * o[0];
                c.y = p[1] + sign * o[1];
                c.z = p[2] + sign * o[2];
                c.u = u;
                c.v = vv;
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
                                        return c;
                };
                native_renderer::Vertex c16 = corner(p, sa, 1, 0, 0), c17 = corner(p, sa, -1, e.vec[0], 0), c18 = corner(p2, sb, 1, 0, e.vec[1]),
                                        c19 = corner(p2, sb, -1, e.vec[0], e.vec[1]);
                if (e.type == 3u)
                {
                    put(c16, c17, c18, c17, c19, c18);
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
                put(c, c16, c17, c, c17, c19, c, c19, c18, c, c18, c16);
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
                put(c, f16, f17, c, f17, f19, c, f19, f18, c, f18, f16);
            }
            else
                put(v16, v17, v18, v17, v19, v18);
        }
        if (room ? written == 0u : tris.empty())
        {
            if (room)
                native_renderer::cancelFrameMesh();
            return;
        }
        float camera[16];
        for (int i = 0; i < 16; ++i)
            camera[i] = real(g_cameraContext + 0x5A0u + i * 4u);
        const uint64_t mesh = (1ull << 58) | g_skinnedDraws++;
        if (room)
            native_renderer::commitFrameMesh(mesh, written);
        else
            native_renderer::uploadMesh(mesh, tris.data(), static_cast<uint32_t>(tris.size()), true);
        native_renderer::DrawState state;
        state.texture = texture(header);
        // SOTC_PTCL_DRAWLOG=1 (debugging, round 11's dust report): each type 7
        // draw's header, texture, blend and its particles' colour / size / cell
        // ranges; =2 every type.
        static const bool drawLog = std::getenv("SOTC_PTCL_DRAWLOG") != nullptr;
        if (drawLog && (e.type == 7u || std::getenv("SOTC_PTCL_DRAWLOG")[0] == '2'))
        {
            float lo[7], hi[7];
            for (int k = 0; k < 7; ++k)
                lo[k] = 1e30f, hi[k] = -1e30f;
            for (uint32_t i = 0; i < count; ++i)
            {
                const uint32_t q = a + i * 48u;
                const float vals[7] = {real(q + 16u), real(q + 20u), real(q + 24u), real(q + 28u), real(q + 12u), real(q + 40u), real(q + 44u)};
                for (int k = 0; k < 7; ++k)
                    lo[k] = std::min(lo[k], vals[k]), hi[k] = std::max(hi[k], vals[k]);
            }
            std::fprintf(stderr, "[ptcldraw] f %llu type %u n %u blend %u hdr %08x layer %u tex %016llx vec %g %g r %g..%g g %g..%g b %g..%g a %g..%g sz %g..%g u %g..%g v %g..%g\n",
                         (unsigned long long)g_frame, e.type, count, g_ptclBlend, header, layer, (unsigned long long)state.texture, e.vec[0], e.vec[1], lo[0], hi[0],
                         lo[1], hi[1], lo[2], hi[2], lo[3], hi[3], lo[4], hi[4], lo[5], hi[5], lo[6], hi[6]);
        }
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

    // modelDisp (0x1097118) without its display list: what it leaves in RAM
    // for the rest of the game, the VIF packets it links for VU1 (CALL tags
    // into each layer's chain: the instance's header, the microcode, every
    // packet of the LOD, the texture requests) left out. Nothing reads those
    // any more with the native renderer drawing, and walking them was most of
    // the VIF1 chain (Vif1Walker) besides modelDisp's own time. Kept:
    //   the parts' camera test results (LOD table node +0x2c),
    //   the instance packet's opacity (+0xdc) and +0xd0,
    //   setShadowEffect (0x1186294) for a drawn part with bit 24,
    //   the last surface per layer (*0x118bfc8, modelSetSurface reads it too).
    // Left out with the lists: texTransRequest (VRAM uploads for the GS that
    // draws nothing; native textures come from RAM) and iosDlLockObj (holds a
    // deletion until the frame's DMA is done; no DMA reads the model now).
    // SOTC_PS2_LISTS=1 / Debug "PS2 model lists": the recompiled modelDisp.
    // VU0 macro arithmetic as the recompiled code does it (PS2_VMUL /
    // PS2_VADD: operands and result with denormals flushed and infinities
    // clamped, a multiply and an add rounded apart) and its CLIP judgement.
    float vuNorm(float v)
    {
        uint32_t b;
        std::memcpy(&b, &v, 4);
        const uint32_t e = b & 0x7F800000u;
        if (e == 0u)
            b &= 0x80000000u;
        else if (e == 0x7F800000u)
            b = (b & 0x80000000u) | 0x7F7FFFFFu;
        std::memcpy(&v, &b, 4);
        return v;
    }
    // vmulax / vmadday / vmaddaz / vmaddw: m (4 columns) x (x, y, z, w),
    // every operand already through vuNorm (each product and sum is again).
    void vuTransform(const float m[16], float x, float y, float z, float w, float out[4])
    {
        for (int r = 0; r < 4; ++r)
        {
            const float a = vuNorm(vuNorm(m[r] * x) + vuNorm(m[4 + r] * y));
            const float b = vuNorm(a + vuNorm(m[8 + r] * z));
            out[r] = vuNorm(b + vuNorm(m[12 + r] * w));
        }
    }
    // ps2VuClipFlags(v, v): +x 0, -x 1, +y 2, -y 3, +z 4, -z 5 against |w|.
    uint32_t vuClipFlags(const float v[4])
    {
        uint32_t wBits;
        std::memcpy(&wBits, &v[3], 4);
        const int32_t limit = (wBits & 0x7F800000u) != 0u ? static_cast<int32_t>(wBits & 0x7FFFFFFFu) : 0x007FFFFF;
        uint32_t flags = 0u;
        for (int c = 0; c < 3; ++c)
        {
            uint32_t bits;
            std::memcpy(&bits, &v[c], 4);
            if (static_cast<int32_t>(bits) > limit)
                flags |= 1u << (2 * c);
            if (static_cast<int32_t>(bits ^ 0x80000000u) > limit)
                flags |= 2u << (2 * c);
        }
        return flags;
    }

    // vuClipFlags (bits as ordered integers against |w|) for four vectors at
    // once, one per lane of the result: no move to the ARM registers per vector.
    inline __m128i vuClipFlags4x(__m128 a, __m128 b, __m128 c, __m128 d)
    {
        _MM_TRANSPOSE4_PS(a, b, c, d); // a: the x of each, b: y, c: z, d: w
        const __m128i w = _mm_castps_si128(d);
        const __m128i expZero = _mm_cmpeq_epi32(_mm_and_si128(w, _mm_set1_epi32(0x7F800000)), _mm_setzero_si128());
        const __m128i lim = _mm_or_si128(_mm_andnot_si128(expZero, _mm_and_si128(w, _mm_set1_epi32(0x7FFFFFFF))),
                                         _mm_and_si128(expZero, _mm_set1_epi32(0x007FFFFF)));
        const __m128i sign = _mm_set1_epi32(static_cast<int>(0x80000000u));
        const auto lanes = [&](__m128 v, int plusBit)
        {
            const __m128i bits = _mm_castps_si128(v);
            return _mm_or_si128(_mm_and_si128(_mm_cmpgt_epi32(bits, lim), _mm_set1_epi32(1 << plusBit)),
                                _mm_and_si128(_mm_cmpgt_epi32(_mm_xor_si128(bits, sign), lim), _mm_set1_epi32(2 << plusBit)));
        };
        return _mm_or_si128(_mm_or_si128(lanes(a, 0), lanes(b, 2)), lanes(c, 4));
    }

    // ---- Prefetching the guest lines a model's draw reads (g_prefetch) ----
    //
    // The Vita's Cortex-A9 has no useful hardware prefetch and a DRAM miss
    // costs ~250 ns; a model's draw walks its instance, its header, its
    // packet list (32 bytes a packet, every LOD), its part tree (0x30 a
    // node, linked) and its surfaces, each line the first time a frame: one
    // miss after the other (modelDispState ~35 us an instance). What a model
    // looked like the last time (ModelShape, by header) and the order of the
    // last frame's draws (g_calls) say what the next draw will read: asked
    // for while this one runs (a few lines per packet), so the misses overlap.
    struct ModelShape
    {
        uint32_t header = 0, packets = 0, packetCount = 0, table = 0, nodes = 0;
        uint32_t surfaces[8] = {}; // the first distinct surfaces drawn (their flags' line)
        uint32_t surfaceCount = 0;
    };
    ModelShape g_shapes[512];
    inline ModelShape &modelShape(uint32_t header) { return g_shapes[((header >> 4) * 0x9E3779B1u) >> 23]; }
    struct CallRecord
    {
        uint32_t instance, object, header, own;
    };
    std::vector<CallRecord> g_calls, g_lastCalls; // this frame's modelDisp calls, the last frame's
    size_t g_callCursor = 0;                      // where this frame is in g_lastCalls
    // A queue of lines to ask for, a few at a time (a PLD past the A9's
    // line fill buffers waits for one: not all at once).
    struct PrefetchQueue
    {
        uint32_t lines[192];
        uint32_t head = 0, tail = 0;
        void push(uint32_t addr)
        {
            if (tail < 192u)
                lines[tail++] = addr;
        }
        void pushRange(uint32_t addr, uint32_t bytes, uint32_t maxLines)
        {
            for (uint32_t a = addr & ~31u, n = 0; a < addr + bytes && n < maxLines; a += 32u, ++n)
                push(a);
        }
        void pump(uint32_t n)
        {
            for (; n && head < tail; --n)
                prefetchGuest(lines[head++]);
        }
        void clear() { head = tail = 0; }
    } g_prefetchQueue;
    void newDrawOrder()
    {
        g_lastCalls.swap(g_calls);
        g_calls.clear();
        g_callCursor = 0;
        g_prefetchQueue.clear();
    }
    // The next draw's lines (from the last frame's order), queued.
    void queueNextDraw(uint32_t instance)
    {
        PrefetchQueue &q = g_prefetchQueue;
        q.pump(8u); // the last one's: a few more now, the rest dropped
        q.clear();
        size_t at = g_callCursor;
        if (at >= g_lastCalls.size() || g_lastCalls[at].instance != instance)
        {
            // Out of step: looked for a little ahead.
            at = ~size_t(0);
            for (size_t i = g_callCursor, end = std::min(g_lastCalls.size(), g_callCursor + 8u); i < end; ++i)
                if (g_lastCalls[i].instance == instance)
                {
                    at = i;
                    break;
                }
            if (at == ~size_t(0))
                return;
        }
        g_callCursor = at + 1u;
        if (g_callCursor >= g_lastCalls.size())
            return;
        const CallRecord &next = g_lastCalls[g_callCursor];
        // First what the next draw reads first (a chain: instance, object, header).
        prefetchGuest(next.instance);
        prefetchGuest(next.instance + 0x240u);
        prefetchGuest(next.instance + 0x260u);
        prefetchGuest(next.object + 0x20u);
        q.pushRange(next.header, 0x80u, 4u);
        q.push(next.own + 0x10u);
        q.push(next.own + 0x30u);
        q.push(next.own + 0x50u);
        q.push(next.own + 0x70u);
        q.push(next.own + 0xD0u);
        const ModelShape &shape = modelShape(next.header);
        if (shape.header != next.header)
            return;
        if (!g_planOn) // with the draw plans the packet list is not read
            q.pushRange(shape.packets, shape.packetCount * 32u, 64u);
        if (shape.table)
            q.pushRange(shape.table, shape.nodes * 0x30u, 64u);
        for (uint32_t i = 0; i < shape.surfaceCount; ++i)
            q.push(shape.surfaces[i] + 0x18u);
    }

    void modelDispState(R5900Context *ctx, uint32_t instance, const float model[16], const ModelPlan *plan = nullptr)
    {
        const uint32_t header = word(word(instance) + 0x20u);
        uint64_t lodWord;
        std::memcpy(&lodWord, g_ram + ((instance + 0x250u) & 0x1FFFFF8u), 8);
        // dsll 21, dsra32 29: bits 40-42, signed.
        uint32_t lod = static_cast<uint32_t>(static_cast<int64_t>(lodWord << 21) >> 61);
        const uint32_t flags18 = word(header + 0x18u) & 1u;
        const bool partCulled = flags18 == 0u && word(header + 0x54u) != 1u;
        if (word(header + 0x1Cu) <= lod && flags18 != 0u)
            lod = word(header + 0x1Cu) - 1u;
        const uint32_t table = word(header + 0x30u + lod * 4u);
        uint32_t maxNode = table; // the last node the walk reached (ModelShape)
        struct ShapeNodes // kept whichever way the function returns
        {
            uint32_t header, table, &maxNode;
            ~ShapeNodes()
            {
                if (!g_prefetch || !table)
                    return;
                ModelShape &shape = modelShape(header);
                if (shape.header != header)
                {
                    shape = ModelShape();
                    shape.header = header;
                }
                if (shape.table != table)
                {
                    shape.table = table;
                    shape.nodes = 0;
                }
                shape.nodes = std::max(shape.nodes, (maxNode - table) / 0x30u + 1u);
            }
        } shapeNodes{header, partCulled ? table : 0u, maxNode};
        if (partCulled)
        {
            if (table == 0u)
                return;
            // camera context +0x5a0 x the model matrix, on VU0.
            float camera[16], clip[16];
            for (int i = 0; i < 16; ++i)
                camera[i] = vuNorm(real(g_cameraContext + 0x5A0u + i * 4u));
            for (int c = 0; c < 4; ++c)
                vuTransform(camera, vuNorm(model[c * 4]), vuNorm(model[c * 4 + 1]), vuNorm(model[c * 4 + 2]), vuNorm(model[c * 4 + 3]), clip + c * 4);
            // modelDisp / manager_sub_01099A08: the box's eight corners in this
            // order through vclipw, the CLIP register (24 bits, the last four
            // judgements) read after each: all of them sharing a plane -> 1
            // (out), the register 0 at every read (this box's corners and the
            // three judgements before them all inside) -> 2, else 0 (partly).
            uint32_t clipRegister = ctx->vu0_clip_flags & 0xFFFFFFu;
            // vuTransform four rows at a time (the same products and sums,
            // each through the same clamp: ps2VuNormalize is vuNorm by lane),
            // the products by each corner's x, y and z made once for the box.
            const __m128 col0 = _mm_loadu_ps(clip), col1 = _mm_loadu_ps(clip + 4), col2 = _mm_loadu_ps(clip + 8), col3 = _mm_loadu_ps(clip + 12);
            const __m128 byW = ps2VuNormalize(_mm_mul_ps(col3, _mm_set1_ps(1.0f)));
            const auto test = [&](uint32_t node) -> uint32_t
            {
                static constexpr uint8_t kCorners[8] = {0, 6, 5, 3, 4, 2, 1, 7}; // bit 0 x, 1 y, 2 z: max
                // the node's min (x, y, z at +0) and max (+0x10), clamped
                const __m128 lo = ps2VuNormalize(_mm_loadu_ps(reinterpret_cast<const float *>(g_ram + (node & 0x1FFFFFCu)))),
                             hi = ps2VuNormalize(_mm_loadu_ps(reinterpret_cast<const float *>(g_ram + ((node + 0x10u) & 0x1FFFFFCu))));
                const __m128 byX[2] = {ps2VuNormalize(_mm_mul_ps(col0, _mm_shuffle_ps(lo, lo, 0x00))), ps2VuNormalize(_mm_mul_ps(col0, _mm_shuffle_ps(hi, hi, 0x00)))};
                const __m128 byY[2] = {ps2VuNormalize(_mm_mul_ps(col1, _mm_shuffle_ps(lo, lo, 0x55))), ps2VuNormalize(_mm_mul_ps(col1, _mm_shuffle_ps(hi, hi, 0x55)))};
                const __m128 byZ[2] = {ps2VuNormalize(_mm_mul_ps(col2, _mm_shuffle_ps(lo, lo, 0xAA))), ps2VuNormalize(_mm_mul_ps(col2, _mm_shuffle_ps(hi, hi, 0xAA)))};
                __m128 v[8];
                for (int k = 0; k < 8; ++k)
                {
                    const uint8_t corner = kCorners[k];
                    v[k] = ps2VuNormalize(_mm_add_ps(byX[corner & 1], byY[(corner >> 1) & 1]));
                    v[k] = ps2VuNormalize(_mm_add_ps(v[k], byZ[(corner >> 2) & 1]));
                    v[k] = ps2VuNormalize(_mm_add_ps(v[k], byW));
                }
                // The corners' judgements f[3..10], after the register's last
                // three (f[0..2]); the register after corner k holds f[k..k+3]
                // (6 bits each, f[k+3] lowest). all = 0x3f AND the eight
                // registers (their low 6 bits: the corners' own), any = the OR
                // of the registers (every f).
                alignas(16) uint32_t f[11];
                f[0] = (clipRegister >> 12) & 0x3Fu;
                f[1] = (clipRegister >> 6) & 0x3Fu;
                f[2] = clipRegister & 0x3Fu;
                _mm_storeu_si128(reinterpret_cast<__m128i *>(f + 3), vuClipFlags4x(v[0], v[1], v[2], v[3]));
                _mm_storeu_si128(reinterpret_cast<__m128i *>(f + 7), vuClipFlags4x(v[4], v[5], v[6], v[7]));
                const uint32_t all = f[3] & f[4] & f[5] & f[6] & f[7] & f[8] & f[9] & f[10];
                const uint32_t any = f[0] | f[1] | f[2] | f[3] | f[4] | f[5] | f[6] | f[7] | f[8] | f[9] | f[10];
                clipRegister = f[10] | (f[9] << 6) | (f[8] << 12) | (f[7] << 18);
                return all ? 1u : (any ? 0u : 2u);
            };
            struct ClipBack // the register as modelDisp leaves it
            {
                R5900Context *ctx;
                uint32_t &value;
                ~ClipBack() { ctx->vu0_clip_flags = value; }
            } clipBack{ctx, clipRegister};
            const auto put = [](uint32_t addr, uint32_t value) { std::memcpy(const_cast<uint8_t *>(g_ram) + (addr & 0x1FFFFFCu), &value, 4); };
            // manager_sub_01099A08: a node takes its parent's result unless
            // that is 0 (partly inside), then it is tested; written at +0x2c.
            // Each node once in a real tree (at most 4096): the budget stops a
            // table with shared or looping links from walking exponentially.
            uint32_t budget = 4096u;
            const auto visit = [&](auto &&self, uint32_t node, uint32_t inherited, int depth) -> void
            {
                if (depth > 64 || budget == 0u)
                    return;
                --budget;
                maxNode = std::max(maxNode, node);
                const uint32_t state = inherited ? inherited : test(node);
                put(node + 0x2Cu, state);
                for (uint32_t link : {0x20u, 0x24u})
                    if (word(node + link) != 0xFFFFFFFFu)
                        self(self, table + word(node + link) * 0x30u, state, depth + 1);
            };
            if (g_prefetch)
            {
                // This model's part tree as it was: asked for at once (the
                // walk below goes from node to node by their links).
                const ModelShape &shape = modelShape(header);
                if (shape.header == header && shape.table == table)
                    for (uint32_t a = table & ~31u, n = 0; a < table + shape.nodes * 0x30u && n < 64u; a += 32u, ++n)
                        prefetchGuest(a);
            }
            const uint32_t root = test(table);
            put(table + 0x2Cu, root);
            if (root == 1u)
                return;
            for (uint32_t link : {0x20u, 0x24u})
                if (word(table + link) != 0xFFFFFFFFu)
                    visit(visit, table + word(table + link) * 0x30u, root, 1);
        }
        const auto put = [](uint32_t addr, uint32_t value) { std::memcpy(const_cast<uint8_t *>(g_ram) + (addr & 0x1FFFFFCu), &value, 4); };
        uint64_t drawFlags;
        std::memcpy(&drawFlags, g_ram + ((instance + 0x260u) & 0x1FFFFF8u), 8);
        if ((drawFlags & 4u) == 0u)
        {
            float opacity = 1.0f - real(instance + 0x244u);
            if (word(header + 0xCu) != 0u)
                opacity *= 1.0f - real(instance + 0x248u);
            const uint32_t packet = word(instance + 0xCu);
            uint32_t bits;
            std::memcpy(&bits, &opacity, 4);
            if (flags18 == 0u)
            {
                if (!(1.0f <= opacity))
                    put(packet + 0xDCu, bits);
            }
            else
            {
                if (real(packet + 0xD0u) != real(instance + 0x24Cu))
                    put(packet + 0xD0u, word(instance + 0x24Cu));
                if (opacity < 1.0f)
                    put(packet + 0xDCu, bits);
            }
        }
        const bool fading = 0.0f < real(instance + 0x244u);
        const uint32_t boss = word(0x1186290u), cache = word(0x118BFC8u);
        const uint32_t packets = word(header + 0x60u), surfaces = word(header + 0x50u);
        ModelShape *shape = nullptr;
        if (g_prefetch)
        {
            shape = &modelShape(header);
            if (shape->header != header)
            {
                *shape = ModelShape();
                shape->header = header;
            }
            if (shape->packets == packets)
                for (uint32_t a = packets & ~31u, n = 0; a < packets + shape->packetCount * 32u && n < 8u; a += 32u, ++n)
                    prefetchGuest(a); // the first lines at once, the rest as the walk goes
            shape->packets = packets;
            shape->surfaceCount = 0;
        }
        const bool memo = g_surfaceMemo && native_renderer::tuning().fastHooks;
        if (plan && memo && plan->header == header && plan->lod == lod && plan->packets == packets && plan->surfaces == surfaces)
        {
            // The plan's packets of this LOD, in the list's order: the same
            // tests and writes as the walk below.
            for (const PlanPacket &e : plan->pk)
            {
                const uint32_t s = e.s;
                SurfaceInfo &info = g_surfaceInfo[e.slot];
                if (!(info.s == s && info.header == header && info.frame == g_frame))
                    surfaceInfo(header, s);
                const uint64_t flags = info.flags;
                if ((flags >> 29) & 1u)
                    continue;
                if (partCulled && e.part != 0xFFFFFFFFu)
                {
                    const uint32_t state = word(table + e.part * 0x30u + 0x2Cu);
                    if (state != 0u && state != 2u)
                        continue;
                }
                if ((flags >> 17) & 1u)
                    continue;
                if ((flags >> 24) & 1u)
                {
                    put(0x1186294u, 1u); // setShadowEffect
                    if (!(boss & 2u))
                        continue;
                }
                if (((flags >> 26) & 1u) && word(0x1186328u) != 0u)
                    continue;
                put(cache + info.layer[fading ? 1 : 0] * 4u, s);
            }
            return;
        }
        uint32_t walked = 0;
        for (uint32_t pk = packets; word(pk) != 0u; pk += 32u)
        {
            ++walked;
            if (shape)
            {
                prefetchGuest(pk + 8u * 32u);
                g_prefetchQueue.pump(2u); // the next draw's lines (queueNextDraw)
            }
            if (word(pk + 4u) != lod)
                continue;
            const uint32_t s = word(pk + 8u) * 0x120u + surfaces;
            if (shape && shape->surfaceCount < 8u &&
                std::find(shape->surfaces, shape->surfaces + shape->surfaceCount, s) == shape->surfaces + shape->surfaceCount)
                shape->surfaces[shape->surfaceCount++] = s;
            SurfaceInfo *info = memo ? &surfaceInfo(header, s) : nullptr;
            uint64_t flags;
            if (info)
                flags = info->flags;
            else
                std::memcpy(&flags, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
            if ((flags >> 29) & 1u)
                continue;
            if (partCulled && word(pk + 12u) != 0xFFFFFFFFu)
            {
                const uint32_t state = word(table + word(pk + 12u) * 0x30u + 0x2Cu);
                if (state != 0u && state != 2u)
                    continue;
            }
            if ((flags >> 17) & 1u)
                continue; // the boss effect's surfaces: no cached surface
            if ((flags >> 24) & 1u)
            {
                put(0x1186294u, 1u); // setShadowEffect
                if (!(boss & 2u))
                    continue;
            }
            if (((flags >> 26) & 1u) && word(0x1186328u) != 0u)
                continue;
            put(cache + (info ? info->layer[fading ? 1 : 0] : surfaceLayer(header, s, fading)) * 4u, s);
        }
        if (shape)
            shape->packetCount = walked;
    }

    // SOTC_MODELDISP_CHECK=1 (debugging): modelDispState against the
    // recompiled modelDisp on the same input, on what it leaves behind: the
    // values modelDispState writes are taken, put back, then modelDisp runs
    // to its return and its values are compared (a log line per mismatch).
    void modelDispCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t instance, const float model[16])
    {
        static uint64_t checks = 0, mismatches = 0;
        std::vector<uint32_t> at;
        const uint32_t header = word(word(instance) + 0x20u);
        for (uint32_t i = 0; i < 32u; ++i)
            at.push_back(word(0x118BFC8u) + i * 4u);
        at.push_back(0x1186294u);
        at.push_back(word(instance + 0xCu) + 0xD0u);
        at.push_back(word(instance + 0xCu) + 0xDCu);
        for (uint32_t lod = 0; lod < 4u; ++lod)
        {
            const uint32_t table = word(header + 0x30u + lod * 4u);
            if (!table || (word(header + 0x18u) & 1u) || word(header + 0x54u) == 1u)
                continue;
            std::vector<uint32_t> todo{0u};
            std::set<uint32_t> seen;
            while (!todo.empty() && seen.size() < 4096u)
            {
                const uint32_t node = todo.back();
                todo.pop_back();
                if (!seen.insert(node).second)
                    continue;
                at.push_back(table + node * 0x30u + 0x2Cu);
                for (uint32_t link : {0x20u, 0x24u})
                    if (word(table + node * 0x30u + link) != 0xFFFFFFFFu)
                        todo.push_back(word(table + node * 0x30u + link));
            }
        }
        std::vector<uint32_t> before(at.size()), mine(at.size());
        for (size_t i = 0; i < at.size(); ++i)
            before[i] = word(at[i]);
        const uint32_t clipBefore = ctx->vu0_clip_flags;
        modelDispState(ctx, instance, model);
        const uint32_t clipMine = ctx->vu0_clip_flags;
        ctx->vu0_clip_flags = clipBefore;
        for (size_t i = 0; i < at.size(); ++i)
        {
            mine[i] = word(at[i]);
            std::memcpy(rdram + (at[i] & 0x1FFFFFCu), &before[i], 4);
        }
        // modelDisp to its return (it can leave on a continuation).
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        g_modelDisp(rdram, ctx, runtime);
        while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
        {
            PS2Runtime::RecompiledFunction next = runtime->hasFunction(ctx->pc) ? runtime->lookupFunction(ctx->pc) : nullptr;
            if (!next)
                break;
            next(rdram, ctx, runtime);
        }
        ++checks;
        if ((ctx->vu0_clip_flags & 0xFFFFFFu) != (clipMine & 0xFFFFFFu))
        {
            ++mismatches;
            if (mismatches < 200u)
                std::fprintf(stderr, "[modeldisp-check] header %08x CLIP recompiled %06x native %06x\n", header, ctx->vu0_clip_flags & 0xFFFFFFu, clipMine & 0xFFFFFFu);
        }
        for (size_t i = 0; i < at.size(); ++i)
            if (word(at[i]) != mine[i])
            {
                ++mismatches;
                if (mismatches < 200u)
                    std::fprintf(stderr, "[modeldisp-check] header %08x at %08x (#%zu) recompiled %08x native %08x before %08x\n", header, at[i], i, word(at[i]), mine[i], before[i]);
            }
        if ((checks & (checks - 1u)) == 0u || checks % 10000u == 0u)
            std::fprintf(stderr, "[modeldisp-check] %llu checks, %llu mismatched values\n", (unsigned long long)checks, (unsigned long long)mismatches);
    }

    // spriteTexFast (0x1c5548): a textured sprite of the screen effects
    // (reflection, bloom, refraction copies...) written as GS registers
    // straight into the gsReg packet, never through gsReg (the 2D hooks do
    // not see it; the native renderer makes those effects itself). Nothing
    // else is changed: left out with the PS2 lists.
    constexpr uint32_t kSpriteTexFast = 0x001C5548u;
    PS2Runtime::RecompiledFunction g_spriteTexFast = nullptr;

    void spriteTexFastHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (native_renderer::tuning().ps2Lists || !GS::skipRaster())
        {
            g_spriteTexFast(rdram, ctx, runtime);
            return;
        }
        ctx->pc = GPR_U32(ctx, 31); // jr ra
    }

    // A model packet's mesh made on the async worker (native_renderer::buildAsync)
    // from a copy of its bytes: read and triangulated there (a skinned one
    // made into its bind pose), taken up at a later frame's start. False:
    // no worker, build it here.
    bool buildPacketAsync(uint64_t id, uint32_t data, uint32_t size, bool twoPasses)
    {
        struct Job
        {
            std::vector<uint8_t> bytes;
            uint32_t base = 0;
            bool ok = false, skinned = false, lit = false;
            std::vector<native_renderer::Vertex> vertices, second;
            SkinnedPacket packet;
        };
        if (!native_renderer::tuning().asyncBuilds)
            return false;
        auto job = std::make_shared<Job>();
        job->base = data & ~3u;
        const uint32_t bytes = (data - job->base) + size + 64u; // and a little after (read as the game's would be)
        try
        {
            job->bytes.resize(bytes);
        }
        catch (const std::bad_alloc &)
        {
            return false;
        }
        if ((job->base & 0x1FFFFFFu) + bytes <= 0x2000000u)
            std::memcpy(job->bytes.data(), g_ram + (job->base & 0x1FFFFFFu), bytes);
        else
            for (uint32_t i = 0; i + 4u <= bytes; i += 4u)
            {
                const uint32_t v = word(job->base + i);
                std::memcpy(&job->bytes[i], &v, 4);
            }
        std::vector<uint64_t> ids{id};
        if (twoPasses)
            ids.push_back(id ^ (1ull << 61));
        const auto work = [job, data, size, twoPasses]
        {
            try
            {
                std::vector<Strip> strips;
                readBatchesFrom(CopyMem{job->base, &job->bytes}, data, size, strips);
                job->skinned = skinned(strips);
                job->lit = lit(strips);
                if (job->skinned)
                    job->packet = makeSkinnedPacket(std::move(strips));
                else
                {
                    const auto place = [](const BatchVertex &v, native_renderer::Vertex &o)
                    {
                        o.x = v.x;
                        o.y = v.y;
                        o.z = v.z;
                        o.nx = v.nx;
                        o.ny = v.ny;
                        o.nz = v.nz;
                    };
                    triangulate(strips, place, job->vertices);
                    if (twoPasses) // two textures: the second pass's mesh, with the second coordinates
                        triangulate(strips, place, job->second, true);
                }
                job->ok = true;
            }
            catch (const std::bad_alloc &)
            {
                job->ok = false; // asked for again at its next draw
            }
            std::vector<uint8_t>().swap(job->bytes);
        };
        const auto finish = [job, id, twoPasses]
        {
            if (!job->ok)
                return;
            if (job->lit)
                g_litPackets.insert(id);
            if (job->skinned)
            {
                if (g_skinnedPackets.find(id) == g_skinnedPackets.end())
                {
                    g_skinnedPacketBytes += job->packet.bytes;
                    g_skinnedPackets.emplace(id, std::move(job->packet));
                    g_skinnedIds.insert(id);
                }
                return;
            }
            native_renderer::uploadMesh(id, job->vertices.data(), static_cast<uint32_t>(job->vertices.size()));
            if (twoPasses)
                native_renderer::uploadMesh(id ^ (1ull << 61), job->second.data(), static_cast<uint32_t>(job->second.size()));
        };
        return native_renderer::buildAsync(std::move(ids), work, finish);
    }

    // modelDispHook's debugging switches, read once: a function-local static
    // costs a guard check with a memory barrier (dmb) at every use on the
    // Vita, and the packet loop used seven of them per packet.
    struct ModelDispEnv
    {
        bool logPasses = std::getenv("SOTC_NATIVE_LOG") != nullptr;
        bool logMat = std::getenv("SOTC_NATIVE_LOGMAT") != nullptr;
        uint32_t skipLayers = [] // debugging: SOTC_NATIVE_SKIPLAYERS=12,18
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
        int logLayer = std::getenv("SOTC_LOG_LAYER") ? std::atoi(std::getenv("SOTC_LOG_LAYER")) : -1;
        // Debugging: who drew what. SOTC_NATIVE_IDCOLOR=1 every model, =<n> > 1
        // only surfaces in layers >= n (the rest drawn as usual).
        int idFrom = std::getenv("SOTC_NATIVE_IDCOLOR") ? std::max(1, std::atoi(std::getenv("SOTC_NATIVE_IDCOLOR"))) : 0;
        float tintAlpha = std::getenv("SOTC_REFLECT_ALPHA") ? static_cast<float>(std::atof(std::getenv("SOTC_REFLECT_ALPHA"))) : 1.0f;
        float refractAlpha = std::getenv("SOTC_REFRACT_ALPHA") ? static_cast<float>(std::atof(std::getenv("SOTC_REFRACT_ALPHA"))) : 1.0f;
        bool showRefraction = std::getenv("SOTC_REFRACT_SHOW") != nullptr; // debugging: where it lands, in red
    };

    void modelDispHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        syncFrame(rdram);
        static const ModelDispEnv env;
        const uint32_t instance = GPR_U32(ctx, 4), matrix = GPR_U32(ctx, 5);
        float model[16];
        for (int i = 0; i < 16; ++i)
            model[i] = real(matrix + i * 4u);
        // The lists are built while the GS draws (the emulated picture shown).
        const bool ps2Lists = native_renderer::tuning().ps2Lists || !GS::skipRaster();
        static const bool check = std::getenv("SOTC_MODELDISP_CHECK") != nullptr;
        if (ps2Lists && !check)
            g_modelDisp(rdram, ctx, runtime); // the emulated drawing goes on as before
        else
        {
            // iosgluGetCameraContext -> get_cur_camera_context, as modelDisp asks.
            g_cameraContext = word(0x0122A8F0u + word(0x0118FE4Cu) * 4u);
            if (!check)
                ctx->pc = GPR_U32(ctx, 31); // jr ra
        }
        HookTimer hookTimer;
        SOTC_PROF_SCOPE(kHookModelDisp);
        if (!g_cameraContext)
            return;
        // SOTC_HIDE_HEADER=<hex> (debugging, round 12): that model left out.
        static const uint32_t hideHeader = std::getenv("SOTC_HIDE_HEADER") ? static_cast<uint32_t>(std::strtoul(std::getenv("SOTC_HIDE_HEADER"), nullptr, 16)) : 0u;
        if (hideHeader && word(word(instance) + 0x20u) == hideHeader)
            return;
        if (g_prefetch)
        {
            queueNextDraw(instance);
            const uint32_t object = word(instance);
            g_calls.push_back({instance, object, word(object + 0x20u), word(instance + 0xCu)});
        }

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
        static const bool viewChecks = std::getenv("SOTC_VIEW_CHECK") != nullptr;
        if (viewChecks)
        {
            const uint32_t own = word(instance + 0xCu);
            if (((word(own) >> 28) & 7u) == 2u)
            {
                const uint32_t ret = word(own + 4u);
                if ((word(ret) & 0xFFFFu) == 5u)
                {
                    float vu[16];
                    for (int i = 0; i < 16; ++i)
                        vu[i] = real(ret + 16u + i * 4u);
                    viewCheck(0, clip, vu);
                }
            }
        }
        // The draw plan of the game's LOD (round 8): modelDispState's packet
        // tests and the packet loop below go through it.
        const bool planOn = g_planOn && !check && native_renderer::tuning().fastHooks && g_surfaceMemo && native_renderer::tuning().packetQuickCheck;
        ModelPlan *gamePlan = nullptr;
        if (planOn)
        {
            const uint32_t h = word(word(instance) + 0x20u);
            uint64_t lw;
            std::memcpy(&lw, rdram + ((instance + 0x250u) & 0x1FFFFF8u), 8);
            uint32_t l = static_cast<uint32_t>((lw >> 40) & 7u);
            if ((word(h + 0x18u) & 1u) && l >= word(h + 0x1Cu) && word(h + 0x1Cu) != 0u)
                l = word(h + 0x1Cu) - 1u;
            gamePlan = modelPlan(h, l);
            ++g_planStats.uses;
        }
        if (check)
            modelDispCheck(rdram, ctx, runtime, instance, model);
        else if (!ps2Lists)
        {
            SOTC_PROF_SCOPE(kHookState);
            modelDispState(ctx, instance, model, gamePlan);
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
        // Quality "Model detail" (SOTC_MODEL_DETAIL): one level finer than
        // the game's, or always level 0 (the finest; far hills and colossi
        // lose their blocky outlines). The parts are then tested here, as
        // the game's results (+0x2c) are for its own level's table.
        const uint32_t gameLod = lod;
        if (const int finer = static_cast<int>(native_renderer::tuning().modelDetail + 0.5f); finer > 0 && lod > 0u)
        {
            lod = finer >= 2 ? 0u : lod - 1u;
            if (word(header + 0x30u + lod * 4u) == 0u && word(header + 0x30u + gameLod * 4u) != 0u)
                lod = gameLod; // no part table there: as the game has it
        }
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
            SOTC_PROF_SCOPE(kHookPartTest);
            partVisible.clear();
            // The box (node +0 min, +0x10 max) through camera x model, VCLIP
            // against |w|: 1 every corner out past one plane, 2 all inside, 0 partly.
            // Without the PS2 lists modelDispState has made the game's own
            // test already (left at +0x2c, as modelDisp would): taken as is.
            const auto test = [&](uint32_t node) -> uint32_t
            {
                if (!ps2Lists && !check && lod == gameLod)
                    return word(node + 0x2Cu); // modelDispState's (the game's own) result
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
            // A tree has at most 4096 nodes (index < 4096), each reached once:
            // the budget stops a table that is not (shared or looping links,
            // e.g. a model still being loaded) from walking exponentially.
            uint32_t budget = 4096u;
            const auto visit = [&](auto &&self, uint32_t index, uint32_t inherited, int depth) -> void
            {
                if (index >= 4096u || depth > 64 || budget == 0u)
                    return;
                --budget;
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
        static VuPalette palette;
        bool hasPalette;
        {
            SOTC_PROF_SCOPE(kHookPalette);
            hasPalette = readPalette(instance, palette);
            if (g_staleCheck)
            {
                staleRecord(matrix, 64u, instance, 0);
                if (hasPalette)
                {
                    const uint32_t ret = word(word(instance + 0xCu) + 4u);
                    staleRecord(ret + 16u, (word(ret) & 0xFFFFu) * 16u, instance, 1);
                }
            }
        }
        const bool fast = native_renderer::tuning().fastHooks;
        const bool memo = fast && g_surfaceMemo;
        uint32_t paletteHandle = 0, paletteLo = 0, paletteHi = 0; // this instance's palette as last sent
        // This instance's record for skinAtKick (made at its first skinned draw).
        int32_t skinRecordIndex = -1;
        const auto skinRecord = [&]() -> uint32_t
        {
            if (skinRecordIndex < 0)
            {
                skinRecordIndex = static_cast<int32_t>(g_skinRecords.size());
                g_skinRecords.push_back({word(instance + 0xCu), palette.lo, palette.hi,
                                         std::vector<float>(palette.data() + palette.lo * 4u, palette.data() + palette.hi * 4u)});
            }
            return static_cast<uint32_t>(skinRecordIndex);
        };
        float projection[16];
        for (int i = 0; i < 16; ++i)
            projection[i] = real(g_cameraContext + 0x40u + i * 4u);
        SOTC_PROF_SCOPE(kHookPackets);
        HookStats &hs = hookStats();
        if (hs.on)
            ++hs.instances, hs.headers.insert(header);
        float lights[32];
        bool haveLights = false;
        // Runs (g_planRun) only without the per-packet debugging outputs.
        const bool runs = g_planRuns && !env.logPasses && !env.logMat && env.logLayer < 0 && !r2Check().on;
        // One packet's draws (both walks below).
        const auto drawPacket = [&](uint32_t k, uint32_t surface, uint32_t s, SurfaceInfo *info, uint32_t size, uint32_t data, uint64_t id, bool changedInPlace,
                                    PlanPacket *e)
        {
            // A plan's packet: what was found out about its id before.
            const bool known = e && e->resolved;
            SkinnedPacket *skinnedPacket = nullptr;
            if (known)
                skinnedPacket = e->skinned;
            else if (g_skinnedIds.count(id))
            {
                const auto found = g_skinnedPackets.find(id);
                if (found != g_skinnedPackets.end())
                    skinnedPacket = &found->second;
            }
            const bool twoPasses = (info ? info->texMode : word(s + 0x1Cu)) == 2u;
            const bool needsBuild = !skinnedPacket && ((!(known && e->ready) && !native_renderer::hasMesh(id)) ||
                                                       (twoPasses && !(known && e->readyTwo) && !native_renderer::hasMesh(id ^ (1ull << 61))));
            if (needsBuild && !changedInPlace && native_renderer::tuning().asyncBuilds && !native_renderer::asyncBudgetLeft())
            {
                // This frame's builds are spent (round 12, the area change):
                // left out like a draw still on the worker, built next frame.
                ++native_renderer::frameCounters().heldDraws;
                return;
            }
            if (needsBuild && !changedInPlace && buildPacketAsync(id, data, size, twoPasses))
                ; // on the worker: this packet is left out until it is taken up
            else if (needsBuild)
            {
                SOTC_PROF_SCOPE(kHookBuild);
                strips.clear();
                readBatches(data, size, strips);
                if (skinned(strips) && lit(strips))
                    g_litPackets.insert(id);
                if (skinned(strips))
                {
                    try
                    {
                        skinnedPacket = &g_skinnedPackets.emplace(id, makeSkinnedPacket(std::move(strips))).first->second;
                        g_skinnedIds.insert(id);
                    }
                    catch (const std::bad_alloc &)
                    {
                        return; // no room in the heap: not drawn this time
                    }
                    g_skinnedPacketBytes += skinnedPacket->bytes;
                }
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
            if (e && (!e->resolved || (twoPasses && !e->readyTwo)))
            {
                // Known from now on (until the id changes, or meshes / skinned packets go).
                e->skinned = skinnedPacket;
                e->ready = skinnedPacket || native_renderer::meshKnown(id);
                e->readyTwo = native_renderer::meshKnown(id ^ (1ull << 61));
                e->lit = g_litPackets.count(id);
                e->resolved = e->ready;
            }
            uint64_t mesh = id;
            const float *transform = clip;
            bool posed = false;                         // skinned: camera space
            uint32_t skinPalette = 0, skinPaletteCount = 0; // posed by the GPU: its palette
            if (skinnedPacket)
            {
                SOTC_PROF_SCOPE(kHookSkin);
                skinnedPacket->used = g_frame;
                if (!hasPalette)
                    return;
                posed = true;
                SkinnedPacket &packet = *skinnedPacket;
                if (fast && packet.gpu && native_renderer::gpuSkinning())
                {
                    // Posed by the GPU: the bind pose kept under the packet's
                    // id, this instance's matrices sent with the draw.
                    if (!native_renderer::hasMesh(id))
                        uploadSkinned(id, packet);
                    if (!paletteHandle || paletteLo != packet.lo || paletteHi != packet.hi)
                    {
                        paletteHandle = native_renderer::framePalette(palette.data() + packet.lo * 4u, packet.hi - packet.lo);
                        paletteLo = packet.lo;
                        paletteHi = packet.hi;
                        if (paletteHandle && g_skinAtKick)
                            g_skinDrawList.push_back({skinRecord(), &packet, 0u, paletteHandle, packet.lo, packet.hi - packet.lo});
                    }
                    if (!paletteHandle)
                        return;
                    skinPalette = paletteHandle;
                    skinPaletteCount = packet.hi - packet.lo;
                    transform = projection;
                }
                // Posed on the CPU (camera space), one mesh per draw this frame.
                else if (fast)
                {
                    // Written straight into the frame's vertex buffer.
                    mesh = (1ull << 63) | g_skinnedDraws++;
                    if (native_renderer::Vertex *out = native_renderer::frameMesh(std::max<uint32_t>(packet.maxVertices, static_cast<uint32_t>(packet.bind.size()))))
                    {
                        const PosedMesh written = poseSkinned(packet, palette.data(), out);
                        native_renderer::commitFrameMesh(mesh, written.vertices, written.indices);
                        if (g_skinAtKick)
                            g_skinDrawList.push_back({skinRecord(), &packet, mesh, 0u, 0u, 0u});
                    }
                    transform = projection;
                }
                else
                {
                    vertices.clear();
                    triangulate(skinnedPacket->strips, [](const BatchVertex &v, native_renderer::Vertex &o)
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
            }
            const bool logPasses = env.logPasses;
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
            const uint32_t skipLayers = env.skipLayers;
            // Layers 12/13 are the reflection pass (reflectionBegin): the world
            // seen in a mirror, drawn by the game into a buffer of its own with
            // the camera context's +0x520 matrix and sampled by the reflecting
            // surfaces (layers 14/15). Never on the screen; not rendered yet.
            const uint32_t layer = info ? info->layer[fading ? 1 : 0] : surfaceLayer(header, s, fading);
            const int logLayer = env.logLayer;
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
                return;
            if ((skipLayers >> (layer & 31u)) & 1u)
                return;
            native_renderer::DrawState state;
            state.skinPalette = skinPalette;
            state.skinPaletteCount = skinPaletteCount;
            const SurfaceInfo *full = info ? &surfaceInfoFull(*info, surface) : nullptr;
            state.texture = full ? full->texture : surfaceTexture(header, surface);
            state.blend = full ? full->blend : surfaceBlend(s);
            state.clamp = full ? full->clamp : surfaceClamp(s);
            state.order = layer; // surfaceLayer above (pure)
            state.opacity = opacity;
            const bool noMips = !native_renderer::tuning().mipmaps;
            if (!noMips)
            {
                if (full)
                    applyLod(full->lod[0], state);
                else
                    surfaceLod(header, surface, state);
            }
            if (logPasses && state.maxLod > 0.0f)
            {
                static std::set<uint64_t> told;
                if (told.insert((static_cast<uint64_t>(header) << 16) | surface).second && told.size() < 30)
                    std::fprintf(stderr, "[mip] header %08x surface %u levels %g L x%g K %g\n", header, surface, state.maxLod + 1.0f, state.lodScale, state.lodK);
            }
            const bool noRow = !native_renderer::tuning().textureScroll;
            if (full && !logPasses)
            {
                if (!noRow && full->scroll)
                {
                    uint32_t pair;
                    std::memcpy(&pair, g_ram + (full->scroll & 0x1FFFFFCu), 4);
                    state.uvOffset[0] = static_cast<float>(static_cast<int32_t>(pair & 0xFFFFu)) / 4096.0f;
                    state.uvOffset[1] = static_cast<float>(static_cast<int32_t>(word(full->scroll + 4u) & 0xFFFFu)) / 4096.0f;
                }
            }
            else if (!noRow && surfaceRow(header, surface, state.uvOffset) && logPasses)
            {
                static std::set<uint64_t> told;
                if (told.insert((static_cast<uint64_t>(header) << 16) | surface).second)
                    std::fprintf(stderr, "[row] header %08x surface %u layer %u offset %g %g\n", header, surface, surfaceLayer(header, s, fading), state.uvOffset[0], state.uvOffset[1]);
            }
            if (R2Check &c = r2Check(); c.on && !full && g_frame % 600u == 0u)
            {
                static uint64_t lastFrame = 0;
                if (lastFrame != g_frame)
                {
                    lastFrame = g_frame;
                    std::fprintf(stderr, "[r2-check] frame %llu (old paths): %llu quick packets, %llu stale; %u garbage moves\n", (unsigned long long)g_frame,
                                 (unsigned long long)c.packetQuick, (unsigned long long)c.packetStale, sotc_decomp::g_guestMoveSerial);
                }
            }
            if (R2Check &c = r2Check(); c.on && full)
            {
                ++c.surfaces;
                native_renderer::DrawState ref;
                surfaceLod(header, surface, ref);
                native_renderer::DrawState mine;
                applyLod(full->lod[0], mine);
                float row[2] = {0.0f, 0.0f};
                surfaceRow(header, surface, row);
                uint64_t f;
                std::memcpy(&f, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
                c.note("texture", surfaceTexture(header, surface) != full->texture);
                c.note("blend", surfaceBlend(s) != full->blend);
                c.note("clamp", surfaceClamp(s) != full->clamp);
                c.note("layer", surfaceLayer(header, s, fading) != layer);
                c.note("lod", std::memcmp(&ref.lodK, &mine.lodK, 4) || ref.lodScale != mine.lodScale || ref.maxLod != mine.maxLod);
                c.note("row", !noRow && (row[0] != state.uvOffset[0] || row[1] != state.uvOffset[1]));
                c.note("flags", f != full->flags || word(s + 0x1Cu) != full->texMode);
                c.note("drawn", !surfaceDrawn(s));
                if (full->texMode == 2u)
                {
                    native_renderer::DrawState ref2, mine2;
                    surfaceLod(header, surface, ref2, 0x3Cu);
                    applyLod(full->lod[1], mine2);
                    c.note("texture2", surfaceTexture(header, surface, 0x3Cu) != full->texture2);
                    c.note("blend2", surfaceBlend(s, 0x43u) != full->blend2);
                    c.note("clamp2", surfaceClamp(s, 0x09u) != full->clamp2);
                    c.note("lod2", std::memcmp(&ref2.lodK, &mine2.lodK, 4) || ref2.lodScale != mine2.lodScale || ref2.maxLod != mine2.maxLod);
                }
                if (g_frame % 600u == 0u)
                {
                    static uint64_t lastFrame = 0;
                    if (lastFrame != g_frame)
                    {
                        lastFrame = g_frame;
                        std::fprintf(stderr, "[r2-check] frame %llu: %llu surface draws checked, %llu mismatches; %llu quick textures, %llu stale; "
                                     "%llu quick packets, %llu stale; %u garbage moves\n",
                                     (unsigned long long)g_frame, (unsigned long long)c.surfaces, (unsigned long long)c.surfaceMismatches,
                                     (unsigned long long)c.texQuick, (unsigned long long)c.texStale, (unsigned long long)c.packetQuick,
                                     (unsigned long long)c.packetStale, sotc_decomp::g_guestMoveSerial);
                    }
                }
            }
            const int idFrom = env.idFrom; // SOTC_NATIVE_IDCOLOR
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
            const bool logMat = env.logMat;
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
            if (known ? e->lit : g_litPackets.count(id))
            {
                // The instance's lights (its own 15 qwords, unpacked to VU1
                // 0x3d8): the light matrix, then colours and ambient; read
                // once for the instance's draws.
                state.lit = true;
                if (!haveLights)
                {
                    const uint32_t own = word(instance + 0xCu);
                    for (uint32_t i = 0; i < 32u; ++i)
                        lights[i] = real(own + 0x10u + i * 4u);
                    haveLights = true;
                }
                std::memcpy(state.light, lights, sizeof(lights));
                if (posed) // skinned: its lights are in camera space
                {
                    std::memcpy(g_viewLights, state.light, sizeof(g_viewLights));
                    g_haveViewLights = true;
                }
            }
            state.reflectionSource = layer == 12u || layer == 13u;
            // Reflecting surfaces (the water): the world's TEST (0x5140b: AREF 0x40,
            // AFAIL FB_ONLY) keeps their colour but not their Z, their vertex alpha
            // being 0x20; the splash rings drawn after on the surface stay visible.
            if (((full ? static_cast<uint32_t>(full->flags) : word(s + 0x18u)) & 0x2200u) == 0x2200u)
                state.noDepthWrite = true;
            const bool twoTextures = (full ? full->texMode : word(s + 0x1Cu)) == 2u && !posed && !state.debugColour;
            if (twoTextures)
                state.writeDepth = true;
            uint64_t sflags;
            if (full)
                sflags = full->flags;
            else
                std::memcpy(&sflags, g_ram + ((s + 0x18u) & 0x1FFFFF8u), 8);
            const bool layer14 = ((sflags >> 25) & 1u) != 0;
            const bool reflective = (layer14 || (sflags & 0x2200u) == 0x2200u) && !noReflect && !state.debugColour;
            if (runs && e && e->resolved && !posed && !twoTextures && !reflective && info && info->texMode != 2u && mesh == id)
            {
                // The first packet of a run (planPackets adds the next ones).
                g_planRun.flush();
                g_planRun.n = 1;
                g_planRun.s = s;
                g_planRun.lit = state.lit;
                g_planRun.transform = transform;
                g_planRun.state = state;
                g_planRun.ids.assign(1u, mesh);
                ++g_frameDraws;
                ++g_modelDraws;
                hs.draws += hs.on;
                return;
            }
            g_planRun.flush();
            native_renderer::draw(mesh, transform, state);
            ++g_frameDraws;
            ++g_modelDraws;
            hs.draws += hs.on;
            if (twoTextures)
            {
                // The microcode draws the batch again with the second texture
                // and coordinates, blended by ALPHA_2 (context 2).
                native_renderer::DrawState second = state;
                second.texture = full ? full->texture2 : surfaceTexture(header, surface, 0x3Cu);
                second.uvOffset[0] = second.uvOffset[1] = 0.0f; // the second pair takes ROW z / w (0)
                second.maxLod = 0.0f;
                if (!noMips)
                {
                    if (full)
                        applyLod(full->lod[1], second);
                    else
                        surfaceLod(header, surface, second, 0x3Cu);
                }
                second.blend = full ? full->blend2 : surfaceBlend(s, 0x43u);
                second.clamp = full ? full->clamp2 : surfaceClamp(s, 0x09u);
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
            if (reflective)
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
                const float tintAlpha = env.tintAlpha;
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
                const float refractAlpha = env.refractAlpha;
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
                    const bool showRefraction = env.showRefraction; // debugging: where it lands, in red
                    if (showRefraction)
                    {
                        refr.reflectionTint[0] = 4.0f;
                        refr.reflectionTint[1] = refr.reflectionTint[2] = 0.0f;
                    }
                    native_renderer::draw(mesh, transform, refr);
                    ++g_frameDraws;
                }
            }
        };
        // The packet list as the game has it (no plan).
        const auto walkPackets = [&]
        {
            for (uint32_t k = 0; k < count; ++k)
            {
                const uint32_t pk = packets + k * 32u;
                if (word(pk) == 0u)
                    break;
                hs.packets += hs.on;
                if (g_prefetch)
                {
                    // The next packets' memo lines (the tags two ahead, the entry
                    // and the surface's state one ahead), the next draw's lines.
                    const uint32_t next = pk + 32u;
                    if (word(next) != 0u)
                    {
                        if (word(next + 32u) != 0u)
                            g_packetMemo.prefetch(word(next + 48u), false);
                        g_packetMemo.prefetch(word(next + 16u), true);
                        __builtin_prefetch(&g_surfaceInfo[surfaceSlot(word(header + 0x50u) + word(next + 8u) * 0x120u)]);
                    }
                    g_prefetchQueue.pump(2u);
                }
                if (word(pk + 4u) != lod)
                    continue;
                hs.lodPackets += hs.on;
                const uint32_t part = word(pk + 12u);
                if (partCulled && part != 0xFFFFFFFFu && part < partVisible.size() && partVisible[part] == 1u)
                    continue;
                const uint32_t surface = word(pk + 8u), s = word(header + 0x50u) + surface * 0x120u;
                SurfaceInfo *info = memo ? &surfaceInfo(header, s) : nullptr;
                if (info ? !surfaceDrawnFlags(info->flags) : !surfaceDrawn(s))
                    continue;
                if (hs.on)
                    ++hs.surfaces, hs.surfaceSet.insert(s);
                const uint32_t size = word(pk), data = word(pk + 16u);
                uint64_t id;
                {
                    SOTC_PROF_SCOPE(kHookPacketId);
                    id = packetId(data, size, fast);
                }
                drawPacket(k, surface, s, info, size, data, id, g_packetChangedInPlace, nullptr);
            }
        };
        // The plan's packets: the same tests, the id kept while packetId
        // would keep it without looking (drawn the frame before, not its
        // stagger frame, not rewritten in place).
        ModelPlan *plan = planOn ? (lod == gameLod ? gamePlan : modelPlan(header, lod)) : nullptr;
        const auto planPackets = [&]
        {
            const size_t n = plan->pk.size();
            for (size_t k = 0; k < n; ++k)
            {
                PlanPacket &e = plan->pk[k];
                if (hs.on)
                    ++hs.packets, ++hs.lodPackets;
                if (g_prefetch)
                {
                    if (k + 1u < n)
                        __builtin_prefetch(&g_surfaceInfo[plan->pk[k + 1u].slot]);
                    g_prefetchQueue.pump(2u);
                }
                if (partCulled && e.part != 0xFFFFFFFFu && e.part < partVisible.size() && partVisible[e.part] == 1u)
                    continue;
                SurfaceInfo *info = &g_surfaceInfo[e.slot];
                if (!(info->s == e.s && info->header == header && info->frame == g_frame))
                    info = &surfaceInfo(header, e.s);
                if (!surfaceDrawnFlags(info->flags))
                    continue;
                if (hs.on)
                    ++hs.surfaces, hs.surfaceSet.insert(e.s);
                uint64_t id;
                bool changedInPlace = false;
                if (e.idFrame == g_frame || (!e.rewritten && e.idFrame + 1u == g_frame && ((g_frame + (e.data >> 5)) & 15u) != 0u))
                {
                    id = e.id;
                    ++g_planStats.trusted;
                }
                else
                {
                    {
                        SOTC_PROF_SCOPE(kHookPacketId);
                        id = packetId(e.data, e.size, true);
                    }
                    changedInPlace = g_packetChangedInPlace;
                    e.rewritten = g_packetRewritten;
                    if (id != e.id)
                    {
                        e.id = id;
                        e.resolved = false;
                    }
                    ++g_planStats.asked;
                }
                e.idFrame = g_frame;
                if (g_planRun.n && runs && e.resolved && e.ready && !e.skinned && e.s == g_planRun.s && e.lit == g_planRun.lit && id == e.id && !changedInPlace)
                {
                    // The run's draw, for this packet's mesh too.
                    g_planRun.ids.push_back(id);
                    ++g_planRun.n;
                    ++g_frameDraws;
                    ++g_modelDraws;
                    hs.draws += hs.on;
                    if (planStatsOn())
                        ++g_planStats.drawn;
                    continue;
                }
                if (planStatsOn())
                    ++g_planStats.drawn;
                drawPacket(static_cast<uint32_t>(k), e.surface, e.s, info, e.size, e.data, id, changedInPlace, &e);
            }
            g_planRun.flush();
        };
        // SOTC_PLAN_VERIFY=<n>: every n-th frame the old walk too, dry, and
        // the two lists of draw hashes compared.
        const bool verify = plan && g_planVerify && g_frame % g_planVerify == 0u && !native_renderer::drawTap();
        if (!verify)
        {
            if (plan)
                planPackets();
            else
                walkPackets();
            return;
        }
        static std::vector<uint64_t> fromPlan, fromWalk;
        fromPlan.clear();
        fromWalk.clear();
        native_renderer::setDrawTap(&fromPlan);
        planPackets();
        native_renderer::setDrawTap(&fromWalk);
        native_renderer::setDrawTapOnly(true);
        walkPackets();
        native_renderer::setDrawTapOnly(false);
        native_renderer::setDrawTap(nullptr);
        PlanStats &ps = g_planStats;
        ++ps.checks;
        ps.draws += fromWalk.size();
        size_t differ = fromPlan.size() != fromWalk.size() ? 1u : 0u;
        ps.countDiffer += differ;
        for (size_t i = 0; i < std::min(fromPlan.size(), fromWalk.size()); ++i)
            differ += fromPlan[i] != fromWalk[i];
        ps.differ += differ;
        if (differ && ps.differ < 40u)
            std::fprintf(stderr, "[plan-verify] frame %llu instance %08x header %08x lod %u: %zu draws (plan) / %zu (walk), %zu differ\n",
                         (unsigned long long)g_frame, instance, header, lod, fromPlan.size(), fromWalk.size(), differ);
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
    if (runtime.hasFunction(kSpriteTexFast))
    {
        g_spriteTexFast = runtime.lookupFunction(kSpriteTexFast);
        runtime.registerFunction(kSpriteTexFast, spriteTexFastHook);
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
        // At each display list's kick: the skinned parts as its DMA reads them
        // (skinAtKick), and the debugging checks.
        ps2_stats::kickHook = []
        {
            if (sotc_native::g_staleCheck)
            {
                sotc_native::staleCompare("kick");
                sotc_native::staleCompare("kick2 (the frame before's)", sotc_native::g_stalePrevious);
            }
            if (sotc_native::g_skinAtKick)
                sotc_native::skinAtKick();
        };
    }
    if (runtime.hasFunction(kZFog))
    {
        g_zFog = runtime.lookupFunction(kZFog);
        runtime.registerFunction(kZFog, zFogHook);
    }
    std::cerr << "[sotc:native] models drawn by the native renderer" << std::endl;
}
