#include <map>
#include <chrono>
#include "runtime/native_renderer.h"
#include "runtime/ps2_stats.h"
#include "runtime/gs/gs_parallel.h"
#include <climits>

namespace
{
    // Row band rendered by the current thread while flushing the primitive
    // queue in parallel (full range when drawing immediately).
    // CLUT version of the draw being rasterized by a band worker (null: current).
    thread_local const uint16_t *t_clut = nullptr;
    // Decoded texture of the draw being rasterized by a band worker (null: sample VRAM).
    thread_local const void *t_texture = nullptr;
    // Copy of VRAM that a feedback draw samples instead of the live memory.
    thread_local const uint8_t *t_textureVram = nullptr;
    const bool kTextureCacheEnabled = std::getenv("PS2X_GS_NOTEXCACHE") == nullptr;
    void bufferRange(uint32_t baseBytes, uint32_t bufferWidthPixels, uint32_t heightPixels, uint32_t psm,
                     uint64_t &begin, uint64_t &end);
    thread_local int t_bandY0 = INT_MIN;
    // Rows per parallel raster band (PS2X_GS_BAND_ROWS overrides).
    int bandRows()
    {
        static const int rows = []
        {
            const char *env = std::getenv("PS2X_GS_BAND_ROWS");
            return env ? std::clamp(std::atoi(env), 2, 256) : 8;
        }();
        return rows;
    }
    thread_local int t_bandY1 = INT_MAX;
    bool inBandMode() { return t_bandY0 != INT_MIN; }
}
#include <set>
#include <sstream>
#include <cstdlib>
#include "runtime/gs/gs_cpu_backend.h"
#include "runtime/gs/ps2_gs_common.h"
#include "runtime/gs/ps2_gs_psmct16.h"
#include "runtime/gs/ps2_gs_psmct32.h"
#include "runtime/gs/ps2_gs_psmt4.h"
#include "runtime/gs/ps2_gs_psmt8.h"
#include "runtime/gs/ps2_gs_memory.h"
#include "runtime/gs/gs_metal_raster.h"
#include "ps2_log.h"
#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace GSInternal;

// No fused multiply-add: plain IEEE float arithmetic, so the GPU rasterizer
// (which disables contraction too) reproduces these results bit for bit.
#pragma clang fp contract(off)

namespace
{
    // Interpolation with explicit fused multiply-adds, written identically in
    // the GPU shaders so both renderers round the same way.
    inline float interp3(float a, float b, float c, float w0, float w1, float w2)
    {
        return std::fma(c, w2, std::fma(b, w1, a * w0));
    }

    inline float lerpf(float a, float b, float t)
    {
        return std::fma(b - a, t, a);
    }

    // Triangle Z as a float pair (hi + lo). The GPU rasterizer runs the very
    // same operations, so both round every pixel's Z identically.
    struct DepthPair
    {
        float hi, lo;
    };

    inline DepthPair twoSum(float a, float b)
    {
        const float s = a + b;
        const float bb = s - a;
        return {s, (a - (s - bb)) + (b - bb)};
    }

    inline DepthPair depthAdd(DepthPair a, DepthPair b)
    {
        DepthPair s = twoSum(a.hi, b.hi);
        s.lo += a.lo + b.lo;
        return twoSum(s.hi, s.lo);
    }

    inline DepthPair depthMul(float w, DepthPair z)
    {
        const float p = w * z.hi;
        const float e = std::fma(w, z.hi, -p);
        return twoSum(p, e + w * z.lo);
    }

    inline DepthPair depthSplit(double z)
    {
        const float hi = static_cast<float>(z);
        return {hi, static_cast<float>(z - static_cast<double>(hi))};
    }

    inline uint32_t depthRound(DepthPair z)
    {
        const DepthPair h = depthAdd(z, {0.5f, 0.0f});
        const float fh = std::floor(h.hi);
        const float rem = (h.hi - fh) + h.lo;
        const int64_t value = static_cast<int64_t>(fh) + static_cast<int64_t>(std::floor(rem));
        return static_cast<uint32_t>(std::clamp<int64_t>(value, 0, 0xFFFFFFFFll));
    }

    float fabsQ(float q)
    {
        return (std::fabs(q) > 1.0e-8f) ? q : 1.0f;
    }

    u16 Rgba8888ToRgba5551(u32 c)
    {
        uint32_t r = ((c >> 0) & 0xFF) >> 3;
        uint32_t g = ((c >> 8) & 0xFF) >> 3;
        uint32_t b = ((c >> 16) & 0xFF) >> 3;
        uint32_t a = ((c >> 24) & 0xFF) >> 7;

        return (r | (g << 5) | (b << 10) | (a << 15));
    }

    u32 Rgba5551ToRgba8888(u16 c)
    {
        u32 r = ((c >> 0) & 0x1F) << 3;
        u32 g = ((c >> 5) & 0x1F) << 3;
        u32 b = ((c >> 10) & 0x1F) << 3;
        u32 a = ((c >> 15) & 0x01) << 7;

        return (r | (g << 8) | (b << 16) | (a << 24));
    }

    u32 pack32(u8 r, u8 g, u8 b, u8 a)
    {
        return static_cast<u32>(r) | (g << 8) | (b << 16) | (a << 24);
    }

    uint32_t applyTexa(const GSTexaReg &texa, uint8_t psm, uint32_t texel)
    {
        if (psm == GS_PSM_CT32)
            return texel;

        const uint8_t r = static_cast<uint8_t>(texel & 0xFFu);
        const uint8_t g = static_cast<uint8_t>((texel >> 8) & 0xFFu);
        const uint8_t b = static_cast<uint8_t>((texel >> 16) & 0xFFu);
        const bool rgbZero = r == 0u && g == 0u && b == 0u;
        uint8_t a = static_cast<uint8_t>((texel >> 24) & 0xFFu);

        switch (psm)
        {
        case GS_PSM_CT24:
            a = (texa.aem && rgbZero) ? 0u : texa.ta0;
            break;
        case GS_PSM_CT16:
        case GS_PSM_CT16S:
            if ((a & 0x80u) != 0u)
                a = texa.ta1;
            else
                a = (texa.aem && rgbZero) ? 0u : texa.ta0;
            break;
        default:
            break;
        }

        return (texel & 0x00FFFFFFu) | (static_cast<uint32_t>(a) << 24);
    }

    uint32_t addrPSMCT16Family(uint32_t basePtr, uint32_t width, uint8_t psm, uint32_t x, uint32_t y)
    {
        switch (psm)
        {
        case GS_PSM_CT16:
            return GSPSMCT16::addrPSMCT16(basePtr, width, x, y);
        case GS_PSM_CT16S:
            return GSPSMCT16::addrPSMCT16S(basePtr, width, x, y);
        case GS_PSM_Z16:
            return GSPSMCT16::addrPSMZ16(basePtr, width, x, y);
        case GS_PSM_Z16S:
            return GSPSMCT16::addrPSMZ16S(basePtr, width, x, y);
        default:
            return 0u;
        }
    }

    std::atomic<uint32_t> s_debugPrimitiveCount{0};
    std::atomic<uint32_t> s_debugPixelCount{0};
    std::atomic<uint32_t> s_debugContext1PrimitiveCount{0};
    std::atomic<uint32_t> s_debugFbp150PixelCount{0};

    int wrapTextureCoordinate(int coordinate,
                              int textureSize,
                              uint8_t mode,
                              uint16_t regionMin,
                              uint16_t regionMax)
    {
        switch (mode & 0x3u)
        {
        case 0: // REPEAT
            return static_cast<int>(static_cast<uint32_t>(coordinate) & static_cast<uint32_t>(textureSize - 1));
        case 1: // CLAMP
            return clampInt(coordinate, 0, textureSize - 1);
        case 2: // REGION_CLAMP
            return std::min(std::max(coordinate, static_cast<int>(regionMin)), static_cast<int>(regionMax));
        case 3: // REGION_REPEAT
            return static_cast<int>((static_cast<uint32_t>(coordinate) & static_cast<uint32_t>(regionMin)) | static_cast<uint32_t>(regionMax));
        default:
            return coordinate;
        }
    }

    bool passesAlphaTest(uint64_t testReg, uint8_t alpha)
    {
        if ((testReg & 0x1u) == 0u)
            return true;

        const uint8_t atst = static_cast<uint8_t>((testReg >> 1) & 0x7u);
        const uint8_t aref = static_cast<uint8_t>((testReg >> 4) & 0xFFu);

        switch (atst)
        {
        case 0:
            return false;
        case 1:
            return true;
        case 2:
            return alpha < aref;
        case 3:
            return alpha <= aref;
        case 4:
            return alpha == aref;
        case 5:
            return alpha >= aref;
        case 6:
            return alpha > aref;
        case 7:
            return alpha != aref;
        default:
            return true;
        }
    }

    struct PixelWriteMask
    {
        bool writeRgb = true;
        bool writeAlpha = true;
        bool writeDepth = true;

        bool writesFramebuffer() const
        {
            return writeRgb || writeAlpha;
        }

        bool writesAnything() const
        {
            return writesFramebuffer() || writeDepth;
        }
    };

    PixelWriteMask classifyAlphaTest(uint64_t testReg, uint8_t alpha, uint8_t framePsm)
    {
        const bool pass = passesAlphaTest(testReg, alpha);
        if (pass)
            return {};

        // TEST.AFAIL controls what happens when the alpha comparison fails.
        switch (static_cast<uint8_t>((testReg >> 12) & 0x3u))
        {
        case 1: // FB_ONLY
            return {true, true, false};
        case 2: // ZB_ONLY
            return {false, false, true};
        case 3: // RGB_ONLY
            // RGB_ONLY is only distinct for RGBA32. The GS treats it as
            // FB_ONLY for RGB24 and RGBA16 framebuffers.
            if (framePsm == GS_PSM_CT32)
                return {true, false, false};
            return {true, true, false};
        case 0: // KEEP
        default:
            return {false, false, false};
        }
    }

    bool passesDestinationAlphaTest(uint64_t testReg, uint8_t framePsm, uint32_t rawFramebufferPixel)
    {
        const bool date = ((testReg >> 14) & 0x1u) != 0u;
        if (!date)
            return true;

        const bool datm = ((testReg >> 15) & 0x1u) != 0u;
        switch (framePsm)
        {
        case GS_PSM_CT32:
            return (((rawFramebufferPixel >> 31) & 0x1u) != 0u) == datm;
        case GS_PSM_CT16:
        case GS_PSM_CT16S:
            return (((rawFramebufferPixel >> 15) & 0x1u) != 0u) == datm;
        case GS_PSM_CT24:
            // RGB24 has no destination alpha, so DATE always passes.
            return true;
        default:
            return true;
        }
    }

    struct TextureCombineResult
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    TextureCombineResult combineTexture(const GSTex0Reg &tex,
                                        uint8_t vr,
                                        uint8_t vg,
                                        uint8_t vb,
                                        uint8_t va,
                                        uint8_t tr,
                                        uint8_t tg,
                                        uint8_t tb,
                                        uint8_t ta)
    {
        const bool textureHasAlpha = tex.tcc != 0u;
        TextureCombineResult out{tr, tg, tb, textureHasAlpha ? ta : va};

        switch (tex.tfx)
        {
        case 0: // MODULATE
            out.r = clampU8((tr * vr) >> 7);
            out.g = clampU8((tg * vg) >> 7);
            out.b = clampU8((tb * vb) >> 7);
            out.a = textureHasAlpha ? clampU8((ta * va) >> 7) : va;
            break;
        case 1: // DECAL
            out.r = tr;
            out.g = tg;
            out.b = tb;
            out.a = textureHasAlpha ? ta : va;
            break;
        case 2: // HIGHLIGHT
            out.r = clampU8(((tr * vr) >> 7) + va);
            out.g = clampU8(((tg * vg) >> 7) + va);
            out.b = clampU8(((tb * vb) >> 7) + va);
            out.a = textureHasAlpha ? clampU8(ta + va) : va;
            break;
        case 3: // HIGHLIGHT2
            out.r = clampU8(((tr * vr) >> 7) + va);
            out.g = clampU8(((tg * vg) >> 7) + va);
            out.b = clampU8(((tb * vb) >> 7) + va);
            out.a = textureHasAlpha ? ta : va;
            break;
        default:
            out.r = tr;
            out.g = tg;
            out.b = tb;
            out.a = textureHasAlpha ? ta : va;
            break;
        }

        return out;
    }

    uint32_t swizzleClutIndexCSM1(uint32_t index)
    {
        // CSM1 swaps address bits 3 and 4. Preserve the remaining bits:
        // 16-bit CLUTs expose a ninth address bit through CSA[4].
        return (index & ~0x18u) | ((index & 0x08u) << 1u) | ((index & 0x10u) >> 1u);
    }

    bool isFourBitIndexedPsm(uint8_t psm)
    {
        return psm == GS_PSM_T4 || psm == GS_PSM_T4HL || psm == GS_PSM_T4HH;
    }

    bool isEightBitIndexedPsm(uint8_t psm)
    {
        return psm == GS_PSM_T8 || psm == GS_PSM_T8H;
    }

    uint8_t lerpChannel(uint8_t c00, uint8_t c10, uint8_t c01, uint8_t c11, float fx, float fy)
    {
        const float top = lerpf(static_cast<float>(c00), static_cast<float>(c10), fx);
        const float bottom = lerpf(static_cast<float>(c01), static_cast<float>(c11), fx);
        return clampU8(static_cast<int>(std::lround(lerpf(top, bottom, fy))));
    }
}

namespace
{
    static constexpr uint32_t kDefaultDisplayWidth = 640u;
    static constexpr uint32_t kDefaultDisplayHeight = 448u;
    static constexpr uint32_t kHostFrameWidth = 640u;
    static constexpr uint32_t kHostFrameHeight = 512u;

    uint16_t encodeFramePixelPSMCT16(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        return static_cast<uint16_t>(((r >> 3) & 0x1Fu) |
                                     (((g >> 3) & 0x1Fu) << 5) |
                                     (((b >> 3) & 0x1Fu) << 10) |
                                     ((a >= 0x40u) ? 0x8000u : 0u));
    }

    void decodeDisplaySize(uint64_t display64, uint32_t &outWidth, uint32_t &outHeight)
    {
        const uint32_t dw = static_cast<uint32_t>((display64 >> 32) & 0x0FFFu);
        const uint32_t dh = static_cast<uint32_t>((display64 >> 44) & 0x07FFu);
        const uint32_t magh = static_cast<uint32_t>((display64 >> 23) & 0x0Fu);

        outWidth = (dw + 1u) / (magh + 1u);
        outHeight = dh + 1u;
        if (outWidth < 64u || outHeight < 64u)
        {
            outWidth = kDefaultDisplayWidth;
            outHeight = kDefaultDisplayHeight;
        }
        outWidth = std::min<uint32_t>(outWidth, kHostFrameWidth);
        outHeight = std::min<uint32_t>(outHeight, kHostFrameHeight);
    }

    GSFrameReg decodeDisplayFrame(uint64_t dispfb64)
    {
        GSFrameReg frame{};
        frame.fbp = static_cast<uint32_t>(dispfb64 & 0x1FFu);
        frame.fbw = static_cast<uint32_t>((dispfb64 >> 9) & 0x3Fu);
        frame.psm = static_cast<uint8_t>((dispfb64 >> 15) & 0x1Fu);
        return frame;
    }

    struct GSDisplayReadOrigin
    {
        uint32_t x = 0u;
        uint32_t y = 0u;
    };

    GSDisplayReadOrigin decodeDisplayReadOrigin(uint64_t dispfb64)
    {
        return {
            static_cast<uint32_t>((dispfb64 >> 32) & 0x7FFu),
            static_cast<uint32_t>((dispfb64 >> 43) & 0x7FFu)};
    }

    bool hasDisplaySetup(uint64_t display64, const GSFrameReg &frame)
    {
        const uint32_t dw = static_cast<uint32_t>((display64 >> 32) & 0x0FFFu);
        const uint32_t dh = static_cast<uint32_t>((display64 >> 44) & 0x07FFu);
        const uint32_t magh = static_cast<uint32_t>((display64 >> 23) & 0x0Fu);
        return frame.fbw != 0u || dw != 0u || dh != 0u || magh != 0u;
    }

    struct GSPmodeState
    {
        bool enableCrt1 = false;
        bool enableCrt2 = false;
        bool mmod = false;
        bool amod = false;
        bool slbg = false;
        uint8_t alp = 0u;
    };

    GSPmodeState decodePmode(uint64_t pmode64)
    {
        return {
            (pmode64 & 0x1ull) != 0ull,
            (pmode64 & 0x2ull) != 0ull,
            ((pmode64 >> 5) & 0x1ull) != 0ull,
            ((pmode64 >> 6) & 0x1ull) != 0ull,
            ((pmode64 >> 7) & 0x1ull) != 0ull,
            static_cast<uint8_t>((pmode64 >> 8) & 0xFFu)};
    }

    struct GSSmode2State
    {
        bool interlaced = false;
        bool frameMode = true;
    };

    GSSmode2State decodeSMode2(uint64_t smode2)
    {
        return {(smode2 & 0x1ull) != 0ull, ((smode2 >> 1) & 0x1ull) != 0ull};
    }

    void applyFieldPresentation(std::vector<uint8_t> &pixels, uint32_t width, uint32_t height, bool oddField)
    {
        if (pixels.empty() || width == 0u || height < 2u)
            return;
        const std::vector<uint8_t> source = pixels;
        for (uint32_t y = 0; y < height; ++y)
        {
            uint32_t sourceY = ((y >> 1u) << 1u) + (oddField ? 1u : 0u);
            if (sourceY >= height)
                sourceY = height - 1u;
            std::memcpy(pixels.data() + y * kHostFrameWidth * 4u,
                        source.data() + sourceY * kHostFrameWidth * 4u,
                        width * 4u);
        }
    }

    void normalizePresentationAlpha(std::vector<uint8_t> &pixels, uint32_t width, uint32_t height)
    {
        for (uint32_t y = 0; y < height; ++y)
        {
            uint8_t *row = pixels.data() + y * kHostFrameWidth * 4u;
            for (uint32_t x = 0; x < width; ++x)
                row[x * 4u + 3u] = 255u;
        }
    }

    uint8_t blendPresentationChannel(uint8_t src, uint8_t dst, uint32_t factor)
    {
        const int delta = static_cast<int>(src) - static_cast<int>(dst);
        return GSInternal::clampU8(static_cast<int>(dst) + ((delta * static_cast<int>(factor)) / 255));
    }

    uint32_t countNonBlackPixels(const std::vector<uint8_t> &pixels, uint32_t width, uint32_t height)
    {
        uint32_t count = 0u;
        for (uint32_t y = 0; y < height; ++y)
        {
            const uint8_t *row = pixels.data() + y * kHostFrameWidth * 4u;
            for (uint32_t x = 0; x < width; ++x)
            {
                if (row[x * 4u] != 0u || row[x * 4u + 1u] != 0u || row[x * 4u + 2u] != 0u)
                    ++count;
            }
        }
        return count;
    }
}

GSCpuBackend::GSCpuBackend()
{
    using namespace GSMem;
    static std::once_flag lookupTablesOnce;
    std::call_once(lookupTablesOnce, []()
                   { InitLookupTables(); });
    for (size_t i = 0; i < kPsmHandlerCount; ++i)
    {
        switch (i)
        {
        case GS_PSM_CT32:
            m_readVramFuncs[i] = ReadCT32;
            m_writeVramFuncs[i] = WriteCT32;
            break;
        case GS_PSM_CT24:
            m_readVramFuncs[i] = ReadCT24;
            m_writeVramFuncs[i] = WriteCT24;
            break;
        case GS_PSM_CT16:
            m_readVramFuncs[i] = ReadCT16;
            m_writeVramFuncs[i] = WriteCT16;
            break;
        case GS_PSM_CT16S:
            m_readVramFuncs[i] = ReadCT16S;
            m_writeVramFuncs[i] = WriteCT16S;
            break;
        case GS_PSM_T8:
            m_readVramFuncs[i] = ReadP8;
            m_writeVramFuncs[i] = WriteP8;
            break;
        case GS_PSM_T8H:
            m_readVramFuncs[i] = ReadP8H;
            m_writeVramFuncs[i] = WriteP8H;
            break;
        case GS_PSM_T4:
            m_readVramFuncs[i] = ReadP4;
            m_writeVramFuncs[i] = WriteP4;
            break;
        case GS_PSM_T4HH:
            m_readVramFuncs[i] = ReadP4HH;
            m_writeVramFuncs[i] = WriteP4HH;
            break;
        case GS_PSM_T4HL:
            m_readVramFuncs[i] = ReadP4HL;
            m_writeVramFuncs[i] = WriteP4HL;
            break;
        case GS_PSM_Z32:
            m_readVramFuncs[i] = ReadZ32;
            m_writeVramFuncs[i] = WriteZ32;
            break;
        case GS_PSM_Z24:
            m_readVramFuncs[i] = ReadZ24;
            m_writeVramFuncs[i] = WriteZ24;
            break;
        case GS_PSM_Z16:
            m_readVramFuncs[i] = ReadZ16;
            m_writeVramFuncs[i] = WriteZ16;
            break;
        case GS_PSM_Z16S:
            m_readVramFuncs[i] = ReadZ16S;
            m_writeVramFuncs[i] = WriteZ16S;
            break;
        default:
            m_readVramFuncs[i] = ReadNull;
            m_writeVramFuncs[i] = WriteNull;
            break;
        }
    }
    Reset();
}

void GSCpuBackend::Initialize(uint8_t *vram, uint32_t vramSize)
{
    if (vram && vramSize < GSMem::MEMORY_SIZE)
        throw std::invalid_argument("GS CPU backend requires at least 4 MiB of VRAM");

    std::lock_guard<std::mutex> lock(m_mutex);
    m_vram = vram;
    m_vramSize = vramSize;
    ResetUnlocked();
#ifdef PS2X_HAVE_METAL
    // Rasterize on the GPU by default (only the backend of the live GS memory;
    // scratch backends used for presentation stay on the CPU).
    // PS2X_GS_GPU=0 keeps everything on the CPU.
    static const bool wantGpu = !std::getenv("PS2X_GS_GPU") || std::atoi(std::getenv("PS2X_GS_GPU")) != 0;
    m_gpu.reset();
    if (wantGpu && m_gpuAllowed && vram)
        m_gpu = GSMetalRaster::create(vram, vramSize);
#endif
}

void GSCpuBackend::Reset()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    flushPendingUnlocked("Reset");
    ResetUnlocked();
}

void GSCpuBackend::ResetUnlocked()
{
    m_clut.fill(0u);
    m_clutCbp.fill(0u);
    m_texturePageCache.Invalidate();
    m_transfer = {};
    m_transfer.direction = 3u;
    m_transferState = {};
    m_transferState.direction = 3u;
    m_localToHostBuffer.clear();
    m_localToHostReadPos = 0u;
    m_textureCache.clear();
    m_textureCacheBytes = 0;
    markVramWritten(0u, static_cast<uint64_t>(m_pageEpoch.size()) * 8192u);
}

namespace
{
    // PS2X_GS_PROFILE=1: per-second split of raster time (serial feedback draws
    // vs banded parallel flushes) and how often the queue is flushed.
    struct GsProfile
    {
        bool on = std::getenv("PS2X_GS_PROFILE") != nullptr;
        double serial = 0, banded = 0;
        uint64_t serialDraws = 0, bandedDraws = 0, flushes = 0, hazardFlushes = 0;
        std::map<std::string, uint64_t> reasons;
        std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
        void report()
        {
            const auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration<double>(now - last).count() < 1.0)
                return;
            last = now;
            std::fprintf(stderr, "[gsprof] serial=%.0fms (%llu draws) banded=%.0fms (%llu draws, %llu flushes, %llu hazard)\n",
                         serial * 1000, (unsigned long long)serialDraws, banded * 1000, (unsigned long long)bandedDraws,
                         (unsigned long long)flushes, (unsigned long long)hazardFlushes);
            for (const auto &[why, n] : reasons)
                std::fprintf(stderr, "[gsprof]   %s %llu\n", why.c_str(), (unsigned long long)n);
            reasons.clear();
            serial = banded = 0;
            serialDraws = bandedDraws = flushes = hazardFlushes = 0;
        }
    } g_gsProfile;
}

#ifdef PS2X_HAVE_METAL
namespace
{
    bool gpuCanDraw(const GSPrimitiveBatch &batch);
}
#endif

void GSCpuBackend::Submit(const GSPrimitiveBatch &batch)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_vram || batch.vertexCount == 0u)
        return;
    ps2_stats::gsPrims[static_cast<uint32_t>(batch.state.prim.type) & 7u].fetch_add(1, std::memory_order_relaxed);
    // PS2X_GS_FAN_DUMP=<dir> (debugging, round 27): a triangle fan into a
    // 256-wide target (FBW 4) scissored to 256 x 256 (SotC's
    // bossDeathEffectBegin mask): at the next other draw, that target's
    // 256 x 256 CT32 alpha as the GS left it, as a PGM (alpha != 0: 255).
    static const char *fanDump = std::getenv("PS2X_GS_FAN_DUMP");
    if (fanDump)
    {
        static int armedFbp = -1, dumps = 0;
        const auto &c = batch.state.context;
        const bool fan = batch.state.prim.type == GS_PRIM_TRIFAN && c.frame.fbw == 4u && batch.vertices[0].r == 0u && batch.vertices[0].a == 128u;
        if (fan && dumps < 50)
            std::fprintf(stderr, "[gs:fan] fbp %u fbw %u sc %u %u ofs %u %u n %u v0 %.2f %.2f v1 %.2f %.2f rgba %u %u %u %u alpha %llx test %llx\n", c.frame.fbp, c.frame.fbw, c.scissor.x1, c.scissor.y1, c.xyoffset.ofx >> 4, c.xyoffset.ofy >> 4, batch.vertexCount, batch.vertices[0].x,
                         batch.vertices[0].y, batch.vertices[1].x, batch.vertices[1].y, batch.vertices[0].r, batch.vertices[0].g, batch.vertices[0].b, batch.vertices[0].a,
                         (unsigned long long)c.alpha, (unsigned long long)c.test);
        if (!fan && armedFbp >= 0 && dumps < 50)
        {
            flushPendingUnlocked("FanDump");
            finishPendingUnlocked();
            char path[512];
            std::snprintf(path, sizeof(path), "%s/gsfan_%03d_fbp%d.pgm", fanDump, dumps++, armedFbp);
            if (FILE *f = std::fopen(path, "wb"))
            {
                std::fprintf(f, "P5 256 256 255\n");
                for (uint32_t y = 0; y < 256u; ++y)
                    for (uint32_t x = 0; x < 256u; ++x)
                        std::fputc((ReadVramUnlocked(0u, static_cast<uint32_t>(armedFbp) * 32u, 4u, x, y) >> 24) ? 255 : 0, f);
                std::fclose(f);
            }
            armedFbp = -1;
        }
        if (fan)
            armedFbp = static_cast<int>(c.frame.fbp);
    }
    // PS2X_GS_SKIP_TBP=<tbp0>: debugging aid, drop textured draws from that address.
    static const long skipTbp = std::getenv("PS2X_GS_SKIP_TBP") ? std::atol(std::getenv("PS2X_GS_SKIP_TBP")) : -1;
    if (skipTbp >= 0 && batch.state.prim.tme && batch.state.context.tex0.tbp0 == static_cast<uint32_t>(skipTbp))
        return;

    // PS2X_GS_TRACE_ALL_AT=<seconds>: log every primitive (type, target, texture,
    // bounding box) for a short window starting at that time.
    static const double traceAllAt = std::getenv("PS2X_GS_TRACE_ALL_AT") ? std::atof(std::getenv("PS2X_GS_TRACE_ALL_AT")) : -1.0;
    if (traceAllAt >= 0.0)
    {
        static const auto start = std::chrono::steady_clock::now();
        static int logged = 0;
        const double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (t >= traceAllAt && logged < 20000)
        {
            ++logged;
            const auto &c = batch.state.context;
            float x0 = 1e9f, y0 = 1e9f, x1 = -1e9f, y1 = -1e9f;
            for (uint32_t i = 0; i < batch.vertexCount; ++i)
            {
                x0 = std::min(x0, batch.vertices[i].x - (c.xyoffset.ofx >> 4));
                y0 = std::min(y0, batch.vertices[i].y - (c.xyoffset.ofy >> 4));
                x1 = std::max(x1, batch.vertices[i].x - (c.xyoffset.ofx >> 4));
                y1 = std::max(y1, batch.vertices[i].y - (c.xyoffset.ofy >> 4));
            }
            std::fprintf(stderr, "[gs:all] type=%u fbp=%u tme=%u tbp=%u psm=0x%x abe=%u bbox=(%.0f,%.0f)-(%.0f,%.0f) rgba=(%u,%u,%u,%u)\n",
                         (unsigned)batch.state.prim.type, c.frame.fbp, (unsigned)batch.state.prim.tme, c.tex0.tbp0, (unsigned)c.tex0.psm,
                         (unsigned)batch.state.prim.abe, x0, y0, x1, y1, batch.vertices[0].r, batch.vertices[0].g, batch.vertices[0].b, batch.vertices[0].a);
        }
    }
    // PS2X_GS_TRACE_AT=<seconds>: log the sprite draws (post-processing passes)
    // issued during a short window starting at that time.
    static const double traceAt = std::getenv("PS2X_GS_TRACE_AT") ? std::atof(std::getenv("PS2X_GS_TRACE_AT")) : -1.0;
    if (traceAt >= 0.0 && batch.state.prim.type == GS_PRIM_SPRITE)
    {
        static const auto start = std::chrono::steady_clock::now();
        static int logged = 0;
        const double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (t >= traceAt && logged < 3000)
        {
            ++logged;
            const auto &c = batch.state.context;
            const GSVertex &a0 = batch.vertices[0], &a1 = batch.vertices[1];
            std::fprintf(stderr,
                         "[gs:spr] z0=%.0f z1=%.0f zpsm=0x%x fbp=%u fbw=%u fpsm=0x%x fbmsk=0x%x zbuf=0x%llx tme=%u fst=%u abe=%u tbp=%u tbw=%u psm=0x%x tw=%u th=%u tfx=%u tcc=%u tex1=0x%llx clamp=0x%llx alpha=0x%llx test=0x%llx sc=(%u,%u,%u,%u) ofs=(%u,%u) xy=(%.1f,%.1f)-(%.1f,%.1f) uv=(%u,%u)-(%u,%u) st=(%.4f,%.4f)-(%.4f,%.4f) rgba=(%u,%u,%u,%u)\n",
                         a0.z, a1.z, c.zbuf.psm, c.frame.fbp, c.frame.fbw, c.frame.psm, c.frame.fbmsk, (unsigned long long)c.zbuf.zbp | ((unsigned long long)c.zbuf.zmask << 32),
                         (unsigned)batch.state.prim.tme, (unsigned)batch.state.prim.fst, (unsigned)batch.state.prim.abe,
                         c.tex0.tbp0, (unsigned)c.tex0.tbw, (unsigned)c.tex0.psm, (unsigned)c.tex0.tw, (unsigned)c.tex0.th, (unsigned)c.tex0.tfx, (unsigned)c.tex0.tcc,
                         (unsigned long long)c.tex1, (unsigned long long)c.clamp, (unsigned long long)c.alpha, (unsigned long long)c.test,
                         c.scissor.x0, c.scissor.y0, c.scissor.x1, c.scissor.y1, c.xyoffset.ofx >> 4, c.xyoffset.ofy >> 4,
                         a0.x, a0.y, a1.x, a1.y, a0.u >> 4, a0.v >> 4, a1.u >> 4, a1.v >> 4, a0.s / a0.q, a0.t / a0.q, a1.s / a1.q, a1.t / a1.q,
                         a0.r, a0.g, a0.b, a0.a);
        }
    }

    static const bool immediate = std::getenv("PS2X_GS_IMMEDIATE") != nullptr;
    UpdateTextureReadMode(batch);
    bool gpuFeedback = false;
#ifdef PS2X_HAVE_METAL
    if (!immediate && !m_textureDirectRead && gpuUploads() && batch.state.prim.tme && gpuCanDraw(batch))
    {
        // Feedback draw on the GPU: it samples a copy of the texture memory
        // taken right before it, like the CPU path below.
        flushPendingUnlocked("GpuFeedback");
        const auto &tex = batch.state.context.tex0;
        bufferRange(tex.tbp0 * 256u, std::max<uint32_t>(batch.state.textureWidth, tex.tbw * 64u),
                    batch.state.textureHeight, tex.psm, m_gpuFeedbackRange.first, m_gpuFeedbackRange.second);
        gpuFeedback = true;
    }
#endif
    if (!gpuFeedback && (immediate || !m_textureDirectRead))
    {
        // Feedback draws read what earlier draws wrote: keep them serial.
        flushPendingUnlocked("Feedback");
        const auto t0 = std::chrono::steady_clock::now();
        if (!immediate && batch.state.prim.tme && m_vram)
        {
            // Sample a copy of the texture's VRAM taken before the draw: rows
            // no longer depend on each other, so the draw is split into bands.
            const auto &tex = batch.state.context.tex0;
            uint64_t begin = 0, end = 0;
            bufferRange(tex.tbp0 * 256u, std::max<uint32_t>(batch.state.textureWidth, tex.tbw * 64u),
                        batch.state.textureHeight, tex.psm, begin, end);
            if (m_feedbackVram.size() != m_vramSize)
                m_feedbackVram.assign(m_vramSize, 0u);
            const uint64_t length = std::min<uint64_t>(end - begin, m_vramSize);
            const uint64_t first = begin % m_vramSize;
            const uint64_t head = std::min<uint64_t>(length, m_vramSize - first);
            std::memcpy(m_feedbackVram.data() + first, m_vram + first, head);
            if (head < length)
                std::memcpy(m_feedbackVram.data(), m_vram, length - head); // wraps around VRAM
            const int kBandRows = bandRows();
            const int bands = static_cast<int>(batch.state.context.scissor.y1) / kBandRows + 1;
            GSParallelFor::instance().run(bands, [&](int band)
                                          {
                                              t_bandY0 = band * kBandRows;
                                              t_bandY1 = t_bandY0 + kBandRows - 1;
                                              t_textureVram = m_feedbackVram.data();
                                              DrawPrimitive(batch);
                                              t_textureVram = nullptr;
                                              t_bandY0 = INT_MIN;
                                              t_bandY1 = INT_MAX;
                                          });
        }
        else
        {
            DrawPrimitive(batch);
        }
        markVramWritten(m_lastFrameRange.first, m_lastFrameRange.second);
        markVramWritten(m_lastZRange.first, m_lastZRange.second);
        if (g_gsProfile.on)
        {
            g_gsProfile.serial += std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
            ++g_gsProfile.serialDraws;
            g_gsProfile.report();
        }
        return;
    }
    // Bands run in parallel, so a draw must not sample memory that an earlier
    // queued draw is still writing (render-to-texture passes like the bloom
    // downsample): finish the queued draws first.
    // Likewise a draw must not overwrite memory a queued draw still samples.
    auto overlapsAny = [](const std::pair<uint64_t, uint64_t> &range, const std::vector<std::pair<uint64_t, uint64_t>> &list)
    {
        if (range.second <= range.first)
            return false;
        for (const auto &other : list)
            if (range.first < other.second && other.first < range.second)
                return true;
        return false;
    };
    if (overlapsAny(m_lastTexRange, m_pendingWrites) ||
        overlapsAny(m_lastFrameRange, m_pendingReads) ||
        overlapsAny(m_lastZRange, m_pendingReads))
    {
        if (g_gsProfile.on && !m_pending.empty())
            ++g_gsProfile.hazardFlushes;
        flushPendingUnlocked("Hazard");
    }
    auto addRange = [](std::vector<std::pair<uint64_t, uint64_t>> &list, const std::pair<uint64_t, uint64_t> &range)
    {
        if (range.second <= range.first)
            return;
        for (auto &existing : list)
        {
            if (range.first <= existing.second && existing.first <= range.second)
            {
                existing.first = std::min(existing.first, range.first);
                existing.second = std::max(existing.second, range.second);
                return;
            }
        }
        list.push_back(range);
    };
    addRange(m_pendingWrites, m_lastFrameRange);
    addRange(m_pendingWrites, m_lastZRange);
    addRange(m_pendingReads, m_lastTexRange);
    if (m_clutChanged || m_clutVersions.empty())
    {
        m_clutVersions.push_back(m_clut);
        m_clutVersionHash.push_back(m_clutHash);
        m_clutChanged = false;
    }
    m_pendingClut.push_back(static_cast<uint32_t>(m_clutVersions.size() - 1u));
    m_pending.push_back(batch);
    if (gpuFeedback)
    {
        flushPendingUnlocked("GpuFeedback");
        m_gpuFeedbackRange = {0u, 0u};
    }
    else if (m_pending.size() >= 4096u)
        flushPendingUnlocked("Full");
}

void GSCpuBackend::flushPendingUnlocked(const char *reason)
{
#ifdef PS2X_HAVE_METAL
    if (m_gpu)
    {
        // GPU work only has to be committed to keep draw order (Metal orders
        // passes that touch the same memory); the CPU waits for it only when
        // it is about to read or write GS memory itself.
        const bool orderingOnly = std::strcmp(reason, "Hazard") == 0 || std::strcmp(reason, "Full") == 0 ||
                                  std::strcmp(reason, "Flush") == 0 || std::strcmp(reason, "GpuUpload") == 0 ||
                                  std::strcmp(reason, "GpuFeedback") == 0;
        if (!m_pending.empty())
        {
            if (g_gsProfile.on)
                ++g_gsProfile.reasons[reason];
            if (orderingOnly)
            {
                // The CPU must not touch this memory until the GPU is done.
                for (const auto *list : {&m_pendingWrites, &m_pendingReads})
                    for (const auto &range : *list)
                        addInflightRange(range);
            }
            rasterizePendingGpu();
            finishPendingUnlocked();
        }
        if (!orderingOnly)
        {
            const auto w0 = std::chrono::steady_clock::now();
            gpuWaitIdle();
            if (g_gsProfile.on)
            {
                const double waited = std::chrono::duration<double>(std::chrono::steady_clock::now() - w0).count();
                g_gsProfile.serial += waited; // reported as "serial" in GPU mode: time the CPU waits for the GPU
                ++g_gsProfile.reasons[std::string("wait:") + reason];
                g_gsProfile.report();
            }
        }
        return;
    }
#endif
    if (m_pending.empty())
        return;
    if (g_gsProfile.on)
        ++g_gsProfile.reasons[reason];
    rasterizePendingCpu();
    finishPendingUnlocked();
}

void GSCpuBackend::rasterizePendingCpu()
{
    const int kBandRows = bandRows();
    // Bin every primitive into the bands its (conservative) Y range touches,
    // so each band only walks the primitives that can draw into it.
    int maxY = 0;
    for (const auto &batch : m_pending)
        maxY = std::max<int>(maxY, batch.state.context.scissor.y1);
    const int bands = maxY / kBandRows + 1;
    if (m_bandBins.size() < static_cast<size_t>(bands))
        m_bandBins.resize(bands);
    for (int b = 0; b < bands; ++b)
        m_bandBins[b].clear();
    for (uint32_t index = 0; index < m_pending.size(); ++index)
    {
        const GSPrimitiveBatch &batch = m_pending[index];
        const auto &ctx = batch.state.context;
        int y0 = static_cast<int>(ctx.scissor.y0);
        int y1 = static_cast<int>(ctx.scissor.y1);
        if (batch.vertexCount > 0)
        {
            const float ofy = static_cast<float>(ctx.xyoffset.ofy >> 4);
            float lo = batch.vertices[0].y, hi = batch.vertices[0].y;
            for (uint32_t v = 1; v < batch.vertexCount; ++v)
            {
                lo = std::min(lo, batch.vertices[v].y);
                hi = std::max(hi, batch.vertices[v].y);
            }
            // Generous margins: rasterizers round, and lines/points have width.
            y0 = std::max(y0, static_cast<int>(std::floor(lo - ofy)) - 2);
            y1 = std::min(y1, static_cast<int>(std::ceil(hi - ofy)) + 2);
        }
        static const bool noBinning = std::getenv("PS2X_GS_NOBIN") != nullptr;
        if (noBinning)
        {
            y0 = 0;
            y1 = maxY;
        }
        if (y1 < y0 || y1 < 0)
            continue;
        const int b0 = std::max(0, y0 / kBandRows);
        const int b1 = std::min(bands - 1, y1 / kBandRows);
        for (int b = b0; b <= b1; ++b)
            m_bandBins[b].push_back(index);
    }
    m_textureDirectRead = true;
    // Entries are never evicted while queued draws hold pointers to them.
    if (m_textureCacheBytes > (256u << 20))
    {
        ++textureCacheClears;
        m_textureCache.clear();
        m_textureCacheBytes = 0;
    }
    m_pendingTexture.assign(m_pending.size(), nullptr);
    if (kTextureCacheEnabled)
    {
        for (uint32_t index = 0; index < m_pending.size(); ++index)
        {
            const GSPrimitiveBatch &batch = m_pending[index];
            if (batch.state.prim.tme && batch.vertexCount != 0u)
                m_pendingTexture[index] = decodedTexture(batch.state, m_clutVersions[m_pendingClut[index]].data(),
                                                         m_clutVersionHash[m_pendingClut[index]]);
        }
    }
    const auto t0 = std::chrono::steady_clock::now();
    if (g_gsProfile.on)
    {
        ++g_gsProfile.flushes;
        g_gsProfile.bandedDraws += m_pending.size();
    }
    GSParallelFor::instance().run(bands, [this, kBandRows](int band)
                                  {
                                      t_bandY0 = band * kBandRows;
                                      t_bandY1 = t_bandY0 + kBandRows - 1;
                                      for (const uint32_t index : m_bandBins[band])
                                      {
                                          t_clut = m_clutVersions[m_pendingClut[index]].data();
                                          t_texture = m_pendingTexture[index];
                                          DrawPrimitive(m_pending[index]);
                                      }
                                      t_clut = nullptr;
                                      t_texture = nullptr;
                                      t_bandY0 = INT_MIN;
                                      t_bandY1 = INT_MAX;
                                  });
    if (g_gsProfile.on)
    {
        g_gsProfile.banded += std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        g_gsProfile.report();
    }
}

void GSCpuBackend::finishPendingUnlocked()
{
    for (const auto &range : m_pendingWrites)
        markVramWritten(range.first, range.second);
    m_pending.clear();
    m_pendingTexture.clear();
    m_pendingClut.clear();
    m_clutVersions.clear();
    m_clutVersionHash.clear();
    m_clutChanged = true;
    m_pendingWrites.clear();
    m_pendingReads.clear();
}

#ifdef PS2X_HAVE_METAL
namespace
{
    bool isTargetPsm(uint32_t psm) { return psm == GS_PSM_CT32 || psm == GS_PSM_CT24 || psm == GS_PSM_CT16 || psm == GS_PSM_CT16S; }
    bool isDepthPsm(uint32_t psm) { return psm == GS_PSM_Z32 || psm == GS_PSM_Z24 || psm == GS_PSM_Z16 || psm == GS_PSM_Z16S; }
    bool isTexturePsm(uint32_t psm)
    {
        switch (psm)
        {
        case GS_PSM_CT32: case GS_PSM_CT24: case GS_PSM_CT16: case GS_PSM_CT16S:
        case GS_PSM_T8: case GS_PSM_T4: case GS_PSM_T8H: case GS_PSM_T4HL: case GS_PSM_T4HH:
        case GS_PSM_Z32: case GS_PSM_Z24: case GS_PSM_Z16: case GS_PSM_Z16S:
            return true;
        default:
            return false;
        }
    }

    // Formats and primitive types the GPU path reproduces exactly.
    bool gpuCanDraw(const GSPrimitiveBatch &batch)
    {
        const GSDrawState &state = batch.state;
        const auto &ctx = state.context;
        const auto type = state.prim.type;
        const bool triangle = type == GS_PRIM_TRIANGLE || type == GS_PRIM_TRISTRIP || type == GS_PRIM_TRIFAN;
        if (!(triangle && batch.vertexCount >= 3) && !(type == GS_PRIM_SPRITE && batch.vertexCount >= 2))
            return false;
        if (!isTargetPsm(ctx.frame.psm))
            return false;
        const uint32_t ztst = static_cast<uint32_t>((ctx.test >> 17) & 3u);
        if ((ztst >= 2u || !ctx.zbuf.zmask) && !isDepthPsm(ctx.zbuf.psm))
            return false;
        if (state.prim.tme && !isTexturePsm(ctx.tex0.psm))
            return false;
        return true;
    }

    void splitDepth(double z, float &hi, float &lo)
    {
        const DepthPair pair = depthSplit(z);
        hi = pair.hi;
        lo = pair.lo;
    }

    uint32_t packRgba(const GSVertex &v)
    {
        return static_cast<uint32_t>(v.r) | (static_cast<uint32_t>(v.g) << 8) | (static_cast<uint32_t>(v.b) << 16) | (static_cast<uint32_t>(v.a) << 24);
    }

    // Same setup as DrawTriangle (without band clipping).
    bool setupGpuTriangle(const GSPrimitiveBatch &batch, GSGpuPrim &p)
    {
        const GSDrawState &state = batch.state;
        const auto &ctx = state.context;
        const int ofx = ctx.xyoffset.ofx >> 4;
        const int ofy = ctx.xyoffset.ofy >> 4;
        float fx[3], fy[3];
        for (int i = 0; i < 3; ++i)
        {
            fx[i] = batch.vertices[i].x - static_cast<float>(ofx);
            fy[i] = batch.vertices[i].y - static_cast<float>(ofy);
        }
        int minX = static_cast<int>(std::floor(std::min({fx[0], fx[1], fx[2]})));
        int maxX = static_cast<int>(std::ceil(std::max({fx[0], fx[1], fx[2]})));
        int minY = static_cast<int>(std::floor(std::min({fy[0], fy[1], fy[2]})));
        int maxY = static_cast<int>(std::ceil(std::max({fy[0], fy[1], fy[2]})));
        minX = clampInt(minX, ctx.scissor.x0, ctx.scissor.x1);
        maxX = clampInt(maxX, ctx.scissor.x0, ctx.scissor.x1);
        minY = clampInt(minY, ctx.scissor.y0, ctx.scissor.y1);
        maxY = clampInt(maxY, ctx.scissor.y0, ctx.scissor.y1);
        if (minY > maxY)
            return false;
        const float denom = (fy[1] - fy[2]) * (fx[0] - fx[2]) + (fx[2] - fx[1]) * (fy[0] - fy[2]);
        if (std::fabs(denom) < 0.001f)
            return false;
        p.kind = 0;
        p.x0 = minX;
        p.x1 = maxX;
        p.y0 = minY;
        p.y1 = maxY;
        p.winding = (denom < 0.0f) ? -1.0f : 1.0f;
        p.invAbsDenom = 1.0f / std::fabs(denom);
        for (int i = 0; i < 3; ++i)
        {
            const GSVertex &v = batch.vertices[i];
            p.fx[i] = fx[i];
            p.fy[i] = fy[i];
            splitDepth(v.z, p.zhi[i], p.zlo[i]);
            p.rgba[i] = packRgba(v);
            p.s[i] = v.s;
            p.t[i] = v.t;
            p.q[i] = v.q;
            p.u[i] = static_cast<float>(v.u);
            p.v[i] = static_cast<float>(v.v);
            p.fog[i] = static_cast<float>(v.fog);
        }
        return true;
    }

    // Same setup as DrawSprite (without band clipping).
    bool setupGpuSprite(const GSPrimitiveBatch &batch, GSGpuPrim &p)
    {
        const GSDrawState &state = batch.state;
        const GSVertex &v0 = batch.vertices[0];
        const GSVertex &v1 = batch.vertices[1];
        const auto &ctx = state.context;
        const float ofx = static_cast<float>(ctx.xyoffset.ofx) / 16.0f;
        const float ofy = static_cast<float>(ctx.xyoffset.ofy) / 16.0f;
        const float sx0 = v0.x - ofx, sy0 = v0.y - ofy, sx1 = v1.x - ofx, sy1 = v1.y - ofy;
        const int unclippedX0 = static_cast<int>(std::ceil(std::min(sx0, sx1)));
        const int unclippedY0 = static_cast<int>(std::ceil(std::min(sy0, sy1)));
        const int unclippedX1 = static_cast<int>(std::ceil(std::max(sx0, sx1))) - 1;
        const int unclippedY1 = static_cast<int>(std::ceil(std::max(sy0, sy1))) - 1;
        if (unclippedX1 < unclippedX0 || unclippedY1 < unclippedY0)
            return false;
        if (unclippedX1 < ctx.scissor.x0 || unclippedX0 > ctx.scissor.x1 ||
            unclippedY1 < ctx.scissor.y0 || unclippedY0 > ctx.scissor.y1)
            return false;
        p.kind = 1;
        p.x0 = clampInt(unclippedX0, ctx.scissor.x0, ctx.scissor.x1);
        p.y0 = clampInt(unclippedY0, ctx.scissor.y0, ctx.scissor.y1);
        p.x1 = clampInt(unclippedX1, ctx.scissor.x0, ctx.scissor.x1);
        p.y1 = clampInt(unclippedY1, ctx.scissor.y0, ctx.scissor.y1);
        p.fx[0] = sx0;
        p.fy[0] = sy0;
        p.fx[1] = sx1;
        p.fy[1] = sy1;
        p.winding = (sx1 != sx0) ? 1.0f / (sx1 - sx0) : 0.0f;
        p.invAbsDenom = (sy1 != sy0) ? 1.0f / (sy1 - sy0) : 0.0f;
        const uint32_t z1 = static_cast<uint32_t>(v1.z);
        std::memcpy(&p.zhi[0], &z1, sizeof(z1));
        p.rgba[0] = packRgba(v1);
        p.fog[0] = static_cast<float>(v1.fog);
        if (state.prim.tme)
        {
            if (state.prim.fst)
            {
                // UV are 12.4 fixed point: keep the fraction (SotC's bloom
                // passes put sprites at texel centres with .5 UVs).
                p.su0 = static_cast<float>(v0.u) / 16.0f;
                p.sv0 = static_cast<float>(v0.v) / 16.0f;
                p.su1 = static_cast<float>(v1.u) / 16.0f;
                p.sv1 = static_cast<float>(v1.v) / 16.0f;
            }
            else
            {
                const float q0 = fabsQ(v0.q);
                const float q1 = fabsQ(v1.q);
                p.su0 = (v0.s / q0) * static_cast<float>(state.textureWidth);
                p.sv0 = (v0.t / q0) * static_cast<float>(state.textureHeight);
                p.su1 = (v1.s / q1) * static_cast<float>(state.textureWidth);
                p.sv1 = (v1.t / q1) * static_cast<float>(state.textureHeight);
            }
        }
        return true;
    }

    GSGpuState makeGpuState(const GSDrawState &state, uint32_t clutIndex)
    {
        const auto &ctx = state.context;
        GSGpuState st{};
        st.fbp = framePageBaseToBlock(ctx.frame.fbp);
        st.fbw = std::max<uint32_t>(ctx.frame.fbw, 1u);
        st.fpsm = ctx.frame.psm;
        st.fbmsk = ctx.frame.fbmsk;
        st.zbp = framePageBaseToBlock(ctx.zbuf.zbp);
        st.zpsm = ctx.zbuf.psm;
        st.zmask = ctx.zbuf.zmask ? 1u : 0u;
        st.test = static_cast<uint32_t>(ctx.test);
        st.asel = ctx.alpha & 3u;
        st.bsel = (ctx.alpha >> 2) & 3u;
        st.csel = (ctx.alpha >> 4) & 3u;
        st.dsel = (ctx.alpha >> 6) & 3u;
        st.fix = static_cast<uint32_t>((ctx.alpha >> 32) & 0xFFu);
        st.abe = state.prim.abe ? 1u : 0u;
        st.pabe = state.pabe ? 1u : 0u;
        st.fba = ((ctx.fba & 1u) != 0u && ctx.frame.psm != GS_PSM_CT24) ? 1u : 0u;
        // Bit 1: COLCLAMP off, the blend's result wraps modulo 256 (SotC's
        // shadow volumes count +1 / 255 per face that way).
        if ((state.colclamp & 1u) == 0u && state.prim.abe)
            st.fba |= 2u;
        st.fge = state.prim.fge ? 1u : 0u;
        st.fogR = state.fogR;
        st.fogG = state.fogG;
        st.fogB = state.fogB;
        st.iip = state.prim.iip ? 1u : 0u;
        st.tme = state.prim.tme ? 1u : 0u;
        st.fst = state.prim.fst ? 1u : 0u;
        st.linear = state.linearFilter ? 1u : 0u;
        st.tbp0 = ctx.tex0.tbp0;
        st.tbw = ctx.tex0.tbw;
        st.tpsm = ctx.tex0.psm;
        st.texW = state.textureWidth;
        st.texH = state.textureHeight;
        st.tcc = ctx.tex0.tcc;
        st.tfx = ctx.tex0.tfx;
        st.cpsm = ctx.tex0.cpsm;
        st.csm = ctx.tex0.csm;
        st.csa = ctx.tex0.csa;
        st.wrapU = static_cast<uint32_t>(ctx.clamp & 0x3u);
        st.wrapV = static_cast<uint32_t>((ctx.clamp >> 2) & 0x3u);
        st.minU = static_cast<uint32_t>((ctx.clamp >> 4) & 0x3FFu);
        st.maxU = static_cast<uint32_t>((ctx.clamp >> 14) & 0x3FFu);
        st.minV = static_cast<uint32_t>((ctx.clamp >> 24) & 0x3FFu);
        st.maxV = static_cast<uint32_t>((ctx.clamp >> 34) & 0x3FFu);
        st.ta0 = state.texa.ta0;
        st.aem = state.texa.aem ? 1u : 0u;
        st.ta1 = state.texa.ta1;
        st.clut = clutIndex;
        return st;
    }
}

void GSCpuBackend::rasterizePendingGpu()
{
    static const bool verify = std::getenv("PS2X_GS_GPU_VERIFY") != nullptr;
    std::vector<uint8_t> before, gpuResult;
    if (verify)
        before.assign(m_vram, m_vram + m_vramSize);

    std::vector<uint16_t> cluts;
    cluts.reserve(m_clutVersions.size() * 512u);
    for (const auto &version : m_clutVersions)
        cluts.insert(cluts.end(), version.begin(), version.end());

    std::vector<GSGpuPrim> prims;
    std::vector<uint64_t> keys;
    std::vector<GSGpuState> states;
    prims.reserve(m_pending.size());
    keys.reserve(m_pending.size());
    auto submit = [&]
    {
        if (!prims.empty())
            m_gpu->draw(prims.data(), keys.data(), prims.size(), states.data(), states.size(), cluts.data(), m_clutVersions.size(),
                        m_gpuFeedbackRange.first, m_gpuFeedbackRange.second);
        prims.clear();
        keys.clear();
        states.clear();
    };
    m_textureDirectRead = true;
    for (uint32_t index = 0; index < m_pending.size(); ++index)
    {
        const GSPrimitiveBatch &batch = m_pending[index];
        if (!gpuCanDraw(batch))
        {
            // Lines, points and unusual formats: CPU, in order.
            submit();
            gpuWaitIdle();
            t_clut = m_clutVersions[m_pendingClut[index]].data();
            DrawPrimitive(batch);
            t_clut = nullptr;
            continue;
        }
        GSGpuPrim prim;
        const bool sprite = batch.state.prim.type == GS_PRIM_SPRITE;
        if (!(sprite ? setupGpuSprite(batch, prim) : setupGpuTriangle(batch, prim)))
            continue;
        const GSGpuState state = makeGpuState(batch.state, m_pendingClut[index]);
        if (states.empty() || std::memcmp(&states.back(), &state, sizeof(state)) != 0)
            states.push_back(state);
        prim.state = static_cast<uint32_t>(states.size() - 1u);
        prims.push_back(prim);
        keys.push_back(GSMetalRaster::pipelineKey(prim.kind, state));
    }
    submit();

    if (verify)
    {
        gpuWaitIdle();
        // PS2X_GS_GPU_VERIFY=1: redo the batch on the CPU from the same memory
        // and report the bytes that differ. The CPU result is kept.
        gpuResult.assign(m_vram, m_vram + m_vramSize);
        std::memcpy(m_vram, before.data(), m_vramSize);
        rasterizePendingCpu();
        static uint64_t batches = 0, mismatchedBatches = 0, mismatchedBytes = 0;
        ++batches;
        size_t diff = 0, first = SIZE_MAX;
        for (size_t i = 0; i < m_vramSize; ++i)
        {
            if (gpuResult[i] != m_vram[i])
            {
                if (first == SIZE_MAX)
                    first = i;
                ++diff;
            }
        }
        if (diff)
        {
            ++mismatchedBatches;
            mismatchedBytes += diff;
            if (mismatchedBatches <= 20)
                std::fprintf(stderr, "[gs:gpu-verify] batch %llu (%zu draws): %zu bytes differ, first at 0x%zx (cpu %02x gpu %02x)\n",
                             (unsigned long long)batches, m_pending.size(), diff, first, m_vram[first], gpuResult[first]);
            if (mismatchedBatches <= 3)
            {
                const size_t at = first & ~size_t(15);
                for (const auto &[name, data] : {std::pair<const char *, const uint8_t *>{"before", before.data()},
                                                 {"cpu   ", m_vram}, {"gpu   ", gpuResult.data()}})
                {
                    std::fprintf(stderr, "[gs:gpu-verify]   %s %06zx:", name, at);
                    for (size_t i = 0; i < 16; ++i)
                        std::fprintf(stderr, " %02x", data[at + i]);
                    std::fprintf(stderr, "\n");
                }
                for (const GSPrimitiveBatch &batch : m_pending)
                {
                    const auto &c = batch.state.context;
                    std::fprintf(stderr, "[gs:gpu-verify]   type=%u gpu=%d fbp=%u fbw=%u fpsm=0x%x zbp=%u zpsm=0x%x zmask=%u test=0x%llx tme=%u fst=%u tbp=%u tbw=%u tpsm=0x%x tfx=%u tcc=%u lin=%d abe=%u alpha=0x%llx fge=%u iip=%u fbmsk=0x%x xy0=(%.1f,%.1f) xy1=(%.1f,%.1f) uv0=(%u,%u) stq0=(%g,%g,%g) rgba1=(%u,%u,%u,%u)\n",
                                 (unsigned)batch.state.prim.type, (int)gpuCanDraw(batch), c.frame.fbp, c.frame.fbw, c.frame.psm, c.zbuf.zbp, c.zbuf.psm, (unsigned)c.zbuf.zmask,
                                 (unsigned long long)c.test, (unsigned)batch.state.prim.tme, (unsigned)batch.state.prim.fst, c.tex0.tbp0, (unsigned)c.tex0.tbw, (unsigned)c.tex0.psm,
                                 (unsigned)c.tex0.tfx, (unsigned)c.tex0.tcc, (int)batch.state.linearFilter, (unsigned)batch.state.prim.abe, (unsigned long long)c.alpha,
                                 (unsigned)batch.state.prim.fge, (unsigned)batch.state.prim.iip, c.frame.fbmsk,
                                 batch.vertices[0].x - (c.xyoffset.ofx >> 4), batch.vertices[0].y - (c.xyoffset.ofy >> 4),
                                 batch.vertices[1].x - (c.xyoffset.ofx >> 4), batch.vertices[1].y - (c.xyoffset.ofy >> 4),
                                 batch.vertices[0].u, batch.vertices[0].v, batch.vertices[0].s, batch.vertices[0].t, batch.vertices[0].q,
                                 batch.vertices[1].r, batch.vertices[1].g, batch.vertices[1].b, batch.vertices[1].a);
                    std::fprintf(stderr, "[gs:gpu-verify]     z=(%.3f, %.3f, %.3f) xy2=(%.4f,%.4f)\n", batch.vertices[0].z, batch.vertices[1].z, batch.vertices[2].z,
                                 batch.vertices[2].x - (c.xyoffset.ofx >> 4), batch.vertices[2].y - (c.xyoffset.ofy >> 4));
                }
            }
        }
        if ((batches % 2000u) == 0u)
            std::fprintf(stderr, "[gs:gpu-verify] %llu batches, %llu differ (%llu bytes)\n",
                         (unsigned long long)batches, (unsigned long long)mismatchedBatches, (unsigned long long)mismatchedBytes);
    }
}
#endif

size_t GSCpuBackend::TextureKeyHash::operator()(const TextureKey &key) const noexcept
{
    uint64_t hash = key.clutHash ^ 0x9E3779B97F4A7C15ull;
    for (const uint32_t value : {key.tbp0, key.tbw, key.psm, key.tw, key.th, key.cpsm, key.csm, key.csa, key.texa})
        hash = (hash ^ value) * 1099511628211ull;
    return static_cast<size_t>(hash);
}

void GSCpuBackend::markVramWritten(uint64_t begin, uint64_t end)
{
    if (end <= begin)
        return;
    const uint64_t epoch = ++m_epochCounter;
    const uint64_t firstPage = begin / 8192u;
    const uint64_t lastPage = std::min<uint64_t>((end - 1u) / 8192u, firstPage + m_pageEpoch.size() - 1u);
    for (uint64_t page = firstPage; page <= lastPage; ++page)
        m_pageEpoch[page % m_pageEpoch.size()] = epoch;
}

namespace
{
    thread_local bool t_decodeAnySize = false;
}

const std::vector<uint32_t> *GSCpuBackend::textureForHw(const GSDrawState &state, uint32_t &width, uint32_t &height, uint64_t &epoch)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    t_decodeAnySize = true;
    const DecodedTexture *texture = decodedTexture(state, m_clut.data(), m_clutHash);
    t_decodeAnySize = false;
    if (!texture)
        return nullptr;
    width = texture->width;
    height = texture->height;
    epoch = texture->version; // changes only when the texels do
    return &texture->texels;
}

uint64_t GSCpuBackend::newestWrite(uint64_t begin, uint64_t end) const
{
    if (end <= begin)
        return 0;
    uint64_t newest = 0;
    const uint64_t firstPage = begin / 8192u;
    const uint64_t lastPage = std::min<uint64_t>((end - 1u) / 8192u, firstPage + m_pageEpoch.size() - 1u);
    for (uint64_t page = firstPage; page <= lastPage; ++page)
        newest = std::max(newest, m_pageEpoch[page % m_pageEpoch.size()]);
    return newest;
}

void GSCpuBackend::fillPage(uint32_t page, uint32_t psm, uint32_t color)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_vram)
        return;
    flushPendingUnlocked("FillPage");
    const uint64_t base = (page % 512u) * 8192ull;
    if (base + 8192u > m_vramSize)
        return;
    // Swizzling only moves pixels within a page: a solid page is the same
    // value in every slot.
    if (psm == GS_PSM_CT16 || psm == GS_PSM_CT16S)
    {
        auto *p = reinterpret_cast<uint16_t *>(m_vram + base);
        std::fill(p, p + 4096, static_cast<uint16_t>(color));
    }
    else
    {
        auto *p = reinterpret_cast<uint32_t *>(m_vram + base);
        if (psm == GS_PSM_CT24)
            for (int i = 0; i < 2048; ++i)
                p[i] = (p[i] & 0xFF000000u) | (color & 0x00FFFFFFu);
        else
            std::fill(p, p + 2048, color);
    }
    markVramWritten(base, base + 8192u);
}

void GSCpuBackend::noteExternalWrite(uint64_t begin, uint64_t end)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    markVramWritten(begin, end);
}

const GSCpuBackend::DecodedTexture *GSCpuBackend::decodedTexture(const GSDrawState &state, const uint16_t *clut, uint64_t clutHash)
{
    const auto &tex = state.context.tex0;
    const uint32_t width = state.textureWidth;
    const uint32_t height = state.textureHeight;
    // Very large textures sampled by small draws cost more to decode than to sample.
    if (!m_vram || (width * height > 512u * 512u && !t_decodeAnySize))
        return nullptr;
    const bool indexed = isFourBitIndexedPsm(tex.psm) || isEightBitIndexedPsm(tex.psm);
    TextureKey key;
    key.tbp0 = tex.tbp0;
    key.tbw = tex.tbw;
    key.psm = tex.psm;
    key.tw = tex.tw;
    key.th = tex.th;
    key.texa = static_cast<uint32_t>(state.texa.ta0) | (static_cast<uint32_t>(state.texa.ta1) << 8) |
               (state.texa.aem ? 0x10000u : 0u);
    if (indexed)
    {
        key.cpsm = tex.cpsm;
        key.csm = tex.csm;
        key.csa = tex.csa;
        key.clutHash = clutHash;
    }

    uint64_t begin = 0, end = 0;
    bufferRange(tex.tbp0 * 256u, std::max<uint32_t>(width, tex.tbw * 64u), height, tex.psm, begin, end);
    uint64_t newestWrite = 0;
    const uint64_t firstPage = begin / 8192u;
    const uint64_t lastPage = std::min<uint64_t>((end - 1u) / 8192u, firstPage + m_pageEpoch.size() - 1u);
    for (uint64_t page = firstPage; page <= lastPage; ++page)
        newestWrite = std::max(newestWrite, m_pageEpoch[page % m_pageEpoch.size()]);

    DecodedTexture &entry = m_textureCache[key];
    if (!entry.texels.empty() && entry.epoch >= newestWrite)
        return &entry;
    // Its pages were written: the same bytes again (games re-upload textures
    // every frame) keep the decoded texels.
    // Hash the 256-byte blocks the texture's rectangle lies in: games share
    // pages between textures and render targets, so whole pages change more
    // often than the texture does.
    uint64_t sourceHash = 1469598103934665603ull;
    {
        uint32_t bw = 8, bh = 8;  // block size in pixels
        bool nibbles = false;
        uint32_t (*addr)(uint32_t, uint32_t, uint32_t, uint32_t) = GSPSMCT32::addrPSMCT32;
        switch (tex.psm)
        {
        case GS_PSM_T8: bw = 16; bh = 16; addr = GSPSMT8::addrPSMT8; break;
        case GS_PSM_T4: bw = 32; bh = 16; addr = GSPSMT4::addrPSMT4; nibbles = true; break;
        case GS_PSM_CT16: case GS_PSM_CT16S: bw = 16; bh = 8; addr = GSPSMCT16::addrPSMCT16; break;
        default: break; // CT32, CT24 and the T8H/T4HL/T4HH forms use CT32 blocks
        }
        if (tex.psm == GS_PSM_CT16S)
            addr = GSPSMCT16::addrPSMCT16S;
        uint64_t word = 0;
        for (uint32_t by = 0; by < (height + bh - 1u) / bh; ++by)
            for (uint32_t bx = 0; bx < (width + bw - 1u) / bw; ++bx)
            {
                uint64_t at = addr(tex.tbp0, tex.tbw, bx * bw, by * bh);
                if (nibbles)
                    at >>= 1;
                at = (at & ~255ull) % m_vramSize;
                for (uint32_t o = 0; o < 256u; o += 8u)
                {
                    std::memcpy(&word, m_vram + at + o, 8u);
                    sourceHash = (sourceHash ^ word) * 1099511628211ull;
                }
            }
        sourceHash ^= clutHash;
    }
    if (!entry.texels.empty() && entry.sourceHash == sourceHash && entry.width == width && entry.height == height)
    {
        entry.epoch = m_epochCounter;
        ++textureHashHits;
        return &entry;
    }
    ++textureDecodes;
    static const bool logDecodes = std::getenv("PS2X_GS_TEXLOG") != nullptr;
    if (logDecodes)
        std::fprintf(stderr, "[gs:tex] decode tbp=%x tbw=%u psm=%x %ux%u cbp=%x %s\n", tex.tbp0, tex.tbw, tex.psm, width, height, tex.cbp,
                     entry.texels.empty() ? "new" : "changed");
    entry.sourceHash = sourceHash;
    ++entry.version;

    if (entry.texels.size() != static_cast<size_t>(width) * height)
    {
        m_textureCacheBytes += static_cast<size_t>(width) * height * 4u;
        entry.texels.resize(static_cast<size_t>(width) * height);
    }
    entry.width = width;
    entry.height = height;
    entry.epoch = m_epochCounter;
    auto decodeRows = [&](int y0, int y1)
    {
        t_clut = clut;
        for (int y = y0; y < y1; ++y)
        {
            uint32_t *row = entry.texels.data() + static_cast<size_t>(y) * width;
            for (uint32_t x = 0; x < width; ++x)
            {
                const uint32_t raw = ReadVramUnlocked(tex.psm, tex.tbp0, tex.tbw, x, static_cast<uint32_t>(y));
                switch (tex.psm)
                {
                case GS_PSM_CT32:
                case GS_PSM_Z32:
                case GS_PSM_CT24:
                case GS_PSM_Z24:
                    row[x] = applyTexa(state.texa, tex.psm, raw);
                    break;
                case GS_PSM_CT16:
                case GS_PSM_CT16S:
                case GS_PSM_Z16:
                case GS_PSM_Z16S:
                    row[x] = applyTexa(state.texa, tex.psm, Rgba5551ToRgba8888(raw));
                    break;
                case GS_PSM_T8:
                case GS_PSM_T8H:
                case GS_PSM_T4:
                case GS_PSM_T4HL:
                case GS_PSM_T4HH:
                    row[x] = LookupCLUT(state, static_cast<u8>(raw), tex.cpsm, tex.csm, tex.csa, tex.psm);
                    break;
                default:
                    row[x] = 0xFFFF00FFu;
                    break;
                }
            }
        }
        t_clut = nullptr;
    };
    if (width * height >= 64u * 1024u)
    {
        constexpr int kRows = 32;
        const int chunks = static_cast<int>((height + kRows - 1) / kRows);
        GSParallelFor::instance().run(chunks, [&](int chunk)
                                      { decodeRows(chunk * kRows, std::min<int>(static_cast<int>(height), (chunk + 1) * kRows)); });
    }
    else
    {
        decodeRows(0, static_cast<int>(height));
    }
    // PS2X_GS_TEXDUMP=<dir>,<tbp>: the decoded RGBA of that texture (raw, per decode).
    static const std::pair<std::string, long> texDump = []
    {
        const char *e = std::getenv("PS2X_GS_TEXDUMP");
        if (!e)
            return std::pair<std::string, long>{"", -1};
        const char *comma = std::strchr(e, ',');
        return std::pair<std::string, long>{comma ? std::string(e, comma) : std::string(e), comma ? std::atol(comma + 1) : -1};
    }();
    if (texDump.second >= 0 && tex.tbp0 == static_cast<uint32_t>(texDump.second))
    {
        static int n = 0;
        const std::string path = texDump.first + "/gs_" + std::to_string(tex.tbp0) + "_" + std::to_string(width) + "x" + std::to_string(height) + "_" + std::to_string(n++) + ".rgba";
        if (FILE *f = std::fopen(path.c_str(), "wb"))
        {
            std::fwrite(entry.texels.data(), 4, entry.texels.size(), f);
            std::fclose(f);
        }
    }
    return &entry;
}

void GSCpuBackend::addInflightRange(const std::pair<uint64_t, uint64_t> &range)
{
    if (range.second <= range.first)
        return;
    for (auto &existing : m_gpuInflight)
    {
        if (range.first <= existing.second && existing.first <= range.second)
        {
            existing.first = std::min(existing.first, range.first);
            existing.second = std::max(existing.second, range.second);
            return;
        }
    }
    m_gpuInflight.push_back(range);
}

void GSCpuBackend::gpuWaitIdle()
{
#ifdef PS2X_HAVE_METAL
    if (m_gpu)
        m_gpu->waitIdle();
#endif
    m_gpuInflight.clear();
}

bool GSCpuBackend::pendingTouches(uint64_t begin, uint64_t end) const
{
    if (end <= begin)
        return false;
    for (const auto &range : m_gpuInflight)
        if (begin < range.second && range.first < end)
            return true;
    if (m_pending.empty())
        return false;
    for (const auto &range : m_pendingWrites)
        if (begin < range.second && range.first < end)
            return true;
    for (const auto &range : m_pendingReads)
        if (begin < range.second && range.first < end)
            return true;
    return false;
}

void GSCpuBackend::LoadClut(const GSTex0Reg &tex0, const GSTexClutReg &texclut)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_vram || (!isFourBitIndexedPsm(tex0.psm) && !isEightBitIndexedPsm(tex0.psm)))
        return;

    switch (tex0.cld)
    {
    case 0u:
    case 6u:
    case 7u:
        return;
    case 1u:
        break;
    case 2u:
        m_clutCbp[0] = tex0.cbp;
        break;
    case 3u:
        m_clutCbp[1] = tex0.cbp;
        break;
    case 4u:
        if (m_clutCbp[0] == tex0.cbp)
            return;
        m_clutCbp[0] = tex0.cbp;
        break;
    case 5u:
        if (m_clutCbp[1] == tex0.cbp)
            return;
        m_clutCbp[1] = tex0.cbp;
        break;
    default:
        return;
    }

    // The CLUT is read from VRAM now: queued draws must not still be writing it.
    uint64_t cb = 0, ce = 0;
    bufferRange(tex0.cbp * 256u, 64u * std::max<uint32_t>(1u, texclut.cbw), 16u + texclut.cov, tex0.cpsm, cb, ce);
    for (const auto *list : {&m_pendingWrites, &m_gpuInflight})
    {
        const bool touched = std::any_of(list->begin(), list->end(), [&](const auto &range)
                                         { return cb < range.second && range.first < ce; });
        if (touched)
        {
            flushPendingUnlocked("LoadClut");
            break;
        }
    }
    LoadClutUnlocked(tex0, texclut);
    m_clutChanged = true;
}

void GSCpuBackend::LoadClutUnlocked(const GSTex0Reg &tex0, const GSTexClutReg &texclut)
{
    struct HashOnExit
    {
        GSCpuBackend &backend;
        ~HashOnExit()
        {
            uint64_t hash = 1469598103934665603ull;
            for (const uint16_t entry : backend.m_clut)
                hash = (hash ^ entry) * 1099511628211ull;
            backend.m_clutHash = hash;
        }
    } hashOnExit{*this};
    const bool fourBit = isFourBitIndexedPsm(tex0.psm);
    const bool sixteenBit = tex0.cpsm == GS_PSM_CT16 || tex0.cpsm == GS_PSM_CT16S;
    const bool thirtyTwoBit = tex0.cpsm == GS_PSM_CT32 || tex0.cpsm == GS_PSM_CT24;
    if (!sixteenBit && !thirtyTwoBit)
        return;

    const uint32_t entryCount = fourBit ? 16u : 256u;
    const uint32_t csaMask = sixteenBit ? 0x1Fu : 0x0Fu;
    const uint32_t destinationBase = (static_cast<uint32_t>(tex0.csa) & csaMask) << 4u;

    const bool loadCsm1Suffix = tex0.csm == 0u && thirtyTwoBit && !fourBit;
    const uint32_t firstEntry = loadCsm1Suffix ? destinationBase : 0u;

    for (uint32_t entry = firstEntry; entry < entryCount; ++entry)
    {
        uint32_t sourceX = 0u;
        uint32_t sourceY = 0u;
        uint32_t sourceWidth = 1u;

        if (tex0.csm == 0u)
        {
            const uint32_t sourceIndex = swizzleClutIndexCSM1(entry);
            sourceX = sourceIndex & 0x0Fu;
            sourceY = sourceIndex >> 4u;
        }
        else
        {
            sourceWidth = texclut.cbw != 0u ? static_cast<uint32_t>(texclut.cbw) : 1u;
            sourceX = (static_cast<uint32_t>(texclut.cou) << 4u) + entry;
            sourceY = static_cast<uint32_t>(texclut.cov);
        }

        const uint32_t raw = ReadTextureVramUnlocked(tex0.cpsm, tex0.cbp, sourceWidth, sourceX, sourceY); 
        const uint32_t destination = (loadCsm1Suffix ? entry : destinationBase + entry) & (sixteenBit ? 0x1FFu : 0x0FFu);
        if (sixteenBit)
        {
            m_clut[destination] = static_cast<uint16_t>(raw);
        }
        else
        {
            m_clut[destination] = static_cast<uint16_t>(raw & 0xFFFFu);
            m_clut[destination + 256u] = static_cast<uint16_t>(raw >> 16u);
        }
    }
}

void GSCpuBackend::Flush()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    flushPendingUnlocked("Flush");
}

void GSCpuBackend::TextureFlush()
{
    // Queued draws read VRAM directly and uploads flush on overlap, so only
    // the page cache (used by serial feedback draws) needs dropping.
    std::lock_guard<std::mutex> lock(m_mutex);
    m_texturePageCache.Invalidate();
}

void GSCpuBackend::Sync(GSSyncReason)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    flushPendingUnlocked("Sync");
}

uint32_t GSCpuBackend::ReadVram(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const_cast<GSCpuBackend *>(this)->flushPendingUnlocked("ReadVram");
    return ReadVramUnlocked(psm, base, bw, x, y);
}

uint32_t GSCpuBackend::ReadVramUnlocked(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y) const
{
    if (!m_vram)
        return 0u;
    return m_readVramFuncs[psm & 0x3Fu](m_vram, base, bw, x, y);
}

uint32_t GSCpuBackend::ReadTextureVramUnlocked(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y)
{
    if (!m_vram)
        return 0u;

    if (t_textureVram)
        return m_readVramFuncs[psm & 0x3Fu](const_cast<uint8_t *>(t_textureVram), base, bw, x, y);
    if (m_textureDirectRead)
        return ReadVramUnlocked(psm, base, bw, x, y);
    return GSMem::ReadTexture(m_texturePageCache, m_vram, psm, base, bw, x, y);
}

namespace
{
    // Page geometry (in pixels) of each storage format; a GS page is 8 KiB.
    void pageDims(uint32_t psm, uint32_t &w, uint32_t &h)
    {
        switch (psm & 0x3Fu)
        {
        case 0x02: case 0x0A: case 0x32: case 0x3A: w = 64; h = 64; break;   // 16-bit
        case 0x13: w = 128; h = 64; break;                                   // T8
        case 0x14: w = 128; h = 128; break;                                  // T4
        default: w = 64; h = 32; break;                                      // 32/24-bit, T8H, T4HL/HH
        }
    }

    // VRAM byte range [begin, end) touched by rows [0, heightPixels) of a buffer.
    void bufferRange(uint32_t baseBytes, uint32_t bufferWidthPixels, uint32_t heightPixels, uint32_t psm,
                     uint64_t &begin, uint64_t &end)
    {
        uint32_t pw = 0, ph = 0;
        pageDims(psm, pw, ph);
        const uint64_t pagesPerRow = std::max<uint32_t>(1u, (bufferWidthPixels + pw - 1u) / pw);
        uint64_t pageRows = (std::max<uint32_t>(heightPixels, 1u) + ph - 1u) / ph;
        if (baseBytes % 8192u != 0u)
            ++pageRows; // a non page-aligned base can spill into one more page row
        begin = baseBytes;
        end = begin + pagesPerRow * pageRows * 8192u;
    }
}

void GSCpuBackend::UpdateTextureReadMode(const GSPrimitiveBatch &batch)
{
    // The page cache only matters when a draw samples memory it is also writing
    // (feedback effects). Otherwise read VRAM directly and skip the page copies.
    const GSDrawState &state = batch.state;
    const auto &ctx = state.context;
    if (!state.prim.tme || batch.vertexCount == 0u)
    {
        m_textureDirectRead = true;
        m_lastTexRange = {0u, 0u};
        // Untextured draws still write: track the whole frame/Z area.
        const uint32_t frameW = std::max<uint32_t>(1u, ctx.frame.fbw) * 64u;
        const uint32_t rows = static_cast<uint32_t>(ctx.scissor.y1) + 1u;
        uint64_t fb = 0, fe = 0, zb = 0, ze = 0;
        bufferRange(ctx.frame.fbp * 8192u, frameW, rows, ctx.frame.psm, fb, fe);
        bufferRange(ctx.zbuf.zbp * 8192u, frameW, rows, ctx.zbuf.psm | 0x30u, zb, ze);
        const bool zWrites = !ctx.zbuf.zmask && ((ctx.test >> 16) & 1u) != 0u;
        m_lastFrameRange = {fb, fe};
        m_lastZRange = zWrites ? std::make_pair(zb, ze) : std::make_pair<uint64_t, uint64_t>(0u, 0u);
        return;
    }
    const uint32_t texW = 1u << std::min<uint32_t>(ctx.tex0.tw, 10u);
    const uint32_t texH = 1u << std::min<uint32_t>(ctx.tex0.th, 10u);
    float maxV = 0.0f, maxY = 0.0f;
    for (uint32_t i = 0; i < batch.vertexCount && i < batch.vertices.size(); ++i)
    {
        const GSVertex &vtx = batch.vertices[i];
        const float v = state.prim.fst ? static_cast<float>(vtx.v) / 16.0f
                                       : (vtx.q != 0.0f ? vtx.t / vtx.q : vtx.t) * static_cast<float>(texH);
        maxV = std::max(maxV, v);
        maxY = std::max(maxY, vtx.y - static_cast<float>(ctx.xyoffset.ofy >> 4));
    }
    // Rows actually sampled (+2 for bilinear neighbours / rounding), clamped to the texture.
    const uint32_t texRows = std::min<uint32_t>(texH, static_cast<uint32_t>(std::ceil(std::max(0.0f, maxV))));
    const uint32_t drawRows = std::min<uint32_t>(static_cast<uint32_t>(ctx.scissor.y1) + 1u,
                                                 static_cast<uint32_t>(std::max(0.0f, maxY)) + 2u);
    uint64_t tb = 0, te = 0, fb = 0, fe = 0, zb = 0, ze = 0;
    bufferRange(ctx.tex0.tbp0 * 256u, std::max<uint32_t>(texW, ctx.tex0.tbw * 64u), texRows, ctx.tex0.psm, tb, te);
    const uint32_t frameW = std::max<uint32_t>(1u, ctx.frame.fbw) * 64u;
    bufferRange(ctx.frame.fbp * 8192u, frameW, drawRows, ctx.frame.psm, fb, fe);
    bufferRange(ctx.zbuf.zbp * 8192u, frameW, drawRows, ctx.zbuf.psm | 0x30u, zb, ze);
    auto overlaps = [](uint64_t a0, uint64_t a1, uint64_t b0, uint64_t b1)
    { return a0 < b1 && b0 < a1; };
    const bool zWrites = !ctx.zbuf.zmask && ((ctx.test >> 16) & 1u) != 0u; // TEST.ZTE
    m_textureDirectRead = !overlaps(tb, te, fb, fe) && !(zWrites && overlaps(tb, te, zb, ze));
    m_lastTexRange = {tb, te};
    m_lastFrameRange = {fb, fe};
    m_lastZRange = zWrites ? std::make_pair(zb, ze) : std::make_pair<uint64_t, uint64_t>(0u, 0u);
    if (!m_textureDirectRead)
        m_texturePageCache.Invalidate();
    static const bool trace = std::getenv("PS2X_TRACE_TEXREAD") != nullptr;
    if (trace)
    {
        static std::set<std::string> seen;
        std::ostringstream key;
        key << "tbp=" << ctx.tex0.tbp0 << " tbw=" << static_cast<int>(ctx.tex0.tbw) << " psm=0x" << std::hex << static_cast<int>(ctx.tex0.psm) << std::dec
            << " tw=" << static_cast<int>(ctx.tex0.tw) << " th=" << static_cast<int>(ctx.tex0.th)
            << " fbp=" << ctx.frame.fbp << " fbw=" << ctx.frame.fbw << " zbp=" << ctx.zbuf.zbp << " zmask=" << ctx.zbuf.zmask
            << " y1=" << ctx.scissor.y1 << " direct=" << m_textureDirectRead
            << " tex=[" << std::hex << tb << "," << te << ") frame=[" << fb << "," << fe << ") z=[" << zb << "," << ze << ")" << std::dec;
        if (seen.insert(key.str()).second)
            std::cerr << "[gs:texread] " << key.str() << std::endl;
    }
}

void GSCpuBackend::WriteVram(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y, uint32_t value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    flushPendingUnlocked("WriteVram");
    WriteVramUnlocked(psm, base, bw, x, y, value);
    markVramWritten(0u, static_cast<uint64_t>(m_pageEpoch.size()) * 8192u);
}

void GSCpuBackend::WriteVramUnlocked(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y, uint32_t value)
{
    if (!m_vram)
        return;
    m_writeVramFuncs[psm & 0x3Fu](m_vram, base, bw, x, y, value);
}

void GSCpuBackend::SnapshotVram(std::vector<uint8_t> &out) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const_cast<GSCpuBackend *>(this)->flushPendingUnlocked("SnapshotVram");
    if (!m_vram || m_vramSize == 0u)
    {
        out.clear();
        return;
    }
    out.resize(m_vramSize);
    std::memcpy(out.data(), m_vram, m_vramSize);
}

GSTransferSnapshot GSCpuBackend::GetTransferSnapshot() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const_cast<GSCpuBackend *>(this)->flushPendingUnlocked("GetTransferSnapshot");
    GSTransferSnapshot result = m_transferState;
    result.localToHostPendingBytes = m_localToHostReadPos < m_localToHostBuffer.size()
                                         ? m_localToHostBuffer.size() - m_localToHostReadPos
                                         : 0u;
    return result;
}

void GSCpuBackend::DrawPrimitive(const GSPrimitiveBatch &batch)
{
    const GSDrawState &state = batch.state;
    const auto &ctx = state.context;
    if (!inBandMode())
        UpdateTextureReadMode(batch);
    // PS2X_TRACE_TEX: log each distinct textured-draw setup once.
    static const bool traceTex = std::getenv("PS2X_TRACE_TEX") != nullptr;
    // PS2X_TRACE_DRAW_TBP=<tbp0>: dump the vertices of the first draws using that texture.
    static const char *traceDrawTbp = std::getenv("PS2X_TRACE_DRAW_TBP");
    static const char *traceDrawPsm = std::getenv("PS2X_TRACE_DRAW_PSM");
    static const double traceDrawAt = std::getenv("PS2X_TRACE_DRAW_AT") ? std::atof(std::getenv("PS2X_TRACE_DRAW_AT")) : 0.0;
    static const auto traceDrawStart = std::chrono::steady_clock::now();
    if (traceDrawTbp && state.prim.tme && ctx.tex0.tbp0 == static_cast<uint32_t>(std::atoi(traceDrawTbp)) &&
        (!inBandMode() || t_bandY0 == 0) &&
        std::chrono::duration<double>(std::chrono::steady_clock::now() - traceDrawStart).count() >= traceDrawAt &&
        (!traceDrawPsm || ctx.tex0.psm == static_cast<uint32_t>(std::strtoul(traceDrawPsm, nullptr, 0))))
    {
        static std::atomic<int> dumped{0};
        if (dumped.fetch_add(1) < 400)
        {
            std::ostringstream line;
            line << "[gs:draw] type=" << static_cast<uint32_t>(state.prim.type) << " psm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.psm) << std::dec
                 << " tw=" << static_cast<uint32_t>(ctx.tex0.tw) << " test=0x" << std::hex << ctx.test << " alpha=0x" << ctx.alpha << std::dec << " fge=" << static_cast<uint32_t>(state.prim.fge);
            for (uint32_t i = 0; i < batch.vertexCount; ++i)
            {
                const GSVertex &v = batch.vertices[i];
                line << " | xy=(" << v.x << "," << v.y << ") z=" << v.z << " stq=(" << v.s << "," << v.t << "," << v.q << ") st/q=("
                     << (v.q != 0.0f ? v.s / v.q : 0.0f) << "," << (v.q != 0.0f ? v.t / v.q : 0.0f) << ") rgba=("
                     << static_cast<int>(v.r) << "," << static_cast<int>(v.g) << "," << static_cast<int>(v.b) << "," << static_cast<int>(v.a) << ") f=" << static_cast<int>(v.fog);
            }
            std::cerr << line.str() << std::endl;
        }
    }
    if (traceTex)
    {
        static std::set<std::string> seen;
        std::ostringstream key;
        key << "fbp=" << ctx.frame.fbp << " fbw=" << ctx.frame.fbw << " fpsm=0x" << std::hex << static_cast<uint32_t>(ctx.frame.psm) << std::dec
            << " fbmsk=0x" << std::hex << ctx.frame.fbmsk << std::dec << " tme=" << static_cast<uint32_t>(state.prim.tme) << " "
            << "type=" << static_cast<uint32_t>(state.prim.type) << " fst=" << static_cast<uint32_t>(state.prim.fst)
            << " tbp0=" << ctx.tex0.tbp0 << " tbw=" << static_cast<uint32_t>(ctx.tex0.tbw)
            << " psm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.psm) << std::dec
            << " tw=" << static_cast<uint32_t>(ctx.tex0.tw) << " th=" << static_cast<uint32_t>(ctx.tex0.th)
            << " tcc=" << static_cast<uint32_t>(ctx.tex0.tcc) << " tfx=" << static_cast<uint32_t>(ctx.tex0.tfx)
            << " cbp=" << ctx.tex0.cbp << " cpsm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.cpsm) << std::dec
            << " csm=" << static_cast<uint32_t>(ctx.tex0.csm) << " csa=" << static_cast<uint32_t>(ctx.tex0.csa)
            << " clamp=0x" << std::hex << ctx.clamp << std::dec;
        if (seen.insert(key.str()).second)
        {
            std::cerr << "[gs:tex] " << key.str()
                      << " uv0=(" << (batch.vertices[0].u >> 4) << "," << (batch.vertices[0].v >> 4) << ")"
                      << " stq0=(" << batch.vertices[0].s << "," << batch.vertices[0].t << "," << batch.vertices[0].q << ")"
                      << " xy0=(" << batch.vertices[0].x << "," << batch.vertices[0].y << ")" << std::endl;
        }
    }
    PS2_IF_AGRESSIVE_LOGS({
        const uint32_t primitiveIndex = s_debugPrimitiveCount.fetch_add(1u, std::memory_order_relaxed);
        if (primitiveIndex < 64u)
        {
            std::cout << "[gs:prim] idx=" << primitiveIndex
                      << " type=" << static_cast<uint32_t>(state.prim.type)
                      << " tme=" << static_cast<uint32_t>(state.prim.tme)
                      << " abe=" << static_cast<uint32_t>(state.prim.abe)
                      << " fst=" << static_cast<uint32_t>(state.prim.fst)
                      << " ctxt=" << static_cast<uint32_t>(state.prim.ctxt)
                      << " fbp=" << ctx.frame.fbp
                      << " fbw=" << ctx.frame.fbw
                      << " psm=0x" << std::hex << static_cast<uint32_t>(ctx.frame.psm) << std::dec
                      << " tex0=("
                      << "tbp0=" << ctx.tex0.tbp0
                      << " tbw=" << static_cast<uint32_t>(ctx.tex0.tbw)
                      << " psm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.psm) << std::dec
                      << " tw=" << static_cast<uint32_t>(ctx.tex0.tw)
                      << " th=" << static_cast<uint32_t>(ctx.tex0.th)
                      << " tcc=" << static_cast<uint32_t>(ctx.tex0.tcc)
                      << " tfx=" << static_cast<uint32_t>(ctx.tex0.tfx)
                      << " cbp=" << ctx.tex0.cbp
                      << " cpsm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.cpsm) << std::dec
                      << " csm=" << static_cast<uint32_t>(ctx.tex0.csm)
                      << " csa=" << static_cast<uint32_t>(ctx.tex0.csa)
                      << ")"
                      << " texclut=("
                      << "cbw=" << static_cast<uint32_t>(state.texclut.cbw)
                      << " cou=" << static_cast<uint32_t>(state.texclut.cou)
                      << " cov=" << state.texclut.cov
                      << ")"
                      << " ofx=" << (ctx.xyoffset.ofx >> 4)
                      << " ofy=" << (ctx.xyoffset.ofy >> 4)
                      << " scissor=(" << ctx.scissor.x0
                      << "," << ctx.scissor.y0
                      << ")-(" << ctx.scissor.x1
                      << "," << ctx.scissor.y1 << ")"
                      << " test=0x" << std::hex << ctx.test
                      << " alpha=0x" << ctx.alpha
                      << std::dec
                      << " v0=(" << batch.vertices[0].x << "," << batch.vertices[0].y << ")"
                      << " uv0=(" << (batch.vertices[0].u >> 4) << "," << (batch.vertices[0].v >> 4) << ")"
                      << " stq0=(" << batch.vertices[0].s << "," << batch.vertices[0].t << "," << batch.vertices[0].q << ")"
                      << " v1=(" << batch.vertices[1].x << "," << batch.vertices[1].y << ")"
                      << " uv1=(" << (batch.vertices[1].u >> 4) << "," << (batch.vertices[1].v >> 4) << ")"
                      << " stq1=(" << batch.vertices[1].s << "," << batch.vertices[1].t << "," << batch.vertices[1].q << ")"
                      << " v2=(" << batch.vertices[2].x << "," << batch.vertices[2].y << ")"
                      << " uv2=(" << (batch.vertices[2].u >> 4) << "," << (batch.vertices[2].v >> 4) << ")"
                      << " stq2=(" << batch.vertices[2].s << "," << batch.vertices[2].t << "," << batch.vertices[2].q << ")"
                      << " rgba0=(" << static_cast<uint32_t>(batch.vertices[0].r) << ","
                      << static_cast<uint32_t>(batch.vertices[0].g) << ","
                      << static_cast<uint32_t>(batch.vertices[0].b) << ","
                      << static_cast<uint32_t>(batch.vertices[0].a) << ")"
                      << " rgba1=(" << static_cast<uint32_t>(batch.vertices[1].r) << ","
                      << static_cast<uint32_t>(batch.vertices[1].g) << ","
                      << static_cast<uint32_t>(batch.vertices[1].b) << ","
                      << static_cast<uint32_t>(batch.vertices[1].a) << ")"
                      << " rgba2=(" << static_cast<uint32_t>(batch.vertices[2].r) << ","
                      << static_cast<uint32_t>(batch.vertices[2].g) << ","
                      << static_cast<uint32_t>(batch.vertices[2].b) << ","
                      << static_cast<uint32_t>(batch.vertices[2].a) << ")"
                      << std::endl;
        }
    });

    PS2_IF_AGRESSIVE_LOGS({
        if ((state.prim.ctxt != 0u || ctx.frame.fbp == 150u) &&
            s_debugContext1PrimitiveCount.fetch_add(1u, std::memory_order_relaxed) < 32u)
        {
            std::cout << "[gs:copy-prim]"
                      << " type=" << static_cast<uint32_t>(state.prim.type)
                      << " tme=" << static_cast<uint32_t>(state.prim.tme)
                      << " abe=" << static_cast<uint32_t>(state.prim.abe)
                      << " fst=" << static_cast<uint32_t>(state.prim.fst)
                      << " ctxt=" << static_cast<uint32_t>(state.prim.ctxt)
                      << " fbp=" << ctx.frame.fbp
                      << " fbw=" << ctx.frame.fbw
                      << " psm=0x" << std::hex << static_cast<uint32_t>(ctx.frame.psm) << std::dec
                      << " tex0=("
                      << "tbp0=" << ctx.tex0.tbp0
                      << " tbw=" << static_cast<uint32_t>(ctx.tex0.tbw)
                      << " psm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.psm) << std::dec
                      << " tcc=" << static_cast<uint32_t>(ctx.tex0.tcc)
                      << " tfx=" << static_cast<uint32_t>(ctx.tex0.tfx)
                      << " cbp=" << ctx.tex0.cbp
                      << " cpsm=0x" << std::hex << static_cast<uint32_t>(ctx.tex0.cpsm) << std::dec
                      << " csm=" << static_cast<uint32_t>(ctx.tex0.csm)
                      << " csa=" << static_cast<uint32_t>(ctx.tex0.csa)
                      << ")"
                      << " texclut=("
                      << "cbw=" << static_cast<uint32_t>(state.texclut.cbw)
                      << " cou=" << static_cast<uint32_t>(state.texclut.cou)
                      << " cov=" << state.texclut.cov
                      << ")"
                      << " ofx=" << (ctx.xyoffset.ofx >> 4)
                      << " ofy=" << (ctx.xyoffset.ofy >> 4)
                      << " scissor=(" << ctx.scissor.x0
                      << "," << ctx.scissor.y0
                      << ")-(" << ctx.scissor.x1
                      << "," << ctx.scissor.y1 << ")"
                      << " test=0x" << std::hex << ctx.test
                      << " alpha=0x" << ctx.alpha
                      << std::dec << std::endl;
        }
    });

    switch (state.prim.type)
    {
    case GS_PRIM_SPRITE:
        DrawSprite(batch);
        break;
    case GS_PRIM_TRIANGLE:
    case GS_PRIM_TRISTRIP:
    case GS_PRIM_TRIFAN:
        DrawTriangle(batch);
        break;
    case GS_PRIM_LINE:
    case GS_PRIM_LINESTRIP:
        DrawLine(batch);
        break;
    case GS_PRIM_POINT:
    {
        const GSVertex &v = batch.vertices[0];
        const auto &ctx = state.context;
        int px = static_cast<int>(v.x) - (ctx.xyoffset.ofx >> 4);
        int py = static_cast<int>(v.y) - (ctx.xyoffset.ofy >> 4);
        WritePixel(state, px, py, static_cast<u32>(v.z), v.r, v.g, v.b, v.a, v.fog);
        break;
    }
    default:
        break;
    }
}

GSCpuBackend::PixelPipe GSCpuBackend::makePixelPipe(const GSDrawState &state) const
{
    const auto &ctx = state.context;
    PixelPipe pipe;
    pipe.state = &state;
    pipe.scissorX0 = ctx.scissor.x0;
    pipe.scissorX1 = ctx.scissor.x1;
    pipe.rowY0 = std::max<int>(ctx.scissor.y0, t_bandY0);
    pipe.rowY1 = std::min<int>(ctx.scissor.y1, t_bandY1);
    pipe.fge = state.prim.fge;
    pipe.fogR = state.fogR;
    pipe.fogG = state.fogG;
    pipe.fogB = state.fogB;
    pipe.fbp = GSInternal::framePageBaseToBlock(ctx.frame.fbp);
    pipe.fbw = std::max<u32>(ctx.frame.fbw, 1u);
    pipe.fpsm = ctx.frame.psm;
    pipe.zbp = GSInternal::framePageBaseToBlock(ctx.zbuf.zbp);
    pipe.zpsm = ctx.zbuf.psm;
    pipe.test = ctx.test;
    pipe.ztestMethod = static_cast<uint32_t>((ctx.test >> 17) & 3u);
    pipe.abe = state.prim.abe;
    pipe.pabe = state.pabe;
    pipe.asel = ctx.alpha & 3;
    pipe.bsel = (ctx.alpha >> 2) & 3;
    pipe.csel = (ctx.alpha >> 4) & 3;
    pipe.dsel = (ctx.alpha >> 6) & 3;
    pipe.fix = static_cast<uint8_t>((ctx.alpha >> 32) & 0xFF);
    pipe.forceAlphaMsb = (ctx.fba & 0x1ull) != 0ull && ctx.frame.psm != GS_PSM_CT24;
    pipe.fbmsk = ctx.frame.fbmsk;
    pipe.zmask = ctx.zbuf.zmask;
    pipe.frame16 = bitsPerPixel(pipe.fpsm) == 16;
    pipe.dateRead = ((ctx.test >> 14) & 0x1u) != 0u &&
                    (pipe.fpsm == GS_PSM_CT32 || pipe.fpsm == GS_PSM_CT16 || pipe.fpsm == GS_PSM_CT16S);
    pipe.readFrame = m_readVramFuncs[pipe.fpsm & 0x3Fu];
    pipe.readZ = m_readVramFuncs[pipe.zpsm & 0x3Fu];
    pipe.writeFrame = m_writeVramFuncs[pipe.fpsm & 0x3Fu];
    pipe.writeZ = m_writeVramFuncs[pipe.zpsm & 0x3Fu];
    return pipe;
}

void GSCpuBackend::WritePixel(const GSDrawState &state, int x, int y, int z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t fog)
{
    WritePixel(makePixelPipe(state), x, y, z, r, g, b, a, fog);
}

void GSCpuBackend::WritePixel(const PixelPipe &pipe, int x, int y, int z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t fog)
{
    const GSDrawState &state = *pipe.state;
    const auto &ctx = state.context;
    // PS2X_PROBE="x,y,x2,y2,seconds": log every draw writing those two pixels
    // of FRAME 224-ish buffers during a short window (debug aid).
    static const struct Probe
    {
        int x0 = -1, y0 = -1, x1 = -1, y1 = -1;
        double at = 0.0;
        Probe()
        {
            if (const char *e = std::getenv("PS2X_PROBE"))
                std::sscanf(e, "%d,%d,%d,%d,%lf", &x0, &y0, &x1, &y1, &at);
        }
    } probe;
    if (__builtin_expect(probe.x0 >= 0, 0) && ((x == probe.x0 && y == probe.y0) || (x == probe.x1 && y == probe.y1)))
    {
        static const auto start = std::chrono::steady_clock::now();
        static std::atomic<int> logged{0};
        const double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (t >= probe.at && logged.fetch_add(1) < 600)
            std::fprintf(stderr, "[probe] (%d,%d) fbp=%u fpsm=0x%x prim=%u tme=%u abe=%u tbp=%u tpsm=0x%x alpha=0x%llx test=0x%llx fbmsk=0x%x in=(%u,%u,%u,%u)\n",
                         x, y, ctx.frame.fbp, ctx.frame.psm, (unsigned)state.prim.type, (unsigned)state.prim.tme, (unsigned)state.prim.abe,
                         ctx.tex0.tbp0, (unsigned)ctx.tex0.psm, (unsigned long long)ctx.alpha, (unsigned long long)ctx.test, ctx.frame.fbmsk, r, g, b, a);
    }
    if (x < pipe.scissorX0 || x > pipe.scissorX1 || y < pipe.rowY0 || y > pipe.rowY1)
        return;

    if (pipe.fge)
    {
        const uint32_t inverseFog = 255u - fog;
        auto applyFog = [&](uint8_t input, uint8_t fogColor) -> uint8_t
        {
            return static_cast<uint8_t>(((static_cast<uint32_t>(fog) * input) >> 8) + ((inverseFog * fogColor) >> 8));
        };

        r = applyFog(r, pipe.fogR);
        g = applyFog(g, pipe.fogG);
        b = applyFog(b, pipe.fogB);
    }

    const u32 fpsm = pipe.fpsm;
    const PixelWriteMask writeMask = classifyAlphaTest(pipe.test, a, static_cast<uint8_t>(fpsm));
    if (!writeMask.writesAnything())
        return;

    const bool preserveDestinationAlpha = writeMask.writeRgb && !writeMask.writeAlpha && fpsm == GS_PSM_CT32;
    // Avoid reading the framebuffer for simple draws.
    const bool frmw = pipe.dateRead || (writeMask.writesFramebuffer() && (pipe.fbmsk != 0 || pipe.abe || preserveDestinationAlpha));

    u32 rawFramebufferPixel = 0;
    u32 fbrgba = 0;
    if (frmw)
    {
        rawFramebufferPixel = pipe.readFrame(m_vram, pipe.fbp, pipe.fbw, x, y);
        fbrgba = rawFramebufferPixel;
        if (pipe.frame16)
            fbrgba = Rgba5551ToRgba8888(fbrgba);
        else if (fpsm == GS_PSM_CT24)
            fbrgba |= 0x80000000u; // the GS supplies 0x80 as destination alpha for RGB24 blending
    }

    if (pipe.dateRead && !passesDestinationAlphaTest(pipe.test, static_cast<uint8_t>(fpsm), rawFramebufferPixel))
        return;

    bool zpass = false;
    switch (pipe.ztestMethod)
    {
    case 0:
        zpass = false;
        break;
    case 1:
        zpass = true;
        break;
    case 2:
        zpass = static_cast<uint32_t>(z) >= pipe.readZ(m_vram, pipe.zbp, pipe.fbw, x, y);
        break;
    case 3:
        zpass = static_cast<uint32_t>(z) > pipe.readZ(m_vram, pipe.zbp, pipe.fbw, x, y);
        break;
    }
    if (!zpass)
        return;

    if (writeMask.writesFramebuffer())
    {
        // PABE disables alpha blending when the source alpha MSB is clear.
        if (pipe.abe && !(pipe.pabe && (a & 0x80u) == 0u))
        {
            const uint8_t dr = fbrgba & 0xFF;
            const uint8_t dg = (fbrgba >> 8) & 0xFF;
            const uint8_t db = (fbrgba >> 16) & 0xFF;
            const uint8_t da = (fbrgba >> 24) & 0xFF;
            auto pickRGB = [](uint8_t sel, int cs, int cd) -> int
            {
                if (sel == 0)
                    return cs;
                if (sel == 1)
                    return cd;
                return 0;
            };
            const int cAlpha = (pipe.csel == 0) ? a : (pipe.csel == 1) ? da : pipe.fix;
            r = clampU8(((pickRGB(pipe.asel, r, dr) - pickRGB(pipe.bsel, r, dr)) * cAlpha >> 7) + pickRGB(pipe.dsel, r, dr));
            g = clampU8(((pickRGB(pipe.asel, g, dg) - pickRGB(pipe.bsel, g, dg)) * cAlpha >> 7) + pickRGB(pipe.dsel, g, dg));
            b = clampU8(((pickRGB(pipe.asel, b, db) - pickRGB(pipe.bsel, b, db)) * cAlpha >> 7) + pickRGB(pipe.dsel, b, db));
        }

        if (writeMask.writeAlpha && pipe.forceAlphaMsb)
            a = static_cast<uint8_t>(a | 0x80u);

        u32 pixel = pack32(r, g, b, a);
        if (pipe.fbmsk != 0)
            pixel = (pixel & ~pipe.fbmsk) | (fbrgba & pipe.fbmsk);
        if (preserveDestinationAlpha)
            pixel = (pixel & 0x00FFFFFFu) | (fbrgba & 0xFF000000u);
        if (pipe.frame16)
            pixel = Rgba8888ToRgba5551(pixel);
        pipe.writeFrame(m_vram, pipe.fbp, pipe.fbw, x, y, pixel);
    }

    if (writeMask.writeDepth && !pipe.zmask)
        pipe.writeZ(m_vram, pipe.zbp, pipe.fbw, x, y, z);
}

uint32_t GSCpuBackend::LookupCLUT(const GSDrawState &state,
                                  uint8_t index,
                                  uint8_t cpsm,
                                  uint8_t csm,
                                  uint8_t csa,
                                  uint8_t sourcePsm)
{
    const uint16_t *clut = t_clut ? t_clut : m_clut.data();
    const bool sixteenBit = cpsm == GS_PSM_CT16 || cpsm == GS_PSM_CT16S;
    const uint32_t csaMask = sixteenBit ? 0x1Fu : 0x0Fu;
    const uint32_t clutBase = (static_cast<uint32_t>(csa) & csaMask) << 4u;
    const uint32_t sourceIndex = isFourBitIndexedPsm(sourcePsm)
                                     ? (static_cast<uint32_t>(index) & 0x0Fu)
                                     : static_cast<uint32_t>(index);

    uint32_t clutIndex = (clutBase + sourceIndex) & (sixteenBit ? 0x1FFu : 0x0FFu);
    if (!sixteenBit && csm == 0u && isEightBitIndexedPsm(sourcePsm))
    {
        const uint32_t block = std::min((sourceIndex & 0xF0u) + clutBase, 240u);
        clutIndex = block + (sourceIndex & 0x0Fu);
    }

    switch (cpsm)
    {
    case GS_PSM_CT32:
    {
        const uint32_t raw = static_cast<uint32_t>(clut[clutIndex]) | (static_cast<uint32_t>(clut[clutIndex + 256u]) << 16u);
        return applyTexa(state.texa, cpsm, raw);
    }
    case GS_PSM_CT24:
    {
        const uint32_t raw = static_cast<uint32_t>(clut[clutIndex]) | (static_cast<uint32_t>(clut[clutIndex + 256u]) << 16u);
        return applyTexa(state.texa, cpsm, raw & 0x00FFFFFFu);
    }
    case GS_PSM_CT16:
    case GS_PSM_CT16S:
        return applyTexa(state.texa, cpsm, Rgba5551ToRgba8888(clut[clutIndex]));
    default:
        break;
    }

    return 0xFFFF00FFu;
}

uint32_t GSCpuBackend::SampleTexture(const GSDrawState &state, float s, float t, float q, uint16_t u, uint16_t v)
{
    const auto &ctx = state.context;
    const auto &tex = ctx.tex0;

    const int texW = state.textureWidth;
    const int texH = state.textureHeight;
    const uint64_t clamp = ctx.clamp;
    const uint8_t wrapU = static_cast<uint8_t>(clamp & 0x3u);
    const uint8_t wrapV = static_cast<uint8_t>((clamp >> 2) & 0x3u);
    const uint16_t minU = static_cast<uint16_t>((clamp >> 4) & 0x3FFu);
    const uint16_t maxU = static_cast<uint16_t>((clamp >> 14) & 0x3FFu);
    const uint16_t minV = static_cast<uint16_t>((clamp >> 24) & 0x3FFu);
    const uint16_t maxV = static_cast<uint16_t>((clamp >> 34) & 0x3FFu);

    float texUf, texVf;
    if (state.prim.fst)
    {
        texUf = static_cast<float>(u) / 16.0f;
        texVf = static_cast<float>(v) / 16.0f;
    }
    else
    {
        const float invQ = 1.0f / fabsQ(q);
        texUf = s * invQ * static_cast<float>(texW);
        texVf = t * invQ * static_cast<float>(texH);
    }

    const DecodedTexture *decoded = static_cast<const DecodedTexture *>(t_texture);
    auto samplePoint = [&](int sampleU, int sampleV) -> uint32_t
    {
        sampleU = wrapTextureCoordinate(sampleU, texW, wrapU, minU, maxU);
        sampleV = wrapTextureCoordinate(sampleV, texH, wrapV, minV, maxV);
        if (decoded && static_cast<uint32_t>(sampleU) < decoded->width && static_cast<uint32_t>(sampleV) < decoded->height)
            return decoded->texels[static_cast<size_t>(sampleV) * decoded->width + static_cast<uint32_t>(sampleU)];

        u32 out = ReadTextureVramUnlocked(tex.psm, tex.tbp0, tex.tbw, sampleU, sampleV);

        switch (tex.psm)
        {
        case GS_PSM_CT32:
        case GS_PSM_Z32:
        case GS_PSM_CT24:
        case GS_PSM_Z24:
            return applyTexa(state.texa, tex.psm, out);
        case GS_PSM_CT16:
        case GS_PSM_CT16S:
        case GS_PSM_Z16:
        case GS_PSM_Z16S:
            return applyTexa(state.texa, tex.psm, Rgba5551ToRgba8888(out));
        case GS_PSM_T8:
        case GS_PSM_T8H:
        case GS_PSM_T4:
        case GS_PSM_T4HL:
        case GS_PSM_T4HH:
            return LookupCLUT(state, static_cast<u8>(out), tex.cpsm, tex.csm, tex.csa, tex.psm);
        }

        return 0xFFFF00FFu;
    };

    if (!state.linearFilter)
    {
        return samplePoint(static_cast<int>(texUf), static_cast<int>(texVf));
    }

    const float sampleU = texUf - 0.5f;
    const float sampleV = texVf - 0.5f;
    const int u0 = static_cast<int>(std::floor(sampleU));
    const int v0 = static_cast<int>(std::floor(sampleV));
    const int u1 = u0 + 1;
    const int v1 = v0 + 1;
    const float fx = sampleU - static_cast<float>(u0);
    const float fy = sampleV - static_cast<float>(v0);

    uint32_t c00, c10, c01, c11;
    // Decoded texture: wrap each coordinate once and read the four texels directly.
    const int wu0 = decoded ? wrapTextureCoordinate(u0, texW, wrapU, minU, maxU) : 0;
    const int wu1 = decoded ? wrapTextureCoordinate(u1, texW, wrapU, minU, maxU) : 0;
    const int wv0 = decoded ? wrapTextureCoordinate(v0, texH, wrapV, minV, maxV) : 0;
    const int wv1 = decoded ? wrapTextureCoordinate(v1, texH, wrapV, minV, maxV) : 0;
    if (decoded && static_cast<uint32_t>(wu0) < decoded->width && static_cast<uint32_t>(wu1) < decoded->width &&
        static_cast<uint32_t>(wv0) < decoded->height && static_cast<uint32_t>(wv1) < decoded->height)
    {
        const uint32_t *row0 = decoded->texels.data() + static_cast<size_t>(wv0) * decoded->width;
        const uint32_t *row1 = decoded->texels.data() + static_cast<size_t>(wv1) * decoded->width;
        c00 = row0[wu0];
        c10 = row0[wu1];
        c01 = row1[wu0];
        c11 = row1[wu1];
    }
    else
    {
        c00 = samplePoint(u0, v0);
        c10 = samplePoint(u1, v0);
        c01 = samplePoint(u0, v1);
        c11 = samplePoint(u1, v1);
    }

    const uint8_t r = lerpChannel(static_cast<uint8_t>(c00 & 0xFFu),
                                  static_cast<uint8_t>(c10 & 0xFFu),
                                  static_cast<uint8_t>(c01 & 0xFFu),
                                  static_cast<uint8_t>(c11 & 0xFFu),
                                  fx, fy);
    const uint8_t g = lerpChannel(static_cast<uint8_t>((c00 >> 8) & 0xFFu),
                                  static_cast<uint8_t>((c10 >> 8) & 0xFFu),
                                  static_cast<uint8_t>((c01 >> 8) & 0xFFu),
                                  static_cast<uint8_t>((c11 >> 8) & 0xFFu),
                                  fx, fy);
    const uint8_t b = lerpChannel(static_cast<uint8_t>((c00 >> 16) & 0xFFu),
                                  static_cast<uint8_t>((c10 >> 16) & 0xFFu),
                                  static_cast<uint8_t>((c01 >> 16) & 0xFFu),
                                  static_cast<uint8_t>((c11 >> 16) & 0xFFu),
                                  fx, fy);
    const uint8_t a = lerpChannel(static_cast<uint8_t>((c00 >> 24) & 0xFFu),
                                  static_cast<uint8_t>((c10 >> 24) & 0xFFu),
                                  static_cast<uint8_t>((c01 >> 24) & 0xFFu),
                                  static_cast<uint8_t>((c11 >> 24) & 0xFFu),
                                  fx, fy);

    return static_cast<uint32_t>(r) |
           (static_cast<uint32_t>(g) << 8) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(a) << 24);
}

void GSCpuBackend::DrawSprite(const GSPrimitiveBatch &batch)
{
    const GSDrawState &state = batch.state;
    const PixelPipe pipe = makePixelPipe(state);
    const GSVertex &v0 = batch.vertices[0];
    const GSVertex &v1 = batch.vertices[1];
    const auto &ctx = state.context;

    // GS rasterization: pixel centres are at integer coordinates and a sprite
    // covers ceil(x0) <= x < ceil(x1) (likewise in y), using the exact 12.4
    // positions. Texture coordinates are interpolated from those positions,
    // which is what half-pixel offsets in downsample/blur passes rely on.
    const float ofx = static_cast<float>(ctx.xyoffset.ofx) / 16.0f;
    const float ofy = static_cast<float>(ctx.xyoffset.ofy) / 16.0f;
    const float sx0 = v0.x - ofx;
    const float sy0 = v0.y - ofy;
    const float sx1 = v1.x - ofx;
    const float sy1 = v1.y - ofy;
    u32 z1 = static_cast<u32>(v1.z);

    const int unclippedX0 = static_cast<int>(std::ceil(std::min(sx0, sx1)));
    const int unclippedY0 = static_cast<int>(std::ceil(std::min(sy0, sy1)));
    const int unclippedX1 = static_cast<int>(std::ceil(std::max(sx0, sx1))) - 1;
    const int unclippedY1 = static_cast<int>(std::ceil(std::max(sy0, sy1))) - 1;
    if (unclippedX1 < unclippedX0 || unclippedY1 < unclippedY0)
        return;

    // If the sprite rectangle is fully outside scissor, nothing should render.
    if (unclippedX1 < ctx.scissor.x0 || unclippedX0 > ctx.scissor.x1 ||
        unclippedY1 < ctx.scissor.y0 || unclippedY0 > ctx.scissor.y1)
        return;

    const int drawX0 = clampInt(unclippedX0, ctx.scissor.x0, ctx.scissor.x1);
    const int drawY0 = clampInt(unclippedY0, ctx.scissor.y0, ctx.scissor.y1);
    const int drawX1 = clampInt(unclippedX1, ctx.scissor.x0, ctx.scissor.x1);
    const int drawY1Scissor = clampInt(unclippedY1, ctx.scissor.y0, ctx.scissor.y1);
    const int drawY0Band = std::max(drawY0, t_bandY0);
    const int drawY1 = std::min(drawY1Scissor, t_bandY1);
    if (drawY0Band > drawY1)
        return;
    const float invDx = (sx1 != sx0) ? 1.0f / (sx1 - sx0) : 0.0f;
    const float invDy = (sy1 != sy0) ? 1.0f / (sy1 - sy0) : 0.0f;

    const uint64_t alphaReg = ctx.alpha;
    const uint8_t alphaMode = static_cast<uint8_t>(alphaReg & 0xFFu);
    const uint8_t alphaFix = static_cast<uint8_t>((alphaReg >> 32) & 0xFFu);

    uint8_t r = v1.r, g = v1.g, b = v1.b, a = v1.a;

    if (state.prim.tme)
    {
        const auto &tex = ctx.tex0;
        const int texW = state.textureWidth;
        const int texH = state.textureHeight;

        float u0f, v0f, u1f, v1f;
        if (state.prim.fst)
        {
            u0f = static_cast<float>(v0.u) / 16.0f; // 12.4 fixed point
            v0f = static_cast<float>(v0.v) / 16.0f;
            u1f = static_cast<float>(v1.u) / 16.0f;
            v1f = static_cast<float>(v1.v) / 16.0f;
        }
        else
        {
            const float q0 = fabsQ(v0.q);
            const float q1 = fabsQ(v1.q);
            u0f = (v0.s / q0) * static_cast<float>(texW);
            v0f = (v0.t / q0) * static_cast<float>(texH);
            u1f = (v1.s / q1) * static_cast<float>(texW);
            v1f = (v1.t / q1) * static_cast<float>(texH);
        }

        auto drawRow = [&](int y)
        {
            const float ty = (static_cast<float>(y) - sy0) * invDy;
            float texVf = lerpf(v0f, v1f, ty);

            for (int x = drawX0; x <= drawX1; ++x)
            {
                const float tx = (static_cast<float>(x) - sx0) * invDx;
                float texUf = lerpf(u0f, u1f, tx);
                uint32_t texel = 0xFFFF00FFu;
                if (state.prim.fst)
                {
                    const int fixedU = static_cast<int>((texUf * 16.0f) + 0.5f);
                    const int fixedV = static_cast<int>((texVf * 16.0f) + 0.5f);
                    const uint16_t sampleU = static_cast<uint16_t>(clampInt(fixedU, 0, 0xFFFF));
                    const uint16_t sampleV = static_cast<uint16_t>(clampInt(fixedV, 0, 0xFFFF));
                    texel = SampleTexture(state, 0.0f, 0.0f, 1.0f, sampleU, sampleV);
                }
                else
                {
                    texel = SampleTexture(state, texUf / static_cast<float>(texW), texVf / static_cast<float>(texH), 1.0f, 0u, 0u);
                }

                uint8_t tr = static_cast<uint8_t>(texel & 0xFF);
                uint8_t tg = static_cast<uint8_t>((texel >> 8) & 0xFF);
                uint8_t tb = static_cast<uint8_t>((texel >> 16) & 0xFF);
                uint8_t ta = static_cast<uint8_t>((texel >> 24) & 0xFF);

                const TextureCombineResult color = combineTexture(tex, r, g, b, a, tr, tg, tb, ta);
                WritePixel(pipe, x, y, z1, color.r, color.g, color.b, color.a, v1.fog);
            }
        };
        drawRowsParallel(drawY0Band, drawY1, drawX1 - drawX0 + 1, drawRow);
    }
    else
    {
        drawRowsParallel(drawY0Band, drawY1, drawX1 - drawX0 + 1, [&](int y)
                         {
                             for (int x = drawX0; x <= drawX1; ++x)
                                 WritePixel(pipe, x, y, z1, r, g, b, a, v1.fog);
                         });
    }
}

void GSCpuBackend::drawRowsParallel(int y0, int y1, int rowWidth, const std::function<void(int)> &drawRow)
{
    // Rows write disjoint pixels, so large primitives can be split into bands.
    // Feedback draws (texture overlapping the target) stay serial and use the
    // page cache to keep their read-before-write behaviour.
    const int rows = y1 - y0 + 1;
    constexpr int kMinParallelPixels = 8192;
    constexpr int kRowsPerBand = 8;
    if (inBandMode() || rows <= kRowsPerBand || rows * rowWidth < kMinParallelPixels || !m_textureDirectRead)
    {
        for (int y = y0; y <= y1; ++y)
            drawRow(y);
        return;
    }
    const int bands = (rows + kRowsPerBand - 1) / kRowsPerBand;
    GSParallelFor::instance().run(bands, [&](int band)
                                  {
                                      const int first = y0 + band * kRowsPerBand;
                                      const int last = std::min(y1, first + kRowsPerBand - 1);
                                      for (int y = first; y <= last; ++y)
                                          drawRow(y);
                                  });
}

void GSCpuBackend::DrawTriangle(const GSPrimitiveBatch &batch)
{
    const GSDrawState &state = batch.state;
    const GSVertex &v0 = batch.vertices[0];
    const GSVertex &v1 = batch.vertices[1];
    const GSVertex &v2 = batch.vertices[2];
    const auto &ctx = state.context;

    int ofx = ctx.xyoffset.ofx >> 4;
    int ofy = ctx.xyoffset.ofy >> 4;

    float fx0 = v0.x - static_cast<float>(ofx);
    float fy0 = v0.y - static_cast<float>(ofy);
    float fx1 = v1.x - static_cast<float>(ofx);
    float fy1 = v1.y - static_cast<float>(ofy);
    float fx2 = v2.x - static_cast<float>(ofx);
    float fy2 = v2.y - static_cast<float>(ofy);

    int minX = static_cast<int>(std::floor(std::min({fx0, fx1, fx2})));
    int maxX = static_cast<int>(std::ceil(std::max({fx0, fx1, fx2})));
    int minY = static_cast<int>(std::floor(std::min({fy0, fy1, fy2})));
    int maxY = static_cast<int>(std::ceil(std::max({fy0, fy1, fy2})));

    minX = clampInt(minX, ctx.scissor.x0, ctx.scissor.x1);
    maxX = clampInt(maxX, ctx.scissor.x0, ctx.scissor.x1);
    minY = clampInt(minY, ctx.scissor.y0, ctx.scissor.y1);
    maxY = clampInt(maxY, ctx.scissor.y0, ctx.scissor.y1);
    minY = std::max(minY, t_bandY0);
    maxY = std::min(maxY, t_bandY1);
    if (minY > maxY)
        return;

    float denom = (fy1 - fy2) * (fx0 - fx2) + (fx2 - fx1) * (fy0 - fy2);
    if (std::fabs(denom) < 0.001f)
        return;

    const float winding = (denom < 0.0f) ? -1.0f : 1.0f;
    const float invAbsDenom = 1.0f / std::fabs(denom);
    constexpr float kEdgeEpsilon = 1.0e-4f;

    // Each barycentric weight is linear in x along a row: w = k * px + c.
    // Solving w >= -eps per edge gives the row's covered span; it is padded by
    // a pixel and every pixel still runs the exact test below, so coverage is
    // unchanged while pixels far outside the triangle are skipped.
    const PixelPipe pipe = makePixelPipe(state);
    const DepthPair z0 = depthSplit(v0.z), z1 = depthSplit(v1.z), z2 = depthSplit(v2.z);
    const float k0 = (fy1 - fy2) * winding * invAbsDenom;
    const float k1 = (fy2 - fy0) * winding * invAbsDenom;
    const float k2 = -(k0 + k1);
    for (int y = minY; y <= maxY; ++y)
    {
        float py = static_cast<float>(y) + 0.5f;
        const float c0 = ((fy1 - fy2) * -fx2 + (fx2 - fx1) * (py - fy2)) * winding * invAbsDenom;
        const float c1 = ((fy2 - fy0) * -fx2 + (fx0 - fx2) * (py - fy2)) * winding * invAbsDenom;
        const float c2 = 1.0f - c0 - c1;
        float spanLo = static_cast<float>(minX), spanHi = static_cast<float>(maxX) + 1.0f;
        bool empty = false;
        for (const auto [k, c] : {std::pair{k0, c0}, std::pair{k1, c1}, std::pair{k2, c2}})
        {
            if (std::fabs(k) < 1.0e-12f)
            {
                empty |= c < -kEdgeEpsilon * 4.0f;
                continue;
            }
            const float bound = (-kEdgeEpsilon - c) / k - 0.5f; // in pixel-index space
            if (k > 0.0f)
                spanLo = std::max(spanLo, bound);
            else
                spanHi = std::min(spanHi, bound);
        }
        if (empty || !(spanLo <= spanHi + 2.0f))
            continue;
        const int rowX0 = std::max(minX, static_cast<int>(std::floor(spanLo)) - 1);
        const int rowX1 = std::min(maxX, static_cast<int>(std::ceil(spanHi)) + 1);
        for (int x = rowX0; x <= rowX1; ++x)
        {
            float px = static_cast<float>(x) + 0.5f;

            float w0 = (std::fma(fy1 - fy2, px - fx2, (fx2 - fx1) * (py - fy2)) * winding) * invAbsDenom;
            float w1 = (std::fma(fy2 - fy0, px - fx2, (fx0 - fx2) * (py - fy2)) * winding) * invAbsDenom;
            float w2 = 1.0f - w0 - w1;

            if (w0 < -kEdgeEpsilon || w1 < -kEdgeEpsilon || w2 < -kEdgeEpsilon)
                continue;

            const uint32_t z = depthRound(depthAdd(depthAdd(depthMul(w0, z0), depthMul(w1, z1)), depthMul(w2, z2)));

            uint8_t r, g, b, a;
            if (state.prim.iip)
            {
                r = clampU8(static_cast<int>(interp3(v0.r, v1.r, v2.r, w0, w1, w2)));
                g = clampU8(static_cast<int>(interp3(v0.g, v1.g, v2.g, w0, w1, w2)));
                b = clampU8(static_cast<int>(interp3(v0.b, v1.b, v2.b, w0, w1, w2)));
                a = clampU8(static_cast<int>(interp3(v0.a, v1.a, v2.a, w0, w1, w2)));
            }
            else
            {
                r = v2.r;
                g = v2.g;
                b = v2.b;
                a = v2.a;
            }

            if (state.prim.tme)
            {
                float is, it, iq;
                uint16_t iu, iv;
                if (state.prim.fst)
                {
                    iu = static_cast<uint16_t>(interp3(v0.u, v1.u, v2.u, w0, w1, w2));
                    iv = static_cast<uint16_t>(interp3(v0.v, v1.v, v2.v, w0, w1, w2));
                    is = 0.0f;
                    it = 0.0f;
                    iq = 1.0f;
                }
                else
                {
                    // The GS DDA interpolates the homogeneous S, T and Q
                    // values. Texel coordinates are calculated from S/Q and
                    // T/Q only after interpolation.
                    is = interp3(v0.s, v1.s, v2.s, w0, w1, w2);
                    it = interp3(v0.t, v1.t, v2.t, w0, w1, w2);
                    iq = interp3(v0.q, v1.q, v2.q, w0, w1, w2);
                    iu = 0;
                    iv = 0;
                }

                uint32_t texel = SampleTexture(state, is, it, iq, iu, iv);

                uint8_t tr = static_cast<uint8_t>(texel & 0xFF);
                uint8_t tg = static_cast<uint8_t>((texel >> 8) & 0xFF);
                uint8_t tb = static_cast<uint8_t>((texel >> 16) & 0xFF);
                uint8_t ta = static_cast<uint8_t>((texel >> 24) & 0xFF);

                const auto &tex = ctx.tex0;
                const uint8_t shadeR = r;
                const uint8_t shadeG = g;
                const uint8_t shadeB = b;
                const uint8_t shadeA = a;
                const TextureCombineResult color = combineTexture(tex, shadeR, shadeG, shadeB, shadeA, tr, tg, tb, ta);

                r = color.r;
                g = color.g;
                b = color.b;
                a = color.a;
            }

            const uint8_t fog = clampU8(static_cast<int>(interp3(v0.fog, v1.fog, v2.fog, w0, w1, w2)));
            WritePixel(pipe, x, y, z, r, g, b, a, fog);
        }
    }
}

void GSCpuBackend::DrawLine(const GSPrimitiveBatch &batch)
{
    const GSDrawState &state = batch.state;
    const PixelPipe pipe = makePixelPipe(state);
    const GSVertex &v0 = batch.vertices[0];
    const GSVertex &v1 = batch.vertices[1];
    const auto &ctx = state.context;

    int ofx = ctx.xyoffset.ofx >> 4;
    int ofy = ctx.xyoffset.ofy >> 4;

    int x0 = static_cast<int>(v0.x) - ofx;
    int y0 = static_cast<int>(v0.y) - ofy;
    int x1 = static_cast<int>(v1.x) - ofx;
    int y1 = static_cast<int>(v1.y) - ofy;

    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    int totalSteps = std::max(std::abs(x1 - x0), std::abs(y1 - y0));
    if (totalSteps == 0)
        totalSteps = 1;
    int step = 0;

    for (;;)
    {
        float t = static_cast<float>(step) / static_cast<float>(totalSteps);
        uint8_t r, g, b, a;
        if (state.prim.iip)
        {
            r = clampU8(static_cast<int>(v0.r + (v1.r - v0.r) * t));
            g = clampU8(static_cast<int>(v0.g + (v1.g - v0.g) * t));
            b = clampU8(static_cast<int>(v0.b + (v1.b - v0.b) * t));
            a = clampU8(static_cast<int>(v0.a + (v1.a - v0.a) * t));
        }
        else
        {
            r = v1.r;
            g = v1.g;
            b = v1.b;
            a = v1.a;
        }

        double z = (v0.z + (v1.z - v0.z) * t);
        const uint8_t fog = clampU8(static_cast<int>(v0.fog + (v1.fog - v0.fog) * t));
        WritePixel(pipe, x0, y0, static_cast<u32>(z), r, g, b, a, fog);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
        ++step;
    }
}

bool GSCpuBackend::uploadTouchesPending(const GSTransferCommand &command) const
{
    if (m_pending.empty() && m_gpuInflight.empty())
        return false;
    // Only the pages under the destination rectangle matter.
    uint32_t pw = 0, ph = 0;
    pageDims(command.bitbltbuf.dpsm, pw, ph);
    const uint32_t pagesPerRow = std::max<uint32_t>(1u, (std::max<uint32_t>(1u, command.bitbltbuf.dbw) * 64u + pw - 1u) / pw);
    const uint32_t basePage = command.bitbltbuf.dbp / 32u;
    const uint32_t spill = (command.bitbltbuf.dbp % 32u) != 0u ? 1u : 0u; // block offset can cross a page
    const uint32_t x0 = command.trxpos.dsax, y0 = command.trxpos.dsay;
    const uint32_t x1 = x0 + std::max<uint32_t>(1u, command.trxreg.rrw) - 1u;
    const uint32_t y1 = y0 + std::max<uint32_t>(1u, command.trxreg.rrh) - 1u;
    for (uint32_t py = y0 / ph; py <= y1 / ph; ++py)
    {
        const uint64_t rowFirst = static_cast<uint64_t>(basePage) + static_cast<uint64_t>(py) * pagesPerRow + x0 / pw;
        const uint64_t rowLast = static_cast<uint64_t>(basePage) + static_cast<uint64_t>(py) * pagesPerRow + x1 / pw + spill;
        if (pendingTouches(rowFirst * 8192u, (rowLast + 1u) * 8192u))
            return true;
    }
    return false;
}

void GSCpuBackend::BeginTransfer(const GSTransferCommand &command)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (command.direction == 0u)
    {
        // Uploads only have to wait for queued draws touching the destination.
        bufferRange(command.bitbltbuf.dbp * 256u, std::max<uint32_t>(1u, command.bitbltbuf.dbw) * 64u,
                    static_cast<uint32_t>(command.trxpos.dsay) + command.trxreg.rrh, command.bitbltbuf.dpsm,
                    m_uploadRange.first, m_uploadRange.second);
        // dsax can push the rectangle one page column further.
        m_uploadRange.second += 8192u;
        if (uploadTouchesPending(command))
            flushPendingUnlocked(gpuUploads() ? "GpuUpload" : "BeginTransfer");
    }
    else
    {
        flushPendingUnlocked("BeginTransfer");
    }
    m_transfer = command;
    m_transferState.x = command.trxpos.dsax;
    m_transferState.y = command.trxpos.dsay;
    m_transferState.totalPixels = static_cast<uint32_t>(command.trxreg.rrw) * static_cast<uint32_t>(command.trxreg.rrh);
    m_transferState.copiedPixels = 0u;
    m_transferState.direction = command.direction;
    m_transferState.localToHostPendingBytes = 0u;

    if (command.direction == 2u)
    {
        uint64_t begin = 0, end = 0;
        bufferRange(command.bitbltbuf.dbp * 256u, std::max<uint32_t>(1u, command.bitbltbuf.dbw) * 64u,
                    static_cast<uint32_t>(command.trxpos.dsay) + command.trxreg.rrh, command.bitbltbuf.dpsm, begin, end);
        markVramWritten(begin, end + 8192u);
        PerformLocalToLocalTransfer();
    }
    else if (command.direction == 1u)
        PerformLocalToHostTransfer();
}

bool GSCpuBackend::gpuUploads() const
{
#ifdef PS2X_HAVE_METAL
    static const bool disabled = std::getenv("PS2X_GS_GPU_UPLOAD") && std::atoi(std::getenv("PS2X_GS_GPU_UPLOAD")) == 0;
    return m_gpu && !disabled && !std::getenv("PS2X_GS_GPU_VERIFY");
#else
    return false;
#endif
}

#ifdef PS2X_HAVE_METAL
// An IMAGE piece over memory the GPU still uses: write it on the GPU, after
// the queued work, instead of waiting for the GPU and writing it here.
bool GSCpuBackend::uploadImageOnGpu(const uint8_t *data, uint32_t sizeBytes)
{
    if (!gpuUploads() || !data || sizeBytes == 0u || m_transferState.direction != 0u ||
        m_transfer.trxreg.rrw == 0u || m_transfer.trxreg.rrh == 0u || m_transferState.totalPixels == 0u)
        return false;
    uint32_t pixels = 0u;
    switch (m_transfer.bitbltbuf.dpsm)
    {
    case GS_PSM_CT32: case GS_PSM_Z32:
        if (sizeBytes % 4u)
            return false;
        pixels = sizeBytes / 4u;
        break;
    case GS_PSM_CT24: case GS_PSM_Z24:
        if (sizeBytes % 3u)
            return false;
        pixels = sizeBytes / 3u;
        break;
    case GS_PSM_CT16: case GS_PSM_CT16S: case GS_PSM_Z16: case GS_PSM_Z16S:
        if (sizeBytes % 2u)
            return false;
        pixels = sizeBytes / 2u;
        break;
    case GS_PSM_T8: case GS_PSM_T8H:
        pixels = sizeBytes;
        break;
    case GS_PSM_T4: case GS_PSM_T4HL: case GS_PSM_T4HH:
        if (m_transferState.copiedPixels % 2u)
            return false;
        pixels = sizeBytes * 2u;
        break;
    default:
        return false;
    }
    if (!uploadTouchesPending(m_transfer))
        return false; // nothing to order against: the CPU writes it directly
    pixels = std::min(pixels, m_transferState.totalPixels - m_transferState.copiedPixels);
    flushPendingUnlocked("GpuUpload");
    const GSGpuUpload upload{m_transfer.bitbltbuf.dbp, std::max<uint32_t>(m_transfer.bitbltbuf.dbw, 1u),
                             m_transfer.bitbltbuf.dpsm, m_transfer.trxpos.dsax, m_transfer.trxpos.dsay,
                             m_transfer.trxreg.rrw, m_transferState.copiedPixels, pixels};
    m_gpu->upload(upload, data, sizeBytes);
    // PS2X_GS_GPU_UPLOAD_VERIFY=1: redo the piece on the CPU and compare.
    static const bool verify = std::getenv("PS2X_GS_GPU_UPLOAD_VERIFY") != nullptr;
    if (verify)
    {
        static uint64_t checked = 0, bad = 0;
        gpuWaitIdle();
        std::vector<uint8_t> gpuResult(m_vram, m_vram + m_vramSize);
        const GSTransferSnapshot saved = m_transferState;
        // Undo the GPU write by replaying the CPU writer over a copy taken
        // before it is not possible here; compare against a CPU write into
        // the GPU result instead (idempotent when both agree).
        uploadImageCpuUnlocked(data, sizeBytes);
        m_transferState = saved;
        ++checked;
        size_t diff = 0, first = SIZE_MAX;
        for (size_t i = 0; i < m_vramSize; ++i)
            if (gpuResult[i] != m_vram[i])
            {
                if (first == SIZE_MAX)
                    first = i;
                ++diff;
            }
        if (diff && ++bad <= 20)
            std::fprintf(stderr, "[gs:upload-verify] psm=0x%x dbp=%u dbw=%u xy=(%u,%u) rrw=%u first=%u count=%u: %zu bytes differ, first at 0x%zx (cpu %02x gpu %02x)\n",
                         upload.dpsm, upload.dbp, upload.dbw, upload.dsax, upload.dsay, upload.rrw, upload.firstPixel, upload.count,
                         diff, first, m_vram[first], gpuResult[first]);
        if ((checked & 1023u) == 0u)
            std::fprintf(stderr, "[gs:upload-verify] %llu pieces, %llu differ\n", (unsigned long long)checked, (unsigned long long)bad);
    }
    addInflightRange(m_uploadRange);
    markVramWritten(m_uploadRange.first, m_uploadRange.second);
    m_transferState.copiedPixels += pixels;
    if (m_transferState.copiedPixels >= m_transferState.totalPixels)
    {
        m_transferState.direction = 3u;
        m_transferState.totalPixels = 0u;
    }
    else
    {
        m_transferState.x = m_transfer.trxpos.dsax + (m_transferState.copiedPixels % m_transfer.trxreg.rrw);
        m_transferState.y = m_transfer.trxpos.dsay + (m_transferState.copiedPixels / m_transfer.trxreg.rrw);
    }
    return true;
}
#endif

void GSCpuBackend::UploadImage(const uint8_t *data, uint32_t sizeBytes)
{
    std::lock_guard<std::mutex> lock(m_mutex);
#ifdef PS2X_HAVE_METAL
    if (uploadImageOnGpu(data, sizeBytes))
        return;
#endif
    // Draws can be queued between the pieces of an IMAGE transfer.
    if (uploadTouchesPending(m_transfer))
        flushPendingUnlocked("UploadImage");
    uploadImageCpuUnlocked(data, sizeBytes);
}

void GSCpuBackend::uploadImageCpuUnlocked(const uint8_t *data, uint32_t sizeBytes)
{
    // Games re-upload the same textures every frame: only pages whose bytes
    // change count as written, so textures decoded from the rest stay valid.
    const uint64_t begin = std::min<uint64_t>(m_uploadRange.first / 8192u * 8192u, m_vramSize);
    const uint64_t end = std::min<uint64_t>((m_uploadRange.second + 8191u) / 8192u * 8192u, m_vramSize);
    if (!m_vram || end <= begin)
    {
        markVramWritten(m_uploadRange.first, m_uploadRange.second);
        uploadImageCpuWrite(data, sizeBytes);
        return;
    }
    m_uploadBefore.assign(m_vram + begin, m_vram + end);
    uploadImageCpuWrite(data, sizeBytes);
    for (uint64_t page = begin; page < end; page += 8192u)
        if (std::memcmp(m_uploadBefore.data() + (page - begin), m_vram + page, 8192u) != 0)
            markVramWritten(page, page + 8192u);
}

void GSCpuBackend::uploadImageCpuWrite(const uint8_t *data, uint32_t sizeBytes)
{
    if (!data || sizeBytes == 0u || !m_vram || m_transferState.direction != 0u)
        return;
    if (m_transfer.trxreg.rrw == 0u || m_transfer.trxreg.rrh == 0u || m_transferState.totalPixels == 0u)
        return;

    const uint32_t dbp = m_transfer.bitbltbuf.dbp;
    const uint32_t dbw = std::max<uint32_t>(m_transfer.bitbltbuf.dbw, 1u);
    const uint8_t dpsm = m_transfer.bitbltbuf.dpsm;
    const uint32_t rrw = m_transfer.trxreg.rrw;
    const uint32_t dsax = m_transfer.trxpos.dsax;
    uint32_t offset = 0u;

    auto advancePixel = [&](uint32_t count)
    {
        const uint32_t totalPixels = m_transferState.totalPixels;
        m_transferState.copiedPixels =
            std::min<uint32_t>(totalPixels, m_transferState.copiedPixels + count);

        if (m_transferState.copiedPixels >= totalPixels)
        {
            m_transferState.direction = 3u;
            m_transferState.totalPixels = 0u;
            return;
        }

        m_transferState.x = dsax + (m_transferState.copiedPixels % rrw);
        m_transferState.y = m_transfer.trxpos.dsay + (m_transferState.copiedPixels / rrw);
    };

    while (offset < sizeBytes && m_transferState.direction == 0u)
    {
        switch (dpsm)
        {
        case GS_PSM_CT32:
        case GS_PSM_Z32:
        {
            if (sizeBytes - offset < 4u)
                return;
            uint32_t value = 0u;
            std::memcpy(&value, data + offset, sizeof(value));
            WriteVramUnlocked(dpsm, dbp, dbw, m_transferState.x, m_transferState.y, value);
            offset += 4u;
            advancePixel(1u);
            break;
        }
        case GS_PSM_CT24:
        case GS_PSM_Z24:
        {
            if (sizeBytes - offset < 3u)
                return;
            const uint32_t value = static_cast<uint32_t>(data[offset]) |
                                   (static_cast<uint32_t>(data[offset + 1u]) << 8u) |
                                   (static_cast<uint32_t>(data[offset + 2u]) << 16u);
            WriteVramUnlocked(dpsm, dbp, dbw, m_transferState.x, m_transferState.y, value);
            offset += 3u;
            advancePixel(1u);
            break;
        }
        case GS_PSM_CT16:
        case GS_PSM_CT16S:
        case GS_PSM_Z16:
        case GS_PSM_Z16S:
        {
            if (sizeBytes - offset < 2u)
                return;
            uint16_t value = 0u;
            std::memcpy(&value, data + offset, sizeof(value));
            WriteVramUnlocked(dpsm, dbp, dbw, m_transferState.x, m_transferState.y, value);
            offset += 2u;
            advancePixel(1u);
            break;
        }
        case GS_PSM_T8:
        case GS_PSM_T8H:
            WriteVramUnlocked(dpsm, dbp, dbw, m_transferState.x, m_transferState.y, data[offset++]);
            advancePixel(1u);
            break;
        case GS_PSM_T4:
        case GS_PSM_T4HL:
        case GS_PSM_T4HH:
        {
            const uint8_t packed = data[offset++];
            const uint32_t firstPixel = m_transferState.copiedPixels;
            WriteVramUnlocked(dpsm, dbp, dbw,
                              dsax + (firstPixel % rrw),
                              m_transfer.trxpos.dsay + (firstPixel / rrw),
                              packed & 0x0Fu);
            if (firstPixel + 1u < m_transferState.totalPixels)
            {
                const uint32_t secondPixel = firstPixel + 1u;
                WriteVramUnlocked(dpsm, dbp, dbw,
                                  dsax + (secondPixel % rrw),
                                  m_transfer.trxpos.dsay + (secondPixel / rrw),
                                  (packed >> 4u) & 0x0Fu);
            }
            advancePixel(std::min<uint32_t>(2u, m_transferState.totalPixels - firstPixel));
            break;
        }
        default:
            return;
        }
    }
}

void GSCpuBackend::PerformLocalToLocalTransfer()
{
    if (!m_vram)
        return;

    const uint32_t rrw = m_transfer.trxreg.rrw;
    const uint32_t rrh = m_transfer.trxreg.rrh;
    const uint32_t total = rrw * rrh;
    if (total == 0u)
    {
        m_transferState.direction = 3u;
        return;
    }

    for (uint32_t pixel = 0; pixel < total; ++pixel)
    {
        uint32_t x = pixel % rrw;
        uint32_t y = pixel / rrw;
        if ((m_transfer.trxpos.dir & 0x2u) != 0u)
            x = rrw - x - 1u;
        if ((m_transfer.trxpos.dir & 0x1u) != 0u)
            y = rrh - y - 1u;

        const uint32_t value = ReadVramUnlocked(m_transfer.bitbltbuf.spsm,
                                                m_transfer.bitbltbuf.sbp,
                                                std::max<uint32_t>(m_transfer.bitbltbuf.sbw, 1u),
                                                x + m_transfer.trxpos.ssax,
                                                y + m_transfer.trxpos.ssay);
        WriteVramUnlocked(m_transfer.bitbltbuf.dpsm,
                          m_transfer.bitbltbuf.dbp,
                          std::max<uint32_t>(m_transfer.bitbltbuf.dbw, 1u),
                          x + m_transfer.trxpos.dsax,
                          y + m_transfer.trxpos.dsay,
                          value);
    }

    m_transferState.copiedPixels = total;
    m_transferState.direction = 3u;
}

void GSCpuBackend::PerformLocalToHostTransfer()
{
    m_localToHostBuffer.clear();
    m_localToHostReadPos = 0u;
    if (!m_vram)
        return;

    const uint32_t rrw = m_transfer.trxreg.rrw;
    const uint32_t rrh = m_transfer.trxreg.rrh;
    const uint32_t sbw = std::max<uint32_t>(m_transfer.bitbltbuf.sbw, 1u);
    const uint8_t spsm = m_transfer.bitbltbuf.spsm;
    const uint32_t bpp = static_cast<uint32_t>(GSMem::BitsPerPixel(static_cast<GSMem::PixelStorageMode>(spsm)));
    const uint32_t total = rrw * rrh;
    m_localToHostBuffer.reserve((static_cast<size_t>(total) * bpp + 7u) / 8u);

    for (uint32_t pixel = 0u; pixel < total; ++pixel)
    {
        const uint32_t x = pixel % rrw;
        const uint32_t y = pixel / rrw;
        const uint32_t value = ReadVramUnlocked(spsm,
                                                m_transfer.bitbltbuf.sbp,
                                                sbw,
                                                x + m_transfer.trxpos.ssax,
                                                y + m_transfer.trxpos.ssay);
        switch (bpp)
        {
        case 32:
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value));
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value >> 8u));
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value >> 16u));
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value >> 24u));
            break;
        case 24:
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value));
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value >> 8u));
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value >> 16u));
            break;
        case 16:
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value));
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value >> 8u));
            break;
        case 8:
            m_localToHostBuffer.push_back(static_cast<uint8_t>(value));
            break;
        case 4:
        {
            if ((pixel & 1u) != 0u)
                break;
            uint32_t next = 0u;
            if (pixel + 1u < total)
            {
                const uint32_t nextPixel = pixel + 1u;
                const uint32_t nextX = nextPixel % rrw;
                const uint32_t nextY = nextPixel / rrw;
                next = ReadVramUnlocked(spsm, m_transfer.bitbltbuf.sbp, sbw,
                                        nextX + m_transfer.trxpos.ssax,
                                        nextY + m_transfer.trxpos.ssay);
            }
            m_localToHostBuffer.push_back(static_cast<uint8_t>((value & 0x0Fu) | ((next & 0x0Fu) << 4u)));
            break;
        }
        default:
            break;
        }
    }

    m_transferState.copiedPixels = total;
    m_transferState.localToHostPendingBytes = m_localToHostBuffer.size();
}

uint32_t GSCpuBackend::ConsumeLocalToHostBytes(uint8_t *dst, uint32_t maxBytes)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    flushPendingUnlocked("ConsumeLocalToHostBytes");
    if (!dst || maxBytes == 0u || m_localToHostReadPos >= m_localToHostBuffer.size())
        return 0u;
    const size_t count = std::min<size_t>(maxBytes, m_localToHostBuffer.size() - m_localToHostReadPos);
    std::memcpy(dst, m_localToHostBuffer.data() + m_localToHostReadPos, count);
    m_localToHostReadPos += count;
    m_transferState.localToHostPendingBytes = m_localToHostBuffer.size() - m_localToHostReadPos;
    return static_cast<uint32_t>(count);
}

bool GSCpuBackend::ClearFramebuffer(const GSContext &context, uint32_t rgba)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    flushPendingUnlocked("ClearFramebuffer");
    if (!m_vram || context.frame.fbw == 0u)
        return false;

    const uint32_t x0 = context.scissor.x0;
    const uint32_t x1 = std::max<uint32_t>(x0, context.scissor.x1);
    const uint32_t y0 = context.scissor.y0;
    const uint32_t y1 = std::max<uint32_t>(y0, context.scissor.y1);
    uint8_t r = static_cast<uint8_t>(rgba);
    uint8_t g = static_cast<uint8_t>(rgba >> 8u);
    uint8_t b = static_cast<uint8_t>(rgba >> 16u);
    uint8_t a = static_cast<uint8_t>(rgba >> 24u);
    if ((context.fba & 1ull) != 0ull && context.frame.psm != GS_PSM_CT24)
        a |= 0x80u;
    {
        uint64_t begin = 0, end = 0;
        bufferRange(context.frame.fbp * 8192u, std::max<uint32_t>(context.frame.fbw, 1u) * 64u, y1 + 1u, context.frame.psm, begin, end);
        markVramWritten(begin, end);
    }

    const uint32_t fbp = GSInternal::framePageBaseToBlock(context.frame.fbp);
    const uint32_t fbw = std::max<uint32_t>(context.frame.fbw, 1u);
    if (context.frame.psm == GS_PSM_CT32 || context.frame.psm == GS_PSM_CT24)
    {
        const uint32_t source = static_cast<uint32_t>(r) |
                                (static_cast<uint32_t>(g) << 8u) |
                                (static_cast<uint32_t>(b) << 16u) |
                                (static_cast<uint32_t>(a) << 24u);
        for (uint32_t y = y0; y <= y1; ++y)
            for (uint32_t x = x0; x <= x1; ++x)
            {
                uint32_t pixel = source;
                if (context.frame.fbmsk != 0u)
                {
                    const uint32_t old = ReadVramUnlocked(context.frame.psm, fbp, fbw, x, y);
                    pixel = (pixel & ~context.frame.fbmsk) | (old & context.frame.fbmsk);
                }
                WriteVramUnlocked(context.frame.psm, fbp, fbw, x, y, pixel);
            }
        return true;
    }

    if (context.frame.psm == GS_PSM_CT16 || context.frame.psm == GS_PSM_CT16S)
    {
        const uint16_t source = encodeFramePixelPSMCT16(r, g, b, a);
        const uint16_t mask = static_cast<uint16_t>(context.frame.fbmsk);
        for (uint32_t y = y0; y <= y1; ++y)
            for (uint32_t x = x0; x <= x1; ++x)
            {
                uint16_t pixel = source;
                if (mask != 0u)
                {
                    const uint16_t old = static_cast<uint16_t>(ReadVramUnlocked(context.frame.psm, fbp, fbw, x, y));
                    pixel = static_cast<uint16_t>((pixel & ~mask) | (old & mask));
                }
                WriteVramUnlocked(context.frame.psm, fbp, fbw, x, y, pixel);
            }
        return true;
    }
    return false;
}

bool GSCpuBackend::CopyFrameToHostRgba(const GSFrameReg &frame,
                                       uint32_t width,
                                       uint32_t height,
                                       std::vector<uint8_t> &outPixels,
                                       bool preserveAlpha,
                                       bool useLocalMemoryLayout,
                                       bool frameBaseIsPages,
                                       uint32_t sourceOriginX,
                                       uint32_t sourceOriginY) const
{
    if (!m_vram || m_vramSize == 0u)
        return false;

    outPixels.assign(kHostFrameWidth * kHostFrameHeight * 4u, 0u);
    const uint32_t baseBytes = frameBaseIsPages ? frame.fbp * 8192u : frame.fbp * 256u;
    const uint32_t basePtr = frameBaseIsPages ? GSInternal::framePageBaseToBlock(frame.fbp) : frame.fbp;
    const uint32_t fbw = frame.fbw ? frame.fbw : kHostFrameWidth / 64u;
    const uint32_t bytesPerPixel = (frame.psm == GS_PSM_CT16 || frame.psm == GS_PSM_CT16S) ? 2u : 4u;
    const uint32_t stride = fbw * 64u * bytesPerPixel;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t *dst = outPixels.data() + y * kHostFrameWidth * 4u;
        for (uint32_t x = 0; x < width; ++x)
        {
            const uint32_t sx = sourceOriginX + x;
            const uint32_t sy = sourceOriginY + y;
            if (frame.psm == GS_PSM_CT32 || frame.psm == GS_PSM_CT24)
            {
                uint32_t color = 0u;
                if (useLocalMemoryLayout)
                    color = ReadVramUnlocked(frame.psm, basePtr, fbw, sx, sy);
                else
                {
                    const uint32_t pixelBytes = frame.psm == GS_PSM_CT24 ? 3u : 4u;
                    const uint64_t offset = static_cast<uint64_t>(baseBytes) + static_cast<uint64_t>(sy) * stride + static_cast<uint64_t>(sx) * pixelBytes;
                    if (offset + pixelBytes > m_vramSize)
                        return false;
                    color = m_vram[offset] | (static_cast<uint32_t>(m_vram[offset + 1u]) << 8u) |
                            (static_cast<uint32_t>(m_vram[offset + 2u]) << 16u);
                    if (pixelBytes == 4u)
                        color |= static_cast<uint32_t>(m_vram[offset + 3u]) << 24u;
                }
                dst[x * 4u] = static_cast<uint8_t>(color);
                dst[x * 4u + 1u] = static_cast<uint8_t>(color >> 8u);
                dst[x * 4u + 2u] = static_cast<uint8_t>(color >> 16u);
                dst[x * 4u + 3u] = preserveAlpha && frame.psm != GS_PSM_CT24 ? static_cast<uint8_t>(color >> 24u) : 255u;
            }
            else if (frame.psm == GS_PSM_CT16 || frame.psm == GS_PSM_CT16S)
            {
                uint16_t color = 0u;
                if (useLocalMemoryLayout)
                    color = static_cast<uint16_t>(ReadVramUnlocked(frame.psm, basePtr, fbw, sx, sy));
                else
                {
                    const uint64_t offset = static_cast<uint64_t>(baseBytes) + static_cast<uint64_t>(sy) * stride + static_cast<uint64_t>(sx) * 2u;
                    if (offset + 2u > m_vramSize)
                        return false;
                    std::memcpy(&color, m_vram + offset, sizeof(color));
                }
                const uint32_t r = color & 31u;
                const uint32_t g = (color >> 5u) & 31u;
                const uint32_t b = (color >> 10u) & 31u;
                dst[x * 4u] = static_cast<uint8_t>((r << 3u) | (r >> 2u));
                dst[x * 4u + 1u] = static_cast<uint8_t>((g << 3u) | (g >> 2u));
                dst[x * 4u + 2u] = static_cast<uint8_t>((b << 3u) | (b >> 2u));
                dst[x * 4u + 3u] = preserveAlpha ? ((color & 0x8000u) ? 0x80u : 0u) : 255u;
            }
            else
            {
                outPixels.clear();
                return false;
            }
        }
    }
    return true;
}

PresentationFrame GSCpuBackend::Present(const GSPresentationRequest &request)
{
    // The native renderer's picture is the one shown: no need to read the
    // display buffer out of local memory, only its size and place.
    if (native_renderer::shown())
    {
#if defined(__vita__)
        // The Vita puts the native picture on screen itself (its present
        // hook); it keeps no CPU copy (latestFrame), so this went on to read
        // and convert the whole display buffer for nothing (round 19).
        return {};
#endif
        const GSPmodeState pmode = decodePmode(request.pmode);
        const bool first = pmode.enableCrt1;
        const uint64_t dispfb = first ? request.dispfb1 : request.dispfb2, display = first ? request.display1 : request.display2;
        uint32_t width = 0u, height = 0u;
        decodeDisplaySize(display, width, height);
        thread_local std::vector<uint8_t> native;
        uint32_t nw = 0, nh = 0;
        if ((pmode.enableCrt1 || pmode.enableCrt2) && width && height && width <= kHostFrameWidth && height <= kHostFrameHeight &&
            native_renderer::latestFrame(native, nw, nh) && nw && nh)
        {
            PresentationFrame frame;
            frame.width = width;
            frame.height = height;
            frame.displayFbp = decodeDisplayFrame(dispfb).fbp;
            frame.sourceFbp = frame.displayFbp;
            frame.vsyncTick = request.vsyncTick;
            frame.pixels.assign(static_cast<size_t>(kHostFrameWidth) * kHostFrameHeight * 4u, 0u);
            for (uint32_t y = 0; y < height; ++y)
            {
                const uint32_t sy = y * nh / height;
                for (uint32_t x = 0; x < width; ++x)
                {
                    const uint32_t sx = x * nw / width;
                    std::memcpy(&frame.pixels[(static_cast<size_t>(y) * kHostFrameWidth + x) * 4u], &native[(static_cast<size_t>(sy) * nw + sx) * 4u], 4);
                    frame.pixels[(static_cast<size_t>(y) * kHostFrameWidth + x) * 4u + 3u] = 255u; // colour only, as the PS2's CRTC shows it
                }
            }
            return frame;
        }
    }
    if (request.skipIfUnchanged)
    {
        // The display memory's last write plus the display setup identify
        // the frame: unchanged means the game has not finished a new one.
        std::lock_guard<std::mutex> lock(m_mutex);
        uint64_t newest = 0;
        const std::pair<uint64_t, uint64_t> outputs[2] = {{request.dispfb1, request.display1}, {request.dispfb2, request.display2}};
        for (const auto &[dispfb, display] : outputs)
        {
            const GSFrameReg frame = decodeDisplayFrame(dispfb);
            const GSDisplayReadOrigin origin = decodeDisplayReadOrigin(dispfb);
            uint32_t width = 0, height = 0;
            decodeDisplaySize(display, width, height);
            if (height == 0u)
                continue;
            uint64_t begin = 0, end = 0;
            bufferRange(frame.fbp * 8192u, std::max<uint32_t>(1u, frame.fbw) * 64u, origin.y + height, frame.psm, begin, end);
            const uint64_t firstPage = begin / 8192u;
            const uint64_t lastPage = std::min<uint64_t>((end + 8191u) / 8192u, firstPage + m_pageEpoch.size());
            for (uint64_t page = firstPage; page < lastPage; ++page)
                newest = std::max(newest, m_pageEpoch[page % m_pageEpoch.size()]);
        }
        const std::array<uint64_t, 6> key{request.pmode, request.dispfb1, request.display1,
                                          request.dispfb2, request.display2, newest};
        // The native picture changes without the GS drawing (it may draw nothing).
        if (key == m_lastPresentKey && !native_renderer::shown())
            return {};
        m_lastPresentKey = key;
    }
    // Snapshot local memory under the backend lock, then perform the expensive
    // display conversion without holding the producer-side raster lock.
    thread_local std::vector<uint8_t> snapshot;
    SnapshotVram(snapshot);
    if (snapshot.empty())
        return {};

    thread_local GSCpuBackend snapshotBackend;
    snapshotBackend.Initialize(snapshot.data(), static_cast<uint32_t>(snapshot.size()));
    PresentationFrame frame = snapshotBackend.PresentFromLocalMemory(request);
    // A game override drawing natively (runtime/native_renderer.h) shows its picture instead.
    if (frame && native_renderer::shown())
    {
        // Scaled to the display output, in the presentation's layout (rows
        // of kHostFrameWidth pixels).
        thread_local std::vector<uint8_t> native;
        uint32_t width = 0, height = 0;
        if (native_renderer::latestFrame(native, width, height) && width && height)
        {
            constexpr uint32_t kStride = 640u;
            for (uint32_t y = 0; y < frame.height; ++y)
            {
                const uint32_t sy = y * height / frame.height;
                for (uint32_t x = 0; x < frame.width && x < kStride; ++x)
                {
                    const size_t dst = (static_cast<size_t>(y) * kStride + x) * 4u;
                    if (dst + 4u > frame.pixels.size())
                        break;
                    const uint32_t sx = x * width / frame.width;
                    std::memcpy(&frame.pixels[dst], &native[(static_cast<size_t>(sy) * width + sx) * 4u], 4);
                    frame.pixels[dst + 3u] = 255u; // the display shows colour only (the PS2's CRTC ignores alpha)
                }
            }
        }
    }
    return frame;
}

PresentationFrame GSCpuBackend::PresentFromLocalMemory(const GSPresentationRequest &request)
{
    PresentationFrame result{};
    const GSPmodeState pmode = decodePmode(request.pmode);
    const GSSmode2State smode2 = decodeSMode2(request.smode2);
    // In FIELD mode the GS scans out every other line of a full-height frame
    // per field. Showing a single field line-doubled halves the vertical
    // resolution, so the whole frame is shown (weave) unless PS2X_GS_BOB=1.
    static const bool bobFields = std::getenv("PS2X_GS_BOB") != nullptr;
    const bool fieldMode = bobFields && smode2.interlaced && !smode2.frameMode;
    const bool oddField = (request.vsyncTick & 1ull) != 0ull;
    const GSFrameReg displayFrame1 = decodeDisplayFrame(request.dispfb1);
    const GSFrameReg displayFrame2 = decodeDisplayFrame(request.dispfb2);
    const GSDisplayReadOrigin origin1 = decodeDisplayReadOrigin(request.dispfb1);
    const GSDisplayReadOrigin origin2 = decodeDisplayReadOrigin(request.dispfb2);
    uint32_t width1 = 0u, height1 = 0u, width2 = 0u, height2 = 0u;
    decodeDisplaySize(request.display1, width1, height1);
    decodeDisplaySize(request.display2, width2, height2);
    const bool valid1 = pmode.enableCrt1 && hasDisplaySetup(request.display1, displayFrame1);
    const bool valid2 = pmode.enableCrt2 && hasDisplaySetup(request.display2, displayFrame2);
    if (!valid1 && !valid2)
        return result;

    auto copySource = [&](const GSFrameReg &displayFrame,
                          const GSDisplayReadOrigin &origin,
                          uint32_t width,
                          uint32_t height,
                          bool allowPreferred,
                          bool preserveAlpha,
                          GSFrameReg &selected,
                          std::vector<uint8_t> &pixels,
                          bool &usedPreferred) -> bool
    {
        selected = displayFrame;
        pixels.clear();
        usedPreferred = false;
        if (allowPreferred && request.hasPreferredSource && request.preferredDestFbp == displayFrame.fbp &&
            (request.preferredSource.fbw != 0u || request.preferredSource.fbp != displayFrame.fbp) &&
            CopyFrameToHostRgba(request.preferredSource, width, height, pixels, preserveAlpha, true, false, 0u, 0u))
        {
            selected = request.preferredSource;
            usedPreferred = true;
        }
        if (pixels.empty() && !CopyFrameToHostRgba(displayFrame, width, height, pixels, preserveAlpha, true, true, origin.x, origin.y))
            return false;

        if (!usedPreferred && displayFrame.fbp == 0u && countNonBlackPixels(pixels, width, height) == 0u)
        {
            for (const GSFrameReg &candidate : request.contextFrames)
            {
                if (candidate.fbp == selected.fbp && candidate.fbw == selected.fbw && candidate.psm == selected.psm)
                    continue;
                std::vector<uint8_t> candidatePixels;
                if (!CopyFrameToHostRgba(candidate, width, height, candidatePixels, preserveAlpha, true, true, 0u, 0u))
                    continue;
                if (countNonBlackPixels(candidatePixels, width, height) == 0u)
                    continue;
                selected = candidate;
                pixels.swap(candidatePixels);
                break;
            }
        }
        return true;
    };

    if (valid1 && valid2)
    {
        GSFrameReg selected1{}, selected2{};
        std::vector<uint8_t> crt1, crt2;
        bool preferred1 = false, preferred2 = false;
        if (copySource(displayFrame1, origin1, width1, height1, false, true, selected1, crt1, preferred1) &&
            copySource(displayFrame2, origin2, width2, height2, false, true, selected2, crt2, preferred2))
        {
            result.width = std::max(width1, width2);
            result.height = std::max(height1, height2);
            result.pixels.assign(kHostFrameWidth * kHostFrameHeight * 4u, 0u);
            const uint8_t bgR = static_cast<uint8_t>(request.bgcolor);
            const uint8_t bgG = static_cast<uint8_t>(request.bgcolor >> 8u);
            const uint8_t bgB = static_cast<uint8_t>(request.bgcolor >> 16u);
            for (uint32_t y = 0; y < result.height; ++y)
                for (uint32_t x = 0; x < result.width; ++x)
                {
                    uint8_t *dst = result.pixels.data() + (y * kHostFrameWidth + x) * 4u;
                    dst[0] = bgR;
                    dst[1] = bgG;
                    dst[2] = bgB;
                    dst[3] = pmode.alp;
                }
            if (!pmode.slbg)
                for (uint32_t y = 0; y < height2; ++y)
                    std::memcpy(result.pixels.data() + y * kHostFrameWidth * 4u, crt2.data() + y * kHostFrameWidth * 4u, width2 * 4u);
            for (uint32_t y = 0; y < height1; ++y)
                for (uint32_t x = 0; x < width1; ++x)
                {
                    const uint8_t *src = crt1.data() + (y * kHostFrameWidth + x) * 4u;
                    uint8_t *dst = result.pixels.data() + (y * kHostFrameWidth + x) * 4u;
                    const uint32_t factor = pmode.mmod ? pmode.alp : std::min<uint32_t>(255u, static_cast<uint32_t>(src[3]) * 2u);
                    dst[0] = blendPresentationChannel(src[0], dst[0], factor);
                    dst[1] = blendPresentationChannel(src[1], dst[1], factor);
                    dst[2] = blendPresentationChannel(src[2], dst[2], factor);
                    dst[3] = pmode.amod ? dst[3] : src[3];
                }
            normalizePresentationAlpha(result.pixels, result.width, result.height);
            if (fieldMode)
                applyFieldPresentation(result.pixels, result.width, result.height, oddField);
            result.displayFbp = displayFrame1.fbp;
            result.sourceFbp = selected1.fbp;
            return result;
        }
    }

    const GSFrameReg &displayFrame = valid1 ? displayFrame1 : displayFrame2;
    const GSDisplayReadOrigin &origin = valid1 ? origin1 : origin2;
    result.width = valid1 ? width1 : width2;
    result.height = valid1 ? height1 : height2;
    GSFrameReg selected = displayFrame;
    if (!copySource(displayFrame, origin, result.width, result.height, true, false, selected, result.pixels, result.usedPreferred))
        return {};
    if (fieldMode)
        applyFieldPresentation(result.pixels, result.width, result.height, oddField);
    normalizePresentationAlpha(result.pixels, result.width, result.height);
    result.displayFbp = displayFrame.fbp;
    result.sourceFbp = selected.fbp;
    return result;
}
