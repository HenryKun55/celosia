// Native renderer on OpenGL (see runtime/native_renderer.h and the Metal
// backend, which this follows pass for pass): GL 3.3 core with GLSL on
// macOS (SOTC_NATIVE_GL=1, to check it against Metal), vitaGL with CG on the
// Vita. Render targets are GL textures, so the picture is y-up: screen
// positions come from the clip position (no gl_FragCoord), and the output is
// flipped once when it is read back.
//
// The GL work runs on a render thread of its own (GlBackend at the end): the
// game thread records a frame while the one before is drawn and shown.
//
// Vita (vitaGL) differences: CG shaders; every framebuffer gets its own depth
// and stencil (nothing to attach); no depth textures, so the passes that read
// depth (bloom mask, depth of field) are left out there for now.
#include "native_backend.h"
#include "runtime/native_recorder.h"
#include "runtime/native_tuning.h"
#include "runtime/ps2_stats.h"

#if defined(__APPLE__) || (defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM))

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <malloc/malloc.h>
#else
#include <mutex>
#include <malloc.h>
#include <vitaGL.h>
#include <psp2/kernel/cpu.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/threadmgr.h>
#define PS2X_NATIVE_VITA 1
std::recursive_mutex &ps2xVitaGlMutex();
extern "C" unsigned int _pthread_stack_default_user; // ps2_vita_runtime.cpp
extern "C" unsigned int _newlib_heap_size_user;      // ps2_vita_runtime.cpp
extern "C" void vglCeloMemStats(size_t *used, size_t *spillBytes, size_t *spillCount, size_t *spillTotal, size_t *spillPeak); // celosia.patch (round 34)
extern "C" uint32_t vgl_celo_scenes; // celosia.patch (round 45): scenes begun (vglCeloSceneStats)
extern "C" void vglCeloOwnMips(int on); // celosia.patch (round 55): glTexImage2D levels >= 1 taken as given
extern "C" GLboolean vglCeloTexImageSwizzled(GLsizei width, GLsizei height, GLint levels, const void *data); // celosia.patch (round 73)
extern "C" void vglCeloLeanDraw(int bits); // celosia.patch (round 56): bit 0 uniform copies by memcpy, bit 1 one vertex stream
extern "C" void vglCeloUniformStats(uint32_t *bytes, uint32_t *wraps); // celosia.patch (round 65c): uniform bytes reserved, pool wraps
#include <psp2/gxm.h>
extern "C" SceGxmNotification query_fence;          // vitaGL's (buffers.c): every scene's end notification, value+1 a scene
extern "C" SceGxmContext *gxm_context;              // vitaGL's (gxm.c): round 87 (glVol87) disables the volumes' fragment program
extern "C" volatile unsigned long long ps2x_vita_swap_us; // raylib_vita.cpp (round 86): time in vglSwapBuffers, summed
#endif

#include "ThreadNaming.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <functional>
#include <map>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <atomic>

namespace
{
    // Round 86 (SOTC_GPU86_LOG, the Vita; on by default with
    // SOTC_FRAME_LOG_ALL=1): the GPU's own timeline. vitaGL ends every GXM
    // scene with one notification (query_fence: its value counts the scenes
    // sent, the word at its address the scenes the GPU has finished). A
    // thread of its own reads both every 0.5 ms: when each scene was seen
    // finished, and how long the GPU had work outstanding (busy) or none
    // (idle). The render thread notes the value at each target bind (which
    // scene draws into which target) and at each present (the frame's last
    // scene, the scenes still on the GPU when the swap begins). Every 30
    // presents a [gpu86] line: the GPU's time a frame (the frame's last scene
    // finished to the next frame's), busy / idle, the backlog, the present
    // and its swap; every 600 a [gpu86s] line: one frame's slowest scenes
    // (their target, the time from the later of the scene before finishing
    // and their own sending to their finishing).
    namespace gpu86
    {
#if defined(PS2X_NATIVE_VITA)
        bool on()
        {
            static const bool v = []
            {
                if (const char *e = std::getenv("SOTC_GPU86_LOG"))
                    return std::atoi(e) != 0;
                const char *a = std::getenv("SOTC_FRAME_LOG_ALL");
                return a && std::atoi(a) != 0;
            }();
            return v;
        }
        constexpr uint32_t kRing = 4096u;
        std::atomic<uint32_t> g_tag[kRing];
        std::atomic<uint64_t> g_doneAt[kRing], g_sentAt[kRing]; // first seen finished / sent
        std::atomic<uint32_t> g_sentTag[kRing];
        std::atomic<uint64_t> g_busyUs{0}, g_idleUs{0};
        uint32_t sent() { return *const_cast<volatile uint32_t *>(&query_fence.value); }
        uint32_t done() { return query_fence.address ? *const_cast<volatile uint32_t *>(query_fence.address) : 0u; }
        void poller()
        {
            ThreadNaming::SetCurrentThreadName("Gpu86");
            uint32_t last = done(), lastSent = sent();
            uint64_t prev = native_renderer::nowUs();
            bool busy = false;
            for (;;)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(500));
                const uint64_t now = native_renderer::nowUs();
                const uint32_t d = done(), s = sent();
                if (d - last < kRing)
                    for (uint32_t v = last + 1u; v - last <= d - last; ++v)
                    {
                        g_doneAt[v & (kRing - 1u)].store(now, std::memory_order_relaxed);
                        g_tag[v & (kRing - 1u)].store(v, std::memory_order_release);
                    }
                last = d;
                if (s - lastSent < kRing)
                    for (uint32_t v = lastSent + 1u; v - lastSent <= s - lastSent; ++v)
                    {
                        g_sentAt[v & (kRing - 1u)].store(now, std::memory_order_relaxed);
                        g_sentTag[v & (kRing - 1u)].store(v, std::memory_order_release);
                    }
                lastSent = s;
                (busy ? g_busyUs : g_idleUs).fetch_add(now - prev, std::memory_order_relaxed);
                busy = static_cast<int32_t>(s - d) > 0;
                prev = now;
            }
        }
        void start()
        {
            static const bool started = []
            {
                std::thread(poller).detach();
                std::fprintf(stderr, "[gpu86] GPU timeline on (vitaGL's scene notifications read every 0.5 ms)\n");
                return true;
            }();
            (void)started;
        }
        bool doneTime(uint32_t v, uint64_t &t)
        {
            if (g_tag[v & (kRing - 1u)].load(std::memory_order_acquire) != v)
                return false;
            t = g_doneAt[v & (kRing - 1u)].load(std::memory_order_relaxed);
            return true;
        }
        bool sentTime(uint32_t v, uint64_t &t)
        {
            if (g_sentTag[v & (kRing - 1u)].load(std::memory_order_acquire) != v)
                return false;
            t = g_sentAt[v & (kRing - 1u)].load(std::memory_order_relaxed);
            return true;
        }
        // Round 87: what the renderer was doing at each bind (the scene's
        // first work: "pass 9-20", "q9" a full-screen quad of mode 9, "copy"),
        // printed with the [gpu86s] scenes.
        char g_label[24] = "";
        void label(const char *fmt, int a = 0, int b = 0)
        {
            if (on())
                std::snprintf(g_label, sizeof g_label, fmt, a, b);
        }
        struct Bind
        {
            uint32_t value, fbo, w, h;
            char what[24];
        };
        struct Frame
        {
            uint32_t last = 0;
            std::vector<Bind> binds;
        };
        constexpr uint32_t kFrames = 8u;
        Frame g_frames[kFrames];
        std::vector<Bind> g_binds; // the frame being drawn's
        uint64_t g_presents = 0, g_checked = 0;
        // The 120 presents' sums.
        double g_presentUs = 0, g_swapUs = 0, g_backlog = 0, g_gpuFrameUs = 0;
        uint64_t g_gpuFrames = 0, g_lastSwap = 0, g_busy0 = 0, g_idle0 = 0, g_n = 0;
        void bind(uint32_t fbo, uint32_t w, uint32_t h)
        {
            if (!on())
                return;
            start();
            if (g_binds.size() < 256u)
            {
                Bind b{sent(), fbo, w, h, {}};
                std::memcpy(b.what, g_label, sizeof b.what);
                g_binds.push_back(b);
            }
        }
        uint32_t backlog() { return on() ? sent() - done() : 0u; }
        // Round 87 (Tuning glInflight87): the scene count at the last present,
        // and the render thread's time waiting for the GPU to reach it.
        uint32_t g_lastPresentScene = 0;
        std::atomic<uint64_t> g_inflightWaitUs{0};
        uint64_t g_inflight0 = 0;
        void waitPreviousFrame()
        {
            if (!query_fence.address)
                return;
            const uint32_t target = g_lastPresentScene;
            const uint64_t start = native_renderer::nowUs();
            while (static_cast<int32_t>(done() - target) < 0 && native_renderer::nowUs() - start < 100000u)
                std::this_thread::sleep_for(std::chrono::microseconds(200));
            g_inflightWaitUs.fetch_add(native_renderer::nowUs() - start, std::memory_order_relaxed);
        }
        // The scene that finished as `w`: begun after the last bind noted with
        // value w - 2 (its draw ended the scene before). A frame's first scene
        // follows the swap, which leaves no scene open: its frame's first bind.
        const Bind *labelOf(uint32_t w, const Frame &f, const Frame &before)
        {
            if (w == before.last + 1u)
                return f.binds.empty() ? nullptr : &f.binds.front();
            const Bind *found = nullptr;
            for (const Frame *fr : {&before, &f})
                for (const Bind &b : fr->binds)
                    if (b.value == w - 2u)
                        found = &b;
            return found;
        }
        void presented(uint64_t tookUs, uint32_t backlogAtStart)
        {
            g_lastPresentScene = sent();
            if (!on())
                return;
            start();
            const uint64_t swap = ps2x_vita_swap_us;
            Frame &f = g_frames[g_presents % kFrames];
            f.last = sent();
            f.binds.swap(g_binds);
            g_binds.clear();
            ++g_presents;
            ++g_n;
            g_presentUs += static_cast<double>(tookUs);
            g_swapUs += static_cast<double>(swap - g_lastSwap);
            g_lastSwap = swap;
            g_backlog += backlogAtStart;
            // Frames 3+ presents old whose last scene and the one before's are seen finished.
            while (g_checked + 3u < g_presents)
            {
                const Frame &a = g_frames[g_checked % kFrames], &b = g_frames[(g_checked + 1u) % kFrames];
                uint64_t ta = 0, tb = 0;
                if (g_checked && doneTime(a.last, ta) && doneTime(b.last, tb) && tb >= ta)
                {
                    g_gpuFrameUs += static_cast<double>(tb - ta);
                    ++g_gpuFrames;
                }
                // Every 600 presents: frame b's scenes, the slowest first.
                if ((g_checked + 1u) % 600u == 0u && b.last - a.last < 128u)
                {
                    struct Row
                    {
                        uint64_t us;
                        uint32_t index;
                        const Bind *label;
                    };
                    std::vector<Row> rows;
                    uint64_t tPrev = 0;
                    bool havePrev = doneTime(a.last, tPrev);
                    for (uint32_t v = a.last + 1u; v - a.last <= b.last - a.last; ++v)
                    {
                        uint64_t t = 0;
                        if (!doneTime(v, t))
                        {
                            havePrev = false;
                            continue;
                        }
                        // From the later of the scene before finishing and this one sent (the GPU idle in between left out).
                        uint64_t ts = 0, from = tPrev;
                        if (sentTime(v, ts) && ts > from)
                            from = ts;
                        if (havePrev && t >= from)
                            rows.push_back(Row{t - from, v - a.last, labelOf(v, b, a)});
                        tPrev = t;
                        havePrev = true;
                    }
                    std::sort(rows.begin(), rows.end(), [](const Row &x, const Row &y) { return x.us > y.us; });
                    char line[2048];
                    int at = std::snprintf(line, sizeof(line), "[gpu86s] present %llu: %u scenes; slowest (scene: target, ms from the one before):", (unsigned long long)(g_checked + 1u),
                                           b.last - a.last);
                    for (size_t i = 0; i < rows.size() && i < 10u && at > 0 && static_cast<size_t>(at) < sizeof(line); ++i)
                    {
                        const Bind *l = rows[i].label;
                        if (l && l->fbo == 0u)
                            at += std::snprintf(line + at, sizeof(line) - at, " %u: display %.2f;", rows[i].index, rows[i].us / 1000.0);
                        else if (l)
                            at += std::snprintf(line + at, sizeof(line) - at, " %u: fbo %u %ux%u %.2f [%s];", rows[i].index, l->fbo, l->w, l->h, rows[i].us / 1000.0, l->what);
                        else
                            at += std::snprintf(line + at, sizeof(line) - at, " %u: ? %.2f;", rows[i].index, rows[i].us / 1000.0);
                    }
                    std::fprintf(stderr, "%s\n", line);
                }
                ++g_checked;
            }
            if (g_n >= 30u)
            {
                const uint64_t busy = g_busyUs.load(std::memory_order_relaxed), idle = g_idleUs.load(std::memory_order_relaxed);
                const double n = static_cast<double>(g_n);
                const uint64_t inflight = g_inflightWaitUs.load(std::memory_order_relaxed);
                std::fprintf(stderr, "[gpu86] presents %llu: gpu frame %.2f ms (%llu frames), gpu busy %.2f idle %.2f ms a present, backlog at present %.1f scenes, present %.2f ms (swap %.2f) | r87 in-flight wait %.2f ms\n",
                             (unsigned long long)g_presents, g_gpuFrames ? g_gpuFrameUs / g_gpuFrames / 1000.0 : 0.0, (unsigned long long)g_gpuFrames,
                             (busy - g_busy0) / n / 1000.0, (idle - g_idle0) / n / 1000.0, g_backlog / n, g_presentUs / n / 1000.0, g_swapUs / n / 1000.0,
                             (inflight - g_inflight0) / n / 1000.0);
                g_inflight0 = inflight;
                g_busy0 = busy;
                g_idle0 = idle;
                g_n = 0;
                g_presentUs = g_swapUs = g_backlog = g_gpuFrameUs = 0;
                g_gpuFrames = 0;
            }
        }
#else
        bool on() { return false; }
        void bind(uint32_t, uint32_t, uint32_t) {}
        void label(const char *, int = 0, int = 0) {}
        void waitPreviousFrame() {}
        uint32_t backlog() { return 0u; }
        void presented(uint64_t, uint32_t) {}
#endif
    }

    // SOTC_BLOOM_DUMP: the GS bloom chain's targets by index (gsBloom[i]),
    // the last drawn frame's, handed out by latestBloomStages.
    const char *const kBloomStageNames[7] = {"mask256", "red128", "red64", "temporal64", "blur64", "blur128", "glow256"};
    std::mutex g_bloomStageMutex;
    std::vector<native_renderer::BloomStage> g_bloomStagesOut;
    constexpr float kNearW = 0.1f;

    // The host heap in use (the Vita's newlib heap, 160 MB; the Mac's malloc
    // zones): the memory log and the eviction under pressure read it.
    uint64_t heapUsed()
    {
#if defined(PS2X_NATIVE_VITA)
        return mallinfo().uordblks;
#else
        malloc_statistics_t stats{};
        malloc_zone_statistics(nullptr, &stats);
        return stats.size_in_use;
#endif
    }

    // vitaGL puts what does not fit its GPU pools (CDRAM, then its share of
    // main RAM) into the newlib heap (use_extra_mem): textures there are
    // counted apart. False elsewhere.
    bool inHostHeap(const void *p)
    {
#if defined(PS2X_NATIVE_VITA)
        static uintptr_t base = 0, end = 0;
        if (!base)
        {
            void *probe = std::malloc(16);
            SceKernelMemBlockInfo info{};
            info.size = sizeof(info);
            if (probe && sceKernelGetMemBlockInfoByAddr(probe, &info) >= 0)
            {
                base = reinterpret_cast<uintptr_t>(info.mappedBase);
                end = base + info.mappedSize;
            }
            std::free(probe);
        }
        const uintptr_t a = reinterpret_cast<uintptr_t>(p);
        return a >= base && a < end;
#else
        (void)p;
        return false;
#endif
    }

#if !defined(PS2X_NATIVE_VITA)
    // ---- GLSL (macOS) ----
    const char *kMainVertex = R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aUV;
uniform vec4 uClip[4], uDirections[4], uColours[4]; // matrix columns
uniform vec4 uMisc;  // lit, opacity, uv offset
uniform vec4 uFogB;  // size, vertex fog, base
uniform vec4 uFogC;  // scale, lod K, lod scale, max lod
uniform vec4 uGuard; // x > 0: layer 1's near plane (see Renderer::guardFor): what lies nearer than w = y cut
out vec4 vUV;        // uv, w, fog x w (the fog factor is linear on the screen, as the GS's)
out vec4 vColor;
out vec4 vScreen;    // clip x, y, w
void main()
{
    vec4 c = uClip[0] * aPos.x + uClip[1] * aPos.y + uClip[2] * aPos.z + uClip[3];
    // Depth from w, near plane at w = 0.1: depth = 1 - 0.1 / w.
    gl_Position = vec4(c.x, c.y, c.w - 0.2, c.w);
    float fog = uFogB.z > 0.5 ? clamp(uFogB.w + uFogC.x / max(c.w, 1e-3), 0.0, 255.0) / 255.0 : 1.0;
    vUV = vec4(aUV + uMisc.zw, c.w, fog * c.w);
    vColor = aColor / 128.0;
    vColor.a *= uMisc.y;
    if (uMisc.x < -0.5) // water (microcode 1): alpha x (0.2 + 0.8 (1 - |n . v|)) in camera space
    {
        vec4 p = uDirections[0] * aPos.x + uDirections[1] * aPos.y + uDirections[2] * aPos.z + uDirections[3];
        float c = dot(normalize(p.xyz), uDirections[1].xyz);
        vColor.a *= 0.2 + 0.8 * (1.0 - abs(c));
    }
    else if (uMisc.x > 0.5)
    {
        vec4 d = max(uDirections[0] * aNormal.x + uDirections[1] * aNormal.y + uDirections[2] * aNormal.z + uDirections[3], 0.0);
        d.w = 1.0;
        vec4 light = uColours[0] * d.x + uColours[1] * d.y + uColours[2] * d.z + uColours[3];
        vColor.rgb = min(vColor.rgb * light.rgb, vec3(255.0 / 128.0));
    }
    // w: the view depth past layer 1's near plane (w - near; linear, so
    // interpolated exactly): a fragment under 0 lies nearer and is cut, as
    // the microcode's clipper cuts the triangle there.
    vScreen = vec4(c.x, c.y, c.w, uGuard.x > 0.0 ? c.w - uGuard.y : 1.0);
}
)";

    const char *kMainFragment = R"(#version 330 core
in vec4 vUV;
in vec4 vColor;
in vec4 vScreen;
uniform sampler2D uTex, uBackground, uReflection, uRefraction;
uniform vec4 uFogA;      // start, width, steps, fogged
uniform vec4 uFogC;      // scale, lod K, lod scale, max lod
uniform vec4 uFogColour; // rgb, textured mode
uniform vec4 uDebug;
uniform vec4 uAlpha;     // alpha test: cut under x (at least 0.02), and at / over y when y > 0
out vec4 oColor;
void main()
{
    if (vScreen.w < 0.0) // layer 1 nearer than its near plane (uGuard)
        discard;
    float textured = uFogColour.w;
    vec2 screen = vScreen.xy / vScreen.z * 0.5 + 0.5;
    float depth = vUV.z;
    // Textured 5 and 4 sample pictures this renderer drew (y-up in GL) at
    // the draw's PS2 UV (v down the screen): v is turned over.
    if (textured > 4.5) // the last frame shown, at the draw's UV
    {
        oColor = vec4(texture(uTex, vec2(vUV.x, 1.0 - vUV.y)).rgb, 1.0) * vColor;
        return;
    }
    if (textured > 3.5)
    {
        vec4 c = vec4(texture(uRefraction, vec2(vUV.x, 1.0 - vUV.y)).rgb, 1.0) * vColor;
        if (c.a < 0.02)
            discard;
        oColor = c;
        return;
    }
    if (textured > 2.5)
    {
        oColor = vec4(clamp(texture(uRefraction, screen).rgb * uDebug.rgb, 0.0, 1.0), -uDebug.w);
        return;
    }
    if (textured > 1.5)
    {
        // TEX0 TCC 0: the mirror's alpha is not read; the vertex alpha (microcode 1) x VU 0x3f3 w.
        // reflectionBegin's ripple: band i of 32 (from the top) shifted by (sin(2 (12 i + t) 24 pi / H) + 1) (6.4 i / H + 0.2) texels.
        vec2 uv = screen;
        if (uFogC.z > 0.0)
        {
            float band = floor(clamp(1.0 - uv.y, 0.0, 0.999) * 32.0);
            uv.x += (sin(2.0 * (12.0 * band + uFogC.y) * 75.39822 / uFogC.z) + 1.0) * (6.4 * band / uFogC.z + 0.2) / 256.0;
        }
        oColor = vec4(clamp(texture(uReflection, uv).rgb * uDebug.rgb * vColor.rgb, 0.0, 1.0), clamp(vColor.a * -uDebug.w, 0.0, 1.0));
        return;
    }
    float lod = uFogC.w > 0.0 ? clamp(log2(max(depth, 1e-3)) * uFogC.z + uFogC.y, 0.0, uFogC.w) : 0.0;
    vec4 t = textured > 0.5 ? textureLod(uTex, vUV.xy, lod) : vec4(vec3(128.0 / 255.0), 1.0); // untextured: the GS takes the vertex colour as is (255 = white)
    float alpha = clamp(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a, 0.0, 1.0);
    if (alpha < max(0.02, uAlpha.x) || (uAlpha.y > 0.0 && alpha >= uAlpha.y)) // the cut-out, the GS alpha test's side (DrawState::alphaTest)
        discard;
    if (uDebug.w > 0.0)
    {
        oColor = vec4(uDebug.rgb, uDebug.w > 1.5 ? alpha : 1.0);
        return;
    }
    vec3 rgb = clamp(t.rgb * vColor.rgb, 0.0, 1.0);
    rgb = mix(uFogColour.rgb, rgb, vUV.w / vUV.z);
    if (uFogA.w > 0.5)
    {
        float f = clamp((depth - uFogA.x) / uFogA.y, 0.0, 1.0);
        if (uFogA.z > 0.0)
            f = floor(f * uFogA.z) / uFogA.z;
        rgb = mix(rgb, texture(uBackground, screen).rgb, f);
    }
    oColor = vec4(rgb, alpha);
}
)";


    // The plain world draws' fragment stage (textured 0 / 1 of kMainFragment,
    // the same arithmetic): only the samplers it reads (vitaGL sets every
    // sampler of a program at each draw). BG: fogged / zFogged draws (the
    // background image); CUT: the alpha cut-out (without it: the opaque
    // variant, for draws whose alpha cannot fall under 0.02). Preceded by
    // "#define BG n" and "#define CUT n" (after the version line).
    const char *kWorldFragment = R"(
in vec4 vUV;
in vec4 vColor;
in vec4 vScreen;
uniform sampler2D uTex;
#if BG
uniform sampler2D uBackground;
uniform vec4 uFogA;      // start, width, steps, fogged
#endif
uniform vec4 uFogC;      // scale, lod K, lod scale, max lod
uniform vec4 uFogColour; // rgb, textured mode
#if CUT
uniform vec4 uAlpha;     // alpha test: cut under x (at least 0.02), and at / over y when y > 0
#endif
out vec4 oColor;
void main()
{
    float textured = uFogColour.w;
    float depth = vUV.z;
    float lod = uFogC.w > 0.0 ? clamp(log2(max(depth, 1e-3)) * uFogC.z + uFogC.y, 0.0, uFogC.w) : 0.0;
    vec4 t = textured > 0.5 ? textureLod(uTex, vUV.xy, lod) : vec4(vec3(128.0 / 255.0), 1.0); // untextured: the GS takes the vertex colour as is (255 = white)
    float alpha = clamp(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a, 0.0, 1.0);
#if CUT
    if (vScreen.w < 0.0) // layer 1 nearer than its near plane (uGuard; such draws take the CUT programs)
        discard;
    if (alpha < max(0.02, uAlpha.x) || (uAlpha.y > 0.0 && alpha >= uAlpha.y)) // the cut-out, the GS alpha test's side (DrawState::alphaTest)
        discard;
#endif
    vec3 rgb = clamp(t.rgb * vColor.rgb, 0.0, 1.0);
    rgb = mix(uFogColour.rgb, rgb, vUV.w / vUV.z);
#if BG
    if (uFogA.w > 0.5)
    {
        vec2 screen = vScreen.xy / vScreen.z * 0.5 + 0.5;
        float f = clamp((depth - uFogA.x) / uFogA.y, 0.0, 1.0);
        if (uFogA.z > 0.0)
            f = floor(f * uFogA.z) / uFogA.z;
        rgb = mix(rgb, texture(uBackground, screen).rgb, f);
    }
#endif
    oColor = vec4(rgb, alpha);
}
)";

    // Round 45: the shadow volumes' fragment stage: nothing (the volume only
    // counts in the stencil; its colour is masked and blended away).
    const char *kVolumeFragment = R"(#version 330 core
out vec4 oColor;
void main()
{
    oColor = vec4(0.0);
}
)";

    // Full-screen passes: a triangle over the screen, uv 0..1 (y up).
    const char *kQuadVertex = R"(#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec4 uQuad; // blur copy: scale, angle, offset (NDC); scale 0: plain
uniform float uZ;   // the NDC z (SOTC_BLOOM_VITA: the Vita bloom mask's quad at the threshold's depth)
out vec2 vUV;
void main()
{
    vec2 p = aPos;
    if (uQuad.x != 0.0)
    {
        vec2 c = aPos * uQuad.x;
        float s = sin(uQuad.y), k = cos(uQuad.y);
        p = vec2(c.x * k - c.y * s, c.x * s + c.y * k) + uQuad.zw;
    }
    gl_Position = vec4(p, uZ, 1.0);
    vUV = aPos * 0.5 + 0.5;
}
)";

    const char *kQuadFragment = R"(#version 330 core
in vec2 vUV;
uniform sampler2D uSrc, uSrc2, uDepth;
uniform int uMode;
uniform vec4 uA, uB, uC, uSize;
out vec4 oColor;
float gsZ(float d, float zA, float zB) { return d >= 1.0 ? 0.0 : zA + zB * (1.0 - d) / 0.1; }
void main()
{
    if (uMode == 0) // copy
        oColor = vec4(texture(uSrc, vUV).rgb, 1.0);
    else if (uMode == 1) // blur: uA.xy step
    {
        float w0 = 0.30, w1 = 0.235, w2 = 0.12, w3 = 0.05;
        vec4 c = texture(uSrc, vUV) * w0;
        c += (texture(uSrc, vUV + uA.xy) + texture(uSrc, vUV - uA.xy)) * w1;
        c += (texture(uSrc, vUV + uA.xy * 2.0) + texture(uSrc, vUV - uA.xy * 2.0)) * w2;
        c += (texture(uSrc, vUV + uA.xy * 3.0) + texture(uSrc, vUV - uA.xy * 3.0)) * w3;
        oColor = c;
    }
    else if (uMode == 2) // temporal: mix(previous, current, uA.x)
        oColor = mix(texture(uSrc2, vUV), texture(uSrc, vUV), uA.x);
    else if (uMode == 3) // blur copy: alpha uA.x
        oColor = vec4(texture(uSrc, vUV).rgb, uA.x);
    else if (uMode == 4) // tint: rgb x uA.rgb, alpha uA.a
        oColor = vec4(clamp(texture(uSrc, vUV).rgb * uA.rgb, 0.0, 1.0), uA.a);
    else if (uMode == 13) // plain copy, alpha kept, shifted by uA.xy
        oColor = texture(uSrc, vUV + uA.xy);
    else if (uMode == 14) // glow: rgb x uA.rgb, alpha x uA.a
    {
        vec4 c = texture(uSrc, vUV);
        oColor = clamp(vec4(c.rgb * uA.rgb, c.a * uA.a), 0.0, 1.0);
    }
    else if (uMode == 5) // shadow darken: alpha uA.x
        oColor = vec4(0.0, 0.0, 0.0, uA.x);
    else if (uMode == 6) // shadow mask write
        oColor = vec4(1.0);
    else if (uMode == 7 || uMode == 8) // 4 x 4 reduction: shadow mask (7) or bloom mask from depth (8)
    {
        vec2 size = uMode == 7 ? vec2(textureSize(uSrc, 0)) : vec2(textureSize(uDepth, 0));
        ivec2 base = ivec2(vUV * size) & ~3;
        float sum = 0.0;
        for (int y = 0; y < 4; ++y)
            for (int x = 0; x < 4; ++x)
            {
                ivec2 at = min(base + ivec2(x, y), ivec2(size) - 1);
                if (uMode == 7)
                    sum += texelFetch(uSrc, at, 0).r;
                else
                    sum += gsZ(texelFetch(uDepth, at, 0).r, uA.y, uA.z) <= uA.x + 1.0 ? 1.0 : 0.0;
            }
        oColor = uMode == 7 ? vec4(vec3(sum / 16.0), 1.0) : vec4(uB.rgb * (sum / 16.0), 1.0);
    }
    else if (uMode == 9) // soft shadow: alpha mask x uA.x
        oColor = vec4(0.0, 0.0, 0.0, clamp(texture(uSrc, vUV).r * uA.x, 0.0, 1.0));
    else if (uMode == 10) // bloom composite: rgb x uB.rgb x uB.a
        oColor = vec4(texture(uSrc, vUV).rgb * uB.rgb * uB.a, 1.0);
    else if (uMode == 15) // the GS chain's mask at 256 x 256 (rows from the top): bilinear over the PS2 frame's
    {                     // texels (p + 0.5) x (2, 1.75), each white where GS Z <= uA.x + 1; uB the mask colour
        vec2 size = vec2(textureSize(uDepth, 0));
        vec2 p = vec2(floor(gl_FragCoord.x), 255.0 - floor(gl_FragCoord.y));
        vec2 t = (p + 0.5) * vec2(2.0, 1.75) - 0.5;
        vec2 i0 = floor(t), f = t - i0;
        float m[4];
        for (int k = 0; k < 4; ++k)
        {
            vec2 at = clamp(i0 + vec2(k & 1, k >> 1), vec2(0.0), vec2(511.0, 447.0));
            vec2 px = (at + 0.5) * size / vec2(512.0, 448.0);
            ivec2 ip = ivec2(min(px.x, size.x - 1.0), min(size.y - 1.0 - px.y, size.y - 1.0));
            m[k] = gsZ(texelFetch(uDepth, max(ip, ivec2(0)), 0).r, uA.y, uA.z) <= uA.x + 1.0 ? 1.0 : 0.0;
        }
        oColor = vec4(uB.rgb * mix(mix(m[0], m[1], f.x), mix(m[2], m[3], f.x), f.y), 1.0);
    }
    else if (uMode == 16) // the GS chain's blur: the 64 x 64 source onto uA.x pixels, 12 shifted lerps (8-bit, truncated), uA.y the tap alpha scale
    {
        // Worked with rows from the top as the GS's: the lerps weigh the later
        // taps more, so the glow drifts (down and right) as the game's does.
        float r = uA.x, k = 64.0 / r;
        vec2 p = vec2(floor(gl_FragCoord.x), r - 1.0 - floor(gl_FragCoord.y));
        vec2 t0 = (p + 0.5) * k;
        vec3 acc = floor(texture(uSrc, vec2(t0.x, 64.0 - t0.y) / 64.0).rgb * 255.0 + 0.001);
        vec2 d[12] = vec2[12](vec2(-1, 0), vec2(1, 0), vec2(0, -1), vec2(0, 1), vec2(-2, 0), vec2(2, 0),
                              vec2(0, -2), vec2(0, 2), vec2(-3, 0), vec2(3, 0), vec2(0, -3), vec2(0, 3));
        // The taps' alpha: 64 / 32 / 16 times the source's alpha (the temporal
        // step's, uA.y of 128), truncated (the GS's MODULATE with TCC 1).
        vec3 w = floor(vec3(64.0, 32.0, 16.0) * uA.y / 128.0) / 128.0;
        for (int i = 0; i < 12; ++i)
        {
            float a = i < 4 ? w.x : (i < 8 ? w.y : w.z);
            vec2 q = p - d[i];
            if (any(lessThan(q, vec2(0.0))) || any(greaterThanEqual(q, vec2(r))))
                continue;
            vec2 t = 0.5 + q * k;
            vec3 c = floor(texture(uSrc, vec2(t.x, 64.0 - t.y) / 64.0).rgb * 255.0 + 0.001);
            acc += floor((c - acc) * a);
        }
        oColor = vec4(acc / 255.0, 1.0);
    }
    else if (uMode == 17) // the GS chain's temporal step: Cd + ((Cs - Cd) x uA.x) truncated, Cs the history (uSrc2)
    {
        vec3 cd = floor(texture(uSrc, vUV).rgb * 255.0 + 0.001), cs = floor(texture(uSrc2, vUV).rgb * 255.0 + 0.001);
        oColor = vec4((cd + floor((cs - cd) * uA.x)) / 255.0, 1.0);
    }
    else if (uMode == 18) // the GS chain's composite: UV 0.5..256.5 over the PS2 frame; uA.xy render pixels per PS2 pixel
    {
        vec2 size = vec2(textureSize(uDepth, 0));
        vec2 x = vec2(gl_FragCoord.x, size.y - gl_FragCoord.y) / uA.xy - 0.5;
        vec2 uv = (0.5 + x * vec2(256.0 / 512.0, 256.0 / 448.0)) / 256.0;
        oColor = vec4(texture(uSrc, vec2(uv.x, 1.0 - uv.y)).rgb * uB.rgb * uB.a, 1.0);
    }
    // SOTC_BLOOM_VITA=1 (Mac): the Vita's bloom modes 8, 11, 15-18 (kQuadFragment's
    // CG below) as 19-24, line for line, so that the Mac runs the Vita's bloom.
    else if (uMode == 19) // Vita 8: the mask's alpha
        oColor = vec4(0.0, 0.0, 0.0, uA.x);
    else if (uMode == 20) // Vita 11: 4 x 4 reduction of the alpha mask in uB.rgb (uSize.xy: texel size)
    {
        vec2 base = floor(vUV / uSize.xy / 4.0) * 4.0 + 0.5;
        float sum = 0.0;
        for (int y = 0; y < 4; ++y)
            for (int x = 0; x < 4; ++x)
                sum += texture(uSrc, (base + vec2(x, y)) * uSize.xy).a;
        oColor = vec4(uB.rgb * (sum / 16.0), 1.0);
    }
    else if (uMode == 21) // Vita 15: the GS chain's mask from the picture's alpha (uSize.xy its size)
    {
        vec2 uv = vUV;
        vec2 p = vec2(floor(uv.x * 256.0), 255.0 - floor(uv.y * 256.0));
        vec2 t = (p + 0.5) * vec2(2.0, 1.75) - 0.5;
        vec2 i0 = floor(t), f = t - i0;
        vec2 s = uSize.xy / vec2(512.0, 448.0);
        vec2 a0 = clamp(i0, vec2(0.0), vec2(511.0, 447.0)), a1 = clamp(i0 + 1.0, vec2(0.0), vec2(511.0, 447.0));
        vec2 x0 = min(floor((a0 + 0.5) * s), uSize.xy - 1.0), x1 = min(floor((a1 + 0.5) * s), uSize.xy - 1.0);
        float m00 = texture(uSrc, (vec2(x0.x, uSize.y - 1.0 - x0.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
        float m10 = texture(uSrc, (vec2(x1.x, uSize.y - 1.0 - x0.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
        float m01 = texture(uSrc, (vec2(x0.x, uSize.y - 1.0 - x1.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
        float m11 = texture(uSrc, (vec2(x1.x, uSize.y - 1.0 - x1.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
        oColor = vec4(uB.rgb * mix(mix(m00, m10, f.x), mix(m01, m11, f.x), f.y), 1.0);
    }
    else if (uMode == 22) // Vita 16: the 12-tap blur (from vUV)
    {
        float r = uA.x, k = 64.0 / r;
        vec2 p = vec2(floor(vUV.x * r), r - 1.0 - floor(vUV.y * r));
        vec2 t0 = (p + 0.5) * k;
        vec3 acc = floor(texture(uSrc, vec2(t0.x, 64.0 - t0.y) / 64.0).rgb * 255.0 + 0.5);
        vec2 d[12] = vec2[12](vec2(-1, 0), vec2(1, 0), vec2(0, -1), vec2(0, 1), vec2(-2, 0), vec2(2, 0),
                              vec2(0, -2), vec2(0, 2), vec2(-3, 0), vec2(3, 0), vec2(0, -3), vec2(0, 3));
        vec3 w = floor(vec3(64.0, 32.0, 16.0) * uA.y / 128.0) / 128.0;
        for (int i = 0; i < 12; ++i)
        {
            float a = i < 4 ? w.x : (i < 8 ? w.y : w.z);
            vec2 q = p - d[i];
            if (q.x >= 0.0 && q.y >= 0.0 && q.x < r && q.y < r)
            {
                vec2 tq = 0.5 + q * k;
                vec3 c = floor(texture(uSrc, vec2(tq.x, 64.0 - tq.y) / 64.0).rgb * 255.0 + 0.5);
                acc += floor((c - acc) * a);
            }
        }
        oColor = vec4(acc / 255.0, 1.0);
    }
    else if (uMode == 23) // Vita 17: the temporal step
    {
        vec3 cd = floor(texture(uSrc, vUV).rgb * 255.0 + 0.5), cs = floor(texture(uSrc2, vUV).rgb * 255.0 + 0.5);
        oColor = vec4((cd + floor((cs - cd) * uA.x)) / 255.0, 1.0);
    }
    else if (uMode == 24) // Vita 18: the composite (uSize.xy the picture's size)
    {
        vec2 x = vec2(vUV.x * uSize.x, uSize.y - vUV.y * uSize.y) / uA.xy - 0.5;
        vec2 g = (0.5 + x * vec2(256.0 / 512.0, 256.0 / 448.0)) / 256.0;
        oColor = vec4(texture(uSrc, vec2(g.x, 1.0 - g.y)).rgb * uB.rgb * uB.a, 1.0);
    }
    else if (uMode == 12) // zFog: background (uSrc) by the depth's slice; uA = start, width, steps
    {
        float d = texelFetch(uDepth, ivec2(vUV * vec2(textureSize(uDepth, 0))), 0).r;
        float w = d >= 1.0 ? 1e30 : 0.1 / (1.0 - d);
        int steps = int(uA.z);
        float a = 0.0;
        for (int i = 0; i <= steps; ++i)
            if (w >= uA.x + uA.y * float(i) / float(steps))
                a = float((i * 128) / steps) / 128.0;
        oColor = vec4(texture(uSrc, vUV).rgb, a);
    }
    else // 11: depth of field: uA = zA, zB, focusZ, slices
    {
        float d = texelFetch(uDepth, ivec2(vUV * vec2(textureSize(uDepth, 0))), 0).r;
        float z = gsZ(d, uA.x, uA.y);
        float keep = 1.0;
        int slices = int(uA.w);
        for (int i = 0; i < slices; ++i)
            if (z <= float(uint(uA.z) >> uint(i)))
                keep *= 1.0 - float(i + 1) / float(slices);
        oColor = vec4(texture(uSrc, vUV).rgb, 1.0 - keep);
    }
}
)";
#else
    // ---- CG (Vita): the same stages. Varyings are float4 (Vita3K pairs them by size). ----
    const char *kMainVertex = R"(
void main(float3 aPos, float3 aNormal, float4 aColor, float2 aUV,
          uniform float4 uClip[4], uniform float4 uDirections[4], uniform float4 uColours[4],
          uniform float4 uMisc, uniform float4 uFogB, uniform float4 uFogC, uniform float4 uGuard,
          float4 out oPosition : POSITION,
          float4 out vUV : TEXCOORD0,
          float4 out vColor : TEXCOORD1,
          float4 out vScreen : TEXCOORD2)
{
    float4 c = uClip[0] * aPos.x + uClip[1] * aPos.y + uClip[2] * aPos.z + uClip[3];
    oPosition = float4(c.x, c.y, c.w - 0.2, c.w);
    float fog = uFogB.z > 0.5 ? clamp(uFogB.w + uFogC.x / max(c.w, 1e-3), 0.0, 255.0) / 255.0 : 1.0;
    vUV = float4(aUV + uMisc.zw, c.w, fog * c.w);
    vColor = aColor / 128.0;
    vColor.a *= uMisc.y;
    if (uMisc.x < -0.5) // water (microcode 1): alpha x (0.2 + 0.8 (1 - |n . v|)) in camera space
    {
        float4 p = uDirections[0] * aPos.x + uDirections[1] * aPos.y + uDirections[2] * aPos.z + uDirections[3];
        float c = dot(normalize(p.xyz), uDirections[1].xyz);
        vColor.a *= 0.2 + 0.8 * (1.0 - abs(c));
    }
    else if (uMisc.x > 0.5)
    {
        float4 d = max(uDirections[0] * aNormal.x + uDirections[1] * aNormal.y + uDirections[2] * aNormal.z + uDirections[3], 0.0);
        d.w = 1.0;
        float4 light = uColours[0] * d.x + uColours[1] * d.y + uColours[2] * d.z + uColours[3];
        vColor.rgb = min(vColor.rgb * light.rgb, float3(255.0 / 128.0, 255.0 / 128.0, 255.0 / 128.0));
    }
    // w: the view depth past layer 1's near plane (see the GLSL's).
    vScreen = float4(c.x, c.y, c.w, uGuard.x > 0.0 ? c.w - uGuard.y : 1.0);
}
)";

    const char *kMainFragment = R"(
float4 main(float4 vUV : TEXCOORD0, float4 vColor : TEXCOORD1, float4 vScreen : TEXCOORD2,
            uniform sampler2D uTex, uniform sampler2D uBackground, uniform sampler2D uReflection, uniform sampler2D uRefraction,
            uniform float4 uFogA, uniform float4 uFogC, uniform float4 uFogColour, uniform float4 uDebug, uniform float4 uAlpha) : COLOR
{
    if (vScreen.w < 0.0) // layer 1 nearer than its near plane (uGuard)
        discard;
    float textured = uFogColour.w;
    float2 screen = vScreen.xy / vScreen.z * 0.5 + 0.5;
    float depth = vUV.z;
    // Textured 5 and 4 sample pictures this renderer drew (y-up) at the
    // draw's PS2 UV (v down the screen): v is turned over.
    if (textured > 4.5)
        return float4(tex2D(uTex, float2(vUV.x, 1.0 - vUV.y)).rgb, 1.0) * vColor;
    if (textured > 3.5)
    {
        float4 c = float4(tex2D(uRefraction, float2(vUV.x, 1.0 - vUV.y)).rgb, 1.0) * vColor;
        if (c.a < 0.02)
            discard;
        return c;
    }
    if (textured > 2.5)
        return float4(saturate(tex2D(uRefraction, screen).rgb * uDebug.rgb), -uDebug.w);
    if (textured > 1.5) // TEX0 TCC 0: the mirror's alpha is not read; reflectionBegin's ripple
    {
        float2 uv = screen;
        if (uFogC.z > 0.0)
        {
            float band = floor(clamp(1.0 - uv.y, 0.0, 0.999) * 32.0);
            uv.x += (sin(2.0 * (12.0 * band + uFogC.y) * 75.39822 / uFogC.z) + 1.0) * (6.4 * band / uFogC.z + 0.2) / 256.0;
        }
        return float4(saturate(tex2D(uReflection, uv).rgb * uDebug.rgb * vColor.rgb), saturate(vColor.a * -uDebug.w));
    }
    float lod = uFogC.w > 0.0 ? clamp(log2(max(depth, 1e-3)) * uFogC.z + uFogC.y, 0.0, uFogC.w) : 0.0;
    float4 t = textured > 0.5 ? tex2Dlod(uTex, float4(vUV.xy, 0.0, lod)) : float4(0.50196, 0.50196, 0.50196, 1.0); // untextured: the vertex colour as is (255 = white)
    float alpha = saturate(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a);
    if (alpha < max(0.02, uAlpha.x) || (uAlpha.y > 0.0 && alpha >= uAlpha.y)) // the cut-out, the GS alpha test's side (DrawState::alphaTest)
        discard;
    if (uDebug.w > 0.0)
        return float4(uDebug.rgb, uDebug.w > 1.5 ? alpha : 1.0); // w 2: SOTC_GL_DYN_TINT (the draw's own alpha)
    float3 rgb = saturate(t.rgb * vColor.rgb);
    rgb = lerp(uFogColour.rgb, rgb, vUV.w / vUV.z);
    if (uFogA.w > 0.5)
    {
        float f = saturate((depth - uFogA.x) / uFogA.y);
        if (uFogA.z > 0.0)
            f = floor(f * uFogA.z) / uFogA.z;
        rgb = lerp(rgb, tex2D(uBackground, screen).rgb, f);
    }
    return float4(rgb, alpha);
}
)";


    // The plain world draws' fragment stage (see the GLSL one).
    const char *kWorldFragment = R"(
float4 main(float4 vUV : TEXCOORD0, float4 vColor : TEXCOORD1, float4 vScreen : TEXCOORD2,
            uniform sampler2D uTex,
#if BG
            uniform sampler2D uBackground, uniform float4 uFogA,
#endif
#if CUT
            uniform float4 uAlpha,
#endif
            uniform float4 uFogC, uniform float4 uFogColour) : COLOR
{
    float textured = uFogColour.w;
    float depth = vUV.z;
#if NOLOD
    // Round 73: a textured draw whose texture has one level: its texel read
    // straight from the interpolated UV (the same texel and filter as at
    // level 0; the GPU fetches it before the program runs), no LOD.
    float4 t = tex2D(uTex, vUV.xy);
    float alpha = saturate(t.a * 2.0 * vColor.a);
#else
    float lod = uFogC.w > 0.0 ? clamp(log2(max(depth, 1e-3)) * uFogC.z + uFogC.y, 0.0, uFogC.w) : 0.0;
    float4 t = textured > 0.5 ? tex2Dlod(uTex, float4(vUV.xy, 0.0, lod)) : float4(0.50196, 0.50196, 0.50196, 1.0); // untextured: the vertex colour as is (255 = white)
    float alpha = saturate(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a);
#endif
#if CUT && SOFT
    // Round 75: the cut-out without `discard`, for draws that write no depth
    // or stencil and blend by the source alpha into a target whose alpha is
    // kept or never read again: a cut pixel leaves 0 x its colour + the
    // destination x 1, the destination as it was (a discard makes the
    // SGX's tile wait on the program for the pixel's visibility). Used
    // without BG only: libshacccg (fast precision) compiles this one to
    // the CUT program's arithmetic instruction for instruction (operands
    // swapped in two commutative ones), but the BG one without the discard
    // takes the colour product in float where the CUT one takes it in half
    // (other roundings: Vita3K's captures 1-3 levels apart on scattered
    // pixels), and half casts in the source do not change its choice.
    bool away = vScreen.w < 0.0 || alpha < max(0.02, uAlpha.x) || (uAlpha.y > 0.0 && alpha >= uAlpha.y);
#elif CUT
    if (vScreen.w < 0.0) // layer 1 nearer than its near plane (uGuard; such draws take the CUT programs)
        discard;
    if (alpha < max(0.02, uAlpha.x) || (uAlpha.y > 0.0 && alpha >= uAlpha.y)) // the cut-out, the GS alpha test's side (DrawState::alphaTest)
        discard;
#endif
    float3 rgb = saturate(t.rgb * vColor.rgb);
    rgb = lerp(uFogColour.rgb, rgb, vUV.w / vUV.z);
#if BG
    if (uFogA.w > 0.5)
    {
        float2 screen = vScreen.xy / vScreen.z * 0.5 + 0.5;
        float f = saturate((depth - uFogA.x) / uFogA.y);
        if (uFogA.z > 0.0)
            f = floor(f * uFogA.z) / uFogA.z;
        rgb = lerp(rgb, tex2D(uBackground, screen).rgb, f);
    }
#endif
#if CUT && SOFT
    return away ? float4(0.0, 0.0, 0.0, 0.0) : float4(rgb, alpha);
#else
    return float4(rgb, alpha);
#endif
}
)";

    // The plain full-screen vertex program, and the one screenBlur's
    // scaled / turned copies (mode 3) use: no branch on the Vita.
    const char *kVolumeFragment = R"(
float4 main() : COLOR
{
    return float4(0.0, 0.0, 0.0, 0.0);
}
)";

    const char *kQuadVertex = R"(
void main(float2 aPos,
          float4 out oPosition : POSITION,
          float4 out vUV : TEXCOORD0)
{
    oPosition = float4(aPos, 0.0, 1.0);
    vUV = float4(aPos * 0.5 + 0.5, 0.0, 0.0);
}
)";
    const char *kQuadVertexTurned = R"(
void main(float2 aPos, uniform float4 uQuad,
          float4 out oPosition : POSITION,
          float4 out vUV : TEXCOORD0)
{
    float2 c = aPos * uQuad.x;
    float s = sin(uQuad.y), k = cos(uQuad.y);
    oPosition = float4(c.x * k - c.y * s + uQuad.z, c.x * s + c.y * k + uQuad.w, 0.0, 1.0);
    vUV = float4(aPos * 0.5 + 0.5, 0.0, 0.0);
}
)";

    // The bloom mask's quad (mode 8), at a depth of its own: uQuad.x the NDC z.
    const char *kQuadVertexDepth = R"(
void main(float2 aPos, uniform float4 uQuad,
          float4 out oPosition : POSITION,
          float4 out vUV : TEXCOORD0)
{
    oPosition = float4(aPos, uQuad.x, 1.0);
    vUV = float4(aPos * 0.5 + 0.5, 0.0, 0.0);
}
)";

    // No depth textures: the depth modes (8, 11, 12) of the Mac are not used
    // on the Vita; 8 and 11 are the bloom's there (its mask kept in the
    // picture's alpha, see Renderer::bloomMask).
    // One program per mode (MODE defined in front): an "uber" shader with a
    // run-time mode and a loop is more than Vita3K's shader translation (and
    // the real GPU's branching) handles well.
    const char *kQuadFragment = R"(
float4 main(float4 vUV : TEXCOORD0, uniform sampler2D uSrc, uniform sampler2D uSrc2,
            uniform float4 uA, uniform float4 uB, uniform float4 uSize) : COLOR
{
    float2 uv = vUV.xy;
#if MODE == 0
    return float4(tex2D(uSrc, uv).rgb, 1.0);
#elif MODE == 1
    float4 c = tex2D(uSrc, uv) * 0.30;
    c += (tex2D(uSrc, uv + uA.xy) + tex2D(uSrc, uv - uA.xy)) * 0.235;
    c += (tex2D(uSrc, uv + uA.xy * 2.0) + tex2D(uSrc, uv - uA.xy * 2.0)) * 0.12;
    c += (tex2D(uSrc, uv + uA.xy * 3.0) + tex2D(uSrc, uv - uA.xy * 3.0)) * 0.05;
    return c;
#elif MODE == 2
    // The bloom's temporal step as the GS does it (round 20): Cd + ((Cs - Cd)
    // x the history's share) truncated in 8 bits, Cd this frame's, Cs the
    // history (uSrc2), uA.x this frame's share. A plain lerp rounded to 8
    // bits left the history stuck up to ~20 levels away from the picture
    // (98% history: the change of a step rounds to nothing): the glow trailed
    // the camera for seconds after it stopped (the title menu, console r19).
    float3 cd = floor(tex2D(uSrc, uv).rgb * 255.0 + 0.5), cs = floor(tex2D(uSrc2, uv).rgb * 255.0 + 0.5);
    return float4((cd + floor((cs - cd) * (1.0 - uA.x))) / 255.0, 1.0);
#elif MODE == 3
    return float4(tex2D(uSrc, uv).rgb, uA.x);
#elif MODE == 4
    return float4(saturate(tex2D(uSrc, uv).rgb * uA.rgb), uA.a);
#elif MODE == 5
    return float4(0.0, 0.0, 0.0, uA.x);
#elif MODE == 6
    return float4(1.0, 1.0, 1.0, 1.0);
#elif MODE == 7
    // 4 x 4 reduction of the shadow mask (uSize: its texel size), unrolled.
    float2 base = floor(uv / uSize.xy / 4.0) * 4.0 + 0.5;
    float sum = 0.0;
    sum += tex2D(uSrc, (base + float2(0, 0)) * uSize.xy).r + tex2D(uSrc, (base + float2(1, 0)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(2, 0)) * uSize.xy).r + tex2D(uSrc, (base + float2(3, 0)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(0, 1)) * uSize.xy).r + tex2D(uSrc, (base + float2(1, 1)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(2, 1)) * uSize.xy).r + tex2D(uSrc, (base + float2(3, 1)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(0, 2)) * uSize.xy).r + tex2D(uSrc, (base + float2(1, 2)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(2, 2)) * uSize.xy).r + tex2D(uSrc, (base + float2(3, 2)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(0, 3)) * uSize.xy).r + tex2D(uSrc, (base + float2(1, 3)) * uSize.xy).r;
    sum += tex2D(uSrc, (base + float2(2, 3)) * uSize.xy).r + tex2D(uSrc, (base + float2(3, 3)) * uSize.xy).r;
    return float4(sum / 16.0, sum / 16.0, sum / 16.0, 1.0);
#elif MODE == 8
    return float4(0.0, 0.0, 0.0, uA.x);
#elif MODE == 9
    return float4(0.0, 0.0, 0.0, saturate(tex2D(uSrc, uv).r * uA.x));
#elif MODE == 13
    // Plain copy, alpha kept, shifted by uA.xy (the glow layer's reductions).
    return tex2D(uSrc, uv + uA.xy);
#elif MODE == 14
    // The glow layer over the picture: rgb x uA.rgb, alpha x uA.a.
    float4 g = tex2D(uSrc, uv);
    return saturate(float4(g.rgb * uA.rgb, g.a * uA.a));
#elif MODE == 11
    // 4 x 4 reduction of the bloom mask in the picture's alpha, in the mask colour uB.rgb.
    float2 base = floor(uv / uSize.xy / 4.0) * 4.0 + 0.5;
    float sum = 0.0;
    sum += tex2D(uSrc, (base + float2(0, 0)) * uSize.xy).a + tex2D(uSrc, (base + float2(1, 0)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(2, 0)) * uSize.xy).a + tex2D(uSrc, (base + float2(3, 0)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(0, 1)) * uSize.xy).a + tex2D(uSrc, (base + float2(1, 1)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(2, 1)) * uSize.xy).a + tex2D(uSrc, (base + float2(3, 1)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(0, 2)) * uSize.xy).a + tex2D(uSrc, (base + float2(1, 2)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(2, 2)) * uSize.xy).a + tex2D(uSrc, (base + float2(3, 2)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(0, 3)) * uSize.xy).a + tex2D(uSrc, (base + float2(1, 3)) * uSize.xy).a;
    sum += tex2D(uSrc, (base + float2(2, 3)) * uSize.xy).a + tex2D(uSrc, (base + float2(3, 3)) * uSize.xy).a;
    return float4(uB.rgb * (sum / 16.0), 1.0);
#elif MODE == 15
    // Round 65b: the GS chain's mask at 256 x 256 (rows from the top) as the
    // Mac's mode 15, from the mask in the picture's alpha (bloomMask) instead
    // of its depth: bilinear over the PS2 frame's texels (p + 0.5) x (2, 1.75),
    // each white where the picture's pixel there is in the mask; uB the mask
    // colour, uSize.xy the picture's size.
    float2 p = float2(floor(uv.x * 256.0), 255.0 - floor(uv.y * 256.0));
    float2 t = (p + 0.5) * float2(2.0, 1.75) - 0.5;
    float2 i0 = floor(t), f = t - i0;
    float2 s = uSize.xy / float2(512.0, 448.0);
    float2 a0 = clamp(i0, float2(0.0, 0.0), float2(511.0, 447.0)), a1 = clamp(i0 + 1.0, float2(0.0, 0.0), float2(511.0, 447.0));
    float2 x0 = min(floor((a0 + 0.5) * s), uSize.xy - 1.0), x1 = min(floor((a1 + 0.5) * s), uSize.xy - 1.0);
    float m00 = tex2D(uSrc, (float2(x0.x, uSize.y - 1.0 - x0.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
    float m10 = tex2D(uSrc, (float2(x1.x, uSize.y - 1.0 - x0.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
    float m01 = tex2D(uSrc, (float2(x0.x, uSize.y - 1.0 - x1.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
    float m11 = tex2D(uSrc, (float2(x1.x, uSize.y - 1.0 - x1.y) + 0.5) / uSize.xy).a >= 0.5 ? 1.0 : 0.0;
    return float4(uB.rgb * lerp(lerp(m00, m10, f.x), lerp(m01, m11, f.x), f.y), 1.0);
#elif MODE == 16
    // The GS chain's blur (the Mac's mode 16): the 64 x 64 source onto uA.x
    // pixels, 12 shifted lerps in 8 bits, truncated, rows from the top; uA.y
    // the source's alpha (of 128) that scales the taps' (see below).
    float r = uA.x, k = 64.0 / r;
    float2 p = float2(floor(uv.x * r), r - 1.0 - floor(uv.y * r));
    float2 t0 = (p + 0.5) * k;
    float3 acc = floor(tex2D(uSrc, float2(t0.x, 64.0 - t0.y) / 64.0).rgb * 255.0 + 0.5);
    // Branchless (round 65e): a tap outside the target adds nothing by its
    // weight, not by an `if` (in Vita3K the taps under the branch were all
    // skipped: the history was never blurred and the sky's glow saturated).
#define GS_TAP(DX, DY, A) \
    { \
        float2 q = p - float2(DX, DY); \
        float2 inside2 = step(float2(0.0, 0.0), q) * step(q, float2(r - 1.0, r - 1.0)); \
        float2 tq = 0.5 + clamp(q, float2(0.0, 0.0), float2(r - 1.0, r - 1.0)) * k; \
        float3 c = floor(tex2D(uSrc, float2(tq.x, 64.0 - tq.y) / 64.0).rgb * 255.0 + 0.5); \
        acc += floor((c - acc) * A) * (inside2.x * inside2.y); \
    }
    // The taps' alpha: 64 / 32 / 16 times the source's (the temporal step's,
    // uA.y of 128), truncated: the GS modulates by the texture's alpha (TCC 1).
    float3 w = floor(float3(64.0, 32.0, 16.0) * uA.y / 128.0) / 128.0;
    GS_TAP(-1.0, 0.0, w.x) GS_TAP(1.0, 0.0, w.x) GS_TAP(0.0, -1.0, w.x) GS_TAP(0.0, 1.0, w.x)
    GS_TAP(-2.0, 0.0, w.y) GS_TAP(2.0, 0.0, w.y) GS_TAP(0.0, -2.0, w.y) GS_TAP(0.0, 2.0, w.y)
    GS_TAP(-3.0, 0.0, w.z) GS_TAP(3.0, 0.0, w.z) GS_TAP(0.0, -3.0, w.z) GS_TAP(0.0, 3.0, w.z)
    return float4(acc / 255.0, 1.0);
#elif MODE == 17
    // The GS chain's temporal step (the Mac's mode 17): Cd + ((Cs - Cd) x uA.x)
    // truncated, Cs the history (uSrc2).
    float3 cd = floor(tex2D(uSrc, uv).rgb * 255.0 + 0.5), cs = floor(tex2D(uSrc2, uv).rgb * 255.0 + 0.5);
    return float4((cd + floor((cs - cd) * uA.x)) / 255.0, 1.0);
#elif MODE == 18
    // The GS chain's composite (the Mac's mode 18): UV 0.5..256.5 over the
    // PS2 frame; uA.xy render pixels per PS2 pixel, uSize.xy the picture's size.
    float2 x = float2(uv.x * uSize.x, uSize.y - uv.y * uSize.y) / uA.xy - 0.5;
    float2 g = (0.5 + x * float2(256.0 / 512.0, 256.0 / 448.0)) / 256.0;
    return float4(tex2D(uSrc, float2(g.x, 1.0 - g.y)).rgb * uB.rgb * uB.a, 1.0);
#else
    return float4(tex2D(uSrc, uv).rgb * uB.rgb * uB.a, 1.0);
#endif
}
)";
#endif

    // A map from 64-bit ids (meshes, textures) to values in one flat table, a
    // power of two long, probed in a line (as native_renderer::IdSet): looked
    // up for every draw, where std::unordered_map's prime bucket count cost an
    // integer division (a library call on the Vita) per lookup. Ids 0 and ~0
    // are kept apart (the table's empty and erased keys). Inserting may move
    // the values (pointers into it hold until the next insert).
    template <typename V>
    class IdMap
    {
    public:
        V *find(uint64_t id)
        {
            if (id == kEmpty || id == kErased)
                return m_special[id == kEmpty ? 0 : 1] ? &m_specialValue[id == kEmpty ? 0 : 1] : nullptr;
            if (m_keys.empty())
                return nullptr;
            for (size_t i = slot(id);; i = (i + 1u) & m_mask)
            {
                const uint64_t k = m_keys[i];
                if (k == id)
                    return &m_values[i];
                if (k == kEmpty)
                    return nullptr;
            }
        }
        V &operator[](uint64_t id)
        {
            if (id == kEmpty || id == kErased)
            {
                const int s = id == kEmpty ? 0 : 1;
                if (!m_special[s])
                {
                    m_special[s] = true;
                    m_specialValue[s] = V{};
                    ++m_size;
                }
                return m_specialValue[s];
            }
            if (V *v = find(id))
                return *v;
            if ((m_used + 1u) * 2u > m_keys.size())
                grow();
            for (size_t i = slot(id);; i = (i + 1u) & m_mask)
            {
                const uint64_t k = m_keys[i];
                if (k == kEmpty || k == kErased)
                {
                    if (k == kEmpty)
                        ++m_used;
                    m_keys[i] = id;
                    m_values[i] = V{};
                    ++m_size;
                    return m_values[i];
                }
            }
        }
        void erase(uint64_t id)
        {
            if (id == kEmpty || id == kErased)
            {
                const int s = id == kEmpty ? 0 : 1;
                if (m_special[s])
                {
                    m_special[s] = false;
                    --m_size;
                }
                return;
            }
            if (m_keys.empty())
                return;
            for (size_t i = slot(id);; i = (i + 1u) & m_mask)
            {
                const uint64_t k = m_keys[i];
                if (k == id)
                {
                    m_keys[i] = kErased;
                    m_values[i] = V{};
                    --m_size;
                    return;
                }
                if (k == kEmpty)
                    return;
            }
        }
        size_t size() const { return m_size; }
        // Round 74: the cache lines a find(id) reads first, asked for ahead.
        void prefetch(uint64_t id) const
        {
            if (m_keys.empty() || id == kEmpty || id == kErased)
                return;
            const size_t i = slot(id);
            __builtin_prefetch(&m_keys[i]);
            __builtin_prefetch(&m_values[i]);
        }
        // f(id, value&); returning true from `erase` drops the entry.
        template <typename F>
        void forEach(F f)
        {
            for (int s = 0; s < 2; ++s)
                if (m_special[s])
                    f(s ? kErased : kEmpty, m_specialValue[s]);
            for (size_t i = 0; i < m_keys.size(); ++i)
                if (m_keys[i] != kEmpty && m_keys[i] != kErased)
                    f(m_keys[i], m_values[i]);
        }
        template <typename F>
        void eraseIf(F f)
        {
            for (int s = 0; s < 2; ++s)
                if (m_special[s] && f(m_specialValue[s]))
                {
                    m_special[s] = false;
                    --m_size;
                }
            for (size_t i = 0; i < m_keys.size(); ++i)
                if (m_keys[i] != kEmpty && m_keys[i] != kErased && f(m_values[i]))
                {
                    m_keys[i] = kErased;
                    m_values[i] = V{};
                    --m_size;
                }
        }

    private:
        static constexpr uint64_t kEmpty = 0, kErased = ~0ull;
        size_t slot(uint64_t id) const
        {
            id ^= id >> 33;
            id *= 0xff51afd7ed558ccdull;
            id ^= id >> 33;
            return static_cast<size_t>(id) & m_mask;
        }
        void grow()
        {
            std::vector<uint64_t> oldKeys;
            std::vector<V> oldValues;
            oldKeys.swap(m_keys);
            oldValues.swap(m_values);
            size_t capacity = 64u;
            while (capacity < (m_size + 1u) * 4u)
                capacity *= 2u;
            m_keys.assign(capacity, kEmpty);
            m_values.assign(capacity, V{});
            m_mask = capacity - 1u;
            m_used = 0;
            for (size_t i = 0; i < oldKeys.size(); ++i)
                if (oldKeys[i] != kEmpty && oldKeys[i] != kErased)
                {
                    size_t j = slot(oldKeys[i]);
                    while (m_keys[j] != kEmpty)
                        j = (j + 1u) & m_mask;
                    m_keys[j] = oldKeys[i];
                    m_values[j] = std::move(oldValues[i]);
                    ++m_used;
                }
        }
        std::vector<uint64_t> m_keys;
        std::vector<V> m_values;
        size_t m_mask = 0, m_used = 0, m_size = 0;
        bool m_special[2] = {false, false};
        V m_specialValue[2] = {};
    };

    struct Mesh
    {
        GLuint buffer = 0;
        int pool = -1;        // a kept mesh in a shared buffer (Renderer::pools), from vertex `first` (buffer 0)
        uint8_t minAlpha = 0; // the least vertex alpha (kept meshes; 0: not known)
        uint8_t maxAlpha = 0xFF; // round 45: the greatest (kept meshes; 0xFF: not known)
        // Round 45: a frame's own mesh's least / greatest vertex alpha, read in
        // planFrame for the alpha test (valid in frame alphaFrame).
        uint64_t alphaFrame = ~0ull;
        uint8_t frameMinAlpha = 0, frameMaxAlpha = 0xFF;
        uint32_t count = 0;
        bool dynamic = false; // in this frame's arena, from vertex `first`
        uint32_t first = 0;
        bool indexed = false; // `count` indices: dynamic from indexArena[firstIndex], else in indexBuffer
        uint32_t firstIndex = 0;
        GLuint indexBuffer = 0;
        bool skinned = false; // uploadSkinnedMesh: SkinVertex, posed by skinProgram
        uint64_t frame = 0;   // the arena it went into (debugging)
        uint64_t used = 0;    // the last frame that drew it (eviction)
        uint64_t bytes = 0;
        // Round 75: a kept, unskinned mesh's bounds (its vertices' least and
        // greatest x, y, z): the least clip w of a draw (planFrame's minW).
        bool box = false;
        float lo[3] = {}, hi[3] = {};
    };

    struct Texture
    {
        GLuint name = 0;
        uint32_t clamp = 0; // the wrap set on it (bit 0 s, bit 1 t clamp): new textures repeat
        uint32_t filter = 2; // the min / mag filter set on it (Tuning::textureFilter; 2 trilinear, as uploaded)
        bool mipped = false; // has mip levels
        uint8_t minAlpha = 0; // the least texel alpha over its levels
        uint8_t maxAlpha = 0xFF; // round 45: the greatest (counted up to 0x80, A = 1: no AREF is above it)
        uint64_t used = 0;
        uint64_t bytes = 0;
        bool inHeap = false; // vitaGL put it in the newlib heap (its pools were full)
        // SOTC_GL_TEX_VERIFY (Vita, round 55): its first level's memory and hash at upload.
        const uint8_t *level0 = nullptr;
        uint32_t level0W = 0, level0H = 0;
        uint64_t level0Hash = 0;
        // Round 73 (Tuning glSwizzle, the Vita): `name` holds the texels in
        // GXM's swizzled layout (swizzled), or, with SOTC_GL_SWIZZLE_BOTH=1,
        // a second texture `swz` holds them so while `name` stays linear
        // (the A/B's `swizzle73` picks one per frame).
        GLuint swz = 0;
        bool swizzled = false;
    };

    struct Draw
    {
        Draw(uint64_t m, const float c[16], const native_renderer::DrawState &s) : mesh(m), state(s) { std::memcpy(clip, c, sizeof(clip)); }
        uint64_t mesh;
        native_renderer::DrawState state;
        float clip[16];
        // drawMulti: the meshes after `mesh` drawn with this state and matrix
        // (extraMeshes[extraFirst ..], extraCount of them), each its own draw
        // once the render thread has the frame (Renderer::expandDraws).
        uint32_t extraFirst = 0, extraCount = 0;
    };

    // A colour target, with the framebuffer that draws into it (and its
    // depth/stencil when it has one).
    struct Target
    {
        GLuint texture = 0, fbo = 0, depth = 0;
        GLuint stencil = 0; // the Vita: a depth-stencil renderbuffer asking vitaGL for a stencil (round 38)
        uint32_t width = 0, height = 0;
    };

    struct Renderer
    {
        GLuint mainProgram = 0, quadProgram = 0;
        GLuint skinProgram = 0; // the main program posing a skinned mesh's bind pose first (0: not built)
#if defined(PS2X_NATIVE_VITA)
        GLuint quadPrograms[19] = {}; // by mode (8 and 11: the bloom mask's, see kQuadFragment; 12 unused; 15-18 the GS bloom chain, round 65b)
#endif
        GLuint quadBuffer = 0, blurBuffer = 0, vao = 0;
        // Uniform locations by (program, name): the names are string literals,
        // so their address is the key (no string built per call: that was 41%
        // of the Vita's time). A few dozen: a list looked through, the last
        // found first.
        struct UniformEntry
        {
            GLuint program;
            const char *name;
            GLint location;
        };
        std::vector<UniformEntry> uniforms;
        Target colorPlain; // the picture's colour alone (passes that read its depth)
        Target color, background, backgroundSmall, reflection, refraction, refractionHalf, snapshot, shadowMask;
        uint64_t bgSmallFrames = 0, bgSmallLogged = 0; // round 87: frames drawn with glBgSmall87
        Target backgroundLow; // round 87 (Tuning glBgSmall87): the background layer drawn at 256 x 256, made at its first use
        Target previous; // the last frame shown (feedbackBlur)
        // Round 45 (Tuning glFewCopies, a host that shows a copy of its own):
        // the last frame shown is that copy (shownFrame), not a copy of its own.
        GLuint previousTexture = 0; // 0: previous.texture
        // Tuning midFrameRedraw: the picture up to a mid-frame effect's layer
        // drawn again into a target of its own (one per effect), and the
        // heat haze's 256 x 256 copy apart from the water's.
        Target prefix[3], hazeSmall;
        Target shownFrame[2]; // what the host shows, alternately (the GPU draws later: never the target being drawn)
        int shownIndex = 0;
        // Round 73 (Tuning glFewScenes, the Vita): a second picture, drawn in
        // turn with `color` (swapped at each frame's start): the host shows
        // the picture as drawn, no copy into shownFrame first, and the one
        // not drawn into is the last frame shown (feedbackBlur's). Own depth
        // and stencil (each frame clears them). colorAltPlain: its colour alone.
        Target colorAlt, colorAltPlain;
        void makeColorAlt(uint32_t w, uint32_t h)
        {
            makeTarget(colorAlt, w, h, true);
            wantStencil(colorAlt);
            if (colorAltPlain.fbo)
                glDeleteFramebuffers(1, &colorAltPlain.fbo);
            colorAltPlain = Target{};
            colorAltPlain.texture = colorAlt.texture;
            colorAltPlain.width = w;
            colorAltPlain.height = h;
            glGenFramebuffers(1, &colorAltPlain.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, colorAltPlain.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAlt.texture, 0);
#if defined(PS2X_NATIVE_VITA)
            if (scratchDepthOn())
                vglFramebufferScratchDepth(colorAltPlain.fbo, GL_TRUE);
#endif
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = ~0u;
        }
        uint64_t shownDirect = 0, shownCopied = 0, glowHoisted = 0, plainReadDraws = 0; // round 73 ([gl:scenes])
        Target glowPrefix; // Vita3K: the picture below layer 8, apart (the glow layer's depth)
        // Round 55: screenBlur drawn on a second picture (blurApart): the
        // picture so far is copied into it and the blur's copies, read from the
        // first, laid over it; the rest of the frame is drawn on it, on the
        // first one's depth. `color` / `colorPlain` swap with these for the
        // rest of that frame (pictureSwapped, undone when the frame is done).
        Target blurOut, blurOutPlain;
        bool pictureSwapped = false;
        uint64_t blurApartCount = 0; // frames that drew their blur that way (the [gl:events] lines)
        void swapPicture()
        {
            std::swap(color.fbo, blurOut.fbo);
            std::swap(color.texture, blurOut.texture);
            std::swap(colorPlain.fbo, blurOutPlain.fbo);
            std::swap(colorPlain.texture, blurOutPlain.texture);
            pictureSwapped = !pictureSwapped;
            boundFbo = ~0u;
        }
        // The second picture and its colour-alone framebuffer (made at first use).
        void makeBlurOut(uint32_t w, uint32_t h)
        {
            makeTarget(blurOut, w, h, false);
            if (blurOutPlain.fbo)
                glDeleteFramebuffers(1, &blurOutPlain.fbo);
            blurOutPlain = Target{};
            blurOutPlain.texture = blurOut.texture;
            blurOutPlain.width = w;
            blurOutPlain.height = h;
            glGenFramebuffers(1, &blurOutPlain.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, blurOutPlain.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurOut.texture, 0);
#if defined(PS2X_NATIVE_VITA)
            if (scratchDepthOn())
                vglFramebufferScratchDepth(blurOutPlain.fbo, GL_TRUE);
#else
            // The rest of the frame is drawn on the picture's depth.
            glBindFramebuffer(GL_FRAMEBUFFER, blurOut.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, color.depth, 0);
#endif
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = ~0u;
        }
        Target glowLayer, glowSmall[4]; // bossDeathEffect: layer 8 apart (on the picture's depth); 256, 128, 64, 64
        Target bloomTex[3];
        bool bloomHistory = false;
        // The GS chain (as Metal): 0 mask 256, 1 128, 2 64, 3 64 lerped, 4 64 history, 5 128, 6 256.
        Target gsBloom[7];
        uint64_t bloomGameFrame = 0; // the game frame this frame's bloom came with (SOTC_BLOOM_LOG)
        // SOTC_BLOOM_LOG: a target's mean (0-255) of channel c (3: alpha);
        // -1 where it cannot be read back (Vita3K's targets read as black).
        float targetMean(const Target &t, int c)
        {
            if (!t.fbo)
                return -1.0f;
            std::vector<uint8_t> px(static_cast<size_t>(t.width) * t.height * 4u);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
            glBindFramebuffer(GL_FRAMEBUFFER, boundFbo);
            double sum = 0.0;
            for (size_t i = static_cast<size_t>(c); i < px.size(); i += 4)
                sum += px[i];
            return static_cast<float>(sum / (static_cast<double>(t.width) * t.height));
        }
        // the share (%) of a target's pixels with alpha >= 128 (the Vita bloom's mask)
        float alphaCover(const Target &t)
        {
            std::vector<uint8_t> px(static_cast<size_t>(t.width) * t.height * 4u);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
            glBindFramebuffer(GL_FRAMEBUFFER, boundFbo);
            size_t n = 0;
            for (size_t i = 3; i < px.size(); i += 4)
                n += px[i] >= 128u ? 1u : 0u;
            return 100.0f * static_cast<float>(n) / static_cast<float>(t.width * t.height);
        }
        bool bloomLog() const { return ps2_stats::bloomLogFrame(bloomGameFrame); }
        // Layer 1 (the sky and the far scenery) is drawn by the PS2 through a
        // projection of its own whose near plane lies far out (Fog::skyNear,
        // ~170 in the shrine): the world microcode's clipper cuts what lies
        // nearer. Drawn without it, a model around the camera inside the
        // shrine (header 0x3a8c60, layer 1) filled the background image with
        // its walls: a bright stone wall in the openings, which the bloom
        // blew to white (round 65e). SOTC_BG_NEAR=<w> overrides (0: off).
        // The background image cleared to the fog colour, as the GS's picture
        // (a sprite in FOGCOL before the sky): where no sky or far scenery
        // covers it the openings showed black (SOTC_BG_CLEAR_FOG=0: black).
        // Layer 1's vertex fog by each draw's FGE (round 65e; SOTC_BG_FGE=0:
        // none on layer 1, as before; SOTC_NATIVE_BGFOG=1: on every one).
        static bool bgFgeOn()
        {
            static const bool on = !std::getenv("SOTC_BG_FGE") || std::atoi(std::getenv("SOTC_BG_FGE")) != 0;
            return on;
        }
        static bool bgClearFog()
        {
            static const bool on = !std::getenv("SOTC_BG_CLEAR_FOG") || std::atoi(std::getenv("SOTC_BG_CLEAR_FOG")) != 0;
            return on;
        }
        void guardFor(const native_renderer::DrawState &st, float out[4]) const
        {
            static const float forced = std::getenv("SOTC_BG_NEAR") ? static_cast<float>(std::atof(std::getenv("SOTC_BG_NEAR"))) : -1.0f;
            const float nearW = forced >= 0.0f ? forced : fog.skyNear;
            if (st.background && !st.screen && nearW > 0.0f)
                out[0] = 1.0f, out[1] = nearW;
        }
        // The GS chain's blur taps (see kQuadFragment's mode 16): the source's alpha, of 128.
        float bloomTapAlpha() const { return native_renderer::bloomTapAlpha(bloom.temporal); }
        void logChain(const char *path, float dt)
        {
            if (!bloomLog())
                return;
            std::fprintf(stderr,
                         "[bloom:gl] f=%llu path=%s threshold=%.0f zA=%.2f zB=%.1f maskDepth=%.7f mask=(%.3f,%.3f,%.3f) temporal=%.4f taps=(%d,%d,%d)/128 history=%d "
                         "means R: mask256 %.2f red128 %.2f red64 %.2f temporal64 %.2f blur64 %.2f blur128 %.2f glow256 %.2f\n",
                         (unsigned long long)bloomGameFrame, path, bloom.threshold, bloom.zA, bloom.zB, dt, bloom.maskColor[0], bloom.maskColor[1],
                         bloom.maskColor[2], bloom.temporal, static_cast<int>(64.0f * bloomTapAlpha() / 128.0f), static_cast<int>(32.0f * bloomTapAlpha() / 128.0f),
                         static_cast<int>(16.0f * bloomTapAlpha() / 128.0f), gsBloomHistory ? 1 : 0, targetMean(gsBloom[0], 0), targetMean(gsBloom[1], 0),
                         targetMean(gsBloom[2], 0), targetMean(gsBloom[3], 0), targetMean(gsBloom[4], 0), targetMean(gsBloom[5], 0),
                         targetMean(gsBloom[6], 0));
        }
        // the picture in the bloom's mask (alpha >= 128) and outside it: mean R, % R >= 250
        void logRegions(const Target &t, const char *when)
        {
            if (!bloomLog() || !t.fbo)
                return;
            std::vector<uint8_t> px(static_cast<size_t>(t.width) * t.height * 4u);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
            glBindFramebuffer(GL_FRAMEBUFFER, boundFbo);
            double inSum = 0.0, outSum = 0.0;
            size_t in = 0, out = 0, inClip = 0;
            for (size_t i = 0; i < px.size(); i += 4)
            {
                if (px[i + 3] >= 128u)
                    inSum += px[i], ++in, inClip += px[i] >= 250u ? 1u : 0u;
                else
                    outSum += px[i], ++out;
            }
            std::fprintf(stderr, "[bloom:gl] f=%llu picture %s: in the mask (%zu px) R mean %.2f, %.2f%% >= 250; outside R mean %.2f\n",
                         (unsigned long long)bloomGameFrame, when, in, in ? inSum / in : 0.0, in ? 100.0 * inClip / in : 0.0, out ? outSum / out : 0.0);
        }
        void logAdd(const char *path, const Target &onto, float before)
        {
            if (!bloomLog())
                return;
            std::fprintf(stderr, "[bloom:gl] f=%llu path=%s add colour=(%.3f,%.3f,%.3f) strength=%.3f -> x%.3f; picture R mean %.2f -> %.2f\n",
                         (unsigned long long)bloomGameFrame, path, bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3],
                         bloom.color[0] * bloom.color[3], before, targetMean(onto, 0));
        }
        Target bloomShow; // SOTC_BLOOM_SHOW=1: the mask the chain read, 4 x 4 reduced, for pictures
        bool gsBloomHistory = false;
        uint32_t width = 0, height = 0;
        // What is bound now (render() starts from nothing known: the host
        // draws with its own state between frames). Binding only on a change
        // keeps vitaGL from redoing its per-call work every draw.
        GLuint boundFbo = ~0u, boundProgram = ~0u;
        // Vita bloom: after its mask is written into the picture's alpha
        // (bloomMask), the rest of the frame leaves the alpha alone.
        bool keepAlpha = false;
#if !defined(PS2X_NATIVE_VITA)
        // SOTC_BLOOM_VITA=1 (the Mac's OpenGL): the Vita's bloom drawn here,
        // its mode numbers (8, 11, 15-18: the CG programs) taken by the GLSL
        // ports 19-24 while its passes run.
        bool vitaModes = false;
        int macMode(int m) const
        {
            if (!vitaModes)
                return m;
            switch (m)
            {
            case 8: return 19;
            case 11: return 20;
            case 15: return 21;
            case 16: return 22;
            case 17: return 23;
            case 18: return 24;
            default: return m;
            }
        }
#endif
        IdMap<Mesh> meshes;
        IdMap<Texture> textures;
        // Kept meshes share a few large vertex buffers (Tuning glMeshPools):
        // the attributes are set once per buffer, not once per mesh (~2,500
        // meshes at the altar, most a few dozen vertices), and the draws of
        // meshes uploaded one after the other can be merged into one range.
        // Each pool's free ranges (start, count) in vertex order; what is let
        // go waits kPoolReuseFrames frames (the GPU may still read it).
        struct Pool
        {
            GLuint buffer = 0;
            uint8_t *mapped = nullptr; // the Vita: its memory, written in place
            uint32_t capacity = 0;
            std::vector<std::pair<uint32_t, uint32_t>> free;
        };
        struct PoolRange
        {
            int pool;
            uint32_t first, count;
            uint64_t frame;
        };
        // 16-bit indices reach every vertex of a pool (merged runs drawn
        // through indices: 32-bit ones from a buffer drew nothing in Vita3K).
        static constexpr uint32_t kPoolVertices = 0xFFFFu; // 2.3 MB each
        static constexpr uint64_t kPoolReuseFrames = 4;
        std::vector<Pool> pools;
        std::vector<PoolRange> poolPending; // let go, not reusable yet
        std::vector<PoolRange> deadRanges;  // evicted (GlBackend::evictKept): let go before the next frame
        bool poolAlloc(uint32_t count, int &pool, uint32_t &first);
        void poolFree(int pool, uint32_t first, uint32_t count) { poolPending.push_back({pool, first, count, arenaFrame}); }
        void poolRelease();
        // Round 34: a pool only goes when empty, and after a scene change the
        // few meshes still drawn keep many pools mostly empty (Vita3K, the
        // route's plain: 7 pools, 15.7 MB of vitaGL memory, 13.7 MB of it
        // free, for 1.7 MB of meshes and 0.8 MB of run copies; on a console
        // that memory is the CDRAM whose exhaustion sends vitaGL's next GPU
        // objects into the newlib heap). poolCompact empties the emptiest
        // such pool into the others' free room (its meshes copied, a budget
        // of vertices a frame; its run copies dropped, made again when drawn),
        // and poolRelease lets it go. The same vertices are drawn
        // (SOTC_GL_POOL_COMPACT=0: off).
        int poolDraining = -1;
        uint64_t poolMoved = 0, poolsEmptied = 0; // ([gl:mem])
        void poolCompact();
        // This frame's draws in their drawing order, with mesh and texture
        // looked up once (not in every pass) and the merges worked out: a
        // run of draws with the same state and matrix whose meshes are plain
        // triangle lists (Tuning glMergeDraws) is drawn by its first as one
        // range: the meshes themselves when they lie side by side in one
        // buffer, else a copy of them made once (runs).
        struct Planned
        {
            Mesh *mesh = nullptr;
            Texture *tex = nullptr;
            uint32_t run = 1;          // the leader: draws merged into it (itself included); 0: merged into an earlier one
            uint32_t mergedCount = 0;  // run > 1: vertices
            int runPool = -1;          // run > 1, meshes apart: its copy (runs), in this pool from runFirst
            uint32_t runFirst = 0;
            uint8_t minAlpha = 0;      // the least vertex alpha of its mesh(es) (0: not known)
            uint8_t maxAlpha = 0xFF;   // round 45: the greatest (0xFF: not known)
            float minW = -1.0f;        // round 75: a background draw's least clip w over its mesh(es)' bounds (-1: not known)
        };
        std::vector<Planned> plan;
        // Merged runs whose meshes lie apart in the pools: their vertices
        // copied side by side once (by the run's mesh ids, which hash the
        // meshes' content), drawn as one range from then on. Indices made per
        // frame would do without the copies, but cost the render thread every
        // frame (and Vita3K drew indexed draws from a shared buffer wrongly).
        struct RunCopy
        {
            int pool = -1; // -1: seen, not copied yet
            uint32_t first = 0, count = 0;
            uint64_t used = 0;
            uint32_t frames = 0; // frames in a row it was drawn (copied from kRunFrames on)
        };
        // A run is copied once drawn kRunFrames frames in a row; the copies
        // together hold at most kRunBudget vertices, the least recently drawn
        // going first. Round 13: 1 (was 4, against runs a moving camera makes
        // that do not last): after an area change the new area's runs were
        // drawn one by one for 4 frames, the render thread 170-190 ms each
        // against ~88 once copied (calibrated Vita3K, 240 MHz).
        static constexpr uint32_t kRunFrames = 1;
        static constexpr uint64_t kRunBudget = 0x30000; // ~7 MB
        IdMap<RunCopy> runs;
        uint64_t runVertices = 0;
        uint64_t runScanFrame = 0; // round 36: the frame of the last scan for room that found none
        bool copyRun(size_t from, size_t to, uint32_t total, RunCopy &copy);
        std::vector<std::pair<uint32_t, uint32_t>> planRanges; // scratch: a run's (first, count)
        void planFrame();
        void mergeStats();
        void sortFree(); // round 87 (Tuning glSort87)
        struct SortKey87
        {
            uint64_t tex, state, where;
            uint32_t k;
        };
        std::vector<SortKey87> sortKeys;
        std::vector<uint64_t> sortOrder;
        std::vector<Planned> sortPlan;
        struct
        {
            uint64_t frames = 0, draws = 0, free = 0, segments = 0, moved = 0;
        } sortStats;
        std::vector<uint32_t> leaders; // round 74: the plan's draws not merged away (Tuning glLeanPass)
        // Round 74 (SOTC_GL_PHASE_LOG=1): where the render thread's draw part
        // goes, a [gl:phases] line every 300 frames: the order and plan, the
        // passes (the world's draws) and within them the draw calls
        // themselves (vitaGL + GXM, any wait for the GPU's rings included),
        // the rest (effects, copies, uploads), present left out.
        struct
        {
            bool on = false;
            uint64_t start = 0, plan = 0, passes = 0, calls = 0, count = 0, total = 0, frames = 0;
            uint64_t arenaUs = 0, arenaBytes = 0; // the frame's own vertices / indices sent
        } phases;
        void phaseEnd()
        {
            if (phases.on && phases.start)
                phases.total += native_renderer::nowUs() - phases.start, phases.start = 0;
        }
        struct
        {
            uint64_t frames = 0, draws = 0, calls = 0, merged = 0, ranges = 0, indexed = 0, noDiscard = 0, bufferSwitches = 0;
            uint64_t progOff = 0, progZero = 0; // SOTC_GL_PROG_CHECK=1: draws with another GL program bound than the cache says / none
            uint64_t alphaSecond = 0; // the alpha test's second passes (round 38)
            uint64_t cut = 0, cutDepth = 0, soft = 0; // round 75: GL calls with discard (with depth writes), through the SOFT programs
            uint64_t vertsCut = 0, vertsSoft = 0, vertsMain = 0, vertsPlain = 0, mainKind[6] = {}, cutWhy[6] = {}, guarded = 0, guardOff = 0; // round 75: vertices by program kind; the full program's draws by textured
        } drawStats;
        // Round 45: the alpha test's passes ([shadow] line): second passes whose
        // mesh's least alpha is not known (the frame's own meshes), draws
        // drawn in one pass because every pixel is under AREF.
        struct
        {
            uint64_t secondUnknown = 0, merged = 0;
        } alphaStats;
        // Round 45 ([gl:scenes] every 300 frames): GXM scenes begun (every
        // render-target switch with a draw is one), target binds, depth /
        // stencil KB loaded and stored (vglCeloSceneStats), per frame.
        struct
        {
            uint64_t frames = 0, scenes = 0, binds = 0, loadKB = 0, storeKB = 0;
        } sceneStats;
        void noteScenes(); // SOTC_GL_DRAW_LOG: every 300 frames
        // Round 86 (Tuning glPresent86): the render thread's loop asks render
        // to leave the present to it (deferPresent); render then keeps what it
        // would have shown here, and the loop shows it after giving the
        // frame's data back to the game thread.
        bool deferPresent = false, presentPending = false;
        GLuint pendingTexture = 0;
        uint32_t pendingWidth = 0, pendingHeight = 0;
        uint64_t presentNow(); // the present's time (frame timing on), added to stats unless deferPresent
        // Round 38: the shadow pass, counted always, one [shadow] line every
        // 300 frames (the console's log): volume draws and their vertices,
        // the darkening passes, the frames with any volume.
        struct
        {
            uint64_t frames = 0, framesWithVolumes = 0, volumes = 0, vertices = 0, darkens = 0, alphaSecond = 0, lastFrame = ~0ull;
            uint64_t ispVolumes = 0, maskFused = 0; // round 87: volumes drawn ISP-only (glVol87); soft masks without the picture's break (glShmask87)
            double since = 0.0;
        } shadowStats;
        // What they hold (GPU memory: on the Vita vitaGL's pools, else the
        // newlib heap), kept as they change: the eviction budget and [gl:mem].
        uint64_t textureBytes = 0, textureHeapBytes = 0, meshBytes = 0, skinnedBytes = 0;
        void forget(const Mesh &m)
        {
            meshBytes -= m.bytes;
            if (m.skinned)
                skinnedBytes -= m.bytes;
        }
        void forget(const Texture &t)
        {
            textureBytes -= t.bytes;
            if (t.inHeap)
                textureHeapBytes -= t.bytes;
        }
        // Round 73: the GL texture a draw binds (the swizzled copy when there is one and the switch is on).
        static GLuint drawName(const Texture &t) { return t.swz && native_renderer::tuning().glSwizzle ? t.swz : t.name; }
        std::vector<Draw> draws;
        std::vector<uint64_t> extraMeshes; // drawMulti's meshes past each draw's first (Draw::extraFirst)
        std::vector<Draw> expanded;        // expandDraws' room (the last frame's list)
        // drawMulti's draws made one per mesh, in their place (the game thread
        // recorded one draw for each run of meshes). Once a frame, first.
        void expandDraws()
        {
            if (extraMeshes.empty())
                return;
            expanded.clear();
            expanded.reserve(draws.size() + extraMeshes.size());
            for (const Draw &d : draws)
            {
                expanded.push_back(d);
                expanded.back().extraCount = 0;
                for (uint32_t i = 0; i < d.extraCount; ++i)
                {
                    expanded.push_back(d);
                    Draw &e = expanded.back();
                    e.mesh = extraMeshes[d.extraFirst + i];
                    e.extraCount = 0;
                }
            }
            draws.swap(expanded);
            extraMeshes.clear();
        }
        std::vector<uint64_t> drawOrder; // layer << 32 | index: the draws in layer order, stable (sorting the draws themselves moved ~450 bytes each)
        // The draw loop's state as last set (pass() forgets it whenever
        // something else may have changed it): a call is made only for a
        // change. vitaGL redoes work on every call (a blend change patches
        // the fragment program; each uniform call looks its parameter up).
        struct DrawCache
        {
            int blend = -1, depthTest = -1, depthMask = -1;
            GLuint buffer = ~0u, texture = ~0u;
        } cache;
        // The main program's per-draw uniforms: locations looked up once, the
        // last values sent (kept across frames: nothing else sets them).
        enum MainUniform
        {
            uClip, uDirections, uColours, uMisc, uFogA, uFogB, uFogC, uFogColour, uDebug, uAlphaTest, uGuard, kMainUniforms
        };
        // The programs drawing meshes, each with its uniforms' locations and
        // the last values sent (a program keeps its own; kept across frames:
        // nothing else sets them): 0 mainProgram, 1 skinProgram, then the
        // lean ones (kWorldFragment) at 2 + 4 x skinned + 2 x BG + CUT.
        struct MainSlot
        {
            GLuint program = 0;
            GLint location[kMainUniforms] = {};
            float last[kMainUniforms][16] = {};
            int lastCount[kMainUniforms] = {};
            GLint palette = -1;
            uint64_t paletteSent = ~0ull; // frame << 32 | handle: what its palette holds
        };
        // Round 45: slot 10 the shadow volumes' program (kVolumeFragment).
        // Round 73: 11-18 the lean ones reading a one-level texture without a
        // LOD (NOLOD; Tuning glPlainReads), at 11 + 4 x skinned + 2 x BG + CUT.
        // Round 75: 19-20 the cut-out without discard (SOFT, without BG; Tuning
        // glSoftCut), at 19 + skinned.
        static constexpr int kMainSlots = 21;
        static constexpr int kVolumeSlot = 10;
        static constexpr int kPlainSlot = 11;
        static constexpr int kSoftSlot = 19;
        MainSlot mains[kMainSlots];
        int mainSlot = 0; // the one in use
        GLuint leanProgram(bool skinned, bool bg, bool cut, bool plain = false) const
        {
            return mains[(plain ? kPlainSlot : 2) + (skinned ? 4 : 0) + (bg ? 2 : 0) + (cut ? 1 : 0)].program;
        }
        GLuint softProgram(bool skinned) const { return mains[kSoftSlot + (skinned ? 1 : 0)].program; }
        std::vector<float> palettes;      // this frame's skin palettes (framePalette)
        void uniform4(MainUniform slot, const float *v, int count)
        {
            MainSlot &m = mains[mainSlot];
            if (m.location[slot] < 0) // not read by this program
                return;
            float *last = m.last[slot];
            const size_t bytes = static_cast<size_t>(count) * 4u * sizeof(float);
            if (m.lastCount[slot] == count)
            {
                // Word by word, inline (memcmp's library call was ~2.5% of the
                // render thread on the Vita, round 13): bits, as memcmp.
                const int n = count * 4;
                int i = 0;
                for (; i < n; ++i)
                {
                    uint32_t a, b;
                    std::memcpy(&a, last + i, 4);
                    std::memcpy(&b, v + i, 4);
                    if (a != b)
                        break;
                }
                if (i == n)
                    return;
            }
            std::memcpy(last, v, bytes);
            m.lastCount[slot] = count;
            glUniform4fv(m.location[slot], count, v);
        }
        // This frame's dynamic vertices (and the indices of the indexed ones).
        // Grown without clearing what is added (frameMesh hands out room
        // that is written once, in place).
        template <typename T>
        struct Arena
        {
            std::unique_ptr<T[]> items;
            size_t count = 0, capacity = 0;
            bool empty() const { return count == 0; }
            size_t size() const { return count; }
            const T *data() const { return items.get(); }
            void clear() { count = 0; }
            size_t bytes() const { return capacity * sizeof(T); }
            // nullptr when the heap has no room (the caller leaves the mesh out).
            T *grow(size_t n)
            {
                if (count + n > capacity)
                {
                    // Doubling, but by at most 1M items past what is asked
                    // (the old and the new block are both held while copying).
                    const size_t want = std::max(count + n, std::min(capacity * 2u + 4096u, count + n + (1u << 20)));
                    std::unique_ptr<T[]> bigger(new (std::nothrow) T[want]);
                    if (!bigger)
                        return nullptr;
                    if (count)
                        std::memcpy(bigger.get(), items.get(), count * sizeof(T));
                    items = std::move(bigger);
                    capacity = want;
                }
                T *at = items.get() + count;
                count += n;
                return at;
            }
            bool append(const T *from, size_t n)
            {
                if (!n)
                    return true;
                T *at = grow(n);
                if (at)
                    std::memcpy(at, from, n * sizeof(T));
                return at != nullptr;
            }
            // Gives back room a bigger frame left (empty arenas only).
            void trim(size_t keep)
            {
                if (count || capacity <= keep)
                    return;
                std::unique_ptr<T[]> smaller(keep ? new (std::nothrow) T[keep] : nullptr);
                if (keep && !smaller)
                    return;
                items = std::move(smaller);
                capacity = keep;
            }
        };
        Arena<native_renderer::Vertex> arena;
        Arena<uint32_t> indexArena;
        GLuint arenaBuffers[3] = {};                 // in turn: the GPU may still read the last two
        GLuint indexBuffers[3] = {};                 // the same for indexArena
        int arenaIndex = 0;
        uint64_t arenaFrame = 0;
        // [gl:tex] every 300 frames (the Vita's log; SOTC_GL_TEX_LOG elsewhere):
        // draws whose texture is not here (drawn white), uploads, GL errors.
        struct
        {
            uint64_t missing = 0, uploads = 0, errors = 0;
            uint64_t swizzled = 0; // round 73: uploads made swizzled (each copy)
            uint32_t lastError = 0;
        } texStats;
        void evictUnused(); // per-frame meshes no frame made for a while (the kept ones: GlBackend::evictKept)
        // Uploads and per-frame meshes of the frame being drawn, made by the
        // game thread (GlBackend) and applied by the render thread before it
        // draws (only that thread makes GL calls).
        struct Upload
        {
            enum Kind : uint8_t
            {
                MeshKind,
                SkinnedKind,
                TextureKind
            } kind = MeshKind;
            uint64_t id = 0;
            uint32_t count = 0, width = 0, height = 0, levels = 1;
            std::vector<uint8_t> bytes; // vertices, or the texture's levels
            std::vector<uint32_t> indices;
        };
        struct FrameMesh // a per-frame mesh: a range of this frame's arena
        {
            uint64_t id;
            uint32_t first, count, firstIndex;
            bool indexed;
        };
        std::vector<Upload> uploads;
        std::vector<FrameMesh> frameMeshes;
        std::vector<GLuint> deadBuffers, deadTextures; // evicted (GlBackend::evictKept): deleted before the next frame
        void applyUploads();
        void applyUploadList(std::vector<Upload> &list); // and empties it
        void uploadMesh(uint64_t id, const native_renderer::Vertex *vertices, uint32_t count);
        void uploadSkinned(uint64_t id, const native_renderer::SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount);
        void uploadTexture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels);
        // What the last frame drawn cost (the render thread's): read at the next hand-over.
        struct
        {
            uint64_t renderUs = 0, presentUs = 0;
            uint64_t lockUs = 0, uploadUs = 0, drawUs = 0, sweepUs = 0; // round 36: renderUs by part
            uint64_t planUs = 0, passUs = 0, callUs = 0, calls = 0; // round 74: drawUs by part (SOTC_GL_PHASE_LOG=1)
            uint32_t runCopies = 0, runScans = 0;                       // round 36: merged runs copied, scans for their room
            uint32_t targets = 0;
            uint32_t scenes = 0, depthKB = 0; // round 73: GPU scenes begun, depth / stencil KB loaded + stored (the Vita)
            uint32_t vertices = 0;            // round 73: vertices (indices) sent by the frame's 3D draws
        } stats;
        // SOTC_GL_SPIKE_LOG=1 (round 44, the console's 150-450 ms draw spikes):
        // the render thread's time between consecutive 3D draws of a frame,
        // the longest gap and the draw before it, logged when the frame's
        // draw part passes SOTC_GL_SPIKE_MS (100). One clock read a draw.
        struct
        {
            bool on = false;
            uint64_t last = 0, gap = 0;
            uint32_t index = 0, drawn = 0, order = 0, count = 0, target = 0;
            bool volume = false, overlay = false, skinned = false, dynamic = false;
            uint16_t alphaTest = 0;
            // Round 45: what came between the two draws of the longest gap
            // (the last effect event run, -100 none; the GXM scenes begun),
            // and the slowest single draw call (its index and time).
            int event = -100, gapEvent = -100;
            uint32_t scenesAt = 0, gapScenes = 0;
            uint64_t callMax = 0;
            uint32_t callIndex = 0;
        } spike;
        std::vector<native_renderer::ScreenBlur> blurs;
        native_renderer::Fog fog;
        native_renderer::Bloom bloom;
        native_renderer::GlowComposite glow;
        native_renderer::DepthOfField dof;
        bool hold = false; // holdFrame: uploads only, not drawn or shown

        std::mutex outputMutex;
        std::vector<uint8_t> output;
        uint32_t outputWidth = 0, outputHeight = 0;
        bool hasOutput = false;
        // SOTC_BLOOM_DUMP=1 (the Mac): every frame's bloom targets read back
        // (top row first), handed out with the frame's picture
        // (native_renderer::latestBloomStages), to compare stage by stage
        // with the GS's buffers.
        std::vector<native_renderer::BloomStage> bloomStages;
        // SOTC_BLOOM_SHOW: 1 the mask, 2 the glow, 3 the picture / mask / glow in turn every 90 game frames,
        // 5 this frame's 64 x 64 reduction, 6 the 64 x 64 history, 7 the background image (256 x 256),
        // 8 the reduction / the temporal step / the history in turn (5, 15, 25).
        int bloomShowMode() const
        {
            static const int mode = std::getenv("SOTC_BLOOM_SHOW") ? std::atoi(std::getenv("SOTC_BLOOM_SHOW")) : 0;
            if (mode == 8) // the 64 x 64 stages in turn: this frame's reduction, the temporal step, the history (blurred)
                return 5 + static_cast<int>((bloomGameFrame / 90u) % 3u) * 10;
            return mode == 3 ? static_cast<int>((bloomGameFrame / 90u) % 3u) : mode;
        }
        void bloomStage(const char *name, const Target &t)
        {
            static const bool on = std::getenv("SOTC_BLOOM_DUMP") && std::atoi(std::getenv("SOTC_BLOOM_DUMP")) != 0;
            if (!on || !t.fbo)
                return;
            native_renderer::BloomStage st;
            st.name = name;
            st.width = t.width;
            st.height = t.height;
            std::vector<uint8_t> px(static_cast<size_t>(t.width) * t.height * 4u);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
            glBindFramebuffer(GL_FRAMEBUFFER, boundFbo);
            st.rgba.resize(px.size());
            const size_t row = static_cast<size_t>(t.width) * 4u;
            for (uint32_t y = 0; y < t.height; ++y)
                std::memcpy(st.rgba.data() + static_cast<size_t>(y) * row, px.data() + static_cast<size_t>(t.height - 1u - y) * row, row);
            for (native_renderer::BloomStage &old : bloomStages)
                if (old.name == st.name)
                {
                    old = std::move(st);
                    return;
                }
            bloomStages.push_back(std::move(st));
        }

        static GLuint compile(GLenum type, const char *source)
        {
#if defined(PS2X_NATIVE_VITA)
            type = type == GL_VERTEX_SHADER ? GL_CG_VERTEX_SHADER_EXT : GL_CG_FRAGMENT_SHADER_EXT;
#endif
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            GLint ok = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
            if (!ok)
            {
                char log[4096] = {};
                GLsizei length = 0;
                glGetShaderInfoLog(shader, sizeof(log), &length, log);
                std::cerr << "[native:gl] shader error: " << std::string(log, std::min<size_t>(length, sizeof(log))) << std::endl;
            }
            return shader;
        }

        static GLuint program(const char *vertex, const char *fragment, std::initializer_list<const char *> attributes)
        {
            return program(compile(GL_VERTEX_SHADER, vertex), fragment, attributes);
        }
        // With a vertex shader already compiled (the lean programs share
        // the main ones': vitaGL keys its vertex program cache by the shader,
        // so switching between them keeps hitting it).
        static GLuint program(GLuint vertexShader, const char *fragment, std::initializer_list<const char *> attributes)
        {
            GLuint p = glCreateProgram();
            glAttachShader(p, vertexShader);
            glAttachShader(p, compile(GL_FRAGMENT_SHADER, fragment));
            GLuint index = 0;
            for (const char *name : attributes)
                glBindAttribLocation(p, index++, name);
            glLinkProgram(p);
            GLint ok = 0;
            glGetProgramiv(p, GL_LINK_STATUS, &ok);
            if (!ok)
            {
                std::cerr << "[native:gl] link failed" << std::endl;
                return 0;
            }
            return p;
        }

        GLint uniform(bool main, const char *name)
        {
            const GLuint program = main ? mainProgram : quadProgram;
            for (const UniformEntry &u : uniforms)
                if (u.name == name && u.program == program)
                    return u.location;
            const GLint location = glGetUniformLocation(program, name);
            uniforms.push_back({program, name, location});
            return location;
        }

        // The full-screen pass program for a mode (one program per mode on the Vita).
        void useQuad(int mode)
        {
#if defined(PS2X_NATIVE_VITA)
            if (mode >= 0 && mode < 19 && quadPrograms[mode])
                quadProgram = quadPrograms[mode];
#else
            (void)mode;
#endif
            useProgram(quadProgram);
        }

        void useProgram(GLuint p)
        {
            if (p == boundProgram)
                return;
            glUseProgram(p);
            boundProgram = p;
            for (int i = 0; i < kMainSlots; ++i)
                if (mains[i].program == p && p)
                {
                    mainSlot = i;
                    break;
                }
        }

        // The main vertex program posing a skinned mesh first: up to three
        // palette matrices by weight, position and normal (camera space),
        // then the main program's own work on them.
        static std::string skinnedVertex(const char *main)
        {
            std::string head = main;
            const size_t open = head.find("{\n", head.find("void main("));
            if (open == std::string::npos)
                return std::string();
            std::string body = head.substr(open + 2);
            head.resize(open + 2);
            for (const auto &[from, to] : {std::pair<std::string, std::string>{"aPos", "sPos"}, {"aNormal", "sNormal"}})
                for (size_t at = body.find(from); at != std::string::npos; at = body.find(from, at + to.size()))
                    body.replace(at, from.size(), to);
            const std::string size = std::to_string(native_renderer::kSkinPaletteQwords);
#if !defined(PS2X_NATIVE_VITA)
            const std::string uv = "layout(location = 3) in vec2 aUV;\n";
            const size_t at = head.find(uv);
            if (at == std::string::npos)
                return std::string();
            head.insert(at + uv.size(), "layout(location = 4) in vec3 aBones;\nlayout(location = 5) in vec3 aWeights;\nuniform vec4 uPalette[" + size + "];\n");
            const std::string pose = R"(    vec3 sPos = vec3(0.0), sNormal = vec3(0.0);
    for (int k = 0; k < 3; ++k)
    {
        int b = int(aBones[k] + 0.5);
        float w = aWeights[k];
        sPos += w * (uPalette[b].xyz * aPos.x + uPalette[b + 1].xyz * aPos.y + uPalette[b + 2].xyz * aPos.z + uPalette[b + 3].xyz);
        sNormal += w * (uPalette[b].xyz * aNormal.x + uPalette[b + 1].xyz * aNormal.y + uPalette[b + 2].xyz * aNormal.z);
    }
)";
#else
            const std::string uv = "float2 aUV,";
            const size_t at = head.find(uv);
            if (at == std::string::npos)
                return std::string();
            head.insert(at + uv.size(), " float3 aBones, float3 aWeights, uniform float4 uPalette[" + size + "],");
            std::string pose = "    float3 sPos = float3(0.0, 0.0, 0.0), sNormal = float3(0.0, 0.0, 0.0);\n";
            for (const char *k : {"x", "y", "z"})
            {
                const std::string b = std::string("(int)(aBones.") + k + " + 0.5)", w = std::string("aWeights.") + k;
                pose += "    {\n        int b = " + b + ";\n";
                pose += "        sPos += " + w + " * (uPalette[b].xyz * aPos.x + uPalette[b + 1].xyz * aPos.y + uPalette[b + 2].xyz * aPos.z + uPalette[b + 3].xyz);\n";
                pose += "        sNormal += " + w + " * (uPalette[b].xyz * aNormal.x + uPalette[b + 1].xyz * aNormal.y + uPalette[b + 2].xyz * aNormal.z);\n    }\n";
            }
#endif
            return head + pose + body;
        }

        static std::string worldFragment(bool bg, bool cut, bool nolod = false, bool soft = false)
        {
            const std::string defines = std::string("#define BG ") + (bg ? "1" : "0") + "\n#define CUT " + (cut ? "1" : "0") + "\n#define NOLOD " + (nolod ? "1" : "0") +
                                        "\n#define SOFT " + (soft ? "1" : "0") + "\n";
#if defined(PS2X_NATIVE_VITA)
            return defines + kWorldFragment;
#else
            return "#version 330 core\n" + defines + kWorldFragment;
#endif
        }

        bool init()
        {
            const GLuint mainVertex = compile(GL_VERTEX_SHADER, kMainVertex);
            mainProgram = program(mainVertex, kMainFragment, {"aPos", "aNormal", "aColor", "aUV"});
#if defined(PS2X_NATIVE_VITA)
            for (int mode : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18})
            {
                const std::string source = "#define MODE " + std::to_string(mode) + "\n" + kQuadFragment;
                quadPrograms[mode] = program(mode == 3 ? kQuadVertexTurned : (mode == 8 ? kQuadVertexDepth : kQuadVertex), source.c_str(), {"aPos"});
                if (!quadPrograms[mode])
                    return false;
                quadProgram = quadPrograms[mode];
                glUseProgram(quadProgram);
                glUniform1i(uniform(false, "uSrc"), 0);
                glUniform1i(uniform(false, "uSrc2"), 1);
            }
            quadProgram = quadPrograms[0];
#else
            quadProgram = program(kQuadVertex, kQuadFragment, {"aPos"});
#endif
            if (!mainProgram || !quadProgram)
                return false;
#if !defined(PS2X_NATIVE_VITA)
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
#endif
            const float tri[6] = {-1, -1, 3, -1, -1, 3};
            glGenBuffers(1, &quadBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
            const float corners[12] = {-1, -1, 1, -1, -1, 1, 1, -1, 1, 1, -1, 1};
            glGenBuffers(1, &blurBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, blurBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);
            GLuint skinVertex = 0;
            {
                std::string skin = skinnedVertex(kMainVertex);
#if defined(PS2X_NATIVE_VITA)
                // SOTC_GL_SKIN_PRECISE=1 (round 65c, testing): the skinning
                // vertex program compiled without libshacccg's fast math /
                // precision / int (a source of its own, so neither the cache
                // nor the VPK's seed hands back the fast binary).
                static const bool skinPrecise = std::getenv("SOTC_GL_SKIN_PRECISE") && std::atoi(std::getenv("SOTC_GL_SKIN_PRECISE")) != 0;
                static const bool fastShaders = !std::getenv("SOTC_SHADER_FAST") || std::atoi(std::getenv("SOTC_SHADER_FAST")) != 0;
                if (skinPrecise && !skin.empty())
                {
                    skin = "// precise\n" + skin;
                    vglSetupRuntimeShaderCompiler(SHARK_OPT_DEFAULT, 0, 0, 0);
                }
#endif
                skinVertex = skin.empty() ? 0 : compile(GL_VERTEX_SHADER, skin.c_str());
#if defined(PS2X_NATIVE_VITA)
                if (skinPrecise && fastShaders)
                    vglSetupRuntimeShaderCompiler(SHARK_OPT_FAST, 1, 1, 1);
                std::fprintf(stderr, "[native:gl] skinning program: %s\n", skinPrecise || !fastShaders ? "precise" : "fast");
#endif
                skinProgram = skin.empty() ? 0 : program(skinVertex, kMainFragment, {"aPos", "aNormal", "aColor", "aUV", "aBones", "aWeights"});
                if (!skinProgram)
                    std::cerr << "[native:gl] no skinning program: skinned meshes posed on the CPU" << std::endl;
            }
            mains[0].program = mainProgram;
            mains[1].program = skinProgram;
#if defined(PS2X_NATIVE_VITA)
            const int plainKinds = 2; // round 73: the NOLOD ones too
#else
            const int plainKinds = 1;
#endif
            for (int plain = 0; plain < plainKinds; ++plain)
                for (int skinned = 0; skinned < 2; ++skinned)
                    for (int bg = 0; bg < 2; ++bg)
                        for (int cut = 0; cut < 2; ++cut)
                        {
                            if (skinned && !skinProgram)
                                continue;
                            const std::string fragment = worldFragment(bg != 0, cut != 0, plain != 0);
                            mains[(plain ? kPlainSlot : 2) + skinned * 4 + bg * 2 + cut].program =
                                skinned ? program(skinVertex, fragment.c_str(), {"aPos", "aNormal", "aColor", "aUV", "aBones", "aWeights"})
                                        : program(mainVertex, fragment.c_str(), {"aPos", "aNormal", "aColor", "aUV"});
                        }
#if defined(PS2X_NATIVE_VITA)
            for (int skinned = 0; skinned < 2; ++skinned) // round 75: the cut-out without discard (not BG: see kWorldFragment)
            {
                if (skinned && !skinProgram)
                    continue;
                const std::string fragment = worldFragment(false, true, false, true);
                mains[kSoftSlot + skinned].program = skinned ? program(skinVertex, fragment.c_str(), {"aPos", "aNormal", "aColor", "aUV", "aBones", "aWeights"})
                                                             : program(mainVertex, fragment.c_str(), {"aPos", "aNormal", "aColor", "aUV"});
            }
#endif
            mains[kVolumeSlot].program = program(mainVertex, kVolumeFragment, {"aPos", "aNormal", "aColor", "aUV"});
            if (!mains[kVolumeSlot].program)
                std::cerr << "[native:gl] no shadow volume program: the volumes drawn with the main one" << std::endl;
            for (MainSlot &m : mains)
            {
                const GLuint p = m.program;
                if (!p)
                    continue;
                glUseProgram(p);
                const char *names[kMainUniforms] = {"uClip", "uDirections", "uColours", "uMisc", "uFogA", "uFogB", "uFogC", "uFogColour", "uDebug", "uAlpha", "uGuard"};
                for (int i = 0; i < kMainUniforms; ++i)
                    m.location[i] = glGetUniformLocation(p, names[i]);
                for (const auto &[name, unit] : {std::pair<const char *, int>{"uTex", 0}, {"uBackground", 1}, {"uReflection", 2}, {"uRefraction", 3}})
                {
                    const GLint at = glGetUniformLocation(p, name);
                    if (at >= 0)
                        glUniform1i(at, unit);
                }
                m.palette = glGetUniformLocation(p, "uPalette");
            }
            if (skinProgram && mains[1].palette < 0)
            {
                std::cerr << "[native:gl] the skinning program has no palette: skinned meshes posed on the CPU" << std::endl;
                skinProgram = 0;
                for (int i : {1, 6, 7, 8, 9, kPlainSlot + 4, kPlainSlot + 5, kPlainSlot + 6, kPlainSlot + 7, kSoftSlot + 1})
                    mains[i].program = 0;
            }
            glUseProgram(quadProgram);
            glUniform1i(uniform(false, "uSrc"), 0);
            glUniform1i(uniform(false, "uSrc2"), 1);
            glUniform1i(uniform(false, "uDepth"), 2);
            spike.on = std::getenv("SOTC_GL_SPIKE_LOG") && std::atoi(std::getenv("SOTC_GL_SPIKE_LOG")) != 0;
#if defined(PS2X_NATIVE_VITA)
            probeEmulator();
            if (native_renderer::tuning().warmShaders)
                warmShaders();
#endif
            return true;
        }

#if defined(PS2X_NATIVE_VITA)
        // Round 55: the two render targets made only at start-up (the
        // emulator probe's 16 x 16, cleared to magenta, and the shader
        // warm-up's 1 x 1) are kept for good instead of deleted: Vita3K keeps
        // a colour surface after the guest frees its memory and samples a
        // texture vitaGL later puts at that address, with the same row
        // stride, as that old surface (what made the attract demo's mist red:
        // the GS backend's 64 x 64 row probe, see gs_hw_backend.cpp). A few KB.
        // SOTC_GL_KEEP_TARGETS=0: deleted, as before.
        std::vector<Target> startTargets;
        static bool keepStartTargets()
        {
            static const bool on = !std::getenv("SOTC_GL_KEEP_TARGETS") || std::atoi(std::getenv("SOTC_GL_KEEP_TARGETS")) != 0;
            return on;
        }
        // A GXM fragment program carries its blend state: vitaGL patches a
        // program again when it is drawn with other blending, and the shader
        // patcher keeps each patched copy. The first draw of a program with a
        // blend state not seen before made that patch mid-game (a cut, a new
        // effect). Done here for every program and blend state the renderer
        // uses, into a 1 x 1 target (Tuning warmShaders, SOTC_GL_WARM).
        void warmShaders()
        {
            const uint64_t start = native_renderer::nowUs();
            Target t;
            makeTarget(t, 1, 1, true);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glViewport(0, 0, 1, 1);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_STENCIL_TEST);
            GLuint vbo = 0;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            const std::vector<uint8_t> zeros(3u * sizeof(native_renderer::SkinVertex), 0);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(zeros.size()), zeros.data(), GL_STATIC_DRAW);
            struct BlendState
            {
                bool on, mask;
                GLenum rgbEq, rgbSrc, rgbDst, aSrc, aDst;
            };
            // setBlend (with and without keepAlpha), quad()'s blends, the shadow volumes'.
            const BlendState blends[] = {
                {false, true, GL_FUNC_ADD, GL_ONE, GL_ZERO, GL_ONE, GL_ZERO},
                {true, true, GL_FUNC_ADD, GL_ONE, GL_ZERO, GL_ZERO, GL_ONE},
                {true, true, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ZERO},
                {true, true, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE},
                {true, true, GL_FUNC_REVERSE_SUBTRACT, GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ZERO},
                {true, true, GL_FUNC_REVERSE_SUBTRACT, GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE},
                {true, true, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO},
                {true, true, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE},
                {true, true, GL_FUNC_ADD, GL_ONE, GL_ONE, GL_ZERO, GL_ONE},
                {true, true, GL_FUNC_ADD, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE},
                {true, false, GL_FUNC_ADD, GL_ZERO, GL_ONE, GL_ZERO, GL_ONE},
            };
            uint32_t draws = 0;
            uint32_t programs = 0;
            for (int which = 0; which < 37; ++which)
            {
                // main, skin, the 12 quad programs, the 8 lean ones, the shadow volumes' (round 45),
                // the GS bloom chain's 15-18 (round 65b), the 8 NOLOD lean ones (round 73), the 2 SOFT ones (round 75)
                const GLuint p = which == 0 ? mainProgram
                                            : (which == 1 ? skinProgram
                                                          : (which < 14 ? quadPrograms[which - 2]
                                                                        : (which < 23 ? mains[2 + which - 14].program
                                                                                      : (which < 27 ? quadPrograms[which - 8]
                                                                                                    : (which < 35 ? mains[kPlainSlot + which - 27].program : mains[kSoftSlot + which - 35].program)))));
                if (!p)
                    continue;
                ++programs;
                glUseProgram(p);
                const bool skinned = which == 1 || (which >= 18 && which < 22) || (which >= 31 && which < 35) || which == 36, quadMode = (which >= 2 && which < 14) || (which >= 23 && which < 27);
                const GLsizei stride = skinned ? sizeof(native_renderer::SkinVertex) : sizeof(native_renderer::Vertex);
                for (GLuint i = 0; i < 6; ++i)
                {
                    const bool used = quadMode ? i == 0 : (i < 4 || skinned);
                    if (!used)
                    {
                        glDisableVertexAttribArray(i);
                        continue;
                    }
                    glEnableVertexAttribArray(i);
                    if (quadMode)
                        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
                    else if (i == 2)
                        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, stride, reinterpret_cast<const void *>(offsetof(native_renderer::Vertex, r)));
                    else
                        glVertexAttribPointer(i, i == 3 ? 2 : 3, GL_FLOAT, GL_FALSE, stride,
                                              reinterpret_cast<const void *>(i == 0 ? offsetof(native_renderer::Vertex, x)
                                                                             : i == 1 ? offsetof(native_renderer::Vertex, nx)
                                                                             : i == 3 ? offsetof(native_renderer::Vertex, u)
                                                                             : i == 4 ? offsetof(native_renderer::SkinVertex, bone)
                                                                                      : offsetof(native_renderer::SkinVertex, weight)));
                }
                for (const BlendState &b : blends)
                {
                    if (b.on)
                    {
                        glEnable(GL_BLEND);
                        glBlendEquationSeparate(b.rgbEq, GL_FUNC_ADD);
                        glBlendFuncSeparate(b.rgbSrc, b.rgbDst, b.aSrc, b.aDst);
                    }
                    else
                        glDisable(GL_BLEND);
                    const GLboolean m = b.mask ? GL_TRUE : GL_FALSE;
                    glColorMask(m, m, m, m);
                    glDrawArrays(GL_TRIANGLES, 0, 3);
                    ++draws;
                }
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            }
            glFinish();
            glDisable(GL_BLEND);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = boundProgram = ~0u;
            glDeleteBuffers(1, &vbo);
            if (keepStartTargets())
                startTargets.push_back(t); // kept: see keepStartTargets
            else
            {
                glDeleteFramebuffers(1, &t.fbo);
                glDeleteTextures(1, &t.texture);
                if (t.depth)
                    glDeleteTextures(1, &t.depth);
            }
            std::fprintf(stderr, "[native:gl] shaders warmed: %u draws (%u programs x %u blend states) in %.1f ms\n", draws, programs,
                         static_cast<unsigned>(sizeof(blends) / sizeof(blends[0])), (native_renderer::nowUs() - start) / 1000.0);
        }
#endif

#if defined(PS2X_NATIVE_VITA)
        // Vita3K: what the GPU draws into a texture never reaches the memory
        // the CPU reads (Vita3K keeps render targets on the host GPU); on the
        // console it does once the GPU is done. A small target cleared to a
        // colour, waited for and read back: not that colour, an emulator
        // (native_renderer::inEmulator; the mid-frame redraw's "only in
        // Vita3K"). SOTC_EMULATOR=0/1 says it instead.
        void probeEmulator()
        {
            if (const char *e = std::getenv("SOTC_EMULATOR"))
            {
                native_renderer::detail::setInEmulator(std::atoi(e) != 0);
                return;
            }
            Target t;
            makeTarget(t, 16, 16, false);
            glBindTexture(GL_TEXTURE_2D, t.texture);
            auto *pixels = static_cast<uint8_t *>(vglGetTexDataPointer(GL_TEXTURE_2D));
            if (!pixels)
                return;
            std::memset(pixels, 0, 16u * 16u * 4u);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glViewport(0, 0, 16, 16);
            glDisable(GL_SCISSOR_TEST);
            glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glFinish();
            uint32_t matching = 0;
            for (uint32_t i = 0; i < 16u * 16u; ++i)
                matching += pixels[i * 4u] > 200u && pixels[i * 4u + 1u] < 50u && pixels[i * 4u + 2u] > 200u ? 1u : 0u;
            const bool emulator = matching < 16u * 16u / 2u;
            native_renderer::detail::setInEmulator(emulator);
            std::fprintf(stderr, "[native:gl] render target read back: %u of 256 pixels drawn: %s\n", matching,
                         emulator ? "an emulator (Vita3K)" : "the console");
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = ~0u;
            if (keepStartTargets())
                startTargets.push_back(t); // kept: see keepStartTargets
            else
            {
                glDeleteFramebuffers(1, &t.fbo);
                glDeleteTextures(1, &t.texture);
            }
        }
#endif

        static GLuint newTexture(uint32_t w, uint32_t h, bool linear)
        {
            GLuint t = 0;
            glGenTextures(1, &t);
            glBindTexture(GL_TEXTURE_2D, t);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(w), static_cast<GLsizei>(h), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            return t;
        }

#if defined(PS2X_NATIVE_VITA)
        static bool texVerifyOn()
        {
            static const bool on = std::getenv("SOTC_GL_TEX_VERIFY") && std::atoi(std::getenv("SOTC_GL_TEX_VERIFY")) != 0;
            return on;
        }
        // The first level as vitaGL lays it out (rows 8-texel aligned).
        static uint64_t level0Hash(const Texture &t)
        {
            uint64_t h = 0xcbf29ce484222325ull;
            const uint32_t stride = ((t.level0W + 7u) & ~7u) * 4u;
            for (uint32_t y = 0; y < t.level0H; ++y)
            {
                const uint8_t *row = t.level0 + static_cast<size_t>(y) * stride;
                for (uint32_t x = 0; x < t.level0W * 4u; x += 4u)
                {
                    uint32_t v;
                    std::memcpy(&v, row + x, 4);
                    h = (h ^ v) * 0x100000001b3ull;
                }
            }
            return h;
        }
#endif
        static void makeTarget(Target &t, uint32_t w, uint32_t h, bool depth)
        {
            if (t.fbo)
                glDeleteFramebuffers(1, &t.fbo);
            if (t.texture)
                glDeleteTextures(1, &t.texture);
            if (t.depth)
                glDeleteTextures(1, &t.depth);
            if (t.stencil)
                glDeleteRenderbuffers(1, &t.stencil);
            t = Target{};
            t.width = w;
            t.height = h;
            t.texture = newTexture(w, h, true);
            glGenFramebuffers(1, &t.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t.texture, 0);
#if !defined(PS2X_NATIVE_VITA)
            if (depth)
            {
                // Depth = 1 - 0.1 / w, read back by the bloom mask and the depth of field.
                glGenTextures(1, &t.depth);
                glBindTexture(GL_TEXTURE_2D, t.depth);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, static_cast<GLsizei>(w), static_cast<GLsizei>(h), 0, GL_DEPTH_STENCIL,
                             GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, t.depth, 0);
            }
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "[native:gl] incomplete framebuffer " << w << "x" << h << std::endl;
#else
            // vitaGL gives every framebuffer a depth buffer of its own at its
            // first scene. Round 35: the targets made without depth (copies,
            // blurs, reductions, full-screen passes: no draw on them tests
            // depth or stencil, which our vitaGL counts in [vgl:dump]) share
            // one scratch depth per size instead (SOTC_GL_SCRATCH_DEPTH=0:
            // each its own, as before): ~1 MB of CDRAM each at 512 x 448.
            if (!depth && scratchDepthOn())
                vglFramebufferScratchDepth(t.fbo, GL_TRUE);
            if (std::getenv("SOTC_GL_LAYER_LOG")) // (debugging, round 55: where each target's colour lives)
            {
                glBindTexture(GL_TEXTURE_2D, t.texture);
                std::fprintf(stderr, "[gl:target] made %ux%u at %p (%u bytes)\n", w, h, vglGetTexDataPointer(GL_TEXTURE_2D), ((w + 7u) & ~7u) * 4u * h);
            }
#endif
        }
        // Round 38: vitaGL gives a framebuffer a stencil buffer only when a
        // depth-stencil renderbuffer is attached; without one the shadow
        // volumes counted on nothing and the darkening's stencil test never
        // passed: no shadow at all on the console (Vita3K keeps a stencil
        // anyway, so they showed there). The targets the world is drawn on
        // (the picture, the glow layer's and the effects' redrawn prefixes)
        // ask for one: vitaGL makes it (D32F + S8) at their first scene.
        // SOTC_GL_NO_STENCIL=1: as before. Elsewhere: nothing (the depth
        // texture is DEPTH32F_STENCIL8 already).
        static void wantStencil(Target &t)
        {
#if defined(PS2X_NATIVE_VITA)
            static const bool off = std::getenv("SOTC_GL_NO_STENCIL") != nullptr;
            if (off || t.stencil || !t.fbo)
                return;
            glGenRenderbuffers(1, &t.stencil);
            glBindRenderbuffer(GL_RENDERBUFFER, t.stencil);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height));
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, t.stencil);
#else
            (void)t;
#endif
        }
#if defined(PS2X_NATIVE_VITA)
        static bool scratchDepthOn()
        {
            static const bool on = !std::getenv("SOTC_GL_SCRATCH_DEPTH") || std::atoi(std::getenv("SOTC_GL_SCRATCH_DEPTH")) != 0;
            return on;
        }
        // Round 35: the shadow mask target is not drawn on the Vita (doShadow
        // returns before its first draw there): not made (SOTC_GL_SHADOWMASK_VITA=1: made and cleared, as before).
        static bool shadowMaskOn()
        {
            static const bool on = std::getenv("SOTC_GL_SHADOWMASK_VITA") && std::atoi(std::getenv("SOTC_GL_SHADOWMASK_VITA")) != 0;
            return on;
        }
#endif

        void ensureTargets(uint32_t w, uint32_t h)
        {
            if (color.texture && width == w && height == h)
                return;
#if defined(PS2X_NATIVE_VITA)
            if (color.texture)
            {
                // A new render scale, live (round 25: the console crashed on
                // it). vitaGL frees a texture's memory at once, not after the
                // GPU is done with it, when the texture was not *sampled* in
                // the last 4 frames (gpu_free_texture_data: tex->last_frame
                // only moves when a shader samples it, not when it is drawn
                // into): targets only drawn into (or sampled now and then)
                // went back to the pool while frames still in flight drew into
                // them, and the new, larger targets were made in that memory.
                // Wait for the GPU first: then every old target is idle.
                glFinish();
                std::fprintf(stderr, "[native:gl] picture %ux%u -> %ux%u (render scale changed)\n", width, height, w, h);
                // Then the picture-sized targets go first and their memory
                // comes back at once (our vitaGL's vglCollectGarbageNow: else
                // 4 frames later), so a larger picture never needs the old
                // and the new targets at the same time (2x: ~60 MB of them).
                auto release = [](Target &t, bool ownTexture = true)
                {
                    if (t.fbo)
                        glDeleteFramebuffers(1, &t.fbo);
                    if (ownTexture && t.texture)
                        glDeleteTextures(1, &t.texture);
                    if (ownTexture && t.depth)
                        glDeleteTextures(1, &t.depth);
                    if (ownTexture && t.stencil)
                        glDeleteRenderbuffers(1, &t.stencil);
                    t = Target{};
                };
                release(colorPlain, false); // (color's texture)
                for (Target *t : {&color, &background, &snapshot, &shadowMask, &previous, &shownFrame[0], &shownFrame[1], &glowLayer, &reflection,
                                  &prefix[0], &prefix[1], &prefix[2], &glowPrefix, &bloomTex[0], &bloomTex[1], &bloomTex[2]})
                    release(*t);
                release(blurOutPlain, false); // (blurOut's texture)
                release(blurOut);
                release(colorAltPlain, false); // (colorAlt's texture)
                release(colorAlt);
                boundFbo = boundProgram = ~0u;
                vglCollectGarbageNow();
            }
#endif
            makeTarget(color, w, h, true);
            wantStencil(color);
            makeTarget(background, w, h, true);
            makeTarget(snapshot, w, h, false);
#if defined(PS2X_NATIVE_VITA)
            if (shadowMaskOn())
#endif
                makeTarget(shadowMask, w, h, false);
            makeTarget(previous, w, h, false);
#if defined(PS2X_NATIVE_VITA)
            // Round 73: the second picture; the host's copies made at their
            // first use only when the picture is shown as drawn (glFewScenes).
            makeColorAlt(w, h);
            if (!native_renderer::tuning().glFewScenes)
#endif
                for (Target &t : shownFrame)
                    makeTarget(t, w, h, false);
            makeTarget(glowLayer, w, h, false);
            for (int i = 0; i < 4; ++i)
                makeTarget(glowSmall[i], i == 0 ? 256u : (i == 1 ? 128u : 64u), i == 0 ? 256u : (i == 1 ? 128u : 64u), false);
            makeTarget(backgroundSmall, 256, 256, false);
#if defined(PS2X_NATIVE_VITA)
            // Full size on the Vita: the 256 x 256 targets drawn mid-frame
            // did not hold there (the refraction copy came out white).
            makeTarget(reflection, w, h, true);
#else
            makeTarget(reflection, 256, 256, true);
#endif
            makeTarget(refraction, 256, 256, false);
            makeTarget(hazeSmall, 256, 256, false);
            for (Target &t : prefix)
                if (t.texture)
                    makeTarget(t, w, h, true), wantStencil(t);
            if (glowPrefix.texture)
                makeTarget(glowPrefix, w, h, true), wantStencil(glowPrefix);
            makeTarget(refractionHalf, 128, 128, false);
            for (Target &t : bloomTex)
                makeTarget(t, std::max(w / 4u, 1u), std::max(h / 4u, 1u), false);
            // The GS bloom chain's targets (the Vita too since round 65b).
            for (int i = 0; i < 7; ++i)
            {
                const uint32_t size = (i == 0 || i == 6) ? 256u : ((i == 1 || i == 5) ? 128u : 64u);
                makeTarget(gsBloom[i], size, size, false);
            }
            gsBloomHistory = false;
            if (colorPlain.fbo)
                glDeleteFramebuffers(1, &colorPlain.fbo);
            colorPlain = Target{};
            colorPlain.texture = color.texture;
            colorPlain.width = w;
            colorPlain.height = h;
            glGenFramebuffers(1, &colorPlain.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, colorPlain.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.texture, 0);
#if defined(PS2X_NATIVE_VITA)
            if (scratchDepthOn()) // drawn with full-screen passes only (round 35)
                vglFramebufferScratchDepth(colorPlain.fbo, GL_TRUE);
#endif
#if !defined(PS2X_NATIVE_VITA)
            // The shadow mask pass tests the main picture's stencil.
            glBindFramebuffer(GL_FRAMEBUFFER, shadowMask.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, color.depth, 0);
            // Layer 8 drawn apart on the picture's depth.
            glBindFramebuffer(GL_FRAMEBUFFER, glowLayer.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, color.depth, 0);
#endif
            if (blurOut.texture)
                makeBlurOut(w, h); // (the Vita's were let go above: made again at first use)
            bloomHistory = false;
            previousTexture = 0;
            width = w;
            height = h;
#if defined(PS2X_NATIVE_VITA)
            // Round 45: each target's first scene made now, once (vitaGL makes a
            // framebuffer's GXM render target and its depth at its first scene:
            // in the middle of a frame the first time an effect is drawn; the
            // console's two 155 ms draw spikes stood at an effect's layer after
            // a load). Cleared to nothing, waited for. SOTC_GL_WARM_TARGETS=0: off.
            static const bool warmTargets = !std::getenv("SOTC_GL_WARM_TARGETS") || std::atoi(std::getenv("SOTC_GL_WARM_TARGETS")) != 0;
            if (warmTargets)
            {
                const uint64_t start = native_renderer::nowUs();
                glDisable(GL_SCISSOR_TEST);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                glDepthMask(GL_TRUE);
                glStencilMask(0xFF);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClearDepthf(1.0f);
                glClearStencil(0);
                uint32_t made = 0;
                for (Target *t : {&color, &background, &snapshot, &shadowMask, &previous, &shownFrame[0], &shownFrame[1], &glowLayer, &glowSmall[0], &glowSmall[1],
                                  &glowSmall[2], &glowSmall[3], &backgroundSmall, &reflection, &refraction, &hazeSmall, &refractionHalf, &bloomTex[0], &bloomTex[1],
                                  &bloomTex[2], &colorPlain, &colorAlt, &colorAltPlain})
                    if (t->fbo)
                    {
                        glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);
                        glViewport(0, 0, static_cast<GLsizei>(t->width), static_cast<GLsizei>(t->height));
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                        ++made;
                    }
                glFinish();
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                boundFbo = boundProgram = ~0u;
                std::fprintf(stderr, "[native:gl] %u targets made at %ux%u (their first scene) in %.1f ms\n", made, w, h, (native_renderer::nowUs() - start) / 1000.0);
            }
            // Round 55 (SOTC_GL_LAYER_LOG): each target's colour memory.
            if (std::getenv("SOTC_GL_LAYER_LOG"))
            {
                const std::pair<const char *, const Target *> named[] = {
                    {"color", &color}, {"background", &background}, {"snapshot", &snapshot}, {"previous", &previous}, {"shown0", &shownFrame[0]},
                    {"shown1", &shownFrame[1]}, {"glowLayer", &glowLayer}, {"glowSmall0", &glowSmall[0]}, {"glowSmall1", &glowSmall[1]},
                    {"glowSmall2", &glowSmall[2]}, {"glowSmall3", &glowSmall[3]}, {"backgroundSmall", &backgroundSmall}, {"reflection", &reflection},
                    {"refraction", &refraction}, {"hazeSmall", &hazeSmall}, {"refractionHalf", &refractionHalf}, {"bloom0", &bloomTex[0]},
                    {"bloom1", &bloomTex[1]}, {"bloom2", &bloomTex[2]}};
                for (const auto &[name, t] : named)
                    if (t->texture)
                    {
                        glBindTexture(GL_TEXTURE_2D, t->texture);
                        std::fprintf(stderr, "[gl:target] %s %ux%u at %p (%u bytes)\n", name, t->width, t->height, vglGetTexDataPointer(GL_TEXTURE_2D),
                                     ((t->width + 7u) & ~7u) * 4u * t->height);
                    }
                for (const Target &t : startTargets)
                {
                    glBindTexture(GL_TEXTURE_2D, t.texture);
                    std::fprintf(stderr, "[gl:target] start %ux%u at %p\n", t.width, t.height, vglGetTexDataPointer(GL_TEXTURE_2D));
                }
            }
            // Round 35: which framebuffer is which ([vgl:dump] fb lines name them by address).
            if (std::getenv("SOTC_VGL_MEMDUMP"))
            {
                const std::pair<const char *, const Target *> named[] = {
                    {"color", &color}, {"colorPlain", &colorPlain}, {"background", &background}, {"snapshot", &snapshot}, {"shadowMask", &shadowMask},
                    {"previous", &previous}, {"shown0", &shownFrame[0]}, {"shown1", &shownFrame[1]}, {"glowLayer", &glowLayer},
                    {"glowSmall0", &glowSmall[0]}, {"glowSmall1", &glowSmall[1]}, {"glowSmall2", &glowSmall[2]}, {"glowSmall3", &glowSmall[3]},
                    {"backgroundSmall", &backgroundSmall}, {"reflection", &reflection}, {"refraction", &refraction}, {"hazeSmall", &hazeSmall},
                    {"prefix0", &prefix[0]}, {"prefix1", &prefix[1]}, {"prefix2", &prefix[2]}, {"glowPrefix", &glowPrefix},
                    {"refractionHalf", &refractionHalf}, {"bloom0", &bloomTex[0]}, {"bloom1", &bloomTex[1]}, {"bloom2", &bloomTex[2]}};
                std::string line = "[native:gl] targets:";
                char one[64];
                for (const auto &[name, t] : named)
                {
                    std::snprintf(one, sizeof(one), " %s=%08x", name, t->fbo);
                    line += one;
                }
                std::fprintf(stderr, "%s\n", line.c_str());
            }
#endif
        }

        // Blending as the Metal pipelines set it (alpha: source kept), or
        // with the destination's alpha kept (keepAlpha: the Vita bloom's mask
        // lives there; blending, not glColorMask, which vitaGL did not hold).
        static void setBlend(native_renderer::Blend blend, bool keepAlpha = false)
        {
            const GLenum alphaSrc = keepAlpha ? GL_ZERO : GL_ONE, alphaDst = keepAlpha ? GL_ONE : GL_ZERO;
            switch (blend)
            {
            case native_renderer::Blend::Opaque:
                if (!keepAlpha)
                {
                    glDisable(GL_BLEND);
                    return;
                }
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE);
                return;
            case native_renderer::Blend::Additive:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, alphaSrc, alphaDst);
                return;
            case native_renderer::Blend::Subtract:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, alphaSrc, alphaDst);
                return;
            default:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, alphaSrc, alphaDst);
                return;
            }
        }

        void bindTarget(const Target &t)
        {
            if (t.fbo == boundFbo)
                return;
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glViewport(0, 0, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height));
            boundFbo = t.fbo;
            ++stats.targets;
            gpu86::bind(t.fbo, t.width, t.height);
        }

        static void bindTexture(int unit, GLuint texture)
        {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        // A full-screen pass into `target` with the quad program in `mode`.
        enum class QuadBlend
        {
            None,
            Add,        // Cs + Cd, alpha kept
            Over,       // Cs x As + Cd x (1 - As), alpha kept
            Darken,     // Cd x (1 - As)
        };
        void quad(const Target &target, int mode, QuadBlend blend, const float a[4] = nullptr, const float b[4] = nullptr,
                  const float quadParams[4] = nullptr, bool corners = false)
        {
            gpu86::label("q%d", mode);
            bindTarget(target);
            useQuad(mode);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE); // vitaGL still writes depth with the test off (GXM: always pass)
            glDisable(GL_CULL_FACE);
            switch (blend)
            {
            case QuadBlend::None:
                glDisable(GL_BLEND);
                break;
            case QuadBlend::Add:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ONE);
                break;
            case QuadBlend::Over:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
                break;
            case QuadBlend::Darken:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
                break;
            }
#if defined(PS2X_NATIVE_VITA)
            glUniform1f(uniform(false, "uMode"), static_cast<float>(mode));
#else
            glUniform1i(uniform(false, "uMode"), macMode(mode));
#endif
            const float zero[4] = {0, 0, 0, 0};
            glUniform4fv(uniform(false, "uA"), 1, a ? a : zero);
            glUniform4fv(uniform(false, "uB"), 1, b ? b : zero);
            glUniform4fv(uniform(false, "uQuad"), 1, quadParams ? quadParams : zero);
            glBindBuffer(GL_ARRAY_BUFFER, corners ? blurBuffer : quadBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            for (GLuint i = 1; i < 4; ++i)
                glDisableVertexAttribArray(i);
            glDrawArrays(GL_TRIANGLES, 0, corners ? 6 : 3);
        }

        // The colour of `from` into `to` (bilinear: reducing blurs it).
        void copy(const Target &from, const Target &to)
        {
            bindTexture(0, from.texture);
            quad(to, 0, QuadBlend::None);
        }

        void snapshotColor() { copy(color, snapshot); }

        void blur(int rounds, float spreadGrow = 0.0f)
        {
            const float lw = static_cast<float>(bloomTex[0].width), lh = static_cast<float>(bloomTex[0].height);
            for (int i = 0; i < rounds; ++i)
            {
                const float spread = 1.0f + spreadGrow * static_cast<float>(i);
                const float k = spread * static_cast<float>(width) / 512.0f; // PS2 pixels: the texture grows with the render scale
                const float h[4] = {k / lw, 0, 0, 0}, v[4] = {0, k / lh, 0, 0};
                bindTexture(0, bloomTex[0].texture);
                quad(bloomTex[1], 1, QuadBlend::None, h);
                bindTexture(0, bloomTex[1].texture);
                quad(bloomTex[0], 1, QuadBlend::None, v);
            }
        }

        void render(uint32_t w, uint32_t h);
        void selfCheck(uint32_t w, uint32_t h)
        {
            native_renderer::Tuning &t = native_renderer::tuning();
            const native_renderer::Tuning saved = t;
            std::vector<uint8_t> a(static_cast<size_t>(w) * h * 4u), b(a.size());
            // SOTC_GL_SELFCHECK_OFF=<names> (round 45): the reference frame drawn
            // with these off instead of the merges and lean programs: fewcopies,
            // atmerge, volprog, depthkeep, merge, lean (comma separated).
            static const char *offList = std::getenv("SOTC_GL_SELFCHECK_OFF");
            if (offList)
            {
                const std::string l = std::string(",") + offList + ",";
                auto has = [&](const char *n) { return l.find(std::string(",") + n + ",") != std::string::npos; };
                t.glFewCopies = t.glFewCopies && !has("fewcopies");
                t.alphaTestMerge = t.alphaTestMerge && !has("atmerge");
                t.glVolumeProgram = t.glVolumeProgram && !has("volprog");
                t.glDepthKeep = t.glDepthKeep && !has("depthkeep");
                t.glMergeDraws = t.glMergeDraws && !has("merge");
                t.glLeanPrograms = t.glLeanPrograms && !has("lean");
            }
            else
                t.glMergeDraws = t.glLeanPrograms = false;
            render(w, h);
            glBindFramebuffer(GL_FRAMEBUFFER, colorPlain.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, a.data());
            t.glFewCopies = saved.glFewCopies;
            t.alphaTestMerge = saved.alphaTestMerge;
            t.glVolumeProgram = saved.glVolumeProgram;
            t.glDepthKeep = saved.glDepthKeep;
            t.glMergeDraws = saved.glMergeDraws;
            t.glLeanPrograms = saved.glLeanPrograms;
            const uint64_t calls = drawStats.calls;
            render(w, h);
            glBindFramebuffer(GL_FRAMEBUFFER, colorPlain.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, b.data());
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = ~0u;
            uint32_t differ = 0, worst = 0;
            for (size_t i = 0; i < a.size(); i += 4)
            {
                uint32_t d = 0;
                for (size_t c = 0; c < 3; ++c)
                    d = std::max<uint32_t>(d, static_cast<uint32_t>(std::abs(int(a[i + c]) - int(b[i + c]))));
                differ += d ? 1u : 0u;
                worst = std::max(worst, d);
            }
            static int dumps = std::getenv("SOTC_GL_SELFCHECK_DUMP") ? std::atoi(std::getenv("SOTC_GL_SELFCHECK_DUMP")) : 0;
            if (differ && dumps > 0)
            {
                --dumps;
                for (int k = 0; k < 2; ++k)
                {
                    char name[96];
                    std::snprintf(name, sizeof(name), "selfcheck_%llu_%c.ppm", (unsigned long long)arenaFrame, k ? 'b' : 'a');
                    if (FILE *f = std::fopen(name, "wb"))
                    {
                        std::fprintf(f, "P6\n%u %u\n255\n", w, h);
                        const std::vector<uint8_t> &img = k ? b : a;
                        for (uint32_t y = 0; y < h; ++y)
                            for (uint32_t x = 0; x < w; ++x)
                                std::fwrite(&img[(static_cast<size_t>(h - 1u - y) * w + x) * 4u], 1, 3, f);
                        std::fclose(f);
                    }
                }
            }
            std::fprintf(stderr, "[gl:selfcheck] frame %llu draws %zu calls %llu: %u of %u pixels differ (most %u)\n", (unsigned long long)arenaFrame, draws.size(),
                         (unsigned long long)(drawStats.calls - calls), differ, w * h, worst);
        }

        // The frame's shared vertex / index buffer filled (bound to `target`).
        // Round 65b, the Vita: glBufferData copies the data in with
        // sceClibMemcpy, which Vita3K runs on the host without its write
        // traps: a draw reading more than 12 KB of the buffer (Vita3K's
        // buffer trapping keeps such ranges by address and copies them again
        // only when the guest is seen writing them) got an older frame's
        // vertices and indices where vitaGL had put this one's at the same
        // address: thin white spikes out of Wander's and Agro's legs and body
        // (the skinned parts posed on the CPU and the shadow volumes are drawn
        // indexed from the start of this buffer); every frame junk with every
        // skinned part posed on the CPU (SOTC_GPU_SKIN=0). The storage is made
        // empty and written here with the CPU's own memcpy, which the traps
        // see (the same bytes). SOTC_GL_ARENA_CLIB=1: glBufferData's copy.
        static void uploadStream(GLenum target, const void *data, size_t bytes, GLenum usage = GL_STREAM_DRAW)
        {
#if defined(PS2X_NATIVE_VITA)
            static const bool clib = std::getenv("SOTC_GL_ARENA_CLIB") && std::atoi(std::getenv("SOTC_GL_ARENA_CLIB")) != 0;
            if (!clib)
            {
                glBufferData(target, static_cast<GLsizeiptr>(bytes), nullptr, usage);
                if (void *to = glMapBuffer(target, GL_WRITE_ONLY))
                {
                    std::memcpy(to, data, bytes);
                    glUnmapBuffer(target);
                    return;
                }
            }
#endif
            glBufferData(target, static_cast<GLsizeiptr>(bytes), data, usage);
        }

        // A mesh's triangles: the list as it is, or the indexed one's
        // indices (the frame's index buffer, bound with the frame's
        // vertices: the indices count from the start of the arena).
        void drawMesh(const Mesh &mesh, GLint firstVertex)
        {
            if (!mesh.indexed)
            {
                glDrawArrays(GL_TRIANGLES, firstVertex, static_cast<GLsizei>(mesh.count));
                return;
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.dynamic ? indexBuffers[arenaIndex] : mesh.indexBuffer);
            const uintptr_t first = mesh.dynamic ? mesh.firstIndex : 0u;
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.count), GL_UNSIGNED_INT, reinterpret_cast<const void *>(first * sizeof(uint32_t)));
        }
    };

    // Ids hash a model's or a texture's content: when the game loads another
    // area into the same memory, the old ones are never asked for again (the
    // title's demo filled the Vita's 160 MB in ~12 min). What no frame drew for
    // ~30 s goes; drawn again, the frontend uploads it anew (hasMesh false).
    void Renderer::evictUnused()
    {
#if defined(PS2X_NATIVE_VITA)
        static const bool texLog = true;
#else
        static const bool texLog = std::getenv("SOTC_GL_TEX_LOG") != nullptr;
#endif
        if (texLog && arenaFrame % 300 == 0)
        {
            uint64_t textureBytes = 0;
            textures.forEach([&](uint64_t, const Texture &t) { textureBytes += t.bytes; });
            std::fprintf(stderr, "[gl:tex] frame %llu missing draws %llu uploads %llu (swizzled %llu) gl errors %llu (last 0x%x) textures %u (%.1f MB)\n",
                         (unsigned long long)arenaFrame, (unsigned long long)texStats.missing, (unsigned long long)texStats.uploads,
                         (unsigned long long)texStats.swizzled, (unsigned long long)texStats.errors, texStats.lastError, static_cast<unsigned>(textures.size()),
                         textureBytes / 1048576.0);
            texStats.missing = texStats.uploads = texStats.errors = texStats.swizzled = 0;
        }
#if defined(PS2X_NATIVE_VITA)
        // SOTC_GL_TEX_VERIFY=1 (debugging, round 55): every 60 frames each
        // texture's first level hashed again where vitaGL keeps it: a change
        // is texture memory written by someone else since its upload.
        if (texVerifyOn() && arenaFrame % 60 == 0)
        {
            glFinish();
            uint64_t checked = 0, changed = 0;
            textures.forEach([&](uint64_t key, Texture &t)
            {
                if (!t.level0)
                    return;
                ++checked;
                const uint64_t h = level0Hash(t);
                if (h == t.level0Hash)
                    return;
                ++changed;
                static uint64_t printed = 0;
                if (printed++ < 40)
                    std::fprintf(stderr, "[gl:texverify] frame %llu texture %016llx %ux%u%s at %p: its first level changed since upload\n",
                                 (unsigned long long)arenaFrame, (unsigned long long)key, t.level0W, t.level0H, t.mipped ? " (mipped)" : "",
                                 static_cast<const void *>(t.level0));
                t.level0Hash = h; // (counted once per change)
            });
            // And each mipped texture's second level against its first (the
            // means of their texels): far apart, the level is not a reduction
            // of the first (memory nobody wrote).
            uint64_t mipped = 0, mipOff = 0;
            if (arenaFrame % 600 == 0)
                textures.forEach([&](uint64_t key, Texture &t)
                {
                    if (!t.level0 || !t.mipped || t.level0W < 2 || t.level0H < 2)
                        return;
                    ++mipped;
                    const uint32_t w0 = t.level0W, h0 = t.level0H, s0 = ((w0 + 7u) & ~7u) * 4u;
                    const uint32_t w1 = w0 / 2u, h1 = h0 / 2u, s1 = ((w1 + 7u) & ~7u) * 4u;
                    const uint8_t *l1 = t.level0 + static_cast<size_t>(s0) * h0;
                    double m0[4] = {}, m1[4] = {};
                    for (uint32_t y = 0; y < h0; ++y)
                        for (uint32_t x = 0; x < w0; ++x)
                            for (int c = 0; c < 4; ++c)
                                m0[c] += t.level0[static_cast<size_t>(y) * s0 + x * 4u + c];
                    for (uint32_t y = 0; y < h1; ++y)
                        for (uint32_t x = 0; x < w1; ++x)
                            for (int c = 0; c < 4; ++c)
                                m1[c] += l1[static_cast<size_t>(y) * s1 + x * 4u + c];
                    bool off = false;
                    for (int c = 0; c < 4; ++c)
                    {
                        m0[c] /= static_cast<double>(w0) * h0;
                        m1[c] /= static_cast<double>(w1) * h1;
                        off = off || std::fabs(m0[c] - m1[c]) > 24.0;
                    }
                    if (!off)
                        return;
                    ++mipOff;
                    static uint64_t printed = 0;
                    if (printed++ < 40)
                        std::fprintf(stderr, "[gl:texverify] frame %llu texture %016llx %ux%u: level 1 mean %.0f %.0f %.0f %.0f, level 0 %.0f %.0f %.0f %.0f\n",
                                     (unsigned long long)arenaFrame, (unsigned long long)key, w0, h0, m1[0], m1[1], m1[2], m1[3], m0[0], m0[1], m0[2], m0[3]);
                });
            if (arenaFrame % 600 == 0)
                std::fprintf(stderr, "[gl:texverify] frame %llu: %llu mipped textures, %llu with a second level unlike the first\n", (unsigned long long)arenaFrame,
                             (unsigned long long)mipped, (unsigned long long)mipOff);
            static uint64_t changedAll = 0;
            changedAll += changed;
            if (arenaFrame % 600 == 0)
                std::fprintf(stderr, "[gl:texverify] frame %llu: %llu textures checked, %llu changed now, %llu in all\n", (unsigned long long)arenaFrame,
                             (unsigned long long)checked, (unsigned long long)changed, (unsigned long long)changedAll);
        }
#endif
        constexpr uint64_t kEvery = 256, kUnused = 1800;
        static const bool log = std::getenv("SOTC_GL_CACHE_LOG") != nullptr;
        if (log && arenaFrame % 600 == 0)
        {
            uint64_t meshBytes = 0, textureBytes = 0;
            meshes.forEach([&](uint64_t, const Mesh &m) { meshBytes += m.bytes; });
            textures.forEach([&](uint64_t, const Texture &t) { textureBytes += t.bytes; });
            std::fprintf(stderr, "[gl:cache] frame %llu meshes %zu (%.1f MB) textures %zu (%.1f MB) arena %zu\n", (unsigned long long)arenaFrame,
                         meshes.size(), meshBytes / 1048576.0, textures.size(), textureBytes / 1048576.0, arena.size());
        }
        if (arenaFrame % kEvery != 0 || arenaFrame < kUnused)
            return;
        // Per-frame meshes only: the game thread never asks for them by id
        // (hasMesh), a later frame that makes one again makes it anew. The
        // kept meshes and the textures go at a hand-over (GlBackend::evictKept),
        // where the game thread's list of what is here changes with them.
        const uint64_t before = arenaFrame - kUnused;
        // Run copies no frame drew lately (a scene left, the camera turned).
        runs.eraseIf([&](const RunCopy &r)
        {
            if (r.used + 600u >= arenaFrame)
                return false;
            if (r.pool >= 0)
            {
                poolFree(r.pool, r.first, r.count);
                runVertices -= r.count;
            }
            return true;
        });
        meshes.eraseIf([&](const Mesh &m)
        {
            if (!m.dynamic || m.frame >= before)
                return false;
            if (m.buffer)
                glDeleteBuffers(1, &m.buffer);
            if (m.indexBuffer)
                glDeleteBuffers(1, &m.indexBuffer);
            if (m.pool >= 0)
                poolFree(m.pool, m.first, m.count);
            forget(m);
            return true;
        });
    }

    void Renderer::uploadMesh(uint64_t id, const native_renderer::Vertex *vertices, uint32_t count)
    {
        Mesh &mesh = meshes[id];
        forget(mesh);
        mesh.used = arenaFrame;
        mesh.dynamic = mesh.indexed = mesh.skinned = false;
        if (mesh.pool >= 0) // made again: the old range goes (the GPU may still read it)
        {
            poolFree(mesh.pool, mesh.first, mesh.count);
            mesh.pool = -1;
        }
        uint8_t minAlpha = 0xFF, maxAlpha = 0;
        float lo[3] = {INFINITY, INFINITY, INFINITY}, hi[3] = {-INFINITY, -INFINITY, -INFINITY};
        for (uint32_t i = 0; i < count; ++i)
        {
            minAlpha = std::min(minAlpha, vertices[i].a);
            maxAlpha = std::max(maxAlpha, vertices[i].a);
            const float v[3] = {vertices[i].x, vertices[i].y, vertices[i].z};
            for (int a = 0; a < 3; ++a)
                lo[a] = std::min(lo[a], v[a]), hi[a] = std::max(hi[a], v[a]);
        }
        mesh.minAlpha = count ? minAlpha : 0;
        mesh.maxAlpha = count ? maxAlpha : 0xFF;
        mesh.box = count && std::isfinite(lo[0] + lo[1] + lo[2] + hi[0] + hi[1] + hi[2]); // (NaN / inf positions: not known)
        std::memcpy(mesh.lo, lo, sizeof(lo));
        std::memcpy(mesh.hi, hi, sizeof(hi));
        int pool = -1;
        uint32_t first = 0;
        if (count && native_renderer::tuning().glMeshPools && poolAlloc(count, pool, first))
        {
            if (mesh.buffer)
            {
                glDeleteBuffers(1, &mesh.buffer);
                mesh.buffer = 0;
            }
            mesh.pool = pool;
            mesh.first = first;
            glBindBuffer(GL_ARRAY_BUFFER, pools[static_cast<size_t>(pool)].buffer);
            const size_t offset = static_cast<size_t>(first) * sizeof(native_renderer::Vertex), bytes = static_cast<size_t>(count) * sizeof(native_renderer::Vertex);
#if defined(PS2X_NATIVE_VITA)
            std::memcpy(pools[static_cast<size_t>(pool)].mapped + offset, vertices, bytes);
#else
            glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(bytes), vertices);
#endif
        }
        else
        {
            mesh.first = 0;
            if (!mesh.buffer)
                glGenBuffers(1, &mesh.buffer);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.buffer);
            uploadStream(GL_ARRAY_BUFFER, vertices, count * sizeof(native_renderer::Vertex), GL_STATIC_DRAW); // (round 65b, see there)
        }
        mesh.count = count;
        mesh.bytes = static_cast<uint64_t>(count) * sizeof(native_renderer::Vertex);
        meshBytes += mesh.bytes;
        cache.buffer = ~0u;
    }

    // First fit over the pools' free ranges; a new pool when none has room
    // (false: larger than a pool, the mesh gets a buffer of its own).
    bool Renderer::poolAlloc(uint32_t count, int &pool, uint32_t &first)
    {
        if (count > kPoolVertices)
            return false;
        for (size_t p = 0; p < pools.size(); ++p)
        {
            if (!pools[p].buffer || static_cast<int>(p) == poolDraining)
                continue;
            auto &free = pools[p].free;
            for (size_t i = 0; i < free.size(); ++i)
                if (free[i].second >= count)
                {
                    pool = static_cast<int>(p);
                    first = free[i].first;
                    free[i].first += count;
                    free[i].second -= count;
                    if (!free[i].second)
                        free.erase(free.begin() + static_cast<std::ptrdiff_t>(i));
                    return true;
                }
        }
        // A new pool (in a slot a pool let go left, if any).
        size_t slot = pools.size();
        for (size_t i = 0; i < pools.size(); ++i)
            if (!pools[i].buffer)
            {
                slot = i;
                break;
            }
        Pool p;
        p.capacity = kPoolVertices;
        glGenBuffers(1, &p.buffer);
        glBindBuffer(GL_ARRAY_BUFFER, p.buffer);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(static_cast<size_t>(kPoolVertices) * sizeof(native_renderer::Vertex)), nullptr, GL_STATIC_DRAW);
#if defined(PS2X_NATIVE_VITA)
        // vitaGL's buffer memory, mapped for good (glBufferSubData would copy
        // the whole buffer anew while the GPU uses it; only ranges no draw in
        // flight reads are written).
        p.mapped = static_cast<uint8_t *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        glUnmapBuffer(GL_ARRAY_BUFFER);
        if (!p.mapped)
        {
            glDeleteBuffers(1, &p.buffer);
            return false;
        }
#endif
        p.free.push_back({count, kPoolVertices - count});
        if (slot == pools.size())
            pools.push_back(std::move(p));
        else
            pools[slot] = std::move(p);
        pool = static_cast<int>(slot);
        first = 0;
        cache.buffer = ~0u;
        return true;
    }

    // Ranges let go long enough ago go back to their pool's free list
    // (kept in order, neighbours joined).
    void Renderer::poolRelease()
    {
        for (const PoolRange &r : deadRanges)
            poolFree(r.pool, r.first, r.count);
        deadRanges.clear();
        size_t kept = 0;
        for (const PoolRange &r : poolPending)
        {
            if (r.frame + kPoolReuseFrames > arenaFrame)
            {
                poolPending[kept++] = r;
                continue;
            }
            auto &free = pools[static_cast<size_t>(r.pool)].free;
            auto at = std::lower_bound(free.begin(), free.end(), std::make_pair(r.first, 0u));
            at = free.insert(at, {r.first, r.count});
            if (at + 1 != free.end() && at->first + at->second == (at + 1)->first)
            {
                at->second += (at + 1)->second;
                free.erase(at + 1);
            }
            if (at != free.begin() && (at - 1)->first + (at - 1)->second == at->first)
            {
                (at - 1)->second += at->second;
                free.erase(at);
            }
        }
        poolPending.resize(kept);
        // A pool with nothing left in it goes (a scene's meshes evicted):
        // its slot is taken again by the next new pool.
        for (Pool &pool : pools)
            if (pool.buffer && pool.free.size() == 1 && pool.free[0].first == 0 && pool.free[0].second == pool.capacity)
            {
                bool pending = false;
                for (const PoolRange &r : poolPending)
                    pending = pending || &pools[static_cast<size_t>(r.pool)] == &pool;
                if (pending)
                    continue;
                glDeleteBuffers(1, &pool.buffer);
                pool = Pool{};
                cache.buffer = ~0u;
            }
    }

    void Renderer::poolCompact()
    {
        static const bool on = !std::getenv("SOTC_GL_POOL_COMPACT") || std::atoi(std::getenv("SOTC_GL_POOL_COMPACT")) != 0;
        if (!on || !native_renderer::tuning().glMeshPools)
            return;
        constexpr uint32_t kMoveBudget = 0x4000u; // vertices a frame (~0.6 MB)
        if (poolDraining < 0)
        {
            if (arenaFrame % 30u != 0u)
                return;
            // Each pool's live vertices (capacity less its free and let-go ranges).
            std::vector<uint64_t> live(pools.size(), 0);
            uint64_t freeAll = 0;
            size_t count = 0;
            for (size_t p = 0; p < pools.size(); ++p)
                if (pools[p].buffer)
                {
                    ++count;
                    uint64_t f = 0;
                    for (const auto &r : pools[p].free)
                        f += r.second;
                    live[p] = pools[p].capacity - f;
                    freeAll += f;
                }
            for (const PoolRange &r : poolPending)
                if (r.pool >= 0 && static_cast<size_t>(r.pool) < live.size())
                    live[static_cast<size_t>(r.pool)] -= std::min<uint64_t>(live[static_cast<size_t>(r.pool)], r.count);
            if (count < 2u)
                return;
            int best = -1;
            for (size_t p = 0; p < pools.size(); ++p)
                if (pools[p].buffer && live[p] * 2u < pools[p].capacity && (best < 0 || live[p] < live[static_cast<size_t>(best)]))
                    best = static_cast<int>(p);
            if (best < 0)
                return;
            uint64_t freeElsewhere = freeAll;
            for (const auto &r : pools[static_cast<size_t>(best)].free)
                freeElsewhere -= r.second;
            // Room for its live vertices elsewhere, with a pool's worth to spare
            // (first fit over ranges: not every free vertex is usable).
            if (freeElsewhere < live[static_cast<size_t>(best)] + kPoolVertices / 4u)
                return;
            poolDraining = best;
        }
        Pool &from = pools[static_cast<size_t>(poolDraining)];
        if (!from.buffer)
        {
            poolDraining = -1;
            return;
        }
        uint32_t moved = 0;
        bool left = false, stuck = false;
        constexpr size_t stride = sizeof(native_renderer::Vertex);
        meshes.forEach([&](uint64_t, Mesh &m)
        {
            if (m.pool != poolDraining || m.dynamic || stuck)
                return;
            if (moved >= kMoveBudget)
            {
                left = true;
                return;
            }
            int pool = -1;
            uint32_t first = 0;
            if (!poolAlloc(m.count, pool, first))
            {
                stuck = true;
                return;
            }
#if defined(PS2X_NATIVE_VITA)
            std::memcpy(pools[static_cast<size_t>(pool)].mapped + static_cast<size_t>(first) * stride, from.mapped + static_cast<size_t>(m.first) * stride,
                        static_cast<size_t>(m.count) * stride);
#else
            glBindBuffer(GL_COPY_READ_BUFFER, from.buffer);
            glBindBuffer(GL_COPY_WRITE_BUFFER, pools[static_cast<size_t>(pool)].buffer);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, static_cast<GLintptr>(m.first) * stride, static_cast<GLintptr>(first) * stride,
                                static_cast<GLsizeiptr>(m.count) * stride);
#endif
            poolFree(poolDraining, m.first, m.count); // (the GPU may still read it: let go after kPoolReuseFrames)
            m.pool = pool;
            m.first = first;
            moved += m.count;
        });
        if (moved)
            cache.buffer = ~0u;
        poolMoved += moved;
        if (stuck)
        {
            poolDraining = -1; // no room elsewhere after all: left as it is
            return;
        }
        if (left)
            return;
        // Its meshes are out: its run copies go too (made again when drawn).
        runs.forEach([&](uint64_t, RunCopy &r)
        {
            if (r.pool != poolDraining)
                return;
            poolFree(r.pool, r.first, r.count);
            runVertices -= r.count;
            r.pool = -1;
            r.frames = 0;
        });
        ++poolsEmptied;
        poolDraining = -1; // poolRelease lets it go once its ranges are back
    }

    void Renderer::uploadSkinned(uint64_t id, const native_renderer::SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount)
    {
        Mesh &mesh = meshes[id];
        forget(mesh);
        mesh.used = arenaFrame;
        mesh.dynamic = false;
        mesh.skinned = true;
        mesh.indexed = true;
        mesh.box = false; // posed by its palette
        uint8_t minAlpha = 0xFF, maxAlpha = 0;
        for (uint32_t i = 0; i < count; ++i)
        {
            minAlpha = std::min(minAlpha, vertices[i].a);
            maxAlpha = std::max(maxAlpha, vertices[i].a);
        }
        mesh.minAlpha = count ? minAlpha : 0;
        mesh.maxAlpha = count ? maxAlpha : 0xFF;
        if (mesh.pool >= 0)
        {
            poolFree(mesh.pool, mesh.first, mesh.count);
            mesh.pool = -1;
        }
        if (!mesh.buffer)
            glGenBuffers(1, &mesh.buffer);
        if (!mesh.indexBuffer)
            glGenBuffers(1, &mesh.indexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffer);
        // (Round 65b: written by the CPU on the Vita, see uploadStream: a bind
        // pose at an address an evicted one had came out as the old one in Vita3K.)
        uploadStream(GL_ARRAY_BUFFER, vertices, count * sizeof(native_renderer::SkinVertex), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
        uploadStream(GL_ELEMENT_ARRAY_BUFFER, indices, indexCount * sizeof(uint32_t), GL_STATIC_DRAW);
        mesh.count = indexCount;
        mesh.bytes = static_cast<uint64_t>(count) * sizeof(native_renderer::SkinVertex) + static_cast<uint64_t>(indexCount) * 4u;
        meshBytes += mesh.bytes;
        skinnedBytes += mesh.bytes;
        cache.buffer = ~0u;
    }

    // Round 73: an RGBA8 chain (levels one after the other, rows packed) in
    // GXM's swizzled layout (Vita3K's encode_morton: y on the even bits, x on
    // the odd ones, the larger side's bits past the smaller's above), the
    // levels GXM takes (down to the smaller side at 1 texel) one after the
    // other. Returns their count. w and h powers of two.
    static uint32_t swizzleChain(const uint8_t *rgba, uint32_t w, uint32_t h, uint32_t levels, std::vector<uint8_t> &out)
    {
        uint32_t most = 1;
        for (uint32_t a = w, b = h; a > 1u && b > 1u; a /= 2u, b /= 2u)
            ++most;
        const uint32_t n = std::min(std::max(levels, 1u), most);
        size_t total = 0;
        for (uint32_t l = 0; l < n; ++l)
            total += static_cast<size_t>(std::max(w >> l, 1u)) * std::max(h >> l, 1u) * 4u;
        out.resize(total);
        auto spread = [](uint32_t x)
        {
            x &= 0x0000FFFFu;
            x = (x ^ (x << 8)) & 0x00FF00FFu;
            x = (x ^ (x << 4)) & 0x0F0F0F0Fu;
            x = (x ^ (x << 2)) & 0x33333333u;
            x = (x ^ (x << 1)) & 0x55555555u;
            return x;
        };
        std::vector<uint32_t> px, py;
        uint8_t *dst = out.data();
        const uint8_t *src = rgba;
        for (uint32_t l = 0, lw = w, lh = h; l < n; ++l)
        {
            const uint32_t least = std::min(lw, lh);
            const uint32_t k = static_cast<uint32_t>(__builtin_ctz(least));
            px.resize(lw);
            py.resize(lh);
            for (uint32_t x = 0; x < lw; ++x)
                px[x] = (spread(x & (least - 1u)) << 1) | ((x >> k) << (2u * k));
            for (uint32_t y = 0; y < lh; ++y)
                py[y] = spread(y & (least - 1u)) | ((y >> k) << (2u * k));
            for (uint32_t y = 0; y < lh; ++y)
            {
                const uint8_t *row = src + static_cast<size_t>(y) * lw * 4u;
                for (uint32_t x = 0; x < lw; ++x)
                    std::memcpy(dst + static_cast<size_t>(px[x] | py[y]) * 4u, row + static_cast<size_t>(x) * 4u, 4);
            }
            dst += static_cast<size_t>(lw) * lh * 4u;
            src += static_cast<size_t>(lw) * lh * 4u;
            lw = std::max(1u, lw / 2u);
            lh = std::max(1u, lh / 2u);
        }
        return n;
    }

    void Renderer::uploadTexture(uint64_t key, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels)
    {
        levels = std::max(1u, levels);
        Texture &t = textures[key];
        forget(t);
        ++texStats.uploads;
        t.used = arenaFrame;
        t.bytes = static_cast<uint64_t>(width) * height * 4u * (levels > 1u ? 4u : 3u) / 3u;
        if (!t.name)
            glGenTextures(1, &t.name);
        {
            // The least alpha (the opaque program variant: no texel cut out).
            size_t texels = 0;
            for (uint32_t level = 0, w = width, h = height; level < levels; ++level)
            {
                texels += static_cast<size_t>(w) * h;
                w = std::max(1u, w / 2u);
                h = std::max(1u, h / 2u);
            }
            // Round 45: and the greatest, up to 0x80 (A = 1): a draw whose
            // texels all fall under AREF draws every pixel without its depth
            // (the GS alpha test's FB_ONLY), one pass. Stops as soon as both
            // are settled (a cut-out texture: a 0 and a 0x80).
            uint8_t least = 0xFF, greatest = 0;
            for (size_t i = 0; i < texels && (least || greatest < 0x80u); ++i)
            {
                const uint8_t a = rgba[i * 4u + 3u];
                least = std::min(least, a);
                greatest = std::max(greatest, a);
            }
            t.minAlpha = least;
            t.maxAlpha = greatest;
        }
        glBindTexture(GL_TEXTURE_2D, t.name);
#if defined(PS2X_NATIVE_VITA)
        // Round 55: the texture's own mip levels (the PS2's), as given. vitaGL
        // took glTexImage2D's levels >= 1 as a request to downscale level 0
        // with an asynchronous GPU transfer, and Vita3K had uploaded the
        // chain by then (it hashes level 0 only): the levels below were
        // whatever that memory held (the attract demo's canyon mist red and
        // teal). SOTC_GL_OWN_MIPS=0: vitaGL's downscale, as before.
        static const bool ownMips = !std::getenv("SOTC_GL_OWN_MIPS") || std::atoi(std::getenv("SOTC_GL_OWN_MIPS")) != 0;
        vglCeloOwnMips(ownMips ? 1 : 0);
#endif
        // Round 73 (Tuning glSwizzle): the texels in GXM's swizzled layout
        // (the same texels; the GPU's fetches of nearby texels in both
        // directions fall in fewer memory bursts than in rows). Powers of two
        // only (the PS2's sizes are), with the texture's own levels.
        // SOTC_GL_SWIZZLE_BOTH=1: a swizzled copy beside the linear texture,
        // the A/B's `swizzle73` picking one each frame (twice the memory).
        bool swizzledDone = false;
        std::vector<uint8_t> chain;
        uint32_t swzLevels = 0;
#if defined(PS2X_NATIVE_VITA)
        static const bool swizzleBoth = std::getenv("SOTC_GL_SWIZZLE_BOTH") && std::atoi(std::getenv("SOTC_GL_SWIZZLE_BOTH")) != 0;
        const bool pow2 = width && height && !(width & (width - 1u)) && !(height & (height - 1u));
        const bool wantSwizzle = (swizzleBoth || native_renderer::tuning().glSwizzle) && pow2 && (ownMips || levels == 1u);
        if (wantSwizzle)
            swzLevels = swizzleChain(rgba, width, height, levels, chain);
        if (wantSwizzle && !swizzleBoth)
            swizzledDone = vglCeloTexImageSwizzled(static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLint>(swzLevels), chain.data());
        texStats.swizzled += swizzledDone ? 1u : 0u;
#endif
        t.swizzled = swizzledDone;
        if (!swizzledDone)
            for (uint32_t level = 0, w = width, h = height; level < levels; ++level)
            {
                glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), GL_RGBA, static_cast<GLsizei>(w), static_cast<GLsizei>(h), 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, rgba);
                if (const GLenum e = glGetError(); e != GL_NO_ERROR)
                {
                    ++texStats.errors;
                    texStats.lastError = e;
                }
                rgba += static_cast<size_t>(w) * h * 4u;
                w = std::max(1u, w / 2u);
                h = std::max(1u, h / 2u);
            }
        textureBytes += t.bytes;
#if !defined(PS2X_NATIVE_VITA)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(levels - 1u));
#else
        t.inHeap = inHostHeap(vglGetTexDataPointer(GL_TEXTURE_2D));
        if (t.inHeap)
            textureHeapBytes += t.bytes;
        if (texVerifyOn())
        {
            glFinish(); // (the level-0 memory as the CPU wrote it)
            t.level0 = static_cast<const uint8_t *>(vglGetTexDataPointer(GL_TEXTURE_2D));
            t.level0W = width;
            t.level0H = height;
            t.level0Hash = level0Hash(t);
        }
#endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels > 1u ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#if defined(PS2X_NATIVE_VITA)
        if (wantSwizzle && swizzleBoth) // the swizzled copy beside it, its parameters the same
        {
            if (!t.swz)
                glGenTextures(1, &t.swz);
            glBindTexture(GL_TEXTURE_2D, t.swz);
            if (vglCeloTexImageSwizzled(static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLint>(swzLevels), chain.data()))
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels > 1u ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                ++texStats.swizzled;
            }
            else
            {
                glDeleteTextures(1, &t.swz);
                t.swz = 0;
            }
            glBindTexture(GL_TEXTURE_2D, t.name);
        }
        else if (t.swz) // (made again without its copy)
        {
            glDeleteTextures(1, &t.swz);
            t.swz = 0;
        }
#endif
        t.clamp = 0;
        t.filter = 2;
        t.mipped = levels > 1u;
        cache.texture = ~0u;
    }

    void Renderer::applyUploadList(std::vector<Upload> &list)
    {
        for (const Upload &u : list)
        {
            switch (u.kind)
            {
            case Upload::MeshKind:
                uploadMesh(u.id, reinterpret_cast<const native_renderer::Vertex *>(u.bytes.data()), u.count);
                break;
            case Upload::SkinnedKind:
                uploadSkinned(u.id, reinterpret_cast<const native_renderer::SkinVertex *>(u.bytes.data()), u.count, u.indices.data(),
                              static_cast<uint32_t>(u.indices.size()));
                break;
            case Upload::TextureKind:
                uploadTexture(u.id, u.width, u.height, u.bytes.data(), u.levels);
                break;
            }
        }
        list.clear();
    }

    // The frame's GL work before its draws, in the order the game thread
    // asked for it: what eviction let go, the uploads, the per-frame meshes.
    void Renderer::applyUploads()
    {
        if (!deadBuffers.empty())
            glDeleteBuffers(static_cast<GLsizei>(deadBuffers.size()), deadBuffers.data());
        if (!deadTextures.empty())
            glDeleteTextures(static_cast<GLsizei>(deadTextures.size()), deadTextures.data());
        deadBuffers.clear();
        deadTextures.clear();
        poolRelease();
        poolCompact();
        applyUploadList(uploads);
        for (const FrameMesh &f : frameMeshes)
        {
            Mesh &mesh = meshes[f.id];
            if (mesh.pool >= 0)
            {
                poolFree(mesh.pool, mesh.first, mesh.count);
                mesh.pool = -1;
            }
            mesh.minAlpha = 0;
            mesh.maxAlpha = 0xFF;
            mesh.box = false;
            mesh.dynamic = true;
            mesh.frame = arenaFrame;
            mesh.first = f.first;
            mesh.skinned = false;
            mesh.indexed = f.indexed;
            mesh.firstIndex = f.firstIndex;
            mesh.count = f.count;
        }
        frameMeshes.clear();
    }

    // Two draws drawn as one: everything the passes read the same, the
    // meshes plain triangle lists in one buffer.
    // Round 56: the arrays' bits compared word by word inline (the library
    // memcmp's calls from here were ~4% of the Vita render thread in the
    // attract demo's heaviest scenes); the same answer as memcmp's == 0.
    template <size_t Bytes>
    static inline bool sameBits(const void *a, const void *b)
    {
        // (round 57: the size a constant, so the loop is unrolled with no
        // spills: planFrame's clip compare kept its bound on the stack)
        static_assert(Bytes % 4u == 0u, "whole words");
        const unsigned char *x = static_cast<const unsigned char *>(a), *y = static_cast<const unsigned char *>(b);
        uint32_t diff = 0;
        for (size_t i = 0; i < Bytes; i += 4)
        {
            uint32_t u, v;
            std::memcpy(&u, x + i, 4);
            std::memcpy(&v, y + i, 4);
            diff |= u ^ v;
        }
        return diff == 0;
    }

    static bool sameState(const Draw &a, const Draw &b);
    static bool sameDraw(const Draw &a, const Draw &b)
    {
        return sameState(a, b) && sameBits<sizeof(a.clip)>(a.clip, b.clip);
    }
    // Round 74 (Tuning glLeanPass): two floats differ unless their bits are
    // equal (no float compare, whose flag transfer stalls the A9) or they
    // compare equal (+0 and -0, as before). Bit-equal NaNs now count as the
    // same: the merged draw sends the same bits either way.
    static bool g_fastCompare = false;
    static_assert(offsetof(native_renderer::DrawState, maxLod) - offsetof(native_renderer::DrawState, opacity) == 5u * sizeof(float),
                  "opacity, uvOffset, lodK, lodScale, maxLod side by side");
    static inline bool sameFloats(const native_renderer::DrawState &x, const native_renderer::DrawState &y)
    {
        if (g_fastCompare && sameBits<6u * sizeof(float)>(&x.opacity, &y.opacity)) // the six as bits at once
            return true;
        return x.opacity == y.opacity && x.uvOffset[0] == y.uvOffset[0] && x.uvOffset[1] == y.uvOffset[1] && x.lodK == y.lodK && x.lodScale == y.lodScale &&
               x.maxLod == y.maxLod;
    }
    static bool sameState(const Draw &a, const Draw &b)
    {
        const native_renderer::DrawState &x = a.state, &y = b.state;
        if (x.texture != y.texture || x.clamp != y.clamp || x.blend != y.blend || x.order != y.order || !sameFloats(x, y) ||
            x.background != y.background || x.backgroundFge != y.backgroundFge || x.fogged != y.fogged || x.writeDepth != y.writeDepth || x.noDepthWrite != y.noDepthWrite ||
            x.overlay != y.overlay || x.screen != y.screen || x.screenDepthTest != y.screenDepthTest || x.shadowVolume || y.shadowVolume ||
            x.reflectionSource != y.reflectionSource || x.reflectionSample != y.reflectionSample || x.refractionSample != y.refractionSample ||
            x.fresnel != y.fresnel || x.screenCopy != y.screenCopy || x.previousFrame != y.previousFrame || x.debugColour != y.debugColour ||
            x.lit != y.lit || x.skinPalette || y.skinPalette)
            return false;
        if (!sameBits<sizeof(x.reflectionTint)>(x.reflectionTint, y.reflectionTint) || !sameBits<sizeof(x.mirrorRipple)>(x.mirrorRipple, y.mirrorRipple))
            return false;
        if ((x.lit || x.fresnel) && !sameBits<sizeof(x.light)>(x.light, y.light))
            return false;
        return true;
    }

    void Renderer::planFrame()
    {
        const size_t n = drawOrder.size();
        plan.assign(n, Planned{});
        const bool ahead = native_renderer::tuning().glLeanPass; // round 74: the next draws' records and lookups asked for ahead
        g_fastCompare = ahead;
        for (size_t k = 0; k < n; ++k)
        {
            if (ahead)
            {
                if (k + 8u < n)
                    __builtin_prefetch(&draws[static_cast<size_t>(drawOrder[k + 8u] & 0xFFFFFFFFu)]);
                if (k + 4u < n)
                {
                    const Draw &next = draws[static_cast<size_t>(drawOrder[k + 4u] & 0xFFFFFFFFu)];
                    meshes.prefetch(next.mesh);
                    if (next.state.texture)
                        textures.prefetch(next.state.texture);
                }
            }
            const Draw &d = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
            Planned &p = plan[k];
            Mesh *m = meshes.find(d.mesh);
            if (m && m->count)
            {
                m->used = arenaFrame;
                p.mesh = m;
                p.minAlpha = m->dynamic ? 0 : m->minAlpha;
                p.maxAlpha = m->dynamic ? 0xFF : m->maxAlpha;
                if (d.state.background && m->box && !m->dynamic && !m->skinned)
                {
                    // Round 75: the least clip w over the box (c.w is linear in
                    // the position: least at the corner each axis picks by its sign).
                    const float *c = d.clip;
                    float w = c[15];
                    for (int a = 0; a < 3; ++a)
                        w += c[4 * a + 3] * (c[4 * a + 3] >= 0.0f ? m->lo[a] : m->hi[a]);
                    p.minW = std::isfinite(w) ? w : -1.0f;
                }
            }
            if (d.state.texture)
                if (Texture *t = textures.find(d.state.texture))
                {
                    t->used = arenaFrame;
                    p.tex = t;
                }
            // Round 45: a frame's own mesh drawn with the GS alpha test, whose
            // texture's least alpha cannot fail it (or untextured): its least
            // and greatest vertex alpha read from the frame's arena once (on
            // the console's route ~65% of the second passes were on meshes of
            // unknown alpha). Only these meshes, once a frame each.
            if (m && m->dynamic && !m->indexed && m->count && native_renderer::tuning().alphaTestMerge && native_renderer::tuning().alphaTest)
            {
                const uint32_t at = d.state.alphaTest;
                if ((at & 1u) && !d.state.fresnel && (!d.state.texture || p.tex))
                {
                    const float aref = static_cast<float>((at >> 4) & 0xFFu) / 128.0f;
                    const bool texOk = !p.tex || static_cast<float>(p.tex->minAlpha) / 255.0f * 2.0f >= aref || p.tex->maxAlpha != 0xFFu;
                    if (texOk && m->alphaFrame != arenaFrame && static_cast<size_t>(m->first) + m->count <= arena.size())
                    {
                        const native_renderer::Vertex *v = arena.data() + m->first;
                        uint8_t lo = 0xFF, hi = 0;
                        for (uint32_t i = 0; i < m->count; ++i)
                        {
                            lo = std::min(lo, v[i].a);
                            hi = std::max(hi, v[i].a);
                        }
                        m->alphaFrame = arenaFrame;
                        m->frameMinAlpha = lo;
                        m->frameMaxAlpha = hi;
                    }
                    if (m->alphaFrame == arenaFrame)
                    {
                        p.minAlpha = m->frameMinAlpha;
                        p.maxAlpha = m->frameMaxAlpha;
                    }
                }
            }
        }
        if (native_renderer::tuning().glSort87 && native_renderer::tuning().glMergeDraws)
            sortFree();
        // Round 87 (SOTC_PASS87_LOG=<n>): every n frames, the frame's draws by
        // layer: count, vertices, blends (opaque / normal / additive /
        // subtract), screen-space draws, the full program's kinds (2 mirror,
        // 3 water, 4 screen copy, 5 last frame), volumes, alpha-tested,
        // background: what each segment of the picture carries.
        static const int passLog = std::getenv("SOTC_PASS87_LOG") ? std::atoi(std::getenv("SOTC_PASS87_LOG")) : 0;
        if (passLog > 0 && arenaFrame % static_cast<uint64_t>(passLog) == 0u)
        {
            struct L
            {
                unsigned draws = 0, verts = 0, blend[4] = {}, screen = 0, kind[6] = {}, vol = 0, at = 0, bg = 0, skinned = 0;
            };
            std::map<uint32_t, L> layers;
            for (size_t k = 0; k < n; ++k)
            {
                const Draw &d = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
                L &l = layers[d.state.order];
                ++l.draws;
                l.verts += plan[k].mesh ? plan[k].mesh->count : 0u;
                ++l.blend[std::min<unsigned>(static_cast<unsigned>(d.state.blend), 3u)];
                l.screen += d.state.screen || d.state.screenDepthTest;
                const int kind = d.state.previousFrame ? 5 : d.state.reflectionSample ? 2 : d.state.refractionSample ? 3 : d.state.screenCopy ? 4 : 0;
                ++l.kind[kind];
                l.vol += d.state.shadowVolume;
                l.at += (d.state.alphaTest & 1u) ? 1u : 0u;
                l.bg += d.state.background;
                l.skinned += plan[k].mesh && plan[k].mesh->skinned;
            }
            std::string line = "[pass87] frame " + std::to_string(arenaFrame) + ":";
            for (const auto &[order, l] : layers)
            {
                char b[200];
                std::snprintf(b, sizeof b, " L%u d%u v%u b%u/%u/%u/%u s%u k%u/%u/%u/%u vol%u at%u bg%u sk%u;", order, l.draws, l.verts, l.blend[2], l.blend[0], l.blend[1], l.blend[3],
                              l.screen, l.kind[2], l.kind[3], l.kind[4], l.kind[5], l.vol, l.at, l.bg, l.skinned);
                line += b;
            }
            std::fprintf(stderr, "%s\n", line.c_str());
        }
        {
            const double now = std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
            if (shadowStats.since == 0.0)
                shadowStats.since = now;
            if (++shadowStats.frames % 300u == 0u)
            {
                const double secs = std::max(now - shadowStats.since, 1e-3);
                std::fprintf(stderr, "[shadow] %llu frames in %.1f s: volumes %.1f/s (%.0f vertices/s), darken passes %.1f/s, frames with volumes %llu; "
                                     "body shadows %d, shadows %d; alpha-test second passes %.1f/s (%.1f/s on meshes of unknown least alpha), %.1f/s drawn in one pass (all under AREF); "
                                     "r87: volumes ISP-only %.1f/s, masks without the break %.1f/s, background at 256 %.1f/s\n",
                             (unsigned long long)shadowStats.frames, secs, shadowStats.volumes / secs, shadowStats.vertices / secs, shadowStats.darkens / secs,
                             (unsigned long long)shadowStats.framesWithVolumes, native_renderer::tuning().bodyShadows ? 1 : 0, native_renderer::tuning().shadows ? 1 : 0,
                             shadowStats.alphaSecond / secs, alphaStats.secondUnknown / secs, alphaStats.merged / secs, shadowStats.ispVolumes / secs, shadowStats.maskFused / secs, (bgSmallFrames - bgSmallLogged) / secs);
                bgSmallLogged = bgSmallFrames;
                alphaStats = {};
                const uint64_t last = shadowStats.lastFrame;
                shadowStats = {};
                shadowStats.since = now;
                shadowStats.lastFrame = last;
            }
        }
        static const bool log = std::getenv("SOTC_GL_DRAW_LOG") != nullptr;
        if (log)
        {
            drawStats.draws += n;
            if (++drawStats.frames % 300u == 0u)
            {
                uint64_t free = 0;
                for (const Pool &pool : pools)
                    for (const auto &range : pool.free)
                        free += range.second;
                for (const PoolRange &r : poolPending)
                    free += r.count;
                unsigned live = 0;
                for (const Pool &pool : pools)
                    live += pool.buffer ? 1u : 0u;
                std::fprintf(stderr, "[gl:pools] %u pools of %u vertices: %.1f%% free; meshes %u (%.1f MB)\n", live, kPoolVertices,
                             live ? 100.0 * free / (live * double(kPoolVertices)) : 0.0, static_cast<unsigned>(meshes.size()), meshBytes / 1048576.0);
                const double f = 300.0;
                std::fprintf(stderr, "[gl:draws] per frame: draws %.0f, GL draw calls %.0f (all passes), merged away %.0f in %.0f ranges + %.0f run copies, "
                                     "no-discard %.0f, discard %.0f (%.0f writing depth), soft cut %.0f, vertex buffer switches %.0f | pools %u, run copies %u (%.0f KB) | program off %.0f, none %.0f\n",
                             drawStats.draws / f, drawStats.calls / f, drawStats.merged / f, drawStats.ranges / f, drawStats.indexed / f,
                             drawStats.noDiscard / f, drawStats.cut / f, drawStats.cutDepth / f, drawStats.soft / f, drawStats.bufferSwitches / f, static_cast<unsigned>(pools.size()), static_cast<unsigned>(runs.size()),
                             runVertices * sizeof(native_renderer::Vertex) / 1024.0, drawStats.progOff / f, drawStats.progZero / f);
                std::fprintf(stderr, "[gl:cut] per frame: vertices through discard %.0f, soft cut %.0f, no cut %.0f, the full program %.0f | the full program's calls by textured 0-5: %.0f %.0f %.0f %.0f %.0f %.0f | depth-writing discards: alpha test %.0f, layer 1's near plane %.0f, least alpha not known: frame mesh %.0f, fresnel %.0f, other %.0f; under 0.03 %.0f | layer 1's near plane: %.0f draws, %.0f wholly past it\n",
                             drawStats.vertsCut / f, drawStats.vertsSoft / f, drawStats.vertsPlain / f, drawStats.vertsMain / f, drawStats.mainKind[0] / f,
                             drawStats.mainKind[1] / f, drawStats.mainKind[2] / f, drawStats.mainKind[3] / f, drawStats.mainKind[4] / f, drawStats.mainKind[5] / f,
                             drawStats.cutWhy[0] / f, drawStats.cutWhy[1] / f, drawStats.cutWhy[2] / f, drawStats.cutWhy[3] / f, drawStats.cutWhy[4] / f, drawStats.cutWhy[5] / f, drawStats.guarded / f, drawStats.guardOff / f);
                const uint64_t frames = drawStats.frames;
                drawStats = {};
                drawStats.frames = frames;
            }
        }
        if (!native_renderer::tuning().glMergeDraws)
            return;
        // SOTC_GL_MERGE_MODE (debugging): bit 0 meshes side by side, bit 1 run copies, bit 2 the frame's own meshes.
        static const int mode = std::getenv("SOTC_GL_MERGE_MODE") ? std::atoi(std::getenv("SOTC_GL_MERGE_MODE")) : 7;
        auto plainMesh = [](const Mesh *m) { return m && !m->skinned && !m->indexed && ((m->dynamic && (mode & 4)) || m->pool >= 0); };
        for (size_t k = 0; k < n;)
        {
            const Draw &lead = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
            const Mesh *m = plan[k].mesh;
            size_t end = k + 1;
            uint32_t total = m ? m->count : 0u;
            if (plainMesh(m))
                while (end < n)
                {
                    const Mesh *o = plan[end].mesh;
                    // One range: the frame's own meshes together, the kept ones
                    // together; at most what vitaGL's glDrawArrays takes (0xC000).
                    if (!plainMesh(o) || o->dynamic != m->dynamic || total + o->count > 0xC000u || plan[end].tex != plan[k].tex ||
                        !sameDraw(lead, draws[static_cast<size_t>(drawOrder[end] & 0xFFFFFFFFu)]))
                        break;
                    total += o->count;
                    ++end;
                }
            if (end - k > 1)
            {
                Planned &p = plan[k];
                bool contiguous = true;
                uint32_t next = m->first;
                uint64_t key = 0x9E3779B97F4A7C15ull;
                for (size_t j = k; j < end; ++j)
                {
                    const Mesh *o = plan[j].mesh;
                    contiguous = contiguous && o->first == next && o->pool == m->pool;
                    next = o->first + o->count;
                    p.minAlpha = std::min(p.minAlpha, plan[j].minAlpha);
                    p.maxAlpha = std::max(p.maxAlpha, plan[j].maxAlpha);
                    p.minW = std::min(p.minW, plan[j].minW); // (-1 if any is not known)
                    key = (key ^ draws[static_cast<size_t>(drawOrder[j] & 0xFFFFFFFFu)].mesh) * 0xff51afd7ed558ccdull;
                    key ^= key >> 29;
                }
                if (!contiguous)
                {
                    // Kept meshes apart: drawn from a copy of the run.
                    RunCopy *copy = nullptr;
                    if (!m->dynamic && (mode & 2))
                    {
                        copy = &runs[key];
                        if (copy->used != arenaFrame) // (the same run twice in a frame counts once)
                            copy->frames = copy->used + 1u == arenaFrame ? copy->frames + 1u : 1u;
                        copy->used = arenaFrame;
                        // SOTC_GL_RUN_FRAMES=<n>: frames in a row before the copy (kRunFrames).
                        static const uint32_t runFrames = std::getenv("SOTC_GL_RUN_FRAMES") ? std::max(1, std::atoi(std::getenv("SOTC_GL_RUN_FRAMES"))) : kRunFrames;
                        if (copy->pool < 0 && copy->frames >= runFrames && !copyRun(k, end, total, *copy))
                            copy->frames = 0; // no room: tried again later
                    }
                    if (!copy || copy->pool < 0 || copy->count != total)
                    {
                        k = end; // drawn one by one
                        continue;
                    }
                    p.runPool = copy->pool;
                    p.runFirst = copy->first;
                }
                else if (!(mode & 1))
                {
                    k = end;
                    continue;
                }
                p.mergedCount = total;
                p.run = static_cast<uint32_t>(end - k);
                for (size_t j = k + 1; j < end; ++j)
                    plan[j].run = 0;
            }
            k = end;
        }
        mergeStats();
    }

    // Round 87 (Tuning glSort87): within each layer, the draws that may go in
    // any order are put in texture order between the ones that keep their
    // place. A draw is free when nothing it draws depends on what was drawn
    // before it except through the depth test: opaque (Blend::Opaque, or
    // Normal with a least alpha of 1: Cs x 1 + Cd x 0), depth tested and
    // written, no alpha test that can fail (its second pass blends), not an
    // overlay / screen / volume / mirror / water / copy draw. Free draws
    // never move past a draw that keeps its place, so every blended draw sees
    // the same draws before it. Two free draws meeting at exactly the same
    // depth keep the later one (LEQUAL), so there the other may show: the
    // only change. Key: the texture, then the state and matrix (sameDraw's
    // fields, hashed), then where the mesh lies (pool, first vertex): the
    // merges need neighbours with the same texture, state and matrix, and a
    // contiguous range or a run copy (made once a run repeats: the order is
    // the same each frame for the same draws).
    void Renderer::sortFree()
    {
        const size_t n = drawOrder.size();
        if (n < 3)
            return;
        sortKeys.clear();
        sortStats.frames++;
        auto freeDraw = [&](size_t k) -> bool
        {
            const Planned &p = plan[k];
            const Mesh *m = p.mesh;
            if (!m || !m->count)
                return false;
            const native_renderer::DrawState &st = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)].state;
            // Layer 1's far scenery and sky meet at the far plane (their
            // depths round to 1.0 there): ties everywhere, so they keep their
            // order (a frozen frame showed a 2 x 2 pixel speck of far landscape
            // swapped with them sorted).
            if (st.background || st.overlay || st.screen || st.screenDepthTest || st.noDepthWrite || st.shadowVolume || st.reflectionSource || st.reflectionSample ||
                st.refractionSample || st.screenCopy || st.previousFrame || st.fresnel || st.debugColour)
                return false;
            if (st.texture && !p.tex)
                return false;
            float least = -1.0f; // alpha = texel a x 2 x vertex a / 128 x opacity, at least
            if (p.minAlpha)
            {
                least = static_cast<float>(p.minAlpha) / 128.0f * st.opacity;
                if (p.tex)
                    least *= static_cast<float>(p.tex->minAlpha) / 255.0f * 2.0f;
            }
            if (st.blend == native_renderer::Blend::Normal)
            {
                if (!(least >= 1.0f))
                    return false;
            }
            else if (st.blend != native_renderer::Blend::Opaque)
                return false;
            const uint32_t at = st.alphaTest;
            if (at & 1u)
            {
                const float aref = static_cast<float>((at >> 4) & 0xFFu) / 128.0f;
                if (!(least >= aref))
                    return false;
            }
            return true;
        };
        auto hashState = [&](const Draw &d) -> uint64_t
        {
            const native_renderer::DrawState &x = d.state;
            uint64_t h = 0xcbf29ce484222325ull;
            auto mix = [&h](uint64_t v) { h = (h ^ v) * 0x100000001b3ull; h ^= h >> 31; };
            uint32_t words[16];
            std::memcpy(words, d.clip, sizeof words);
            for (uint32_t w : words)
                mix(w);
            uint32_t f[6];
            std::memcpy(f, &x.opacity, sizeof f);
            for (uint32_t w : f)
                mix(w);
            mix(static_cast<uint64_t>(x.clamp) | static_cast<uint64_t>(x.blend) << 8 | static_cast<uint64_t>(x.background) << 16 | static_cast<uint64_t>(x.backgroundFge) << 17 |
                static_cast<uint64_t>(x.fogged) << 18 | static_cast<uint64_t>(x.writeDepth) << 19 | static_cast<uint64_t>(x.lit) << 20 |
                static_cast<uint64_t>(x.skinPalette) << 32);
            if (x.lit)
            {
                uint32_t l[32];
                std::memcpy(l, x.light, sizeof l);
                for (uint32_t w : l)
                    mix(w);
            }
            return h;
        };
        size_t k = 0;
        while (k < n)
        {
            const uint64_t layer = drawOrder[k] >> 32;
            if (!freeDraw(k))
            {
                ++k;
                continue;
            }
            size_t end = k + 1;
            while (end < n && (drawOrder[end] >> 32) == layer && freeDraw(end))
                ++end;
            if (end - k >= 3)
            {
                sortKeys.clear();
                for (size_t j = k; j < end; ++j)
                {
                    const Draw &d = draws[static_cast<size_t>(drawOrder[j] & 0xFFFFFFFFu)];
                    const Mesh *m = plan[j].mesh;
                    const uint64_t where = (m->skinned ? 1ull << 63 : 0ull) | (m->indexed ? 1ull << 62 : 0ull) | (m->dynamic ? 1ull << 61 : 0ull) |
                                           (static_cast<uint64_t>(static_cast<uint32_t>(m->pool + 1) & 0xFFFFu) << 32) | m->first;
                    sortKeys.push_back({d.state.texture, hashState(d), where, static_cast<uint32_t>(j)});
                }
                std::sort(sortKeys.begin(), sortKeys.end(), [](const auto &a, const auto &b)
                {
                    if (a.tex != b.tex)
                        return a.tex < b.tex;
                    if (a.state != b.state)
                        return a.state < b.state;
                    if (a.where != b.where)
                        return a.where < b.where;
                    return a.k < b.k;
                });
                sortOrder.clear();
                sortPlan.clear();
                for (const auto &key : sortKeys)
                {
                    sortOrder.push_back(drawOrder[key.k]);
                    sortPlan.push_back(plan[key.k]);
                }
                for (size_t j = k; j < end; ++j)
                {
                    sortStats.moved += sortOrder[j - k] != drawOrder[j];
                    drawOrder[j] = sortOrder[j - k];
                    plan[j] = sortPlan[j - k];
                }
                sortStats.free += end - k;
                ++sortStats.segments;
            }
            k = end;
        }
        sortStats.draws += n;
        static const bool log = std::getenv("SOTC_GL_DRAW_LOG") != nullptr || std::getenv("SOTC_SORT87_LOG") != nullptr;
        if (log && sortStats.frames % 300u == 0u)
        {
            const double f = 300.0;
            std::fprintf(stderr, "[sort87] per frame: draws %.0f, free %.0f in %.0f segments, moved %.0f\n", sortStats.draws / f, sortStats.free / f,
                         sortStats.segments / f, sortStats.moved / f);
            sortStats = {};
        }
    }

    // Round 74 (SOTC_GL_MERGE_STATS=1, analysis): why two draws next to each
    // other in the frame's order were not drawn as one, counted a frame and
    // printed every 300 frames ([gl:merge]).
    void Renderer::mergeStats()
    {
        static const bool on = std::getenv("SOTC_GL_MERGE_STATS") && std::atoi(std::getenv("SOTC_GL_MERGE_STATS")) != 0;
        if (!on)
            return;
        enum
        {
            kLeaders, kSkinned, kIndexed, kVolume, kDynamic, kNoMesh, rVolume, rSkinSame, rSkinPalette, rSkinOther, rIndexed, rDynMix, rTex, rClipSameMesh, rClip,
            rState, rCap, rApart, rLayer, kCount
        };
        static const char *const names[kCount] = {"leaders", "skinned", "indexed", "volume", "dynamic", "nomesh", "|vol", "skin-sameall-but-tex/mesh",
                                                  "skin-palette", "skin-other", "indexed", "dyn/kept", "tex", "clip(same mesh)", "clip", "state", "cap", "apart", "layer"};
        static uint64_t counts[kCount] = {};
        static uint64_t frames = 0;
        const size_t n = drawOrder.size();
        const Draw *prev = nullptr;
        const Planned *prevPlan = nullptr;
        for (size_t k = 0; k < n; ++k)
        {
            const Planned &p = plan[k];
            const Draw &d = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
            if (!p.mesh)
            {
                ++counts[kNoMesh];
                continue;
            }
            if (!p.run)
            {
                prev = &d;
                prevPlan = &p;
                continue;
            }
            ++counts[kLeaders];
            counts[kSkinned] += p.mesh->skinned;
            counts[kIndexed] += p.mesh->indexed && !p.mesh->skinned;
            counts[kVolume] += d.state.shadowVolume;
            counts[kDynamic] += p.mesh->dynamic;
            if (prev)
            {
                const Mesh *a = prevPlan->mesh, *b = p.mesh;
                int r;
                if (prev->state.order != d.state.order)
                    r = rLayer;
                else if (prev->state.shadowVolume || d.state.shadowVolume)
                    r = rVolume;
                else if (a->skinned || b->skinned)
                {
                    if (a->skinned && b->skinned)
                    {
                        native_renderer::DrawState x = prev->state, y = d.state;
                        const bool palette = x.skinPalette == y.skinPalette && x.skinPaletteCount == y.skinPaletteCount;
                        x.skinPalette = y.skinPalette = 0;
                        Draw dx(0, prev->clip, x), dy(0, d.clip, y);
                        dx.state.texture = dy.state.texture = 0;
                        r = !palette ? rSkinPalette : (sameDraw(dx, dy) ? rSkinSame : rSkinOther);
                    }
                    else
                        r = rSkinOther;
                }
                else if (a->indexed || b->indexed)
                    r = rIndexed;
                else if (a->dynamic != b->dynamic)
                    r = rDynMix;
                else if (prevPlan->tex != p.tex)
                    r = rTex;
                else if (!sameState(*prev, d))
                    r = rState;
                else if (!sameBits<sizeof(d.clip)>(prev->clip, d.clip))
                    r = prev->mesh == d.mesh ? rClipSameMesh : rClip;
                else
                    r = rCap; // (the vertex cap, or a run apart not copied yet)
                ++counts[r];
            }
            prev = &d;
            prevPlan = &p;
        }
        if (++frames % 300u != 0u)
            return;
        std::string line = "[gl:merge] per frame:";
        for (int i = 0; i < kCount; ++i)
        {
            char buf[64];
            std::snprintf(buf, sizeof buf, " %s %.1f", names[i], counts[i] / 300.0);
            line += buf;
            counts[i] = 0;
        }
        std::fprintf(stderr, "%s\n", line.c_str());
    }

    // A run's kept meshes copied side by side into a pool range of its own.
    bool Renderer::copyRun(size_t from, size_t to, uint32_t total, RunCopy &copy)
    {
        if (runVertices + total > kRunBudget)
        {
            // Round 36: one scan a frame at most. Each copy past the budget
            // scanned and sorted every copy (and every run that found no room
            // scanned again, every frame): a camera turn bringing a few
            // hundred new runs made it quadratic on the render thread (the
            // console's 110-170 ms render frames with few uploads are a
            // suspect). Now a scan frees down to 7/8 of the budget, and a
            // frame whose scan found nothing old enough does not scan again.
            // SOTC_GL_RUN_EVICT_OLD=1: as before.
            static const bool oldWay = std::getenv("SOTC_GL_RUN_EVICT_OLD") && std::atoi(std::getenv("SOTC_GL_RUN_EVICT_OLD")) != 0;
            if (!oldWay && runScanFrame == arenaFrame)
                return false;
            runScanFrame = arenaFrame;
            ++stats.runScans;
            const uint64_t goal = oldWay ? kRunBudget : std::min<uint64_t>(kRunBudget - kRunBudget / 8u, kRunBudget - std::min<uint64_t>(total, kRunBudget));
            // Room made from the copies drawn longest ago (not this frame's
            // or the last one's: the GPU may still read them).
            std::vector<std::pair<uint64_t, uint64_t>> old; // (used, key)
            runs.forEach([&](uint64_t key, const RunCopy &r)
            {
                if (r.pool >= 0 && r.used + 2u < arenaFrame)
                    old.push_back({r.used, key});
            });
            std::sort(old.begin(), old.end());
            for (const auto &[used, key] : old)
            {
                if (runVertices + (oldWay ? total : 0u) <= goal)
                    break;
                RunCopy *r = runs.find(key);
                poolFree(r->pool, r->first, r->count);
                runVertices -= r->count;
                r->pool = -1;
                r->frames = 0;
            }
            if (runVertices + total > kRunBudget)
                return false;
            runScanFrame = 0; // room made: later copies this frame may use it without a scan
        }
        int pool = -1;
        uint32_t first = 0;
        if (!poolAlloc(total, pool, first))
            return false;
        constexpr size_t stride = sizeof(native_renderer::Vertex);
#if !defined(PS2X_NATIVE_VITA)
        glBindBuffer(GL_COPY_WRITE_BUFFER, pools[static_cast<size_t>(pool)].buffer);
#endif
        uint32_t at = first;
        for (size_t j = from; j < to; ++j)
        {
            const Mesh *o = plan[j].mesh;
            const Pool &src = pools[static_cast<size_t>(o->pool)];
#if defined(PS2X_NATIVE_VITA)
            std::memcpy(pools[static_cast<size_t>(pool)].mapped + static_cast<size_t>(at) * stride, src.mapped + static_cast<size_t>(o->first) * stride,
                        static_cast<size_t>(o->count) * stride);
#else
            glBindBuffer(GL_COPY_READ_BUFFER, src.buffer);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, static_cast<GLintptr>(o->first) * stride, static_cast<GLintptr>(at) * stride,
                                static_cast<GLsizeiptr>(o->count) * stride);
#endif
            at += o->count;
        }
        copy.pool = pool;
        copy.first = first;
        copy.count = total;
        copy.used = arenaFrame;
        runVertices += total;
        ++stats.runCopies;
        return true;
    }

    // Round 45: the frame's GXM scenes, binds and depth traffic, a [gl:scenes]
    // line every 300 frames (the console's log; elsewhere SOTC_GL_SCENE_LOG=1).
    // Round 86: the host's present of the picture render kept (the swap:
    // vitaGL's display queue, which waits while the GPU is two frames behind).
    uint64_t Renderer::presentNow()
    {
        if (!presentPending)
            return 0u;
        presentPending = false;
        const auto &hook = native_renderer::detail::presentHook();
        if (!hook)
            return 0u;
        const bool timed = native_renderer::tuning().frameTiming();
        gpu86::bind(0u, 0u, 0u); // the display's scene
        const uint64_t start = native_renderer::nowUs();
        const uint32_t backlog = gpu86::backlog();
        hook(pendingTexture, pendingWidth, pendingHeight);
        const uint64_t took = native_renderer::nowUs() - start;
        gpu86::presented(took, backlog);
        // Round 86 (Vita3K testing): SOTC_PRESENT86_SIM_MS=<ms> waits until
        // that long after the present began, a console GPU's wait in the
        // swap, to see the hand-over's overlap where the host GPU never makes
        // the swap wait.
        // SOTC_PRESENT86_SIM_VSYNC=1: and it ends on the next 60 Hz vblank
        // after that, as the console's swap returns when a flip frees the
        // display queue.
        static const int simMs = std::getenv("SOTC_PRESENT86_SIM_MS") ? std::atoi(std::getenv("SOTC_PRESENT86_SIM_MS")) : 0;
        static const bool simVsync = std::getenv("SOTC_PRESENT86_SIM_VSYNC") && std::atoi(std::getenv("SOTC_PRESENT86_SIM_VSYNC")) != 0;
        if (simMs > 0)
        {
            uint64_t until = start + static_cast<uint64_t>(simMs) * 1000u;
            if (simVsync)
            {
                constexpr uint64_t kVblankUs = 16667u;
                until = (until + kVblankUs - 1u) / kVblankUs * kVblankUs;
            }
            const uint64_t now = native_renderer::nowUs();
            if (until > now)
                std::this_thread::sleep_for(std::chrono::microseconds(until - now));
        }
        const uint64_t all = timed ? native_renderer::nowUs() - start : 0u;
        if (!deferPresent)
            stats.presentUs += all;
        return all;
    }

    void Renderer::noteScenes()
    {
        uint32_t scenes = 0, loadKB = 0, storeKB = 0;
#if defined(PS2X_NATIVE_VITA)
        vglCeloSceneStats(&scenes, &loadKB, &storeKB);
        constexpr bool log = true;
#else
        static const bool log = std::getenv("SOTC_GL_SCENE_LOG") != nullptr;
#endif
        stats.scenes = scenes; // round 73: the frame log's (this frame's)
        if (phases.on && ++phases.frames >= 300u)
        {
            const double f = static_cast<double>(phases.frames);
            std::fprintf(stderr, "[gl:phases] per frame over %llu frames: render %.2f ms: plan %.2f, passes %.2f (draw calls %.2f: %.0f calls, %.1f us each), rest %.2f (the frame's vertices sent %.2f: %.0f KB) | lean pass %d, keep layout %d, render prio %d\n",
                         (unsigned long long)phases.frames, phases.total / f / 1000.0, phases.plan / f / 1000.0, phases.passes / f / 1000.0, phases.calls / f / 1000.0,
                         phases.count / f, phases.count ? static_cast<double>(phases.calls) / phases.count : 0.0, (phases.total - phases.plan - phases.passes) / f / 1000.0,
                         phases.arenaUs / f / 1000.0, phases.arenaBytes / f / 1024.0,
                         native_renderer::tuning().glLeanPass ? 1 : 0, native_renderer::tuning().glKeepLayout ? 1 : 0, native_renderer::tuning().renderPrio ? 1 : 0);
            phases.plan = phases.passes = phases.calls = phases.count = phases.total = phases.frames = phases.arenaUs = phases.arenaBytes = 0;
        }
        stats.depthKB = loadKB + storeKB;
        sceneStats.scenes += scenes;
        sceneStats.loadKB += loadKB;
        sceneStats.storeKB += storeKB;
        sceneStats.binds += stats.targets; // (counted from the hand-over: this frame's)
        if (++sceneStats.frames < 300u)
            return;
        const double n = static_cast<double>(sceneStats.frames);
        if (log)
            std::fprintf(stderr, "[gl:scenes] per frame over %llu frames: %.1f scenes, %.1f target binds, depth / stencil %.0f KB loaded, %.0f KB stored | depth keep %d, few copies %d, volume program %d, alpha merge %d"
                                 " | few scenes %d: shown as drawn %llu, copied %llu, glow at the shadows' break %llu | plain reads %d: %.0f draws a frame\n",
                         (unsigned long long)sceneStats.frames, sceneStats.scenes / n, sceneStats.binds / n, sceneStats.loadKB / n, sceneStats.storeKB / n,
                         native_renderer::tuning().glDepthKeep ? 1 : 0, native_renderer::tuning().glFewCopies ? 1 : 0, native_renderer::tuning().glVolumeProgram ? 1 : 0,
                         native_renderer::tuning().alphaTestMerge ? 1 : 0, native_renderer::tuning().glFewScenes ? 1 : 0, (unsigned long long)shownDirect,
                         (unsigned long long)shownCopied, (unsigned long long)glowHoisted, native_renderer::tuning().glPlainReads ? 1 : 0, plainReadDraws / n);
        shownDirect = shownCopied = glowHoisted = plainReadDraws = 0;
        sceneStats.frames = sceneStats.scenes = sceneStats.binds = sceneStats.loadKB = sceneStats.storeKB = 0;
    }

    void Renderer::render(uint32_t w, uint32_t h)
    {
        static const bool phaseLog = std::getenv("SOTC_GL_PHASE_LOG") && std::atoi(std::getenv("SOTC_GL_PHASE_LOG")) != 0;
        phases.on = phaseLog;
        phases.start = phaseLog ? native_renderer::nowUs() : 0u;
        // Round 87 (Tuning glInflight87, a console A/B candidate): the frame
        // begins its scenes only once the GPU has finished the frame before
        // (at most this frame on the GPU, instead of the 2-3 the display
        // queue allows). Same commands, same picture. If the console's GPU
        // time a frame drops with it, the frames in flight were costing the
        // GPU itself (GXM's parameter buffer, 16 MB, shared by every scene in
        // flight: ~120 000 vertices a frame with three float4 varyings fill
        // ~8.5 MB, so two or three frames in flight overflow it and GXM
        // renders scenes in parts, the picture stored and loaded again each time).
#if defined(PS2X_NATIVE_VITA)
        if (native_renderer::tuning().glInflight87)
            gpu86::waitPreviousFrame();
#endif
        ensureTargets(w, h);
        // Round 73 (Tuning glFewScenes): the two pictures in turn (switched
        // either way, so that the frame after one shown as drawn never draws
        // into it: it is feedbackBlur's last frame).
#if defined(PS2X_NATIVE_VITA)
        const bool fewScenes = native_renderer::tuning().glFewScenes;
        if (colorAlt.fbo && native_renderer::detail::presentHook())
        {
            std::swap(color, colorAlt);
            std::swap(colorPlain, colorAltPlain);
        }
#else
        const bool fewScenes = false;
#endif
        boundFbo = boundProgram = ~0u;
        // (a frame that drew its screen blur on the second picture: the
        // targets back in their places for the next one, whichever way it ends)
        struct PictureBack
        {
            Renderer *r;
            ~PictureBack()
            {
                if (r->pictureSwapped)
                    r->swapPicture();
            }
        } pictureBack{this};
        // Round 45 (Tuning glDepthKeep, the Vita): which scenes load / store
        // their depth and stencil (vglCeloDepthKeep: bit 0 load, bit 1 store;
        // -1 vitaGL's forced both). The copies' and passes' scratch depth is
        // never tested, the display's never either (the host draws its
        // picture and menus without depth tests), the background image and
        // the mirror are drawn in one scene from a clear; the picture's
        // scenes are set as they go (`keepColour`).
#if defined(PS2X_NATIVE_VITA)
        const bool depthKeep = native_renderer::tuning().glDepthKeep;
        vglCeloScratchKeep(depthKeep ? 0 : -1);
        vglCeloLeanDraw((native_renderer::tuning().glLeanDraw ? 3 : 0) | (native_renderer::tuning().glKeepLayout ? 4 : 0)); // round 56 (bit 2: round 74)
        {
            // Round 74 (Tuning renderPrio, a console experiment): the render
            // thread at SOTC_RENDER_PRIO_VALUE (default 149, above the hook
            // worker's 150) while on, its own priority otherwise.
            static const SceUID self = sceKernelGetThreadId();
            static const int own = []
            {
                SceKernelThreadInfo info{};
                info.size = sizeof(info);
                return sceKernelGetThreadInfo(sceKernelGetThreadId(), &info) >= 0 ? info.currentPriority : 191;
            }();
            static const int high = std::getenv("SOTC_RENDER_PRIO_VALUE") ? std::atoi(std::getenv("SOTC_RENDER_PRIO_VALUE")) : 149;
            static int set = -1;
            const int want = native_renderer::tuning().renderPrio ? high : own;
            if (want != set && want >= 64 && want <= 191)
            {
                sceKernelChangeThreadPriority(self, want);
                if (set < 0 || native_renderer::tuning().renderPrio)
                    std::fprintf(stderr, "[gl] render thread priority %d (own %d)\n", want, own);
                set = want;
            }
        }
        vglCeloDepthKeep(0, depthKeep ? 0 : -1);
        auto keepDepth = [&](const Target &t, int keep)
        {
            if (t.fbo)
                vglCeloDepthKeep(t.fbo, depthKeep ? keep : -1);
        };
        keepDepth(background, 0);
        keepDepth(reflection, 0);
#else
        auto keepDepth = [](const Target &, int) {};
#endif
        drawOrder.clear();
        drawOrder.reserve(draws.size());
        // Round 74 (Tuning glLeanPass): read once a frame (the A/B flips it from the game thread).
        const bool leanPass = native_renderer::tuning().glLeanPass;
        uint32_t maxOrder = 0;
        if (leanPass)
            for (const Draw &d : draws)
                maxOrder = std::max(maxOrder, d.state.order);
        if (leanPass && maxOrder < 256u)
        {
            // In layer order by counting: the sort's order (layer, then index).
            uint32_t at[257] = {};
            for (const Draw &d : draws)
                ++at[d.state.order + 1u];
            for (uint32_t i = 1; i <= maxOrder; ++i)
                at[i] += at[i - 1u];
            drawOrder.resize(draws.size());
            for (size_t i = 0; i < draws.size(); ++i)
            {
                const uint32_t order = draws[i].state.order;
                drawOrder[at[order]++] = (static_cast<uint64_t>(order) << 32) | i;
            }
        }
        else
        {
            for (size_t i = 0; i < draws.size(); ++i)
                drawOrder.push_back((static_cast<uint64_t>(draws[i].state.order) << 32) | i);
            std::sort(drawOrder.begin(), drawOrder.end());
        }
        planFrame();
        // The draws a pass may draw (not merged into an earlier one), in order.
        leaders.clear();
        if (leanPass)
            for (size_t k = 0; k < plan.size(); ++k)
                if (plan[k].run)
                    leaders.push_back(static_cast<uint32_t>(k));
        if (phases.on)
        {
            const uint64_t planned = native_renderer::nowUs() - phases.start;
            phases.plan += planned;
            stats.planUs += planned;
        }
        const uint64_t arenaStart = phases.on ? native_renderer::nowUs() : 0u;
        if (phases.on)
            phases.arenaBytes += arena.size() * sizeof(native_renderer::Vertex) + indexArena.size() * sizeof(uint32_t);
        if (!arena.empty() || !indexArena.empty())
        {
            // One upload for every dynamic mesh of the frame (in place of one
            // buffer allocated and filled per mesh: 100-250 a frame).
            arenaIndex = (arenaIndex + 1) % 3;
            if (!arena.empty())
            {
                if (!arenaBuffers[arenaIndex])
                    glGenBuffers(1, &arenaBuffers[arenaIndex]);
                glBindBuffer(GL_ARRAY_BUFFER, arenaBuffers[arenaIndex]);
                uploadStream(GL_ARRAY_BUFFER, arena.data(), arena.size() * sizeof(native_renderer::Vertex));
            }
            if (!indexArena.empty())
            {
                if (!indexBuffers[arenaIndex])
                    glGenBuffers(1, &indexBuffers[arenaIndex]);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[arenaIndex]);
                uploadStream(GL_ELEMENT_ARRAY_BUFFER, indexArena.data(), indexArena.size() * sizeof(uint32_t));
            }
        }
        if (phases.on)
            phases.arenaUs += native_renderer::nowUs() - arenaStart;
        const bool fogOn = fog.enabled && fog.width > 0.0f;
        bool haveMirror = false;
        for (const Draw &d : draws)
            if (d.state.reflectionSource)
            {
                haveMirror = true;
                break;
            }
#if defined(PS2X_NATIVE_VITA)
        // The soft path draws a separate mask that must test the picture's
        // stencil; vitaGL framebuffers cannot share it, so the mask covered
        // everything (the horizon vanished with shadows on): darkened in
        // place until round 65c. The hard edges showed the volumes' own
        // triangles on the bodies they come from (Agro's rump and legs in
        // flat light and dark facets, Wander's cloak speckled): the GS (and
        // the Mac) blur the mask. Round 65c: the mask is written into the
        // picture's alpha (free until bloomMask writes it), reduced and
        // blurred as the Mac's (doShadow). SOTC_SHADOW_SOFT_VITA=0: hard, as before.
        static const bool hardShadows = std::getenv("SOTC_SHADOW_SOFT_VITA") && std::atoi(std::getenv("SOTC_SHADOW_SOFT_VITA")) == 0;
#else
        static const bool hardShadows = std::getenv("SOTC_SHADOW_HARD") != nullptr;
#endif
        // What the water (textured 3) and the heat haze (4) read when the
        // mid-frame redraw made them their own copies (0: the usual sources).
        GLuint waterSource = 0, hazeSourceTexture = 0;
        uint32_t softOrder = 0;
        float softAmount = 0.0f;
        if (!hardShadows)
            for (const Draw &d : draws)
                if (d.state.shadowVolume)
                {
                    softOrder = std::max(softOrder, d.state.order + 1u);
                    softAmount = d.state.shadow;
                }
        const bool softShadows = softOrder != 0;
        static const bool bgFog = std::getenv("SOTC_NATIVE_BGFOG") != nullptr;

        // Round 87 (Tuning glShmask87): the soft shadows' darkening (and the
        // glow composite after it) left for the picture's next scene, which
        // runs it before its draws (see maskFuse below).
        std::function<void()> pendingResolve;
        auto pass = [&](Target &target, bool backgroundOnly, uint32_t fromOrder = 0, uint32_t toOrder = ~0u, bool load = false, bool mirror = false,
                        bool apart = false)
        {
            struct PassTime // round 74 (phases)
            {
                Renderer *r;
                uint64_t start;
                ~PassTime()
                {
                    if (r->phases.on)
                    {
                        const uint64_t took = native_renderer::nowUs() - start;
                        r->phases.passes += took;
                        r->stats.passUs += took;
                    }
                }
            } passTime{this, phases.on ? native_renderer::nowUs() : 0u};
            gpu86::label(backgroundOnly ? "bg %d-%d" : mirror ? "mirror %d-%d" : apart ? "apart %d-%d" : "pass %d-%d", static_cast<int>(fromOrder),
                         toOrder == ~0u ? 99 : static_cast<int>(toOrder));
            bindTarget(target);
            if (pendingResolve && load && !backgroundOnly && !mirror && !apart && &target == &color)
            {
                const std::function<void()> resolve = std::move(pendingResolve);
                pendingResolve = nullptr;
                resolve();
                bindTarget(target);
            }
            if (apart) // a layer of its own over the picture's depth: colour cleared to nothing
            {
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            if (!load)
            {
                if (mirror) // reflectionBegin clears with alpha 0x80
                    glClearColor(fog.colour[0], fog.colour[1], fog.colour[2], 1.0f);
                else if (backgroundOnly && bgClearFog()) // the GS's frame starts from a sprite in FOGCOL under the sky (round 65e)
                    glClearColor(fog.colour[0], fog.colour[1], fog.colour[2], 1.0f);
                else
                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#if defined(PS2X_NATIVE_VITA)
                glClearDepthf(1.0f);
#else
                glClearDepth(1.0);
#endif
                glClearStencil(0);
                glDepthMask(GL_TRUE);
                glStencilMask(0xFF);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            }
            glDisable(GL_CULL_FACE);
            if (!mirror && !backgroundOnly && !load && fogOn && native_renderer::tuning().skyImage)
            {
                // The background as the PS2 shows it: bgEnd's reduced copy, stretched back.
                glDisable(GL_STENCIL_TEST);
                bindTexture(0, backgroundSmall.texture);
                quad(target, 0, QuadBlend::None);
            }
            useProgram(mainProgram);
            if (!backgroundOnly)
                bindTexture(1, backgroundSmall.texture);
            if (!mirror)
            {
                bindTexture(2, reflection.texture);
                bindTexture(3, refraction.texture);
            }
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL); // as the GS's GEQUAL (and far pixels round to the clear value 1.0 in the Vita's depth buffer)
            glDepthMask(GL_TRUE);
            glDisable(GL_STENCIL_TEST);
            uint32_t shadowOrder = 0;
            float shadowAmount = 0.0f;
            auto resolveShadows = [&]
            {
                if (!shadowAmount)
                    return;
                if (softShadows)
                {
                    shadowAmount = 0.0f;
                    return;
                }
                // Darken where the count is not 0, and clear it.
                glEnable(GL_STENCIL_TEST);
                glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
                const float a[4] = {shadowAmount, 0, 0, 0};
                quad(target, 5, QuadBlend::Darken, a);
                ++shadowStats.darkens;
                glDisable(GL_STENCIL_TEST);
                useProgram(mainProgram);
                // quad() turned the depth test off: the rest of the pass needs
                // it (without it, everything after layer 7 covered the scene).
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glDepthMask(GL_TRUE);
                shadowAmount = 0.0f;
                cache.blend = cache.depthTest = cache.depthMask = -1;
                cache.buffer = cache.texture = ~0u;
            };
            cache.blend = cache.depthTest = cache.depthMask = -1;
            cache.buffer = cache.texture = ~0u;
            const bool leanOn = native_renderer::tuning().glLeanPrograms, noDiscard = native_renderer::tuning().glNoDiscard;
            // Round 75 (glSoftCut): the cut-out without discard where a cut pixel
            // drawn with colour and alpha 0 leaves the target as it was: the
            // glow layer's alpha is read by its composite; every other target
            // the world passes draw into has its alpha either kept (the Vita
            // bloom's mask: keepAlpha) or not read again (the shadows' and the
            // bloom's masks overwrite all of it first; copies, fog, mirror and
            // display read the colour only).
#if defined(PS2X_NATIVE_VITA)
            const bool softOn = native_renderer::tuning().glSoftCut && &target != &glowLayer;
#else
            const bool softOn = false;
#endif
            uint32_t tintIndex = 0; // SOTC_GL_DYN_TINT
            bool bgDepthPending = false; // layer 1 drawn into the picture: its depth reset before layer 2
            // SOTC_BLOOM_LOG (with SOTC_BLOOM_RECT=x0,y0,x1,y1 in PS2 pixels, rows from the top;
            // default the shrine's left opening): the picture's mean R there after each layer.
            const bool rectLog = bloomLog() && (&target == &color || &target == &background) && !mirror;
            uint32_t rectOrder = ~0u;
            auto logRect = [&](const char *what)
            {
                static const std::array<int, 4> r = []
                {
                    std::array<int, 4> v = {95, 30, 125, 100};
                    if (const char *e = std::getenv("SOTC_BLOOM_RECT"))
                        std::sscanf(e, "%d,%d,%d,%d", &v[0], &v[1], &v[2], &v[3]);
                    return v;
                }();
                const float sx = static_cast<float>(target.width) / 512.0f, sy = static_cast<float>(target.height) / 448.0f;
                const int x0 = static_cast<int>(r[0] * sx), x1 = static_cast<int>(r[2] * sx), y0 = static_cast<int>(r[1] * sy), y1 = static_cast<int>(r[3] * sy);
                std::vector<uint8_t> px(static_cast<size_t>(x1 - x0) * (y1 - y0) * 4u);
                glReadPixels(x0, static_cast<int>(target.height) - y1, x1 - x0, y1 - y0, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
                double sum = 0.0, a = 0.0;
                for (size_t i = 0; i < px.size(); i += 4)
                    sum += px[i], a += px[i + 3];
                const double n = static_cast<double>(px.size() / 4u);
                std::fprintf(stderr, "[bloom:gl] f=%llu rect %s: R mean %.2f alpha mean %.2f\n", (unsigned long long)bloomGameFrame, what, sum / n, a / n);
            };
            // Round 74 (Tuning glLeanPass): only this pass's layers' draws that
            // are drawn (the rest went on at the layer test below, with nothing
            // done for them but the bloom log's).
            const bool byLeaders = leanPass && !rectLog;
            size_t firstAt = 0, endAt = drawOrder.size();
            if (byLeaders)
            {
                auto layerOf = [&](uint32_t k) { return static_cast<uint32_t>(drawOrder[k] >> 32); };
                firstAt = static_cast<size_t>(std::partition_point(leaders.begin(), leaders.end(), [&](uint32_t k) { return layerOf(k) < fromOrder; }) - leaders.begin());
                endAt = static_cast<size_t>(std::partition_point(leaders.begin(), leaders.end(), [&](uint32_t k) { return layerOf(k) < toOrder; }) - leaders.begin());
            }
            for (size_t li = firstAt; li < endAt; ++li)
            {
                const size_t k = byLeaders ? leaders[li] : li;
                if (byLeaders && li + 4u < endAt) // (the record four draws on, asked for ahead)
                    __builtin_prefetch(&draws[static_cast<size_t>(drawOrder[leaders[li + 4u]] & 0xFFFFFFFFu)]);
                const Planned &planned = plan[k];
                if (!planned.run) // merged into an earlier draw
                    continue;
                const Draw &d = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
                static const bool rectDraws = std::getenv("SOTC_BLOOM_RECT_DRAWS") != nullptr;
                if (rectLog && rectDraws && backgroundOnly && d.state.background && d.state.order >= fromOrder && d.state.order < toOrder)
                {
                    char what[160];
                    std::snprintf(what, sizeof what, "background before draw %zu (layer %u tex %llx blend %d opacity %.3f fogged %d alphaTest %u screen %d mesh %llx)", k,
                                  d.state.order, (unsigned long long)d.state.texture, static_cast<int>(d.state.blend), d.state.opacity, d.state.fogged ? 1 : 0,
                                  d.state.alphaTest, d.state.screen ? 1 : 0, (unsigned long long)d.mesh);
                    logRect(what);
                }
                if (rectLog && d.state.order != rectOrder && d.state.order >= fromOrder && d.state.order < toOrder)
                {
                    char what[64];
                    std::snprintf(what, sizeof what, "%s before layer %u", backgroundOnly ? "background" : "picture", d.state.order);
                    logRect(what);
                    rectOrder = d.state.order;
                }
                if (backgroundOnly && !d.state.background)
                    continue;
                if (mirror != d.state.reflectionSource)
                    continue;
                if (d.state.reflectionSample && !haveMirror)
                    continue;
                if (!backgroundOnly && fogOn && d.state.background)
                    continue;
                if (d.state.order < fromOrder || d.state.order >= toOrder)
                    continue;
                // Round 65e: layer 1 drawn into the picture (no zFog this
                // frame): after it, bgEnd sets the PS2's Z to 1 over the whole
                // frame (a Z-only sprite), so the sky and the far scenery are
                // "nothing drawn" for what follows: the bloom's mask (Z <= the
                // threshold + 1) covers them on the GS. The depth cleared here
                // as there (SOTC_BG_DEPTH_RESET=0: kept, as before: the bridge's
                // far scenery out of the mask, its glow missing).
                if (!backgroundOnly && !mirror && bgDepthPending && d.state.order > 1u)
                {
                    bgDepthPending = false;
                    static const bool reset = !std::getenv("SOTC_BG_DEPTH_RESET") || std::atoi(std::getenv("SOTC_BG_DEPTH_RESET")) != 0;
                    if (reset)
                    {
                        glDepthMask(GL_TRUE);
#if defined(PS2X_NATIVE_VITA)
                        glClearDepthf(1.0f);
#else
                        glClearDepth(1.0);
#endif
                        glClear(GL_DEPTH_BUFFER_BIT);
                        cache.depthMask = -1;
                    }
                }
                if (!backgroundOnly && !mirror && d.state.order == 1u)
                    bgDepthPending = true;
                if (shadowAmount && d.state.order > shadowOrder)
                    resolveShadows();
                if (!planned.mesh)
                    continue;
                static const int skipKind = std::getenv("SOTC_GL_DRAW_SKIP") ? std::atoi(std::getenv("SOTC_GL_DRAW_SKIP")) : 0; // debugging (round 65e)
                static const uint32_t skipMask = std::getenv("SOTC_GL_DRAW_SKIP_MASK") ? static_cast<uint32_t>(std::strtoul(std::getenv("SOTC_GL_DRAW_SKIP_MASK"), nullptr, 16)) : 0u;
                if (skipMask && planned.mesh->dynamic && !planned.mesh->indexed && ((skipMask >> (d.state.order & 31u)) & 1u))
                    continue;
                if ((skipKind == 1 && d.state.overlay) || (skipKind == 2 && planned.mesh->skinned) || (skipKind == 3 && d.state.blend == native_renderer::Blend::Additive) ||
                    (skipKind == 5 && planned.mesh->skinned && d.state.blend != native_renderer::Blend::Opaque) || (skipKind == 6 && planned.mesh->skinned && d.state.blend == native_renderer::Blend::Opaque) ||
                    (skipKind == 7 && planned.mesh->dynamic) || (skipKind >= 100 && skipKind < 200 && d.state.screen && d.state.order == static_cast<uint32_t>(skipKind - 100)) ||
                    (skipKind >= 200 && planned.mesh->dynamic && !planned.mesh->indexed && d.state.order == static_cast<uint32_t>(skipKind - 200)) || (skipKind == 8 && planned.mesh->indexed) || (skipKind == 9 && !planned.mesh->skinned && d.state.order >= 2u && d.state.order <= 3u))
                    continue;
                Mesh &mesh = *planned.mesh;
                if (bloomLog() && mesh.dynamic && !mesh.indexed && &target == &color) // (round 65e) the frame's non-indexed per-frame draws
                    std::fprintf(stderr, "[gl:dyn] f=%llu order %u screen %d blend %d tex %llx count %u skinned %d overlay %d mesh %016llx\n",
                                 (unsigned long long)bloomGameFrame, d.state.order, d.state.screen ? 1 : 0, static_cast<int>(d.state.blend),
                                 (unsigned long long)d.state.texture, mesh.count, mesh.skinned ? 1 : 0, d.state.overlay ? 1 : 0, (unsigned long long)d.mesh);
                if (spike.on)
                {
                    const uint64_t now = native_renderer::nowUs();
#if defined(PS2X_NATIVE_VITA)
                    const uint32_t scenesNow = vgl_celo_scenes;
#else
                    const uint32_t scenesNow = 0;
#endif
                    if (spike.last && now - spike.last > spike.gap)
                        spike.gap = now - spike.last, spike.index = spike.drawn, spike.gapEvent = spike.event, spike.gapScenes = scenesNow - spike.scenesAt;
                    spike.last = now;
                    spike.event = -100;
                    spike.scenesAt = scenesNow;
                    if (spike.index == spike.drawn) // the draw the gap ends at
                    {
                        spike.order = d.state.order, spike.count = mesh.count, spike.target = target.fbo;
                        spike.volume = d.state.shadowVolume, spike.overlay = d.state.overlay, spike.skinned = mesh.skinned, spike.dynamic = mesh.dynamic;
                        spike.alphaTest = d.state.alphaTest;
                    }
                    ++spike.drawn;
                }
                bindTarget(target);
                const bool skinned = mesh.skinned;
                if (skinned && (!skinProgram || !d.state.skinPalette))
                    continue;
                static const bool checkArena = std::getenv("SOTC_GL_ARENA_LOG") != nullptr;
                if (checkArena && mesh.dynamic && mesh.frame != arenaFrame)
                {
                    static int n = 0;
                    if (n++ < 20)
                        std::fprintf(stderr, "[arena] mesh %016llx drawn in arena %llu, uploaded in %llu (order %u)\n", (unsigned long long)d.mesh,
                                     (unsigned long long)arenaFrame, (unsigned long long)mesh.frame, d.state.order);
                }
                const GLsizei stride = skinned ? sizeof(native_renderer::SkinVertex) : sizeof(native_renderer::Vertex);
                // A dynamic mesh is a range of the frame's buffer: its start goes
                // to glDrawArrays (vitaGL keeps a pointer offset in GXM's 16-bit
                // attribute offset: past 64 KB it read garbage).
                const size_t base = 0u;
                // A merged run whose meshes lie apart is drawn from its copy (runs).
                const bool runCopy = planned.run > 1 && planned.runPool >= 0;
                const GLint firstVertex = runCopy ? static_cast<GLint>(planned.runFirst) : (mesh.dynamic || mesh.pool >= 0 ? static_cast<GLint>(mesh.first) : 0);
                const GLuint vertexBuffer = runCopy ? pools[static_cast<size_t>(planned.runPool)].buffer
                                                    : (mesh.dynamic ? arenaBuffers[arenaIndex] : (mesh.pool >= 0 ? pools[static_cast<size_t>(mesh.pool)].buffer : mesh.buffer));
                if (vertexBuffer != cache.buffer) // the frame's dynamic meshes share one buffer, the kept ones a few: set once
                {
                    cache.buffer = vertexBuffer;
                    ++drawStats.bufferSwitches;
                    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, x)));
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, nx)));
                    glEnableVertexAttribArray(2);
                    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, r)));
                    glEnableVertexAttribArray(3);
                    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, u)));
                    for (GLuint i = 4; i < 6; ++i)
                    {
                        if (!skinned)
                        {
                            glDisableVertexAttribArray(i);
                            continue;
                        }
                        glEnableVertexAttribArray(i);
                        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, stride,
                                              reinterpret_cast<const void *>(i == 4 ? offsetof(native_renderer::SkinVertex, bone) : offsetof(native_renderer::SkinVertex, weight)));
                    }
                }
                Texture *tex = planned.tex;
                const int textured = (skipKind == 4 && mesh.skinned) ? 0 : d.state.previousFrame ? 5 : d.state.reflectionSample ? 2 : (d.state.refractionSample ? 3 : (d.state.screenCopy ? 4 : (tex ? 1 : 0)));
                const bool vertexFog = fog.vertexFog && !d.state.screen && (!d.state.background || bgFog || (d.state.backgroundFge && bgFgeOn()));
#if defined(PS2X_NATIVE_VITA)
                // No depth texture for the zFog pass: the world before its layer
                // blends to the background by its own depth here, slice for slice.
                const bool zFogged = fogOn && fog.steps > 0 && !d.state.screen && !d.state.background && d.state.order < fog.order;
#else
                const bool zFogged = false;
#endif
                const float fogA[4] = {fog.start, fog.width, static_cast<float>(fog.steps), (!backgroundOnly && fogOn && (d.state.fogged || zFogged)) ? 1.0f : 0.0f};
                // The program: a lean one for the plain world draws (only the
                // samplers they read; without the cut-out when no pixel can be
                // cut), the full one for the rest.
                GLuint program = skinned ? skinProgram : mainProgram;
                if (d.state.shadowVolume && native_renderer::tuning().glVolumeProgram && mains[kVolumeSlot].program)
                    program = mains[kVolumeSlot].program; // round 45: only counts
                // alpha = texel a x 2 (0x80 = 1) x vertex a / 128 x opacity, at least (-1: not known):
                float leastAlpha = -1.0f;
                if (!d.state.fresnel && planned.minAlpha && (textured == 0 || textured == 1))
                {
                    leastAlpha = static_cast<float>(planned.minAlpha) / 128.0f * d.state.opacity;
                    if (textured == 1)
                        leastAlpha *= static_cast<float>(tex->minAlpha) / 255.0f * 2.0f;
                }
                // The GS alpha test (DrawState::alphaTest, round 38; GEQUAL / GREATER
                // with AFAIL FB_ONLY / RGB_ONLY, SotC's models): the pixels at AREF
                // or over with the draw's depth writes, then those under it again
                // with the depth read only, when some can be under (the texture's
                // and the mesh's least alpha say).
                const uint32_t at = native_renderer::tuning().alphaTest ? d.state.alphaTest : 0u;
                const uint32_t atst = (at >> 1) & 7u, afail = (at >> 12) & 3u;
                const bool atOn = (at & 1u) && (atst == 5u || atst == 6u) && (afail == 1u || afail == 3u) && !d.state.overlay && !d.state.screen &&
                                  !d.state.background && !d.state.noDepthWrite && !d.state.screenDepthTest && !d.state.shadowVolume &&
                                  d.state.blend != native_renderer::Blend::Additive;
                const float aref = static_cast<float>((at >> 4) & 0xFFu) / 128.0f;
                // Round 45 (Tuning alphaTestMerge): every pixel under AREF (the
                // greatest texel and vertex alphas say, a margin for the GPU's
                // rounding): the first pass would draw nothing; the second
                // alone, in one pass.
                bool atAllUnder = false;
                if (atOn && native_renderer::tuning().alphaTestMerge && !d.state.fresnel && planned.maxAlpha != 0xFFu && (textured == 0 || textured == 1))
                {
                    float greatest = static_cast<float>(planned.maxAlpha) / 128.0f * d.state.opacity;
                    if (textured == 1)
                        greatest *= static_cast<float>(tex->maxAlpha) / 255.0f * 2.0f;
                    atAllUnder = greatest < aref - 1.0f / 256.0f;
                }
                const bool atSecond = atOn && !atAllUnder && !(leastAlpha >= aref);
                float guard[4] = {0, 0, 0, 0};
                guardFor(d.state, guard);
                // Round 75 (glFarGuard): a background draw lying wholly past
                // layer 1's near plane (its mesh's bounds say, with a margin
                // for the GPU's rounding) has no pixel for the guard to cut:
                // drawn without it (and, its alphas allowing, without discard).
                if (guard[0] > 0.0f)
                {
                    ++drawStats.guarded;
                    if (native_renderer::tuning().glFarGuard && planned.minW > guard[1] * 1.001f + 0.001f)
                    {
                        guard[0] = guard[1] = 0.0f;
                        ++drawStats.guardOff;
                    }
                }
                // SOTC_GL_DYN_TINT=1 (debugging, round 65e): each non-indexed per-frame
                // draw's colour replaced by one of 12 (in draw order within the frame;
                // [gl:dyn] lines every 120 frames name them), its alpha kept.
                static const bool dynTint = std::getenv("SOTC_GL_DYN_TINT") && std::atoi(std::getenv("SOTC_GL_DYN_TINT")) != 0;
                static const int dynTintMode = std::getenv("SOTC_GL_DYN_TINT") ? std::atoi(std::getenv("SOTC_GL_DYN_TINT")) : 0;
                const bool tintThis = dynTint && !d.state.shadowVolume && textured <= 1 &&
                                      (dynTintMode == 2 ? true : dynTintMode == 3 ? (mesh.dynamic || mesh.skinned) : dynTintMode == 4 ? (!mesh.dynamic && !mesh.skinned && d.state.order >= 5u) : (mesh.dynamic && !mesh.indexed));
                // Round 75: the draw blends by the source alpha (a pixel of alpha 0
                // leaves the colour as it was) and writes no depth (nor stencil:
                // the world draws have it off): see softOn.
                const bool blendsBySourceAlpha = d.state.blend == native_renderer::Blend::Normal || d.state.blend == native_renderer::Blend::Additive ||
                                                 d.state.blend == native_renderer::Blend::Subtract;
                const bool backdropDraw = d.state.screen || d.state.background;
                const bool writesNoDepth = d.state.overlay || (d.state.blend == native_renderer::Blend::Additive && !d.state.writeDepth) || d.state.noDepthWrite || d.state.screenDepthTest || atAllUnder;
                const bool softHere = softOn && blendsBySourceAlpha && !backdropDraw && !d.state.shadowVolume;
                GLuint softSecond = 0; // the alpha test's second pass (no depth writes) through the SOFT program
                bool cutUsed = false, softUsed = false;
                if (leanOn && !d.state.shadowVolume && textured <= 1 && !d.state.debugColour && !tintThis)
                {
                    bool cut = true;
                    if (noDiscard && leastAlpha >= 0.0f && !(guard[0] > 0.0f))
                        cut = !(leastAlpha >= 0.03f) || atSecond || atAllUnder; // the cut-out is at 0.02
                    // Round 73 (Tuning glPlainReads): a texture of one level read with no LOD.
                    const bool plain = textured == 1 && !tex->mipped && native_renderer::tuning().glPlainReads;
                    GLuint lean = leanProgram(skinned, fogA[3] > 0.5f, cut, plain);
                    if (plain && !lean)
                        lean = leanProgram(skinned, fogA[3] > 0.5f, cut);
                    else if (plain)
                        ++plainReadDraws;
                    if (cut && softHere && !plain && !(fogA[3] > 0.5f)) // (not BG: see kWorldFragment's SOFT)
                    {
                        const GLuint soft = softProgram(skinned);
                        if (soft && writesNoDepth)
                            lean = soft, softUsed = true;
                        else if (soft && atSecond)
                            softSecond = soft;
                    }
                    if (lean)
                    {
                        program = lean;
                        drawStats.noDiscard += cut ? 0u : 1u;
                        cutUsed = cut && !softUsed;
                    }
                }
                useProgram(program);
                auto sendPalette = [&]
                {
                    if (!skinned)
                        return;
                    // The draw's palette (each program keeps the last one sent).
                    MainSlot &m = mains[mainSlot];
                    const uint64_t key = (arenaFrame << 32) | (static_cast<uint64_t>(d.state.skinPalette) << 8) | std::min(d.state.skinPaletteCount, 255u);
                    const size_t from = static_cast<size_t>(d.state.skinPalette - 1u) * 4u;
                    const uint32_t count = std::min(d.state.skinPaletteCount, native_renderer::kSkinPaletteQwords);
                    if (key != m.paletteSent && from + count * 4u <= palettes.size() && m.palette >= 0)
                    {
                        glUniform4fv(m.palette, static_cast<GLsizei>(count), palettes.data() + from);
                        m.paletteSent = key;
                    }
                };
                sendPalette();
                uniform4(uClip, d.clip, 4);
                if (d.state.lit || d.state.fresnel) // read by the vertex program only then
                {
                    uniform4(uDirections, d.state.light, 4);
                    uniform4(uColours, d.state.light + 16, 4);
                }
                if (d.state.shadowVolume)
                {
                    if (backgroundOnly)
                        continue;
                    // The count where the faces pass the depth test: front +1, back -1.
                    const float misc[4] = {0, 1, 0, 0}, none[4] = {0, 0, 0, 0};
                    uniform4(uMisc, misc, 1);
                    uniform4(uFogB, none, 1);
                    uniform4(uFogC, none, 1);
                    uniform4(uFogA, none, 1);
                    uniform4(uFogColour, none, 1);
                    uniform4(uDebug, none, 1);
                    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
#if defined(PS2X_NATIVE_VITA)
                    // Round 87 (Tuning glVol87, a console A/B candidate): the
                    // count drawn as GXM's depth / stencil-only primitives (the
                    // fragment program disabled: the ISP alone, as vitaGL's own
                    // depth / stencil clears), no blend: the volume's pixels
                    // never reach the USSE or the tile's colour.
                    const bool volIsp = native_renderer::tuning().glVol87;
                    if (volIsp)
                    {
                        glDisable(GL_BLEND);
                        sceGxmSetFrontFragmentProgramEnable(gxm_context, SCE_GXM_FRAGMENT_PROGRAM_DISABLED);
                        sceGxmSetBackFragmentProgramEnable(gxm_context, SCE_GXM_FRAGMENT_PROGRAM_DISABLED);
                        ++shadowStats.ispVolumes;
                    }
                    else
                    {
                    // vitaGL bakes the colour mask into the fragment program's
                    // blend info, which did not hold (the volumes were painted
                    // over the horizon): blend so the colour cannot change too.
                    glEnable(GL_BLEND);
                    glBlendEquation(GL_FUNC_ADD);
                    glBlendFunc(GL_ZERO, GL_ONE);
                    }
#endif
                    glDepthMask(GL_FALSE);
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(GL_LEQUAL); // as the GS's GEQUAL (and far pixels round to the clear value 1.0 in the Vita's depth buffer)
                    glEnable(GL_STENCIL_TEST);
                    glStencilMask(0xFF);
                    glStencilFuncSeparate(GL_FRONT_AND_BACK, GL_ALWAYS, 0, 0xFF);
                    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
                    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
#if !defined(PS2X_NATIVE_VITA)
                    glEnable(GL_DEPTH_CLAMP); // the GS does not cut at the near plane
#endif
                    drawMesh(mesh, firstVertex);
#if defined(PS2X_NATIVE_VITA)
                    if (volIsp)
                    {
                        sceGxmSetFrontFragmentProgramEnable(gxm_context, SCE_GXM_FRAGMENT_PROGRAM_ENABLED);
                        sceGxmSetBackFragmentProgramEnable(gxm_context, SCE_GXM_FRAGMENT_PROGRAM_ENABLED);
                    }
#endif
                    stats.vertices += mesh.count;
                    ++shadowStats.volumes;
                    shadowStats.vertices += mesh.count;
                    if (shadowStats.lastFrame != arenaFrame)
                        shadowStats.lastFrame = arenaFrame, ++shadowStats.framesWithVolumes;
#if !defined(PS2X_NATIVE_VITA)
                    glDisable(GL_DEPTH_CLAMP);
#endif
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glDisable(GL_STENCIL_TEST);
                    shadowOrder = d.state.order;
                    shadowAmount = d.state.shadow;
                    cache.blend = cache.depthTest = cache.depthMask = -1;
                    continue;
                }
                if (static_cast<int>(d.state.blend) != cache.blend)
                {
                    setBlend(d.state.blend, keepAlpha);
                    cache.blend = static_cast<int>(d.state.blend);
                }
                // The depth function is always LEQUAL here (the GS's GEQUAL: a
                // surface drawn again in place passes); test and writes vary.
                const bool backdrop = d.state.screen || d.state.background;
                // Round 38: layer 1 (sky, far landscape) depth tested into the background
                // image, as the GS draws it (Tuning::backgroundDepth); draw order hid near
                // cliffs behind the trees and far faces before.
                const bool bgDepth = backgroundOnly && d.state.background && !d.state.screen && native_renderer::tuning().backgroundDepth;
                const int depthTest = backdrop && !d.state.screenDepthTest && !bgDepth ? 0 : 1;
                const int depthMask = d.state.screenDepthTest ? 0
                                      : bgDepth ? 1
                                      : backdrop ? cache.depthMask
                                               : (d.state.overlay || (d.state.blend == native_renderer::Blend::Additive && !d.state.writeDepth) || d.state.noDepthWrite ? 0 : 1);
                if (depthTest != cache.depthTest)
                {
                    if (depthTest)
                    {
                        glEnable(GL_DEPTH_TEST);
                        glDepthFunc(GL_LEQUAL);
                    }
                    else
                        glDisable(GL_DEPTH_TEST);
                    cache.depthTest = depthTest;
                }
                const int depthMaskDrawn = atAllUnder ? 0 : depthMask; // all under AREF: FB_ONLY, no depth written
                if (depthMaskDrawn != cache.depthMask)
                {
                    glDepthMask(depthMaskDrawn ? GL_TRUE : GL_FALSE);
                    cache.depthMask = depthMaskDrawn;
                }
                if (d.state.texture && !tex)
                    ++texStats.missing;
                if (textured == 5)
                {
                    bindTexture(0, previousTexture ? previousTexture : previous.texture);
                    cache.texture = ~0u;
                }
                if (textured == 3 || textured == 4)
                {
                    const native_renderer::Tuning &t = native_renderer::tuning();
                    const int source = static_cast<int>((textured == 4 ? t.hazeSource : t.waterSource) + 0.5f);
                    const GLuint redrawn = textured == 4 ? hazeSourceTexture : waterSource;
                    bindTexture(3, redrawn && source != 2 ? redrawn : (source == 2 ? (previousTexture ? previousTexture : previous.texture) : (source == 1 ? snapshot.texture : refraction.texture)));
                }
                if (textured == 1)
                {
                    tex->used = arenaFrame;
                    const GLuint texName = drawName(*tex);
                    if (cache.texture != texName)
                    {
                        bindTexture(0, texName);
                        cache.texture = texName;
                    }
                    if (tex->clamp != (d.state.clamp & 3u)) // a texture's wrap is its own state: set when it changes
                    {
                        tex->clamp = d.state.clamp & 3u;
                        for (const GLuint n : {tex->swz, tex->name}) // (both copies: round 73)
                            if (n)
                            {
                                bindTexture(0, n); // unit 0 active (a refraction draw may have left unit 3)
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (d.state.clamp & 1u) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (d.state.clamp & 2u) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
                            }
                        bindTexture(0, texName);
                    }
                    // Quality "Texture filtering": nearest, bilinear, trilinear (anisotropic: Metal only, trilinear here).
                    const uint32_t filter = d.state.screen ? 2u : static_cast<uint32_t>(std::clamp(native_renderer::tuning().textureFilter + 0.5f, 0.0f, 2.0f)); // 2D keeps the default
                    if (tex->filter != filter)
                    {
                        tex->filter = filter;
                        const bool mipped = tex->mipped;
                        for (const GLuint n : {tex->swz, tex->name}) // (both copies: round 73)
                            if (n)
                            {
                                bindTexture(0, n);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                                filter == 0 ? (mipped ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST)
                                                            : (filter == 1 ? (mipped ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR) : (mipped ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR)));
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter == 0 ? GL_NEAREST : GL_LINEAR);
                            }
                        bindTexture(0, texName);
                    }
                }
                const float misc[4] = {d.state.fresnel ? -1.0f : (d.state.lit ? 1.0f : 0.0f), d.state.opacity, d.state.uvOffset[0], d.state.uvOffset[1]};
                const float fogB[4] = {static_cast<float>(w), static_cast<float>(h), vertexFog ? 1.0f : 0.0f, fog.base};
                // The mirror's ripple (t, H) where a reflection reads no mip level.
                const float fogC[4] = {fog.scale, d.state.reflectionSample ? d.state.mirrorRipple[0] : d.state.lodK,
                                       d.state.reflectionSample ? d.state.mirrorRipple[1] : d.state.lodScale, d.state.reflectionSample ? 0.0f : d.state.maxLod};
                const float fogColour[4] = {fog.colour[0], fog.colour[1], fog.colour[2], static_cast<float>(textured)};
                float debug[4] = {static_cast<float>(d.state.debugColour & 0xFFu) / 255.0f, static_cast<float>((d.state.debugColour >> 8) & 0xFFu) / 255.0f,
                                  static_cast<float>((d.state.debugColour >> 16) & 0xFFu) / 255.0f, d.state.debugColour ? 1.0f : 0.0f};
                if (d.state.reflectionSample || d.state.refractionSample)
                {
                    for (int k = 0; k < 3; ++k)
                        debug[k] = d.state.reflectionTint[k];
                    debug[3] = -d.state.reflectionTint[3];
                }
                if (tintThis)
                {
                    static const uint32_t palette[12] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF,
                                                         0xFF8000, 0x8000FF, 0x00FF80, 0x804000, 0xFF0080, 0x808080};
                    // mode 2/3: a colour per draw (index i: r = (i & 7) x 36, g = (i >> 3 & 7) x 36, b = (i >> 6 & 3) x 85)
                    const uint32_t c = dynTintMode >= 2 ? ((((tintIndex & 7u) * 36u) << 16) | ((((tintIndex >> 3) & 7u) * 36u) << 8) | (((tintIndex >> 6) & 3u) * 85u))
                                                        : palette[tintIndex % 12u];
                    debug[0] = static_cast<float>((c >> 16) & 0xFFu) / 255.0f;
                    debug[1] = static_cast<float>((c >> 8) & 0xFFu) / 255.0f;
                    debug[2] = static_cast<float>(c & 0xFFu) / 255.0f;
                    debug[3] = 2.0f;
                    static uint64_t loggedFrame = ~0ull;
                    if ((arenaFrame % 120u == 0u || loggedFrame == arenaFrame) && &target == &color)
                    {
                        loggedFrame = arenaFrame;
                        std::fprintf(stderr, "[gl:dyn] frame %llu tint #%u %06x: order %u screen %d blend %d tex %llx count %u textured %d mesh %016llx indexed %d pool %d overlay %d alphaTest %x opacity %.3f\n",
                                     (unsigned long long)arenaFrame, tintIndex, c, d.state.order, d.state.screen ? 1 : 0, static_cast<int>(d.state.blend),
                                     (unsigned long long)d.state.texture, mesh.count, textured, (unsigned long long)d.mesh, mesh.indexed ? 1 : 0, mesh.pool,
                                     d.state.overlay ? 1 : 0, d.state.alphaTest, d.state.opacity);
                    }
                    ++tintIndex;
                }
                uniform4(uMisc, misc, 1);
                uniform4(uFogA, fogA, 1);
                uniform4(uFogB, fogB, 1);
                uniform4(uFogC, fogC, 1);
                uniform4(uFogColour, fogColour, 1);
                uniform4(uDebug, debug, 1);
                uniform4(uGuard, guard, 1);
                const float alphaCut[4] = {atSecond ? aref : 0.0f, atAllUnder ? aref : 0.0f, 0.0f, 0.0f};
                if (atAllUnder)
                    ++alphaStats.merged;
                if (atSecond && !planned.minAlpha)
                    ++alphaStats.secondUnknown;
                uniform4(uAlphaTest, alphaCut, 1);
                {
                    // SOTC_GL_LAYER_LOG=<layer> (debugging, round 55): every 120
                    // frames, the first draws of that layer as the GPU gets them.
                    static const int logLayer = std::getenv("SOTC_GL_LAYER_LOG") ? std::atoi(std::getenv("SOTC_GL_LAYER_LOG")) : -1;
                    static uint64_t logFrame = ~0ull;
                    static int logged = 0;
                    if (logLayer >= 0 && static_cast<int>(d.state.order) == logLayer && arenaFrame % 120u == 0u)
                    {
                        if (logFrame != arenaFrame)
                            logFrame = arenaFrame, logged = 0;
                        const void *texData = nullptr;
#if defined(PS2X_NATIVE_VITA)
                        if (tex)
                        {
                            bindTexture(0, tex->name);
                            texData = vglGetTexDataPointer(GL_TEXTURE_2D);
                        }
#endif
                        if (logged < 4)
                            std::fprintf(stderr, "[gl:layer] data %p\n", texData);
                        if (logged++ < 4)
                            std::fprintf(stderr, "[gl:layer] frame %llu mesh %016llx dyn %d first %d count %u buf %u prog %u slot %d textured %d tex %u (%ux? mipped %d alpha %u..%u filter %u clamp %u) blend %d "
                                                 "misc %g %g %g %g fogA %g %g %g %g fogB %g %g %g %g fogC %g %g %g %g fogCol %g %g %g %g debug %g %g %g %g cut %g %g least %g\n",
                                         (unsigned long long)arenaFrame, (unsigned long long)d.mesh, mesh.dynamic ? 1 : 0, static_cast<int>(firstVertex), mesh.count,
                                         vertexBuffer, program, mainSlot, textured, tex ? tex->name : 0u, 0u, tex && tex->mipped ? 1 : 0, tex ? tex->minAlpha : 0u,
                                         tex ? tex->maxAlpha : 0u, tex ? tex->filter : 0u, tex ? tex->clamp : 0u, static_cast<int>(d.state.blend), misc[0], misc[1],
                                         misc[2], misc[3], fogA[0], fogA[1], fogA[2], fogA[3], fogB[0], fogB[1], fogB[2], fogB[3], fogC[0], fogC[1], fogC[2], fogC[3],
                                         fogColour[0], fogColour[1], fogColour[2], fogColour[3], debug[0], debug[1], debug[2], debug[3], alphaCut[0], alphaCut[1],
                                         leastAlpha);
                    }
                }
                {
                    static const long seqFrame = std::getenv("SOTC_GL_DRAWSEQ") ? std::atol(std::getenv("SOTC_GL_DRAWSEQ")) : -1;
                    static FILE *seq = nullptr;
                    if (seqFrame >= 0 && arenaFrame == static_cast<uint64_t>(seqFrame))
                    {
                        if (!seq)
                            seq = std::fopen("gl_drawseq.txt", "w");
                        auto hash = [](const float *f, int n, uint64_t h)
                        {
                            for (int i = 0; i < n; ++i)
                            {
                                uint32_t b;
                                std::memcpy(&b, &f[i], 4);
                                h = (h ^ b) * 0x100000001b3ull;
                            }
                            return h;
                        };
                        uint64_t u = hash(misc, 4, 1469598103934665603ull);
                        u = hash(fogA, 4, u);
                        u = hash(fogB, 4, u);
                        u = hash(fogC, 4, u);
                        u = hash(fogColour, 4, u);
                        u = hash(debug, 4, u);
                        if (d.state.lit || d.state.fresnel)
                            u = hash(d.state.light, 32, u);
                        std::fprintf(seq, "%p o %u mesh %016llx dyn %d first %u count %u idx %d buf %u tex %u clamp %u blend %d dt %d dm %d textured %d skin %d clip %016llx uni %016llx\n",
                                     (void *)&target, d.state.order, (unsigned long long)d.mesh, mesh.dynamic ? 1 : 0, firstVertex, planned.run > 1 ? planned.mergedCount : mesh.count,
                                     mesh.indexed ? 1 : 0, vertexBuffer, textured == 1 ? tex->name : 0u, d.state.clamp & 3u, (int)d.state.blend,
                                     cache.depthTest, cache.depthMask, textured, skinned ? 1 : 0, (unsigned long long)hash(d.clip, 16, 1469598103934665603ull),
                                     (unsigned long long)u);
                    }
                    else if (seq)
                    {
                        std::fclose(seq);
                        seq = nullptr;
                    }
                }
                ++drawStats.calls;
                drawStats.cut += cutUsed ? 1u : 0u;
                drawStats.cutDepth += cutUsed && cache.depthMask ? 1u : 0u;
                if (cutUsed && cache.depthMask) // round 75: why a depth-writing draw keeps its discard
                    ++drawStats.cutWhy[atSecond ? 0 : guard[0] > 0.0f ? 1 : leastAlpha < 0.0f ? (mesh.dynamic ? 2 : d.state.fresnel ? 3 : 4) : 5];
                drawStats.soft += softUsed ? 1u : 0u;
                {
                    const uint32_t nv = planned.run > 1 ? planned.mergedCount : mesh.count;
                    const bool full = program == mainProgram || program == skinProgram;
                    (cutUsed ? drawStats.vertsCut : softUsed ? drawStats.vertsSoft : full ? drawStats.vertsMain : drawStats.vertsPlain) += nv;
                    if (full)
                        ++drawStats.mainKind[std::min(textured, 5)];
                }
                static const bool progCheck = std::getenv("SOTC_GL_PROG_CHECK") != nullptr;
                if (progCheck)
                {
                    GLint cur = 0;
                    glGetIntegerv(GL_CURRENT_PROGRAM, &cur);
                    drawStats.progOff += static_cast<GLuint>(cur) != boundProgram;
                    drawStats.progZero += cur == 0;
                }
                const uint64_t callStart = spike.on || phases.on ? native_renderer::nowUs() : 0u;
                if (planned.run > 1)
                {
                    drawStats.merged += planned.run - 1u;
                    ++(planned.runPool >= 0 ? drawStats.indexed : drawStats.ranges);
                    glDrawArrays(GL_TRIANGLES, firstVertex, static_cast<GLsizei>(planned.mergedCount));
                }
                else
                    drawMesh(mesh, firstVertex);
                stats.vertices += planned.run > 1 ? planned.mergedCount : mesh.count;
                if (phases.on)
                {
                    const uint64_t took = native_renderer::nowUs() - callStart;
                    phases.calls += took, ++phases.count;
                    stats.callUs += took, ++stats.calls;
                }
                if (spike.on)
                {
                    const uint64_t took = native_renderer::nowUs() - callStart;
                    if (took > spike.callMax)
                        spike.callMax = took, spike.callIndex = spike.drawn - 1u;
                }
                if (atSecond)
                {
                    // The pixels under AREF: colour, no depth write.
                    const float under[4] = {0.0f, aref, 0.0f, 0.0f};
                    if (softSecond)
                    {
                        // Round 75: through the SOFT program (its own uniforms).
                        useProgram(softSecond);
                        sendPalette();
                        uniform4(uClip, d.clip, 4);
                        if (d.state.lit || d.state.fresnel)
                        {
                            uniform4(uDirections, d.state.light, 4);
                            uniform4(uColours, d.state.light + 16, 4);
                        }
                        uniform4(uMisc, misc, 1);
                        uniform4(uFogA, fogA, 1);
                        uniform4(uFogB, fogB, 1);
                        uniform4(uFogC, fogC, 1);
                        uniform4(uFogColour, fogColour, 1);
                        uniform4(uDebug, debug, 1);
                        uniform4(uGuard, guard, 1);
                        ++drawStats.soft;
                    }
                    else if (cutUsed)
                        ++drawStats.cut;
                    uniform4(uAlphaTest, under, 1);
                    if (cache.depthMask != 0)
                    {
                        glDepthMask(GL_FALSE);
                        cache.depthMask = 0;
                    }
                    const uint64_t secondStart = phases.on ? native_renderer::nowUs() : 0u;
                    if (planned.run > 1)
                        glDrawArrays(GL_TRIANGLES, firstVertex, static_cast<GLsizei>(planned.mergedCount));
                    else
                        drawMesh(mesh, firstVertex);
                    if (phases.on)
                    {
                        const uint64_t took = native_renderer::nowUs() - secondStart;
                        phases.calls += took, ++phases.count;
                        stats.callUs += took, ++stats.calls;
                    }
                    stats.vertices += planned.run > 1 ? planned.mergedCount : mesh.count;
                    ++drawStats.alphaSecond;
                    ++shadowStats.alphaSecond;
                }
            }
            resolveShadows();
            if (rectLog)
            {
                logRect(backgroundOnly ? "background after its pass" : "picture after the pass");
                static const char *dumpDir = std::getenv("SOTC_BLOOM_RECT_DUMP");
                static int dumped = 0;
                if (dumpDir && dumped < 8)
                {
                    std::vector<uint8_t> px(static_cast<size_t>(target.width) * target.height * 4u);
                    glReadPixels(0, 0, static_cast<GLsizei>(target.width), static_cast<GLsizei>(target.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
                    char path[512];
                    std::snprintf(path, sizeof path, "%s/gl_%llu_%d_%s.ppm", dumpDir, (unsigned long long)bloomGameFrame, dumped++, backgroundOnly ? "background" : "picture");
                    if (FILE *f = std::fopen(path, "wb"))
                    {
                        std::fprintf(f, "P6\n%u %u\n255\n", target.width, target.height);
                        for (uint32_t y = 0; y < target.height; ++y)
                            for (uint32_t x = 0; x < target.width; ++x)
                                std::fwrite(&px[(static_cast<size_t>(target.height - 1u - y) * target.width + x) * 4u], 1, 3, f);
                        std::fclose(f);
                    }
                }
            }
            for (GLuint i = 1; i < 6; ++i)
                glDisableVertexAttribArray(i);
        };

        if (haveMirror)
            pass(reflection, false, 0, ~0u, false, true);
        // Round 87 (Tuning glBgSmall87, a console A/B candidate). With fog on,
        // the background layer (sky and far scenery, ~450 alpha-tested draws
        // at the title) is drawn into its own 512 x 448 target only to be
        // reduced to 256 x 256 (bgEnd's copy), which is all that is read
        // after it: the fog's colour source and the sky image stretched back
        // over the picture (the PS2 shows that reduced copy too). Drawn
        // straight at 256 x 256: ~3.5x fewer pixels for the layer's whole
        // overdraw (every draw blends and most cut out, so none is hidden).
        // Its pixels are sampled once instead of averaged from ~2 x 2, then
        // stretched back as before.
        if (fogOn)
        {
            if (native_renderer::tuning().glBgSmall87)
            {
                if (!backgroundLow.texture)
                    makeTarget(backgroundLow, 256, 256, true);
#if defined(PS2X_NATIVE_VITA)
                if (backgroundLow.fbo)
                    vglCeloDepthKeep(backgroundLow.fbo, depthKeep ? 0 : -1);
#endif
            }
            if (native_renderer::tuning().glBgSmall87 && backgroundLow.fbo)
            {
                pass(backgroundLow, true);
                copy(backgroundLow, backgroundSmall);
                ++bgSmallFrames;
            }
            else
            {
                pass(background, true);
                copy(background, backgroundSmall); // bgEnd: reduced to 256 x 256
            }
        }
#if defined(PS2X_NATIVE_VITA)
        const bool bloomOn = false; // the Mac's reads depth; the Vita's is vitaBloom
        const bool vitaBloom = bloom.enabled && bloom.color[3] > 0.0f;
        const bool dofOn = false;
#else
        // SOTC_BLOOM_VITA=1: the Vita's bloom (its mask in the picture's
        // alpha, the chain from there) instead of the Mac's, to measure it
        // against the GS on the same frames.
        static const bool vitaBloomOnMac = std::getenv("SOTC_BLOOM_VITA") && std::atoi(std::getenv("SOTC_BLOOM_VITA")) != 0;
        const bool bloomOn = bloom.enabled && !vitaBloomOnMac;
        const bool vitaBloom = vitaBloomOnMac && bloom.enabled && bloom.color[3] > 0.0f;
        const bool dofOn = dof.enabled && dof.slices > 0;
#endif
        auto doBloom = [&]
        {
            const float a[4] = {bloom.threshold, bloom.zA, bloom.zB, 0};
            const float maskColour[4] = {bloom.maskColor[0], bloom.maskColor[1], bloom.maskColor[2], 1};
            bindTexture(2, color.depth);
#if !defined(PS2X_NATIVE_VITA)
            static const bool oldBloom = std::getenv("SOTC_BLOOM_OLD") != nullptr;
            if (!oldBloom)
            {
                // The GS chain, as the Metal renderer does it (see there).
                quad(gsBloom[0], 15, QuadBlend::None, a, maskColour);
                bindTexture(0, gsBloom[0].texture);
                quad(gsBloom[1], 0, QuadBlend::None);
                bindTexture(0, gsBloom[1].texture);
                quad(gsBloom[2], 0, QuadBlend::None);
                const float alpha[4] = {gsBloomHistory ? bloom.temporal : 0.0f, 0, 0, 0};
                bindTexture(0, gsBloom[2].texture);
                bindTexture(1, gsBloom[4].texture);
                quad(gsBloom[3], 17, QuadBlend::None, alpha);
                const float taps = bloomTapAlpha();
                const float r64[4] = {64, taps, 0, 0}, r128[4] = {128, taps, 0, 0};
                bindTexture(0, gsBloom[3].texture);
                quad(gsBloom[4], 16, QuadBlend::None, r64);
                gsBloomHistory = true;
                quad(gsBloom[5], 16, QuadBlend::None, r128);
                bindTexture(0, gsBloom[5].texture);
                quad(gsBloom[6], 0, QuadBlend::None);
                for (int i = 0; i < 7; ++i)
                    bloomStage(kBloomStageNames[i], gsBloom[i]);
                float dtMac = 1.0f;
                if (bloom.zB > 0.0f)
                    dtMac = std::clamp(1.0f - 0.1f * (bloom.threshold + 1.0f - bloom.zA) / bloom.zB, 0.0f, 1.0f);
                logChain("mac-depth", dtMac);
                const float before = bloomLog() ? targetMean(colorPlain, 0) : 0.0f;
                const float scale[4] = {static_cast<float>(width) / 512.0f, static_cast<float>(height) / 448.0f, 0, 0};
                const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3]};
                bindTexture(0, gsBloom[6].texture);
                quad(colorPlain, 18, QuadBlend::Add, scale, colour);
                logAdd("mac-depth", colorPlain, before);
                return;
            }
#endif
            quad(bloomTex[0], 8, QuadBlend::None, a, maskColour);
            const int rounds = std::clamp(static_cast<int>(native_renderer::tuning().bloomRounds + 0.5f), 1, 8); // F10 panel (SOTC_BLOOM_ROUNDS)
            const float spreadGrow = native_renderer::tuning().bloomSpread;
            blur(rounds, spreadGrow);
            // Last frame's bloom over this one's with alpha `temporal` (98% history).
            const float amount[4] = {bloomHistory ? 1.0f - bloom.temporal : 1.0f, 0, 0, 0};
            bindTexture(0, bloomTex[0].texture);
            bindTexture(1, bloomTex[2].texture);
            quad(bloomTex[1], 2, QuadBlend::None, amount);
            std::swap(bloomTex[1], bloomTex[2]);
            bloomHistory = true;
            const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3]};
            bindTexture(0, bloomTex[2].texture);
            quad(colorPlain, 10, QuadBlend::Add, nullptr, colour);
        };
        auto doBlur = [&](const native_renderer::ScreenBlur &b, const Target *redrawn)
        {
            if (b.steps <= 0)
                return;
            if (!redrawn)
                snapshotColor();
            bindTexture(0, redrawn ? redrawn->texture : snapshot.texture);
            for (int i = 0; i < b.steps; ++i)
            {
                const float f = static_cast<float>(b.steps - i) / static_cast<float>(b.steps);
                const float params[4] = {(b.scale - 1.0f) * f + 1.0f, std::fabs(b.angle) >= 0.01570796f ? -b.angle * f : 0.0f,
                                         b.dx * f / 256.0f, -b.dy * f / 224.0f}; // PS2 pixels
                const float alpha[4] = {1.0f / static_cast<float>(i + 1), 0, 0, 0};
                quad(color, 3, QuadBlend::Over, alpha, nullptr, params, true);
            }
        };
        // Round 55 (blurApart): the blur on a second picture. The picture so
        // far (`color`, complete: nothing is drawn into it after this) is
        // copied into blurOut (where a copy smaller than the screen leaves
        // the picture as it was, as on the GS), the copies, read from it, are
        // laid over that, and the rest of the frame goes on in blurOut, on
        // the picture's depth: no mid-frame copy taken up again (the
        // console) and no second drawing of the scene (Vita3K's mid-frame
        // redraw, which smeared there). `hazeHere`: the heat haze's copy at
        // the same layer made from the same picture first. False: not done
        // (the depth could not be shared).
        auto doBlurApart = [&](const native_renderer::ScreenBlur &b, bool hazeHere) -> bool
        {
            if (!blurOut.texture)
                makeBlurOut(w, h);
#if defined(PS2X_NATIVE_VITA)
            bindTarget(color); // drawn into this frame: its depth exists
            if (!vglFramebufferShareDepth(blurOut.fbo, color.fbo))
                return false;
            keepDepth(blurOut, 3); // the picture's depth loaded; kept for the scenes after
#endif
            if (hazeHere)
            {
                const native_renderer::Tuning &t = native_renderer::tuning();
                const int hazeFrom = static_cast<int>(t.hazeSource + 0.5f);
                if (hazeFrom == 0)
                {
                    copy(color, hazeSmall); // the PS2's 256 x 256, blurred once through 128 x 128
                    copy(hazeSmall, refractionHalf);
                    copy(refractionHalf, hazeSmall);
                    hazeSourceTexture = hazeSmall.texture;
                }
                else
                    hazeSourceTexture = color.texture; // (source 1: the picture itself, full size; 2 reads the last frame)
            }
            const GLuint from = color.texture;
            const float none[4] = {0, 0, 0, 0};
            bindTexture(0, from);
            quad(blurOut, 13, QuadBlend::None, none); // a plain copy, its alpha too (the Vita bloom's mask)
            bindTexture(0, from);
            for (int i = 0; i < b.steps; ++i)
            {
                const float f = static_cast<float>(b.steps - i) / static_cast<float>(b.steps);
                const float params[4] = {(b.scale - 1.0f) * f + 1.0f, std::fabs(b.angle) >= 0.01570796f ? -b.angle * f : 0.0f,
                                         b.dx * f / 256.0f, -b.dy * f / 224.0f}; // PS2 pixels
                const float alpha[4] = {1.0f / static_cast<float>(i + 1), 0, 0, 0};
                quad(blurOut, 3, QuadBlend::Over, alpha, nullptr, params, true);
            }
            swapPicture(); // the rest of the frame: on blurOut (now `color`)
            bindTarget(color);
            return true;
        };
        // bossDeathEffect (layer 8): the layer drawn apart into a buffer cleared
        // to nothing, reduced to 256, 128, 64 and blurred, back to 256; laid
        // over the picture sharp (amountA) and blurred (amountB), in the tint.
        // The glow layer drawn on the depth of `depthOf` (the picture, or in
        // Vita3K the picture below layer 8 drawn again apart, see the
        // mid-frame redraw below), reduced and blurred.
        bool glowBuilt = false;
        // Round 73 (glFewScenes): `hoistGlow`, the glow layer and its
        // reductions are drawn at the soft shadows' break (same layer: they
        // read the picture's depth only, not its colour); `lastEvent`, the
        // effect being run is the frame's last: the scene of the picture its
        // write-back begins is the picture's last, which need not store its
        // depth / stencil (finalKeep, called just before that write-back).
        bool hoistGlow = false, lastEvent = false, glowWasHoisted = false;
        auto finalKeep = [&]
        {
            if (fewScenes && lastEvent)
                keepDepth(color, 1);
        };
        // Round 87 (Tuning glShmask87, a console A/B candidate). The soft
        // shadows' mask needs the picture put down (its alpha reduced 4 x 4
        // and blurred elsewhere), and the darkening takes it up again. Where
        // the next thing the frame does is the water's copy of the picture
        // (the altar: shadows and the glow layer at 8, the copy at 9, nothing
        // drawn into the picture between them), that scene of the picture
        // held only the darkening and the glow's composite before it was put
        // down again for the copy. Now the copy is made from the picture as
        // the mask left it and gets the darkening and the composite itself
        // (the same quads at 256 x 256), and the picture gets them at the
        // start of its next scene: one scene of the 512 x 448 picture (its
        // colour and depth / stencil loaded and stored) fewer a frame.
        bool maskFuse = false, maskPending = false, glowPending = false;
        auto resolveMask = [&](Target &t)
        {
            const bool picture = &t == &color;
            if (picture)
            {
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 0, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
            }
            const float amount[4] = {softAmount, 0, 0, 0};
            bindTexture(0, bloomTex[0].texture);
            quad(t, 9, QuadBlend::Darken, amount);
            if (picture)
            {
                glDisable(GL_STENCIL_TEST);
                ++shadowStats.darkens;
            }
            if (glowPending)
            {
                const std::pair<const Target *, float> layers[2] = {{&glowLayer, glow.amountA}, {&glowSmall[0], glow.amountB}};
                for (const auto &[source, amount] : layers)
                {
                    if (!(amount > 0.0f))
                        continue;
                    const float tint[4] = {glow.tint[0], glow.tint[1], glow.tint[2], amount};
                    bindTexture(0, source->texture);
                    quad(t, 14, QuadBlend::Over, tint);
                }
                if (picture)
                    glowPending = false;
            }
        };
        // The frame log's [gl:glow] line (round 24: the dead colossus's
        // coloured blobs on the console): how layer 8 went, every 120 frames
        // that have layer 8 draws.
        const char *glowHow = nullptr;
        auto glowLog = [&]
        {
            static uint64_t lastLog = 0;
            const bool recording = native_recorder::active(); // the scene recorder: every frame with layer 8 draws
            if (!recording && (!native_renderer::tuning().frameLog || arenaFrame < lastLog + 120u))
                return;
            unsigned n = 0, textured = 0, blends[4] = {};
            for (const Draw &d : draws)
                if (d.state.order == glow.order)
                {
                    ++n;
                    textured += d.state.texture ? 1u : 0u;
                    ++blends[std::min<unsigned>(static_cast<unsigned>(d.state.blend), 3u)];
                }
            if (!n)
                return;
            char line[320];
            std::snprintf(line, sizeof(line), "[gl:glow] frame %llu: layer %u %u draws (%u textured; blends %u %u %u %u) %s | tint %.3f %.3f %.3f amounts %.3f %.3f rounds %d | glow mode %.0f redraw %.0f emulator %d",
                          (unsigned long long)arenaFrame, glow.order, n, textured, blends[0], blends[1], blends[2], blends[3], glowHow ? glowHow : "not reached",
                          glow.tint[0], glow.tint[1], glow.tint[2], glow.amountA, glow.amountB, glow.rounds, native_renderer::tuning().glowVita,
                          native_renderer::tuning().midFrameRedraw, native_renderer::inEmulator() ? 1 : 0);
            if (recording)
                native_recorder::backendNote(arenaFrame, line);
            if (!native_renderer::tuning().frameLog || arenaFrame < lastLog + 120u)
                return;
            lastLog = arenaFrame;
            std::fprintf(stderr, "%s\n", line);
        };
        auto buildGlow = [&](Target &depthOf) -> bool
        {
#if defined(PS2X_NATIVE_VITA)
            if (!hoistGlow) // (round 73: hoisted, the picture was drawn into just before; a bind with no draw counts a target)
                bindTarget(depthOf); // drawn into this frame: its depth exists
            if (!vglFramebufferShareDepth(glowLayer.fbo, depthOf.fbo))
            {
                glowHow = "plain: the picture's depth could not be shared";
                return false;
            }
#else
            (void)depthOf;
#endif
            pass(glowLayer, false, glow.order, glow.order + 1u, true, false, true);
            auto reduce = [&](const Target &from, const Target &to, float shift)
            {
                const float offset[4] = {shift / static_cast<float>(from.width), shift / static_cast<float>(from.height), 0, 0};
                bindTexture(0, from.texture);
                quad(to, 13, QuadBlend::None, offset);
            };
            reduce(glowLayer, glowSmall[0], 0.0f);
            if (glow.rounds > 0)
            {
                reduce(glowSmall[0], glowSmall[1], 0.0f);
                if (glow.rounds > 1)
                {
                    reduce(glowSmall[1], glowSmall[2], 0.0f);
                    if (glow.rounds > 2)
                        for (int i = 0; i <= glow.rounds; ++i)
                        {
                            reduce(glowSmall[2], glowSmall[3], 0.5f);
                            reduce(glowSmall[3], glowSmall[2], -0.5f);
                        }
                    reduce(glowSmall[2], glowSmall[1], 0.0f);
                }
                reduce(glowSmall[1], glowSmall[0], 0.0f);
            }
            return true;
        };
        auto doGlow = [&]
        {
            if (!glowBuilt && !buildGlow(color))
            {
                pass(color, false, glow.order, glow.order + 1u, true); // as before round 19: plainly
                glowLog();
                return;
            }
            glowHow = glowWasHoisted ? "apart (on the picture's depth, drawn at the shadows' break)"
                      : glowBuilt    ? "apart (on the picture drawn again)"
                                     : "apart (on the picture's depth)";
            glowLog();
            if (maskPending) // round 87: after the deferred darkening (doRefraction)
            {
                glowPending = true;
                return;
            }
            finalKeep();
            const std::pair<const Target *, float> layers[2] = {{&glowLayer, glow.amountA}, {&glowSmall[0], glow.amountB}};
            for (const auto &[source, amount] : layers)
            {
                if (!(amount > 0.0f))
                    continue;
                const float tint[4] = {glow.tint[0], glow.tint[1], glow.tint[2], amount};
                bindTexture(0, source->texture);
                quad(color, 14, QuadBlend::Over, tint);
            }
        };
#if defined(PS2X_NATIVE_VITA)
        // Round 65c: the soft shadows' mask on the Vita. The counted pixels
        // (the volumes' stencil in `t`, which is bound) into t's alpha, the
        // colour kept (as bloomMask writes its mask), the count cleared; then
        // reduced 4 x 4 (mode 11, from the alpha) into bloomTex[0] (free: the
        // GS bloom chain has targets of its own) and blurred as the Mac's.
        static const int shadowBlurRounds = std::getenv("SOTC_SHADOW_BLUR") ? std::atoi(std::getenv("SOTC_SHADOW_BLUR")) : 1;
        auto shadowAlphaMask = [&](const Target &t)
        {
            bindTarget(t);
            useQuad(8);
            glEnable(GL_BLEND); // colour kept, alpha written
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            for (GLuint i = 1; i < 4; ++i)
                glDisableVertexAttribArray(i);
            const float q[4] = {-1.0f, 0, 0, 0};
            glUniform4fv(uniform(false, "uQuad"), 1, q);
            for (int k = 0; k < 2; ++k)
            {
                const float a[4] = {k ? 1.0f : 0.0f, 0, 0, 0};
                if (k)
                {
                    glEnable(GL_STENCIL_TEST);
                    glStencilMask(0xFF);
                    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
                    glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
                }
                glUniform4fv(uniform(false, "uA"), 1, a);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            glDisable(GL_STENCIL_TEST);
            useQuad(11);
            const float texel[4] = {1.0f / static_cast<float>(width), 1.0f / static_cast<float>(height), 0, 0};
            glUniform4fv(uniform(false, "uSize"), 1, texel);
            const float white[4] = {1, 1, 1, 1};
            bindTexture(0, t.texture);
            quad(bloomTex[0], 11, QuadBlend::None, nullptr, white);
            blur(shadowBlurRounds);
        };
#endif
        auto doShadow = [&]
        {
            // The counted pixels as a mask (testing the main picture's stencil), reduced 4 x 4, blurred.
#if defined(PS2X_NATIVE_VITA)
            {
                // Round 65c (see shadowAlphaMask): the mask from the picture
                // itself, then the picture darkened by it (taken up again: in
                // Vita3K too, the pictures and the timing as with the hard
                // shadows; the picture below the volumes drawn again apart for
                // the mask, as the mid-frame redraw does, cost Vita3K 2-4x the
                // frame time in the plains). The quad clears what is left of the count.
                shadowAlphaMask(color);
                // Round 73: the glow layer at the same layer drawn now, while
                // the picture is put down for the mask (see hoistGlow).
                if (hoistGlow && !glowBuilt)
                {
                    glowBuilt = glowWasHoisted = buildGlow(color);
                    glowHoisted += glowWasHoisted ? 1u : 0u;
                }
                if (maskFuse)
                {
                    // Round 87: the darkening waits for the water's copy (doRefraction).
                    maskPending = true;
                    return;
                }
                finalKeep();
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 0, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
                const float amount[4] = {softAmount, 0, 0, 0};
                bindTexture(0, bloomTex[0].texture);
                quad(color, 9, QuadBlend::Darken, amount);
                glDisable(GL_STENCIL_TEST);
                ++shadowStats.darkens;
                return;
            }
            if (shadowMaskOn())
            {
#endif
            bindTarget(shadowMask);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
#if defined(PS2X_NATIVE_VITA)
            }
            else
                glClearColor(0, 0, 0, 0); // (the state the clear left; the mask itself is not drawn here)
#endif
            glEnable(GL_STENCIL_TEST);
            glStencilMask(0xFF);
            glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
#if defined(PS2X_NATIVE_VITA)
            // The stencil count is in the main picture's own framebuffer:
            // write the mask over it, in its alpha... not on the Vita yet.
            glDisable(GL_STENCIL_TEST);
            return;
#endif
            {
                bindTarget(shadowMask);
                useQuad(6);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);
#if defined(PS2X_NATIVE_VITA)
                glUniform1f(uniform(false, "uMode"), 6.0f);
#else
                glUniform1i(uniform(false, "uMode"), 6);
#endif
                const float zero[4] = {0, 0, 0, 0};
                glUniform4fv(uniform(false, "uQuad"), 1, zero);
                glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            glDisable(GL_STENCIL_TEST);
            bindTexture(0, shadowMask.texture);
            quad(bloomTex[0], 7, QuadBlend::None);
            static const int rounds = std::getenv("SOTC_SHADOW_BLUR") ? std::atoi(std::getenv("SOTC_SHADOW_BLUR")) : 1;
            blur(rounds);
            const float amount[4] = {softAmount, 0, 0, 0};
            bindTexture(0, bloomTex[0].texture);
            quad(color, 9, QuadBlend::Darken, amount);
        };
        bool haveRefraction = false;
        uint32_t screenCopyOrder = 0;
        for (const Draw &d : draws)
        {
            if (d.state.refractionSample)
                haveRefraction = true;
            if (d.state.screenCopy && !screenCopyOrder)
                screenCopyOrder = d.state.order;
        }
        auto doRefraction = [&]
        {
            // The copies the chosen sources need (Tuning hazeSource / waterSource).
            const native_renderer::Tuning &t = native_renderer::tuning();
            const int haze = static_cast<int>(t.hazeSource + 0.5f), water = static_cast<int>(t.waterSource + 0.5f);
            if (haze == 2 && water == 2)
                return;
            // Round 45 (Tuning glFewCopies): when no draw reads the full-size
            // snapshot (source 1), the 256 x 256 copy is made from the picture
            // itself (the snapshot is the same pixels at the same size).
            const bool direct = t.glFewCopies && haze != 1 && water != 1;
            if (!direct)
                snapshotColor();
            if (haze == 0 || water == 0)
            {
                copy(direct ? color : snapshot, refraction);
#if defined(PS2X_NATIVE_VITA)
                if (maskPending)
                    resolveMask(refraction); // round 87: the copy gets what the picture will
#endif
                copy(refraction, refractionHalf);
                copy(refractionHalf, refraction);
            }
#if defined(PS2X_NATIVE_VITA)
            if (maskPending)
            {
                // Round 87: and the picture at the start of its next scene.
                maskPending = false;
                pendingResolve = [&] { resolveMask(color); };
                ++shadowStats.maskFused;
            }
#endif
            // Each draw binds its source (textured 3 / 4 in pass).
        };
        auto doFog = [&]
        {
            const float a[4] = {fog.start, fog.width, static_cast<float>(fog.steps), 0};
            bindTexture(0, backgroundSmall.texture);
            bindTexture(2, color.depth);
            quad(colorPlain, 12, QuadBlend::Over, a);
        };
        auto doDof = [&]
        {
            snapshotColor();
            copy(snapshot, refraction);
            for (int i = 0; i < dof.rounds; ++i)
            {
                copy(refraction, refractionHalf);
                copy(refractionHalf, refraction);
            }
            const float a[4] = {dof.zA, dof.zB, static_cast<float>(dof.focusZ), static_cast<float>(dof.slices)};
            bindTexture(0, refraction.texture);
            bindTexture(2, color.depth);
            quad(colorPlain, 11, QuadBlend::Over, a);
        };
        // The Vita's bloom (no depth textures): at the bloom's layer, the
        // picture's alpha becomes its mask: 0 everywhere, then 1 where a
        // full-screen quad at the threshold's depth passes the depth test
        // (what lies beyond GS Z threshold + 1: the sky and the far
        // landscape). The rest of the frame keeps that alpha; the mask is
        // reduced and blurred after the frame, and that glow is added here,
        // at the bloom's layer, the next frame (under the 2D drawn later, as
        // the PS2 adds it: one frame late, under the game's own 98% history).
        // SOTC_BLOOM_SAMEFRAME=0: round 18's order (the glow of frame n added
        // in frame n + 1). By default (round 19) the mask is reduced, blurred
        // and mixed with the history at the bloom's layer and added there,
        // the same frame, as the PS2's chain does: the glow no longer trails
        // the camera by a frame (one more split of the picture's scene).
        // Not where the picture is drawn again for mid-frame effects (Vita3K:
        // taken up again after drawing elsewhere, it came back as an older
        // frame): there the glow stays one frame late.
        const bool bloomSameFrameOn = native_renderer::tuning().bloomSameFrame; // (the debug menu flips it live, round 20)
        const float redrawSetting = native_renderer::tuning().midFrameRedraw;
        const bool bloomSameFrame = bloomSameFrameOn && !(redrawSetting >= 1.5f ? native_renderer::inEmulator() : redrawSetting >= 0.5f);
        // The mask (alpha) reduced 4 x 4 in the mask colour, blurred, over
        // last frame's at the game's temporal share: bloomTex[2].
        // Round 65b: the GS chain itself (the Mac's doBloom since round 20:
        // a 256 x 256 mask, 128 and 64 reductions, the 64 x 64 history step,
        // the 12-tap blurs onto 64 and 128, stretched over the picture), its
        // mask read from the picture's alpha. Before (SOTC_BLOOM_GS_VITA=0)
        // the Vita had the Mac's first glow: the mask reduced 4 x 4 at the
        // picture's size and blurred a few texels, at 0.46 of the strength
        // (set by eye): narrow, it left the bright sky's edges hard (the
        // pillars outside the shrine against a flat white band) where the
        // GS's spreads ~30 PS2 pixels over them.
        static const bool gsChain = !std::getenv("SOTC_BLOOM_GS_VITA") || std::atoi(std::getenv("SOTC_BLOOM_GS_VITA")) != 0;
#if !defined(PS2X_NATIVE_VITA)
        // (the Mac with SOTC_BLOOM_VITA=1: the CG modes' GLSL ports while these run)
        struct VitaModes
        {
            bool &on, was;
            explicit VitaModes(bool &f) : on(f), was(f) { on = true; }
            ~VitaModes() { on = was; }
        };
#define SOTC_VITA_MODES VitaModes vitaModesScope(vitaModes)
#else
#define SOTC_VITA_MODES
#endif
        auto gsBloomChain = [&]
        {
            SOTC_VITA_MODES;
            bloomStage("picture", color); // its alpha: the mask the chain reads
            if (bloomLog())
                std::fprintf(stderr, "[bloom:gl] f=%llu mask in the picture's alpha: %.2f%% of the pixels (alpha mean %.2f)\n",
                             (unsigned long long)bloomGameFrame, alphaCover(color), targetMean(color, 3));
            if (bloomShowMode() == 1 || (std::getenv("SOTC_BLOOM_SHOW") && std::atoi(std::getenv("SOTC_BLOOM_SHOW")) == 3))
            {
                if (!bloomShow.texture)
                    makeTarget(bloomShow, std::max(width / 4u, 1u), std::max(height / 4u, 1u), false);
                useQuad(11);
                const float texel[4] = {1.0f / static_cast<float>(width), 1.0f / static_cast<float>(height), 0, 0};
                glUniform4fv(uniform(false, "uSize"), 1, texel);
                const float white[4] = {1, 1, 1, 1};
                bindTexture(0, color.texture);
                quad(bloomShow, 11, QuadBlend::None, nullptr, white);
            }
            const float maskColour[4] = {bloom.maskColor[0], bloom.maskColor[1], bloom.maskColor[2], 1};
            useQuad(15);
            const float size[4] = {static_cast<float>(width), static_cast<float>(height), 0, 0};
            glUniform4fv(uniform(false, "uSize"), 1, size);
            bindTexture(0, color.texture);
            quad(gsBloom[0], 15, QuadBlend::None, nullptr, maskColour);
            bindTexture(0, gsBloom[0].texture);
            quad(gsBloom[1], 0, QuadBlend::None);
            bindTexture(0, gsBloom[1].texture);
            quad(gsBloom[2], 0, QuadBlend::None);
            const float alpha[4] = {gsBloomHistory ? bloom.temporal : 0.0f, 0, 0, 0};
            bindTexture(0, gsBloom[2].texture);
            bindTexture(1, gsBloom[4].texture);
            quad(gsBloom[3], 17, QuadBlend::None, alpha);
            const float taps = bloomTapAlpha();
            const float r64[4] = {64, taps, 0, 0}, r128[4] = {128, taps, 0, 0};
            bindTexture(0, gsBloom[3].texture);
            quad(gsBloom[4], 16, QuadBlend::None, r64);
            gsBloomHistory = true;
            quad(gsBloom[5], 16, QuadBlend::None, r128);
            bindTexture(0, gsBloom[5].texture);
            quad(gsBloom[6], 0, QuadBlend::None);
            bloomHistory = true;
            for (int i = 0; i < 7; ++i)
                bloomStage(kBloomStageNames[i], gsBloom[i]);
            float dtV = 1.0f;
            if (bloom.zB > 0.0f)
                dtV = std::clamp(1.0f - 0.1f * (bloom.threshold + 1.0f - bloom.zA) / bloom.zB, 0.0f, 1.0f);
            logChain(bloomSameFrame ? "vita-sameframe" : "vita-late", dtV);
        };
        // The chain's glow added to the picture (QuadBlend::Add keeps its alpha).
        auto gsBloomAdd = [&]
        {
            SOTC_VITA_MODES;
            useQuad(18);
            const float size[4] = {static_cast<float>(width), static_cast<float>(height), 0, 0};
            glUniform4fv(uniform(false, "uSize"), 1, size);
            const float scale[4] = {static_cast<float>(width) / 512.0f, static_cast<float>(height) / 448.0f, 0, 0};
            const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3]};
            const float before = bloomLog() ? targetMean(color, 0) : 0.0f;
            logRegions(color, "before the add");
            bindTexture(0, gsBloom[6].texture);
            quad(color, 18, QuadBlend::Add, scale, colour);
            logAdd(bloomSameFrame ? "vita-sameframe" : "vita-late", color, before);
            logRegions(color, "after the add");
        };
        auto bloomReduce = [&]
        {
            SOTC_VITA_MODES;
            if (gsChain)
            {
                gsBloomChain();
                return;
            }
            useQuad(11);
            const float texel[4] = {1.0f / static_cast<float>(width), 1.0f / static_cast<float>(height), 0, 0};
            glUniform4fv(uniform(false, "uSize"), 1, texel);
            const float maskColour[4] = {bloom.maskColor[0], bloom.maskColor[1], bloom.maskColor[2], 1};
            bindTexture(0, color.texture);
            quad(bloomTex[0], 11, QuadBlend::None, nullptr, maskColour);
            const int rounds = std::clamp(static_cast<int>(native_renderer::tuning().bloomRounds + 0.5f), 1, 8);
            blur(rounds, native_renderer::tuning().bloomSpread);
            const float amount[4] = {bloomHistory ? 1.0f - bloom.temporal : 1.0f, 0, 0, 0};
            bindTexture(0, bloomTex[0].texture);
            bindTexture(1, bloomTex[2].texture);
            quad(bloomTex[1], 2, QuadBlend::None, amount);
            std::swap(bloomTex[1], bloomTex[2]);
            bloomHistory = true;
        };
        auto bloomMask = [&]
        {
            SOTC_VITA_MODES;
            if (bloomHistory && !bloomSameFrame)
            {
                if (gsChain)
                    gsBloomAdd();
                else
                {
                    // Strength x 0.46: this glow is the narrow one the Mac had
                    // before the GS chain (its strength was set by eye then).
                    const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3] * 0.46f};
                    bindTexture(0, bloomTex[2].texture);
                    quad(color, 10, QuadBlend::Add, nullptr, colour);
                }
            }
            float dt = 1.0f;
            if (bloom.zB > 0.0f)
                dt = std::clamp(1.0f - 0.1f * (bloom.threshold + 1.0f - bloom.zA) / bloom.zB, 0.0f, 1.0f);
            bindTarget(color);
            useQuad(8);
            glEnable(GL_BLEND); // colour kept, alpha written
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);
            glDisable(GL_CULL_FACE);
            glDisable(GL_STENCIL_TEST);
            glDepthMask(GL_FALSE);
            glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            for (GLuint i = 1; i < 4; ++i)
                glDisableVertexAttribArray(i);
            for (int k = 0; k < 2; ++k)
            {
                const float a[4] = {k ? 1.0f : 0.0f, 0, 0, 0}, q[4] = {k ? 2.0f * dt - 1.0f : -1.0f, 0, 0, 0};
                if (k)
                {
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(GL_LEQUAL);
                }
                else
                    glDisable(GL_DEPTH_TEST);
                glUniform4fv(uniform(false, "uA"), 1, a);
#if defined(PS2X_NATIVE_VITA)
                glUniform4fv(uniform(false, "uQuad"), 1, q);
#else
                const float plain[4] = {0, 0, 0, 0};
                glUniform4fv(uniform(false, "uQuad"), 1, plain);
                glUniform1f(uniform(false, "uZ"), q[0]);
                glUniform1i(uniform(false, "uMode"), 19);
#endif
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
#if !defined(PS2X_NATIVE_VITA)
            glUniform1f(uniform(false, "uZ"), 0.0f);
#endif
            glDepthFunc(GL_LEQUAL);
            if (bloomSameFrame)
            {
                bloomReduce();
                finalKeep();
                if (gsChain)
                    gsBloomAdd();
                else
                {
                    const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3] * 0.46f};
                    bindTexture(0, bloomTex[2].texture);
                    quad(color, 10, QuadBlend::Add, nullptr, colour);
                }
                glEnable(GL_DEPTH_TEST); // as the frame's pass left it
                glDepthFunc(GL_LEQUAL);
                return;
            }
            keepAlpha = true;
        };
        struct Event
        {
            uint32_t order;
            int blur; // -1 bloom, -2 glow, -3 shadows, -4 refraction copy, -5 depth of field
            int prefix = -1; // midFrameRedraw: the target its picture so far was drawn into
        };
        std::vector<Event> events;
        if (dofOn)
            events.push_back({dof.order, -5});
#if !defined(PS2X_NATIVE_VITA)
        if (fogOn && fog.steps > 0)
            events.push_back({fog.order, -6}); // zFog reads depth: not on the Vita yet
#endif
        if (softShadows)
            events.push_back({softOrder, -3});
        if (haveRefraction)
            events.push_back({9u, -4});
        if (screenCopyOrder)
            events.push_back({screenCopyOrder, -4});
#if defined(PS2X_NATIVE_VITA)
        // The Vita: the glow layer shares the picture's depth through our
        // vitaGL's vglFramebufferShareDepth (round 19; before, layer 8 was
        // drawn plainly into the picture: the dead colossus in its raw
        // vertex colours, the sigils without their halo). Only when layer 8
        // has draws this frame: each use splits the picture's scene.
        // SOTC_GLOW_VITA: 0 round 18's plain layer 8; 1 (default) apart on the
        // console, plain in Vita3K; 2 apart in Vita3K too. Vita3K loses the
        // picture taken up again after drawing elsewhere, so there the glow
        // needs the picture below layer 8 drawn a second time (the mid-frame
        // redraw below): the right picture, but a cost the console does not
        // have, which would skew the calibrated timing (the shrine has
        // layer 8 draws in most frames).
        const int glowMode = static_cast<int>(native_renderer::tuning().glowVita + 0.5f); // (live, round 20)
        const float glowRedraw = native_renderer::tuning().midFrameRedraw;
        const bool glowRedraws = glowRedraw >= 1.5f ? native_renderer::inEmulator() : glowRedraw >= 0.5f;
        if (glow.enabled && (glowMode >= 2 || (glowMode == 1 && !glowRedraws)))
        {
            bool layerDrawn = false;
            for (const Draw &d : draws)
                if (d.state.order == glow.order)
                {
                    layerDrawn = true;
                    break;
                }
            if (layerDrawn)
                events.push_back({glow.order, -2});
        }
        else
        {
            // Layer 8 drawn plainly into the picture: why (the frame log's
            // [gl:glow] line and the scene recorder's note).
            glowHow = !glow.enabled ? "plain: the glow composite is off this frame"
                      : glowMode == 0 ? "plain: SOTC_GLOW_VITA=0"
                                      : "plain: emulator (SOTC_GLOW_VITA=2 draws it apart)";
            // Round 27: in Vita3K (the composite left out for the timing's
            // sake) the layer's blended draws at the composite's coverage,
            // 1 - (1 - A) (1 - B) of the sharp and blurred copies laid over
            // the picture, rather than at full: the dead colossus's black mask
            // and the shadow figures dark and see-through, not solid black
            // (the blur and the tint are left out). SOTC_GLOW_PLAIN_FADE=0: as
            // round 26.
            static const bool fade = !std::getenv("SOTC_GLOW_PLAIN_FADE") || std::atoi(std::getenv("SOTC_GLOW_PLAIN_FADE")) != 0;
            if (fade && glow.enabled && glowMode == 1)
            {
                const float coverage = 1.0f - (1.0f - std::clamp(glow.amountA, 0.0f, 1.0f)) * (1.0f - std::clamp(glow.amountB, 0.0f, 1.0f));
                for (Draw &d : draws)
                    if (d.state.order == glow.order && d.state.blend == native_renderer::Blend::Normal)
                        d.state.opacity *= coverage;
                glowHow = "plain: emulator, blended draws at the composite's coverage (SOTC_GLOW_VITA=2 draws it apart)";
            }
            glowLog();
        }
#else
        if (glow.enabled)
            events.push_back({glow.order, -2});
#endif
        if (bloomOn)
            events.push_back({bloom.order, -1});
        else if (vitaBloom)
            events.push_back({bloom.order, -8});
        else
            bloomHistory = gsBloomHistory = false;
        for (size_t i = 0; i < blurs.size(); ++i)
            events.push_back({blurs[i].order, static_cast<int>(i)});
        std::stable_sort(events.begin(), events.end(), [](const Event &x, const Event &y) { return x.order < y.order; });
        // Round 55: one screen blur in the frame is drawn on a second picture
        // (doBlurApart) instead of from the picture drawn again (Vita3K) or a
        // mid-frame copy (the console): Tuning::blurApart (SOTC_SBLUR_PINGPONG,
        // A/B `sblurapart`; the Vita's default, the Mac's OpenGL with =1).
        const int pingPong = native_renderer::tuning().blurApart ? 1 : 0;
        const bool blurApart = pingPong != 0 && blurs.size() == 1u && blurs[0].steps > 0;
        // The heat haze at the blur's layer reads the same picture: its copy made there too.
        const bool hazeWithBlur = blurApart && screenCopyOrder && screenCopyOrder == blurs[0].order && static_cast<int>(native_renderer::tuning().hazeSource + 0.5f) != 2;
        bool hazeDone = false;
        static const bool eventLog = std::getenv("SOTC_GL_EVENT_LOG") != nullptr; // debugging: each frame's effects (layer:kind)
        if (eventLog)
        {
            std::string line;
            for (const Event &ev : events)
                line += " " + std::to_string(ev.order) + ":" + std::to_string(ev.blur);
            unsigned layer8 = 0;
            for (const Draw &d : draws)
                layer8 += d.state.order == 8u ? 1u : 0u;
            std::fprintf(stderr, "[gl:events] frame %llu draws %u layer8 %u glow %d%s%s (blurs apart so far %llu)\n", (unsigned long long)arenaFrame,
                         static_cast<unsigned>(draws.size()), layer8, glow.enabled ? 1 : 0, line.c_str(), blurApart ? " blur-apart" : "",
                         (unsigned long long)blurApartCount);
        }
        // Mid-frame redraw (Tuning midFrameRedraw; the Vita's default): an
        // effect that reads the picture so far (water refraction, heat haze,
        // screen blur) gets it drawn again into a target of its own, before
        // the picture itself, which is then drawn in one go. Taking the
        // picture up again after drawing elsewhere lost it in Vita3K (what was
        // drawn before came back as an older frame; with the effects that add
        // light, the screen went white). Costs the draws below that layer a
        // second time, only in frames with such an effect.
        const float redrawMode = native_renderer::tuning().midFrameRedraw;
        if (redrawMode >= 1.5f ? native_renderer::inEmulator() : redrawMode >= 0.5f)
        {
            const native_renderer::Tuning &t = native_renderer::tuning();
            const int hazeFrom = static_cast<int>(t.hazeSource + 0.5f), waterFrom = static_cast<int>(t.waterSource + 0.5f);
            int k = 0, last = -1;
            uint32_t lastOrder = ~0u;
            static const bool share = !std::getenv("SOTC_GL_REDRAW_NOSHARE");
#if defined(PS2X_NATIVE_VITA)
            // The glow layer (round 19): the picture below layer 8 drawn
            // apart, the layer on its depth, so that the picture is not
            // taken up again after it (lost in Vita3K).
            for (const Event &ev : events)
                if (ev.blur == -2)
                {
                    if (!glowPrefix.texture)
                        makeTarget(glowPrefix, w, h, true), wantStencil(glowPrefix);
                    boundFbo = ~0u;
                    pass(glowPrefix, false, 0, ev.order, false);
                    glowBuilt = buildGlow(glowPrefix);
                    break;
                }
#endif
            for (Event &ev : events)
            {
                const bool water = ev.blur == -4 && haveRefraction && ev.order == 9u && waterFrom != 2;
                const bool haze = ev.blur == -4 && screenCopyOrder && ev.order == screenCopyOrder && hazeFrom != 2 && !hazeWithBlur;
                if (!((ev.blur >= 0 && !blurApart) || water || haze))
                    continue;
                // Effects at the same layer read the same picture (the draws
                // before that layer): drawn again once for all of them
                // (SOTC_GL_REDRAW_NOSHARE=1: once each, as before).
                if (share && last >= 0 && ev.order == lastOrder)
                    ev.prefix = last;
                else
                {
                    if (k >= 3)
                        continue;
                    Target &pre = prefix[k];
                    if (!pre.texture)
                        makeTarget(pre, w, h, true), wantStencil(pre);
                    boundFbo = ~0u;
                    pass(pre, false, 0, ev.order, false);
                    ev.prefix = last = k++;
                    lastOrder = ev.order;
                }
                Target &pre = prefix[ev.prefix];
                auto small = [&](Target &to) // the PS2's: 256 x 256, blurred once through 128 x 128
                {
                    copy(pre, to);
                    copy(to, refractionHalf);
                    copy(refractionHalf, to);
                };
                if (water)
                {
                    if (waterFrom == 0)
                        small(refraction);
                    waterSource = waterFrom == 0 ? refraction.texture : pre.texture;
                }
                if (haze)
                {
                    if (hazeFrom == 0)
                        small(hazeSmall);
                    hazeSourceTexture = hazeFrom == 0 ? hazeSmall.texture : pre.texture;
                }
            }
        }
        uint32_t from = 0;
        bool loaded = false;
        auto eventSkipped = [&](const Event &ev)
        {
            return (ev.prefix >= 0 && ev.blur == -4) || // its copies are made
                   (hazeWithBlur && ev.blur == -4 && screenCopyOrder && ev.order == screenCopyOrder && !(haveRefraction && ev.order == 9u)); // made by the blur at its layer (doBlurApart)
        };
#if defined(PS2X_NATIVE_VITA)
        // Round 87 (glShmask87): the shadows' event followed (after the glow
        // layer at its own layer, hoisted) by the water's copy at 9, made from
        // the picture itself, and nothing drawn into the picture between.
        if (softShadows && fewScenes && native_renderer::tuning().glShmask87)
        {
            const native_renderer::Tuning &tu = native_renderer::tuning();
            const int haze = static_cast<int>(tu.hazeSource + 0.5f), water = static_cast<int>(tu.waterSource + 0.5f);
            const bool directCopy = tu.glFewCopies && haze != 1 && water != 1 && (haze == 0 || water == 0);
            for (size_t i = 0; i < events.size() && directCopy; ++i)
            {
                if (events[i].blur != -3)
                    continue;
                size_t j = i + 1;
                const bool glowAt = j < events.size() && events[j].blur == -2 && events[j].order == events[i].order && !eventSkipped(events[j]);
                if (glowAt)
                    ++j;
                if (j < events.size() && events[j].blur == -4 && !eventSkipped(events[j]) && haveRefraction && events[j].order == 9u && events[j].order > events[i].order)
                {
                    bool empty = true;
                    for (const Draw &d : draws)
                        if (d.state.order >= events[i].order && d.state.order < events[j].order && !(glowAt && d.state.order == glow.order))
                        {
                            empty = false;
                            break;
                        }
                    maskFuse = empty;
                }
                break;
            }
        }
#endif
        size_t lastRun = events.size();
        for (size_t i = 0; i < events.size(); ++i)
            if (!eventSkipped(events[i]))
                lastRun = i;
        for (size_t ei = 0; ei < events.size(); ++ei)
        {
            const Event &ev = events[ei];
            if (eventSkipped(ev))
                continue;
            lastEvent = ei == lastRun;
#if defined(PS2X_NATIVE_VITA)
            // Round 73: the glow layer at the shadows' layer (the shadows' event comes first there).
            hoistGlow = fewScenes && ev.blur == -3 && ei + 1u < events.size() && events[ei + 1u].blur == -2 && events[ei + 1u].order == ev.order &&
                        !eventSkipped(events[ei + 1u]);
#endif
            // The picture's first scene starts from a clear (nothing to load);
            // every scene but its last is taken up again (stored). The scene a
            // pass begins takes the setting at its start: back to "both" as
            // soon as the first one is open (an effect's quads on the picture
            // begin the next one).
            keepDepth(color, loaded ? 3 : 2);
            pass(color, false, from, ev.order, loaded);
            keepDepth(color, 3);
            loaded = true;
            from = ev.order;
            spike.event = ev.blur;
            if (ev.blur == -8)
                bloomMask();
            else if (ev.blur == -6)
                doFog();
            else if (ev.blur == -5)
                doDof();
            else if (ev.blur == -4)
                doRefraction();
            else if (ev.blur == -3)
                doShadow();
            else if (ev.blur == -2)
            {
                doGlow();
                from = glow.order + 1u; // drawn apart
            }
            else if (ev.blur < 0)
                doBloom();
            else if (blurApart)
            {
                if (!doBlurApart(blurs[static_cast<size_t>(ev.blur)], hazeWithBlur))
                {
                    if (hazeWithBlur && !hazeDone) // (the depth could not be shared: the copies as the console made them)
                        doRefraction();
                    doBlur(blurs[static_cast<size_t>(ev.blur)], nullptr);
                }
                hazeDone = true;
                ++blurApartCount;
            }
            else
                doBlur(blurs[static_cast<size_t>(ev.blur)], ev.prefix >= 0 ? &prefix[ev.prefix] : nullptr);
        }
        // The last one: nothing reads its depth after it (the next frame clears).
        keepDepth(color, loaded ? 1 : 0);
        pass(color, false, from, ~0u, loaded);
        const bool bloomAdd = keepAlpha;
        keepAlpha = false;
        if (bloomAdd)
            bloomReduce(); // round 18's order: added by the next frame's bloomMask
        // SOTC_BLOOM_SHOW (debugging, the Vita's bloom): the picture replaced by
        // the mask the chain read (1, white = in the mask) or the glow it made
        // (2, gsBloom[6]), for pictures of what the chain works on (Vita3K's
        // window: its render targets cannot be read back).
        if (bloomShowMode() && gsBloomHistory)
        {
            const int show = bloomShowMode();
            const Target &from = show == 1 ? bloomShow
                                 : show == 5 ? gsBloom[2]
                                 : (show == 6 || show == 25) ? gsBloom[4]
                                 : show == 15 ? gsBloom[3]
                                 : show == 7 ? backgroundSmall
                                             : gsBloom[6];
            if (from.texture)
            {
                bindTexture(0, from.texture);
                quad(color, 0, QuadBlend::None);
            }
        }
        // Round 45 (Tuning glFewCopies, the Vita): the host's copy below is the
        // last frame shown; without such a host (the Mac's read-back) a copy of its own.
#if defined(PS2X_NATIVE_VITA)
        const bool previousIsShown = native_renderer::tuning().glFewCopies && static_cast<bool>(native_renderer::detail::presentHook());
#else
        const bool previousIsShown = false;
#endif
        if (!previousIsShown)
        {
            copy(colorPlain, previous);
            previousTexture = previous.texture;
        }
        // SOTC_GL_SHOTS=<n> (debugging, round 51): every n-th frame drawn
        // saved as shot_<frame>.ppm (colour) and shot_<frame>_a.pgm (alpha) in
        // SOTC_GL_SHOT_DIR (the Vita: ux0:data/celosia/shots), for pictures of
        // the Vita build (the console: Vita3K never writes its render targets
        // back to the guest's memory, so there they come out black).
        static const uint64_t shotEvery = std::getenv("SOTC_GL_SHOTS") ? std::strtoull(std::getenv("SOTC_GL_SHOTS"), nullptr, 10) : 0u;
        if (shotEvery && arenaFrame % shotEvery == 0u)
        {
#if defined(PS2X_NATIVE_VITA)
            static const std::string dir = std::getenv("SOTC_GL_SHOT_DIR") ? std::getenv("SOTC_GL_SHOT_DIR") : "ux0:data/celosia/shots";
#else
            static const std::string dir = std::getenv("SOTC_GL_SHOT_DIR") ? std::getenv("SOTC_GL_SHOT_DIR") : "shots";
#endif
            std::vector<uint8_t> px(static_cast<size_t>(w) * h * 4u);
            glFinish(); // (vitaGL reads the target's memory: the GPU done with it first)
            glBindFramebuffer(GL_FRAMEBUFFER, colorPlain.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = ~0u;
            for (int k = 0; k < 2; ++k)
            {
                const std::string path = dir + "/shot_" + std::to_string(arenaFrame) + (k ? "_a.pgm" : ".ppm");
                if (FILE *f = std::fopen(path.c_str(), "wb"))
                {
                    std::fprintf(f, "%s\n%u %u\n255\n", k ? "P5" : "P6", w, h);
                    std::vector<uint8_t> line(static_cast<size_t>(w) * (k ? 1u : 3u));
                    for (uint32_t y = 0; y < h; ++y)
                    {
                        const uint8_t *src = &px[static_cast<size_t>(h - 1u - y) * w * 4u];
                        for (uint32_t x = 0; x < w; ++x)
                        {
                            if (k)
                                line[x] = src[x * 4u + 3u];
                            else
                                std::memcpy(&line[x * 3u], &src[x * 4u], 3);
                        }
                        std::fwrite(line.data(), 1, line.size(), f);
                    }
                    std::fclose(f);
                }
            }
        }


        if (const auto &hook = native_renderer::detail::presentHook())
        {
            // The host shows a copy of its own: the next frame starts drawing
            // into `color` before the GPU has put this one on the screen.
            // Round 73 (glFewScenes, the Vita): the picture itself, as drawn:
            // the next frame draws into the other one (colorAlt, swapped at
            // its start), and the GPU runs the scenes in order (the host's
            // scene reads it before the frame after next draws into it
            // again). Not a frame that ended on blurOut (a blur drawn apart:
            // the next one with a blur draws into it mid-frame): copied.
            GLuint shownTexture = 0;
            if (fewScenes && previousIsShown && colorAlt.fbo && !pictureSwapped)
            {
                shownTexture = colorPlain.texture;
                ++shownDirect;
            }
            else
            {
                shownIndex ^= 1;
                if (!shownFrame[shownIndex].fbo) // (glFewScenes: made at their first use)
                    makeTarget(shownFrame[shownIndex], w, h, false);
                copy(colorPlain, shownFrame[shownIndex]);
                shownTexture = shownFrame[shownIndex].texture;
                ++shownCopied;
            }
            if (previousIsShown)
                previousTexture = shownTexture;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = boundProgram = ~0u;
            phaseEnd();
            pendingTexture = shownTexture;
            pendingWidth = w;
            pendingHeight = h;
            presentPending = true;
            noteScenes();
            if (!deferPresent)
                presentNow();
            return;
        }
        phaseEnd();
        // Read back, top row first (the GL picture is y-up).
        std::vector<uint8_t> pixels(static_cast<size_t>(w) * h * 4u);
        glBindFramebuffer(GL_FRAMEBUFFER, color.fbo);
        glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (!bloomStages.empty())
        {
            std::lock_guard<std::mutex> stageLock(g_bloomStageMutex);
            g_bloomStagesOut = bloomStages;
        }
        std::lock_guard<std::mutex> lock(outputMutex);
        output.resize(pixels.size());
        const size_t row = static_cast<size_t>(w) * 4u;
        for (uint32_t y = 0; y < h; ++y)
            std::memcpy(output.data() + static_cast<size_t>(y) * row, pixels.data() + static_cast<size_t>(h - 1u - y) * row, row);
        outputWidth = w;
        outputHeight = h;
        hasOutput = true;
        noteScenes();
    }

    // The game thread's side of the renderer (every Backend call) and the
    // render thread. The game thread records a frame (its draws, per-frame
    // vertices and indices, skin palettes, effects, uploads) apart from the
    // one being drawn; endFrame hands it over and goes on with the next,
    // waiting only while the render thread still draws the one before (at
    // most one frame in flight). The render thread makes every GL call: the
    // uploads, the frame's passes, the present (the host's swap, with its
    // menus and overlays). Tuning renderThread off: endFrame also waits for
    // its own frame to be drawn (the timing of one thread).
    class GlBackend final : public native_renderer::detail::Backend
    {
    public:
        explicit GlBackend(Renderer *renderer) : r(renderer)
        {
            const auto mb = [](const char *name, double fallback)
            {
                const char *v = std::getenv(name);
                return static_cast<uint64_t>((v ? std::atof(v) : fallback) * 1048576.0);
            };
#if defined(PS2X_NATIVE_VITA)
            // vitaGL's GPU pools (112 MB CDRAM + ~100 MB of main RAM) also
            // hold the render targets and its own buffers; past them it takes
            // the newlib heap, which the game needs (see evictKept).
            m_cacheBudget = mb("SOTC_GL_CACHE_MB", 96.0);
            m_heapHigh = mb("SOTC_GL_HEAP_MB", 104.0);
            m_heapLimit = static_cast<uint64_t>(_newlib_heap_size_user) - mb("SOTC_GL_HEAP_RESERVE_MB", 12.0);
            m_flushBytes = mb("SOTC_GL_UPLOAD_FLUSH_MB", 8.0);
            m_memLog = true;
#else
            m_cacheBudget = mb("SOTC_GL_CACHE_MB", 0.0);
            if (!m_cacheBudget)
                m_cacheBudget = ~0ull;
            m_heapHigh = mb("SOTC_GL_HEAP_MB", 0.0);
            m_heapLimit = 0;
            m_flushBytes = mb("SOTC_GL_UPLOAD_FLUSH_MB", 64.0);
            m_memLog = std::getenv("SOTC_GL_MEM_LOG") || std::getenv("SOTC_GL_CACHE_LOG");
#endif
            m_startUs = native_renderer::nowUs();
        }

        // Starts the render thread. makeCurrent: called there first (macOS:
        // the context becomes that thread's); initThere: the renderer is
        // built there too (else it was, on the caller: the Vita's CG compiler
        // needs the main thread's stack).
        bool start(std::function<void()> makeCurrent, bool initThere)
        {
#if defined(PS2X_NATIVE_VITA)
            // std::thread takes the pthread default stack (32 KB on the Vita).
            const unsigned int stackDefault = _pthread_stack_default_user;
            _pthread_stack_default_user = 512u * 1024u;
#endif
            std::thread([this, makeCurrent = std::move(makeCurrent), initThere] { loop(makeCurrent, initThere); }).detach();
#if defined(PS2X_NATIVE_VITA)
            _pthread_stack_default_user = stackDefault;
#endif
            std::unique_lock<std::mutex> lock(m_mutex);
            while (!m_started)
                m_idle.wait_for(lock, kRecheck);
            return m_initOk;
        }

        bool hasMesh(uint64_t id) override { return m_knownMeshes.count(id) != 0; }
        uint64_t meshEvictions() override { return m_meshEvictions; }
        void uploadMesh(uint64_t id, const native_renderer::Vertex *vertices, uint32_t count, bool perFrame) override
        {
            if (perFrame && !noArena())
            {
                const uint32_t first = static_cast<uint32_t>(m_rec.arena.size());
                if (!m_rec.arena.append(vertices, count))
                {
                    ++m_skipped; // no room: this frame goes without it
                    return;
                }
                m_rec.frameMeshes.push_back({id, first, count, 0u, false});
                return;
            }
            const auto *from = reinterpret_cast<const uint8_t *>(vertices);
            if (!queue(Renderer::Upload::MeshKind, id, count, from, count * sizeof(native_renderer::Vertex)))
                return;
            if (!perFrame)
                m_knownMeshes.insert(id);
            flushIfLarge();
        }
        // SOTC_GL_UNINDEX=1 (testing, round 65e): the indexed per-frame meshes
        // written out as triangle lists in the frame's arena.
        static bool unindexFrameMeshes()
        {
            static const bool on = std::getenv("SOTC_GL_UNINDEX") && std::atoi(std::getenv("SOTC_GL_UNINDEX")) != 0;
            return on;
        }
        std::vector<native_renderer::Vertex> m_unindexScratch;
        native_renderer::Vertex *frameMesh(uint32_t count) override
        {
            if (noArena())
                return Backend::frameMesh(count);
            m_pendingFirst = m_rec.arena.size();
            m_pendingIndexFirst = m_rec.indexArena.size();
            return m_rec.arena.grow(count);
        }
        uint32_t *frameMeshIndices(uint32_t count, uint32_t &base) override
        {
            if (noArena())
                return Backend::frameMeshIndices(count, base);
            base = static_cast<uint32_t>(m_pendingFirst);
            m_rec.indexArena.count = m_pendingIndexFirst;
            return m_rec.indexArena.grow(count);
        }
        void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount) override
        {
            if (noArena())
            {
                Backend::commitFrameMesh(id, count, indexCount);
                return;
            }
            if (indexCount && unindexFrameMeshes())
            {
                // The triangle list written out in the arena in place of the
                // indexed mesh (see unindexFrameMeshes).
                m_unindexScratch.assign(m_rec.arena.data() + m_pendingFirst, m_rec.arena.data() + m_pendingFirst + count);
                m_rec.arena.count = m_pendingFirst;
                native_renderer::Vertex *to = m_rec.arena.grow(indexCount);
                const uint32_t *idx = m_rec.indexArena.data() + m_pendingIndexFirst;
                for (uint32_t i = 0; i < indexCount; ++i)
                {
                    const uint32_t k = idx[i] - static_cast<uint32_t>(m_pendingFirst);
                    to[i] = k < count ? m_unindexScratch[k] : m_unindexScratch[0];
                }
                m_rec.indexArena.count = m_pendingIndexFirst;
                m_rec.frameMeshes.push_back({id, static_cast<uint32_t>(m_pendingFirst), indexCount, 0u, false});
                return;
            }
            m_rec.frameMeshes.push_back({id, static_cast<uint32_t>(m_pendingFirst), indexCount ? indexCount : count, static_cast<uint32_t>(m_pendingIndexFirst),
                                         indexCount != 0u});
            m_rec.arena.count = m_pendingFirst + count; // the room not used goes back
            m_rec.indexArena.count = m_pendingIndexFirst + indexCount;
        }
        void cancelFrameMesh() override
        {
            if (noArena())
                return;
            m_rec.arena.count = m_pendingFirst;
            m_rec.indexArena.count = m_pendingIndexFirst;
        }
        bool canSkin() override { return r->skinProgram != 0; } // set once by init
        void uploadSkinnedMesh(uint64_t id, const native_renderer::SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount) override
        {
            const auto *from = reinterpret_cast<const uint8_t *>(vertices);
            Renderer::Upload *u = queue(Renderer::Upload::SkinnedKind, id, count, from, count * sizeof(native_renderer::SkinVertex), indexCount * 4u);
            if (!u)
                return;
            try
            {
                u->indices.assign(indices, indices + indexCount);
            }
            catch (const std::bad_alloc &)
            {
                m_recUploadBytes -= u->bytes.size() + indexCount * 4u;
                m_rec.uploads.pop_back();
                ++m_skipped;
                return;
            }
            m_knownMeshes.insert(id);
            flushIfLarge();
        }
        uint32_t framePalette(const float *qwords, uint32_t count) override
        {
            const size_t at = m_rec.palettes.size();
            m_rec.palettes.insert(m_rec.palettes.end(), qwords, qwords + count * 4u);
            return static_cast<uint32_t>(at / 4u) + 1u;
        }
        void rewriteFramePalette(uint32_t handle, const float *qwords, uint32_t count) override
        {
            const size_t at = static_cast<size_t>(handle - 1u) * 4u;
            if (handle && at + static_cast<size_t>(count) * 4u <= m_rec.palettes.size())
                std::memcpy(m_rec.palettes.data() + at, qwords, static_cast<size_t>(count) * 16u);
        }
        bool hasTexture(uint64_t id) override { return m_knownTextures.count(id) != 0; }
        void uploadTexture(uint64_t key, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels) override
        {
            if (!width || !height)
                return;
            levels = std::max(1u, levels);
            size_t bytes = 0;
            for (uint32_t level = 0, w = width, h = height; level < levels; ++level)
            {
                bytes += static_cast<size_t>(w) * h * 4u;
                w = std::max(1u, w / 2u);
                h = std::max(1u, h / 2u);
            }
            Renderer::Upload *u = queue(Renderer::Upload::TextureKind, key, 0u, rgba, bytes);
            if (!u)
                return; // not known: asked for again next time
            u->width = width;
            u->height = height;
            u->levels = levels;
            m_knownTextures.insert(key);
            flushIfLarge();
        }
        void beginFrame() override { clearFrame(); }
        void draw(uint64_t mesh, const float clip[16], const native_renderer::DrawState &state) override { m_rec.draws.emplace_back(mesh, clip, state); }
        void drawMulti(const uint64_t *meshes, uint32_t count, const float clip[16], const native_renderer::DrawState &state) override
        {
            if (count == 0u)
                return;
            Draw &d = m_rec.draws.emplace_back(meshes[0], clip, state);
            if (count == 1u)
                return;
            d.extraFirst = static_cast<uint32_t>(m_rec.extraMeshes.size());
            d.extraCount = count - 1u;
            m_rec.extraMeshes.insert(m_rec.extraMeshes.end(), meshes + 1, meshes + count);
        }
        void setFog(const native_renderer::Fog &fog) override { m_rec.fog = fog; }
        void setBloom(const native_renderer::Bloom &bloom) override { m_rec.bloom = bloom; }
        void addScreenBlur(const native_renderer::ScreenBlur &blur) override { m_rec.blurs.push_back(blur); }
        void setGlowComposite(const native_renderer::GlowComposite &glow) override { m_rec.glow = glow; }
        void setDepthOfField(const native_renderer::DepthOfField &dof) override { m_rec.dof = dof; }
        void endFrame(uint32_t width, uint32_t height) override
        {
            const bool timed = native_renderer::tuning().frameTiming();
            const uint64_t start = timed ? native_renderer::nowUs() : 0u;
            std::unique_lock<std::mutex> lock(m_mutex);
            waitData(lock);
            // The render thread waits: what it holds changes hands. What the
            // last frame drawn cost goes into this frame's counters.
            if (timed)
            {
                native_renderer::FrameCounters &c = native_renderer::frameCounters();
                c.renderUs += r->stats.renderUs;
                c.presentUs += r->stats.presentUs;
                c.renderLockUs += r->stats.lockUs;
                c.renderUploadUs += r->stats.uploadUs;
                c.renderDrawUs += r->stats.drawUs;
                c.renderSweepUs += r->stats.sweepUs;
                c.renderPlanUs += r->stats.planUs;
                c.renderPassUs += r->stats.passUs;
                c.renderCallUs += r->stats.callUs;
                c.renderCalls += static_cast<uint32_t>(r->stats.calls);
                c.renderRunCopies += r->stats.runCopies;
                c.renderRunScans += r->stats.runScans;
                c.targets += r->stats.targets;
                c.scenes += r->stats.scenes;
                c.depthKB += r->stats.depthKB;
                c.vertices += r->stats.vertices;
            }
            r->stats = {};
            // SOTC_GL_FREEZE=n[,d] (round 73, debugging): from the n-th frame
            // handed over that has d draws or more (default 1), the render
            // thread draws that frame again and again (the game goes on; its
            // uploads are still applied): a still picture whatever the game
            // does, for window captures of the same frame with a switch on and
            // off (SOTC_AB_FLAG; Vita3K's targets cannot be read back).
            static const std::pair<uint64_t, uint64_t> freezeAt = []
            {
                unsigned long long n = 0, d = 1;
                if (const char *e = std::getenv("SOTC_GL_FREEZE"))
                    std::sscanf(e, "%llu,%llu", &n, &d);
                return std::pair<uint64_t, uint64_t>(n, d);
            }();
            ++m_handOvers;
            if (freezeAt.first && !m_frozen && m_handOvers >= freezeAt.first && m_rec.draws.size() >= freezeAt.second && !m_rec.hold)
            {
                m_frozen = true;
                std::fprintf(stderr, "[gl:freeze] hand-over %llu: this frame (%u draws) drawn from now on\n", (unsigned long long)m_handOvers, static_cast<unsigned>(m_rec.draws.size()));
            }
            else if (m_frozen)
            {
                r->uploads.swap(m_rec.uploads);
                r->hold = false;
                m_rec.hold = false;
                clearFrame();
                m_rec.uploads.clear();
                m_queued = true;
                m_work.notify_one();
                if (!native_renderer::tuning().renderThread)
                    waitIdle(lock);
                return;
            }
            std::swap(r->draws, m_rec.draws);
            std::swap(r->extraMeshes, m_rec.extraMeshes);
            std::swap(r->arena, m_rec.arena);
            std::swap(r->indexArena, m_rec.indexArena);
            std::swap(r->palettes, m_rec.palettes);
            std::swap(r->blurs, m_rec.blurs);
            r->uploads.swap(m_rec.uploads); // the render thread left its list empty
            r->frameMeshes.swap(m_rec.frameMeshes);
            r->fog = m_rec.fog;
            r->bloom = m_rec.bloom;
            r->bloomGameFrame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
            r->glow = m_rec.glow;
            r->dof = m_rec.dof;
            r->hold = m_rec.hold;
            m_rec.hold = false;
            clearFrame();
            m_rec.uploads.clear();
            m_uploadPeak = std::max(m_uploadPeak, m_frameUploadBytes + m_recUploadBytes);
            m_frameUploadBytes = m_recUploadBytes = 0;
            evictKept();
            trimRecording();
            if (m_memLog && native_renderer::nowUs() >= m_nextMemLog)
            {
                m_nextMemLog = native_renderer::nowUs() + 5000000u;
                logMemory();
            }
            m_width = width;
            m_height = height;
            m_queued = true;
            m_work.notify_one();
            const bool threaded = native_renderer::tuning().renderThread;
            if (!threaded)
                waitIdle(lock);
            if (timed)
            {
                native_renderer::FrameCounters &c = native_renderer::frameCounters();
                c.waitUs += native_renderer::nowUs() - start;
                c.renderThread = threaded;
            }
        }
        void finishFrames() override
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            waitIdle(lock);
        }
        bool timesItself() override { return true; }
        void holdFrame() override { m_rec.hold = true; }
        bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height) override
        {
            std::lock_guard<std::mutex> lock(r->outputMutex);
            if (!r->hasOutput)
                return false;
            rgba = r->output;
            width = r->outputWidth;
            height = r->outputHeight;
            return true;
        }

    private:
        // vitaGL is one GL for every thread: the host and this take turns.
        struct Lock
        {
#if defined(PS2X_NATIVE_VITA)
            std::lock_guard<std::recursive_mutex> guard{ps2xVitaGlMutex()};
#endif
        };
        static bool noArena()
        {
            static const bool on = std::getenv("SOTC_GL_NOARENA") != nullptr; // debugging: one buffer per mesh
            return on;
        }

        // The frame the game thread is making (Renderer has the same fields
        // for the frame being drawn; endFrame swaps them).
        struct Recording
        {
            std::vector<Draw> draws;
            std::vector<uint64_t> extraMeshes; // drawMulti's (Draw::extraFirst)
            Renderer::Arena<native_renderer::Vertex> arena;
            Renderer::Arena<uint32_t> indexArena;
            std::vector<float> palettes;
            std::vector<native_renderer::ScreenBlur> blurs;
            native_renderer::Fog fog;
            native_renderer::Bloom bloom;
            native_renderer::GlowComposite glow;
            native_renderer::DepthOfField dof;
            bool hold = false;
            std::vector<Renderer::Upload> uploads; // kept until handed over (uploads come between frames too)
            std::vector<Renderer::FrameMesh> frameMeshes;
        };

        void clearFrame()
        {
            m_rec.draws.clear();
            m_rec.extraMeshes.clear();
            m_rec.arena.clear();
            m_rec.indexArena.clear();
            m_rec.palettes.clear();
            m_rec.blurs.clear();
            m_rec.frameMeshes.clear();
        }

        // An upload with its own copy of the data (the caller's buffer is
        // reused at once). nullptr when the heap has no room for it (the
        // caller does not count it as uploaded: it is asked for again).
        Renderer::Upload *queue(Renderer::Upload::Kind kind, uint64_t id, uint32_t count, const uint8_t *data, size_t bytes, size_t extra = 0)
        {
            if (m_heapLimit && m_heap + m_recUploadBytes + bytes + extra > m_heapLimit)
            {
                m_heap = heapUsed(); // the last reading may be old
                if (m_heap + m_recUploadBytes + bytes + extra > m_heapLimit)
                {
                    ++m_skipped;
                    return nullptr;
                }
            }
            bool added = false;
            try
            {
                m_rec.uploads.emplace_back();
                added = true;
                Renderer::Upload &u = m_rec.uploads.back();
                u.kind = kind;
                u.id = id;
                u.count = count;
                u.bytes.assign(data, data + bytes);
            }
            catch (const std::bad_alloc &)
            {
                if (added)
                    m_rec.uploads.pop_back();
                ++m_skipped;
                return nullptr;
            }
            m_recUploadBytes += bytes + extra;
            return &m_rec.uploads.back();
        }

        // A scene load asks for hundreds of textures in one frame: each
        // copy waited in the queue until the hand-over (the frame being
        // recorded and the one drawn held up to ~2 x 30 MB). Past
        // m_flushBytes what is queued goes to the render thread at once, on
        // a list of its own (m_side) that it applies as soon as it is free
        // (before the next frame's draws), and the copies go. The game thread
        // goes on (it used to wait for the frame being drawn, then for the
        // uploads); it waits only while 4 x m_flushBytes are still on that
        // list. SOTC_GL_FLUSH_WAIT=1: the earlier way.
        void flushIfLarge()
        {
            if (m_recUploadBytes >= m_flushBytes)
                flushUploads();
        }
        void flushUploads()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            static const bool waitOld = std::getenv("SOTC_GL_FLUSH_WAIT") != nullptr;
            if (waitOld)
            {
                waitIdle(lock);
                // The render thread is idle: the frame handed over is drawn, its
                // upload list empty.
                r->uploads.swap(m_rec.uploads);
                m_rec.uploads.clear();
                m_uploadsOnly = true;
                m_queued = true;
                m_work.notify_one();
                waitIdle(lock);
            }
            else
            {
                while (m_sideBytes > 4u * m_flushBytes)
                    m_idle.wait_for(lock, kRecheck);
                for (Renderer::Upload &u : m_rec.uploads)
                    m_side.push_back(std::move(u));
                m_rec.uploads.clear();
                m_sideBytes += m_recUploadBytes;
                m_work.notify_one();
            }
            m_frameUploadBytes += m_recUploadBytes;
            m_recUploadBytes = 0;
            ++m_flushes;
        }

        // The recording's arenas and draw list grow to the largest frame and
        // kept that size: given back when the last ~600 frames needed less
        // than half (a scene's peak does not stay for the rest of the run).
        void trimRecording()
        {
            m_vertexPeak = std::max(m_vertexPeak, r->arena.size());
            m_indexPeak = std::max(m_indexPeak, r->indexArena.size());
            m_drawPeak = std::max(m_drawPeak, r->draws.size() + r->extraMeshes.size());
            if (++m_trimFrames >= 600u)
            {
                m_vertexPeakPrev = m_vertexPeak;
                m_indexPeakPrev = m_indexPeak;
                m_drawPeakPrev = m_drawPeak;
                m_vertexPeak = m_indexPeak = m_drawPeak = 0;
                m_trimFrames = 0;
            }
            const size_t vertices = std::max(m_vertexPeak, m_vertexPeakPrev), indices = std::max(m_indexPeak, m_indexPeakPrev),
                         draws = std::max(m_drawPeak, m_drawPeakPrev);
            if (m_rec.arena.capacity > vertices * 2u + 65536u)
                m_rec.arena.trim(vertices + vertices / 4u + 4096u);
            if (m_rec.indexArena.capacity > indices * 2u + 65536u)
                m_rec.indexArena.trim(indices + indices / 4u + 4096u);
            if (m_rec.draws.capacity() > draws * 2u + 1024u)
            {
                std::vector<Draw>().swap(m_rec.draws);
                try
                {
                    m_rec.draws.reserve(draws + draws / 4u + 256u);
                }
                catch (const std::bad_alloc &)
                {
                }
            }
        }

        // [gl:mem] every 5 s (the Vita; SOTC_GL_MEM_LOG elsewhere): what each
        // pool holds, next to the heap. GPU: textures and kept meshes (vitaGL's
        // pools; "in heap": vitaGL had to put them in the newlib heap). Host:
        // uploads waiting (the most one frame queued), the frame arenas and
        // draw lists (both frames), the hooks' skinned packets (bind poses).
        void logMemory()
        {
            // The working set: drawn in the last 120 frames (what a budget
            // could not evict).
            const uint64_t recent = r->arenaFrame > 120u ? r->arenaFrame - 120u : 0u;
            uint32_t kept = 0;
            uint64_t recentBytes = 0;
            r->meshes.forEach([&](uint64_t, const Mesh &m)
            {
                if (!m.dynamic)
                {
                    ++kept;
                    recentBytes += m.used >= recent ? m.bytes : 0u;
                }
            });
            r->textures.forEach([&](uint64_t, const Texture &t) { recentBytes += t.used >= recent ? t.bytes : 0u; });
            const uint64_t skinnedPackets = native_renderer::frontendMemory();
            const double MB = 1048576.0;
            const uint64_t arenas = r->arena.bytes() + r->indexArena.bytes() + m_rec.arena.bytes() + m_rec.indexArena.bytes();
            const uint64_t lists = (r->draws.capacity() + m_rec.draws.capacity()) * sizeof(Draw) + (r->palettes.capacity() + m_rec.palettes.capacity()) * sizeof(float);
            m_heap = heapUsed();
            // Round 34: the mesh pools themselves (each 2.3 MB of vitaGL
            // memory however little of it the meshes use; a pool goes only
            // when empty): their number, size, room free, largest free range,
            // ranges let go but not yet reusable; and the merged runs' copies.
            uint32_t pools = 0;
            uint64_t poolFree = 0, poolLargest = 0, poolPending = 0;
            for (const Renderer::Pool &pool : r->pools)
                if (pool.buffer)
                {
                    ++pools;
                    for (const auto &f : pool.free)
                    {
                        poolFree += f.second;
                        poolLargest = std::max<uint64_t>(poolLargest, f.second);
                    }
                }
            for (const Renderer::PoolRange &pr : r->poolPending)
                poolPending += pr.count;
            const double vb = static_cast<double>(sizeof(native_renderer::Vertex));
            std::fprintf(stderr,
                         "[gl:mem] t=%.0f heap %.1f MB | textures %u %.1f MB (in heap %.1f) | meshes %u %.1f MB (skinned %.1f) | drawn in 120 frames %.1f MB, budget %.0f MB | "
                         "uploads peak %.1f MB a frame, flushes %u | arenas %.1f MB, draw lists %.1f MB | hooks' skinned packets %.1f MB | "
                         "evicted aged %u, budget %u (%.1f MB), pressure %u (%.1f MB), kept in pools %u | skipped %u | "
                         "mesh pools %u (%.1f MB, free %.1f MB, largest free %.0f KB, let go %.1f MB; emptied %llu, %.1f MB moved), run copies %.1f MB | "
                         "vgl pools free %.1f MB, low-room passes %llu\n",
                         (native_renderer::nowUs() - m_startUs) / 1e6, m_heap / MB, static_cast<unsigned>(r->textures.size()), r->textureBytes / MB,
                         r->textureHeapBytes / MB, kept, r->meshBytes / MB, r->skinnedBytes / MB, recentBytes / MB, m_cacheBudget == ~0ull ? 0.0 : m_cacheBudget / MB,
                         m_uploadPeak / MB, m_flushes, arenas / MB, lists / MB, skinnedPackets / MB, m_evictedAged, m_evictedBudget,
                         m_evictedBudgetBytes / MB, m_evictedPressure, m_evictedPressureBytes / MB, m_pressureSpared, m_skipped, pools,
                         pools * static_cast<double>(Renderer::kPoolVertices) * vb / MB, poolFree * vb / MB, poolLargest * vb / 1024.0, poolPending * vb / MB,
                         static_cast<unsigned long long>(r->poolsEmptied), r->poolMoved * vb / MB, r->runVertices * vb / MB, m_poolFree / MB,
                         static_cast<unsigned long long>(m_poolLowPasses));
#if defined(PS2X_NATIVE_VITA)
            {
                // Round 65c: vitaGL's uniform circular pool, a frame's share
                // of it since the last line (it wraps without waiting for the
                // GPU: more than the pool in flight and draws get later
                // draws' uniforms).
                uint32_t bytes = 0, wraps = 0;
                vglCeloUniformStats(&bytes, &wraps);
                const uint64_t frames = r->arenaFrame > m_unifFrame ? r->arenaFrame - m_unifFrame : 1u;
                std::fprintf(stderr, "[gl:unif] %.0f KB of uniforms a frame over %llu frames, pool wraps %u\n",
                             static_cast<double>(bytes - m_unifBytes) / 1024.0 / static_cast<double>(frames), static_cast<unsigned long long>(frames), wraps - m_unifWraps);
                m_unifBytes = bytes, m_unifWraps = wraps, m_unifFrame = r->arenaFrame;
            }
#endif
            m_pressureSpared = 0;
            m_uploadPeak = 0;
            m_flushes = m_evictedAged = m_evictedBudget = m_evictedPressure = m_skipped = 0;
            m_evictedBudgetBytes = m_evictedPressureBytes = 0;
        }

        // Ids hash a model's or a texture's content: when the game loads another
        // area into the same memory, the old ones are never asked for again (the
        // title's demo filled the Vita's 160 MB in ~12 min). What no frame drew
        // for 1800 frames goes; drawn again, the frontend uploads it anew
        // (hasMesh false). Sooner when the textures and kept meshes pass
        // m_cacheBudget: the least recently drawn first, unused for
        // kBudgetAge frames at least, down to 7/8 of the budget. Sooner still
        // when the host heap passes m_heapHigh: all unused for kPressureAge
        // frames. On the Vita the budget keeps vitaGL inside its GPU pools: a
        // scene change kept the last area's textures for ~1800 frames next
        // to the new one's (~100 MB), vitaGL put the rest in the newlib heap
        // (heap used 75 -> 145 MB) and the next allocation failed (72 min run).
        // Done at a hand-over, with the render thread waiting: the frame handed
        // over (its draws and uploads) keeps what it uses, and the game
        // thread's list of what is here loses the rest before it records the
        // next frame. The GL objects go on the render thread.
        // Round 27: the budget's and the pressure's ages in time too. In
        // frames alone, at a fight's 8-20 fps, 30 frames were 1.5-4 s: the
        // terrain behind the camera went and came back as holes (held builds)
        // when the camera turned. Now a candidate must also be unused for
        // kPressureMs / kBudgetMs; near the heap's limit (half way from
        // m_heapHigh to m_heapLimit) the frame ages alone, as before.
        static constexpr uint32_t kFrameTimes = 4096;
        uint64_t m_frameTimes[kFrameTimes] = {};
        // The newest frame handed over at least `ms` ago (0: none that old).
        uint64_t frameBefore(uint64_t frame, uint64_t now, uint64_t ms) const
        {
            const uint64_t limit = std::min<uint64_t>(frame, kFrameTimes - 1u);
            for (uint64_t back = 1; back <= limit; ++back)
            {
                const uint64_t f = frame - back, t = m_frameTimes[f % kFrameTimes];
                if (t && now - t >= ms * 1000u)
                    return f;
            }
            return frame > kFrameTimes ? frame - kFrameTimes : 0u; // older than the times kept
        }
        static uint64_t mb(const char *name, double fallback)
        {
            const char *v = std::getenv(name);
            return static_cast<uint64_t>((v ? std::atof(v) : fallback) * 1048576.0);
        }
        uint64_t m_poolFree = 0, m_poolLowPasses = 0; // round 34: vitaGL's pools' room, the passes it started ([gl:mem])
        void evictKept()
        {
            constexpr uint64_t kEvery = 256, kUnused = 1800, kBudgetAge = 120, kPressureAge = 30, kHeapEvery = 16;
            static const uint64_t kPressureMs = std::getenv("SOTC_GL_PRESSURE_MS") ? static_cast<uint64_t>(std::max(0, std::atoi(std::getenv("SOTC_GL_PRESSURE_MS")))) : 8000u;
            static const uint64_t kBudgetMs = std::getenv("SOTC_GL_BUDGET_MS") ? static_cast<uint64_t>(std::max(0, std::atoi(std::getenv("SOTC_GL_BUDGET_MS")))) : 20000u;
            const uint64_t frame = r->arenaFrame + 1u; // the frame handed over
            const uint64_t nowTime = native_renderer::nowUs();
            m_frameTimes[frame % kFrameTimes] = nowTime;
            if (m_heapHigh && frame % kHeapEvery == 0)
                m_heap = heapUsed();
            const bool aging = frame % kEvery == 0 && frame >= kUnused;
            const bool pressure = m_heapHigh && m_heap > m_heapHigh && frame >= m_nextPressure;
            // Round 34 (the Vita, SOTC_GL_POOL_LOW_MB, default 12; 0: off):
            // vitaGL's pools nearly full (CDRAM, its main-RAM and phycont pools,
            // by celosia.patch's own count) count as over budget: what no frame
            // drew for the budget's ages (120 frames and 20 s) goes, the least
            // recently drawn first, until the pools have twice that free. Past
            // them vitaGL puts GPU objects in the newlib heap (the console's
            // round 32 session: CDRAM 0.0 MB free at t=143-163 s, the heap
            // climbing in 2-3 MB steps after it).
            uint64_t poolShort = 0;
#if defined(PS2X_NATIVE_VITA)
            static const uint64_t kPoolLow = mb("SOTC_GL_POOL_LOW_MB", 12.0);
            if (kPoolLow && frame % kHeapEvery == 0)
            {
                size_t used[VGL_MEM_ALL] = {}, spill = 0, spills = 0, total = 0, peak = 0;
                vglCeloMemStats(used, &spill, &spills, &total, &peak);
                uint64_t free = 0;
                for (int t : {VGL_MEM_VRAM, VGL_MEM_RAM, VGL_MEM_PHYCONT})
                {
                    const uint64_t all = vglMemTotal(static_cast<vglMemType>(t));
                    free += all > used[t] ? all - used[t] : 0u;
                }
                m_poolFree = free;
                if (free < kPoolLow)
                    poolShort = 2u * kPoolLow - free;
            }
#endif
            const bool overBudget = (r->textureBytes + r->meshBytes > m_cacheBudget || poolShort) && frame >= m_nextBudget;
            if (!aging && !pressure && !overBudget)
                return;
            m_keepMeshes = native_renderer::IdSet();
            m_keepTextures = native_renderer::IdSet();
            for (const Draw &d : r->draws)
            {
                m_keepMeshes.insert(d.mesh);
                if (d.state.texture)
                    m_keepTextures.insert(d.state.texture);
            }
            for (const uint64_t mesh : r->extraMeshes) // drawMulti's, not expanded yet
                m_keepMeshes.insert(mesh);
            for (const Renderer::Upload &u : r->uploads)
                (u.kind == Renderer::Upload::TextureKind ? m_keepTextures : m_keepMeshes).insert(u.id);
            for (const Renderer::Upload &u : m_side) // flushed, not applied yet
                (u.kind == Renderer::Upload::TextureKind ? m_keepTextures : m_keepMeshes).insert(u.id);
            const uint64_t agedBefore = aging ? frame - kUnused : 0u;
            const bool critical = pressure && m_heapLimit > m_heapHigh && m_heap > m_heapHigh + (m_heapLimit - m_heapHigh) / 2u;
            // Round 33: under pressure (not critical) only what lives in the
            // newlib heap goes while vitaGL's pools have room: the meshes and
            // textures in the pools free no heap when evicted, and the
            // console's round 32 session (heap 105-109 MB from the game's own
            // use, pools a quarter free) threw away everything unused for 8 s
            // every 16 frames, then built and uploaded it again (hitches of
            // 150-200 ms with 60-700 mesh uploads). SOTC_GL_PRESSURE_POOLS=0:
            // as before.
            static const bool spareInPools = !std::getenv("SOTC_GL_PRESSURE_POOLS") || std::atoi(std::getenv("SOTC_GL_PRESSURE_POOLS")) != 0;
            bool heapOnly = false;
#if defined(PS2X_NATIVE_VITA)
            // Round 36: whatever room the pools have. Round 33 also asked for
            // 8 MB of CDRAM and 2 MB of vitaGL's main-RAM pool free, but the
            // console's RAM pool (~10 MB) sits at 8.2 used for good: the
            // spare never applied there (`kept in pools 0` in nearly every
            // [gl:mem] line of round 35's session, heap 108-110 MB), so every
            // mesh and texture not drawn for 8 s went every 16 frames, the
            // pools' as well, which give the heap nothing back. Riding through
            // a canyon, the walls seen a few seconds before came back as
            // holes (held terrain, built again at the terrain budget a frame:
            // the console's "walls you can see through", 2026-10-30). A short
            // pool is the budget's job (poolShort above).
            // SOTC_GL_PRESSURE_POOLFREE=1: round 33's condition.
            static const bool poolFreeRule = std::getenv("SOTC_GL_PRESSURE_POOLFREE") && std::atoi(std::getenv("SOTC_GL_PRESSURE_POOLFREE")) != 0;
            heapOnly = pressure && !critical && spareInPools &&
                       (!poolFreeRule || (vglMemFree(VGL_MEM_VRAM) >= (8u << 20) && vglMemFree(VGL_MEM_RAM) >= (2u << 20)));
#else
            (void)spareInPools;
#endif
            uint64_t pressureBefore = pressure && frame > kPressureAge ? frame - kPressureAge : 0u;
            uint64_t budgetBefore = overBudget && frame > kBudgetAge ? frame - kBudgetAge : 0u;
            if (pressureBefore && !critical)
                pressureBefore = std::min(pressureBefore, frameBefore(frame, nowTime, kPressureMs));
            if (budgetBefore)
                budgetBefore = std::min(budgetBefore, frameBefore(frame, nowTime, kBudgetMs));
            const uint64_t oldest = std::max({agedBefore, pressureBefore, budgetBefore});
            m_candidates.clear();
            r->meshes.forEach([&](uint64_t id, const Mesh &m)
            {
                if (!m.dynamic && m.used < oldest && !m_keepMeshes.count(id))
                    m_candidates.push_back({m.used, id, m.bytes, false});
            });
            r->textures.forEach([&](uint64_t id, const Texture &t)
            {
                if (t.used < oldest && !m_keepTextures.count(id))
                    m_candidates.push_back({t.used, id, t.bytes, true});
            });
            std::sort(m_candidates.begin(), m_candidates.end(), [](const Candidate &a, const Candidate &b) { return a.used < b.used; });
            uint64_t cached = r->textureBytes + r->meshBytes;
            uint64_t target = m_cacheBudget - m_cacheBudget / 8u;
            if (poolShort)
            {
                target = std::min(target, cached > poolShort ? cached - poolShort : 0u);
                ++m_poolLowPasses;
            }
            for (const Candidate &c : m_candidates)
            {
                const bool aged = c.used < agedBefore, pressed = c.used < pressureBefore, budget = c.used < budgetBefore && cached > target;
                if (!aged && !pressed && !budget)
                    continue;
                if (heapOnly && !aged && !budget && !(c.texture && r->textures.find(c.id)->inHeap))
                {
                    ++m_pressureSpared;
                    continue;
                }
                if (aged)
                    ++m_evictedAged;
                else if (pressed)
                {
                    ++m_evictedPressure;
                    m_evictedPressureBytes += c.bytes;
                }
                else
                {
                    ++m_evictedBudget;
                    m_evictedBudgetBytes += c.bytes;
                }
                cached -= c.bytes;
                if (c.texture)
                {
                    Texture *t = r->textures.find(c.id);
                    if (t->name)
                        r->deadTextures.push_back(t->name);
                    if (t->swz)
                        r->deadTextures.push_back(t->swz);
                    r->forget(*t);
                    r->textures.erase(c.id);
                    m_knownTextures.erase(c.id);
                }
                else
                {
                    Mesh *m = r->meshes.find(c.id);
                    if (m->buffer)
                        r->deadBuffers.push_back(m->buffer);
                    if (m->indexBuffer)
                        r->deadBuffers.push_back(m->indexBuffer);
                    if (m->pool >= 0)
                        r->deadRanges.push_back({m->pool, m->first, m->count, 0});
                    r->forget(*m);
                    r->meshes.erase(c.id);
                    m_knownMeshes.erase(c.id);
                    ++m_meshEvictions;
                }
            }
            // A scene that needs more than the budget: looked at again later
            // (not every frame); under pressure, after the next heap reading.
            if (overBudget && (cached > m_cacheBudget || poolShort))
                m_nextBudget = frame + 64u;
            if (pressure)
            {
                m_nextPressure = frame + kHeapEvery;
                m_rec.arena.trim(0);
                m_rec.indexArena.trim(0);
            }
        }

        void loop(const std::function<void()> &makeCurrent, bool initThere)
        {
            ThreadNaming::SetCurrentThreadName("RenderThread");
            // Core 2 on the Vita (native_renderer::threads).
            native_renderer::threads::place(native_renderer::threads::kRender);
            if (makeCurrent)
                makeCurrent();
            const bool ok = !initThere || r->init();
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_started = true;
                m_initOk = ok;
            }
            m_idle.notify_all();
            if (!ok)
                return;
            for (;;)
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                while (!m_queued && m_side.empty())
                    m_work.wait_for(lock, kRecheck);
                // Flushed uploads first (they were asked for before this frame's).
                if (!m_side.empty())
                {
                    m_sideApplying.swap(m_side);
                    const uint64_t bytes = m_sideBytes;
                    m_sideBytes = 0;
                    m_busy = true;
                    lock.unlock();
                    {
                        Lock gl;
                        r->applyUploadList(m_sideApplying);
                    }
                    lock.lock();
                    (void)bytes;
                    if (!m_queued)
                    {
                        m_busy = false;
                        m_idle.notify_all();
                        continue;
                    }
                }
                m_queued = false;
                m_busy = true;
                const uint32_t width = m_width, height = m_height;
                const bool uploadsOnly = m_uploadsOnly; // flushUploads (SOTC_GL_FLUSH_WAIT)
                m_uploadsOnly = false;
                lock.unlock();
                if (uploadsOnly)
                {
                    Lock gl;
                    r->applyUploads();
                }
                else
                {
                    const uint64_t lockStart = native_renderer::nowUs();
                    Lock gl;
                    const uint64_t start = native_renderer::nowUs(), present = r->stats.presentUs;
                    r->stats.lockUs += start - lockStart;
                    ++r->arenaFrame;
                    r->applyUploads();
                    const uint64_t uploaded = native_renderer::nowUs();
                    r->stats.uploadUs += uploaded - start;
                    r->expandDraws();
                    // Round 86: the present after the data is given back (below).
                    r->deferPresent = native_renderer::tuning().glPresent86 && native_renderer::tuning().renderThread;
                    if (!r->hold) // held: the last picture stays on screen
                    {
                        // SOTC_GL_SELFCHECK=N (debugging, with bloom and the
                        // motion trail off: they carry the last frame over):
                        // every N frames the frame is drawn first with the
                        // draw merges and lean programs off, then as set, and
                        // the two pictures compared.
                        static const int check = std::getenv("SOTC_GL_SELFCHECK") ? std::atoi(std::getenv("SOTC_GL_SELFCHECK")) : 0;
                        if (check > 0 && r->arenaFrame % static_cast<uint64_t>(check) == 0u)
                        {
                            r->deferPresent = false;
                            r->selfCheck(width, height);
                        }
                        else
                            r->render(width, height);
                    }
                    const uint64_t drawn = native_renderer::nowUs();
                    r->stats.drawUs += (drawn - uploaded) - (r->stats.presentUs - present);
                    if (r->spike.on)
                    {
                        static const uint64_t limitUs = std::getenv("SOTC_GL_SPIKE_MS") ? std::strtoull(std::getenv("SOTC_GL_SPIKE_MS"), nullptr, 10) * 1000u : 100000u;
                        const uint64_t drawPart = (drawn - uploaded) - (r->stats.presentUs - present);
                        if (drawPart > limitUs)
                            std::fprintf(stderr,
                                         "[gl:spike] arena %llu: draw %.1f ms (upload %.1f, present %.1f), %u draws; longest gap %.1f ms ending at draw %u (fbo %u order %u, %u vertices%s%s%s%s, test %04x); "
                                         "in the gap: event %d (-100 none; -1 bloom, -2 glow, -3 shadows, -4 refraction copy, -8 Vita bloom, 0+ screen blur), %u scenes; "
                                         "slowest draw call %.1f ms (draw %u); targets made %s\n",
                                         (unsigned long long)r->arenaFrame, drawPart / 1000.0, (uploaded - start) / 1000.0, (r->stats.presentUs - present) / 1000.0,
                                         r->spike.drawn, r->spike.gap / 1000.0, r->spike.index, r->spike.target, r->spike.order, r->spike.count,
                                         r->spike.volume ? ", shadow volume" : "", r->spike.overlay ? ", overlay" : "", r->spike.skinned ? ", skinned" : "",
                                         r->spike.dynamic ? ", dynamic" : "", r->spike.alphaTest, r->spike.gapEvent, r->spike.gapScenes, r->spike.callMax / 1000.0,
                                         r->spike.callIndex, std::getenv("SOTC_GL_WARM_TARGETS") && std::atoi(std::getenv("SOTC_GL_WARM_TARGETS")) == 0 ? "lazily" : "at start");
                        r->spike.callMax = 0;
                        r->spike.event = r->spike.gapEvent = -100;
                        r->spike.last = r->spike.gap = 0;
                        r->spike.drawn = r->spike.index = 0;
                    }
                    r->evictUnused();
                    r->stats.sweepUs += native_renderer::nowUs() - drawn;
                    r->stats.renderUs += (native_renderer::nowUs() - start) - (r->stats.presentUs - present);
                    // Round 86 (glPresent86): the frame's data back to the game
                    // thread first (it may hand the next frame over now), then
                    // the present, still under the GL lock; the next frame
                    // starts when it returns. Its time goes into the counters
                    // under the hand-over's mutex (the game thread reads them there).
                    if (r->presentPending)
                    {
                        lock.lock();
                        m_busy = false;
                        m_presenting = true;
                        m_idle.notify_all();
                        lock.unlock();
                        const uint64_t took = r->presentNow();
                        lock.lock();
                        r->stats.presentUs += took;
                        m_presenting = false;
                        m_idle.notify_all();
                        lock.unlock();
                    }
                    r->deferPresent = false;
                }
                lock.lock();
                m_busy = false;
                m_idle.notify_all();
            }
        }

        Renderer *r;
        // The game thread's.
        Recording m_rec;
        native_renderer::IdSet m_knownMeshes, m_knownTextures; // what hasMesh / hasTexture answer
        uint64_t m_meshEvictions = 0;                          // meshEvictions
        size_t m_pendingFirst = 0, m_pendingIndexFirst = 0;
        // Memory (see evictKept, flushUploads, trimRecording, logMemory).
        struct Candidate
        {
            uint64_t used, id, bytes;
            bool texture;
        };
        std::vector<Candidate> m_candidates;
        native_renderer::IdSet m_keepMeshes, m_keepTextures;
        uint64_t m_cacheBudget = ~0ull, m_heapHigh = 0, m_heapLimit = 0, m_flushBytes = 0, m_heap = 0;
        uint64_t m_nextBudget = 0, m_nextPressure = 0;
        uint64_t m_recUploadBytes = 0, m_frameUploadBytes = 0, m_uploadPeak = 0;
        size_t m_vertexPeak = 0, m_indexPeak = 0, m_drawPeak = 0, m_vertexPeakPrev = 0, m_indexPeakPrev = 0, m_drawPeakPrev = 0;
        uint32_t m_trimFrames = 0;
        bool m_memLog = false;
        uint64_t m_startUs = 0, m_nextMemLog = 0;
        uint32_t m_flushes = 0, m_evictedAged = 0, m_evictedBudget = 0, m_evictedPressure = 0, m_skipped = 0;
        uint32_t m_pressureSpared = 0; // round 33: kept under pressure (in vitaGL's pools, not the heap)
        uint32_t m_unifBytes = 0, m_unifWraps = 0; // round 65c: vglCeloUniformStats at the last [gl:unif] line
        uint64_t m_unifFrame = 0;
        uint64_t m_evictedBudgetBytes = 0, m_evictedPressureBytes = 0;
        // The hand-over.
        // One condition each way. The waits also look again every few ms: in
        // Vita3K a wake-up of the newlib pthread condition got lost now and
        // then (the game thread waited on an idle render thread for good).
        std::mutex m_mutex;
        std::condition_variable m_work, m_idle; // a frame handed over; the render thread idle
        static constexpr std::chrono::milliseconds kRecheck{2};
        void waitIdle(std::unique_lock<std::mutex> &lock)
        {
            while (m_queued || m_busy || m_presenting)
                m_idle.wait_for(lock, kRecheck);
        }
        // Round 86: the hand-over's own wait: the frame drawn done with (its
        // present may still be running, glPresent86).
        void waitData(std::unique_lock<std::mutex> &lock)
        {
            while (m_queued || m_busy)
                m_idle.wait_for(lock, kRecheck);
        }
        bool m_presenting = false; // round 86: the render thread in a deferred present
        bool m_started = false, m_initOk = false;
        bool m_queued = false; // a frame handed over, not started yet
        bool m_busy = false;   // the render thread drawing
        uint64_t m_handOvers = 0; // round 73 (SOTC_GL_FREEZE)
        bool m_frozen = false;
        bool m_uploadsOnly = false; // what is handed over is uploads alone (flushUploads, SOTC_GL_FLUSH_WAIT)
        std::vector<Renderer::Upload> m_side, m_sideApplying; // flushed uploads (flushUploads); the render thread's copy while it applies them
        uint64_t m_sideBytes = 0;
        uint32_t m_width = 0, m_height = 0;
    };
}

namespace native_renderer
{
    bool latestBloomStages(std::vector<BloomStage> &out)
    {
        std::lock_guard<std::mutex> lock(g_bloomStageMutex);
        out = g_bloomStagesOut;
        return !out.empty();
    }
}

namespace native_renderer::detail
{
    Backend *createGlBackend(std::function<void()> makeCurrent)
    {
        auto *renderer = new Renderer();
#if defined(PS2X_NATIVE_VITA)
        // Built here, on the main thread (the CG compiler needs its stack).
        if (!renderer->init())
        {
            delete renderer;
            return nullptr;
        }
        const bool initThere = false;
#else
        const bool initThere = true;
#endif
        auto *backend = new GlBackend(renderer);
        if (!backend->start(std::move(makeCurrent), initThere))
            return nullptr; // its thread has ended; the renderer is left as it is
        return backend;
    }
}

#endif
