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
#include "runtime/native_tuning.h"

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
#endif

#include "ThreadNaming.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
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

namespace
{
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
    vScreen = vec4(c.x, c.y, c.w, 0.0);
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
out vec4 oColor;
void main()
{
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
    vec4 t = textured > 0.5 ? textureLod(uTex, vUV.xy, lod) : vec4(1.0);
    float alpha = clamp(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a, 0.0, 1.0);
    if (alpha < 0.02)
        discard;
    if (uDebug.w > 0.0)
    {
        oColor = vec4(uDebug.rgb, 1.0);
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
out vec4 oColor;
void main()
{
    float textured = uFogColour.w;
    float depth = vUV.z;
    float lod = uFogC.w > 0.0 ? clamp(log2(max(depth, 1e-3)) * uFogC.z + uFogC.y, 0.0, uFogC.w) : 0.0;
    vec4 t = textured > 0.5 ? textureLod(uTex, vUV.xy, lod) : vec4(1.0);
    float alpha = clamp(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a, 0.0, 1.0);
#if CUT
    if (alpha < 0.02)
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

    // Full-screen passes: a triangle over the screen, uv 0..1 (y up).
    const char *kQuadVertex = R"(#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec4 uQuad; // blur copy: scale, angle, offset (NDC); scale 0: plain
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
    gl_Position = vec4(p, 0.0, 1.0);
    vUV = aPos * 0.5 + 0.5;
}
)";

    const char *kQuadFragment = R"(#version 330 core
in vec2 vUV;
uniform sampler2D uSrc, uSrc2, uDepth;
uniform int uMode;
uniform vec4 uA, uB, uC;
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
    else if (uMode == 16) // the GS chain's blur: the 64 x 64 source onto uA.x pixels, 12 shifted lerps (8-bit, truncated)
    {
        // Worked with rows from the top as the GS's: the lerps weigh the later
        // taps more, so the glow drifts (down and right) as the game's does.
        float r = uA.x, k = 64.0 / r;
        vec2 p = vec2(floor(gl_FragCoord.x), r - 1.0 - floor(gl_FragCoord.y));
        vec2 t0 = (p + 0.5) * k;
        vec3 acc = floor(texture(uSrc, vec2(t0.x, 64.0 - t0.y) / 64.0).rgb * 255.0 + 0.001);
        vec2 d[12] = vec2[12](vec2(-1, 0), vec2(1, 0), vec2(0, -1), vec2(0, 1), vec2(-2, 0), vec2(2, 0),
                              vec2(0, -2), vec2(0, 2), vec2(-3, 0), vec2(3, 0), vec2(0, -3), vec2(0, 3));
        for (int i = 0; i < 12; ++i)
        {
            float a = i < 4 ? 0.5 : (i < 8 ? 0.25 : 0.125);
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
          uniform float4 uMisc, uniform float4 uFogB, uniform float4 uFogC,
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
    vScreen = float4(c.x, c.y, c.w, 0.0);
}
)";

    const char *kMainFragment = R"(
float4 main(float4 vUV : TEXCOORD0, float4 vColor : TEXCOORD1, float4 vScreen : TEXCOORD2,
            uniform sampler2D uTex, uniform sampler2D uBackground, uniform sampler2D uReflection, uniform sampler2D uRefraction,
            uniform float4 uFogA, uniform float4 uFogC, uniform float4 uFogColour, uniform float4 uDebug) : COLOR
{
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
    float4 t = textured > 0.5 ? tex2Dlod(uTex, float4(vUV.xy, 0.0, lod)) : float4(1.0, 1.0, 1.0, 1.0);
    float alpha = saturate(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a);
    if (alpha < 0.02)
        discard;
    if (uDebug.w > 0.0)
        return float4(uDebug.rgb, 1.0);
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
            uniform float4 uFogC, uniform float4 uFogColour) : COLOR
{
    float textured = uFogColour.w;
    float depth = vUV.z;
    float lod = uFogC.w > 0.0 ? clamp(log2(max(depth, 1e-3)) * uFogC.z + uFogC.y, 0.0, uFogC.w) : 0.0;
    float4 t = textured > 0.5 ? tex2Dlod(uTex, float4(vUV.xy, 0.0, lod)) : float4(1.0, 1.0, 1.0, 1.0);
    float alpha = saturate(t.a * (textured > 0.5 ? 2.0 : 1.0) * vColor.a);
#if CUT
    if (alpha < 0.02)
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
    return float4(rgb, alpha);
}
)";

    // The plain full-screen vertex program, and the one screenBlur's
    // scaled / turned copies (mode 3) use: no branch on the Vita.
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
    };

    struct Texture
    {
        GLuint name = 0;
        uint32_t clamp = 0; // the wrap set on it (bit 0 s, bit 1 t clamp): new textures repeat
        uint32_t filter = 2; // the min / mag filter set on it (Tuning::textureFilter; 2 trilinear, as uploaded)
        bool mipped = false; // has mip levels
        uint8_t minAlpha = 0; // the least texel alpha over its levels
        uint64_t used = 0;
        uint64_t bytes = 0;
        bool inHeap = false; // vitaGL put it in the newlib heap (its pools were full)
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
        uint32_t width = 0, height = 0;
    };

    struct Renderer
    {
        GLuint mainProgram = 0, quadProgram = 0;
        GLuint skinProgram = 0; // the main program posing a skinned mesh's bind pose first (0: not built)
#if defined(PS2X_NATIVE_VITA)
        GLuint quadPrograms[15] = {}; // by mode (8 and 11: the bloom mask's, see kQuadFragment; 12 unused)
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
        Target previous; // the last frame shown (feedbackBlur)
        // Tuning midFrameRedraw: the picture up to a mid-frame effect's layer
        // drawn again into a target of its own (one per effect), and the
        // heat haze's 256 x 256 copy apart from the water's.
        Target prefix[3], hazeSmall;
        Target shownFrame[2]; // what the host shows, alternately (the GPU draws later: never the target being drawn)
        int shownIndex = 0;
        Target glowPrefix; // Vita3K: the picture below layer 8, apart (the glow layer's depth)
        Target glowLayer, glowSmall[4]; // bossDeathEffect: layer 8 apart (on the picture's depth); 256, 128, 64, 64
        Target bloomTex[3];
        bool bloomHistory = false;
        // The GS chain (as Metal): 0 mask 256, 1 128, 2 64, 3 64 lerped, 4 64 history, 5 128, 6 256.
        Target gsBloom[7];
        bool gsBloomHistory = false;
        uint32_t width = 0, height = 0;
        // What is bound now (render() starts from nothing known: the host
        // draws with its own state between frames). Binding only on a change
        // keeps vitaGL from redoing its per-call work every draw.
        GLuint boundFbo = ~0u, boundProgram = ~0u;
        // Vita bloom: after its mask is written into the picture's alpha
        // (bloomMask), the rest of the frame leaves the alpha alone.
        bool keepAlpha = false;
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
        bool copyRun(size_t from, size_t to, uint32_t total, RunCopy &copy);
        std::vector<std::pair<uint32_t, uint32_t>> planRanges; // scratch: a run's (first, count)
        void planFrame();
        struct
        {
            uint64_t frames = 0, draws = 0, calls = 0, merged = 0, ranges = 0, indexed = 0, noDiscard = 0, bufferSwitches = 0;
            uint64_t progOff = 0, progZero = 0; // SOTC_GL_PROG_CHECK=1: draws with another GL program bound than the cache says / none
        } drawStats; // SOTC_GL_DRAW_LOG: every 300 frames
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
            uClip, uDirections, uColours, uMisc, uFogA, uFogB, uFogC, uFogColour, uDebug, kMainUniforms
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
        static constexpr int kMainSlots = 10;
        MainSlot mains[kMainSlots];
        int mainSlot = 0; // the one in use
        GLuint leanProgram(bool skinned, bool bg, bool cut) const { return mains[2 + (skinned ? 4 : 0) + (bg ? 2 : 0) + (cut ? 1 : 0)].program; }
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
            uint32_t targets = 0;
        } stats;
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
            if (mode >= 0 && mode < 15 && quadPrograms[mode])
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

        static std::string worldFragment(bool bg, bool cut)
        {
            const std::string defines = std::string("#define BG ") + (bg ? "1" : "0") + "\n#define CUT " + (cut ? "1" : "0") + "\n";
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
            for (int mode : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14})
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
                const std::string skin = skinnedVertex(kMainVertex);
                skinVertex = skin.empty() ? 0 : compile(GL_VERTEX_SHADER, skin.c_str());
                skinProgram = skin.empty() ? 0 : program(skinVertex, kMainFragment, {"aPos", "aNormal", "aColor", "aUV", "aBones", "aWeights"});
                if (!skinProgram)
                    std::cerr << "[native:gl] no skinning program: skinned meshes posed on the CPU" << std::endl;
            }
            mains[0].program = mainProgram;
            mains[1].program = skinProgram;
            for (int skinned = 0; skinned < 2; ++skinned)
                for (int bg = 0; bg < 2; ++bg)
                    for (int cut = 0; cut < 2; ++cut)
                    {
                        if (skinned && !skinProgram)
                            continue;
                        const std::string fragment = worldFragment(bg != 0, cut != 0);
                        mains[2 + skinned * 4 + bg * 2 + cut].program =
                            skinned ? program(skinVertex, fragment.c_str(), {"aPos", "aNormal", "aColor", "aUV", "aBones", "aWeights"})
                                    : program(mainVertex, fragment.c_str(), {"aPos", "aNormal", "aColor", "aUV"});
                    }
            for (MainSlot &m : mains)
            {
                const GLuint p = m.program;
                if (!p)
                    continue;
                glUseProgram(p);
                const char *names[kMainUniforms] = {"uClip", "uDirections", "uColours", "uMisc", "uFogA", "uFogB", "uFogC", "uFogColour", "uDebug"};
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
                for (int i : {1, 6, 7, 8, 9})
                    mains[i].program = 0;
            }
            glUseProgram(quadProgram);
            glUniform1i(uniform(false, "uSrc"), 0);
            glUniform1i(uniform(false, "uSrc2"), 1);
            glUniform1i(uniform(false, "uDepth"), 2);
#if defined(PS2X_NATIVE_VITA)
            probeEmulator();
            if (native_renderer::tuning().warmShaders)
                warmShaders();
#endif
            return true;
        }

#if defined(PS2X_NATIVE_VITA)
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
            for (int which = 0; which < 22; ++which)
            {
                // main, skin, the 12 quad programs, the 8 lean ones
                const GLuint p = which == 0 ? mainProgram : (which == 1 ? skinProgram : (which < 14 ? quadPrograms[which - 2] : mains[2 + which - 14].program));
                if (!p)
                    continue;
                ++programs;
                glUseProgram(p);
                const bool skinned = which == 1 || which >= 18, quadMode = which >= 2 && which < 14;
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
            glDeleteFramebuffers(1, &t.fbo);
            glDeleteTextures(1, &t.texture);
            if (t.depth)
                glDeleteTextures(1, &t.depth);
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
            glDeleteFramebuffers(1, &t.fbo);
            glDeleteTextures(1, &t.texture);
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

        static void makeTarget(Target &t, uint32_t w, uint32_t h, bool depth)
        {
            if (t.fbo)
                glDeleteFramebuffers(1, &t.fbo);
            if (t.texture)
                glDeleteTextures(1, &t.texture);
            if (t.depth)
                glDeleteTextures(1, &t.depth);
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
            (void)depth; // vitaGL gives every framebuffer its depth and stencil
#endif
        }

        void ensureTargets(uint32_t w, uint32_t h)
        {
            if (color.texture && width == w && height == h)
                return;
            makeTarget(color, w, h, true);
            makeTarget(background, w, h, true);
            makeTarget(snapshot, w, h, false);
            makeTarget(shadowMask, w, h, false);
            makeTarget(previous, w, h, false);
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
                    makeTarget(t, w, h, true);
            if (glowPrefix.texture)
                makeTarget(glowPrefix, w, h, true);
            makeTarget(refractionHalf, 128, 128, false);
            for (Target &t : bloomTex)
                makeTarget(t, std::max(w / 4u, 1u), std::max(h / 4u, 1u), false);
#if !defined(PS2X_NATIVE_VITA)
            for (int i = 0; i < 7; ++i)
            {
                const uint32_t size = (i == 0 || i == 6) ? 256u : ((i == 1 || i == 5) ? 128u : 64u);
                makeTarget(gsBloom[i], size, size, false);
            }
            gsBloomHistory = false;
#endif
            if (colorPlain.fbo)
                glDeleteFramebuffers(1, &colorPlain.fbo);
            colorPlain = Target{};
            colorPlain.texture = color.texture;
            colorPlain.width = w;
            colorPlain.height = h;
            glGenFramebuffers(1, &colorPlain.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, colorPlain.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.texture, 0);
#if !defined(PS2X_NATIVE_VITA)
            // The shadow mask pass tests the main picture's stencil.
            glBindFramebuffer(GL_FRAMEBUFFER, shadowMask.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, color.depth, 0);
            // Layer 8 drawn apart on the picture's depth.
            glBindFramebuffer(GL_FRAMEBUFFER, glowLayer.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, color.depth, 0);
#endif
            bloomHistory = false;
            width = w;
            height = h;
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
            glUniform1i(uniform(false, "uMode"), mode);
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
            const bool merge = t.glMergeDraws, lean = t.glLeanPrograms;
            std::vector<uint8_t> a(static_cast<size_t>(w) * h * 4u), b(a.size());
            t.glMergeDraws = t.glLeanPrograms = false;
            render(w, h);
            glBindFramebuffer(GL_FRAMEBUFFER, colorPlain.fbo);
            glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, a.data());
            t.glMergeDraws = merge;
            t.glLeanPrograms = lean;
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
            std::fprintf(stderr, "[gl:selfcheck] frame %llu draws %zu calls %llu: %u of %u pixels differ (most %u)\n", (unsigned long long)arenaFrame, draws.size(),
                         (unsigned long long)(drawStats.calls - calls), differ, w * h, worst);
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
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.count), GL_UNSIGNED_INT,
                           reinterpret_cast<const void *>(static_cast<uintptr_t>(mesh.dynamic ? mesh.firstIndex : 0u) * sizeof(uint32_t)));
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
            std::fprintf(stderr, "[gl:tex] frame %llu missing draws %llu uploads %llu gl errors %llu (last 0x%x) textures %zu (%.1f MB)\n",
                         (unsigned long long)arenaFrame, (unsigned long long)texStats.missing, (unsigned long long)texStats.uploads,
                         (unsigned long long)texStats.errors, texStats.lastError, textures.size(), textureBytes / 1048576.0);
            texStats.missing = texStats.uploads = texStats.errors = 0;
        }
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
        uint8_t minAlpha = 0xFF;
        for (uint32_t i = 0; i < count; ++i)
            minAlpha = std::min(minAlpha, vertices[i].a);
        mesh.minAlpha = count ? minAlpha : 0;
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
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(native_renderer::Vertex)), vertices, GL_STATIC_DRAW);
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
            if (!pools[p].buffer)
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

    void Renderer::uploadSkinned(uint64_t id, const native_renderer::SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount)
    {
        Mesh &mesh = meshes[id];
        forget(mesh);
        mesh.used = arenaFrame;
        mesh.dynamic = false;
        mesh.skinned = true;
        mesh.indexed = true;
        uint8_t minAlpha = 0xFF;
        for (uint32_t i = 0; i < count; ++i)
            minAlpha = std::min(minAlpha, vertices[i].a);
        mesh.minAlpha = count ? minAlpha : 0;
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
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(native_renderer::SkinVertex)), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexCount * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
        mesh.count = indexCount;
        mesh.bytes = static_cast<uint64_t>(count) * sizeof(native_renderer::SkinVertex) + static_cast<uint64_t>(indexCount) * 4u;
        meshBytes += mesh.bytes;
        skinnedBytes += mesh.bytes;
        cache.buffer = ~0u;
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
            uint8_t least = 0xFF;
            for (size_t i = 0; i < texels && least; ++i)
                least = std::min(least, rgba[i * 4u + 3u]);
            t.minAlpha = least;
        }
        glBindTexture(GL_TEXTURE_2D, t.name);
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
#endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels > 1u ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
    static bool sameDraw(const Draw &a, const Draw &b)
    {
        const native_renderer::DrawState &x = a.state, &y = b.state;
        if (x.texture != y.texture || x.clamp != y.clamp || x.blend != y.blend || x.order != y.order || x.opacity != y.opacity ||
            x.uvOffset[0] != y.uvOffset[0] || x.uvOffset[1] != y.uvOffset[1] || x.lodK != y.lodK || x.lodScale != y.lodScale || x.maxLod != y.maxLod ||
            x.background != y.background || x.fogged != y.fogged || x.writeDepth != y.writeDepth || x.noDepthWrite != y.noDepthWrite ||
            x.overlay != y.overlay || x.screen != y.screen || x.screenDepthTest != y.screenDepthTest || x.shadowVolume || y.shadowVolume ||
            x.reflectionSource != y.reflectionSource || x.reflectionSample != y.reflectionSample || x.refractionSample != y.refractionSample ||
            x.fresnel != y.fresnel || x.screenCopy != y.screenCopy || x.previousFrame != y.previousFrame || x.debugColour != y.debugColour ||
            x.lit != y.lit || x.skinPalette || y.skinPalette)
            return false;
        if (std::memcmp(x.reflectionTint, y.reflectionTint, sizeof(x.reflectionTint)) || std::memcmp(x.mirrorRipple, y.mirrorRipple, sizeof(x.mirrorRipple)))
            return false;
        if ((x.lit || x.fresnel) && std::memcmp(x.light, y.light, sizeof(x.light)))
            return false;
        return std::memcmp(a.clip, b.clip, sizeof(a.clip)) == 0;
    }

    void Renderer::planFrame()
    {
        const size_t n = drawOrder.size();
        plan.assign(n, Planned{});
        for (size_t k = 0; k < n; ++k)
        {
            const Draw &d = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
            Planned &p = plan[k];
            Mesh *m = meshes.find(d.mesh);
            if (m && m->count)
            {
                m->used = arenaFrame;
                p.mesh = m;
                p.minAlpha = m->dynamic ? 0 : m->minAlpha;
            }
            if (d.state.texture)
                if (Texture *t = textures.find(d.state.texture))
                {
                    t->used = arenaFrame;
                    p.tex = t;
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
                                     "no-discard %.0f, vertex buffer switches %.0f | pools %u, run copies %u (%.0f KB) | program off %.0f, none %.0f\n",
                             drawStats.draws / f, drawStats.calls / f, drawStats.merged / f, drawStats.ranges / f, drawStats.indexed / f,
                             drawStats.noDiscard / f, drawStats.bufferSwitches / f, static_cast<unsigned>(pools.size()), static_cast<unsigned>(runs.size()),
                             runVertices * sizeof(native_renderer::Vertex) / 1024.0, drawStats.progOff / f, drawStats.progZero / f);
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
    }

    // A run's kept meshes copied side by side into a pool range of its own.
    bool Renderer::copyRun(size_t from, size_t to, uint32_t total, RunCopy &copy)
    {
        if (runVertices + total > kRunBudget)
        {
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
                if (runVertices + total <= kRunBudget)
                    break;
                RunCopy *r = runs.find(key);
                poolFree(r->pool, r->first, r->count);
                runVertices -= r->count;
                r->pool = -1;
                r->frames = 0;
            }
            if (runVertices + total > kRunBudget)
                return false;
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
        return true;
    }

    void Renderer::render(uint32_t w, uint32_t h)
    {
        ensureTargets(w, h);
        boundFbo = boundProgram = ~0u;
        drawOrder.clear();
        drawOrder.reserve(draws.size());
        for (size_t i = 0; i < draws.size(); ++i)
            drawOrder.push_back((static_cast<uint64_t>(draws[i].state.order) << 32) | i);
        std::sort(drawOrder.begin(), drawOrder.end());
        planFrame();
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
                glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(arena.size() * sizeof(native_renderer::Vertex)), arena.data(), GL_STREAM_DRAW);
            }
            if (!indexArena.empty())
            {
                if (!indexBuffers[arenaIndex])
                    glGenBuffers(1, &indexBuffers[arenaIndex]);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[arenaIndex]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexArena.size() * sizeof(uint32_t)), indexArena.data(), GL_STREAM_DRAW);
            }
        }
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
        // everything (the horizon vanished with shadows on): darken in place.
        constexpr bool hardShadows = true;
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

        auto pass = [&](Target &target, bool backgroundOnly, uint32_t fromOrder = 0, uint32_t toOrder = ~0u, bool load = false, bool mirror = false,
                        bool apart = false)
        {
            bindTarget(target);
            if (apart) // a layer of its own over the picture's depth: colour cleared to nothing
            {
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            if (!load)
            {
                if (mirror) // reflectionBegin clears with alpha 0x80
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
            for (size_t k = 0; k < drawOrder.size(); ++k)
            {
                const Planned &planned = plan[k];
                if (!planned.run) // merged into an earlier draw
                    continue;
                const Draw &d = draws[static_cast<size_t>(drawOrder[k] & 0xFFFFFFFFu)];
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
                if (shadowAmount && d.state.order > shadowOrder)
                    resolveShadows();
                if (!planned.mesh)
                    continue;
                Mesh &mesh = *planned.mesh;
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
                const int textured = d.state.previousFrame ? 5 : d.state.reflectionSample ? 2 : (d.state.refractionSample ? 3 : (d.state.screenCopy ? 4 : (tex ? 1 : 0)));
                const bool vertexFog = fog.vertexFog && !d.state.screen && (!d.state.background || bgFog);
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
                if (leanOn && !d.state.shadowVolume && textured <= 1 && !d.state.debugColour)
                {
                    bool cut = true;
                    if (noDiscard && !d.state.fresnel && planned.minAlpha)
                    {
                        // alpha = texel a x 2 (0x80 = 1) x vertex a / 128 x opacity, at least:
                        float least = static_cast<float>(planned.minAlpha) / 128.0f * d.state.opacity;
                        if (textured == 1)
                            least *= static_cast<float>(tex->minAlpha) / 255.0f * 2.0f;
                        cut = !(least >= 0.03f); // the cut-out is at 0.02
                    }
                    if (const GLuint lean = leanProgram(skinned, fogA[3] > 0.5f, cut))
                    {
                        program = lean;
                        drawStats.noDiscard += cut ? 0u : 1u;
                    }
                }
                useProgram(program);
                if (skinned)
                {
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
                }
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
                    // vitaGL bakes the colour mask into the fragment program's
                    // blend info, which did not hold (the volumes were painted
                    // over the horizon): blend so the colour cannot change too.
                    glEnable(GL_BLEND);
                    glBlendEquation(GL_FUNC_ADD);
                    glBlendFunc(GL_ZERO, GL_ONE);
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
                const int depthTest = backdrop && !d.state.screenDepthTest ? 0 : 1;
                const int depthMask = d.state.screenDepthTest ? 0
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
                if (depthMask != cache.depthMask)
                {
                    glDepthMask(depthMask ? GL_TRUE : GL_FALSE);
                    cache.depthMask = depthMask;
                }
                if (d.state.texture && !tex)
                    ++texStats.missing;
                if (textured == 5)
                {
                    bindTexture(0, previous.texture);
                    cache.texture = ~0u;
                }
                if (textured == 3 || textured == 4)
                {
                    const native_renderer::Tuning &t = native_renderer::tuning();
                    const int source = static_cast<int>((textured == 4 ? t.hazeSource : t.waterSource) + 0.5f);
                    const GLuint redrawn = textured == 4 ? hazeSourceTexture : waterSource;
                    bindTexture(3, redrawn && source != 2 ? redrawn : (source == 2 ? previous.texture : (source == 1 ? snapshot.texture : refraction.texture)));
                }
                if (textured == 1)
                {
                    tex->used = arenaFrame;
                    if (cache.texture != tex->name)
                    {
                        bindTexture(0, tex->name);
                        cache.texture = tex->name;
                    }
                    if (tex->clamp != (d.state.clamp & 3u)) // a texture's wrap is its own state: set when it changes
                    {
                        tex->clamp = d.state.clamp & 3u;
                        bindTexture(0, tex->name); // unit 0 active (a refraction draw may have left unit 3)
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (d.state.clamp & 1u) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (d.state.clamp & 2u) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
                    }
                    // Quality "Texture filtering": nearest, bilinear, trilinear (anisotropic: Metal only, trilinear here).
                    const uint32_t filter = d.state.screen ? 2u : static_cast<uint32_t>(std::clamp(native_renderer::tuning().textureFilter + 0.5f, 0.0f, 2.0f)); // 2D keeps the default
                    if (tex->filter != filter)
                    {
                        tex->filter = filter;
                        bindTexture(0, tex->name);
                        const bool mipped = tex->mipped;
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        filter == 0 ? (mipped ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST)
                                                    : (filter == 1 ? (mipped ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR) : (mipped ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR)));
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter == 0 ? GL_NEAREST : GL_LINEAR);
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
                uniform4(uMisc, misc, 1);
                uniform4(uFogA, fogA, 1);
                uniform4(uFogB, fogB, 1);
                uniform4(uFogC, fogC, 1);
                uniform4(uFogColour, fogColour, 1);
                uniform4(uDebug, debug, 1);
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
                static const bool progCheck = std::getenv("SOTC_GL_PROG_CHECK") != nullptr;
                if (progCheck)
                {
                    GLint cur = 0;
                    glGetIntegerv(GL_CURRENT_PROGRAM, &cur);
                    drawStats.progOff += static_cast<GLuint>(cur) != boundProgram;
                    drawStats.progZero += cur == 0;
                }
                if (planned.run > 1)
                {
                    drawStats.merged += planned.run - 1u;
                    ++(planned.runPool >= 0 ? drawStats.indexed : drawStats.ranges);
                    glDrawArrays(GL_TRIANGLES, firstVertex, static_cast<GLsizei>(planned.mergedCount));
                }
                else
                    drawMesh(mesh, firstVertex);
            }
            resolveShadows();
            for (GLuint i = 1; i < 6; ++i)
                glDisableVertexAttribArray(i);
        };

        if (haveMirror)
            pass(reflection, false, 0, ~0u, false, true);
        if (fogOn)
        {
            pass(background, true);
            copy(background, backgroundSmall); // bgEnd: reduced to 256 x 256
        }
#if defined(PS2X_NATIVE_VITA)
        const bool bloomOn = false; // the Mac's reads depth; the Vita's is vitaBloom
        const bool vitaBloom = bloom.enabled && bloom.color[3] > 0.0f;
        const bool dofOn = false;
#else
        const bool bloomOn = bloom.enabled;
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
                const float r64[4] = {64, 0, 0, 0}, r128[4] = {128, 0, 0, 0};
                bindTexture(0, gsBloom[3].texture);
                quad(gsBloom[4], 16, QuadBlend::None, r64);
                gsBloomHistory = true;
                quad(gsBloom[5], 16, QuadBlend::None, r128);
                bindTexture(0, gsBloom[5].texture);
                quad(gsBloom[6], 0, QuadBlend::None);
                const float scale[4] = {static_cast<float>(width) / 512.0f, static_cast<float>(height) / 448.0f, 0, 0};
                const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3]};
                bindTexture(0, gsBloom[6].texture);
                quad(colorPlain, 18, QuadBlend::Add, scale, colour);
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
        // bossDeathEffect (layer 8): the layer drawn apart into a buffer cleared
        // to nothing, reduced to 256, 128, 64 and blurred, back to 256; laid
        // over the picture sharp (amountA) and blurred (amountB), in the tint.
        // The glow layer drawn on the depth of `depthOf` (the picture, or in
        // Vita3K the picture below layer 8 drawn again apart, see the
        // mid-frame redraw below), reduced and blurred.
        bool glowBuilt = false;
        auto buildGlow = [&](Target &depthOf) -> bool
        {
#if defined(PS2X_NATIVE_VITA)
            bindTarget(depthOf); // drawn into this frame: its depth exists
            if (!vglFramebufferShareDepth(glowLayer.fbo, depthOf.fbo))
                return false;
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
                return;
            }
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
        auto doShadow = [&]
        {
            // The counted pixels as a mask (testing the main picture's stencil), reduced 4 x 4, blurred.
            bindTarget(shadowMask);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
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
            snapshotColor();
            if (haze == 0 || water == 0)
            {
                copy(snapshot, refraction);
                copy(refraction, refractionHalf);
                copy(refractionHalf, refraction);
            }
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
        auto bloomReduce = [&]
        {
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
            if (bloomHistory && !bloomSameFrame)
            {
                // Strength x 0.46: this glow is the narrow one the Mac had
                // before the GS chain (its strength was set by eye then).
                const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3] * 0.46f};
                bindTexture(0, bloomTex[2].texture);
                quad(color, 10, QuadBlend::Add, nullptr, colour);
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
                glUniform4fv(uniform(false, "uQuad"), 1, q);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            glDepthFunc(GL_LEQUAL);
            if (bloomSameFrame)
            {
                bloomReduce();
                const float colour[4] = {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3] * 0.46f};
                bindTexture(0, bloomTex[2].texture);
                quad(color, 10, QuadBlend::Add, nullptr, colour);
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
#else
        if (glow.enabled)
            events.push_back({glow.order, -2});
#endif
        if (bloomOn)
            events.push_back({bloom.order, -1});
#if defined(PS2X_NATIVE_VITA)
        else if (vitaBloom)
            events.push_back({bloom.order, -8});
#endif
        else
            bloomHistory = gsBloomHistory = false;
        for (size_t i = 0; i < blurs.size(); ++i)
            events.push_back({blurs[i].order, static_cast<int>(i)});
        std::stable_sort(events.begin(), events.end(), [](const Event &x, const Event &y) { return x.order < y.order; });
        static const bool eventLog = std::getenv("SOTC_GL_EVENT_LOG") != nullptr; // debugging: each frame's effects (layer:kind)
        if (eventLog)
        {
            std::string line;
            for (const Event &ev : events)
                line += " " + std::to_string(ev.order) + ":" + std::to_string(ev.blur);
            unsigned layer8 = 0;
            for (const Draw &d : draws)
                layer8 += d.state.order == 8u ? 1u : 0u;
            std::fprintf(stderr, "[gl:events] frame %llu draws %u layer8 %u glow %d%s\n", (unsigned long long)arenaFrame, static_cast<unsigned>(draws.size()), layer8,
                         glow.enabled ? 1 : 0, line.c_str());
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
                        makeTarget(glowPrefix, w, h, true);
                    boundFbo = ~0u;
                    pass(glowPrefix, false, 0, ev.order, false);
                    glowBuilt = buildGlow(glowPrefix);
                    break;
                }
#endif
            for (Event &ev : events)
            {
                const bool water = ev.blur == -4 && haveRefraction && ev.order == 9u && waterFrom != 2;
                const bool haze = ev.blur == -4 && screenCopyOrder && ev.order == screenCopyOrder && hazeFrom != 2;
                if (!(ev.blur >= 0 || water || haze))
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
                        makeTarget(pre, w, h, true);
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
        for (const Event &ev : events)
        {
            if (ev.prefix >= 0 && ev.blur == -4)
                continue; // its copies are made
            pass(color, false, from, ev.order, loaded);
            loaded = true;
            from = ev.order;
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
            else
                doBlur(blurs[static_cast<size_t>(ev.blur)], ev.prefix >= 0 ? &prefix[ev.prefix] : nullptr);
        }
        pass(color, false, from, ~0u, loaded);
        const bool bloomAdd = keepAlpha;
        keepAlpha = false;
        if (bloomAdd)
            bloomReduce(); // round 18's order: added by the next frame's bloomMask
        copy(colorPlain, previous);


        if (const auto &hook = native_renderer::detail::presentHook())
        {
            // The host shows a copy of its own: the next frame starts drawing
            // into `color` before the GPU has put this one on the screen.
            shownIndex ^= 1;
            copy(colorPlain, shownFrame[shownIndex]);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            boundFbo = boundProgram = ~0u;
            const bool timed = native_renderer::tuning().frameTiming();
            const uint64_t start = timed ? native_renderer::nowUs() : 0u;
            hook(shownFrame[shownIndex].texture, w, h);
            if (timed)
                stats.presentUs += native_renderer::nowUs() - start;
            return;
        }
        // Read back, top row first (the GL picture is y-up).
        std::vector<uint8_t> pixels(static_cast<size_t>(w) * h * 4u);
        glBindFramebuffer(GL_FRAMEBUFFER, color.fbo);
        glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        std::lock_guard<std::mutex> lock(outputMutex);
        output.resize(pixels.size());
        const size_t row = static_cast<size_t>(w) * 4u;
        for (uint32_t y = 0; y < h; ++y)
            std::memcpy(output.data() + static_cast<size_t>(y) * row, pixels.data() + static_cast<size_t>(h - 1u - y) * row, row);
        outputWidth = w;
        outputHeight = h;
        hasOutput = true;
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
            waitIdle(lock);
            // The render thread waits: what it holds changes hands. What the
            // last frame drawn cost goes into this frame's counters.
            if (timed)
            {
                native_renderer::FrameCounters &c = native_renderer::frameCounters();
                c.renderUs += r->stats.renderUs;
                c.presentUs += r->stats.presentUs;
                c.targets += r->stats.targets;
            }
            r->stats = {};
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
            std::fprintf(stderr,
                         "[gl:mem] t=%.0f heap %.1f MB | textures %u %.1f MB (in heap %.1f) | meshes %u %.1f MB (skinned %.1f) | drawn in 120 frames %.1f MB, budget %.0f MB | "
                         "uploads peak %.1f MB a frame, flushes %u | arenas %.1f MB, draw lists %.1f MB | hooks' skinned packets %.1f MB | "
                         "evicted aged %u, budget %u (%.1f MB), pressure %u (%.1f MB) | skipped %u\n",
                         (native_renderer::nowUs() - m_startUs) / 1e6, m_heap / MB, static_cast<unsigned>(r->textures.size()), r->textureBytes / MB,
                         r->textureHeapBytes / MB, kept, r->meshBytes / MB, r->skinnedBytes / MB, recentBytes / MB, m_cacheBudget == ~0ull ? 0.0 : m_cacheBudget / MB,
                         m_uploadPeak / MB, m_flushes, arenas / MB, lists / MB, skinnedPackets / MB, m_evictedAged, m_evictedBudget,
                         m_evictedBudgetBytes / MB, m_evictedPressure, m_evictedPressureBytes / MB, m_skipped);
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
        void evictKept()
        {
            constexpr uint64_t kEvery = 256, kUnused = 1800, kBudgetAge = 120, kPressureAge = 30, kHeapEvery = 16;
            const uint64_t frame = r->arenaFrame + 1u; // the frame handed over
            if (m_heapHigh && frame % kHeapEvery == 0)
                m_heap = heapUsed();
            const bool aging = frame % kEvery == 0 && frame >= kUnused;
            const bool pressure = m_heapHigh && m_heap > m_heapHigh && frame >= m_nextPressure;
            const bool overBudget = r->textureBytes + r->meshBytes > m_cacheBudget && frame >= m_nextBudget;
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
            const uint64_t pressureBefore = pressure && frame > kPressureAge ? frame - kPressureAge : 0u;
            const uint64_t budgetBefore = overBudget && frame > kBudgetAge ? frame - kBudgetAge : 0u;
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
            const uint64_t target = m_cacheBudget - m_cacheBudget / 8u;
            for (const Candidate &c : m_candidates)
            {
                const bool aged = c.used < agedBefore, pressed = c.used < pressureBefore, budget = c.used < budgetBefore && cached > target;
                if (!aged && !pressed && !budget)
                    continue;
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
            if (overBudget && cached > m_cacheBudget)
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
#if defined(PS2X_NATIVE_VITA)
            // A core of its own: the game thread keeps to the other two (ps2_runtime.cpp).
            sceKernelChangeThreadCpuAffinityMask(sceKernelGetThreadId(), SCE_KERNEL_CPU_MASK_USER_2);
#endif
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
                    Lock gl;
                    const uint64_t start = native_renderer::nowUs(), present = r->stats.presentUs;
                    ++r->arenaFrame;
                    r->applyUploads();
                    r->expandDraws();
                    if (!r->hold) // held: the last picture stays on screen
                    {
                        // SOTC_GL_SELFCHECK=N (debugging, with bloom and the
                        // motion trail off: they carry the last frame over):
                        // every N frames the frame is drawn first with the
                        // draw merges and lean programs off, then as set, and
                        // the two pictures compared.
                        static const int check = std::getenv("SOTC_GL_SELFCHECK") ? std::atoi(std::getenv("SOTC_GL_SELFCHECK")) : 0;
                        if (check > 0 && r->arenaFrame % static_cast<uint64_t>(check) == 0u)
                            r->selfCheck(width, height);
                        else
                            r->render(width, height);
                    }
                    r->evictUnused();
                    r->stats.renderUs += (native_renderer::nowUs() - start) - (r->stats.presentUs - present);
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
            while (m_queued || m_busy)
                m_idle.wait_for(lock, kRecheck);
        }
        bool m_started = false, m_initOk = false;
        bool m_queued = false; // a frame handed over, not started yet
        bool m_busy = false;   // the render thread drawing
        bool m_uploadsOnly = false; // what is handed over is uploads alone (flushUploads, SOTC_GL_FLUSH_WAIT)
        std::vector<Renderer::Upload> m_side, m_sideApplying; // flushed uploads (flushUploads); the render thread's copy while it applies them
        uint64_t m_sideBytes = 0;
        uint32_t m_width = 0, m_height = 0;
    };
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
