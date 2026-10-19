// Native renderer on OpenGL (see runtime/native_renderer.h and the Metal
// backend, which this follows pass for pass): GL 3.3 core with GLSL on
// macOS (SOTC_NATIVE_GL=1, to check it against Metal), vitaGL with CG on the
// Vita. Render targets are GL textures, so the picture is y-up: screen
// positions come from the clip position (no gl_FragCoord), and the output is
// flipped once when it is read back.
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
#else
#include <mutex>
#include <vitaGL.h>
#define PS2X_NATIVE_VITA 1
std::recursive_mutex &ps2xVitaGlMutex();
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
    constexpr float kNearW = 0.1f;

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
    if (textured > 4.5) // the last frame shown, at the draw's UV
    {
        oColor = vec4(texture(uTex, vUV.xy).rgb, 1.0) * vColor;
        return;
    }
    if (textured > 3.5)
    {
        vec4 c = vec4(texture(uRefraction, vUV.xy).rgb, 1.0) * vColor;
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
    if (textured > 4.5)
        return float4(tex2D(uTex, vUV.xy).rgb, 1.0) * vColor;
    if (textured > 3.5)
    {
        float4 c = float4(tex2D(uRefraction, vUV.xy).rgb, 1.0) * vColor;
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

    // No depth textures: the depth modes (8, 11, 12) are not used on the Vita.
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
    return lerp(tex2D(uSrc2, uv), tex2D(uSrc, uv), uA.xxxx);
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
#elif MODE == 9
    return float4(0.0, 0.0, 0.0, saturate(tex2D(uSrc, uv).r * uA.x));
#else
    return float4(tex2D(uSrc, uv).rgb * uB.rgb * uB.a, 1.0);
#endif
}
)";
#endif

    struct Mesh
    {
        GLuint buffer = 0;
        uint32_t count = 0;
        bool dynamic = false; // in this frame's arena, from vertex `first`
        uint32_t first = 0;
        uint64_t frame = 0;   // the arena it went into (debugging)
        uint64_t used = 0;    // the last frame that drew it (eviction)
        uint64_t bytes = 0;
    };

    struct Texture
    {
        GLuint name = 0;
        uint64_t used = 0;
        uint64_t bytes = 0;
    };

    struct Draw
    {
        uint64_t mesh;
        native_renderer::DrawState state;
        float clip[16];
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
#if defined(PS2X_NATIVE_VITA)
        GLuint quadPrograms[11] = {}; // by mode (8: depth, not on the Vita)
#endif
        GLuint quadBuffer = 0, blurBuffer = 0, vao = 0;
        // Uniform locations by (program, name): the names are string literals,
        // so their address is the key (no string built per call: that was 41%
        // of the Vita's time).
        struct UniformKey
        {
            GLuint program;
            const char *name;
            bool operator==(const UniformKey &o) const { return program == o.program && name == o.name; }
        };
        struct UniformKeyHash
        {
            size_t operator()(const UniformKey &k) const { return std::hash<const void *>()(k.name) ^ (static_cast<size_t>(k.program) * 0x9E3779B1u); }
        };
        std::unordered_map<UniformKey, GLint, UniformKeyHash> uniforms;
        Target colorPlain; // the picture's colour alone (passes that read its depth)
        Target color, background, backgroundSmall, reflection, refraction, refractionHalf, snapshot, shadowMask;
        Target previous; // the last frame shown (feedbackBlur)
        Target shownFrame[2]; // what the host shows, alternately (the GPU draws later: never the target being drawn)
        int shownIndex = 0;
        Target glowLayer, glowSmall[4]; // bossDeathEffect: layer 8 apart (on the picture's depth); 256, 128, 64, 64
        Target bloomTex[3];
        bool bloomHistory = false;
        // The GS chain (as Metal): 0 mask 256, 1 128, 2 64, 3 64 lerped, 4 64 history, 5 128, 6 256.
        Target gsBloom[7];
        bool gsBloomHistory = false;
        uint32_t width = 0, height = 0;
        std::unordered_map<uint64_t, Mesh> meshes;
        std::unordered_map<uint64_t, Texture> textures;
        std::vector<Draw> draws;
        std::vector<native_renderer::Vertex> arena; // this frame's dynamic vertices
        GLuint arenaBuffers[3] = {};                 // in turn: the GPU may still read the last two
        int arenaIndex = 0;
        uint64_t arenaFrame = 0;
        // [gl:tex] every 300 frames (the Vita's log; SOTC_GL_TEX_LOG elsewhere):
        // draws whose texture is not here (drawn white), uploads, GL errors.
        struct
        {
            uint64_t missing = 0, uploads = 0, errors = 0;
            uint32_t lastError = 0;
        } texStats;
        void evictUnused(); // meshes and textures no frame drew for a while
        std::vector<native_renderer::ScreenBlur> blurs;
        native_renderer::Fog fog;
        native_renderer::Bloom bloom;
        native_renderer::GlowComposite glow;
        native_renderer::DepthOfField dof;

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
            GLuint p = glCreateProgram();
            glAttachShader(p, compile(GL_VERTEX_SHADER, vertex));
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
            const UniformKey key{program, name};
            auto it = uniforms.find(key);
            if (it != uniforms.end())
                return it->second;
            const GLint location = glGetUniformLocation(program, name);
            uniforms.emplace(key, location);
            return location;
        }

        // The full-screen pass program for a mode (one program per mode on the Vita).
        void useQuad(int mode)
        {
#if defined(PS2X_NATIVE_VITA)
            if (mode >= 0 && mode < 11 && quadPrograms[mode])
                quadProgram = quadPrograms[mode];
#else
            (void)mode;
#endif
            glUseProgram(quadProgram);
        }

        bool init()
        {
            mainProgram = program(kMainVertex, kMainFragment, {"aPos", "aNormal", "aColor", "aUV"});
#if defined(PS2X_NATIVE_VITA)
            for (int mode : {0, 1, 2, 3, 4, 5, 6, 7, 9, 10})
            {
                const std::string source = "#define MODE " + std::to_string(mode) + "\n" + kQuadFragment;
                quadPrograms[mode] = program(mode == 3 ? kQuadVertexTurned : kQuadVertex, source.c_str(), {"aPos"});
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
            glUseProgram(mainProgram);
            glUniform1i(uniform(true, "uTex"), 0);
            glUniform1i(uniform(true, "uBackground"), 1);
            glUniform1i(uniform(true, "uReflection"), 2);
            glUniform1i(uniform(true, "uRefraction"), 3);
            glUseProgram(quadProgram);
            glUniform1i(uniform(false, "uSrc"), 0);
            glUniform1i(uniform(false, "uSrc2"), 1);
            glUniform1i(uniform(false, "uDepth"), 2);
            return true;
        }

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

        // Blending as the Metal pipelines set it (alpha: source kept).
        static void setBlend(native_renderer::Blend blend)
        {
            switch (blend)
            {
            case native_renderer::Blend::Opaque:
                glDisable(GL_BLEND);
                return;
            case native_renderer::Blend::Additive:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ZERO);
                return;
            case native_renderer::Blend::Subtract:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ZERO);
                return;
            default:
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
                return;
            }
        }

        void bindTarget(const Target &t)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, t.fbo);
            glViewport(0, 0, static_cast<GLsizei>(t.width), static_cast<GLsizei>(t.height));
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
            for (const auto &t : textures)
                textureBytes += t.second.bytes;
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
            for (const auto &m : meshes)
                meshBytes += m.second.bytes;
            for (const auto &t : textures)
                textureBytes += t.second.bytes;
            std::fprintf(stderr, "[gl:cache] frame %llu meshes %zu (%.1f MB) textures %zu (%.1f MB) arena %zu\n", (unsigned long long)arenaFrame,
                         meshes.size(), meshBytes / 1048576.0, textures.size(), textureBytes / 1048576.0, arena.size());
        }
        if (arenaFrame % kEvery != 0 || arenaFrame < kUnused)
            return;
        const uint64_t before = arenaFrame - kUnused;
        for (auto it = meshes.begin(); it != meshes.end();)
        {
            const Mesh &m = it->second;
            if ((m.dynamic ? m.frame : m.used) < before)
            {
                if (m.buffer)
                    glDeleteBuffers(1, &m.buffer);
                it = meshes.erase(it);
            }
            else
                ++it;
        }
        for (auto it = textures.begin(); it != textures.end();)
        {
            if (it->second.used < before)
            {
                glDeleteTextures(1, &it->second.name);
                it = textures.erase(it);
            }
            else
                ++it;
        }
    }

    void Renderer::render(uint32_t w, uint32_t h)
    {
        ensureTargets(w, h);
        if (!arena.empty())
        {
            // One upload for every dynamic mesh of the frame (in place of one
            // buffer allocated and filled per mesh: 100-250 a frame).
            arenaIndex = (arenaIndex + 1) % 3;
            if (!arenaBuffers[arenaIndex])
                glGenBuffers(1, &arenaBuffers[arenaIndex]);
            glBindBuffer(GL_ARRAY_BUFFER, arenaBuffers[arenaIndex]);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(arena.size() * sizeof(native_renderer::Vertex)), arena.data(), GL_STREAM_DRAW);
        }
        std::stable_sort(draws.begin(), draws.end(), [](const Draw &a, const Draw &b) { return a.state.order < b.state.order; });
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
            glUseProgram(mainProgram);
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
                glUseProgram(mainProgram);
                // quad() turned the depth test off: the rest of the pass needs
                // it (without it, everything after layer 7 covered the scene).
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glDepthMask(GL_TRUE);
                shadowAmount = 0.0f;
            };
            for (const Draw &d : draws)
            {
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
                auto mesh = meshes.find(d.mesh);
                if (mesh == meshes.end() || mesh->second.count == 0)
                    continue;
                mesh->second.used = arenaFrame;
                glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
                glViewport(0, 0, static_cast<GLsizei>(target.width), static_cast<GLsizei>(target.height));
                glUseProgram(mainProgram);
                static const bool checkArena = std::getenv("SOTC_GL_ARENA_LOG") != nullptr;
                if (checkArena && mesh->second.dynamic && mesh->second.frame != arenaFrame)
                {
                    static int n = 0;
                    if (n++ < 20)
                        std::fprintf(stderr, "[arena] mesh %016llx drawn in arena %llu, uploaded in %llu (order %u)\n", (unsigned long long)d.mesh,
                                     (unsigned long long)arenaFrame, (unsigned long long)mesh->second.frame, d.state.order);
                }
                const GLsizei stride = sizeof(native_renderer::Vertex);
                // A dynamic mesh is a range of the frame's buffer: its start goes
                // to glDrawArrays (vitaGL keeps a pointer offset in GXM's 16-bit
                // attribute offset: past 64 KB it read garbage).
                const size_t base = 0u;
                const GLint firstVertex = mesh->second.dynamic ? static_cast<GLint>(mesh->second.first) : 0;
                glBindBuffer(GL_ARRAY_BUFFER, mesh->second.dynamic ? arenaBuffers[arenaIndex] : mesh->second.buffer);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, x)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, nx)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, r)));
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(base + offsetof(native_renderer::Vertex, u)));
                glUniform4fv(uniform(true, "uClip"), 4, d.clip);
                glUniform4fv(uniform(true, "uDirections"), 4, d.state.light);
                glUniform4fv(uniform(true, "uColours"), 4, d.state.light + 16);
                if (d.state.shadowVolume)
                {
                    if (backgroundOnly)
                        continue;
                    // The count where the faces pass the depth test: front +1, back -1.
                    const float misc[4] = {0, 1, 0, 0}, none[4] = {0, 0, 0, 0};
                    glUniform4fv(uniform(true, "uMisc"), 1, misc);
                    glUniform4fv(uniform(true, "uFogB"), 1, none);
                    glUniform4fv(uniform(true, "uFogC"), 1, none);
                    glUniform4fv(uniform(true, "uFogA"), 1, none);
                    glUniform4fv(uniform(true, "uFogColour"), 1, none);
                    glUniform4fv(uniform(true, "uDebug"), 1, none);
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
                    glDrawArrays(GL_TRIANGLES, firstVertex, static_cast<GLsizei>(mesh->second.count));
#if !defined(PS2X_NATIVE_VITA)
                    glDisable(GL_DEPTH_CLAMP);
#endif
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glDisable(GL_STENCIL_TEST);
                    shadowOrder = d.state.order;
                    shadowAmount = d.state.shadow;
                    continue;
                }
                setBlend(d.state.blend);
                const bool backdrop = d.state.screen || d.state.background;
                if (backdrop)
                    glDisable(GL_DEPTH_TEST);
                else
                {
                    glEnable(GL_DEPTH_TEST);
                    if (d.state.overlay)
                    {
                        glDepthFunc(GL_LEQUAL);
                        glDepthMask(GL_FALSE);
                    }
                    else
                    {
                        glDepthFunc(GL_LEQUAL); // the GS's GEQUAL: a surface drawn again in place passes
                        glDepthMask((d.state.blend == native_renderer::Blend::Additive && !d.state.writeDepth) || d.state.noDepthWrite ? GL_FALSE : GL_TRUE);
                    }
                }
                auto tex = d.state.texture ? textures.find(d.state.texture) : textures.end();
                if (d.state.texture && tex == textures.end())
                    ++texStats.missing;
                const int textured = d.state.previousFrame ? 5 : d.state.reflectionSample ? 2 : (d.state.refractionSample ? 3 : (d.state.screenCopy ? 4 : (tex != textures.end() ? 1 : 0)));
                if (textured == 5)
                    bindTexture(0, previous.texture);
                if (textured == 3 || textured == 4)
                {
                    const native_renderer::Tuning &t = native_renderer::tuning();
                    const int source = static_cast<int>((textured == 4 ? t.hazeSource : t.waterSource) + 0.5f);
                    bindTexture(3, source == 2 ? previous.texture : (source == 1 ? snapshot.texture : refraction.texture));
                }
                if (textured == 1)
                {
                    tex->second.used = arenaFrame;
                    bindTexture(0, tex->second.name);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (d.state.clamp & 1u) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (d.state.clamp & 2u) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
                }
                const float misc[4] = {d.state.fresnel ? -1.0f : (d.state.lit ? 1.0f : 0.0f), d.state.opacity, d.state.uvOffset[0], d.state.uvOffset[1]};
                const bool vertexFog = fog.vertexFog && !d.state.screen && (!d.state.background || bgFog);
#if defined(PS2X_NATIVE_VITA)
                // No depth texture for the zFog pass: the world before its layer
                // blends to the background by its own depth here, slice for slice.
                const bool zFogged = fogOn && fog.steps > 0 && !d.state.screen && !d.state.background && d.state.order < fog.order;
#else
                const bool zFogged = false;
#endif
                const float fogA[4] = {fog.start, fog.width, static_cast<float>(fog.steps), (!backgroundOnly && fogOn && (d.state.fogged || zFogged)) ? 1.0f : 0.0f};
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
                glUniform4fv(uniform(true, "uMisc"), 1, misc);
                glUniform4fv(uniform(true, "uFogA"), 1, fogA);
                glUniform4fv(uniform(true, "uFogB"), 1, fogB);
                glUniform4fv(uniform(true, "uFogC"), 1, fogC);
                glUniform4fv(uniform(true, "uFogColour"), 1, fogColour);
                glUniform4fv(uniform(true, "uDebug"), 1, debug);
                glDrawArrays(GL_TRIANGLES, firstVertex, static_cast<GLsizei>(mesh->second.count));
            }
            resolveShadows();
            for (GLuint i = 1; i < 4; ++i)
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
        const bool bloomOn = false; // reads depth
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
        auto doBlur = [&](const native_renderer::ScreenBlur &b)
        {
            if (b.steps <= 0)
                return;
            snapshotColor();
            bindTexture(0, snapshot.texture);
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
        auto doGlow = [&]
        {
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
        struct Event
        {
            uint32_t order;
            int blur; // -1 bloom, -2 glow, -3 shadows, -4 refraction copy, -5 depth of field
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
#if !defined(PS2X_NATIVE_VITA) // vitaGL framebuffers cannot share the picture's depth
        if (glow.enabled)
            events.push_back({glow.order, -2});
#endif
        if (bloomOn)
            events.push_back({bloom.order, -1});
        else
            bloomHistory = gsBloomHistory = false;
        for (size_t i = 0; i < blurs.size(); ++i)
            events.push_back({blurs[i].order, static_cast<int>(i)});
        std::stable_sort(events.begin(), events.end(), [](const Event &x, const Event &y) { return x.order < y.order; });
        uint32_t from = 0;
        bool loaded = false;
        for (const Event &ev : events)
        {
            pass(color, false, from, ev.order, loaded);
            loaded = true;
            from = ev.order;
            if (ev.blur == -6)
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
                doBlur(blurs[static_cast<size_t>(ev.blur)]);
        }
        pass(color, false, from, ~0u, loaded);
        copy(colorPlain, previous);


        if (const auto &hook = native_renderer::detail::presentHook())
        {
            // The host shows a copy of its own: the next frame starts drawing
            // into `color` before the GPU has put this one on the screen.
            shownIndex ^= 1;
            copy(colorPlain, shownFrame[shownIndex]);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            hook(shownFrame[shownIndex].texture, w, h);
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

    class GlBackend final : public native_renderer::detail::Backend
    {
    public:
        explicit GlBackend(Renderer *renderer) : r(renderer) {}

        bool hasMesh(uint64_t id) override { return r->meshes.count(id) != 0; }
        void uploadMesh(uint64_t id, const native_renderer::Vertex *vertices, uint32_t count, bool perFrame) override
        {
            static const bool noArena = std::getenv("SOTC_GL_NOARENA") != nullptr; // debugging: one buffer per mesh
            if (perFrame && !noArena)
            {
                Mesh &mesh = r->meshes[id];
                mesh.dynamic = true;
                mesh.frame = r->arenaFrame;
                mesh.first = static_cast<uint32_t>(r->arena.size());
                mesh.count = count;
                r->arena.insert(r->arena.end(), vertices, vertices + count);
                return;
            }
            Lock lock;
            Mesh &mesh = r->meshes[id];
            mesh.used = r->arenaFrame;
            if (!mesh.buffer)
                glGenBuffers(1, &mesh.buffer);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.buffer);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(native_renderer::Vertex)), vertices, GL_STATIC_DRAW);
            mesh.count = count;
            mesh.bytes = static_cast<uint64_t>(count) * sizeof(native_renderer::Vertex);
        }
        bool hasTexture(uint64_t id) override { return r->textures.count(id) != 0; }
        void uploadTexture(uint64_t key, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels) override
        {
            if (!width || !height)
                return;
            Lock lock;
            levels = std::max(1u, levels);
            Texture &t = r->textures[key];
            ++r->texStats.uploads;
            t.used = r->arenaFrame;
            t.bytes = static_cast<uint64_t>(width) * height * 4u * (levels > 1u ? 4u : 3u) / 3u;
            if (!t.name)
                glGenTextures(1, &t.name);
            glBindTexture(GL_TEXTURE_2D, t.name);
            for (uint32_t level = 0, w = width, h = height; level < levels; ++level)
            {
                glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), GL_RGBA, static_cast<GLsizei>(w), static_cast<GLsizei>(h), 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, rgba);
                if (const GLenum e = glGetError(); e != GL_NO_ERROR)
                {
                    ++r->texStats.errors;
                    r->texStats.lastError = e;
                }
                rgba += static_cast<size_t>(w) * h * 4u;
                w = std::max(1u, w / 2u);
                h = std::max(1u, h / 2u);
            }
#if !defined(PS2X_NATIVE_VITA)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(levels - 1u));
#endif
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels > 1u ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        void beginFrame() override
        {
            r->draws.clear();
            r->arena.clear();
            ++r->arenaFrame;
            r->blurs.clear();
        }
        void draw(uint64_t mesh, const float clip[16], const native_renderer::DrawState &state) override
        {
            Draw d;
            d.mesh = mesh;
            d.state = state;
            std::memcpy(d.clip, clip, sizeof(d.clip));
            r->draws.push_back(d);
        }
        void setFog(const native_renderer::Fog &fog) override { r->fog = fog; }
        void setBloom(const native_renderer::Bloom &bloom) override { r->bloom = bloom; }
        void addScreenBlur(const native_renderer::ScreenBlur &blur) override { r->blurs.push_back(blur); }
        void setGlowComposite(const native_renderer::GlowComposite &glow) override { r->glow = glow; }
        void setDepthOfField(const native_renderer::DepthOfField &dof) override { r->dof = dof; }
        void endFrame(uint32_t width, uint32_t height) override
        {
            Lock lock;
            r->render(width, height);
            r->evictUnused();
        }
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
        Renderer *r;
    };
}

namespace native_renderer::detail
{
    Backend *createGlBackend()
    {
        auto *renderer = new Renderer();
        if (!renderer->init())
        {
            delete renderer;
            return nullptr;
        }
        return new GlBackend(renderer);
    }
}

#endif
