#include "runtime/gs/gs_hw_backend.h"
#include "runtime/ps2_diag.h"
#include "runtime/native_renderer.h"
#include "runtime/gs/gs_cpu_backend.h"
#include "runtime/gs/gs_direct_frame.h"
#include "runtime/host_menu.h"
#include "runtime/ps2_stats.h"
#include "runtime/ps2_thread_load.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#define PS2X_GS_HW_GL 1
#elif defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM)
// vitaGL (GXM): one GL state for every thread, so the host and the GS thread
// take turns under the raylib shim's lock. CG shaders, no depth textures, no
// fences, no constant blend color: see the PS2X_GS_HW_VITA paths.
#include <vitaGL.h>
#include <mutex>
#define PS2X_GS_HW_GL 1
#define PS2X_GS_HW_VITA 1
std::recursive_mutex &ps2xVitaGlMutex();
uint32_t ps2xVitaHostFrames();
#endif

#if !defined(GL_CG_FRAGMENT_SHADER_EXT)
#define GL_CG_FRAGMENT_SHADER_EXT_OR_ZERO 0
#else
#define GL_CG_FRAGMENT_SHADER_EXT_OR_ZERO GL_CG_FRAGMENT_SHADER_EXT
#endif

#if defined(PS2X_GS_HW_VITA)
// Waiting for the host's turn counts as the GS thread waiting (and on its own).
#define PS2X_GS_GL_LOCK()                                                                                                   \
    std::unique_lock<std::recursive_mutex> glLock(ps2xVitaGlMutex(), std::try_to_lock);                                     \
    if (!glLock.owns_lock())                                                                                                \
    {                                                                                                                       \
        const auto glWaitStart = std::chrono::steady_clock::now();                                                          \
        {                                                                                                                   \
            ps2_load::Wait glWait;                                                                                          \
            glLock.lock();                                                                                                  \
        }                                                                                                                   \
        ps2_load::gsGlLockWaitUs.fetch_add(static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(     \
                                               std::chrono::steady_clock::now() - glWaitStart)                              \
                                               .count()),                                                                   \
                                           std::memory_order_relaxed);                                                      \
    }
#else
#define PS2X_GS_GL_LOCK() ((void)0)
#endif

namespace
{
    std::function<void()> g_makeCurrent;
    std::function<void(uint32_t, uint32_t, uint32_t)> g_presentHook;
}

void GSHwBackend::setPresentHook(std::function<void(uint32_t texture, uint32_t width, uint32_t height)> hook)
{
#if defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM)
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex()); // not while the GS thread runs it
#endif
    g_presentHook = std::move(hook);
}

#if !(defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM))
void GSHwBackend::setContextProvider(std::function<void()> makeCurrent) { g_makeCurrent = std::move(makeCurrent); }
#endif
bool GSHwBackend::available()
{
#if defined(PS2X_GS_HW_GL)
    return static_cast<bool>(g_makeCurrent);
#else
    return false;
#endif
}

#if defined(PS2X_GS_HW_GL)

namespace
{
    // Alpha inside the GPU is stored in PS2 units: 0x80 = 1.0.
    constexpr float kAlphaOne = 128.0f;

    bool isColorPsm(uint32_t psm) { return psm == GS_PSM_CT32 || psm == GS_PSM_CT24 || psm == GS_PSM_CT16 || psm == GS_PSM_CT16S; }
    uint32_t bytesPerPixel(uint32_t psm) { return (psm == GS_PSM_CT16 || psm == GS_PSM_CT16S || psm == GS_PSM_Z16 || psm == GS_PSM_Z16S) ? 2u : 4u; }

    const char *kVertexShader = R"(#version 330 core
layout(location = 0) in vec3 aPos;     // render-target pixels, depth 0..1
layout(location = 1) in vec3 aTex;     // s, t, q (or u, v, 1 in texels)
layout(location = 2) in vec4 aColor;   // 0..255
layout(location = 3) in float aFog;    // 0..255
uniform vec2 uTarget;
out vec3 vTex;
out vec4 vColor;
out float vFog;
out float vDepth;
void main()
{
    // Depth goes to gl_FragDepth as is: GS Z values are often tiny, and the
    // NDC mapping (z * 2 - 1) would round them together near -1.
    gl_Position = vec4(aPos.xy / uTarget * 2.0 - 1.0, 0.0, 1.0);
    vDepth = aPos.z;
    vTex = aTex;
    vColor = aColor;
    vFog = aFog;
}
)";

    const char *kFragmentShader = R"(#version 330 core
in vec3 vTex;
in vec4 vColor;
in float vFog;
in float vDepth;
uniform sampler2D uTexture;
uniform int uTme, uTfx, uTcc, uFst, uFge, uAtest, uAref, uAtestFailed;
uniform vec2 uTexSize;       // texels of the bound texture
uniform vec2 uGsSize;        // GS texture size (2^TW, 2^TH)
uniform ivec2 uWrap;         // CLAMP.WMS/WMT: repeat, clamp, region clamp, region repeat
uniform ivec4 uRegion;       // MINU, MINV, MAXU, MAXV
uniform int uLinear;
uniform float uAlphaFix;     // >= 0: texture alpha (TA0; + 256: AEM)
uniform float uTexAlphaScale; // texture alpha read / scale -> PS2 alpha units of 128
uniform vec3 uFogColor;
uniform int uExactBlend;     // 1: the GS blend here, in integers, on a copy of the target (uDst);
                             // 2: GL blends Cs * As (+ Cd ...), the source biased to truncate like the GS
uniform sampler2D uDst;
uniform ivec4 uBlendSel;     // ALPHA: A, B, D in {Cs, Cd, 0}; C in {As, Ad, FIX}
uniform float uBlendFixC;    // ALPHA.FIX
out vec4 oColor;

// GS texel addressing (CLAMP register) in GS texture space, then a fetch from
// the bound texture: render targets are larger than the GS texture they hold.
int wrapCoord(int v, int size, int mode, int lo, int hi)
{
    if (mode == 0) return v & (size - 1);
    if (mode == 1) return clamp(v, 0, size - 1);
    if (mode == 2) return clamp(v, lo, hi);
    return (v & lo) | hi;
}
vec4 fetchTexel(ivec2 p)
{
    ivec2 size = ivec2(uGsSize);
    p = ivec2(wrapCoord(p.x, size.x, uWrap.x, uRegion.x, uRegion.z), wrapCoord(p.y, size.y, uWrap.y, uRegion.y, uRegion.w));
    return texelFetch(uTexture, clamp(p, ivec2(0), ivec2(uTexSize) - 1), 0);
}
vec4 sampleGs(vec2 tc)
{
    if (uLinear == 0)
        return fetchTexel(ivec2(floor(tc)));
    vec2 b = tc - 0.5;
    ivec2 i = ivec2(floor(b));
    vec2 f = b - floor(b);
    vec4 t00 = fetchTexel(i), t10 = fetchTexel(i + ivec2(1, 0)), t01 = fetchTexel(i + ivec2(0, 1)), t11 = fetchTexel(i + ivec2(1, 1));
    return mix(mix(t00, t10, f.x), mix(t01, t11, f.x), f.y);
}

void main()
{
    vec4 c = vColor;              // 0..255, 0x80 = 1.0 for modulation
    if (uTme != 0)
    {
        vec2 tc = (uFst != 0) ? vTex.xy : vTex.xy / vTex.z * uGsSize;
        vec4 t = sampleGs(tc);
        t.rgb *= 255.0;
        t.a *= 255.0 * uTexAlphaScale;
        if (uAlphaFix >= 256.0)
            t.a = all(lessThan(t.rgb, vec3(0.5))) ? 0.0 : uAlphaFix - 256.0;
        else if (uAlphaFix >= 0.0)
            t.a = uAlphaFix;
        if (uExactBlend != 0)
            t = floor(t + 0.5); // the GS's texel values (filtering rounds)
        vec3 rgb;
        float a;
        if (uTfx == 0)      { rgb = t.rgb * c.rgb / 128.0; a = (uTcc != 0) ? t.a * c.a / 128.0 : c.a; }   // MODULATE
        else if (uTfx == 1) { rgb = t.rgb;                  a = (uTcc != 0) ? t.a : c.a; }               // DECAL
        else if (uTfx == 2) { rgb = t.rgb * c.rgb / 128.0 + c.a; a = (uTcc != 0) ? t.a + c.a : c.a; }   // HIGHLIGHT
        else                { rgb = t.rgb * c.rgb / 128.0 + c.a; a = (uTcc != 0) ? t.a : c.a; }       // HIGHLIGHT2
        c = vec4(min(rgb, vec3(255.0)), min(a, 255.0));
        if (uExactBlend != 0)
            c = floor(c + 1.0 / 512.0); // (T * C) >> 7
    }
    if (uFge != 0)
        c.rgb = mix(uFogColor, c.rgb, vFog / 255.0);
    // Alpha test. uAtestFailed: this pass draws the pixels that fail it
    // (AFAIL modes other than KEEP still write part of the pixel).
    if (uAtest != 1)
    {
        float a = floor(c.a + 0.5);
        bool pass = uAtest == 0 ? false :
                    uAtest == 2 ? a < float(uAref) :
                    uAtest == 3 ? a <= float(uAref) :
                    uAtest == 4 ? a == float(uAref) :
                    uAtest == 5 ? a >= float(uAref) :
                    uAtest == 6 ? a > float(uAref) : a != float(uAref);
        if (pass == (uAtestFailed != 0))
            discard;
    }
    oColor = vec4(c.rgb / 255.0, c.a / 128.0);
    if (uExactBlend == 1)
    {
        // ((A - B) * C >> 7) + D, clamped: GL's blender rounds where the GS
        // truncates, which feedback passes (SotC's bloom) add up frame after frame.
        vec4 dst = texelFetch(uDst, ivec2(gl_FragCoord.xy), 0);
        vec3 cs = floor(c.rgb + 1.0 / 512.0), cd = floor(dst.rgb * 255.0 + 0.5);
        float ad = floor(dst.a * 128.0 + 0.5);
        vec3 A = uBlendSel.x == 0 ? cs : uBlendSel.x == 1 ? cd : vec3(0.0);
        vec3 B = uBlendSel.y == 0 ? cs : uBlendSel.y == 1 ? cd : vec3(0.0);
        float C = uBlendSel.z == 0 ? floor(c.a + 1.0 / 512.0) : uBlendSel.z == 1 ? ad : uBlendFixC;
        vec3 D = uBlendSel.w == 0 ? cs : uBlendSel.w == 1 ? cd : vec3(0.0);
        oColor.rgb = clamp(floor((A - B) * C / 128.0) + D, 0.0, 255.0) / 255.0;
    }
    else if (uExactBlend == 2)
    {
        // The blender adds Cs * As to the rest and rounds to nearest; the
        // GS truncates ((Cs - Cd) * As >> 7). Products are in 1/128 steps,
        // so taking 0.498 off the Cs * As term rounds to the truncated value.
        float a = floor(c.a + 1.0 / 512.0);
        vec3 cs = floor(c.rgb + 1.0 / 512.0);
        oColor.rgb = a > 0.0 ? max(cs - 0.498 * 128.0 / a, 0.0) / 255.0 : cs / 255.0;
        oColor.a = a / 128.0;
    }
    gl_FragDepth = vDepth;
}
)";

    // VU1 batches (GSVuBatch): the world microcode's transform on the GPU.
    // Clip position = M p; screen = clip.xy / w in GS pixels; Z = clip.z / w;
    // S, T, Q = uv q, q -- the same as a perspective-correct uv, which the
    // rasterizer does. Colour, fog and Z are interpolated in screen space,
    // as the GS does (noperspective). Clipping, including behind the eye, is
    // the GPU's (the microcode's guard-band clipper is not needed).
    const char *kVuVertexShader = R"(#version 330 core
layout(location = 0) in vec3 aPos;     // object space
layout(location = 1) in vec2 aTex;     // texture coordinate (normalised)
layout(location = 2) in vec4 aColor;   // 0..255
layout(location = 3) in float aBatch;  // index into uM / uBatch
uniform mat4 uM[32];
uniform vec4 uBatch[32];    // fog = clamp(x + y q, 0, 255); zw: XYOFFSET in pixels
uniform vec4 uNear[32];     // the microcode clipper's near plane (GSVuBatch)
uniform vec2 uTarget;
uniform float uDepthScale;  // GS Z -> 0..1
out vec3 vTex;
noperspective out vec4 vColor;
noperspective out float vFog;
noperspective out float vDepth;
void main()
{
    int b = int(aBatch);
    vec4 c = uM[b] * vec4(aPos, 1.0);
    vec4 k = uBatch[b];
    float q = 1.0 / c.w;
    vec2 p = c.xy * q - k.zw + 0.5;
    // z: the clipper's planes (z >= -w: near; z <= w: its far plane, C row
    // w being M's); the depth itself goes through vDepth.
    gl_Position = vec4((p / uTarget * 2.0 - 1.0) * c.w, dot(uNear[b], vec4(aPos, 1.0)) - c.w, c.w);
    vDepth = clamp(c.z * q * uDepthScale, 0.0, 1.0);
    vTex = vec3(aTex, 1.0);
    vColor = aColor;
    vFog = clamp(k.x + k.y * q, 0.0, 255.0);
}
)";

    struct VuGpuVertex
    {
        float x, y, z;
        float s, t;
        uint8_t r, g, b, a;
        float batch;
    };
    // Consecutive VU batches with the same draw state share one draw call
    // (as triangle lists), each with its own matrix and constants.
    constexpr size_t kVuBatchesPerDraw = 32;

    // COLCLAMP off: the GS wraps colors modulo 256 (SotC's shadow volumes add
    // +1 and 255 per face). Such draws add into a float target; this writes
    // the sums back as 8-bit values.
    const char *kWrapVertexShader = R"(#version 330 core
void main()
{
    vec2 p = vec2((gl_VertexID & 1) != 0 ? 3.0 : -1.0, (gl_VertexID & 2) != 0 ? 3.0 : -1.0);
    gl_Position = vec4(p, 0.0, 1.0);
}
)";
    const char *kWrapFragmentShader = R"(#version 330 core
uniform sampler2D uSums;
out vec4 oColor;
void main()
{
    vec4 v = texelFetch(uSums, ivec2(gl_FragCoord.xy), 0);
    vec3 c = mod(floor(v.rgb * 255.0 + 0.5), 256.0) / 255.0;
    oColor = vec4(c, v.a);
}
)";

#if defined(PS2X_GS_HW_VITA)
    // The same renderer in CG (vitaGL compiles it with libshacccg). Depth is
    // the position's z (glDepthRange(-1, 1) keeps it as is): no gl_FragDepth.
    // Integer state comes as floats; texelFetch is a point-sampled tex2D.
    const char *kVertexShaderCg = R"(
void main(float3 aPos, float3 aTex, float4 aColor, float aFog,
          uniform float2 uTarget,
          uniform float uFlipY,
          float4 out oPosition : POSITION,
          float4 out vTex : TEXCOORD0,
          float4 out vColor : TEXCOORD1)
{
    // Varyings are all float4 (Vita3K pairs them by size). vTex.w: fog.
    float2 p = aPos.xy / uTarget * 2.0 - 1.0;
    oPosition = float4(p.x, p.y * uFlipY, aPos.z, 1.0);
    vTex = float4(aTex, aFog);
    vColor = aColor;
}
)";

    const char *kFragmentShaderCg = R"(
uniform sampler2D uTexture;
uniform float uTme, uTfx, uTcc, uFst, uFge, uAtest, uAref, uAtestFailed;
uniform float2 uTexSize;
uniform float2 uGsSize;
uniform float2 uWrap;
uniform float4 uRegion;
uniform float uLinear;
uniform float uHwSample;     // 1: the bound texture is the GS texture, GL filtering and wrapping do
uniform float uAlphaFix;
uniform float uTexAlphaScale;
uniform float3 uFogColor;
uniform float uBlendFix;     // >= 0: output alpha is FIX (blend factor C = FIX)

float wrapCoord(float v, float size, float mode, float lo, float hi)
{
    if (mode < 0.5) return v - size * floor(v / size);
    if (mode < 1.5) return clamp(v, 0.0, size - 1.0);
    if (mode < 2.5) return clamp(v, lo, hi);
    float m = lo + 1.0; // region repeat: UMSK is 2^n - 1 in practice
    return v - m * floor(v / m) + hi;
}
float4 fetchTexel(float2 p)
{
    p = float2(wrapCoord(p.x, uGsSize.x, uWrap.x, uRegion.x, uRegion.z), wrapCoord(p.y, uGsSize.y, uWrap.y, uRegion.y, uRegion.w));
    p = clamp(p, float2(0.0, 0.0), uTexSize - 1.0);
    return tex2D(uTexture, (p + 0.5) / uTexSize);
}
float4 sampleGs(float2 tc)
{
    if (uHwSample > 0.5)
        return tex2D(uTexture, tc / uTexSize);
    if (uLinear < 0.5)
        return fetchTexel(floor(tc));
    float2 b = tc - 0.5;
    float2 i = floor(b);
    float2 f = b - i;
    float4 t00 = fetchTexel(i), t10 = fetchTexel(i + float2(1.0, 0.0)), t01 = fetchTexel(i + float2(0.0, 1.0)), t11 = fetchTexel(i + float2(1.0, 1.0));
    return lerp(lerp(t00, t10, f.x), lerp(t01, t11, f.x), f.y);
}

float4 main(float4 vTex : TEXCOORD0, float4 vColor : TEXCOORD1) : COLOR
{
    float vFog = vTex.w;
    float4 c = vColor;
    if (uTme > 0.5)
    {
        float2 tc = (uFst > 0.5) ? vTex.xy : vTex.xy / vTex.z * uGsSize;
        float4 t = sampleGs(tc);
        t.rgb *= 255.0;
        t.a *= 255.0 * uTexAlphaScale;
        if (uAlphaFix >= 256.0)
            t.a = all(t.rgb < float3(0.5, 0.5, 0.5)) ? 0.0 : uAlphaFix - 256.0;
        else if (uAlphaFix >= 0.0)
            t.a = uAlphaFix;
        float3 rgb;
        float a;
        if (uTfx < 0.5)      { rgb = t.rgb * c.rgb / 128.0; a = (uTcc > 0.5) ? t.a * c.a / 128.0 : c.a; }
        else if (uTfx < 1.5) { rgb = t.rgb;                  a = (uTcc > 0.5) ? t.a : c.a; }
        else if (uTfx < 2.5) { rgb = t.rgb * c.rgb / 128.0 + c.a; a = (uTcc > 0.5) ? t.a + c.a : c.a; }
        else                 { rgb = t.rgb * c.rgb / 128.0 + c.a; a = (uTcc > 0.5) ? t.a : c.a; }
        c = float4(min(rgb, float3(255.0, 255.0, 255.0)), min(a, 255.0));
    }
    if (uFge > 0.5)
        c.rgb = lerp(uFogColor, c.rgb, vFog / 255.0);
    if (uAtest < 0.5 || uAtest > 1.5)
    {
        float a = floor(c.a + 0.5);
        bool passes = uAtest < 0.5 ? false :
                    uAtest < 2.5 ? a < uAref :
                    uAtest < 3.5 ? a <= uAref :
                    uAtest < 4.5 ? a == uAref :
                    uAtest < 5.5 ? a >= uAref :
                    uAtest < 6.5 ? a > uAref : a != uAref;
        if (passes == (uAtestFailed > 0.5))
            discard;
    }
    return float4(c.rgb / 255.0, (uBlendFix >= 0.0 ? uBlendFix : c.a) / 128.0);
}
)";

    // VU batches (kVuVertexShader in CG). CG has no noperspective: colour and
    // fog are sent times w with w itself in vLin.x; dividing by the
    // interpolated w gives their screen-space interpolation. Z is the
    // position's z times w (linear in screen space after the divide). ST
    // stays perspective-correct (S, T, 1).
    const char *kVuVertexShaderCg = R"(
void main(float3 aPos, float2 aTex, float4 aColor, float aBatch,
          uniform float4 uM[128],
          uniform float4 uBatch[32],
          uniform float2 uTarget,
          uniform float uFlipY,
          uniform float uDepthScale,
          float4 out oPosition : POSITION,
          float4 out vTex : TEXCOORD0,
          float4 out vColor : TEXCOORD1,
          float4 out vLin : TEXCOORD2)
{
    int b = (int)aBatch;
    float4 c = uM[b * 4] * aPos.x + uM[b * 4 + 1] * aPos.y + uM[b * 4 + 2] * aPos.z + uM[b * 4 + 3];
    float4 k = uBatch[b];
    float q = 1.0 / c.w;
    float2 p = (c.xy * q - k.zw + 0.5) / uTarget * 2.0 - 1.0;
    float depth = clamp(c.z * q * uDepthScale, 0.0, 1.0);
    oPosition = float4(p.x * c.w, p.y * uFlipY * c.w, depth * c.w, c.w);
    vTex = float4(aTex, 1.0, clamp(k.x + k.y * q, 0.0, 255.0) * c.w);
    vColor = aColor * c.w;
    vLin = float4(c.w, 0.0, 0.0, 0.0);
}
)";
    // The GS fragment stage behind the division by the interpolated w.
    const char *kVuFragmentMainCg = R"(
float4 main(float4 vTex : TEXCOORD0, float4 vColor : TEXCOORD1, float4 vLin : TEXCOORD2) : COLOR
{
    float r = 1.0 / vLin.x;
    return gsMain(float4(vTex.xyz, vTex.w * r), vColor * r);
}
)";
#endif

    GLuint compile(GLenum type, const char *source)
    {
#if defined(PS2X_GS_HW_VITA)
        type = type == GL_VERTEX_SHADER ? GL_CG_VERTEX_SHADER_EXT : GL_CG_FRAGMENT_SHADER_EXT;
#endif
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint ok = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            char log[2048] = {};
            GLsizei length = 0;
            glGetShaderInfoLog(shader, sizeof(log), &length, log);
            std::cerr << "[gs:hw] " << (type == GL_FRAGMENT_SHADER || type == GL_CG_FRAGMENT_SHADER_EXT_OR_ZERO ? "fragment" : "vertex")
                      << " shader error: " << std::string(log, std::min<size_t>(length, sizeof(log))) << std::endl;
        }
        return shader;
    }

    GLuint buildProgram(const char *vertexSource, const char *fragmentSource)
    {
        GLuint vs = compile(GL_VERTEX_SHADER, vertexSource);
        GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentSource);
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glBindAttribLocation(program, 0, "aPos");
        glBindAttribLocation(program, 1, "aTex");
        glBindAttribLocation(program, 2, "aColor");
        glBindAttribLocation(program, 3, "aFog");
        glBindAttribLocation(program, 3, "aBatch");
        glLinkProgram(program);
        return program;
    }

#if defined(PS2X_GS_HW_VITA)
    // libshacccg needs more stack than the GS thread has: the program is
    // compiled on the main thread, when the host has vitaGL up.
    GLuint g_vitaProgram = 0;
    GLuint g_vitaVuProgram = 0;
#endif

    struct HwVertex
    {
        float x, y, z;
        float s, t, q;
        uint8_t r, g, b, a;
        float fog;
    };

    // Everything a draw's GPU state depends on; consecutive primitives with an
    // equal key go into one draw call.
    struct DrawKey
    {
        uint32_t fbp, fbw, fpsm, fbmsk;
        uint32_t zbp, zpsm, zmask;
        uint64_t test, alpha, clamp, tex1;
        uint32_t tme, fst, abe, fge, tfx, tcc;
        uint64_t texture; // GL texture name | (render target flag << 32)
        uint32_t texW, texH;
        float texAlphaScale;
        uint32_t gsW, gsH;   // GS texture size (2^TW, 2^TH): STQ coordinates are relative to it
        int32_t alphaFix;    // >= 0: texture alpha replaced by this (CT24 read of a drawn target)
        uint32_t sx0, sy0, sx1, sy1;
        uint32_t fogColor;
        uint32_t linear;
        uint32_t colclamp;
        uint64_t tex0; // raw TEX0 (trace only; equal for equal draws)
        bool operator==(const DrawKey &o) const { return std::memcmp(this, &o, sizeof(DrawKey)) == 0; }
    };

    constexpr uint64_t kNeverLoaded = ~0ull;

    // Games read one buffer at several widths (SotC: 256 and 512 at FBP 336):
    // each layout is its own target; GS pages have one owner among them.
    inline uint32_t makeTargetKey(uint32_t fbp, uint32_t fbw, uint32_t psm)
    {
        return (fbp & 0x1FFu) | ((std::max<uint32_t>(1u, fbw) & 0x3Fu) << 9) | ((psm & 0x3Fu) << 15);
    }

    // A GS frame buffer drawn by the GPU. GS memory and the GPU texture are
    // kept in sync per 8 KiB page (a 64x32 or 64x64 pixel cell): a page is
    // dirty when the GPU drew it after the last write-back, and stale when GS
    // memory changed since it was loaded (its epoch is newer than synced[]).
    struct RenderTarget
    {
        uint32_t fbp = 0, fbw = 0, psm = 0;
        uint32_t key = 0;               // targetKey(fbp, fbw, psm): one target per layout
        uint32_t width = 0, height = 0;
        GLuint texture = 0;
        uint32_t pw = 64, ph = 32;      // page size in pixels
        uint32_t cols = 0, rows = 0;    // pages across, down
        std::vector<uint8_t> dirty;     // per page
        std::vector<uint64_t> synced;   // per page: GS memory epoch last loaded/written (kNeverLoaded)
        uint32_t dirtyCount = 0;
        uint64_t version = 0;           // bumped whenever the GPU copy changes

        uint32_t page(uint32_t col, uint32_t row) const { return (fbp + row * cols + col) % 512u; }
        bool anyDirty() const { return dirtyCount != 0; }
    };

    // GS memory pages (8 KiB) a buffer rectangle's rows [y0, y1] cover, as bytes.
    void pageDims(uint32_t psm, uint32_t &pw, uint32_t &ph)
    {
        switch (psm)
        {
        case GS_PSM_CT16: case GS_PSM_CT16S: case GS_PSM_Z16: case GS_PSM_Z16S: pw = 64; ph = 64; break;
        case GS_PSM_T8: pw = 128; ph = 64; break;
        case GS_PSM_T4: pw = 128; ph = 128; break;
        default: pw = 64; ph = 32; break;
        }
    }

    void rowPages(uint32_t baseBytes, uint32_t bufferWidth, uint32_t y0, uint32_t y1, uint32_t psm, uint64_t &begin, uint64_t &end)
    {
        uint32_t pw = 64, ph = 32;
        pageDims(psm, pw, ph);
        const uint64_t perRow = std::max<uint32_t>(1u, (std::max<uint32_t>(1u, bufferWidth) + pw - 1u) / pw);
        const uint64_t basePage = baseBytes / 8192u;
        begin = (basePage + (y0 / ph) * perRow) * 8192u;
        end = (basePage + (y1 / ph + 1u) * perRow + ((baseBytes % 8192u) ? 1u : 0u)) * 8192u;
    }

    struct DepthTarget
    {
        uint32_t zbp = 0;
        uint32_t width = 0, height = 0;
        GLuint texture = 0;
#if defined(PS2X_GS_HW_VITA)
        // vitaGL: a framebuffer's depth buffer is its own (hidden). One per
        // Z buffer, its color attachment switched to the target drawn.
        GLuint fbo = 0;
        GLuint attached = 0;
        uint32_t fboWidth = 0, fboHeight = 0;
#endif
    };
}

#if defined(PS2X_GS_HW_VITA)
void GSHwBackend::setContextProvider(std::function<void()> makeCurrent)
{
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    if (!g_vitaProgram)
        g_vitaProgram = buildProgram(kVertexShaderCg, kFragmentShaderCg);
    if (!g_vitaVuProgram)
    {
        std::string fs = kFragmentShaderCg;
        const size_t at = fs.find("float4 main(");
        if (at != std::string::npos)
        {
            fs.replace(at, 11, "float4 gsMain");
            fs += kVuFragmentMainCg;
            g_vitaVuProgram = buildProgram(kVuVertexShaderCg, fs.c_str());
            GLint linked = 0;
            glGetProgramiv(g_vitaVuProgram, GL_LINK_STATUS, &linked);
            if (!linked)
            {
                std::cerr << "[gs:hw] VU batch program link failed" << std::endl;
                g_vitaVuProgram = 0;
            }
        }
    }
    g_makeCurrent = std::move(makeCurrent);
}
#endif

struct GSHwBackend::Impl
{
    std::unique_ptr<GSCpuBackend> shadow = std::make_unique<GSCpuBackend>();
    uint8_t *vram = nullptr;
    uint32_t vramSize = 0;

    bool glReady = false;
    bool glFailed = false;
    std::thread::id glThread;
    GLuint program = 0, vao = 0, vbo = 0;
    // VU1 batches: their own vertex stage and layout; the fragment stage is
    // the GS one (screen-space interpolation of colour, fog and Z).
    GLuint vuProgram = 0, vuVao = 0, vuVbo = 0;
    GLuint activeProgram = 0;
    std::unordered_map<std::string, GLint> vuUniforms;
    bool pendingVu = false;
    bool pendingNonSprite = false; // the queue has primitives other than sprites
    // Copies of a target's area for blends done in the shader (by size).
    struct DstCopy
    {
        GLuint texture = 0, fbo = 0;
    };
    std::unordered_map<uint64_t, DstCopy> dstCopies;
    std::vector<VuGpuVertex> vuPending;
    std::vector<float> vuMatrices; // 16 per batch
    std::vector<float> vuConstants; // 4 per batch: fog base, fog scale, XYOFFSET
    std::vector<float> vuNear; // 4 per batch: the near plane (Mac; the Vita gets batches that stay in front of it)
    GLuint wrapProgram = 0; // float sums -> 8-bit values modulo 256
    struct WrapTarget
    {
        GLuint texture = 0;
        uint32_t width = 0, height = 0;
    } wrapTarget;
    std::unordered_map<uint64_t, GLuint> wrapFramebuffers; // (depth zbp + 1) -> FBO
    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<uint32_t, RenderTarget> targets;  // by fbp
    // The one target whose GPU copy of each GS page is newer (-1: none).
    // Targets alias (one buffer read at two widths): a page has one owner.
    std::array<int32_t, 512> dirtyOwner = []
    {
        std::array<int32_t, 512> a{};
        a.fill(-1);
        return a;
    }();
    std::unordered_map<uint32_t, DepthTarget> depths;    // by zbp
    std::unordered_map<uint64_t, GLuint> framebuffers;   // (fbp << 32 | zbp+1)
    struct CachedTexture
    {
        GLuint texture = 0;
        uint64_t epoch = ~0ull;
        uint32_t width = 0, height = 0;
    };
    std::unordered_map<const void *, CachedTexture> textures;
    std::unordered_map<GLuint, uint32_t> textureMaxAlpha; // decoded textures: largest texel alpha

    std::vector<HwVertex> pending;
    DrawKey pendingKey{};
    bool hasPending = false;
    uint64_t drawCalls = 0, primitives = 0;
    // PS2X_GS_HW_STATS=1: per-second counters.
    struct Stats
    {
        uint64_t writeBacks = 0, writeBackPixels = 0, reloads = 0, textureUploads = 0, rtTextures = 0;
        double writeBackMs = 0, reloadMs = 0;
        uint64_t presentWb = 0, textureWb = 0, transferWb = 0, clutWb = 0, discards = 0, aliasWb = 0, otherWb = 0, memoryClears = 0, directPresents = 0, wrapDraws = 0, gpuPageMoves = 0, exactBlendCopies = 0;
    } stats;
    const char *wbReason = "other";
    // PS2X_GS_HW_TRACE=N: list the draw calls of the first frame after guest vsync N.
    // PS2X_GS_HW_VERIFY=1 (with PS2X_GS_HW_TRACE): the CPU renderer draws too
    // and owns GS memory; in the traced frame each draw call starts from its
    // result and the GPU's output is compared with it (mean |diff| per draw).
    const bool verify = std::getenv("PS2X_GS_HW_VERIFY") != nullptr;
    uint64_t presents = 0, tracedDraws = 0;
    bool traceDone = false;
    bool tracing() const
    {
        static const long frame = std::getenv("PS2X_GS_HW_TRACE") ? std::atol(std::getenv("PS2X_GS_HW_TRACE")) : -1;
        return frame >= 0 && !traceDone && presents >= static_cast<uint64_t>(frame);
    }
    void reportStats()
    {
#if defined(PS2X_GS_HW_VITA)
        static const bool on = true; // to the log file, once a second
#else
        static const bool on = std::getenv("PS2X_GS_HW_STATS") != nullptr;
#endif
        static auto last = std::chrono::steady_clock::now();
        if (!on)
            return;
        const auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration<double>(now - last).count() < 1.0)
            return;
        last = now;
        std::fprintf(stderr, "[gs:hw] draws=%llu prims=%llu wb=%llu (%.0f ms, %llu px; present %llu texture %llu transfer %llu clut %llu) reloads=%llu (%.0f ms) texUploads=%llu rtTex=%llu discards=%llu memClears=%llu direct=%llu wrap=%llu exactBlend=%llu pageMoves=%llu aliasWb=%llu otherWb=%llu\n",
                     (unsigned long long)drawCalls, (unsigned long long)primitives, (unsigned long long)stats.writeBacks, stats.writeBackMs,
                     (unsigned long long)stats.writeBackPixels, (unsigned long long)stats.presentWb, (unsigned long long)stats.textureWb,
                     (unsigned long long)stats.transferWb, (unsigned long long)stats.clutWb, (unsigned long long)stats.reloads, stats.reloadMs,
                     (unsigned long long)stats.textureUploads, (unsigned long long)stats.rtTextures, (unsigned long long)stats.discards,
                     (unsigned long long)stats.memoryClears, (unsigned long long)stats.directPresents,
                     (unsigned long long)stats.wrapDraws, (unsigned long long)stats.exactBlendCopies, (unsigned long long)stats.gpuPageMoves,
                     (unsigned long long)stats.aliasWb, (unsigned long long)stats.otherWb);
        std::fprintf(stderr, "[gs:hw] textures: decodes=%llu hashHits=%llu cacheClears=%llu\n", (unsigned long long)shadow->textureDecodes,
                     (unsigned long long)shadow->textureHashHits, (unsigned long long)shadow->textureCacheClears);
        shadow->textureDecodes = shadow->textureHashHits = 0;
        stats = {};
        drawCalls = primitives = 0;
    }

    bool onGlThread() const { return glReady && std::this_thread::get_id() == glThread; }

    bool ensureGl()
    {
        if (glReady)
            return std::this_thread::get_id() == glThread;
        if (glFailed || !g_makeCurrent)
            return false;
        g_makeCurrent();
        glThread = std::this_thread::get_id();
#if defined(PS2X_GS_HW_VITA)
        program = g_vitaProgram; // built on the main thread (setContextProvider)
#else
        program = buildProgram(kVertexShader, kFragmentShader);
#endif
        GLint ok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            glFailed = true;
            std::cerr << "[gs:hw] program link failed" << std::endl;
            return false;
        }
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, x)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, s)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, r)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, fog)));
        glUseProgram(program);
        glUniform1i(uniform("uTexture"), 0);
#if defined(PS2X_GS_HW_VITA)
        // No COLCLAMP wrap path yet (it needs a float target): wrapProgram 0.
        // Round 55: not measured in Vita3K (native_renderer::inEmulator, the
        // native renderer's probe at start-up). There it reads nothing back
        // (all "unknown": the defaults), and its 64 x 64 targets were the
        // attract demo's red mist: fill() rewrites a texture just drawn
        // into, so vitaGL moves that texture's storage and frees the old
        // block, which the framebuffer still points at; Vita3K keeps the
        // red-banded colour surface it rendered there, and the next 64 x 64
        // RGBA texture vitaGL put in that block (often the canyon mist's puff)
        // was sampled as it (proved with the bands made green: the mist came
        // out green, build/agent-scratch/r55/pg4). SOTC_GS_MEASURE_ROWS=1:
        // measured anyway.
        static const bool measureAnyway = std::getenv("SOTC_GS_MEASURE_ROWS") && std::atoi(std::getenv("SOTC_GS_MEASURE_ROWS")) != 0;
        if (native_renderer::inEmulator() && !measureAnyway)
            std::fprintf(stderr, "[gs:hw] vitaGL rows: not measured in an emulator (the defaults; SOTC_GS_MEASURE_ROWS=1 measures)\n");
        else
            measureOrientation();
        vuProgram = g_vitaVuProgram; // built on the main thread (setContextProvider)
#else
        {
            // VU batches: the GS fragment stage with screen-space colour, fog and Z.
            std::string fs = kFragmentShader;
            for (const char *v : {"in vec4 vColor;", "in float vFog;", "in float vDepth;"})
            {
                const size_t at = fs.find(v);
                if (at != std::string::npos)
                    fs.insert(at, "noperspective ");
            }
            vuProgram = buildProgram(kVuVertexShader, fs.c_str());
            GLint linked = 0;
            glGetProgramiv(vuProgram, GL_LINK_STATUS, &linked);
            if (!linked)
            {
                std::cerr << "[gs:hw] VU batch program link failed" << std::endl;
                vuProgram = 0;
            }
        }
#endif
        if (vuProgram)
        {
            glUseProgram(vuProgram);
            glUniform1i(glGetUniformLocation(vuProgram, "uTexture"), 0);
            glGenVertexArrays(1, &vuVao);
            glGenBuffers(1, &vuVbo);
            glBindVertexArray(vuVao);
            glBindBuffer(GL_ARRAY_BUFFER, vuVbo);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VuGpuVertex), reinterpret_cast<void *>(offsetof(VuGpuVertex, x)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VuGpuVertex), reinterpret_cast<void *>(offsetof(VuGpuVertex, s)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VuGpuVertex), reinterpret_cast<void *>(offsetof(VuGpuVertex, r)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VuGpuVertex), reinterpret_cast<void *>(offsetof(VuGpuVertex, batch)));
            glUseProgram(program);
        }
#if !defined(PS2X_GS_HW_VITA)
        {
            GLuint wvs = compile(GL_VERTEX_SHADER, kWrapVertexShader), wfs = compile(GL_FRAGMENT_SHADER, kWrapFragmentShader);
            wrapProgram = glCreateProgram();
            glAttachShader(wrapProgram, wvs);
            glAttachShader(wrapProgram, wfs);
            glLinkProgram(wrapProgram);
        }
#endif
        glReady = true;
        std::cerr << "[gs:hw] OpenGL renderer: " << reinterpret_cast<const char *>(glGetString(GL_RENDERER)) << std::endl;
        return true;
    }

#if defined(PS2X_GS_HW_VITA)
    // Round 55: in Vita3K a GS target that grows keeps its old texture
    // instead of deleting it (SOTC_GL_KEEP_TARGETS=0: deleted, as before).
    // Vita3K looks every texture up in its cache of colour surfaces first,
    // by address and row stride, and keeps a surface after the guest frees
    // its memory (it sees no frees, and no writes on this Mac): a texture
    // vitaGL later put in such memory was drawn as the old surface's
    // picture (the red mist of the attract demo, see measureOrientation's
    // call).
    std::vector<GLuint> retiredTargets;
    static bool keepRetiredTargets()
    {
        static const bool on = !std::getenv("SOTC_GL_KEEP_TARGETS") || std::atoi(std::getenv("SOTC_GL_KEEP_TARGETS")) != 0;
        return on && native_renderer::inEmulator(); // (the console reads its memory: nothing to keep there)
    }
    // Which way vitaGL puts rows into a framebuffer's memory for a draw, a
    // scissor and a blit's source and destination (it depends on how it was
    // built): each works on GS rows [0, 16) of a 64x64 target and the memory
    // rows reached say. GS row y must end up in texture memory row y.
    void measureOrientation()
    {
        GLuint tex[2] = {0, 0}, fbo[2] = {0, 0};
        glGenTextures(2, tex);
        glGenFramebuffers(2, fbo);
        for (int i = 0; i < 2; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, tex[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[i], 0);
        }
        // Texture memory: rows [0, redRows) red, the rest black.
        auto fill = [&](int i, uint32_t redRows)
        {
            std::vector<uint8_t> px(64u * 64u * 4u);
            for (uint32_t y = 0; y < 64; ++y)
                for (uint32_t x = 0; x < 64; ++x)
                {
                    uint8_t *p = px.data() + (y * 64u + x) * 4u;
                    p[0] = y < redRows ? 255 : 0;
                    p[1] = p[2] = 0;
                    p[3] = 255;
                }
            gpuIdle();
            glBindTexture(GL_TEXTURE_2D, tex[i]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
        };
        // 0: the top band (memory rows 0..15) is red, 1: the bottom band, -1: neither.
        auto band = [&](int i) -> int
        {
            gpuIdle();
            const uint8_t *m = textureMemory(tex[i]);
            if (!m)
                return -1;
            const bool top = m[4 * textureStride(64) + 32 * 4] > 128, bottom = m[60 * textureStride(64) + 32 * 4] > 128;
            return top && !bottom ? 0 : bottom && !top ? 1 : -1;
        };
        const char *names[3] = {"unknown", "top-down", "flipped"};
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        // 1. A draw over rows 0..15 with the y axis as is.
        fill(0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
        glViewport(0, 0, 64, 64);
        glUseProgram(program);
        glUniform2f(uniform("uTarget"), 64.0f, 64.0f);
        glUniform1f(uniform("uFlipY"), 1.0f);
        glUniform1f(uniform("uBlendFix"), -1.0f);
        setInt("uTme", 0);
        setInt("uFge", 0);
        setInt("uAtest", 1);
        setInt("uAtestFailed", 0);
        HwVertex quad[6]{};
        const float xy[6][2] = {{0, 0}, {64, 0}, {64, 16}, {0, 0}, {64, 16}, {0, 16}};
        for (int k = 0; k < 6; ++k)
        {
            quad[k].x = xy[k][0];
            quad[k].y = xy[k][1];
            quad[k].r = 255;
            quad[k].a = 128;
        }
        bindVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        const int draw = band(0);
        drawFlipY = draw == 1 ? -1.0f : 1.0f;
        // 2. A scissored clear of rows 0..15.
        fill(1, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[1]);
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 0, 64, 16);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        const int scissor = band(1);
        scissorFlip = scissor == 1;
        // 3. Blit rows 0..15 to rows 0..15, the source red in its top band:
        // nothing arrives if the source rows count from the bottom.
        fill(0, 16);
        fill(1, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[0]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo[1]);
        glBlitFramebuffer(0, 0, 64, 16, 0, 0, 64, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        int blitSrc = 0, blitDst = band(1);
        if (blitDst < 0)
        {
            blitSrc = 1;
            fill(1, 0);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[0]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo[1]);
            glBlitFramebuffer(0, 48, 64, 64, 0, 0, 64, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            blitDst = band(1);
        }
        if (blitDst < 0)
            blitSrc = -1;
        // blitRect flips both ends together.
        blitFlip = blitDst == 1;
        if (blitSrc >= 0 && blitDst >= 0 && blitSrc != blitDst)
            std::fprintf(stderr, "[gs:hw] vitaGL blits flip one end only: page moves will be wrong\n");
        std::fprintf(stderr, "[gs:hw] vitaGL rows: draw %s, scissor %s, blit source %s, blit target %s\n", names[draw + 1], names[scissor + 1],
                     names[blitSrc + 1], names[blitDst + 1]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (ps2_diag::env("SOTC_GL_LAYER_LOG"))
            for (int i = 0; i < 2; ++i)
                std::fprintf(stderr, "[gs:hw] orientation target %d (64x64) at %p\n", i, static_cast<const void *>(textureMemory(tex[i])));
        glDeleteFramebuffers(2, fbo);
        glDeleteTextures(2, tex);
        scissorState.valid = false;
    }
#endif

    GLint uniform(const char *name)
    {
        const bool vu = activeProgram != 0 && activeProgram == vuProgram;
        auto &cache = vu ? vuUniforms : uniforms;
        auto it = cache.find(name);
        if (it != cache.end())
            return it->second;
        const GLint location = glGetUniformLocation(vu ? vuProgram : program, name);
        cache.emplace(name, location);
        return location;
    }

    // Integer state: ints in GLSL, floats in the CG shader.
    void setInt(const char *name, GLint value)
    {
#if defined(PS2X_GS_HW_VITA)
        glUniform1f(uniform(name), static_cast<float>(value));
#else
        glUniform1i(uniform(name), value);
#endif
    }

    void bindVertices()
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
#if defined(PS2X_GS_HW_VITA)
        // The host draws with client arrays in between: state from scratch.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, x)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, s)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, r)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(HwVertex), reinterpret_cast<void *>(offsetof(HwVertex, fog)));
#endif
    }

    static void bufferRange(uint32_t baseBytes, uint32_t widthPixels, uint32_t heightPixels, uint32_t psm, uint64_t &begin, uint64_t &end)
    {
        begin = baseBytes;
        end = static_cast<uint64_t>(baseBytes) + static_cast<uint64_t>(widthPixels) * heightPixels * bytesPerPixel(psm);
    }

    // ---- GL helpers ----------------------------------------------------------

    // vitaGL redraws its scissor mask on every scissor change: keep the last
    // one (forgotten when the host drew a frame in between).
    struct ScissorState
    {
        bool valid = false, on = false;
        GLint x = 0, y = 0, w = 0, h = 0;
    } scissorState;
    uint32_t hostFramesSeen = ~0u;
    // How this GL draws, scissors and blits rows (vitaGL: measured at start;
    // GS row y is always texture memory row y).
    float drawFlipY = 1.0f;
    bool scissorFlip = false, blitFlip = false;

    void setScissor(const RenderTarget *rt, bool on, GLint x = 0, GLint y = 0, GLint w = 0, GLint h = 0)
    {
        if (on && scissorFlip && rt)
            y = static_cast<GLint>(rt->height) - y - h;
#if defined(PS2X_GS_HW_VITA)
        const uint32_t frames = ps2xVitaHostFrames();
        if (frames != hostFramesSeen)
        {
            hostFramesSeen = frames;
            scissorState.valid = false;
        }
#endif
        if (scissorState.valid && scissorState.on == on && (!on || (scissorState.x == x && scissorState.y == y && scissorState.w == w && scissorState.h == h)))
            return;
        if (on)
        {
            if (!scissorState.valid || !scissorState.on)
                glEnable(GL_SCISSOR_TEST);
            glScissor(x, y, w, h);
        }
        else
            glDisable(GL_SCISSOR_TEST);
        scissorState = {true, on, x, y, w, h};
    }

    // Copies [sx, sx + w) x [sy, sy + h) of one target's framebuffer to (dx, dy) of another's.
    void blitRect(GLuint from, GLuint to, uint32_t fromHeight, uint32_t toHeight, GLint sx, GLint sy, GLint dx, GLint dy, GLint w, GLint h)
    {
        setScissor(nullptr, false);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, from);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to);
        if (blitFlip)
        {
            sy = static_cast<GLint>(fromHeight) - sy - h;
            dy = static_cast<GLint>(toHeight) - dy - h;
        }
        glBlitFramebuffer(sx, sy, sx + w, sy + h, dx, dy, dx + w, dy + h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

#if defined(PS2X_GS_HW_VITA)
    // Waits for every queued GPU job. vitaGL then opens a scene on the bound
    // framebuffer: a small one keeps that cheap.
    GLuint idleFbo = 0, idleTexture = 0;
    void gpuIdle()
    {
        if (!idleFbo)
        {
            glGenTextures(1, &idleTexture);
            glBindTexture(GL_TEXTURE_2D, idleTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glGenFramebuffers(1, &idleFbo);
            glBindFramebuffer(GL_FRAMEBUFFER, idleFbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, idleTexture, 0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, idleFbo);
        glFinish();
    }
    uint8_t *textureMemory(GLuint texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        return static_cast<uint8_t *>(vglGetTexDataPointer(GL_TEXTURE_2D));
    }
    static uint32_t textureStride(uint32_t width) { return ((width + 7u) & ~7u) * 4u; }
#endif

    // RGBA8 pixels of rt's [x0, x0 + w) x [y0, y0 + h), GS row y0 first.
    void readTarget(RenderTarget &rt, uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t *out)
    {
#if defined(PS2X_GS_HW_VITA)
        // vitaGL's glReadPixels takes rows from the wrong end of a framebuffer
        // for partial reads, and does not wait for other framebuffers' jobs.
        gpuIdle();
        const uint8_t *memory = textureMemory(rt.texture);
        const uint32_t stride = textureStride(rt.width);
        if (!memory)
            return;
        for (uint32_t y = 0; y < h; ++y)
            std::memcpy(out + static_cast<size_t>(y) * w * 4u, memory + static_cast<size_t>(y0 + y) * stride + x0 * 4u, static_cast<size_t>(w) * 4u);
#else
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer(rt, nullptr));
        glReadPixels(static_cast<GLint>(x0), static_cast<GLint>(y0), static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, out);
#endif
    }

    // RGBA8 pixels into rt's [x0, x0 + w) x [y0, y0 + h).
    void writeTarget(RenderTarget &rt, uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, const uint8_t *in)
    {
#if defined(PS2X_GS_HW_VITA)
        // Once the GPU is done with it: vitaGL's glTexSubImage2D copies a
        // recently used texture first, which would race the jobs still
        // drawing it. (Not straight into its memory: Vita3K faults on CPU
        // writes to render target memory.)
        gpuIdle();
        glBindTexture(GL_TEXTURE_2D, rt.texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(x0), static_cast<GLint>(y0), static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA,
                        GL_UNSIGNED_BYTE, in);
#else
        glBindTexture(GL_TEXTURE_2D, rt.texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(x0), static_cast<GLint>(y0), static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA,
                        GL_UNSIGNED_BYTE, in);
#endif
    }

    // ---- GS memory <-> GPU ------------------------------------------------

    static uint64_t pageEpochOf(GSCpuBackend &shadow, uint32_t page) { return shadow.newestWrite(page * 8192ull, page * 8192ull + 8192ull); }

    // GPU -> GS memory for the dirty pages of rt in rows [r0, r1) whose GS
    // memory range overlaps [begin, end) (all dirty pages by default).
    void writeBackPages(RenderTarget &rt, uint64_t begin = 0, uint64_t end = ~0ull)
    {
        if (!rt.anyDirty())
            return;
        if (verify)
        {
            for (uint32_t i = 0; i < rt.dirty.size(); ++i)
                clearDirty(rt, i);
            return;
        }
        if (!vram)
            return;
        bool flushed = false;
        const auto t0 = std::chrono::steady_clock::now();
        std::vector<uint8_t> pixels;
        for (uint32_t row = 0; row < rt.rows; ++row)
        {
            // Dirty pages of this row that overlap the range: read the span once.
            uint32_t c0 = ~0u, c1 = 0;
            for (uint32_t col = 0; col < rt.cols; ++col)
            {
                const uint32_t i = row * rt.cols + col;
                const uint64_t pb = rt.page(col, row) * 8192ull;
                if (rt.dirty[i] && pb < end && begin < pb + 8192ull)
                {
                    c0 = std::min(c0, col);
                    c1 = std::max(c1, col);
                }
            }
            if (c0 == ~0u)
                continue;
            if (!flushed)
            {
                flushPending();
                flushed = true;
            }
            const uint32_t x0 = c0 * rt.pw, w = (c1 - c0 + 1) * rt.pw, y0 = row * rt.ph, h = std::min(rt.ph, rt.height - y0);
            pixels.resize(static_cast<size_t>(w) * h * 4u);
            readTarget(rt, x0, y0, w, h, pixels.data());
            {
                std::lock_guard<std::mutex> lock(shadow->vramMutex());
                for (uint32_t col = c0; col <= c1; ++col)
                {
                    const uint32_t i = row * rt.cols + col;
                    const uint64_t pb = rt.page(col, row) * 8192ull;
                    if (!rt.dirty[i] || !(pb < end && begin < pb + 8192ull))
                        continue;
                    for (uint32_t y = 0; y < h; ++y)
                        for (uint32_t x = (col - c0) * rt.pw; x < (col - c0 + 1) * rt.pw; ++x)
                        {
                            const uint8_t *p = pixels.data() + (static_cast<size_t>(y) * w + x) * 4u;
                            const uint32_t alpha = std::min(255u, (static_cast<uint32_t>(p[3]) * 128u + 127u) / 255u);
                            uint32_t value = p[0] | (p[1] << 8) | (p[2] << 16) | (alpha << 24);
                            if (rt.psm == GS_PSM_CT16 || rt.psm == GS_PSM_CT16S)
                                value = (p[0] >> 3) | ((p[1] >> 3) << 5) | ((p[2] >> 3) << 10) | (alpha >= 0x40 ? 0x8000u : 0u);
                            shadow->WriteVramUnlocked(rt.psm, rt.fbp * 32u, rt.fbw, x0 + x, y0 + y, value);
                        }
                    stats.writeBackPixels += static_cast<uint64_t>(rt.pw) * h;
                }
            }
            for (uint32_t col = c0; col <= c1; ++col)
            {
                const uint32_t i = row * rt.cols + col;
                const uint64_t pb = rt.page(col, row) * 8192ull;
                if (!rt.dirty[i] || !(pb < end && begin < pb + 8192ull))
                    continue;
                shadow->noteExternalWrite(pb, pb + 8192ull);
                rt.synced[i] = pageEpochOf(*shadow, rt.page(col, row));
                clearDirty(rt, i);
            }
            ++stats.writeBacks;
            if (!std::strcmp(wbReason, "present")) ++stats.presentWb;
            else if (!std::strcmp(wbReason, "texture")) ++stats.textureWb;
            else if (!std::strcmp(wbReason, "transfer")) ++stats.transferWb;
            else if (!std::strcmp(wbReason, "clut")) ++stats.clutWb;
            else if (!std::strcmp(wbReason, "alias")) ++stats.aliasWb;
            else
            {
                ++stats.otherWb;
                static const bool log = std::getenv("PS2X_GS_HW_WBLOG") != nullptr;
                if (log)
                    std::fprintf(stderr, "[gs:wb] %s fbp=%u psm=%u row=%u\n", wbReason, rt.fbp, rt.psm, row);
            }
        }
        if (flushed)
            stats.writeBackMs += std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    }

    // GS memory -> GPU for the stale, clean pages of rt touching the pixel
    // rectangle [x0, x1) x [y0, y1).
    void loadStalePages(RenderTarget &rt, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
    {
        x1 = std::min(x1, rt.width);
        y1 = std::min(y1, rt.height);
        if (x1 <= x0 || y1 <= y0)
            return;
        const uint32_t pc0 = x0 / rt.pw, pc1 = (x1 - 1) / rt.pw, pr0 = y0 / rt.ph, pr1 = (y1 - 1) / rt.ph;
        std::vector<uint8_t> pixels;
        for (uint32_t row = pr0; row <= pr1; ++row)
        {
            uint32_t c0 = ~0u, c1 = 0;
            for (uint32_t col = pc0; col <= pc1; ++col)
            {
                const uint32_t i = row * rt.cols + col;
                if (!rt.dirty[i] && (rt.synced[i] == kNeverLoaded || pageEpochOf(*shadow, rt.page(col, row)) > rt.synced[i]))
                {
                    c0 = std::min(c0, col);
                    c1 = std::max(c1, col);
                }
            }
            if (c0 == ~0u)
                continue;
            const auto t0 = std::chrono::steady_clock::now();
            ++stats.reloads;
            const uint32_t px0 = c0 * rt.pw, w = (c1 - c0 + 1) * rt.pw, py0 = row * rt.ph, h = std::min(rt.ph, rt.height - py0);
            pixels.resize(static_cast<size_t>(w) * h * 4u);
            {
                std::lock_guard<std::mutex> lock(shadow->vramMutex());
                for (uint32_t y = 0; y < h; ++y)
                    for (uint32_t x = 0; x < w; ++x)
                    {
                        uint32_t v = shadow->ReadVramUnlocked(rt.psm, rt.fbp * 32u, rt.fbw, px0 + x, py0 + y);
                        uint8_t *p = pixels.data() + (static_cast<size_t>(y) * w + x) * 4u;
                        if (rt.psm == GS_PSM_CT16 || rt.psm == GS_PSM_CT16S)
                        {
                            p[0] = static_cast<uint8_t>((v & 0x1Fu) << 3);
                            p[1] = static_cast<uint8_t>(((v >> 5) & 0x1Fu) << 3);
                            p[2] = static_cast<uint8_t>(((v >> 10) & 0x1Fu) << 3);
                            v = (v & 0x8000u) ? 0x80000000u : 0u;
                        }
                        else
                        {
                            p[0] = static_cast<uint8_t>(v);
                            p[1] = static_cast<uint8_t>(v >> 8);
                            p[2] = static_cast<uint8_t>(v >> 16);
                        }
                        const uint32_t raw = rt.psm == GS_PSM_CT24 ? 0x80u : (v >> 24);
                        p[3] = static_cast<uint8_t>(std::min(255u, raw * 255u / 128u));
                    }
            }
            // Pages in [c0, c1] that were not stale are reloaded too: they are
            // clean, so GS memory holds what the GPU has.
            ++rt.version;
            writeTarget(rt, px0, py0, w, h, pixels.data());
            for (uint32_t col = c0; col <= c1; ++col)
            {
                const uint32_t i = row * rt.cols + col;
                if (!rt.dirty[i])
                    rt.synced[i] = pageEpochOf(*shadow, rt.page(col, row));
            }
            stats.reloadMs += std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
        }
    }

    void markDirty(RenderTarget &rt, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
    {
        x1 = std::min(x1, rt.width);
        y1 = std::min(y1, rt.height);
        if (x1 <= x0 || y1 <= y0)
            return;
        for (uint32_t row = y0 / rt.ph; row <= (y1 - 1) / rt.ph; ++row)
            for (uint32_t col = x0 / rt.pw; col <= (x1 - 1) / rt.pw; ++col)
            {
                uint8_t &d = rt.dirty[row * rt.cols + col];
                if (!d)
                {
                    d = 1;
                    ++rt.dirtyCount;
                }
                dirtyOwner[rt.page(col, row)] = static_cast<int32_t>(rt.key);
            }
    }

    void clearDirty(RenderTarget &rt, uint32_t i)
    {
        if (!rt.dirty[i])
            return;
        rt.dirty[i] = 0;
        --rt.dirtyCount;
        const uint32_t page = (rt.fbp + i) % 512u;
        if (dirtyOwner[page] == static_cast<int32_t>(rt.key))
            dirtyOwner[page] = -1;
    }

    // A GS page is the same 64x32 (or 64x64) pixel block whatever the buffer
    // width: the newer copy of one moves between two targets of one format as
    // a GPU copy of that block. The receiving target then owns it (dirty).
    bool movePageOnGpu(RenderTarget &from, RenderTarget &to, uint32_t page, uint32_t toCol, uint32_t toRow)
    {
        const bool sameFormat = from.psm == to.psm || ((from.psm == GS_PSM_CT32 || from.psm == GS_PSM_CT24) && (to.psm == GS_PSM_CT32 || to.psm == GS_PSM_CT24));
        if (!sameFormat || from.pw != to.pw || from.ph != to.ph)
            return false;
        uint32_t fromIndex = ~0u;
        for (uint32_t i = 0; i < from.dirty.size(); ++i)
            if ((from.fbp + i) % 512u == page)
            {
                fromIndex = i;
                break;
            }
        if (fromIndex == ~0u || !from.dirty[fromIndex])
            return false;
        const uint32_t fx = (fromIndex % from.cols) * from.pw, fy = (fromIndex / from.cols) * from.ph;
        const uint32_t tx = toCol * to.pw, ty = toRow * to.ph;
        if (fy + from.ph > from.height || ty + to.ph > to.height)
            return false;
        flushPending();
        blitRect(framebuffer(from, nullptr), framebuffer(to, nullptr), from.height, to.height, fx, fy, tx, ty, from.pw, from.ph);
        clearDirty(from, fromIndex);
        const uint32_t toIndex = toRow * to.cols + toCol;
        if (!to.dirty[toIndex])
        {
            to.dirty[toIndex] = 1;
            ++to.dirtyCount;
        }
        dirtyOwner[page] = static_cast<int32_t>(to.key);
        to.synced[toIndex] = pageEpochOf(*shadow, page);
        ++to.version;
        ++stats.gpuPageMoves;
        return true;
    }

    // Before rt uses the pages of a rectangle: pages another target holds
    // newer on the GPU go to GS memory first (or are dropped when the caller
    // overwrites them whole), so rt then loads them.
    void resolveAliases(RenderTarget &rt, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, bool drop = false)
    {
        x1 = std::min(x1, rt.width);
        y1 = std::min(y1, rt.height);
        if (x1 <= x0 || y1 <= y0)
            return;
        for (uint32_t row = y0 / rt.ph; row <= (y1 - 1) / rt.ph; ++row)
            for (uint32_t col = x0 / rt.pw; col <= (x1 - 1) / rt.pw; ++col)
            {
                const uint32_t page = rt.page(col, row);
                const int32_t owner = dirtyOwner[page];
                if (owner < 0 || owner == static_cast<int32_t>(rt.key))
                    continue;
                auto it = targets.find(static_cast<uint32_t>(owner));
                if (it == targets.end())
                {
                    dirtyOwner[page] = -1;
                    continue;
                }
                RenderTarget &other = it->second;
                if (drop)
                {
                    for (uint32_t i = 0; i < other.dirty.size(); ++i)
                        if ((other.fbp + i) % 512u == page)
                            clearDirty(other, i);
                }
                else if (!movePageOnGpu(other, rt, page, col, row))
                {
                    const char *reason = wbReason;
                    wbReason = "alias";
                    writeBackPages(other, page * 8192ull, page * 8192ull + 8192ull);
                    wbReason = reason;
                }
                if (dirtyOwner[page] == owner)
                    dirtyOwner[page] = -1;
            }
    }

    bool dirtyOverlaps(const RenderTarget &rt, uint64_t begin, uint64_t end)
    {
        if (!rt.anyDirty())
            return false;
        for (uint32_t i = 0; i < rt.dirty.size(); ++i)
        {
            const uint64_t pb = ((rt.fbp + i) % 512u) * 8192ull;
            if (rt.dirty[i] && pb < end && begin < pb + 8192ull)
                return true;
        }
        return false;
    }

    // CPU-side users of GS memory [begin, end): GPU results land there first.
    void writeBackOverlapping(uint64_t begin, uint64_t end)
    {
        // A GPU-newer page has one owner (dirtyOwner): look the range's pages
        // up instead of scanning every target's pages (once per texture).
        if (end <= begin)
            return;
        const uint64_t firstPage = begin / 8192u, lastPage = (end - 1u) / 8192u;
        const uint64_t count = std::min<uint64_t>(lastPage - firstPage + 1u, 512u);
        for (uint64_t i = 0; i < count; ++i)
        {
            const uint32_t page = static_cast<uint32_t>((firstPage + i) % 512u);
            const int32_t owner = dirtyOwner[page];
            if (owner < 0)
                continue;
            auto it = targets.find(static_cast<uint32_t>(owner));
            if (it == targets.end())
            {
                dirtyOwner[page] = -1;
                continue;
            }
            writeBackPages(it->second, begin, end); // all of its dirty pages in the range
        }
    }

    // A transfer only waits for the queued draws when it writes or reads GS
    // memory they draw into (target, Z) or sample: otherwise uploads (many a
    // frame) would split the queue into small draws.
    void flushPendingBeforeTransfer(const GSTransferCommand &command)
    {
        if (!hasPending)
            return;
        std::array<uint8_t, 512> pages{};
        auto mark = [&pages](uint64_t begin, uint64_t end)
        {
            for (uint64_t page = begin / 8192u; page < (end + 8191u) / 8192u && page < begin / 8192u + 512u; ++page)
                pages[page % 512u] = 1;
        };
        const auto &b = command.bitbltbuf;
        const uint32_t rows = std::max<uint32_t>(1u, command.trxreg.rrh);
        uint64_t begin = 0, end = 0;
        rowPages(b.dbp * 256u, std::max<uint32_t>(1u, b.dbw) * 64u, command.trxpos.dsay, command.trxpos.dsay + rows - 1u, b.dpsm, begin, end);
        mark(begin, end);
        if (command.direction != 0u)
        {
            rowPages(b.sbp * 256u, std::max<uint32_t>(1u, b.sbw) * 64u, command.trxpos.ssay, command.trxpos.ssay + rows - 1u, b.spsm, begin, end);
            mark(begin, end);
        }
        auto hits = [&pages](uint32_t firstPage, uint32_t count)
        {
            for (uint32_t i = 0; i < count && i < 512u; ++i)
                if (pages[(firstPage + i) % 512u])
                    return true;
            return false;
        };
        const DrawKey &key = pendingKey;
        auto it = targets.find(makeTargetKey(key.fbp, key.fbw, key.fpsm));
        const uint32_t targetPages = it != targets.end() ? static_cast<uint32_t>(it->second.dirty.size()) : 512u;
        bool touches = hits(key.fbp, targetPages) || hits(key.zbp, targetPages);
        if (!touches && key.tme)
        {
            const uint32_t tbp0 = static_cast<uint32_t>(key.tex0 & 0x3FFFu), tbw = static_cast<uint32_t>((key.tex0 >> 14) & 0x3Fu);
            const uint32_t tpsm = static_cast<uint32_t>((key.tex0 >> 20) & 0x3Fu);
            rowPages(tbp0 * 256u, std::max<uint32_t>(1u, tbw) * 64u, 0u, std::max<uint32_t>(1u, key.gsH) - 1u, tpsm, begin, end);
            touches = hits(static_cast<uint32_t>(begin / 8192u), static_cast<uint32_t>((end - begin + 8191u) / 8192u));
        }
        if (touches)
            flushPending();
    }

    // A host upload into the rectangle of a transfer: GPU pages it replaces
    // whole are dropped (they reload from GS memory when drawn or sampled);
    // pages it covers in part are written back first so the rest survives.
    void prepareUpload(const GSTransferCommand &command)
    {
        const auto &b = command.bitbltbuf;
        uint32_t pw = 64, ph = 32;
        pageDims(b.dpsm, pw, ph);
        const uint32_t bufferWidth = std::max<uint32_t>(1u, b.dbw) * 64u;
        const uint32_t perRow = std::max<uint32_t>(1u, bufferWidth / pw);
        const uint32_t x0 = command.trxpos.dsax, y0 = command.trxpos.dsay;
        const uint32_t x1 = x0 + std::max<uint32_t>(1u, command.trxreg.rrw), y1 = y0 + std::max<uint32_t>(1u, command.trxreg.rrh);
        const bool aligned = (b.dbp % 32u) == 0u;
        std::vector<uint8_t> whole(512, 0), touched(512, 0);
        for (uint32_t row = y0 / ph; row <= (y1 - 1) / ph; ++row)
            for (uint32_t col = x0 / pw; col <= (x1 - 1) / pw && col < perRow; ++col)
            {
                const uint32_t page = (b.dbp / 32u + row * perRow + col) % 512u;
                touched[page] = 1;
                if (aligned && x0 <= col * pw && (col + 1) * pw <= x1 && y0 <= row * ph && (row + 1) * ph <= y1)
                    whole[page] = 1;
                if (!aligned) // the rectangle straddles into the next page too
                    touched[(page + 1) % 512u] = 1;
            }
        for (auto &[targetKey, rt] : targets)
        {
            if (!rt.anyDirty())
                continue;
            for (uint32_t i = 0; i < rt.dirty.size(); ++i)
            {
                const uint32_t page = (rt.fbp + i) % 512u;
                if (!rt.dirty[i] || !touched[page])
                    continue;
                if (whole[page])
                {
                    clearDirty(rt, i); // replaced: GS memory's new contents win
                    ++stats.discards;
                }
                else
                    writeBackPages(rt, page * 8192ull, page * 8192ull + 8192ull);
            }
        }
    }

    RenderTarget &target(uint32_t fbp, uint32_t fbw, uint32_t psm, uint32_t neededHeight)
    {
        const uint32_t key = makeTargetKey(fbp, fbw, psm);
        RenderTarget &rt = targets[key];
        const uint32_t width = std::max<uint32_t>(1u, fbw) * 64u;
        uint32_t pw = 64, ph = 32;
        pageDims(psm, pw, ph);
        const uint32_t height = std::max<uint32_t>(rt.height, std::min<uint32_t>(2048u, (neededHeight + 63u) & ~63u));
        if (rt.texture == 0 || rt.width != width || rt.height < height || rt.psm != psm)
        {
            if (rt.texture != 0)
            {
                writeBackPages(rt);
#if defined(PS2X_GS_HW_VITA)
                for (auto &[zbp, d] : depths)
                    if (d.attached == rt.texture)
                        d.attached = 0;
#endif
#if defined(PS2X_GS_HW_VITA)
                // Round 55: a GS target that grows leaves its old texture
                // kept, not deleted (keepRetiredTargets): Vita3K keeps the
                // colour surface it saw at that address after the memory is
                // freed and samples a texture vitaGL later puts there, with
                // the same row stride, as that old surface.
                if (ps2_diag::env("SOTC_GL_LAYER_LOG"))
                    std::fprintf(stderr, "[gs:hw] target fbp %u %ux%u at %p %s\n", rt.fbp, rt.width, rt.height, static_cast<const void *>(textureMemory(rt.texture)),
                                 keepRetiredTargets() ? "retired (kept)" : "deleted");
                if (keepRetiredTargets())
                    retiredTargets.push_back(rt.texture);
                else
#endif
                glDeleteTextures(1, &rt.texture);
                for (auto it = framebuffers.begin(); it != framebuffers.end();)
                {
                    if ((it->first >> 32) == key)
                    {
                        glDeleteFramebuffers(1, &it->second);
                        it = framebuffers.erase(it);
                    }
                    else
                        ++it;
                }
            }
            rt.fbp = fbp;
            rt.key = key;
            rt.fbw = std::max<uint32_t>(1u, fbw);
            rt.psm = psm;
            rt.width = width;
            rt.height = std::max<uint32_t>(height, 64u);
            rt.pw = pw;
            rt.ph = ph;
            rt.cols = std::max<uint32_t>(1u, width / pw);
            rt.rows = (rt.height + ph - 1u) / ph;
            rt.dirty.assign(static_cast<size_t>(rt.cols) * rt.rows, 0);
            rt.synced.assign(static_cast<size_t>(rt.cols) * rt.rows, kNeverLoaded); // every page stale
            rt.dirtyCount = 0;
            glGenTextures(1, &rt.texture);
            glBindTexture(GL_TEXTURE_2D, rt.texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rt.width, rt.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#if defined(PS2X_GS_HW_VITA)
            if (ps2_diag::env("SOTC_GL_LAYER_LOG")) // (debugging, round 55: where the GS's targets live)
                std::fprintf(stderr, "[gs:hw] target fbp %u fbw %u %ux%u at %p (%u bytes)\n", fbp, rt.fbw, rt.width, rt.height,
                             static_cast<const void *>(textureMemory(rt.texture)), ((rt.width + 7u) & ~7u) * 4u * rt.height);
#endif
        }
        return rt;
    }

    DepthTarget &depth(uint32_t zbp, uint32_t width, uint32_t height)
    {
#if defined(PS2X_GS_HW_VITA)
        // One depth buffer per Z buffer and size: vitaGL drops it when the
        // attached size changes (SotC draws its shadow maps with the scene's
        // ZBP at 256x256 and 128x128).
        DepthTarget &v = depths[zbp | ((width >> 6) << 9) | ((height >> 6) << 20)];
        v.zbp = zbp;
        v.width = std::max(v.width, width);
        v.height = std::max(v.height, height);
        return v;
#endif
        DepthTarget &d = depths[zbp];
        if (d.texture == 0 || d.width < width || d.height < height)
        {
            if (d.texture)
                glDeleteTextures(1, &d.texture);
            d.zbp = zbp;
            d.width = std::max(d.width, width);
            d.height = std::max(d.height, height);
            glGenTextures(1, &d.texture);
            glBindTexture(GL_TEXTURE_2D, d.texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, d.width, d.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            for (auto it = framebuffers.begin(); it != framebuffers.end();)
            {
                if ((it->first & 0xFFFFFFFFu) == zbp + 1u)
                {
                    glDeleteFramebuffers(1, &it->second);
                    it = framebuffers.erase(it);
                }
                else
                    ++it;
            }
        }
        return d;
    }

    GLuint framebuffer(RenderTarget &rt, DepthTarget *d)
    {
#if defined(PS2X_GS_HW_VITA)
        if (d)
        {
            // The Z buffer's framebuffer with rt as its color. vitaGL keeps the
            // depth buffer while the attached size stays the same.
            if (!d->fbo)
                glGenFramebuffers(1, &d->fbo);
            if (d->attached != rt.texture)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, d->fbo);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.texture, 0);
                if (d->fboWidth && (d->fboWidth != rt.width || d->fboHeight != rt.height))
                    std::fprintf(stderr, "[gs:hw] Z buffer %u: %ux%u target fbp=%u after %ux%u, depth lost\n", d->zbp, rt.width, rt.height, rt.fbp, d->fboWidth,
                                 d->fboHeight);
                d->attached = rt.texture;
                d->fboWidth = rt.width;
                d->fboHeight = rt.height;
            }
            return d->fbo;
        }
#endif
        const uint64_t key = (static_cast<uint64_t>(rt.key) << 32) | (d ? d->zbp + 1u : 0u);
        auto it = framebuffers.find(key);
        if (it != framebuffers.end())
            return it->second;
        GLuint fbo = 0;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.texture, 0);
        if (d)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, d->texture, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "[gs:hw] incomplete framebuffer fbp=" << rt.fbp << std::endl;
        framebuffers.emplace(key, fbo);
        return fbo;
    }

    // ---- Textures -----------------------------------------------------------

    // Returns the GL texture for the draw (0: none) and its alpha scale.
    GLuint textureFor(const GSDrawState &state, float &alphaScale, uint32_t &texW, uint32_t &texH, int32_t &alphaFix)
    {
        const auto &tex = state.context.tex0;
        texW = state.textureWidth;
        texH = state.textureHeight;
        alphaFix = -1;
        // A texture at the start of a render target the GPU drew: sample it there.
        for (auto &[targetKey, rt] : targets)
        {
            const bool sameFormat = tex.psm == rt.psm || (tex.psm == GS_PSM_CT24 && rt.psm == GS_PSM_CT32);
            if (tex.tbp0 == rt.fbp * 32u && sameFormat && rt.texture && tex.tbw * 64u == rt.width && rt.anyDirty())
            {
                // Pages the texture reaches that the GPU does not hold yet come
                // from GS memory first.
                flushPending();
                resolveAliases(rt, 0, 0, std::max<uint32_t>(state.textureWidth, 1u), std::max<uint32_t>(state.textureHeight, 1u));
                loadStalePages(rt, 0, 0, std::max<uint32_t>(state.textureWidth, 1u), std::max<uint32_t>(state.textureHeight, 1u));
                if (tracing())
                    checkRtTexture(rt, state);
                alphaScale = kAlphaOne / 255.0f; // targets keep alpha as A / 0x80
                if (tex.psm == GS_PSM_CT24)
                    alphaFix = state.texa.ta0 + (state.texa.aem ? 256 : 0);
                texW = rt.width;
                texH = rt.height;
                ++stats.rtTextures;
                return rt.texture;
            }
        }
        // Otherwise decode from GS memory, after the GPU's results reach it.
        uint64_t begin = 0, end = 0;
        rowPages(tex.tbp0 * 256u, std::max<uint32_t>(state.textureWidth, tex.tbw * 64u), 0u, state.textureHeight - 1u, tex.psm, begin, end);
        wbReason = "texture";
        writeBackOverlapping(begin, end);
        wbReason = "other";
        uint32_t w = 0, h = 0;
        uint64_t epoch = 0;
        const std::vector<uint32_t> *texels = shadow->textureForHw(state, w, h, epoch);
        if (!texels || w == 0 || h == 0)
            return 0;
        CachedTexture &cached = textures[texels];
        if (cached.texture == 0)
            glGenTextures(1, &cached.texture);
        glBindTexture(GL_TEXTURE_2D, cached.texture);
        if (cached.epoch != epoch || cached.width != w || cached.height != h)
        {
            // Queued draws may sample this texture as it was: draw them first.
            if (hasPending && cached.width != 0)
                flushPending();
            glBindTexture(GL_TEXTURE_2D, cached.texture);
            if (cached.width == w && cached.height == h)
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, texels->data());
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels->data());
            ++stats.textureUploads;
            uint32_t maxAlpha = 0;
            for (uint32_t t : *texels)
                maxAlpha = std::max(maxAlpha, t >> 24);
            textureMaxAlpha[cached.texture] = maxAlpha;
            cached.epoch = epoch;
            cached.width = w;
            cached.height = h;
        }
        alphaScale = 1.0f; // decoded texels carry the GS alpha as is
        texW = w;
        texH = h;
        return cached.texture;
    }

    // ---- Drawing -------------------------------------------------------------

    // Returns FIX when the blend reads it from the source alpha instead of a
    // constant (no glBlendColor): the shader then outputs FIX as alpha (-1: no).
    int32_t applyBlend(const DrawKey &key)
    {
        if (!key.abe)
        {
            glDisable(GL_BLEND);
            return -1;
        }
        // (A - B) * C + D with A, B, D in {Cs, Cd, 0} and C in {As, Ad, FIX}.
        const uint32_t a = key.alpha & 3u, b = (key.alpha >> 2) & 3u, c = (key.alpha >> 4) & 3u, d = (key.alpha >> 6) & 3u;
#if defined(PS2X_GS_HW_VITA)
        const int32_t fix = c == 2 ? static_cast<int32_t>((key.alpha >> 32) & 0xFFu) : -1;
        const GLenum cf = c == 1 ? GL_DST_ALPHA : GL_SRC_ALPHA;
        const GLenum cinv = c == 1 ? GL_ONE_MINUS_DST_ALPHA : GL_ONE_MINUS_SRC_ALPHA;
#else
        const int32_t fix = -1;
        const GLenum cf = c == 0 ? GL_SRC_ALPHA : c == 1 ? GL_DST_ALPHA : GL_CONSTANT_ALPHA;
        const GLenum cinv = c == 0 ? GL_ONE_MINUS_SRC_ALPHA : c == 1 ? GL_ONE_MINUS_DST_ALPHA : GL_ONE_MINUS_CONSTANT_ALPHA;
        if (c == 2)
            glBlendColor(0, 0, 0, static_cast<float>((key.alpha >> 32) & 0xFFu) / kAlphaOne);
#endif
        glEnable(GL_BLEND);
        GLenum eq = GL_FUNC_ADD, src = GL_ONE, dst = GL_ZERO;
        enum { CS = 0, CD = 1, ZERO = 2 };
        if (a == CS && b == CD && d == CD) { src = cf; dst = cinv; }
        else if (a == CS && b == ZERO && d == CD) { src = cf; dst = GL_ONE; }
        else if (a == CS && b == ZERO && d == ZERO) { src = cf; dst = GL_ZERO; }
        else if (a == CS && b == CD && d == ZERO) { eq = GL_FUNC_SUBTRACT; src = cf; dst = cf; }
        else if (a == CD && b == CS && d == CS) { src = cinv; dst = cf; }
        else if (a == ZERO && b == CS && d == CD) { eq = GL_FUNC_REVERSE_SUBTRACT; src = cf; dst = GL_ONE; }
        else if (a == CD && b == ZERO && d == CS) { src = GL_ONE; dst = cf; }
        else if (a == CD && b == ZERO && d == ZERO) { src = GL_ZERO; dst = cf; }
        else if (a == b) { src = d == CS ? GL_ONE : GL_ZERO; dst = d == CD ? GL_ONE : GL_ZERO; }
        else if (a == CD && b == CS && d == CD) { eq = GL_FUNC_REVERSE_SUBTRACT; src = cf; dst = GL_ONE; } // approx: Cd - Cs*C
        else { src = cf; dst = cinv; }
        glBlendEquationSeparate(eq, GL_FUNC_ADD);
        // The frame buffer keeps the source alpha (the GS does not blend alpha).
        glBlendFuncSeparate(src, dst, GL_ONE, GL_ZERO);
        return fix;
    }

    // Trace: a render target about to be sampled, against the texture decoded
    // from GS memory after a write-back (what the CPU path would sample).
    void checkRtTexture(RenderTarget &rt, const GSDrawState &state)
    {
        flushPending();
        const uint32_t w = std::min<uint32_t>(rt.width, state.textureWidth), h = std::min<uint32_t>(rt.height, state.textureHeight);
        std::vector<uint8_t> px(static_cast<size_t>(rt.width) * h * 4u);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer(rt, nullptr));
        glReadPixels(0, 0, rt.width, h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
        writeBackPages(rt);
        uint32_t tw = 0, th = 0;
        uint64_t epoch = 0;
        const std::vector<uint32_t> *texels = shadow->textureForHw(state, tw, th, epoch);
        if (!texels)
            return;
        double dRgb = 0, dA = 0;
        uint64_t n = 0;
        for (uint32_t y = 0; y < std::min(h, th); ++y)
            for (uint32_t x = 0; x < std::min(w, tw); ++x)
            {
                const uint8_t *p = px.data() + (static_cast<size_t>(y) * rt.width + x) * 4u;
                const uint32_t t = (*texels)[static_cast<size_t>(y) * tw + x];
                dRgb += std::abs(int(p[0]) - int(t & 0xFF)) + std::abs(int(p[1]) - int((t >> 8) & 0xFF)) + std::abs(int(p[2]) - int((t >> 16) & 0xFF));
                const int ga = state.context.tex0.psm == GS_PSM_CT24 ? state.texa.ta0 : int((p[3] * 128u + 127u) / 255u);
                dA += std::abs(ga - int(t >> 24));
                ++n;
            }
        std::fprintf(stderr, "[gs:rtcheck] fbp=%u %ux%u (rt %ux%u dirtyPages=%u) tpsm=%u dRGB=%.2f dA=%.2f\n", rt.fbp, w, h, rt.width, rt.height, rt.dirtyCount,
                     state.context.tex0.psm, n ? dRgb / n / 3.0 : 0.0, n ? dA / n : 0.0);
    }

    // Verify mode: the CPU renderer's Z buffer into the GPU depth target, so
    // depth-tested draws start from the same state.
    GLuint depthLoadProgram = 0, depthLoadTexture = 0;
    void loadDepthFromCpu(RenderTarget &rt, const DrawKey &key)
    {
#if defined(PS2X_GS_HW_VITA)
        (void)rt;
        (void)key;
        return; // verify mode is desktop only
#else
        DepthTarget &d = depth(key.zbp, rt.width, rt.height);
        if (!depthLoadProgram)
        {
            static const char *vs = R"(#version 330 core
void main() { vec2 p = vec2((gl_VertexID & 1) != 0 ? 3.0 : -1.0, (gl_VertexID & 2) != 0 ? 3.0 : -1.0); gl_Position = vec4(p, 0.0, 1.0); }
)";
            static const char *fs = R"(#version 330 core
uniform sampler2D uZ;
out vec4 oColor;
void main() { gl_FragDepth = texelFetch(uZ, ivec2(gl_FragCoord.xy), 0).r; oColor = vec4(0.0); }
)";
            depthLoadProgram = glCreateProgram();
            glAttachShader(depthLoadProgram, compile(GL_VERTEX_SHADER, vs));
            glAttachShader(depthLoadProgram, compile(GL_FRAGMENT_SHADER, fs));
            glLinkProgram(depthLoadProgram);
            glGenTextures(1, &depthLoadTexture);
        }
        const float scale = (key.zpsm == GS_PSM_Z16 || key.zpsm == GS_PSM_Z16S) ? 65536.0f : key.zpsm == GS_PSM_Z24 ? 16777216.0f : 4294967296.0f;
        std::vector<float> z(static_cast<size_t>(rt.width) * rt.height);
        {
            std::lock_guard<std::mutex> lock(shadow->vramMutex());
            for (uint32_t y = 0; y < rt.height; ++y)
                for (uint32_t x = 0; x < rt.width; ++x)
                {
                    uint32_t v = shadow->ReadVramUnlocked(key.zpsm, key.zbp * 32u, rt.fbw, x, y);
                    if (key.zpsm == GS_PSM_Z24)
                        v &= 0xFFFFFFu;
                    z[static_cast<size_t>(y) * rt.width + x] = static_cast<float>(std::min(1.0, v / static_cast<double>(scale)));
                }
        }
        glBindTexture(GL_TEXTURE_2D, depthLoadTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, rt.width, rt.height, 0, GL_RED, GL_FLOAT, z.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer(rt, &d));
        glViewport(0, 0, rt.width, rt.height);
        setScissor(&rt, false);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glDepthMask(GL_TRUE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glUseProgram(depthLoadProgram);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(depthLoadProgram, "uZ"), 0);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(program);
#endif
    }

    std::vector<uint8_t> verifyBefore; // traced verify frame: target before the draw
    void compareWithCpu(RenderTarget &rt)
    {
        const DrawKey &key = pendingKey;
        float fx0 = 1e9f, fy0 = 1e9f, fx1 = -1e9f, fy1 = -1e9f;
        for (const auto &v : pending)
        {
            fx0 = std::min(fx0, v.x); fy0 = std::min(fy0, v.y); fx1 = std::max(fx1, v.x); fy1 = std::max(fy1, v.y);
        }
        const int x0 = std::max<int>(key.sx0, static_cast<int>(fx0)), x1 = std::min<int>(key.sx1, static_cast<int>(fx1));
        const int y0 = std::max<int>(key.sy0, static_cast<int>(fy0)), y1 = std::min<int>(key.sy1, static_cast<int>(fy1));
        if (x1 < x0 || y1 < y0)
            return;
        const int w = x1 - x0 + 1, h = y1 - y0 + 1;
        std::vector<uint8_t> px(static_cast<size_t>(w) * h * 4u);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer(rt, nullptr));
        glReadPixels(x0, y0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
        (void)shadow->ReadVram(GS_PSM_CT32, 0, 1, 0, 0);
        double dRgb = 0, dA = 0, gpuSum = 0, cpuSum = 0;
        uint32_t maxD = 0;
        int worstX = 0, worstY = 0;
        uint32_t worstCpu = 0;
        uint32_t onlyGpu = 0, onlyCpu = 0, both = 0; // pixels changed by the draw on one side only
        std::lock_guard<std::mutex> lock(shadow->vramMutex());
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
            {
                const uint8_t *p = px.data() + (static_cast<size_t>(y) * w + x) * 4u;
                uint32_t v = shadow->ReadVramUnlocked(rt.psm, rt.fbp * 32u, rt.fbw, x0 + x, y0 + y);
                uint32_t cr = v & 0xFF, cg = (v >> 8) & 0xFF, cb = (v >> 16) & 0xFF, ca = v >> 24;
                if (rt.psm == GS_PSM_CT16 || rt.psm == GS_PSM_CT16S)
                {
                    cr = (v & 0x1F) << 3; cg = ((v >> 5) & 0x1F) << 3; cb = ((v >> 10) & 0x1F) << 3; ca = (v & 0x8000) ? 0x80 : 0;
                }
                const uint32_t ga = std::min(255u, (static_cast<uint32_t>(p[3]) * 128u + 127u) / 255u);
                const uint32_t d = static_cast<uint32_t>(std::abs(int(p[0]) - int(cr)) + std::abs(int(p[1]) - int(cg)) + std::abs(int(p[2]) - int(cb)));
                dRgb += d;
                if (d > maxD)
                {
                    worstX = x0 + x;
                    worstY = y0 + y;
                    worstCpu = cr | (cg << 8) | (cb << 16) | (ca << 24);
                }
                maxD = std::max(maxD, d);
                if (!verifyBefore.empty() && d > 24u)
                {
                    const uint8_t *b = verifyBefore.data() + (static_cast<size_t>(y0 + y) * rt.width + (x0 + x)) * 4u;
                    const bool gpuChanged = std::abs(int(p[0]) - int(b[0])) + std::abs(int(p[1]) - int(b[1])) + std::abs(int(p[2]) - int(b[2])) > 6;
                    const bool cpuChanged = std::abs(int(cr) - int(b[0])) + std::abs(int(cg) - int(b[1])) + std::abs(int(cb) - int(b[2])) > 6;
                    if (gpuChanged && !cpuChanged) ++onlyGpu;
                    else if (cpuChanged && !gpuChanged) ++onlyCpu;
                    else ++both;
                }
                if (rt.psm == GS_PSM_CT32)
                    dA += std::abs(int(ga) - int(ca));
                gpuSum += p[0] + p[1] + p[2];
                cpuSum += cr + cg + cb;
            }
        const double n = static_cast<double>(w) * h;
        std::fprintf(stderr, "[gs:verify] fbp=%u area=%dx%d@%d,%d dRGB=%.2f max=%u dA=%.2f gpuMean=%.1f cpuMean=%.1f%s\n", rt.fbp, w, h, x0, y0,
                     dRgb / n / 3.0, maxD, dA / n, gpuSum / n / 3.0, cpuSum / n / 3.0, dRgb / n / 3.0 > 2.0 ? "  <<<" : "");
        if (maxD > 60u && !verifyBefore.empty())
        {
            const uint8_t *b = verifyBefore.data() + (static_cast<size_t>(worstY) * rt.width + worstX) * 4u;
            const uint8_t *g = px.data() + (static_cast<size_t>(worstY - y0) * w + (worstX - x0)) * 4u;
            std::fprintf(stderr, "[gs:verify]   bad pixels: only GPU drew %u, only CPU drew %u, both %u; worst (%d,%d) before=%02x%02x%02x gpu=%02x%02x%02x cpu=%06x\n",
                         onlyGpu, onlyCpu, both, worstX, worstY, b[0], b[1], b[2], g[0], g[1], g[2], worstCpu & 0xFFFFFFu);
        }
    }

    // Whether (A - B) * C + D can leave 0..255 for the queued primitives: an
    // interpolation (D == B) with C <= 1 cannot; sums and C > 1 can.
    bool blendCanOverflow(const DrawKey &key)
    {
        static const bool always = std::getenv("PS2X_GS_HW_WRAP_ALL") != nullptr;
        if (always)
            return true;
        const uint32_t a = key.alpha & 3u, b = (key.alpha >> 2) & 3u, c = (key.alpha >> 4) & 3u, d = (key.alpha >> 6) & 3u;
        if (a == b)
            return false; // just D
        uint32_t factor = 0x80u;
        if (c == 2u)
            factor = static_cast<uint32_t>((key.alpha >> 32) & 0xFFu);
        else if (c == 0u)
        {
            uint32_t vertexAlpha = 0;
            for (const auto &v : pending)
                vertexAlpha = std::max<uint32_t>(vertexAlpha, v.a);
            uint32_t as = vertexAlpha;
            if (key.tme && key.texture && key.tcc)
            {
                uint32_t texAlpha = 0x80u; // render targets keep alpha <= 0x80
                if (key.alphaFix >= 0)
                    texAlpha = static_cast<uint32_t>(key.alphaFix & 0xFF);
                else if (auto it = textureMaxAlpha.find(static_cast<GLuint>(key.texture)); it != textureMaxAlpha.end())
                    texAlpha = it->second;
                as = key.tfx == 0u ? texAlpha * vertexAlpha / 128u : key.tfx == 2u ? texAlpha + vertexAlpha : texAlpha;
            }
            factor = as;
        }
        // c == 1 (destination alpha) is stored <= 0x80.
        const bool interpolation = d == b && d != 2u; // (A - D) * C + D
        if (interpolation && factor <= 0x80u)
            return false;
        if (d == 2u && b == 2u && factor <= 0x80u)
            return false; // A * C
        return true;
    }

    GLint wrapX0 = 0, wrapY0 = 0, wrapX1 = 0, wrapY1 = 0;
    void wrapArea(const DrawKey &key, const RenderTarget &rt)
    {
        float x0 = 1e9f, y0 = 1e9f, x1 = -1e9f, y1 = -1e9f;
        for (const auto &v : pending)
        {
            x0 = std::min(x0, v.x); y0 = std::min(y0, v.y); x1 = std::max(x1, v.x); y1 = std::max(y1, v.y);
        }
        wrapX0 = std::max<GLint>(static_cast<GLint>(key.sx0), static_cast<GLint>(std::floor(x0)) - 1);
        wrapY0 = std::max<GLint>(static_cast<GLint>(key.sy0), static_cast<GLint>(std::floor(y0)) - 1);
        wrapX1 = std::min<GLint>(std::min<GLint>(static_cast<GLint>(key.sx1) + 1, static_cast<GLint>(rt.width)), static_cast<GLint>(std::ceil(x1)) + 1);
        wrapY1 = std::min<GLint>(std::min<GLint>(static_cast<GLint>(key.sy1) + 1, static_cast<GLint>(rt.height)), static_cast<GLint>(std::ceil(y1)) + 1);
    }

    // Copies the scissored area of rt into a float target (sharing rt's depth
    // buffer) and returns its framebuffer for an additive draw.
    GLuint beginWrapAdd(RenderTarget &rt, DepthTarget *d, const DrawKey &key)
    {
        if (!wrapProgram)
            return 0;
        if (wrapTarget.texture == 0 || wrapTarget.width < rt.width || wrapTarget.height < rt.height)
        {
            if (wrapTarget.texture)
                glDeleteTextures(1, &wrapTarget.texture);
            for (auto &[k, fbo] : wrapFramebuffers)
                glDeleteFramebuffers(1, &fbo);
            wrapFramebuffers.clear();
            wrapTarget.width = std::max(wrapTarget.width, rt.width);
            wrapTarget.height = std::max(wrapTarget.height, rt.height);
            glGenTextures(1, &wrapTarget.texture);
            glBindTexture(GL_TEXTURE_2D, wrapTarget.texture);
#if !defined(PS2X_GS_HW_VITA)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, wrapTarget.width, wrapTarget.height, 0, GL_RGBA, GL_FLOAT, nullptr);
#endif
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        if (d && (d->width < rt.width || d->height < rt.height))
            return 0;
        const uint64_t fkey = d ? d->zbp + 1u : 0u;
        GLuint &fbo = wrapFramebuffers[fkey];
        if (!fbo)
        {
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, wrapTarget.texture, 0);
            if (d)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, d->texture, 0);
        }
        // The queued primitives' area of rt as the starting values.
        wrapArea(key, rt);
        const GLint x0 = wrapX0, y0 = wrapY0, x1 = wrapX1, y1 = wrapY1;
        if (x1 <= x0 || y1 <= y0)
            return 0;
        blitRect(framebuffer(rt, nullptr), fbo, rt.height, wrapTarget.height, x0, y0, x0, y0, x1 - x0, y1 - y0);
        ++stats.wrapDraws;
        return fbo;
    }

    // The sums, modulo 256, back into rt's scissored area.
    void endWrapAdd(RenderTarget &rt, const DrawKey &key, bool writeR, bool writeG, bool writeB, bool writeA)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer(rt, nullptr));
        glViewport(0, 0, rt.width, rt.height);
        setScissor(&rt, true, wrapX0, wrapY0, wrapX1 - wrapX0, wrapY1 - wrapY0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glColorMask(writeR, writeG, writeB, writeA);
        glUseProgram(wrapProgram);
        glUniform1i(glGetUniformLocation(wrapProgram, "uSums"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wrapTarget.texture);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(program);
    }

    // [x0, x1) x [y0, y1) of rt into its copy (texture unit 1) for a blend
    // done in the shader; rt's framebuffer, viewport and scissor are restored.
    void copyTargetArea(RenderTarget &rt, DepthTarget *d, const DrawKey &key, GLint x0, GLint y0, GLint x1, GLint y1)
    {
        DstCopy &copy = dstCopies[(static_cast<uint64_t>(rt.width) << 32) | rt.height];
        if (!copy.texture)
        {
            glGenTextures(1, &copy.texture);
            glBindTexture(GL_TEXTURE_2D, copy.texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rt.width, rt.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenFramebuffers(1, &copy.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, copy.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, copy.texture, 0);
        }
        if (x1 > x0 && y1 > y0)
            blitRect(framebuffer(rt, nullptr), copy.fbo, rt.height, rt.height, x0, y0, x0, y0, x1 - x0, y1 - y0);
        ++stats.exactBlendCopies;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer(rt, d));
        setScissor(&rt, true, key.sx0, key.sy0, key.sx1 >= key.sx0 ? key.sx1 - key.sx0 + 1 : 0, key.sy1 >= key.sy0 ? key.sy1 - key.sy0 + 1 : 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, copy.texture);
        glActiveTexture(GL_TEXTURE0);
    }

    void flushPending()
    {
        if (!hasPending || (pending.empty() && vuPending.empty()))
        {
            pending.clear();
            clearVuPending();
            hasPending = false;
            pendingVu = false;
            pendingNonSprite = false;
            return;
        }
        const DrawKey &key = pendingKey;
        RenderTarget &rt = targets[makeTargetKey(key.fbp, key.fbw, key.fpsm)];
        const uint32_t ztst = (key.test >> 17) & 3u;
        const bool zte = ((key.test >> 16) & 1u) != 0u;
        const bool useDepth = zte && ztst != 1u ? true : (zte && !key.zmask);
        DepthTarget *d = useDepth ? &depth(key.zbp, rt.width, rt.height) : nullptr;
        // COLCLAMP off and a blend that can leave 0..255: the GS wraps.
        // (Not for VU batches yet: the wrap area comes from screen-space vertices.)
        const bool wrapAdd = !pendingVu && !key.colclamp && key.abe && (rt.psm == GS_PSM_CT32 || rt.psm == GS_PSM_CT24) && blendCanOverflow(key);
        GLuint wrapFbo = 0;
        if (wrapAdd)
            wrapFbo = beginWrapAdd(rt, d, key);
        glBindFramebuffer(GL_FRAMEBUFFER, wrapFbo ? wrapFbo : framebuffer(rt, d));
        glViewport(0, 0, rt.width, rt.height);
        setScissor(&rt, true, key.sx0, key.sy0, key.sx1 >= key.sx0 ? key.sx1 - key.sx0 + 1 : 0, key.sy1 >= key.sy0 ? key.sy1 - key.sy0 + 1 : 0);
#if defined(PS2X_GS_HW_VITA)
        glDepthRangef(-1.0f, 1.0f); // window z = the vertex's z (0..1)
#endif
        if (d)
        {
            glEnable(GL_DEPTH_TEST);
            static const GLenum funcs[4] = {GL_NEVER, GL_ALWAYS, GL_GEQUAL, GL_GREATER};
            glDepthFunc(funcs[ztst]);
        }
        else
            glDisable(GL_DEPTH_TEST);
        const uint32_t m = key.fbmsk;
        const bool writeR = (m & 0xFFu) != 0xFFu, writeG = (m & 0xFF00u) != 0xFF00u, writeB = (m & 0xFF0000u) != 0xFF0000u;
        const bool writeA = key.fpsm != GS_PSM_CT24 && (m & 0xFF000000u) != 0xFF000000u;
        const bool writeZ = d && !key.zmask;
        // Blended sprites into small targets (post-processing buffers, which
        // games feed back frame after frame: SotC's bloom) blend in the
        // shader like the GS, on a copy of the target; elsewhere GL's
        // rounding is half a unit off once, not worth a copy per draw.
        // PS2X_GS_HW_EXACT_BLEND=0: GL's blender everywhere.
#if defined(PS2X_GS_HW_VITA)
        const bool exactBlend = false, biasBlend = false;
#else
        static const bool exactBlendOn = !std::getenv("PS2X_GS_HW_EXACT_BLEND") || std::atoi(std::getenv("PS2X_GS_HW_EXACT_BLEND")) != 0;
        // (Cs - Cd) * As + Cd and Cs * As + Cd (most blends) only need the
        // source biased (mode 2, any draw); the other blends of small targets
        // read a copy of the target (mode 1).
        const uint32_t blendA = key.alpha & 3u, blendB = (key.alpha >> 2) & 3u, blendC = (key.alpha >> 4) & 3u, blendD = (key.alpha >> 6) & 3u;
        const bool biasBlend = exactBlendOn && !wrapFbo && key.abe && blendA == 0u && blendC == 0u && blendD == 1u && (blendB == 1u || blendB == 2u) &&
                               (rt.psm == GS_PSM_CT32 || rt.psm == GS_PSM_CT24);
        const bool exactBlend = !biasBlend && exactBlendOn && !pendingVu && !wrapFbo && key.abe && !pendingNonSprite && (rt.psm == GS_PSM_CT32 || rt.psm == GS_PSM_CT24) &&
                                rt.width <= 256u && rt.height <= 256u;
#endif
        int32_t blendFix = applyBlend(key);
        if (exactBlend)
        {
            glDisable(GL_BLEND);
            blendFix = -1;
        }
        activeProgram = pendingVu ? vuProgram : program;
        glUseProgram(activeProgram);
        setInt("uExactBlend", exactBlend ? 1 : biasBlend ? 2 : 0);
        if (exactBlend)
        {
            setInt("uDst", 1);
            glUniform4i(uniform("uBlendSel"), static_cast<GLint>(key.alpha & 3u), static_cast<GLint>((key.alpha >> 2) & 3u),
                        static_cast<GLint>((key.alpha >> 4) & 3u), static_cast<GLint>((key.alpha >> 6) & 3u));
            glUniform1f(uniform("uBlendFixC"), static_cast<float>((key.alpha >> 32) & 0xFFu));
        }
        glUniform2f(uniform("uTarget"), static_cast<float>(rt.width), static_cast<float>(rt.height));
        setInt("uTme", key.tme && key.texture ? 1 : 0);
        setInt("uTfx", static_cast<GLint>(key.tfx));
        setInt("uTcc", static_cast<GLint>(key.tcc));
        setInt("uFst", static_cast<GLint>(key.fst));
        setInt("uFge", static_cast<GLint>(key.fge));
        const bool ate = (key.test & 1u) != 0u;
        const uint32_t atst = ate ? static_cast<uint32_t>((key.test >> 1) & 7u) : 1u;
        const uint32_t afail = static_cast<uint32_t>((key.test >> 12) & 3u);
        setInt("uAtest", static_cast<GLint>(atst));
        setInt("uAref", static_cast<GLint>((key.test >> 4) & 0xFFu));
        glUniform2f(uniform("uTexSize"), static_cast<float>(key.texW), static_cast<float>(key.texH));
        glUniform1f(uniform("uTexAlphaScale"), key.texAlphaScale);
        glUniform1f(uniform("uAlphaFix"), key.tme ? static_cast<float>(key.alphaFix) : -1.0f);
        glUniform2f(uniform("uGsSize"), static_cast<float>(std::max<uint32_t>(key.gsW, 1u)), static_cast<float>(std::max<uint32_t>(key.gsH, 1u)));
        const uint32_t wms = key.clamp & 3u, wmt = (key.clamp >> 2) & 3u;
#if defined(PS2X_GS_HW_VITA)
        glUniform2f(uniform("uWrap"), static_cast<float>(wms), static_cast<float>(wmt));
        glUniform4f(uniform("uRegion"), static_cast<float>((key.clamp >> 4) & 0x3FFu), static_cast<float>((key.clamp >> 24) & 0x3FFu),
                    static_cast<float>((key.clamp >> 14) & 0x3FFu), static_cast<float>((key.clamp >> 34) & 0x3FFu));
        glUniform1f(uniform("uFlipY"), drawFlipY);
        glUniform1f(uniform("uBlendFix"), static_cast<float>(blendFix));
        // A texture that is the whole GS texture with plain repeat/clamp: the
        // texture unit filters and wraps (one fetch instead of four).
        const bool hwSample = key.texW == key.gsW && key.texH == key.gsH && wms < 2u && wmt < 2u;
        setInt("uHwSample", hwSample ? 1 : 0);
#else
        const bool hwSample = true;
        glUniform2i(uniform("uWrap"), static_cast<GLint>(wms), static_cast<GLint>(wmt));
        glUniform4i(uniform("uRegion"), static_cast<GLint>((key.clamp >> 4) & 0x3FFu), static_cast<GLint>((key.clamp >> 24) & 0x3FFu),
                    static_cast<GLint>((key.clamp >> 14) & 0x3FFu), static_cast<GLint>((key.clamp >> 34) & 0x3FFu));
#endif
        setInt("uLinear", key.linear ? 1 : 0);
        glUniform3f(uniform("uFogColor"), static_cast<float>(key.fogColor & 0xFFu), static_cast<float>((key.fogColor >> 8) & 0xFFu),
                    static_cast<float>((key.fogColor >> 16) & 0xFFu));
        if (key.tme && key.texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(key.texture));
            // texelFetch ignores the filter; a point-sampled tex2D needs NEAREST.
            const GLint filter = key.linear && hwSample ? GL_LINEAR : GL_NEAREST;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wms == 0 ? GL_REPEAT : GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wmt == 0 ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        }
        GLenum drawMode = GL_TRIANGLES;
        GLsizei vertexCount = static_cast<GLsizei>(pending.size());
        if (pendingVu)
        {
            const GLsizei batches = static_cast<GLsizei>(vuConstants.size() / 4u);
#if defined(PS2X_GS_HW_VITA)
            glUniform4fv(uniform("uM"), batches * 4, vuMatrices.data()); // float4 uM[128]: the columns
#else
            glUniformMatrix4fv(uniform("uM"), batches, GL_FALSE, vuMatrices.data());
#endif
            glUniform4fv(uniform("uBatch"), batches, vuConstants.data());
#if !defined(PS2X_GS_HW_VITA)
            glUniform4fv(uniform("uNear"), batches, vuNear.data());
#endif
            const float depthScale = (key.zpsm == GS_PSM_Z16 || key.zpsm == GS_PSM_Z16S) ? 65536.0f
                                     : key.zpsm == GS_PSM_Z24 ? 16777216.0f : 4294967296.0f;
            glUniform1f(uniform("uDepthScale"), 1.0f / depthScale);
            glBindVertexArray(vuVao);
            glBindBuffer(GL_ARRAY_BUFFER, vuVbo);
            glBufferData(GL_ARRAY_BUFFER, vuPending.size() * sizeof(VuGpuVertex), vuPending.data(), GL_STREAM_DRAW);
            vertexCount = static_cast<GLsizei>(vuPending.size());
        }
        else
        {
            bindVertices();
            glBufferData(GL_ARRAY_BUFFER, pending.size() * sizeof(HwVertex), pending.data(), GL_STREAM_DRAW);
        }
        if (tracing() && pendingVu)
            std::fprintf(stderr, "[gs:trace] vu fbp=%u batches=%zu verts=%zu tex=%llx fge=%u abe=%u alpha=%llx scissor=(%u,%u)-(%u,%u)\n", key.fbp,
                         vuConstants.size() / 4u, vuPending.size(), (unsigned long long)key.tex0, key.fge, key.abe, (unsigned long long)key.alpha,
                         key.sx0, key.sy0, key.sx1, key.sy1);
        if (tracing() && !pendingVu)
        {
            ++tracedDraws;
            float x0 = 1e9f, y0 = 1e9f, x1 = -1e9f, y1 = -1e9f;
            for (const auto &v : pending)
            {
                x0 = std::min(x0, v.x); y0 = std::min(y0, v.y); x1 = std::max(x1, v.x); y1 = std::max(y1, v.y);
            }
            std::fprintf(stderr, "[gs:trace] fbp=%u fbw=%u fpsm=%u fbmsk=%08x zbp=%u zmsk=%u test=%llx abe=%u alpha=%llx tme=%u tex=%llx tfx=%u tcc=%u fst=%u fge=%u gs=%ux%u bound=%ux%u afix=%d "
                         "verts=%zu bbox=(%.0f,%.0f)-(%.0f,%.0f) scissor=(%u,%u)-(%u,%u) c0=%02x%02x%02x%02x z0=%.9g zpsm=%u\n",
                         key.fbp, key.fbw, key.fpsm, key.fbmsk, key.zbp, key.zmask, (unsigned long long)key.test, key.abe, (unsigned long long)key.alpha,
                         key.tme, (unsigned long long)key.tex0, key.tfx, key.tcc, key.fst, key.fge, key.gsW, key.gsH, key.texW, key.texH, key.alphaFix,
                         pending.size(), x0, y0, x1, y1, key.sx0, key.sy0, key.sx1, key.sy1, pending[0].r, pending[0].g, pending[0].b, pending[0].a, pending[0].z, key.zpsm);
            if (pending.size() <= 12)
                for (const auto &v : pending)
                    std::fprintf(stderr, "[gs:trace]   v (%.1f,%.1f) z=%.9g st=(%g,%g,%g) rgba=%02x%02x%02x%02x fog=%g\n", v.x, v.y, v.z, v.s, v.t, v.q, v.r, v.g, v.b, v.a, v.fog);
        }
        if (verify && tracing() && !pendingVu)
        {
            verifyBefore.resize(static_cast<size_t>(rt.width) * rt.height * 4u);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer(rt, nullptr));
            glReadPixels(0, 0, rt.width, rt.height, GL_RGBA, GL_UNSIGNED_BYTE, verifyBefore.data());
            glBindFramebuffer(GL_FRAMEBUFFER, wrapFbo ? wrapFbo : framebuffer(rt, d));
        }
        // Pass 1: pixels that pass the alpha test write everything enabled.
        // Pass 2 (AFAIL != KEEP): failing pixels write FB only, Z only, or RGB only.
        const GLsizei count = vertexCount;
        const bool exactAlpha = hostmenu::settings().exactAlpha.load(std::memory_order_relaxed);
        const bool failPass = atst != 1u && afail != 0u && (exactAlpha || atst == 0u);
        const bool failColor = afail != 2u, failZ = afail == 2u && writeZ;
        auto drawPass = [&](bool failing, GLint first, GLsizei n)
        {
            setInt("uAtestFailed", failing ? 1 : 0);
            const bool r = failing ? failColor && writeR : writeR, g = failing ? failColor && writeG : writeG, b = failing ? failColor && writeB : writeB;
            const bool a = failing ? afail == 1u && writeA : writeA;
            const bool z = failing ? failZ : writeZ;
            if (blendFix >= 0 && a)
            {
                // The blend reads FIX from the shader's alpha: the alpha the
                // pixels keep goes in a pass of its own (no Z write, so the
                // color pass's depth test sees the same buffer).
                glDisable(GL_BLEND);
                glUniform1f(uniform("uBlendFix"), -1.0f);
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
                if (d)
                    glDepthMask(GL_FALSE);
                glDrawArrays(drawMode, first, n);
                glEnable(GL_BLEND);
                glUniform1f(uniform("uBlendFix"), static_cast<float>(blendFix));
                glColorMask(r, g, b, GL_FALSE);
            }
            else
                glColorMask(r, g, b, a);
            if (d)
                glDepthMask(z ? GL_TRUE : GL_FALSE);
            glDrawArrays(drawMode, first, n);
            ++drawCalls;
        };
        // PS2X_GS_HW_AFAIL_ORDERED=1 (diagnostics): both passes triangle by
        // triangle, in the GS's order.
        static const bool ordered = std::getenv("PS2X_GS_HW_AFAIL_ORDERED") != nullptr;
        if (ordered && !pendingVu && atst != 0u && failPass && (failColor || failZ))
        {
            for (GLint t = 0; t < count; t += 3)
            {
                drawPass(false, t, 3);
                drawPass(true, t, 3);
            }
        }
        else
        {
            auto drawRange = [&](GLint first, GLsizei n)
            {
                // Pass 1: pixels that pass the alpha test write everything enabled.
                if (atst != 0u)
                    drawPass(false, first, n);
                // Pass 2 (AFAIL != KEEP): failing pixels write FB only, Z only or RGB
                // only. Settings menu "Exact transparency" off: pixels that fail a
                // real comparison are dropped (ATST=NEVER draws still need it).
                if (failPass && (failColor || failZ))
                    drawPass(true, first, n);
            };
            if (exactBlend)
            {
                // Sprites read the target as it was before them: a run of
                // sprites that do not overlap shares one copy of its area.
                GLint x0 = 0, y0 = 0, x1 = 0, y1 = 0, runFirst = 0;
                for (GLint first = 0; first < count; first += 6)
                {
                    float bx0 = 1e30f, by0 = 1e30f, bx1 = -1e30f, by1 = -1e30f;
                    for (GLint i = first; i < first + 6 && i < count; ++i)
                    {
                        bx0 = std::min(bx0, pending[i].x); bx1 = std::max(bx1, pending[i].x);
                        by0 = std::min(by0, pending[i].y); by1 = std::max(by1, pending[i].y);
                    }
                    const GLint sx0 = std::max<GLint>(static_cast<GLint>(std::floor(bx0)) - 1, static_cast<GLint>(key.sx0));
                    const GLint sy0 = std::max<GLint>(static_cast<GLint>(std::floor(by0)) - 1, static_cast<GLint>(key.sy0));
                    const GLint sx1 = std::min<GLint>(static_cast<GLint>(std::ceil(bx1)) + 1, std::min<GLint>(static_cast<GLint>(key.sx1) + 1, static_cast<GLint>(rt.width)));
                    const GLint sy1 = std::min<GLint>(static_cast<GLint>(std::ceil(by1)) + 1, std::min<GLint>(static_cast<GLint>(key.sy1) + 1, static_cast<GLint>(rt.height)));
                    if (first > runFirst && sx0 < x1 && x0 < sx1 && sy0 < y1 && y0 < sy1)
                    {
                        copyTargetArea(rt, d, key, x0, y0, x1, y1);
                        drawRange(runFirst, first - runFirst);
                        runFirst = first;
                        x0 = sx0; y0 = sy0; x1 = sx1; y1 = sy1;
                    }
                    else if (first == runFirst)
                    {
                        x0 = sx0; y0 = sy0; x1 = sx1; y1 = sy1;
                    }
                    else
                    {
                        x0 = std::min(x0, sx0); y0 = std::min(y0, sy0); x1 = std::max(x1, sx1); y1 = std::max(y1, sy1);
                    }
                }
                copyTargetArea(rt, d, key, x0, y0, x1, y1);
                drawRange(runFirst, count - runFirst);
            }
            else
                drawRange(0, count);
        }
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        if (wrapFbo)
            endWrapAdd(rt, key, writeR, writeG, writeB, writeA);
        ++rt.version;
        if (verify && tracing() && !pendingVu)
            compareWithCpu(rt);
        // PS2X_GS_HW_STEPDUMP=<dir> (with PS2X_GS_HW_TRACE): in the traced
        // frame, the display target after every draw covering much of it.
        static const char *stepDir = std::getenv("PS2X_GS_HW_STEPDUMP");
        if (stepDir && tracing() && rt.fbp == 224u && !pendingVu)
        {
            float bx0 = 1e9f, bx1 = -1e9f, by0 = 1e9f, by1 = -1e9f;
            for (const auto &v : pending)
            {
                bx0 = std::min(bx0, v.x); bx1 = std::max(bx1, v.x); by0 = std::min(by0, v.y); by1 = std::max(by1, v.y);
            }
            if ((bx1 - bx0) * (by1 - by0) > 0.3f * rt.width * 448.0f)
            {
                static int step = 0;
                std::vector<uint8_t> px(static_cast<size_t>(rt.width) * 448u * 4u);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer(rt, nullptr));
                glReadPixels(0, 0, rt.width, 448, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
                char path[512];
                std::snprintf(path, sizeof(path), "%s/step_%03d_tex%x_a%llx.ppm", stepDir, step++, static_cast<unsigned>(key.tex0 & 0x3FFFu),
                              static_cast<unsigned long long>(key.alpha));
                if (FILE *f = std::fopen(path, "wb"))
                {
                    std::fprintf(f, "P6\n%u %u\n255\n", rt.width, 448u);
                    for (size_t i = 0; i < px.size(); i += 4)
                        std::fwrite(&px[i], 1, 3, f);
                    std::fclose(f);
                }
            }
        }
        pending.clear();
        clearVuPending();
        hasPending = false;
        pendingVu = false;
        pendingNonSprite = false;
        activeProgram = program;
    }

    void clearVuPending()
    {
        vuPending.clear();
        vuMatrices.clear();
        vuConstants.clear();
        vuNear.clear();
    }

    // The GPU state a draw with this GS state needs (textures: textureKey).
    DrawKey baseKey(const GSDrawState &state, const RenderTarget &rt)
    {
        const GSContext &ctx = state.context;
        DrawKey key{};
        key.fbp = ctx.frame.fbp;
        key.fbw = ctx.frame.fbw;
        key.fpsm = ctx.frame.psm;
        key.fbmsk = ctx.frame.fbmsk;
        key.zbp = ctx.zbuf.zbp;
        key.zpsm = ctx.zbuf.psm;
        key.zmask = ctx.zbuf.zmask;
        key.test = ctx.test;
        key.alpha = ctx.alpha;
        key.clamp = ctx.clamp;
        key.tme = state.prim.tme;
        key.fst = state.prim.fst;
        key.abe = state.prim.abe;
        key.fge = state.prim.fge;
        key.tfx = ctx.tex0.tfx;
        key.tcc = ctx.tex0.tcc;
        key.sx0 = ctx.scissor.x0;
        key.sy0 = ctx.scissor.y0;
        key.sx1 = std::min<uint32_t>(ctx.scissor.x1, rt.width - 1u);
        key.sy1 = std::min<uint32_t>(ctx.scissor.y1, rt.height - 1u);
        key.fogColor = state.fogR | (state.fogG << 8) | (state.fogB << 16);
        key.linear = state.linearFilter;
        key.colclamp = static_cast<uint32_t>(state.colclamp & 1u);
        key.texAlphaScale = 1.0f;
        return key;
    }

    void textureKey(const GSDrawState &state, DrawKey &key)
    {
        const GSContext &ctx = state.context;
        float alphaScale = 1.0f;
        uint32_t tw = 0, th = 0;
        int32_t alphaFix = -1;
        key.texture = textureFor(state, alphaScale, tw, th, alphaFix);
        key.alphaFix = alphaFix;
        key.tex0 = (static_cast<uint64_t>(ctx.tex0.tbp0)) | (static_cast<uint64_t>(ctx.tex0.tbw) << 14) | (static_cast<uint64_t>(ctx.tex0.psm) << 20) |
                   (static_cast<uint64_t>(ctx.tex0.cbp) << 37);
        key.gsW = state.textureWidth;
        key.gsH = state.textureHeight;
        key.texAlphaScale = alphaScale;
        key.texW = tw;
        key.texH = th;
    }

    // A VU1 batch (GSVuBatch), drawn with the VU vertex stage; it joins the
    // queued batches when the draw state is the same. The GPU decides where
    // it lands: its pages are those of the scissor area.
    void submitVu(const GSVuBatch &batch)
    {
        if (!ensureGl() || !vuProgram)
            return;
        const GSDrawState &state = batch.state;
        const GSContext &ctx = state.context;
        if (!isColorPsm(ctx.frame.psm) || batch.vertices.empty())
            return;
        const auto type = state.prim.type;
        if (type != GS_PRIM_TRIANGLE && type != GS_PRIM_TRISTRIP && type != GS_PRIM_TRIFAN)
            return;
        RenderTarget &rt = target(ctx.frame.fbp, ctx.frame.fbw, ctx.frame.psm, static_cast<uint32_t>(ctx.scissor.y1) + 1u);
        DrawKey key = baseKey(state, rt);
        if (key.tme)
        {
            flushPendingIfTextureChanges(state);
            textureKey(state, key);
        }
        if (hasPending && (!pendingVu || !(key == pendingKey) || vuConstants.size() / 4u >= kVuBatchesPerDraw))
            flushPending();
        if (key.sx1 < key.sx0 || key.sy1 < key.sy0)
            return;
        resolveAliases(rt, key.sx0, key.sy0, key.sx1 + 1u, key.sy1 + 1u);
        loadStalePages(rt, key.sx0, key.sy0, key.sx1 + 1u, key.sy1 + 1u);
        markDirty(rt, key.sx0, key.sy0, key.sx1 + 1u, key.sy1 + 1u);
        if (hasPending && !pendingVu) // a write-back above drew the queue
            flushPending();
        pendingKey = key;
        hasPending = true;
        pendingVu = true;

        const float index = static_cast<float>(vuConstants.size() / 4u);
        vuMatrices.insert(vuMatrices.end(), batch.m, batch.m + 16);
        vuConstants.insert(vuConstants.end(), {batch.fogBase, batch.fogScale, ctx.xyoffset.ofx / 16.0f, ctx.xyoffset.ofy / 16.0f});
        vuNear.insert(vuNear.end(), batch.nearPlane, batch.nearPlane + 4);
        const auto &in = batch.vertices;
        auto put = [&](const GSVuVertex &v)
        {
            vuPending.push_back({v.x, v.y, v.z, v.s / 4096.0f, v.t / 4096.0f, v.r, v.g, v.b, v.a, index});
        };
        const size_t n = in.size();
        if (type == GS_PRIM_TRIANGLE)
            for (size_t i = 0; i + 2 < n; i += 3)
                put(in[i]), put(in[i + 1]), put(in[i + 2]);
        else
            for (size_t i = 0; i + 2 < n; ++i)
                put(type == GS_PRIM_TRIFAN ? in[0] : in[i]), put(in[i + 1]), put(in[i + 2]);
        primitives += n;
    }

    void submit(const GSPrimitiveBatch &batch)
    {
        if (!ensureGl())
            return;
        const GSDrawState &state = batch.state;
        const GSContext &ctx = state.context;
        const auto type = state.prim.type;
        if (!isColorPsm(ctx.frame.psm))
            return; // drawing into Z or texture formats: not handled yet

        const float ofx = ctx.xyoffset.ofx / 16.0f, ofy = ctx.xyoffset.ofy / 16.0f;
        RenderTarget &rt = target(ctx.frame.fbp, ctx.frame.fbw, ctx.frame.psm, static_cast<uint32_t>(ctx.scissor.y1) + 1u);

        DrawKey key = baseKey(state, rt);
        if (!verify && clearInMemory(batch, rt, key, ofx, ofy))
            return;
        // PS2X_GS_HW_SKIP=tbp,tbp: drop textured draws from these TBP0s (diagnostics).
        static const std::vector<uint32_t> skipTbp = []
        {
            std::vector<uint32_t> list;
            if (const char *env = std::getenv("PS2X_GS_HW_SKIP"))
                for (const char *p = env; *p;)
                {
                    char *end = nullptr;
                    list.push_back(static_cast<uint32_t>(std::strtoul(p, &end, 16)));
                    p = (*end == ',') ? end + 1 : end;
                    if (end == p && *p) ++p;
                }
            return list;
        }();
        if (key.tme && std::find(skipTbp.begin(), skipTbp.end(), static_cast<uint32_t>(ctx.tex0.tbp0)) != skipTbp.end())
            return;
        if (key.tme)
        {
            flushPendingIfTextureChanges(state);
            textureKey(state, key);
        }
        if (hasPending && !(key == pendingKey))
            flushPending();
        if (verify && !hasPending && tracing())
        {
            (void)shadow->ReadVram(GS_PSM_CT32, 0, 1, 0, 0); // the CPU renderer's pending work lands
            std::fill(rt.synced.begin(), rt.synced.end(), kNeverLoaded);
            loadStalePages(rt, 0, 0, rt.width, rt.height);
            const uint32_t ztst = (key.test >> 17) & 3u;
            if (((key.test >> 16) & 1u) && (ztst != 1u || !key.zmask))
                loadDepthFromCpu(rt, key);
        }
        pendingKey = key;
        hasPending = true;

        const float depthScale = (ctx.zbuf.psm == GS_PSM_Z16 || ctx.zbuf.psm == GS_PSM_Z16S) ? 65536.0f
                                 : ctx.zbuf.psm == GS_PSM_Z24 ? 16777216.0f : 4294967296.0f;
        auto make = [&](const GSVertex &v)
        {
            HwVertex o{};
            o.x = v.x - ofx + 0.5f;
            o.y = v.y - ofy + 0.5f;
            o.z = static_cast<float>(std::min(1.0, v.z / depthScale));
            if (state.prim.fst)
            {
                o.s = v.u / 16.0f;
                o.t = v.v / 16.0f;
                o.q = 1.0f;
            }
            else
            {
                o.s = v.s;
                o.t = v.t;
                o.q = v.q;
            }
            o.r = v.r;
            o.g = v.g;
            o.b = v.b;
            o.a = v.a;
            o.fog = v.fog;
            return o;
        };
        auto flatten = [&](HwVertex v, const GSVertex &last)
        {
            if (!state.prim.iip)
            {
                v.r = last.r;
                v.g = last.g;
                v.b = last.b;
                v.a = last.a;
            }
            return v;
        };
        const GSVertex *vs = batch.vertices.data();
        {
            // Pages this primitive can touch: those GS memory changed come to
            // the GPU first (queued primitives only draw pages already marked
            // dirty, so the order is kept); then they are the GPU's.
            float xmin = 1e30f, xmax = -1e30f, ymin = 1e30f, ymax = -1e30f;
            for (int i = 0; i < std::max<int>(1, batch.vertexCount); ++i)
            {
                xmin = std::min(xmin, vs[i].x - ofx);
                xmax = std::max(xmax, vs[i].x - ofx);
                ymin = std::min(ymin, vs[i].y - ofy);
                ymax = std::max(ymax, vs[i].y - ofy);
            }
            const int x0 = std::max<int>(static_cast<int>(key.sx0), static_cast<int>(std::floor(xmin)) - 1);
            const int x1 = std::min<int>(static_cast<int>(key.sx1), static_cast<int>(std::ceil(xmax)) + 1);
            const int y0 = std::max<int>(static_cast<int>(key.sy0), static_cast<int>(std::floor(ymin)) - 1);
            const int y1 = std::min<int>(static_cast<int>(key.sy1), static_cast<int>(std::ceil(ymax)) + 1);
            if (x1 < x0 || y1 < y0)
                return;
            resolveAliases(rt, static_cast<uint32_t>(x0), static_cast<uint32_t>(y0), static_cast<uint32_t>(x1) + 1u, static_cast<uint32_t>(y1) + 1u);
            loadStalePages(rt, static_cast<uint32_t>(x0), static_cast<uint32_t>(y0), static_cast<uint32_t>(x1) + 1u, static_cast<uint32_t>(y1) + 1u);
            pendingKey = key; // a write-back above may have drawn the queue
            hasPending = true;
            if (!verify)
                markDirty(rt, static_cast<uint32_t>(x0), static_cast<uint32_t>(y0), static_cast<uint32_t>(x1) + 1u, static_cast<uint32_t>(y1) + 1u);
        }
        if (type != GS_PRIM_SPRITE)
            pendingNonSprite = true;
        if ((type == GS_PRIM_TRIANGLE || type == GS_PRIM_TRISTRIP || type == GS_PRIM_TRIFAN) && batch.vertexCount >= 3)
        {
            for (int i = 0; i < 3; ++i)
                pending.push_back(flatten(make(vs[i]), vs[2]));
        }
        else if (type == GS_PRIM_SPRITE && batch.vertexCount >= 2)
        {
            HwVertex a = make(vs[0]), b = make(vs[1]);
            a.r = b.r; a.g = b.g; a.b = b.b; a.a = b.a; a.z = b.z; a.fog = b.fog;
            a.q = b.q;
            HwVertex c = a, d = a;
            c.x = b.x; c.s = b.s;          // top right
            d.y = b.y; d.t = b.t;          // bottom left
            pending.insert(pending.end(), {a, c, b, a, b, d});
        }
        else if ((type == GS_PRIM_LINE || type == GS_PRIM_LINESTRIP) && batch.vertexCount >= 2)
        {
            // Lines as one-pixel quads.
            HwVertex a = make(vs[0]), b = flatten(make(vs[1]), vs[1]);
            a = flatten(a, vs[1]);
            const float dx = b.x - a.x, dy = b.y - a.y;
            const float len = std::max(1e-6f, std::sqrt(dx * dx + dy * dy));
            const float nx = -dy / len * 0.5f, ny = dx / len * 0.5f;
            HwVertex a0 = a, a1 = a, b0 = b, b1 = b;
            a0.x += nx; a0.y += ny; a1.x -= nx; a1.y -= ny;
            b0.x += nx; b0.y += ny; b1.x -= nx; b1.y -= ny;
            pending.insert(pending.end(), {a0, b0, b1, a0, b1, a1});
        }
        else if (type == GS_PRIM_POINT && batch.vertexCount >= 1)
        {
            HwVertex p = make(vs[0]);
            HwVertex a = p, b = p, c = p, d = p;
            a.x -= 0.5f; a.y -= 0.5f; b.x += 0.5f; b.y -= 0.5f; c.x += 0.5f; c.y += 0.5f; d.x -= 0.5f; d.y += 0.5f;
            pending.insert(pending.end(), {a, b, c, a, c, d});
        }
        ++primitives;
        if (pending.size() > 60000)
            flushPending();
    }

    // A solid sprite over whole pages that only writes color (a clear): done
    // in GS memory, page by page. The GPU loads those pages again only if it
    // draws or samples them; the texture uploads games put right after a
    // clear then find nothing of the GPU's to write back.
    bool clearInMemory(const GSPrimitiveBatch &batch, RenderTarget &rt, const DrawKey &key, float ofx, float ofy)
    {
        const GSDrawState &state = batch.state;
        const GSContext &ctx = state.context;
        if (state.prim.type != GS_PRIM_SPRITE || batch.vertexCount < 2 || state.prim.tme || state.prim.abe || state.prim.fge || ctx.frame.fbmsk != 0u)
            return false;
        const uint64_t test = ctx.test;
        const bool ate = (test & 1u) != 0u;
        const uint32_t atst = static_cast<uint32_t>((test >> 1) & 7u), afail = static_cast<uint32_t>((test >> 12) & 3u);
        const bool zte = ((test >> 16) & 1u) != 0u;
        const uint32_t ztst = static_cast<uint32_t>((test >> 17) & 3u);
        if ((test >> 14) & 1u) // DATE: depends on the frame buffer
            return false;
        bool writesZ = zte && !ctx.zbuf.zmask;
        if (ate && atst == 0u && afail == 1u)
            writesZ = false; // every pixel fails, frame buffer only
        else if (ate && atst != 1u)
            return false;    // a real alpha comparison
        if (writesZ || (zte && ztst != 1u))
            return false;
        const GSVertex &a = batch.vertices[0], &b = batch.vertices[1];
        const int x0 = std::max<int>(static_cast<int>(key.sx0), static_cast<int>(std::ceil(std::min(a.x, b.x) - ofx)));
        const int x1 = std::min<int>(static_cast<int>(key.sx1) + 1, static_cast<int>(std::ceil(std::max(a.x, b.x) - ofx)));
        const int y0 = std::max<int>(static_cast<int>(key.sy0), static_cast<int>(std::ceil(std::min(a.y, b.y) - ofy)));
        const int y1 = std::min<int>(static_cast<int>(key.sy1) + 1, static_cast<int>(std::ceil(std::max(a.y, b.y) - ofy)));
        if (x1 <= x0 || y1 <= y0)
            return true; // covers nothing
        const uint32_t pw = rt.pw, ph = rt.ph;
        if (x0 % pw || y0 % ph || (x1 % pw && static_cast<uint32_t>(x1) < rt.width) || static_cast<uint32_t>(y1) % ph ||
            static_cast<uint32_t>(x1) > rt.width || static_cast<uint32_t>(y1) > rt.height)
            return false; // part of a page: the GPU draws it
        uint32_t color = b.r | (b.g << 8) | (b.b << 16) | (static_cast<uint32_t>(b.a) << 24);
        if (rt.psm == GS_PSM_CT16 || rt.psm == GS_PSM_CT16S)
            color = (b.r >> 3) | ((b.g >> 3) << 5) | ((b.b >> 3) << 10) | (b.a >= 0x80 ? 0x8000u : 0u);
        flushPending();
        resolveAliases(rt, static_cast<uint32_t>(x0), static_cast<uint32_t>(y0), static_cast<uint32_t>(x1), static_cast<uint32_t>(y1), true);
        // The GPU's copy gets the same clear, so both stay in step.
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer(rt, nullptr));
        setScissor(&rt, true, x0, y0, x1 - x0, y1 - y0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, rt.psm == GS_PSM_CT24 ? GL_FALSE : GL_TRUE);
        const float alpha = (rt.psm == GS_PSM_CT16 || rt.psm == GS_PSM_CT16S) ? ((color & 0x8000u) ? 1.0f : 0.0f) : std::min(1.0f, b.a / kAlphaOne);
        glClearColor(b.r / 255.0f, b.g / 255.0f, b.b / 255.0f, alpha);
        glClear(GL_COLOR_BUFFER_BIT);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        for (uint32_t row = y0 / ph; row < static_cast<uint32_t>(y1) / ph; ++row)
            for (uint32_t col = x0 / pw; col < (static_cast<uint32_t>(x1) + pw - 1u) / pw; ++col)
            {
                const uint32_t i = row * rt.cols + col;
                // A page the GPU never loaded keeps other pixels (CT24 alpha)
                // only in GS memory: it stays to be loaded.
                const bool wasLoaded = rt.synced[i] != kNeverLoaded && !(rt.psm == GS_PSM_CT24 && !rt.dirty[i] &&
                                                                          pageEpochOf(*shadow, rt.page(col, row)) > rt.synced[i]);
                shadow->fillPage(rt.page(col, row), rt.psm, color);
                clearDirty(rt, i);
                if (wasLoaded || rt.psm != GS_PSM_CT24)
                    rt.synced[i] = pageEpochOf(*shadow, rt.page(col, row));
            }
        ++rt.version;
        if (tracing())
            std::fprintf(stderr, "[gs:trace] memory clear fbp=%u psm=%u rect=(%d,%d)-(%d,%d) color=%08x\n", rt.fbp, rt.psm, x0, y0, x1, y1, color);
        ++stats.memoryClears;
        ++primitives;
        return true;
    }

    // ---- Presentation ----------------------------------------------------------

    // The displayed area goes straight from the GPU: copied into one of a ring
    // of textures the host draws (no trip through GS memory). Falls back to
    // the CPU presentation for display setups it does not cover.
    struct PresentSlot
    {
        GLuint texture = 0, fbo = 0;
        uint32_t width = 0, height = 0;
#if !defined(PS2X_GS_HW_VITA)
        GLsync fence = nullptr;
#endif
    };
    std::array<PresentSlot, 12> presentRing{}; // more than the frames GSAsyncBackend keeps
    uint32_t presentNext = 0;
    std::array<uint64_t, 4> lastPresentKey{};

    // PS2X_SNAPSHOT_FRAMES=<n>,<n>,...: save the first picture shown from
    // game frame n on, as <PS2X_SNAPSHOT_DIR>/gameframe_<n>.ppm (same moment of a
    // PS2X_INPUT_CLOCK=frames run on any platform and speed).
    void snapshotIfDue(GLuint fbo, uint32_t width, uint32_t height)
    {
        static const std::vector<uint64_t> frames = []
        {
            std::vector<uint64_t> out;
            if (const char *env = std::getenv("PS2X_SNAPSHOT_FRAMES"))
                for (const char *p = env; *p;)
                {
                    char *end = nullptr;
                    const uint64_t n = std::strtoull(p, &end, 10);
                    if (end == p)
                        break;
                    out.push_back(n);
                    p = *end ? end + 1 : end;
                }
            std::sort(out.begin(), out.end());
            return out;
        }();
        static size_t next = 0;
        const uint64_t now = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        // A SNAP entry of the input script asks for this picture by name.
        const int request = ps2_stats::snapshotRequest.exchange(-1, std::memory_order_relaxed);
        std::string name;
        uint64_t at = now;
        if (request >= 0)
            name = "snap_" + std::to_string(request);
        else
        {
            if (next >= frames.size() || now < frames[next])
                return;
            while (next + 1 < frames.size() && now >= frames[next + 1])
                ++next; // missed ones are named by the last due
            at = frames[next++];
            name = "gameframe_" + std::to_string(at);
        }
        std::vector<uint8_t> rgba(static_cast<size_t>(width) * height * 4u);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glReadPixels(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        const char *dir = std::getenv("PS2X_SNAPSHOT_DIR");
#if defined(PS2X_GS_HW_VITA)
        if (!dir)
            dir = "ux0:data/celosia/shots";
#endif
        if (!dir)
            return;
        const std::string path = std::string(dir) + "/" + name + ".ppm";
        if (FILE *f = std::fopen(path.c_str(), "wb"))
        {
            std::fprintf(f, "P6\n%u %u\n255\n", width, height);
            std::vector<uint8_t> row(static_cast<size_t>(width) * 3u);
            for (uint32_t y = 0; y < height; ++y)
            {
                const uint32_t srcY = blitFlip ? height - 1u - y : y;
                for (uint32_t x = 0; x < width; ++x)
                    std::memcpy(&row[x * 3u], &rgba[(static_cast<size_t>(srcY) * width + x) * 4u], 3);
                std::fwrite(row.data(), 1, row.size(), f);
            }
            std::fclose(f);
            std::cerr << "[snapshot] frame=" << at << " now=" << now << " " << path << std::endl;
        }
    }

    // Returns whether it handled the request; frame stays empty when the
    // picture did not change (no new frame).
    bool presentDirect(const GSPresentationRequest &request, PresentationFrame &frame)
    {
        static const bool disabled = std::getenv("PS2X_GS_HW_DIRECT") && std::atoi(std::getenv("PS2X_GS_HW_DIRECT")) == 0;
        if (disabled || verify)
            return false;
        // One circuit (both enabled on the same buffer is what games do).
        const bool en1 = (request.pmode & 1u) != 0u, en2 = (request.pmode & 2u) != 0u;
        if (!en1 && !en2)
            return false;
        // Both on the same buffer and size; DX/DY may differ (a field offset).
        const uint64_t sizeMask = ~0x7FFFFFull; // DW, DH, MAGH, MAGV
        if (en1 && en2 && (request.dispfb1 != request.dispfb2 || (request.display1 & sizeMask) != (request.display2 & sizeMask)))
            return false;
        const uint64_t dispfb = en1 ? request.dispfb1 : request.dispfb2, display = en1 ? request.display1 : request.display2;
        const uint32_t fbp = static_cast<uint32_t>(dispfb & 0x1FFu), fbw = static_cast<uint32_t>((dispfb >> 9) & 0x3Fu);
        const uint32_t psm = static_cast<uint32_t>((dispfb >> 15) & 0x1Fu);
        const uint32_t dbx = static_cast<uint32_t>((dispfb >> 32) & 0x7FFu), dby = static_cast<uint32_t>((dispfb >> 43) & 0x7FFu);
        const uint32_t magh = static_cast<uint32_t>((display >> 23) & 0xFu) + 1u;
        const uint32_t dw = static_cast<uint32_t>((display >> 32) & 0xFFFu) + 1u, dh = static_cast<uint32_t>((display >> 44) & 0x7FFu) + 1u;
        const uint32_t width = dw / magh, height = dh;
        auto it = targets.find(makeTargetKey(fbp, fbw, psm));
        if (it == targets.end() && (psm == GS_PSM_CT24 || psm == GS_PSM_CT32))
            it = targets.find(makeTargetKey(fbp, fbw, psm == GS_PSM_CT24 ? GS_PSM_CT32 : GS_PSM_CT24));
        if (it == targets.end() || width == 0u || height == 0u)
            return false;
        RenderTarget &rt = it->second;
        const bool psmOk = rt.psm == psm || (psm == GS_PSM_CT24 && rt.psm == GS_PSM_CT32) || (psm == GS_PSM_CT32 && rt.psm == GS_PSM_CT24);
        if (rt.fbw != fbw || !psmOk || dbx + width > rt.width || dby + height > rt.height)
            return false;

        flushPending();
        resolveAliases(rt, dbx, dby, dbx + width, dby + height);
        loadStalePages(rt, dbx, dby, dbx + width, dby + height);
        const std::array<uint64_t, 4> key{static_cast<uint64_t>(fbp) | (static_cast<uint64_t>(width) << 16) | (static_cast<uint64_t>(height) << 32),
                                          rt.version, dispfb, display};
        if (request.skipIfUnchanged && key == lastPresentKey)
            return true;
        lastPresentKey = key;

        PresentSlot &slot = presentRing[presentNext++ % presentRing.size()];
#if !defined(PS2X_GS_HW_VITA)
        if (slot.fence)
        {
            glDeleteSync(slot.fence);
            slot.fence = nullptr;
        }
#endif
        if (slot.texture == 0 || slot.width != width || slot.height != height)
        {
            if (slot.texture == 0)
            {
                glGenTextures(1, &slot.texture);
                glGenFramebuffers(1, &slot.fbo);
            }
            glBindTexture(GL_TEXTURE_2D, slot.texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindFramebuffer(GL_FRAMEBUFFER, slot.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, slot.texture, 0);
            slot.width = width;
            slot.height = height;
        }
        blitRect(framebuffer(rt, nullptr), slot.fbo, rt.height, slot.height, dbx, dby, 0, 0, width, height);
        snapshotIfDue(slot.fbo, width, height);
        GSDirectFrame direct;
        direct.texture = slot.texture;
#if defined(PS2X_GS_HW_VITA)
        // One GL for both threads: the host's draw comes after this in the
        // GPU's queue. The host draws it without blending (alpha is not shown).
        direct.fence = 0;
        if (g_presentHook)
        {
            g_presentHook(slot.texture, width, height);
            scissorState.valid = false; // the host's drawing changed the GL state
        }
#else
        // The display shows color only: opaque for the host's blending.
        glBindFramebuffer(GL_FRAMEBUFFER, slot.fbo);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        slot.fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glFlush();
        direct.fence = reinterpret_cast<uint64_t>(slot.fence);
#endif
        direct.store(frame.pixels);
        frame.width = width;
        frame.height = height;
        frame.displayFbp = fbp;
        frame.sourceFbp = fbp;
        ++stats.directPresents;
        return true;
    }

    // A draw sampling a render target the queued draws write: draw those first.
    void flushPendingIfTextureChanges(const GSDrawState &state)
    {
        if (hasPending && state.context.tex0.tbp0 == pendingKey.fbp * 32u)
            flushPending();
    }
};

GSHwBackend::GSHwBackend() : m_impl(std::make_unique<Impl>()) {}
GSHwBackend::~GSHwBackend() = default;

void GSHwBackend::Initialize(uint8_t *vram, uint32_t vramSize)
{
    m_impl->vram = vram;
    m_impl->vramSize = vramSize;
    m_impl->shadow->Initialize(vram, vramSize);
}

void GSHwBackend::Reset() { m_impl->shadow->Reset(); }

void GSHwBackend::Submit(const GSPrimitiveBatch &batch)
{
    PS2X_GS_GL_LOCK();
    m_impl->submit(batch);
    if (m_impl->verify)
        m_impl->shadow->Submit(batch);
}

bool GSHwBackend::SupportsVuBatch() const
{
#if defined(PS2X_GS_HW_VITA)
    return g_vitaVuProgram != 0 && available();
#else
    return available();
#endif
}

void GSHwBackend::SubmitVuBatch(const GSVuBatch &batch)
{
    PS2X_GS_GL_LOCK();
    m_impl->submitVu(batch);
}

void GSHwBackend::LoadClut(const GSTex0Reg &tex0, const GSTexClutReg &texclut)
{
    PS2X_GS_GL_LOCK();
    // Only indexed textures with a loading CLD read a CLUT (as GSCpuBackend);
    // TEX0 writes with CBP=0 for plain textures are common.
    const bool indexed = tex0.psm == GS_PSM_T8 || tex0.psm == GS_PSM_T8H || tex0.psm == GS_PSM_T4 || tex0.psm == GS_PSM_T4HL ||
                         tex0.psm == GS_PSM_T4HH;
    const bool loads = tex0.cld >= 1u && tex0.cld <= 5u;
    if (indexed && loads && m_impl->onGlThread())
    {
        // The CLUT comes from GS memory, which the GPU may have drawn (a
        // write-back draws the queued draws first when they reach it). Queued
        // draws keep their textures: a texture decoded again with the new
        // CLUT draws them first (textureFor).
        m_impl->wbReason = "clut";
        static const bool log = std::getenv("PS2X_GS_HW_CLUTLOG") != nullptr;
        if (log)
            for (auto &[targetKey, rt] : m_impl->targets)
                if (m_impl->dirtyOverlaps(rt, tex0.cbp * 256u, tex0.cbp * 256u + 1024u))
                    std::fprintf(stderr, "[gs:clutwb] cbp=%x page=%u rt fbp=%u fbw=%u psm=%u\n", tex0.cbp, tex0.cbp / 32u, rt.fbp, rt.fbw, rt.psm);
        m_impl->writeBackOverlapping(tex0.cbp * 256u, tex0.cbp * 256u + 1024u);
        m_impl->wbReason = "other";
    }
    m_impl->shadow->LoadClut(tex0, texclut);
}

void GSHwBackend::BeginTransfer(const GSTransferCommand &command)
{
    PS2X_GS_GL_LOCK();
    if (m_impl->onGlThread())
    {
        {
            ps2_stats::TimeInto flushTime{ps2_stats::gsXferUs[0]};
            m_impl->flushPendingBeforeTransfer(command);
        }
        ps2_stats::TimeInto hwTime{ps2_stats::gsXferUs[1]};
        // Uploads and copies go through GS memory: make the GPU's results land
        // there first so partial overwrites keep the rest.
        uint64_t begin = 0, end = 0;
        const auto &b = command.bitbltbuf;
        const uint32_t rows = std::max<uint32_t>(1u, command.trxreg.rrh);
        rowPages(b.dbp * 256u, std::max<uint32_t>(1u, b.dbw) * 64u, command.trxpos.dsay, command.trxpos.dsay + rows - 1u, b.dpsm, begin, end);
        m_impl->wbReason = "transfer";
        if (command.direction == 0u)
            m_impl->prepareUpload(command); // host -> GS upload
        else
            m_impl->writeBackOverlapping(begin, end);
        if (command.direction != 0u)
        {
            rowPages(b.sbp * 256u, std::max<uint32_t>(1u, b.sbw) * 64u, command.trxpos.ssay, command.trxpos.ssay + rows - 1u, b.spsm, begin, end);
            m_impl->writeBackOverlapping(begin, end);
        }
        m_impl->wbReason = "other";
    }
    ps2_stats::TimeInto shadowTime{ps2_stats::gsXferUs[2]};
    m_impl->shadow->BeginTransfer(command);
}

void GSHwBackend::UploadImage(const uint8_t *data, uint32_t sizeBytes) { m_impl->shadow->UploadImage(data, sizeBytes); }

void GSHwBackend::Flush()
{
    PS2X_GS_GL_LOCK();
    if (m_impl->onGlThread())
    {
        m_impl->flushPending();
#if !defined(PS2X_GS_HW_VITA)
        glFlush(); // for the host's context (vitaGL: one GL, and a flush ends the scene)
#endif
    }
}

void GSHwBackend::TextureFlush()
{
    // TEXFLUSH changes no GS memory: transfers (BeginTransfer), textures
    // decoded again (textureFor) and render targets sampled as textures
    // draw the queue when they must. Games write it before most draws: a
    // flush here split every merged VU batch.
    m_impl->shadow->TextureFlush();
}

void GSHwBackend::Sync(GSSyncReason reason) { m_impl->shadow->Sync(reason); }

PresentationFrame GSHwBackend::Present(const GSPresentationRequest &request)
{
    PS2X_GS_GL_LOCK();
    if (m_impl->onGlThread())
    {
        m_impl->flushPending();
        PresentationFrame direct;
        const bool directOk = m_impl->presentDirect(request, direct);
        // The display circuits read GS memory: bring the displayed targets there.
        for (const uint64_t dispfb : {request.dispfb1, request.dispfb2})
        {
            const uint32_t fbp = static_cast<uint32_t>(dispfb & 0x1FFu);
            m_impl->wbReason = "present";
            if (!directOk)
                for (auto &[targetKey, rt] : m_impl->targets)
                    if (rt.fbp == fbp)
                        m_impl->writeBackPages(rt);
            m_impl->wbReason = "other";
        }
        m_impl->reportStats();
        if (m_impl->tracing())
            std::fprintf(stderr, "[gs:trace] present pmode=%llx dispfb1=%llx display1=%llx dispfb2=%llx display2=%llx\n", (unsigned long long)request.pmode, (unsigned long long)request.dispfb1, (unsigned long long)request.display1, (unsigned long long)request.dispfb2, (unsigned long long)request.display2);
        if (m_impl->tracing() && m_impl->tracedDraws > 0)
            m_impl->traceDone = true; // one whole frame with draws
        // PS2X_GS_HW_TRACE counts guest vsyncs: the same moment at any speed.
        m_impl->presents = request.vsyncTick;
        if (directOk)
            return direct;
    }
    return m_impl->shadow->Present(request);
}

bool GSHwBackend::ClearFramebuffer(const GSContext &context, uint32_t rgba) { return m_impl->shadow->ClearFramebuffer(context, rgba); }
uint32_t GSHwBackend::ConsumeLocalToHostBytes(uint8_t *dst, uint32_t maxBytes) { return m_impl->shadow->ConsumeLocalToHostBytes(dst, maxBytes); }
uint32_t GSHwBackend::ReadVram(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y) const { return m_impl->shadow->ReadVram(psm, base, bw, x, y); }
void GSHwBackend::WriteVram(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y, uint32_t value) { m_impl->shadow->WriteVram(psm, base, bw, x, y, value); }
void GSHwBackend::SnapshotVram(std::vector<uint8_t> &out) const { m_impl->shadow->SnapshotVram(out); }
GSTransferSnapshot GSHwBackend::GetTransferSnapshot() const { return m_impl->shadow->GetTransferSnapshot(); }

#else // no GL on this platform: forward everything to the CPU backend

struct GSHwBackend::Impl
{
    std::unique_ptr<GSCpuBackend> shadow = std::make_unique<GSCpuBackend>();
};
GSHwBackend::GSHwBackend() : m_impl(std::make_unique<Impl>()) {}
GSHwBackend::~GSHwBackend() = default;
void GSHwBackend::Initialize(uint8_t *vram, uint32_t vramSize) { m_impl->shadow->Initialize(vram, vramSize); }
void GSHwBackend::Reset() { m_impl->shadow->Reset(); }
void GSHwBackend::Submit(const GSPrimitiveBatch &batch) { m_impl->shadow->Submit(batch); }
bool GSHwBackend::SupportsVuBatch() const { return false; }
void GSHwBackend::SubmitVuBatch(const GSVuBatch &) {}
void GSHwBackend::LoadClut(const GSTex0Reg &tex0, const GSTexClutReg &texclut) { m_impl->shadow->LoadClut(tex0, texclut); }
void GSHwBackend::BeginTransfer(const GSTransferCommand &command) { m_impl->shadow->BeginTransfer(command); }
void GSHwBackend::UploadImage(const uint8_t *data, uint32_t sizeBytes) { m_impl->shadow->UploadImage(data, sizeBytes); }
void GSHwBackend::Flush() { m_impl->shadow->Flush(); }
void GSHwBackend::TextureFlush() { m_impl->shadow->TextureFlush(); }
void GSHwBackend::Sync(GSSyncReason reason) { m_impl->shadow->Sync(reason); }
PresentationFrame GSHwBackend::Present(const GSPresentationRequest &request) { return m_impl->shadow->Present(request); }
bool GSHwBackend::ClearFramebuffer(const GSContext &context, uint32_t rgba) { return m_impl->shadow->ClearFramebuffer(context, rgba); }
uint32_t GSHwBackend::ConsumeLocalToHostBytes(uint8_t *dst, uint32_t maxBytes) { return m_impl->shadow->ConsumeLocalToHostBytes(dst, maxBytes); }
uint32_t GSHwBackend::ReadVram(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y) const { return m_impl->shadow->ReadVram(psm, base, bw, x, y); }
void GSHwBackend::WriteVram(uint32_t psm, uint32_t base, uint32_t bw, uint32_t x, uint32_t y, uint32_t value) { m_impl->shadow->WriteVram(psm, base, bw, x, y, value); }
void GSHwBackend::SnapshotVram(std::vector<uint8_t> &out) const { m_impl->shadow->SnapshotVram(out); }
GSTransferSnapshot GSHwBackend::GetTransferSnapshot() const { return m_impl->shadow->GetTransferSnapshot(); }

#endif
