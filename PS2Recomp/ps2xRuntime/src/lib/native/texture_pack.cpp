// Texture packs (worklist #21): see runtime/texture_pack.h.
#include "runtime/texture_pack.h"
#include "runtime/native_tuning.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <unordered_set>

#if !defined(__vita__)
#include <filesystem>
#include "raylib.h"
#endif

namespace texture_pack
{
    uint64_t textureId(const uint8_t *rgba, uint32_t width, uint32_t height)
    {
        // Eight bytes at a time (little-endian, as both hosts are), each mixed
        // in by a multiply and a shift; the tail a byte at a time; splitmix64's
        // finish. Not a cryptographic hash: two different pictures with the
        // same id are possible but not expected among a game's few thousand.
        uint64_t x = 0x9E3779B97F4A7C15ull ^ ((static_cast<uint64_t>(width) << 32) | height);
        const size_t n = static_cast<size_t>(width) * height * 4u;
        size_t i = 0;
        for (; i + 8u <= n; i += 8u)
        {
            uint64_t v;
            std::memcpy(&v, rgba + i, 8u);
            x ^= v;
            x *= 0xFF51AFD7ED558CCDull;
            x ^= x >> 32;
        }
        for (; i < n; ++i)
        {
            x ^= rgba[i];
            x *= 0x100000001B3ull;
        }
        x ^= x >> 30;
        x *= 0xBF58476D1CE4E5B9ull;
        x ^= x >> 27;
        x *= 0x94D049BB133111EBull;
        x ^= x >> 31;
        return x;
    }

    uint32_t buildMips(std::vector<uint8_t> &rgba, uint32_t width, uint32_t height, uint32_t levels)
    {
        if (!width || !height || levels == 0u)
            return 0u;
        size_t total = 0;
        uint32_t made = 0;
        for (uint32_t w = width, h = height; made < levels && w >= 1u && h >= 1u; ++made, w /= 2u, h /= 2u)
            total += static_cast<size_t>(w) * h * 4u;
        rgba.resize(total);
        size_t at = 0;
        for (uint32_t level = 1, w = width, h = height; level < made; ++level)
        {
            const uint32_t nw = w / 2u, nh = h / 2u;
            const uint8_t *src = &rgba[at];
            uint8_t *dst = &rgba[at + static_cast<size_t>(w) * h * 4u];
            for (uint32_t y = 0; y < nh; ++y)
                for (uint32_t x = 0; x < nw; ++x)
                    for (uint32_t c = 0; c < 4u; ++c)
                    {
                        const uint32_t sum = src[((2u * y) * w + 2u * x) * 4u + c] + src[((2u * y) * w + 2u * x + 1u) * 4u + c] +
                                             src[((2u * y + 1u) * w + 2u * x) * 4u + c] + src[((2u * y + 1u) * w + 2u * x + 1u) * 4u + c];
                        dst[(y * nw + x) * 4u + c] = static_cast<uint8_t>((sum + 2u) / 4u);
                    }
            at += static_cast<size_t>(w) * h * 4u;
            w = nw;
            h = nh;
        }
        return made;
    }

#if defined(__vita__)
    bool active() { return false; }
    bool apply(std::vector<uint8_t> &, uint32_t &, uint32_t &, uint32_t &) { return false; }
#else
    namespace
    {
        const char *kPackDir = "textures/pack";
        const char *kDumpDir = "textures/dump";

        // The pack's ids, from its folder's file names, read once.
        const std::unordered_set<uint64_t> &packIds()
        {
            static const std::unordered_set<uint64_t> ids = []
            {
                std::unordered_set<uint64_t> found;
                std::error_code error;
                for (const auto &entry : std::filesystem::directory_iterator(kPackDir, error))
                {
                    const std::string name = entry.path().filename().string();
                    if (name.size() != 20u || name.compare(16, 4, ".png") != 0 ||
                        name.find_first_not_of("0123456789abcdef") < 16u)
                        continue;
                    found.insert(std::stoull(name.substr(0, 16), nullptr, 16));
                }
                std::fprintf(stderr, "[texpack] %zu pictures in %s/\n", found.size(), kPackDir);
                return found;
            }();
            return ids;
        }

        std::string fileName(const char *dir, uint64_t id)
        {
            char name[64];
            std::snprintf(name, sizeof(name), "/%016llx.png", static_cast<unsigned long long>(id));
            return std::string(dir) + name;
        }

        // Level 0 as a PNG, once per id per run (and not over a file there).
        void dump(uint64_t id, const uint8_t *rgba, uint32_t width, uint32_t height)
        {
            static std::mutex lock;
            static std::unordered_set<uint64_t> written;
            {
                std::lock_guard<std::mutex> hold(lock);
                if (!written.insert(id).second)
                    return;
                static const bool made = []
                {
                    std::error_code error;
                    std::filesystem::create_directories(kDumpDir, error);
                    return !error;
                }();
                if (!made)
                    return;
            }
            const std::string path = fileName(kDumpDir, id);
            std::error_code error;
            if (std::filesystem::exists(path, error))
                return;
            const size_t n = static_cast<size_t>(width) * height * 4u;
            std::vector<uint8_t> png(rgba, rgba + n);
            bool bright = false;
            for (size_t i = 3; i < n; i += 4u)
            {
                bright |= png[i] > 0x80u;
                png[i] = alphaToPng(png[i]);
            }
            Image image{png.data(), static_cast<int>(width), static_cast<int>(height), 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
            if (!ExportImage(image, path.c_str()))
                std::fprintf(stderr, "[texpack] could not write %s\n", path.c_str());
            else if (bright)
                std::fprintf(stderr, "[texpack] %016llx has alpha above 0x80: a picture made from its dump comes back with 0x80 there\n",
                             static_cast<unsigned long long>(id));
        }

        // The pack's picture for `id` over the game's, or false (none, or
        // unusable: logged once).
        bool replace(uint64_t id, std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height, uint32_t &levels)
        {
            if (!packIds().count(id))
                return false;
            const std::string path = fileName(kPackDir, id);
            Image image = LoadImage(path.c_str());
            if (!image.data)
            {
                std::fprintf(stderr, "[texpack] could not read %s\n", path.c_str());
                return false;
            }
            ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
            if (image.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
            {
                std::fprintf(stderr, "[texpack] %s: not an 8-bit RGB / RGBA picture (format %d)\n", path.c_str(), image.format);
                UnloadImage(image);
                return false;
            }
            const uint32_t w = static_cast<uint32_t>(image.width), h = static_cast<uint32_t>(image.height);
            // The same shape, the game's size times a power of two (1 to 8).
            uint32_t scale = 0;
            for (uint32_t s = 1; s <= 8u; s *= 2u)
                if (w == width * s && h == height * s)
                    scale = s;
            if (!scale)
            {
                std::fprintf(stderr, "[texpack] %s is %ux%u: the game's texture is %ux%u, so it can be %ux%u, %ux%u, %ux%u or %ux%u\n", path.c_str(), w, h,
                             width, height, width, height, width * 2u, height * 2u, width * 4u, height * 4u, width * 8u, height * 8u);
                UnloadImage(image);
                return false;
            }
            const size_t n = static_cast<size_t>(w) * h * 4u;
            std::vector<uint8_t> picture(static_cast<const uint8_t *>(image.data), static_cast<const uint8_t *>(image.data) + n);
            UnloadImage(image);
            for (size_t i = 3; i < n; i += 4u)
                picture[i] = alphaFromPng(picture[i]);
            // A dump copied in as it was: the game's own levels kept.
            if (scale == 1u && std::equal(picture.begin(), picture.end(), rgba.begin()))
                return false;
            uint32_t extra = 0;
            for (uint32_t s = scale; s > 1u; s /= 2u)
                ++extra;
            const uint32_t made = buildMips(picture, w, h, std::max(levels, 1u) + extra);
            rgba.swap(picture);
            width = w;
            height = h;
            levels = made;
            return true;
        }
    }

    bool active()
    {
        const native_renderer::Tuning &t = native_renderer::tuning();
        return t.texturePack || t.textureDump;
    }

    bool apply(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height, uint32_t &levels)
    {
        if (!active() || !width || !height || rgba.size() < static_cast<size_t>(width) * height * 4u)
            return false;
        const native_renderer::Tuning &t = native_renderer::tuning();
        const uint64_t id = textureId(rgba.data(), width, height);
        if (t.textureDump)
            dump(id, rgba.data(), width, height);
        return t.texturePack && replace(id, rgba, width, height, levels);
    }
#endif
}
