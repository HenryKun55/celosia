// raylib subset for the PS Vita: video on vitaGL (GXM), audio through SDL2,
// controls from the Vita system libraries. See raylib.h.

#include "raylib.h"

#include <SDL2/SDL.h>
#include <vitaGL.h>
#include <psp2/ctrl.h>
#include <psp2/appmgr.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/power.h>
#include <psp2/touch.h>

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <map>
#include <mutex>
#include <vector>

// vitaGL is one GL state for the whole process: the host (between
// BeginDrawing and EndDrawing, and in texture calls) and the GPU GS renderer
// on its own thread take turns under this lock.
std::recursive_mutex &ps2xVitaGlMutex()
{
    static std::recursive_mutex mutex;
    return mutex;
}

// The boot's timeline (as ps2_stats::bootMark: the process clock).
static void bootMark(const char *what)
{
    std::fprintf(stderr, "[boot] %.3f s %s\n", static_cast<double>(sceKernelGetProcessTimeWide()) / 1e6, what);
}

// vitaGL's own steps inside vglInit (our patch calls it; round 20: the
// console's first r19 boot spent 51 s between "SDL up" and "vitaGL up").
extern "C" void vglBootMark(const char *what)
{
    bootMark(what);
}

// The shader cache's seed (round 19): vitaGL (HAVE_SHADER_CACHE) keeps each
// compiled shader in ux0:data/shader_cache/<title id>/v2/{v,f}/XX/<hash>.gxp
// and loads it instead of compiling the same source again. The VPK can carry
// such a cache (app0:shader_cache/{v,f}/XX/..., copied from a Vita3K run's
// tree, PS2X_VITA_SHADER_SEED): its files are copied where missing, so that
// the first boot on a console skips libshacccg too (~20 s of the boot in
// calibrated Vita3K). A source changed since the seed was made has another
// hash: compiled and cached as before.
// vitaGL's own root reads the title id into 12 bytes without room for its
// end (Vita3K: "CELO00001n"): the root is given here instead.
static char g_shaderCacheRoot[128];
static void shaderCacheRoot()
{
    char titleId[256] = {};
    if (sceAppMgrAppParamGetString(0, 12, titleId, sizeof(titleId)) < 0 || !titleId[0])
        std::strcpy(titleId, "CELO00001");
    titleId[9] = '\0'; // title ids are 9 characters
    std::snprintf(g_shaderCacheRoot, sizeof(g_shaderCacheRoot), "ux0:data/shader_cache/%s", titleId);
    vglSetShaderCachePath(g_shaderCacheRoot);
}
static void seedShaderCache()
{
    char root[160];
    std::snprintf(root, sizeof(root), "%s/v2", g_shaderCacheRoot);
    unsigned copied = 0, kept = 0;
    std::vector<char> buffer;
    for (const char *kind : {"v", "f"})
    {
        char from[160];
        std::snprintf(from, sizeof(from), "app0:shader_cache/%s", kind);
        const SceUID dirs = sceIoDopen(from);
        if (dirs < 0)
            continue;
        SceIoDirent sub{};
        while (sceIoDread(dirs, &sub) > 0)
        {
            if (sub.d_name[0] == '.')
                continue;
            char subFrom[256];
            std::snprintf(subFrom, sizeof(subFrom), "%s/%s", from, sub.d_name);
            const SceUID files = sceIoDopen(subFrom);
            if (files < 0)
                continue;
            SceIoDirent file{};
            while (sceIoDread(files, &file) > 0)
            {
                if (file.d_name[0] == '.')
                    continue;
                char src[320], dst[320];
                std::snprintf(src, sizeof(src), "%s/%s", subFrom, file.d_name);
                std::snprintf(dst, sizeof(dst), "%s/%s/%s/%s", root, kind, sub.d_name, file.d_name);
                SceIoStat st{};
                if (sceIoGetstat(dst, &st) >= 0)
                {
                    ++kept;
                    continue;
                }
                const SceUID in = sceIoOpen(src, SCE_O_RDONLY, 0);
                if (in < 0)
                    continue;
                const SceOff size = sceIoLseek(in, 0, SCE_SEEK_END);
                sceIoLseek(in, 0, SCE_SEEK_SET);
                buffer.resize(static_cast<size_t>(size > 0 ? size : 0));
                const int got = size > 0 ? sceIoRead(in, buffer.data(), static_cast<SceSize>(size)) : 0;
                sceIoClose(in);
                if (got != size)
                    continue;
                // Written under another name and renamed: a boot cut short
                // leaves no half file for vitaGL to load.
                // The subfolder (vitaGL no longer makes all 512 at start).
                char dir[320];
                std::snprintf(dir, sizeof(dir), "%s/%s/%s", root, kind, sub.d_name);
                sceIoMkdir(dir, 0777);
                char part[330];
                std::snprintf(part, sizeof(part), "%s.part", dst);
                const SceUID out = sceIoOpen(part, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
                if (out < 0)
                    continue;
                const int put = sceIoWrite(out, buffer.data(), static_cast<SceSize>(size));
                sceIoClose(out);
                if (put == size && sceIoRename(part, dst) >= 0)
                    ++copied;
                else
                    sceIoRemove(part);
            }
            sceIoDclose(files);
        }
        sceIoDclose(dirs);
    }
    if (copied || kept)
        std::fprintf(stderr, "[vita] shader cache seed: %u copied, %u already there\n", copied, kept);
}

namespace
{
    std::atomic<uint32_t> g_hostFrames{0};
}

// Bumped when the host has drawn (and changed GL state): the GS renderer
// then sets its state again.
uint32_t ps2xVitaHostFrames() { return g_hostFrames.load(std::memory_order_relaxed); }

namespace
{
    bool g_glReady = false;
    bool g_quit = false;
    int g_width = 960, g_height = 544;
    uint64_t g_startCounter = 0;

    struct TextureSlot
    {
        GLuint texture = 0;
        std::vector<uint8_t> pixels; // last upload, for LoadImageFromTexture
    };
    std::map<unsigned int, TextureSlot> g_textures;
    unsigned int g_nextTexture = 1;

    // FPS over the last second of EndDrawing() calls.
    int g_fps = 0;
    int g_framesThisSecond = 0;
    double g_fpsWindowStart = 0.0;

    // Controls: ports 0 (Vita / PS TV virtual) and 1 (PS TV first DS3/DS4) merged.
    SceCtrlData g_pad{};
    bool g_rearLeft = false, g_rearRight = false;

    // Audio
    SDL_AudioDeviceID g_audioDevice = 0;
    int g_audioBufferFrames = 1024;
    std::atomic<AudioCallback> g_audioCallback{nullptr};

    void pollInput()
    {
        SceCtrlData merged{};
        merged.lx = merged.ly = merged.rx = merged.ry = 128;
        for (int port = 0; port <= 1; ++port)
        {
            SceCtrlData data{};
            if (sceCtrlPeekBufferPositiveExt2(port, &data, 1) < 0)
                continue;
            merged.buttons |= data.buttons;
            auto pick = [](unsigned char current, unsigned char value)
            {
                return std::abs(int(value) - 128) > std::abs(int(current) - 128) ? value : current;
            };
            merged.lx = pick(merged.lx, data.lx);
            merged.ly = pick(merged.ly, data.ly);
            merged.rx = pick(merged.rx, data.rx);
            merged.ry = pick(merged.ry, data.ry);
        }
        g_pad = merged;

        // Vita 1000/2000 have no L2/R2: the rear touchpad's left and right
        // halves stand in for them.
        SceTouchData touch{};
        g_rearLeft = g_rearRight = false;
        if (sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch, 1) > 0)
        {
            for (unsigned i = 0; i < touch.reportNum; ++i)
            {
                if (touch.report[i].x < 960)
                    g_rearLeft = true;
                else
                    g_rearRight = true;
            }
        }
    }

    void audioThunk(void *, Uint8 *stream, int bytes)
    {
        const unsigned int frames = static_cast<unsigned int>(bytes) / 4u; // s16 stereo
        if (AudioCallback callback = g_audioCallback.load(std::memory_order_acquire))
            callback(stream, frames);
        else
            std::memset(stream, 0, static_cast<size_t>(bytes));
    }
}

void InitWindow(int, int, const char *)
{
    // The Vita screen, whatever size the host asked for.
    const int width = 960, height = 544;
    g_width = width;
    g_height = height;
    // The highest clocks the Vita offers to games (the defaults are 333 MHz
    // CPU, 111 MHz GPU): the emulated EE, the hooks and vitaGL's draw calls
    // are CPU-bound, the GPU draws the picture several times a frame.
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
    std::fprintf(stderr, "[vita] clocks: cpu %d MHz, bus %d, gpu %d, xbar %d\n", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency(),
                 scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
    bootMark("window: clocks set");
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
        std::fprintf(stderr, "[vita] SDL_Init failed: %s\n", SDL_GetError());
    bootMark("window: SDL up");
    // Shaders are compiled at run time (libshacccg); fast math is fine here.
    vglSetupRuntimeShaderCompiler(SHARK_OPT_FAST, 1, 1, 1);
    bootMark("window: shader compiler set up");
    // Some framebuffers get several scenes a frame (the small copies of the
    // picture, the bloom's): GXM sizes a render target for this many scenes
    // a frame (vitaGL's default for framebuffers: 1).
    vglSetupRenderTargetScenesNum(1, 8);
    // vitaGL takes the free main RAM minus the threshold; the runtime's
    // threads (game, VU1, GS, GIF, audio) get their stacks from that RAM too,
    // so leave it plenty. GPU memory (CDRAM) is all vitaGL's.
    // Returns whether it had to fall back to a smaller resolution, not success.
    shaderCacheRoot();
    {
        SceKernelFreeMemorySizeInfo info{};
        info.size = sizeof(info);
        sceKernelGetFreeMemorySize(&info);
        std::fprintf(stderr, "[vita] free memory before vitaGL: user %.1f MB, cdram %.1f MB, phycont %.1f MB\n", info.size_user / 1048576.0,
                     info.size_cdram / 1048576.0, info.size_phycont / 1048576.0);
    }
    bootMark("window: shader cache root");
    // Round 34, SOTC_VGL_POOLS=console (testing in Vita3K, off by default):
    // vitaGL's pools as a console has them (round 32's console log: CDRAM
    // 96 MB, main RAM 11 MB; free before vitaGL: user 112, phycont 26 MB),
    // where Vita3K's free memory gives a ~100 MB main-RAM pool and the newlib
    // heap never takes GPU objects.
    const char *pools = std::getenv("SOTC_VGL_POOLS");
    if (pools && std::strcmp(pools, "console") == 0)
    {
        std::fprintf(stderr, "[vita] SOTC_VGL_POOLS=console: vitaGL pools of RAM 16, CDRAM 96, phycont 26 MB (a console's)\n");
        vglInitWithCustomSizes(4 * 1024 * 1024, width, height, 16 * 1024 * 1024, 96 * 1024 * 1024, 26 * 1024 * 1024, 0, SCE_GXM_MULTISAMPLE_NONE);
    }
    else
        vglInitWithCustomThreshold(4 * 1024 * 1024, width, height, 96 * 1024 * 1024, 0, 0, 0x8C6000 /* the common dialog share, as vglInitExtended */,
                                   SCE_GXM_MULTISAMPLE_NONE);
    bootMark("window: vitaGL up");
    seedShaderCache(); // vitaGL made the cache's folders
    bootMark("window: shader cache seeded");
    g_glReady = true;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sceCtrlSetSamplingModeExt(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_START);
    g_startCounter = SDL_GetPerformanceCounter();
}

void CloseWindow(void)
{
    for (auto &[id, slot] : g_textures)
        if (slot.texture)
            glDeleteTextures(1, &slot.texture);
    g_textures.clear();
    g_glReady = false; // vitaGL has no shutdown: the process ends next
    SDL_Quit();
}

bool WindowShouldClose(void) { return g_quit; }
bool IsWindowReady(void) { return g_glReady; }
void SetConfigFlags(unsigned int) {}
// Presentation follows the display's vsync: 30 or less shows each picture
// for at least two vblanks (interval 2), else every vblank.
void SetTargetFPS(int fps) { eglSwapInterval(nullptr, fps > 0 && fps <= 30 ? 2 : 1); }
int GetFPS(void) { return g_fps; }

double GetTime(void)
{
    if (g_startCounter == 0)
        g_startCounter = SDL_GetPerformanceCounter();
    return static_cast<double>(SDL_GetPerformanceCounter() - g_startCounter) /
           static_cast<double>(SDL_GetPerformanceFrequency());
}

int GetScreenWidth(void) { return g_width; }
int GetScreenHeight(void) { return g_height; }

void PollInputEvents(void) { pollInput(); }

void BeginDrawing(void)
{
    pollInput();
    ps2xVitaGlMutex().lock(); // released by EndDrawing
    g_hostFrames.fetch_add(1, std::memory_order_relaxed);
    // The GS renderer has its own GL state set: back to the host's.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glDepthRangef(0.0f, 1.0f);
    glDepthMask(GL_TRUE);
    glViewport(0, 0, g_width, g_height);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

extern "C" void vglCeloMemStats(size_t *used, size_t *spillBytes, size_t *spillCount, size_t *spillTotal, size_t *spillPeak); // celosia.patch (round 34)

void EndDrawing(void)
{
    if (g_glReady)
        vglSwapBuffers(GL_FALSE);
    static bool firstShown = false;
    if (!firstShown)
    {
        firstShown = true;
        bootMark("first picture on screen");
    }
    ps2xVitaGlMutex().unlock();
    // Logs go to files on ux0: keep them current in case the app is killed.
    std::fflush(stdout);
    std::fflush(stderr);
    ++g_framesThisSecond;
    const double now = GetTime();
    static double nextMemLog = 0.0;
    if (now >= nextMemLog)
    {
        nextMemLog = now + 5.0;
        const struct mallinfo mi = mallinfo();
        // What the system has left for this app (code size and the heap
        // reservation come out of it): the headroom on a real Vita.
        SceKernelFreeMemorySizeInfo freeInfo{};
        freeInfo.size = sizeof(freeInfo);
        sceKernelGetFreeMemorySize(&freeInfo);
        // Round 34: what each vitaGL pool holds by its own count (celosia.patch:
        // Vita3K's mspace statistics are not real) and the GPU objects vitaGL
        // put in the newlib heap because its pools had no room.
        size_t used[VGL_MEM_ALL] = {}, spill = 0, spills = 0, spillTotal = 0, spillPeak = 0;
        vglCeloMemStats(used, &spill, &spills, &spillTotal, &spillPeak);
        const double MB = 1048576.0;
        std::fprintf(stderr, "[vita:mem] t=%.0f heap used=%.1f MB arena=%.1f MB | vgl vram=%.1f/%.1f ram=%.1f/%.1f MB | system free user=%.1f cdram=%.1f phycont=%.1f MB"
                             " | vgl used vram %.1f/%.1f ram %.1f/%.1f phycont %.1f/%.1f budget %.1f/%.1f MB, GPU objects in the heap %u (%.1f MB, peak %.1f, %.1f MB put there in all)\n",
                     now, mi.uordblks / MB, mi.arena / MB, vglMemFree(VGL_MEM_VRAM) / MB, vglMemTotal(VGL_MEM_VRAM) / MB, vglMemFree(VGL_MEM_RAM) / MB,
                     vglMemTotal(VGL_MEM_RAM) / MB, freeInfo.size_user / MB, freeInfo.size_cdram / MB, freeInfo.size_phycont / MB, used[VGL_MEM_VRAM] / MB,
                     vglMemTotal(VGL_MEM_VRAM) / MB, used[VGL_MEM_RAM] / MB, vglMemTotal(VGL_MEM_RAM) / MB, used[VGL_MEM_PHYCONT] / MB,
                     vglMemTotal(VGL_MEM_PHYCONT) / MB, used[VGL_MEM_BUDGET] / MB, vglMemTotal(VGL_MEM_BUDGET) / MB, static_cast<unsigned>(spills), spill / MB,
                     spillPeak / MB, spillTotal / MB);
    }
    if (now - g_fpsWindowStart >= 1.0)
    {
        g_fps = g_framesThisSecond;
        g_framesThisSecond = 0;
        g_fpsWindowStart = now;
    }
}

namespace
{
    // Immediate quads with client arrays (vitaGL's fixed-function path).
    void drawQuads(GLuint texture, const float *xy, const float *uv, int quads, Color color)
    {
        if (!g_glReady || quads <= 0)
            return;
        std::vector<float> pos, tex;
        pos.reserve(quads * 12);
        tex.reserve(quads * 12);
        for (int q = 0; q < quads; ++q)
        {
            static const int order[6] = {0, 1, 2, 0, 2, 3};
            for (int k : order)
            {
                pos.push_back(xy[q * 8 + k * 2]);
                pos.push_back(xy[q * 8 + k * 2 + 1]);
                if (uv)
                {
                    tex.push_back(uv[q * 8 + k * 2]);
                    tex.push_back(uv[q * 8 + k * 2 + 1]);
                }
            }
        }
        if (texture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
            glDisable(GL_TEXTURE_2D);
        glColor4ub(color.r, color.g, color.b, color.a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, pos.data());
        if (texture)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, tex.data());
        }
        glDrawArrays(GL_TRIANGLES, 0, quads * 6);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        if (texture)
            glDisable(GL_TEXTURE_2D);
    }

    void rectQuad(float *xy, float x, float y, float w, float h)
    {
        const float v[8] = {x, y, x + w, y, x + w, y + h, x, y + h};
        std::memcpy(xy, v, sizeof(v));
    }
}

void ClearBackground(Color color)
{
    if (!g_glReady)
        return;
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2, float, Color tint)
{
    // As raylib: a negative source height flips the picture vertically.
    if (source.height < 0.0f)
        source.y -= source.height;
    if (texture.id & 0x80000000u)
    {
        // A GL texture name (the GPU GS renderer's frame): opaque, drawn as is.
        if (texture.width <= 0 || texture.height <= 0)
            return;
        float xy[8], uv[8];
        rectQuad(xy, dest.x, dest.y, dest.width, dest.height);
        rectQuad(uv, source.x / texture.width, source.y / texture.height, source.width / texture.width, source.height / texture.height);
        glDisable(GL_BLEND);
        drawQuads(texture.id & 0x7FFFFFFFu, xy, uv, 1, tint);
        glEnable(GL_BLEND);
        return;
    }
    const auto it = g_textures.find(texture.id);
    if (it == g_textures.end() || !it->second.texture || texture.width <= 0 || texture.height <= 0)
        return;
    float xy[8], uv[8];
    rectQuad(xy, dest.x, dest.y, dest.width, dest.height);
    rectQuad(uv, source.x / texture.width, source.y / texture.height, source.width / texture.width, source.height / texture.height);
    drawQuads(it->second.texture, xy, uv, 1, tint);
}

void DrawRectangle(int posX, int posY, int width, int height, Color color)
{
    float xy[8];
    rectQuad(xy, static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(width), static_cast<float>(height));
    drawQuads(0, xy, nullptr, 1, color);
}

namespace
{
#include "default_font.inc"

    // raylib's default font as a 128x128 atlas, laid out the way LoadFontDefault does.
    struct GlyphRect
    {
        int x, y, w, h;
    };
    GLuint g_fontTexture = 0;
    GlyphRect g_glyphs[224];

    bool ensureFont()
    {
        if (g_fontTexture)
            return true;
        if (!g_glReady)
            return false;
        std::vector<uint32_t> pixels(128 * 128);
        for (int i = 0, counter = 0; i < 128 * 128; i += 32, ++counter)
            for (int j = 31; j >= 0; --j)
                pixels[i + j] = (kDefaultFontBits[counter] & (1u << j)) ? 0xFFFFFFFFu : 0x00FFFFFFu;
        glGenTextures(1, &g_fontTexture);
        glBindTexture(GL_TEXTURE_2D, g_fontTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        constexpr int kHeight = 10, kDivisor = 1;
        int line = 0, posX = kDivisor, testX = kDivisor;
        for (int i = 0; i < 224; ++i)
        {
            const int w = kDefaultFontWidths[i];
            g_glyphs[i] = GlyphRect{posX, kDivisor + line * (kHeight + kDivisor), w, kHeight};
            testX += w + kDivisor;
            if (testX >= 128)
            {
                ++line;
                posX = 2 * kDivisor + w;
                testX = posX;
                g_glyphs[i].x = kDivisor;
                g_glyphs[i].y = kDivisor + line * (kHeight + kDivisor);
            }
            else
                posX = testX;
        }
        return true;
    }

    // UTF-8 -> code point (Latin-1 range is all the font has; others -> '?').
    int nextCodepoint(const char *&p)
    {
        const unsigned char c = static_cast<unsigned char>(*p++);
        if (c < 0x80)
            return c;
        if ((c & 0xE0) == 0xC0 && (static_cast<unsigned char>(*p) & 0xC0) == 0x80)
            return ((c & 0x1F) << 6) | (static_cast<unsigned char>(*p++) & 0x3F);
        while ((static_cast<unsigned char>(*p) & 0xC0) == 0x80)
            ++p;
        return '?';
    }

    int glyphIndex(int codepoint)
    {
        return (codepoint >= 32 && codepoint < 256) ? codepoint - 32 : '?' - 32;
    }
}

// raylib's DrawText: default font scaled to fontSize, fontSize / 10 spacing.
void DrawText(const char *text, int posX, int posY, int fontSize, Color color)
{
    if (!text || !ensureFont())
        return;
    fontSize = std::max(fontSize, 10);
    const float scale = fontSize / 10.0f;
    const int spacing = fontSize / 10;
    std::vector<float> xy, uv;
    float x = static_cast<float>(posX);
    int y = posY;
    for (const char *p = text; *p;)
    {
        const int cp = nextCodepoint(p);
        if (cp == '\n')
        {
            x = static_cast<float>(posX);
            y += fontSize + fontSize / 2;
            continue;
        }
        const GlyphRect &src = g_glyphs[glyphIndex(cp)];
        if (cp != ' ')
        {
            float q[8], t[8];
            rectQuad(q, x, static_cast<float>(y), src.w * scale, static_cast<float>(fontSize));
            rectQuad(t, src.x / 128.0f, src.y / 128.0f, src.w / 128.0f, src.h / 128.0f);
            xy.insert(xy.end(), q, q + 8);
            uv.insert(uv.end(), t, t + 8);
        }
        x += src.w * scale + spacing;
    }
    drawQuads(g_fontTexture, xy.data(), uv.data(), static_cast<int>(xy.size() / 8), color);
}

int MeasureText(const char *text, int fontSize)
{
    if (!text)
        return 0;
    ensureFont();
    fontSize = std::max(fontSize, 10);
    const float scale = fontSize / 10.0f;
    const int spacing = fontSize / 10;
    float width = 0.0f, best = 0.0f;
    int glyphs = 0;
    for (const char *p = text; *p;)
    {
        const int cp = nextCodepoint(p);
        if (cp == '\n')
        {
            best = std::max(best, width);
            width = 0.0f;
            glyphs = 0;
            continue;
        }
        width += kDefaultFontWidths[glyphIndex(cp)] * scale + (glyphs++ ? spacing : 0);
    }
    return static_cast<int>(std::max(best, width));
}

Image GenImageColor(int width, int height, Color color)
{
    Image image{};
    image.width = width;
    image.height = height;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    auto *pixels = static_cast<uint8_t *>(std::malloc(static_cast<size_t>(width) * height * 4u));
    for (int i = 0; pixels && i < width * height; ++i)
        std::memcpy(pixels + i * 4, &color, 4);
    image.data = pixels;
    return image;
}

void ImageCrop(Image *image, Rectangle crop)
{
    if (!image || !image->data)
        return;
    const int x = std::clamp(static_cast<int>(crop.x), 0, image->width);
    const int y = std::clamp(static_cast<int>(crop.y), 0, image->height);
    const int w = std::clamp(static_cast<int>(crop.width), 0, image->width - x);
    const int h = std::clamp(static_cast<int>(crop.height), 0, image->height - y);
    auto *out = static_cast<uint8_t *>(std::malloc(static_cast<size_t>(w) * h * 4u));
    const auto *in = static_cast<const uint8_t *>(image->data);
    for (int row = 0; out && row < h; ++row)
        std::memcpy(out + row * w * 4, in + ((y + row) * image->width + x) * 4, static_cast<size_t>(w) * 4u);
    std::free(image->data);
    image->data = out;
    image->width = w;
    image->height = h;
}

void UnloadImage(Image image) { std::free(image.data); }
bool ExportImage(Image, const char *) { return false; }

Texture2D LoadTextureFromImage(Image image)
{
    Texture2D texture{};
    if (!g_glReady)
        return texture;
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    TextureSlot slot;
    glGenTextures(1, &slot.texture);
    glBindTexture(GL_TEXTURE_2D, slot.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.id = g_nextTexture++;
    texture.width = image.width;
    texture.height = image.height;
    texture.mipmaps = 1;
    texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    g_textures[texture.id] = std::move(slot);
    if (image.data)
        UpdateTexture(texture, image.data);
    return texture;
}

Image LoadImageFromTexture(Texture2D texture)
{
    Image image = GenImageColor(texture.width, texture.height, BLANK);
    const auto it = g_textures.find(texture.id);
    if (it != g_textures.end() && image.data && it->second.pixels.size() == static_cast<size_t>(texture.width) * texture.height * 4u)
        std::memcpy(image.data, it->second.pixels.data(), it->second.pixels.size());
    return image;
}

void UpdateTexture(Texture2D texture, const void *pixels)
{
    const auto it = g_textures.find(texture.id);
    if (it == g_textures.end() || !it->second.texture || !pixels)
        return;
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    glBindTexture(GL_TEXTURE_2D, it->second.texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void UnloadTexture(Texture2D texture)
{
    const auto it = g_textures.find(texture.id);
    if (it == g_textures.end())
        return;
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    if (it->second.texture)
        glDeleteTextures(1, &it->second.texture);
    g_textures.erase(it);
}

bool IsKeyDown(int) { return false; }
bool IsKeyPressed(int) { return false; }
bool IsGamepadAvailable(int gamepad) { return gamepad == 0; }

bool IsGamepadButtonDown(int gamepad, int button)
{
    if (gamepad != 0)
        return false;
    const unsigned int b = g_pad.buttons;
    switch (button)
    {
    case GAMEPAD_BUTTON_LEFT_FACE_UP: return b & SCE_CTRL_UP;
    case GAMEPAD_BUTTON_LEFT_FACE_RIGHT: return b & SCE_CTRL_RIGHT;
    case GAMEPAD_BUTTON_LEFT_FACE_DOWN: return b & SCE_CTRL_DOWN;
    case GAMEPAD_BUTTON_LEFT_FACE_LEFT: return b & SCE_CTRL_LEFT;
    case GAMEPAD_BUTTON_RIGHT_FACE_UP: return b & SCE_CTRL_TRIANGLE;
    case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT: return b & SCE_CTRL_CIRCLE;
    case GAMEPAD_BUTTON_RIGHT_FACE_DOWN: return b & SCE_CTRL_CROSS;
    case GAMEPAD_BUTTON_RIGHT_FACE_LEFT: return b & SCE_CTRL_SQUARE;
    case GAMEPAD_BUTTON_LEFT_TRIGGER_1: return b & SCE_CTRL_L1;
    case GAMEPAD_BUTTON_RIGHT_TRIGGER_1: return b & SCE_CTRL_R1;
    case GAMEPAD_BUTTON_LEFT_TRIGGER_2: return (b & SCE_CTRL_L2) || g_rearLeft;
    case GAMEPAD_BUTTON_RIGHT_TRIGGER_2: return (b & SCE_CTRL_R2) || g_rearRight;
    case GAMEPAD_BUTTON_MIDDLE_LEFT: return b & SCE_CTRL_SELECT;
    case GAMEPAD_BUTTON_MIDDLE_RIGHT: return b & SCE_CTRL_START;
    case GAMEPAD_BUTTON_LEFT_THUMB: return b & SCE_CTRL_L3;
    case GAMEPAD_BUTTON_RIGHT_THUMB: return b & SCE_CTRL_R3;
    default: return false;
    }
}

float GetGamepadAxisMovement(int gamepad, int axis)
{
    if (gamepad != 0)
        return 0.0f;
    auto norm = [](unsigned char v)
    {
        const float f = (static_cast<float>(v) - 128.0f) / 127.0f;
        return std::clamp(f, -1.0f, 1.0f);
    };
    switch (axis)
    {
    case GAMEPAD_AXIS_LEFT_X: return norm(g_pad.lx);
    case GAMEPAD_AXIS_LEFT_Y: return norm(g_pad.ly);
    case GAMEPAD_AXIS_RIGHT_X: return norm(g_pad.rx);
    case GAMEPAD_AXIS_RIGHT_Y: return norm(g_pad.ry);
    default: return 0.0f;
    }
}

void InitAudioDevice(void)
{
    if (g_audioDevice != 0)
        return;
    SDL_AudioSpec want{}, have{};
    want.freq = 48000;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = static_cast<Uint16>(g_audioBufferFrames);
    want.callback = audioThunk;
    g_audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (g_audioDevice == 0)
        std::fprintf(stderr, "[vita] SDL audio failed: %s\n", SDL_GetError());
}

void CloseAudioDevice(void)
{
    if (g_audioDevice != 0)
        SDL_CloseAudioDevice(g_audioDevice);
    g_audioDevice = 0;
}

bool IsAudioDeviceReady(void) { return g_audioDevice != 0; }
void SetAudioStreamBufferSizeDefault(int size) { g_audioBufferFrames = std::max(256, size); }

AudioStream LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels)
{
    AudioStream stream{};
    stream.sampleRate = sampleRate;
    stream.sampleSize = sampleSize;
    stream.channels = channels;
    return stream;
}

void SetAudioStreamCallback(AudioStream, AudioCallback callback) { g_audioCallback.store(callback, std::memory_order_release); }

void PlayAudioStream(AudioStream)
{
    if (g_audioDevice != 0)
        SDL_PauseAudioDevice(g_audioDevice, 0);
}
