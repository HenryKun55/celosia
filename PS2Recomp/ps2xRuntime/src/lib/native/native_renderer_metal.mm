// Native renderer on Metal (see runtime/native_renderer.h).
#include "native_backend.h"
#include "runtime/native_tuning.h"

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

#include <algorithm>
#include <cmath>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <unordered_map>

namespace
{
    const char *kShaders = R"MSL(
#include <metal_stdlib>
using namespace metal;

struct VIn
{
    float3 position [[attribute(0)]];
    float3 normal [[attribute(1)]];
    float2 uv [[attribute(3)]];
    uchar4 color [[attribute(2)]];
};

// The near plane: the game's geometry comes this close to the camera (a
// camera low in a pit, on the ground); the PS2 cuts much nearer than w = 1.
constant float kNearW = 0.1;

struct VOut
{
    float4 position [[position]];
    float3 normal;
    float2 uv;
    float4 color;
    float depth; // w
    float fog [[center_no_perspective]]; // GS vertex fog factor, 1: none; linear on the screen as the GS interpolates it
};

struct FogParams
{
    float start, width, steps;
    int fogged;
    float2 size;
    int vertexFog;
    float base, scale;
    float lodK, lodScale, maxLod; // GS mip selection: level = log2(w) x 2^L + K, up to MXL (-1: level 0 only)
    float3 colour;
    float4 debug; // w > 0: flat debug colour
    int clamp;    // the GS's CLAMP: bit 0 u, bit 1 v
    int hwLod;    // texture filtering anisotropic: the GPU picks the mip level (by the pixel's footprint), not the GS rule
    float2 ripple; // the mirror's ripple (t, H; H = 0: none)
    int atst;      // the GS alpha test's ATST (-1: none, only alpha < 0.02 cut)
    float aref;    // its AREF (0x80 = 1)
    int atSide;    // 0: draw the pixels that pass, 1: those that fail
    int atPad;
};

// The GS's alpha test: A = texture alpha x vertex alpha >> 7 (0x80 = 1) against AREF.
bool gsAlphaPass(float a, int atst, float aref)
{
    const float A = floor(a * 128.0 + 0.001);
    switch (atst)
    {
    case 0: return false;
    case 1: return true;
    case 2: return A < aref;
    case 3: return A <= aref;
    case 4: return A == aref;
    case 5: return A >= aref;
    case 6: return A > aref;
    default: return A != aref;
    }
}

// A model's texture: the filtering the player picked (the samplers bound at
// 0-3, one per CLAMP mode), at the GS's mip level or the GPU's own.
float4 sampleModel(texture2d<float> tex, sampler s, float2 uv, float lod, bool hw)
{
    return hw ? tex.sample(s, uv) : tex.sample(s, uv, level(lod));
}

struct Light
{
    float4x4 directions; // columns: the normal's x, y, z, 1
    float4x4 colours;    // columns: light 0, 1, 2, ambient
    int lit;
    float opacity;
    float2 uvOffset; // VIF ROW: texture scrolling
};

vertex VOut nativeVertex(VIn in [[stage_in]], constant float4x4 &clip [[buffer(1)]], constant Light &light [[buffer(2)]],
                         constant FogParams &fog [[buffer(3)]])
{
    VOut out;
    float4 c = clip * float4(in.position, 1.0);
    // Depth from w (near plane at w = kNearW): z / w = 1 - kNearW / w.
    out.position = float4(c.x, c.y, c.w - kNearW, c.w);
    out.depth = c.w;
    out.fog = fog.vertexFog != 0 ? clamp(fog.base + fog.scale / max(c.w, 1e-3), 0.0, 255.0) / 255.0 : 1.0;
    out.normal = in.normal;
    out.uv = in.uv + light.uvOffset;
    out.color = float4(in.color) / 128.0; // 0x80 = 1.0
    out.color.a *= light.opacity;
    if (light.lit < 0)
    {
        // Water (microcode 1): alpha x (0.2 + 0.8 (1 - |n . v|)) in camera space.
        float4 p = light.directions * float4(in.position, 1.0);
        float4 n = light.directions * float4(0.0, 1.0, 0.0, 0.0);
        float c = dot(normalize(p.xyz), n.xyz);
        out.color.a *= 0.2 + 0.8 * (1.0 - abs(c));
    }
    else if (light.lit != 0)
    {
        float4 d = max(light.directions * float4(in.normal, 1.0), 0.0);
        d.w = 1.0;
        out.color.rgb = min(out.color.rgb * (light.colours * d).rgb, 255.0 / 128.0);
    }
    return out;
}

fragment float4 nativeFragment(VOut in [[stage_in]], texture2d<float> tex [[texture(0)]], texture2d<float> background [[texture(1)]],
                               texture2d<float> reflection [[texture(2)]], texture2d<float> refraction [[texture(3)]],
                               constant int &textured [[buffer(0)]], constant FogParams &fog [[buffer(1)]],
                               sampler sRepeat [[sampler(0)]], sampler sClampU [[sampler(1)]], sampler sClampV [[sampler(2)]],
                               sampler sClampUV [[sampler(3)]])
{
    float3 shade = in.color.rgb;
    if (textured == 5)
    {
        // The last frame shown, at the draw's own UV.
        constexpr sampler clampLinear(address::clamp_to_edge, filter::linear);
        return float4(tex.sample(clampLinear, in.uv).rgb, 1.0) * in.color;
    }
    if (textured == 4)
    {
        // The picture before this layer (a reduced copy) at the draw's own UV.
        constexpr sampler clampLinear(address::clamp_to_edge, filter::linear);
        float4 c = float4(refraction.sample(clampLinear, in.uv).rgb, 1.0) * in.color;
        if (c.a < 0.02)
            discard_fragment();
        return c;
    }
    if (textured == 3)
    {
        // The picture so far at this pixel (waterRefractionBegin's reduced copy), tinted.
        constexpr sampler clampLinear(address::clamp_to_edge, filter::linear);
        float3 r = refraction.sample(clampLinear, in.position.xy / fog.size).rgb;
        return float4(saturate(r * fog.debug.rgb), -fog.debug.w);
    }
    if (textured == 2)
    {
        // The mirrored world at this pixel, tinted (debug.w < 0: the tint's alpha).
        constexpr sampler clampLinear(address::clamp_to_edge, filter::linear);
        // reflectionBegin's layer 15 TEX0 is PSMCT24 with TCC 0: the mirror
        // image's alpha is never read; ALPHA 0x44 blends by the vertex alpha
        // (microcode 1's, x VU 0x3f3 w in debug.w). The mirror as reflectionBegin
        // ripples it: band i of 32 (8 rows of its 256) shifted right by
        // (sin(2 (12 i + t) 24 pi / H) + 1) (0.8 x 8 i / H + 0.2) texels.
        float2 uv = in.position.xy / fog.size;
        if (fog.ripple.y > 0.0)
        {
            const float band = floor(saturate(uv.y) * 32.0);
            uv.x += (sin(2.0 * (12.0 * band + fog.ripple.x) * 75.39822 / fog.ripple.y) + 1.0) * (0.8 * 8.0 * band / fog.ripple.y + 0.2) / 256.0;
        }
        float3 r = reflection.sample(clampLinear, uv).rgb;
        return float4(saturate(r * fog.debug.rgb * in.color.rgb), saturate(in.color.a * -fog.debug.w));
    }
    const float lod = fog.maxLod > 0.0 ? clamp(log2(max(in.depth, 1e-3)) * fog.lodScale + fog.lodK, 0.0, fog.maxLod) : 0.0;
    const bool hw = fog.hwLod != 0 && fog.maxLod > 0.0; // a texture the game samples at level 0 only stays there
    // Untextured, the GS takes the vertex colour as it is (255 = white),
    // where a textured draw modulates by it (0x80 = 1): half the 0x80 scale.
    float4 t = float4(float3(128.0 / 255.0), 1.0);
    if (textured != 0)
        t = fog.clamp == 3 ? sampleModel(tex, sClampUV, in.uv, lod, hw)
            : fog.clamp == 2 ? sampleModel(tex, sClampV, in.uv, lod, hw)
            : fog.clamp == 1 ? sampleModel(tex, sClampU, in.uv, lod, hw)
                             : sampleModel(tex, sRepeat, in.uv, lod, hw);
    float alpha = saturate(t.a * (textured != 0 ? 2.0 : 1.0) * in.color.a); // texture alpha 0x80 = opaque
    if (fog.atst >= 0 && gsAlphaPass(t.a * (textured != 0 ? 255.0 / 128.0 : 1.0) * in.color.a, fog.atst, fog.aref) == (fog.atSide != 0))
        discard_fragment();
    if (alpha < 0.02)
        discard_fragment();
    if (fog.debug.w > 0.0)
        return float4(fog.debug.rgb, 1.0);
    if (fog.steps < -0.5) // debugging: the vertex fog factor and depth
        return float4(in.fog, saturate(in.depth / 100.0), 0.0, 1.0);
    float3 rgb = saturate(t.rgb * shade);
    rgb = mix(fog.colour, rgb, in.fog);
    if (fog.fogged != 0)
    {
        // SotC's zFog: the scene fades into the background image (the sky)
        // in steps of depth.
        float f = saturate((in.depth - fog.start) / fog.width);
        if (fog.steps > 0.0)
            f = floor(f * fog.steps) / fog.steps;
        constexpr sampler clampLinear(address::clamp_to_edge, filter::linear);
        float3 bg = background.sample(clampLinear, in.position.xy / fog.size).rgb;
        rgb = mix(rgb, bg, f);
    }
    return float4(rgb, alpha);
}

// ---- full-screen passes (bloom) ----
struct Quad
{
    float4 position [[position]];
    float2 uv;
};

vertex Quad fullscreenVertex(uint id [[vertex_id]])
{
    Quad q;
    float2 p = float2((id << 1) & 2, id & 2);
    q.position = float4(p * 2.0 - 1.0, 0.0, 1.0);
    q.uv = float2(p.x, 1.0 - p.y);
    return q;
}

struct BloomParams
{
    float threshold, zA, zB, temporal;
    float4 maskColor;
    float4 color;
    float2 texel; // of the source
};

// The mask at a quarter resolution: the share of the 4x4 block beyond the threshold.
fragment float4 bloomMask(Quad in [[stage_in]], depth2d<float> depth [[texture(0)]], constant BloomParams &b [[buffer(0)]])
{
    float sum = 0.0;
    uint2 size = uint2(depth.get_width(), depth.get_height());
    uint2 base = uint2(in.uv * float2(size)) & ~uint2(3);
    for (uint y = 0; y < 4; ++y)
        for (uint x = 0; x < 4; ++x)
        {
            float d = depth.read(min(base + uint2(x, y), size - 1));
            // depth = 1 - kNearW / w (cleared: 1, nothing drawn); GS Z = zA + zB / w.
            float gsz = d >= 1.0 ? 0.0 : b.zA + b.zB * (1.0 - d) / kNearW;
            sum += gsz <= b.threshold + 1.0 ? 1.0 : 0.0;
        }
    return float4(b.maskColor.rgb * (sum / 16.0), 1.0);
}

// The GS chain (0x109e2a0) in its own buffers. The mask: the PS2 frame
// (512 x 448) white where GS Z <= the threshold + 1 (the sprite at that Z,
// ZTST GEQUAL), reduced to 256 x 256 by a bilinear sprite (texel coordinates
// (p + 0.5) x (2, 1.75): two by two texels averaged).
fragment float4 bloomMaskGS(Quad in [[stage_in]], depth2d<float> depth [[texture(0)]], constant BloomParams &b [[buffer(0)]])
{
    const uint2 size = uint2(depth.get_width(), depth.get_height());
    const float2 scale = float2(size) / float2(512.0, 448.0);
    const float2 p = floor(in.position.xy);
    const float2 t = (p + 0.5) * float2(2.0, 1.75) - 0.5; // texel space, centres at integers
    const float2 i0 = floor(t), f = t - i0;
    float m[4];
    for (int k = 0; k < 4; ++k)
    {
        const float2 at = clamp(i0 + float2(k & 1, k >> 1), float2(0.0), float2(511.0, 447.0));
        const uint2 px = min(uint2((at + 0.5) * scale), size - 1);
        const float d = depth.read(px);
        const float gsz = d >= 1.0 ? 0.0 : b.zA + b.zB * (1.0 - d) / kNearW;
        m[k] = gsz <= b.threshold + 1.0 ? 1.0 : 0.0;
    }
    const float v = mix(mix(m[0], m[1], f.x), mix(m[2], m[3], f.x), f.y);
    return float4(b.maskColor.rgb * v, 1.0);
}

// The GS chain's blur: a copy of the 64 x 64 source onto a target of R
// pixels, then 12 sprites shifted by 1, 2, 3 pixels (left, right, up, down)
// lerped over it (ALPHA 0x44) by 64, 32, 16 / 128, each sampling the source
// bilinearly at u = 0.5 + (p - d) x 64 / R (the copy at (p + 0.5) x 64 / R),
// a sprite covering only d <= p < d + R.
fragment float4 bloomBlur12(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant float &targetSize [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    const float r = targetSize, k = 64.0 / r;
    const float2 p = floor(in.position.xy);
    // In 8-bit steps, each lerp truncated as the GS's ((Cs - Cd) x A >> 7):
    // through the history the truncation compounds frame after frame.
    float3 acc = floor(src.sample(s, (p + 0.5) * k / 64.0).rgb * 255.0 + 0.001);
    const float2 d[12] = {float2(-1, 0), float2(1, 0), float2(0, -1), float2(0, 1), float2(-2, 0), float2(2, 0),
                          float2(0, -2), float2(0, 2), float2(-3, 0), float2(3, 0), float2(0, -3), float2(0, 3)};
    for (int i = 0; i < 12; ++i)
    {
        const float a = i < 4 ? 0.5 : (i < 8 ? 0.25 : 0.125);
        const float2 q = p - d[i];
        if (any(q < 0.0) || any(q >= r))
            continue;
        const float3 c = floor(src.sample(s, (0.5 + q * k) / 64.0).rgb * 255.0 + 0.001);
        acc += floor((c - acc) * a);
    }
    return float4(acc / 255.0, 1.0);
}

// The GS chain's temporal step: the history (the last frame's blurred
// result) drawn over this frame's 64 x 64 with alpha `temporal` (0x44:
// Cd + ((Cs - Cd) x As >> 7), truncated).
fragment float4 bloomTemporalGS(Quad in [[stage_in]], texture2d<float> current [[texture(0)]], texture2d<float> history [[texture(1)]],
                                constant float &alpha [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::nearest);
    const float3 cd = floor(current.sample(s, in.uv).rgb * 255.0 + 0.001), cs = floor(history.sample(s, in.uv).rgb * 255.0 + 0.001);
    return float4((cd + floor((cs - cd) * alpha)) / 255.0, 1.0);
}

// The composite sprite: 0..512 x 0..448 with UV 0.5..256.5 of the 256 x 256
// result, added (ALPHA 0x48) in the colour 0x118c140 (alpha 0x118c14c).
fragment float4 bloomCompositeGS(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant BloomParams &b [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    const float2 x = in.position.xy / b.texel - 0.5; // PS2 pixels (b.texel: render pixels per PS2 pixel)
    const float2 uv = (0.5 + x * float2(256.0 / 512.0, 256.0 / 448.0)) / 256.0;
    return float4(src.sample(s, uv).rgb * b.color.rgb * b.color.a, 1.0);
}

fragment float4 bloomBlur(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant float2 &step [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    const float w[4] = {0.30, 0.235, 0.12, 0.05};
    float4 c = src.sample(s, in.uv) * w[0];
    for (int i = 1; i < 4; ++i)
        c += (src.sample(s, in.uv + step * float(i)) + src.sample(s, in.uv - step * float(i))) * w[i];
    return c;
}

fragment float4 bloomTemporal(Quad in [[stage_in]], texture2d<float> current [[texture(0)]], texture2d<float> previous [[texture(1)]],
                              constant float &amount [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::nearest);
    return mix(previous.sample(s, in.uv), current.sample(s, in.uv), amount);
}

struct BlurCopy
{
    float scale, angle, alpha, pad;
    float2 offset; // NDC
};

vertex Quad blurCopyVertex(uint id [[vertex_id]], constant BlurCopy &b [[buffer(0)]])
{
    const float2 corner[6] = {float2(-1, -1), float2(1, -1), float2(-1, 1), float2(1, -1), float2(1, 1), float2(-1, 1)};
    float2 c = corner[id] * b.scale;
    float s = sin(b.angle), k = cos(b.angle);
    Quad q;
    q.position = float4(float2(c.x * k - c.y * s, c.x * s + c.y * k) + b.offset, 0.0, 1.0);
    q.uv = float2(corner[id].x * 0.5 + 0.5, 0.5 - corner[id].y * 0.5);
    return q;
}

fragment float4 blurCopyFragment(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant BlurCopy &b [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    return float4(src.sample(s, in.uv).rgb, b.alpha);
}

fragment float4 tintComposite(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant float4 &tint [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    return float4(saturate(src.sample(s, in.uv).rgb * tint.rgb), tint.a);
}

struct DofParams
{
    float zA, zB;
    uint focusZ;
    int slices;
};

// depthOfField's slices: the blurred copy where the GS Z is at most
// focusZ >> i, each pass blending by (i + 1) / slices over the last.
fragment float4 dofComposite(Quad in [[stage_in]], texture2d<float> blurred [[texture(0)]], depth2d<float> depth [[texture(1)]],
                             constant DofParams &p [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    uint2 size = uint2(depth.get_width(), depth.get_height());
    float d = depth.read(min(uint2(in.position.xy), size - 1));
    float gsz = d >= 1.0 ? 0.0 : p.zA + p.zB * (1.0 - d) / kNearW;
    float keep = 1.0; // the share of the picture left
    for (int i = 0; i < p.slices; ++i)
        if (gsz <= float(p.focusZ >> uint(i)))
            keep *= 1.0 - float(i + 1) / float(p.slices);
    return float4(blurred.sample(s, in.uv).rgb, 1.0 - keep);
}

// zFog: the background over the picture by the fog amount of the depth's slice.
fragment float4 fogComposite(Quad in [[stage_in]], texture2d<float> background [[texture(0)]], depth2d<float> depth [[texture(1)]],
                             constant float4 &p [[buffer(0)]]) // start, width, steps
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    uint2 size = uint2(depth.get_width(), depth.get_height());
    float d = depth.read(min(uint2(in.position.xy), size - 1));
    float w = d >= 1.0 ? 1e30 : kNearW / (1.0 - d);
    int steps = int(p.z);
    float a = 0.0;
    for (int i = 0; i <= steps; ++i)
        if (w >= p.x + p.y * float(i) / float(steps))
            a = float((i * 128) / steps) / 128.0;
    return float4(background.sample(s, in.uv).rgb, a);
}

// bossDeathEffectEnd's chain: plain bilinear copies (no blending), alpha kept.
fragment float4 glowReduce(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant float2 &offset [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    return src.sample(s, in.uv + offset);
}

// Its composite: the layer's copy modulated by the tint (0x80 = 1), blended
// by the copy's alpha times the amount.
fragment float4 glowComposite(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant float4 &tint [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    float4 c = src.sample(s, in.uv);
    return float4(saturate(c.rgb * tint.rgb), saturate(c.a * tint.a));
}

fragment float4 copyTexture(Quad in [[stage_in]], texture2d<float> src [[texture(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    return float4(src.sample(s, in.uv).rgb, 1.0);
}

// Effect detail >= 1: a reduction of the picture (to the PS2's 256 x 256
// copies) as the average of each destination pixel's whole footprint. At
// the PS2's size one bilinear tap is that average (2 x 2 texels); at 3x
// the same tap took 2 x 2 of 6 x 6 texels: the DoF / zFog / refraction
// copies came out aliased (blocky far cliffs).
fragment float4 reduceBox(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant float2 &dstSize [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    const float2 foot = float2(src.get_width(), src.get_height()) / dstSize; // source texels per destination pixel
    const int2 n = int2(clamp(ceil(foot / 2.0), 1.0, 8.0));                    // bilinear taps, 2 x 2 texels each
    const float2 corner = floor(in.position.xy) / dstSize;
    float4 sum = float4(0.0);
    for (int y = 0; y < n.y; ++y)
        for (int x = 0; x < n.x; ++x)
            sum += src.sample(s, corner + (float2(x, y) + 0.5) / float2(n) / dstSize);
    return sum / float(n.x * n.y);
}

// MSAA: the picture as the full-screen effects left it, put back into every
// sample before the next layers are drawn (exact texels, alpha kept).
fragment float4 reloadColor(Quad in [[stage_in]], texture2d<float> src [[texture(0)]])
{
    return src.read(uint2(in.position.xy));
}

fragment float4 shadowDarken(Quad in [[stage_in]], constant float &amount [[buffer(0)]])
{
    return float4(0.0, 0.0, 0.0, amount);
}

// Soft shadows (shadowDraw): the counted pixels as a mask, reduced 4 x 4,
// blurred, and the picture darkened by it.
fragment float4 shadowMaskWrite(Quad in [[stage_in]])
{
    return float4(1.0);
}

fragment float4 shadowReduce(Quad in [[stage_in]], texture2d<float> mask [[texture(0)]])
{
    float sum = 0.0;
    uint2 size = uint2(mask.get_width(), mask.get_height());
    uint2 base = uint2(in.uv * float2(size)) & ~uint2(3);
    for (uint y = 0; y < 4; ++y)
        for (uint x = 0; x < 4; ++x)
            sum += mask.read(min(base + uint2(x, y), size - 1)).r;
    return float4(float3(sum / 16.0), 1.0);
}

fragment float4 shadowSoft(Quad in [[stage_in]], texture2d<float> mask [[texture(0)]], constant float &amount [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    return float4(0.0, 0.0, 0.0, saturate(mask.sample(s, in.uv).r * amount));
}

fragment float4 bloomComposite(Quad in [[stage_in]], texture2d<float> src [[texture(0)]], constant BloomParams &b [[buffer(0)]])
{
    constexpr sampler s(address::clamp_to_edge, filter::linear);
    return float4(src.sample(s, in.uv).rgb * b.color.rgb * b.color.a, 1.0);
}
)MSL";

    struct Mesh
    {
        id<MTLBuffer> buffer = nil;
        uint32_t count = 0;
    };

    struct Draw
    {
        uint64_t mesh;
        native_renderer::DrawState state;
        float clip[16];
    };

    struct Renderer
    {
        id<MTLDevice> device = nil;
        id<MTLCommandQueue> queue = nil;
        id<MTLRenderPipelineState> pipeline = nil;  // normal blend
        id<MTLRenderPipelineState> additive = nil;
        id<MTLRenderPipelineState> opaque = nil;
        id<MTLRenderPipelineState> subtract = nil;
        id<MTLRenderPipelineState> volume = nil, darken = nil; // shadows
        id<MTLRenderPipelineState> shadowMaskPipeline = nil, shadowReducePipeline = nil, shadowSoftPipeline = nil;
        id<MTLTexture> shadowMask = nil; // full size: the counted pixels
        id<MTLDepthStencilState> volumeState = nil, shadowedState = nil;
        id<MTLDepthStencilState> depthEqualReadOnly = nil;
        id<MTLDepthStencilState> depthOff = nil;
        id<MTLDepthStencilState> depthReadOnly = nil;
        id<MTLDepthStencilState> depthState = nil;
        id<MTLTexture> color = nil, depth = nil, backgroundColor = nil;
        id<MTLTexture> backgroundSmall = nil; // bgEnd's 256 x 256 copy of the background
        id<MTLTexture> reflectionColor = nil, reflectionDepth = nil; // the mirrored world, 256 x 256
        id<MTLTexture> refractionColor = nil, refractionHalf = nil;  // the picture before layer 9, 256 x 256 (via 128 x 128)
        id<MTLRenderPipelineState> copySmall = nil, backdrop = nil;
        id<MTLRenderPipelineState> reducePipeline = nil; // reduceBox
        bool smoothReduce = false;                       // this frame: Tuning::effectDetail >= 1

        // A reduction from -> to: the whole footprint averaged (effect
        // detail >= 1, from more than twice as large), else `plain`.
        bool setReduce(id<MTLRenderCommandEncoder> e, id<MTLTexture> from, id<MTLTexture> to, id<MTLRenderPipelineState> plain)
        {
            if (smoothReduce && reducePipeline && (from.width > to.width * 2u || from.height > to.height * 2u))
            {
                const float size[2] = {static_cast<float>(to.width), static_cast<float>(to.height)};
                [e setRenderPipelineState:reducePipeline];
                [e setFragmentBytes:size length:sizeof(size) atIndex:0];
                return true;
            }
            [e setRenderPipelineState:plain];
            return false;
        }
        id<MTLRenderPipelineState> blurCopy = nil, tintOver = nil, glowOver = nil, glowReducePipeline = nil;
        id<MTLTexture> previousColor = nil; // the last frame shown (feedbackBlur)
        id<MTLTexture> glowLayer = nil;     // layer 8 alone (bossDeathEffectBegin's buffer), full size
        id<MTLTexture> glowSmall[4] = {};   // its chain: 256, 128, 64, 64
        native_renderer::GlowComposite glow;
        native_renderer::DepthOfField dof;
        id<MTLRenderPipelineState> dofPipeline = nil, fogPipeline = nil;
        id<MTLTexture> snapshot = nil; // the picture so far, for screenBlur
        std::vector<native_renderer::ScreenBlur> blurs;
        native_renderer::Fog fog;
        native_renderer::Bloom bloom;
        id<MTLTexture> bloomTex[3] = {nil, nil, nil}; // ping, pong, history
        bool bloomHistory = false;
        id<MTLRenderPipelineState> maskPipeline = nil, blurPipeline = nil, temporalPipeline = nil, compositePipeline = nil;
        id<MTLRenderPipelineState> maskGsPipeline = nil, blur12Pipeline = nil, compositeGsPipeline = nil, temporalGsPipeline = nil;
        // The GS chain's buffers: 0 the mask (256), 1 128, 2 64 (this frame),
        // 3 64 (lerped with the history), 4 64 (history: the blurred 3),
        // 5 128 (the blurred 3), 6 256 (that, enlarged).
        id<MTLTexture> gsBloom[7] = {nil, nil, nil, nil, nil, nil, nil};
        bool gsBloomHistory = false;
        uint32_t width = 0, height = 0;
        std::unordered_map<uint64_t, Mesh> meshes;
        std::unordered_map<uint64_t, id<MTLTexture>> textures;
        std::vector<Draw> draws;

        id<MTLLibrary> library = nil; // kept: the MSAA pipelines are built when first asked for
        // Texture filtering (Tuning::textureFilter, 0-6): per mode, a sampler
        // per CLAMP (repeat, u clamped, v clamped, both).
        id<MTLSamplerState> samplers[7][4] = {};
        // MSAA (Tuning::msaa): the scene's pipelines at a sample count, and
        // the multisampled targets, resolved into color / depth / glowLayer /
        // shadowMask at the end of each pass.
        struct ScenePipelines
        {
            id<MTLRenderPipelineState> normal = nil, additive = nil, opaque = nil, subtract = nil, volume = nil, darken = nil, shadowMask = nil,
                                       backdrop = nil, reload = nil;
        };
        ScenePipelines msPipelines[4]; // by log2(samples); [0] unused
        id<MTLTexture> colorMS = nil, depthMS = nil, glowMS = nil, shadowMaskMS = nil;
        NSUInteger samples = 1;        // the targets'
        uint32_t reflectionSize = 256; // the mirror image's (Tuning::effectDetail)
        bool colorDirty = false;       // MSAA: color changed by a full-screen effect since the last resolve

        std::mutex outputMutex;
        std::vector<uint8_t> output;
        uint32_t outputWidth = 0, outputHeight = 0;
        bool hasOutput = false;

        bool init()
        {
            device = MTLCreateSystemDefaultDevice();
            if (!device)
                return false;
            queue = [device newCommandQueue];
            NSError *error = nil;
            library = [device newLibraryWithSource:[NSString stringWithUTF8String:kShaders] options:nil error:&error];
            if (!library)
            {
                std::cerr << "[native] shader compile failed: " << [[error localizedDescription] UTF8String] << std::endl;
                return false;
            }
            MTLVertexDescriptor *layout = [MTLVertexDescriptor vertexDescriptor];
            layout.attributes[0].format = MTLVertexFormatFloat3;
            layout.attributes[0].offset = offsetof(native_renderer::Vertex, x);
            layout.attributes[1].format = MTLVertexFormatFloat3;
            layout.attributes[1].offset = offsetof(native_renderer::Vertex, nx);
            layout.attributes[2].format = MTLVertexFormatUChar4;
            layout.attributes[2].offset = offsetof(native_renderer::Vertex, r);
            layout.attributes[3].format = MTLVertexFormatFloat2;
            layout.attributes[3].offset = offsetof(native_renderer::Vertex, u);
            layout.layouts[0].stride = sizeof(native_renderer::Vertex);
            MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
            desc.vertexFunction = [library newFunctionWithName:@"nativeVertex"];
            desc.fragmentFunction = [library newFunctionWithName:@"nativeFragment"];
            desc.vertexDescriptor = layout;
            desc.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
            desc.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
            desc.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
            auto blend = desc.colorAttachments[0];
            blend.blendingEnabled = YES;
            blend.rgbBlendOperation = MTLBlendOperationAdd;
            blend.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
            blend.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            blend.alphaBlendOperation = MTLBlendOperationAdd;
            blend.sourceAlphaBlendFactor = MTLBlendFactorOne;
            blend.destinationAlphaBlendFactor = MTLBlendFactorZero;
            pipeline = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.destinationRGBBlendFactor = MTLBlendFactorOne;
            additive = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.rgbBlendOperation = MTLBlendOperationReverseSubtract;
            subtract = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.rgbBlendOperation = MTLBlendOperationAdd;
            blend.blendingEnabled = NO;
            opaque = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.writeMask = MTLColorWriteMaskNone;
            volume = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.writeMask = MTLColorWriteMaskAll;
            {
                // The darkening of the shadowed pixels: Cd x (1 - amount).
                id<MTLFunction> savedVertex = desc.vertexFunction, savedFragment = desc.fragmentFunction;
                MTLVertexDescriptor *savedLayout = [desc.vertexDescriptor retain];
                desc.vertexFunction = [library newFunctionWithName:@"fullscreenVertex"];
                desc.fragmentFunction = [library newFunctionWithName:@"shadowDarken"];
                desc.vertexDescriptor = nil;
                blend.blendingEnabled = YES;
                blend.sourceRGBBlendFactor = MTLBlendFactorZero;
                blend.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                blend.sourceAlphaBlendFactor = MTLBlendFactorZero;
                blend.destinationAlphaBlendFactor = MTLBlendFactorOne;
                darken = [device newRenderPipelineStateWithDescriptor:desc error:&error];
                [desc.fragmentFunction release];
                desc.fragmentFunction = [library newFunctionWithName:@"shadowMaskWrite"];
                blend.blendingEnabled = NO;
                shadowMaskPipeline = [device newRenderPipelineStateWithDescriptor:desc error:&error];
                [desc.vertexFunction release];
                [desc.fragmentFunction release];
                desc.vertexFunction = savedVertex;
                desc.fragmentFunction = savedFragment;
                desc.vertexDescriptor = savedLayout;
                [savedLayout release];
            }
            [desc release];
            auto fullscreen = [&](NSString *fragment, bool add) -> id<MTLRenderPipelineState>
            {
                MTLRenderPipelineDescriptor *d = [[MTLRenderPipelineDescriptor alloc] init];
                d.vertexFunction = [library newFunctionWithName:@"fullscreenVertex"];
                d.fragmentFunction = [library newFunctionWithName:fragment];
                d.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
                if (add)
                {
                    d.colorAttachments[0].blendingEnabled = YES;
                    d.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorOne;
                    d.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOne;
                    d.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
                    d.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
                }
                NSError *e = nil;
                id<MTLRenderPipelineState> state = [device newRenderPipelineStateWithDescriptor:d error:&e];
                [d.vertexFunction release];
                [d.fragmentFunction release];
                [d release];
                if (!state)
                    std::cerr << "[native] bloom pipeline failed: " << [[e localizedDescription] UTF8String] << std::endl;
                return state;
            };
            maskPipeline = fullscreen(@"bloomMask", false);
            blurPipeline = fullscreen(@"bloomBlur", false);
            temporalPipeline = fullscreen(@"bloomTemporal", false);
            compositePipeline = fullscreen(@"bloomComposite", true);
            maskGsPipeline = fullscreen(@"bloomMaskGS", false);
            blur12Pipeline = fullscreen(@"bloomBlur12", false);
            compositeGsPipeline = fullscreen(@"bloomCompositeGS", true);
            temporalGsPipeline = fullscreen(@"bloomTemporalGS", false);
            copySmall = fullscreen(@"copyTexture", false);
            reducePipeline = fullscreen(@"reduceBox", false);
            glowReducePipeline = fullscreen(@"glowReduce", false);
            shadowReducePipeline = fullscreen(@"shadowReduce", false);
            {
                MTLRenderPipelineDescriptor *d = [[MTLRenderPipelineDescriptor alloc] init];
                d.vertexFunction = [library newFunctionWithName:@"fullscreenVertex"];
                d.fragmentFunction = [library newFunctionWithName:@"dofComposite"];
                d.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
                d.colorAttachments[0].blendingEnabled = YES;
                d.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
                d.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                d.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
                d.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
                NSError *e = nil;
                dofPipeline = [device newRenderPipelineStateWithDescriptor:d error:&e];
                if (!dofPipeline)
                    std::cerr << "[native] dof pipeline failed: " << [[e localizedDescription] UTF8String] << std::endl;
                [d.fragmentFunction release];
                d.fragmentFunction = [library newFunctionWithName:@"fogComposite"];
                fogPipeline = [device newRenderPipelineStateWithDescriptor:d error:&e];
                if (!fogPipeline)
                    std::cerr << "[native] fog pipeline failed: " << [[e localizedDescription] UTF8String] << std::endl;
                [d.vertexFunction release];
                [d.fragmentFunction release];
                [d release];
            }
            {
                // Cd x (1 - As): the darkening by the soft mask.
                MTLRenderPipelineDescriptor *d = [[MTLRenderPipelineDescriptor alloc] init];
                d.vertexFunction = [library newFunctionWithName:@"fullscreenVertex"];
                d.fragmentFunction = [library newFunctionWithName:@"shadowSoft"];
                d.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
                d.colorAttachments[0].blendingEnabled = YES;
                d.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorZero;
                d.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                d.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
                d.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
                NSError *e = nil;
                shadowSoftPipeline = [device newRenderPipelineStateWithDescriptor:d error:&e];
                [d.vertexFunction release];
                [d.fragmentFunction release];
                [d release];
            }
            {
                // Cs x As + Cd x (1 - As); As may exceed 1 (GS alpha above 0x80).
                MTLRenderPipelineDescriptor *d = [[MTLRenderPipelineDescriptor alloc] init];
                d.vertexFunction = [library newFunctionWithName:@"fullscreenVertex"];
                d.fragmentFunction = [library newFunctionWithName:@"tintComposite"];
                d.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
                d.colorAttachments[0].blendingEnabled = YES;
                d.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
                d.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                d.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
                d.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
                NSError *e = nil;
                tintOver = [device newRenderPipelineStateWithDescriptor:d error:&e];
                d.fragmentFunction = [library newFunctionWithName:@"glowComposite"];
                glowOver = [device newRenderPipelineStateWithDescriptor:d error:&e];
                [d.vertexFunction release];
                [d.fragmentFunction release];
                [d release];
            }
            {
                MTLRenderPipelineDescriptor *d = [[MTLRenderPipelineDescriptor alloc] init];
                d.vertexFunction = [library newFunctionWithName:@"blurCopyVertex"];
                d.fragmentFunction = [library newFunctionWithName:@"blurCopyFragment"];
                d.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
                d.colorAttachments[0].blendingEnabled = YES;
                d.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
                d.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                d.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
                d.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
                NSError *e = nil;
                blurCopy = [device newRenderPipelineStateWithDescriptor:d error:&e];
                [d.vertexFunction release];
                [d.fragmentFunction release];
                [d release];
            }
            {
                MTLRenderPipelineDescriptor *d = [[MTLRenderPipelineDescriptor alloc] init];
                d.vertexFunction = [library newFunctionWithName:@"fullscreenVertex"];
                d.fragmentFunction = [library newFunctionWithName:@"copyTexture"];
                d.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
                d.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
                d.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
                NSError *e = nil;
                backdrop = [device newRenderPipelineStateWithDescriptor:d error:&e];
                [d.vertexFunction release];
                [d.fragmentFunction release];
                [d release];
            }
            for (int mode = 0; mode < 7; ++mode)
                for (int c = 0; c < 4; ++c)
                {
                    // 0 nearest, 1 bilinear, 2 trilinear (the old fixed samplers), 3-6 anisotropic 2x-16x.
                    MTLSamplerDescriptor *sd = [[MTLSamplerDescriptor alloc] init];
                    sd.minFilter = sd.magFilter = mode == 0 ? MTLSamplerMinMagFilterNearest : MTLSamplerMinMagFilterLinear;
                    sd.mipFilter = mode <= 1 ? MTLSamplerMipFilterNearest : MTLSamplerMipFilterLinear;
                    sd.sAddressMode = (c & 1) ? MTLSamplerAddressModeClampToEdge : MTLSamplerAddressModeRepeat;
                    sd.tAddressMode = (c & 2) ? MTLSamplerAddressModeClampToEdge : MTLSamplerAddressModeRepeat;
                    sd.rAddressMode = c == 3 ? MTLSamplerAddressModeClampToEdge : MTLSamplerAddressModeRepeat;
                    sd.maxAnisotropy = mode >= 3 ? (1u << (mode - 2)) : 1u;
                    samplers[mode][c] = [device newSamplerStateWithDescriptor:sd];
                    [sd release];
                }
            if (!pipeline)
            {
                std::cerr << "[native] pipeline failed: " << [[error localizedDescription] UTF8String] << std::endl;
                return false;
            }
            // The GS tests Z with GEQUAL (its Z grows towards the camera): a
            // surface drawn again in the same place (the translucent layers
            // over the ground) passes. Here: less or equal.
            MTLDepthStencilDescriptor *depthDesc = [[MTLDepthStencilDescriptor alloc] init];
            depthDesc.depthCompareFunction = MTLCompareFunctionLessEqual;
            depthDesc.depthWriteEnabled = YES;
            depthState = [device newDepthStencilStateWithDescriptor:depthDesc];
            depthDesc.depthWriteEnabled = NO;
            depthReadOnly = [device newDepthStencilStateWithDescriptor:depthDesc];
            depthDesc.depthCompareFunction = MTLCompareFunctionLessEqual;
            depthEqualReadOnly = [device newDepthStencilStateWithDescriptor:depthDesc];
            depthDesc.depthCompareFunction = MTLCompareFunctionAlways;
            depthOff = [device newDepthStencilStateWithDescriptor:depthDesc];
            {
                // Shadow volumes: the count where the faces pass the depth test.
                MTLStencilDescriptor *front = [[MTLStencilDescriptor alloc] init], *back = [[MTLStencilDescriptor alloc] init];
                front.stencilCompareFunction = back.stencilCompareFunction = MTLCompareFunctionAlways;
                front.depthStencilPassOperation = MTLStencilOperationIncrementWrap;
                back.depthStencilPassOperation = MTLStencilOperationDecrementWrap;
                depthDesc.depthCompareFunction = MTLCompareFunctionLess;
                depthDesc.depthWriteEnabled = NO;
                depthDesc.frontFaceStencil = front;
                depthDesc.backFaceStencil = back;
                volumeState = [device newDepthStencilStateWithDescriptor:depthDesc];
                // The darkening: where the count is not 0; it clears the count.
                front.stencilCompareFunction = back.stencilCompareFunction = MTLCompareFunctionNotEqual;
                front.depthStencilPassOperation = back.depthStencilPassOperation = MTLStencilOperationZero;
                depthDesc.depthCompareFunction = MTLCompareFunctionAlways;
                depthDesc.frontFaceStencil = front;
                depthDesc.backFaceStencil = back;
                shadowedState = [device newDepthStencilStateWithDescriptor:depthDesc];
                [front release];
                [back release];
            }
            [depthDesc release];
            return true;
        }

        // The scene's pipelines at 2^k samples (as init's, plus the reload).
        bool buildMsPipelines(int k)
        {
            ScenePipelines &p = msPipelines[k];
            if (p.normal)
                return true;
            MTLVertexDescriptor *layout = [MTLVertexDescriptor vertexDescriptor];
            layout.attributes[0].format = MTLVertexFormatFloat3;
            layout.attributes[0].offset = offsetof(native_renderer::Vertex, x);
            layout.attributes[1].format = MTLVertexFormatFloat3;
            layout.attributes[1].offset = offsetof(native_renderer::Vertex, nx);
            layout.attributes[2].format = MTLVertexFormatUChar4;
            layout.attributes[2].offset = offsetof(native_renderer::Vertex, r);
            layout.attributes[3].format = MTLVertexFormatFloat2;
            layout.attributes[3].offset = offsetof(native_renderer::Vertex, u);
            layout.layouts[0].stride = sizeof(native_renderer::Vertex);
            NSError *error = nil;
            id<MTLFunction> vertex = [library newFunctionWithName:@"nativeVertex"], fragment = [library newFunctionWithName:@"nativeFragment"];
            id<MTLFunction> full = [library newFunctionWithName:@"fullscreenVertex"];
            MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
            desc.rasterSampleCount = 1u << k;
            desc.vertexFunction = vertex;
            desc.fragmentFunction = fragment;
            desc.vertexDescriptor = layout;
            desc.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
            desc.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
            desc.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
            auto blend = desc.colorAttachments[0];
            blend.blendingEnabled = YES;
            blend.rgbBlendOperation = MTLBlendOperationAdd;
            blend.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
            blend.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            blend.alphaBlendOperation = MTLBlendOperationAdd;
            blend.sourceAlphaBlendFactor = MTLBlendFactorOne;
            blend.destinationAlphaBlendFactor = MTLBlendFactorZero;
            p.normal = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.destinationRGBBlendFactor = MTLBlendFactorOne;
            p.additive = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.rgbBlendOperation = MTLBlendOperationReverseSubtract;
            p.subtract = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.rgbBlendOperation = MTLBlendOperationAdd;
            blend.blendingEnabled = NO;
            p.opaque = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.writeMask = MTLColorWriteMaskNone;
            p.volume = [device newRenderPipelineStateWithDescriptor:desc error:&error];
            blend.writeMask = MTLColorWriteMaskAll;
            desc.vertexFunction = full;
            desc.vertexDescriptor = nil;
            const std::pair<NSString *, id<MTLRenderPipelineState> *> plain[] = {
                {@"shadowMaskWrite", &p.shadowMask}, {@"copyTexture", &p.backdrop}, {@"reloadColor", &p.reload}};
            for (const auto &[name, out] : plain)
            {
                id<MTLFunction> f = [library newFunctionWithName:name];
                desc.fragmentFunction = f;
                *out = [device newRenderPipelineStateWithDescriptor:desc error:&error];
                [f release];
            }
            {
                id<MTLFunction> f = [library newFunctionWithName:@"shadowDarken"];
                desc.fragmentFunction = f;
                blend.blendingEnabled = YES;
                blend.sourceRGBBlendFactor = MTLBlendFactorZero;
                blend.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                blend.sourceAlphaBlendFactor = MTLBlendFactorZero;
                blend.destinationAlphaBlendFactor = MTLBlendFactorOne;
                p.darken = [device newRenderPipelineStateWithDescriptor:desc error:&error];
                [f release];
            }
            [desc release];
            [vertex release];
            [fragment release];
            [full release];
            const bool ok = p.normal && p.additive && p.subtract && p.opaque && p.volume && p.shadowMask && p.backdrop && p.reload && p.darken;
            if (!ok)
            {
                std::cerr << "[native] MSAA x" << (1u << k) << " pipelines failed: " << (error ? [[error localizedDescription] UTF8String] : "?") << std::endl;
                p = ScenePipelines{};
            }
            return ok;
        }

        void ensureTargets(uint32_t w, uint32_t h, NSUInteger wantSamples, uint32_t wantReflection)
        {
            if (color && width == w && height == h && samples == wantSamples && reflectionSize == wantReflection)
                return;
            samples = wantSamples;
            reflectionSize = wantReflection;
            for (id<MTLTexture> *t : {&colorMS, &depthMS, &glowMS, &shadowMaskMS})
            {
                [*t release];
                *t = nil;
            }
            if (samples > 1u)
            {
                MTLTextureDescriptor *m = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:w height:h mipmapped:NO];
                m.textureType = MTLTextureType2DMultisample;
                m.sampleCount = samples;
                m.usage = MTLTextureUsageRenderTarget;
                m.storageMode = MTLStorageModePrivate;
                colorMS = [device newTextureWithDescriptor:m];
                glowMS = [device newTextureWithDescriptor:m];
                shadowMaskMS = [device newTextureWithDescriptor:m];
                m.pixelFormat = MTLPixelFormatDepth32Float_Stencil8;
                depthMS = [device newTextureWithDescriptor:m];
            }
            [color release];
            [depth release];
            [backgroundColor release];
            MTLTextureDescriptor *cd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:w height:h mipmapped:NO];
            cd.usage = MTLTextureUsageRenderTarget;
            cd.storageMode = MTLStorageModeShared;
            color = [device newTextureWithDescriptor:cd];
            [snapshot release];
            {
                MTLTextureDescriptor *sd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:w height:h mipmapped:NO];
                sd.usage = MTLTextureUsageShaderRead;
                sd.storageMode = MTLStorageModePrivate;
                snapshot = [device newTextureWithDescriptor:sd];
            }
            cd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
            cd.storageMode = MTLStorageModePrivate;
            backgroundColor = [device newTextureWithDescriptor:cd];
            [backgroundSmall release];
            [refractionColor release];
            [refractionHalf release];
            for (uint32_t size : {256u, 128u})
            {
                MTLTextureDescriptor *fd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:size height:size mipmapped:NO];
                fd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                fd.storageMode = MTLStorageModePrivate;
                (size == 256u ? refractionColor : refractionHalf) = [device newTextureWithDescriptor:fd];
            }
            [reflectionColor release];
            [reflectionDepth release];
            {
                MTLTextureDescriptor *rd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:reflectionSize height:reflectionSize mipmapped:NO];
                rd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                rd.storageMode = MTLStorageModePrivate;
                reflectionColor = [device newTextureWithDescriptor:rd];
                MTLTextureDescriptor *rz = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float_Stencil8 width:reflectionSize height:reflectionSize mipmapped:NO];
                rz.usage = MTLTextureUsageRenderTarget;
                rz.storageMode = MTLStorageModePrivate;
                reflectionDepth = [device newTextureWithDescriptor:rz];
            }
            {
                MTLTextureDescriptor *sd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:256 height:256 mipmapped:NO];
                sd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                sd.storageMode = MTLStorageModePrivate;
                backgroundSmall = [device newTextureWithDescriptor:sd];
            }
            MTLTextureDescriptor *dd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float_Stencil8 width:w height:h mipmapped:NO];
            dd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
            dd.storageMode = MTLStorageModePrivate;
            depth = [device newTextureWithDescriptor:dd];
            [shadowMask release];
            {
                MTLTextureDescriptor *md = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:w height:h mipmapped:NO];
                md.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                md.storageMode = MTLStorageModePrivate;
                shadowMask = [device newTextureWithDescriptor:md];
            }
            [previousColor release];
            {
                MTLTextureDescriptor *pd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:w height:h mipmapped:NO];
                pd.usage = MTLTextureUsageShaderRead;
                pd.storageMode = MTLStorageModePrivate;
                previousColor = [device newTextureWithDescriptor:pd];
            }
            [glowLayer release];
            {
                MTLTextureDescriptor *gd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:w height:h mipmapped:NO];
                gd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                gd.storageMode = MTLStorageModePrivate;
                glowLayer = [device newTextureWithDescriptor:gd];
            }
            for (int i = 0; i < 4; ++i)
            {
                [glowSmall[i] release];
                const uint32_t size = i == 0 ? 256u : (i == 1 ? 128u : 64u);
                MTLTextureDescriptor *gd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:size height:size mipmapped:NO];
                gd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                gd.storageMode = MTLStorageModePrivate;
                glowSmall[i] = [device newTextureWithDescriptor:gd];
            }
            for (id<MTLTexture> &t : bloomTex)
            {
                [t release];
                MTLTextureDescriptor *bd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                              width:std::max(w / 4u, 1u) height:std::max(h / 4u, 1u) mipmapped:NO];
                bd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                bd.storageMode = MTLStorageModePrivate;
                t = [device newTextureWithDescriptor:bd];
            }
            for (int i = 0; i < 7; ++i)
            {
                [gsBloom[i] release];
                const uint32_t size = (i == 0 || i == 6) ? 256u : ((i == 1 || i == 5) ? 128u : 64u);
                MTLTextureDescriptor *bd = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:size height:size mipmapped:NO];
                bd.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                bd.storageMode = MTLStorageModePrivate;
                gsBloom[i] = [device newTextureWithDescriptor:bd];
            }
            gsBloomHistory = false;
            bloomHistory = false;
            width = w;
            height = h;
        }

        void render(uint32_t w, uint32_t h)
        {
            @autoreleasepool
            {
                {
                    // The player's quality: MSAA as far as the GPU has it, the mirror's size.
                    const native_renderer::Tuning &q = native_renderer::tuning();
                    int k = std::clamp(static_cast<int>(q.msaa + 0.5f), 0, 3);
                    while (k > 0 && (![device supportsTextureSampleCount:(1u << k)] || !buildMsPipelines(k)))
                        --k;
                    ensureTargets(w, h, 1u << k, 256u << std::clamp(static_cast<int>(q.effectDetail + 0.5f), 0, 2));
                }
                const ScenePipelines &msp = msPipelines[samples > 1u ? __builtin_ctz(static_cast<unsigned>(samples)) : 0];
                const int filterMode = std::clamp(static_cast<int>(native_renderer::tuning().textureFilter + 0.5f), 0, 6);
                colorDirty = false;
                smoothReduce = native_renderer::tuning().effectDetail >= 0.5f;
                std::stable_sort(draws.begin(), draws.end(), [](const Draw &a, const Draw &b) { return a.state.order < b.state.order; });
                id<MTLCommandBuffer> commands = [queue commandBuffer];
                const bool fogOn = fog.enabled && fog.width > 0.0f;
                // The mirrored world first, when this frame has any.
                bool haveMirror = false;
                for (const Draw &d : draws)
                    if (d.state.reflectionSource)
                    {
                        haveMirror = true;
                        break;
                    }
                // Shadows as shadowDraw makes them: the volumes' count is kept
                // until the layer after theirs, then blurred and darkened by.
                static const bool hardShadows = std::getenv("SOTC_SHADOW_HARD") != nullptr; // debugging
                uint32_t softOrder = 0;
                float softAmount = 0.0f;
                if (!hardShadows && shadowMaskPipeline && shadowReducePipeline && shadowSoftPipeline && blurPipeline)
                    for (const Draw &d : draws)
                        if (d.state.shadowVolume)
                        {
                            softOrder = std::max(softOrder, d.state.order + 1u);
                            softAmount = d.state.shadow;
                        }
                const bool softShadows = softOrder != 0;
                auto pass = [&](id<MTLTexture> target, bool backgroundOnly, uint32_t fromOrder = 0, uint32_t toOrder = ~0u, bool load = false,
                                bool mirror = false, bool apart = false)
                {
                    // MSAA: the picture and the glow layer are drawn multisampled and
                    // resolved at the end of the pass (the depth too, for the effects).
                    const bool ms = samples > 1u && !mirror && (target == color || target == glowLayer);
                    if (ms && load && !apart && colorDirty)
                    {
                        // The full-screen effects drew on the resolved picture: read back below.
                        id<MTLBlitCommandEncoder> blit = [commands blitCommandEncoder];
                        [blit copyFromTexture:color toTexture:snapshot];
                        [blit endEncoding];
                    }
                    id<MTLRenderPipelineState> pNormal = ms ? msp.normal : pipeline, pAdditive = ms ? msp.additive : additive,
                                               pOpaque = ms ? msp.opaque : opaque, pSubtract = ms ? msp.subtract : subtract,
                                               pVolume = ms ? msp.volume : volume, pDarken = ms ? msp.darken : darken,
                                               pBackdrop = ms ? msp.backdrop : backdrop;
                    MTLRenderPassDescriptor *desc = [MTLRenderPassDescriptor renderPassDescriptor];
                    desc.colorAttachments[0].texture = ms ? (target == color ? colorMS : glowMS) : target;
                    desc.colorAttachments[0].loadAction = load && !apart ? MTLLoadActionLoad : MTLLoadActionClear;
                    // The PS2 frame starts black; the mirror image is cleared to the fog
                    // colour (reflectionBegin: alpha 0x80); a layer drawn apart (on the
                    // scene's depth) to nothing.
                    desc.colorAttachments[0].clearColor = mirror ? MTLClearColorMake(fog.colour[0], fog.colour[1], fog.colour[2], 1.0)
                                                                 : MTLClearColorMake(0.0, 0.0, 0.0, apart ? 0.0 : 1.0);
                    desc.colorAttachments[0].storeAction = MTLStoreActionStore;
                    id<MTLTexture> depth = ms ? depthMS : (mirror ? reflectionDepth : this->depth);
                    desc.depthAttachment.texture = depth;
                    desc.depthAttachment.loadAction = load ? MTLLoadActionLoad : MTLLoadActionClear;
                    desc.depthAttachment.clearDepth = 1.0;
                    desc.depthAttachment.storeAction = MTLStoreActionStore;
                    desc.stencilAttachment.texture = depth;
                    desc.stencilAttachment.loadAction = load ? MTLLoadActionLoad : MTLLoadActionClear;
                    desc.stencilAttachment.clearStencil = 0;
                    desc.stencilAttachment.storeAction = MTLStoreActionStore;
                    if (ms)
                    {
                        desc.colorAttachments[0].resolveTexture = target;
                        desc.colorAttachments[0].storeAction = MTLStoreActionStoreAndMultisampleResolve;
                        desc.depthAttachment.resolveTexture = this->depth;
                        desc.depthAttachment.depthResolveFilter = MTLMultisampleDepthResolveFilterSample0;
                        desc.depthAttachment.storeAction = MTLStoreActionStoreAndMultisampleResolve;
                    }
                    id<MTLRenderCommandEncoder> encoder = [commands renderCommandEncoderWithDescriptor:desc];
                    [encoder setCullMode:MTLCullModeNone];
                    int boundFilter = 2; // the samplers bound: the player's for 3D, trilinear for 2D
                    for (int c = 0; c < 4; ++c)
                        [encoder setFragmentSamplerState:samplers[2][c] atIndex:c];
                    if (ms && load && !apart && colorDirty && msp.reload)
                    {
                        [encoder setRenderPipelineState:msp.reload];
                        [encoder setDepthStencilState:depthOff];
                        [encoder setFragmentTexture:snapshot atIndex:0];
                        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                    }
                    if (ms && target == color)
                        colorDirty = false;
                    if (!backgroundOnly)
                        [encoder setFragmentTexture:backgroundSmall atIndex:1];
                    if (!mirror)
                        [encoder setFragmentTexture:reflectionColor atIndex:2];
                    if (!mirror)
                        [encoder setFragmentTexture:refractionColor atIndex:3];
                    if (!mirror && !backgroundOnly && !load && fogOn && pBackdrop && native_renderer::tuning().skyImage)
                    {
                        // The background as the PS2 shows it: bgEnd's reduced copy,
                        // stretched back (bgDraw), under everything.
                        [encoder setRenderPipelineState:pBackdrop];
                        [encoder setDepthStencilState:depthOff];
                        [encoder setFragmentTexture:backgroundSmall atIndex:0];
                        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                    }
                    static const bool additiveZ = std::getenv("SOTC_NATIVE_ADDZ") != nullptr; // debugging
                    static const bool showFog = std::getenv("SOTC_NATIVE_SHOWFOG") != nullptr; // debugging
                    static const bool noOverlayFog = std::getenv("SOTC_FOG_NO_OVERLAY") != nullptr; // debugging
                    static const bool bgFog = std::getenv("SOTC_NATIVE_BGFOG") != nullptr; // every batch has FGE, the sky too
                    const bool gsAlphaTestOn = native_renderer::tuning().alphaTest;
                    // Round 38: layer 1 (the sky and the far landscape: bgEnd copies it, as the
                    // GS drew it into the frame, Z tested, then sets Z to 1) drawn with the depth
                    // test into the background image; without it the trees behind a far cliff
                    // and the cliffs' far sides were drawn over it in draw order.
                    const bool bgDepth = native_renderer::tuning().backgroundDepth;
                    uint32_t alphaTestSecondPasses = 0; // (a count for debugging)
                    int bound = -1;
                    // Shadow volumes counted so far, waiting for their darkening.
                    uint32_t shadowOrder = 0;
                    float shadowAmount = 0.0f;
                    auto resolveShadows = [&]
                    {
                        if (!shadowAmount)
                            return;
                        if (softShadows) // resolved by doShadow, the count kept
                        {
                            shadowAmount = 0.0f;
                            return;
                        }
                        [encoder setRenderPipelineState:pDarken];
                        [encoder setDepthStencilState:shadowedState];
                        [encoder setStencilReferenceValue:0];
                        [encoder setFragmentBytes:&shadowAmount length:sizeof(shadowAmount) atIndex:0];
                        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                        shadowAmount = 0.0f;
                        bound = -1;
                    };
                    for (const Draw &d : draws)
                    {
                        if (backgroundOnly && !d.state.background)
                            continue;
                        if (mirror != d.state.reflectionSource)
                            continue;
                        if (showFog && !backgroundOnly && (d.state.screen || d.state.order > 6u))
                            continue; // debugging: only the opaque world
                        if (d.state.reflectionSample && !haveMirror)
                            continue;
                        if (!backgroundOnly && fogOn && d.state.background)
                            continue; // shown through the backdrop
                        if (d.state.order < fromOrder || d.state.order >= toOrder)
                            continue;
                        if (shadowAmount && d.state.order > shadowOrder)
                            resolveShadows();
                        if (d.state.shadowVolume)
                        {
                            if (backgroundOnly)
                                continue;
                            auto it = meshes.find(d.mesh);
                            if (it == meshes.end() || it->second.count == 0)
                                continue;
                            [encoder setRenderPipelineState:pVolume];
                            [encoder setDepthStencilState:volumeState];
                            // The GS does not cut at the near plane: the caps microcode 8
                            // puts at z = 0.1 stay, nearer than everything.
                            [encoder setDepthClipMode:MTLDepthClipModeClamp];
                            [encoder setVertexBuffer:it->second.buffer offset:0 atIndex:0];
                            [encoder setVertexBytes:d.clip length:sizeof(d.clip) atIndex:1];
                            struct
                            {
                                float light[32];
                                int lit;
                                float opacity;
                                int pad[2];
                            } none = {};
                            none.opacity = 1.0f;
                            [encoder setVertexBytes:&none length:sizeof(none) atIndex:2];
                            struct
                            {
                                float values[28];
                            } nofog = {};
                            [encoder setVertexBytes:&nofog length:sizeof(nofog) atIndex:3];
                            [encoder setFragmentBytes:&nofog length:sizeof(nofog) atIndex:1];
                            const int untextured = 0;
                            [encoder setFragmentBytes:&untextured length:sizeof(untextured) atIndex:0];
                            [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:it->second.count];
                            [encoder setDepthClipMode:MTLDepthClipModeClip];
                            shadowOrder = d.state.order;
                            shadowAmount = d.state.shadow;
                            bound = -1;
                            continue;
                        }
                        const int blendMode = static_cast<int>(d.state.blend);
                        const bool backdrop = d.state.screen || d.state.background;
                        const int mode = blendMode | (backdrop ? 8 : 0) | (backgroundOnly && d.state.background ? 256 : 0) | (d.state.overlay ? 16 : 0) | (d.state.writeDepth ? 32 : 0) | (d.state.noDepthWrite ? 64 : 0) |
                                         (d.state.screenDepthTest ? 128 : 0);
                        if (mode != bound)
                        {
                            [encoder setRenderPipelineState:blendMode == 1 ? pAdditive : (blendMode == 2 ? pOpaque : (blendMode == 3 ? pSubtract : pNormal))];
                            // Additive glows do not hide what is drawn after them; 2D and the
                            // background (the sky, a small dome drawn first) ignore depth; a
                            // second pass lands on its first.
                            [encoder setDepthStencilState:d.state.screenDepthTest ? depthReadOnly
                                                          : (backgroundOnly && bgDepth && d.state.background && !d.state.screen) ? depthState
                                                          : backdrop ? depthOff
                                                                   : (d.state.overlay ? depthEqualReadOnly
                                                                                      : ((blendMode == 1 && !d.state.writeDepth && !additiveZ) || d.state.noDepthWrite ? depthReadOnly : depthState))];
                            bound = mode;
                        }
                        auto it = meshes.find(d.mesh);
                        if (it == meshes.end() || it->second.count == 0)
                            continue;
                        [encoder setVertexBuffer:it->second.buffer offset:0 atIndex:0];
                        [encoder setVertexBytes:d.clip length:sizeof(d.clip) atIndex:1];
                        struct
                        {
                            float light[32];
                            int lit;
                            float opacity;
                            float uvOffset[2];
                        } light;
                        std::memcpy(light.light, d.state.light, sizeof(light.light));
                        light.lit = d.state.fresnel ? -1 : (d.state.lit ? 1 : 0);
                        light.opacity = d.state.opacity;
                        light.uvOffset[0] = d.state.uvOffset[0];
                        light.uvOffset[1] = d.state.uvOffset[1];
                        [encoder setVertexBytes:&light length:sizeof(light) atIndex:2];
                        auto tex = d.state.texture ? textures.find(d.state.texture) : textures.end();
                        const int textured = d.state.previousFrame ? 5 : d.state.reflectionSample ? 2 : (d.state.refractionSample ? 3 : (d.state.screenCopy ? 4 : (tex != textures.end() ? 1 : 0)));
                        if (textured == 1)
                            [encoder setFragmentTexture:tex->second atIndex:0];
                        else if (textured == 5)
                            [encoder setFragmentTexture:previousColor atIndex:0];
                        [encoder setFragmentBytes:&textured length:sizeof(textured) atIndex:0];
                        // Texture filtering is the 3D's: 2D (menus, text, the map) keeps the default.
                        const int drawFilter = d.state.screen ? 2 : filterMode;
                        if (drawFilter != boundFilter)
                        {
                            for (int c = 0; c < 4; ++c)
                                [encoder setFragmentSamplerState:samplers[drawFilter][c] atIndex:c];
                            boundFilter = drawFilter;
                        }
                        struct
                        {
                            float start, width, steps;
                            int fogged;
                            float size[2];
                            int vertexFog;
                            float base, scale;
                            float lodK, lodScale, maxLod;
                            float colour[4];
                            float debug[4];
                            int clamp;
                            int hwLod; // anisotropic filtering: the GPU's own mip level
                            float ripple[2];
                            int atst;
                            float aref;
                            int atSide, atPad;
                        } fp = {fog.start, fog.width, (showFog && !d.state.screen) ? -1.0f : static_cast<float>(fog.steps), (!backgroundOnly && fogOn && d.state.fogged) ? 1 : 0,
                                {static_cast<float>(w), static_cast<float>(h)},
                                (fog.vertexFog && !d.state.screen && (!d.state.background || bgFog) && !(d.state.overlay && noOverlayFog)) ? 1 : 0, fog.base, fog.scale,
                                d.state.lodK, d.state.lodScale, d.state.maxLod,
                                {fog.colour[0], fog.colour[1], fog.colour[2], 0},
                                {static_cast<float>(d.state.debugColour & 0xFFu) / 255.0f, static_cast<float>((d.state.debugColour >> 8) & 0xFFu) / 255.0f,
                                 static_cast<float>((d.state.debugColour >> 16) & 0xFFu) / 255.0f, d.state.debugColour ? 1.0f : 0.0f},
                                d.state.clamp, drawFilter >= 3 ? 1 : 0, {d.state.mirrorRipple[0], d.state.mirrorRipple[1]}, -1, 0.0f, 0, 0};
                        if (d.state.reflectionSample || d.state.refractionSample)
                        {
                            for (int k = 0; k < 3; ++k)
                                fp.debug[k] = d.state.reflectionTint[k];
                            fp.debug[3] = -d.state.reflectionTint[3];
                        }
                        // The GS alpha test (DrawState::alphaTest): the passing pixels with the
                        // draw's own depth state, then (AFAIL FB_ONLY / RGB_ONLY, when a pixel
                        // can fail) the failing ones again with the depth read only.
                        const uint32_t at = gsAlphaTestOn ? d.state.alphaTest : 0u;
                        const int afail = static_cast<int>((at >> 12) & 3u);
                        const bool atTwo = (at & 1u) && ((at >> 1) & 7u) != 1u && (afail == 1 || afail == 3) && !d.state.overlay && !backdrop &&
                                           blendMode != 1 && !d.state.noDepthWrite && !d.state.screenDepthTest;
                        if (at & 1u)
                        {
                            fp.atst = static_cast<int>((at >> 1) & 7u);
                            fp.aref = static_cast<float>((at >> 4) & 0xFFu);
                            // ZB_ONLY / KEEP and the passes that write no depth anyway: the failing pixels dropped
                            // (FB_ONLY there draws them as the passing ones: no test).
                            if (!atTwo && (afail == 1 || afail == 3))
                                fp.atst = -1;
                        }
                        [encoder setFragmentBytes:&fp length:sizeof(fp) atIndex:1];
                        [encoder setVertexBytes:&fp length:sizeof(fp) atIndex:3];
                        if (!(atTwo && fp.atst == 0)) // ATST NEVER: every pixel fails
                            [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:it->second.count];
                        if (atTwo)
                        {
                            fp.atSide = 1;
                            [encoder setFragmentBytes:&fp length:sizeof(fp) atIndex:1];
                            [encoder setDepthStencilState:depthReadOnly];
                            [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:it->second.count];
                            bound = -1;
                            ++alphaTestSecondPasses;
                        }
                    }
                    resolveShadows();
                    [encoder endEncoding];
                };
                if (haveMirror)
                    pass(reflectionColor, false, 0, ~0u, false, true);
                if (fogOn && copySmall)
                {
                    pass(backgroundColor, true); // the sky, what distance fades into
                    // bgEnd: reduced to 256 x 256 (bilinear), which blurs it.
                    MTLRenderPassDescriptor *desc = [MTLRenderPassDescriptor renderPassDescriptor];
                    desc.colorAttachments[0].texture = backgroundSmall;
                    desc.colorAttachments[0].loadAction = MTLLoadActionDontCare;
                    desc.colorAttachments[0].storeAction = MTLStoreActionStore;
                    id<MTLRenderCommandEncoder> e = [commands renderCommandEncoderWithDescriptor:desc];
                    setReduce(e, backgroundColor, backgroundSmall, copySmall);
                    [e setFragmentTexture:backgroundColor atIndex:0];
                    [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                    [e endEncoding];
                }
                const bool bloomOn = bloom.enabled && maskPipeline && blurPipeline && temporalPipeline && compositePipeline;
                auto doBloom = [&]
                {
                    struct
                    {
                        float threshold, zA, zB, temporal;
                        float maskColor[4];
                        float color[4];
                        float texel[2];
                        float pad[2];
                    } bp = {bloom.threshold, bloom.zA, bloom.zB, bloom.temporal,
                            {bloom.maskColor[0], bloom.maskColor[1], bloom.maskColor[2], 1.0f},
                            {bloom.color[0], bloom.color[1], bloom.color[2], bloom.color[3]}, {0, 0}, {0, 0}};
                    auto quad = [&](id<MTLTexture> target, id<MTLRenderPipelineState> state, bool load, auto bind)
                    {
                        MTLRenderPassDescriptor *desc = [MTLRenderPassDescriptor renderPassDescriptor];
                        desc.colorAttachments[0].texture = target;
                        desc.colorAttachments[0].loadAction = load ? MTLLoadActionLoad : MTLLoadActionDontCare;
                        desc.colorAttachments[0].storeAction = MTLStoreActionStore;
                        id<MTLRenderCommandEncoder> e = [commands renderCommandEncoderWithDescriptor:desc];
                        [e setRenderPipelineState:state];
                        bind(e);
                        [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                        [e endEncoding];
                    };
                    static const bool oldBloom = std::getenv("SOTC_BLOOM_OLD") != nullptr; // the earlier approximation, to compare
                    if (!oldBloom && maskGsPipeline && blur12Pipeline && compositeGsPipeline && temporalGsPipeline && copySmall)
                    {
                        // As the GS chain does it, frame after frame: mask -> 256 -> 128 ->
                        // 64; lerp with the history (the last frame's blurred result) by
                        // `temporal`; the history = that blurred at 64; the output = it
                        // blurred at 128, enlarged to 256, added over the picture. The
                        // history being blurred again every frame is what spreads the glow
                        // far over the sky and makes it swell when the sky comes into view.
                        bp.texel[0] = static_cast<float>(w) / 512.0f;
                        bp.texel[1] = static_cast<float>(h) / 448.0f;
                        quad(gsBloom[0], maskGsPipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:depth atIndex:0];
                            [e setFragmentBytes:&bp length:sizeof(bp) atIndex:0];
                        });
                        quad(gsBloom[1], copySmall, false, [&](id<MTLRenderCommandEncoder> e) { [e setFragmentTexture:gsBloom[0] atIndex:0]; });
                        quad(gsBloom[2], copySmall, false, [&](id<MTLRenderCommandEncoder> e) { [e setFragmentTexture:gsBloom[1] atIndex:0]; });
                        const float alpha = gsBloomHistory ? bloom.temporal : 0.0f;
                        quad(gsBloom[3], temporalGsPipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:gsBloom[2] atIndex:0];
                            [e setFragmentTexture:gsBloom[4] atIndex:1];
                            [e setFragmentBytes:&alpha length:sizeof(alpha) atIndex:0];
                        });
                        const float r64 = 64.0f, r128 = 128.0f;
                        quad(gsBloom[4], blur12Pipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:gsBloom[3] atIndex:0];
                            [e setFragmentBytes:&r64 length:sizeof(r64) atIndex:0];
                        });
                        gsBloomHistory = true;
                        quad(gsBloom[5], blur12Pipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:gsBloom[3] atIndex:0];
                            [e setFragmentBytes:&r128 length:sizeof(r128) atIndex:0];
                        });
                        quad(gsBloom[6], copySmall, false, [&](id<MTLRenderCommandEncoder> e) { [e setFragmentTexture:gsBloom[5] atIndex:0]; });
                        quad(color, compositeGsPipeline, true, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:gsBloom[6] atIndex:0];
                            [e setFragmentBytes:&bp length:sizeof(bp) atIndex:0];
                        });
                        return;
                    }
                    const float lw = static_cast<float>(bloomTex[0].width), lh = static_cast<float>(bloomTex[0].height);
                    quad(bloomTex[0], maskPipeline, false, [&](id<MTLRenderCommandEncoder> e)
                    {
                        [e setFragmentTexture:depth atIndex:0];
                        [e setFragmentBytes:&bp length:sizeof(bp) atIndex:0];
                    });
                    // The GS chain reduces the mask in stages, then blurs it once.
                    const int rounds = std::clamp(static_cast<int>(native_renderer::tuning().bloomRounds + 0.5f), 1, 8); // F10 panel (SOTC_BLOOM_ROUNDS)
                    const float spreadGrow = native_renderer::tuning().bloomSpread;
                    for (int i = 0; i < rounds; ++i)
                    {
                        const float spread = 1.0f + spreadGrow * static_cast<float>(i);
                        // In PS2 pixels: the texture grows with the render scale.
                        const float k = spread * static_cast<float>(w) / 512.0f;
                        const float stepH[2] = {k / lw, 0.0f}, stepV[2] = {0.0f, k / lh};
                        quad(bloomTex[1], blurPipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:bloomTex[0] atIndex:0];
                            [e setFragmentBytes:stepH length:sizeof(stepH) atIndex:0];
                        });
                        quad(bloomTex[0], blurPipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:bloomTex[1] atIndex:0];
                            [e setFragmentBytes:stepV length:sizeof(stepV) atIndex:0];
                        });
                    }
                    // With the last frame's: bloomTex[1] = mix(history, current), kept as history.
                    // The GS draws last frame's bloom over this one's with alpha
                    // `temporal` (0x7d: 98% history): the glow eases in and out.
                    const float amount = bloomHistory ? 1.0f - bloom.temporal : 1.0f;
                    quad(bloomTex[1], temporalPipeline, false, [&](id<MTLRenderCommandEncoder> e)
                    {
                        [e setFragmentTexture:bloomTex[0] atIndex:0];
                        [e setFragmentTexture:bloomTex[2] atIndex:1];
                        [e setFragmentBytes:&amount length:sizeof(amount) atIndex:0];
                    });
                    std::swap(bloomTex[1], bloomTex[2]);
                    bloomHistory = true;
                    quad(color, compositePipeline, true, [&](id<MTLRenderCommandEncoder> e)
                    {
                        [e setFragmentTexture:bloomTex[2] atIndex:0];
                        [e setFragmentBytes:&bp length:sizeof(bp) atIndex:0];
                    });
                };
                auto doBlur = [&](const native_renderer::ScreenBlur &b)
                {
                    if (!blurCopy || b.steps <= 0)
                        return;
                    id<MTLBlitCommandEncoder> blit = [commands blitCommandEncoder];
                    [blit copyFromTexture:color toTexture:snapshot];
                    [blit endEncoding];
                    MTLRenderPassDescriptor *desc = [MTLRenderPassDescriptor renderPassDescriptor];
                    desc.colorAttachments[0].texture = color;
                    desc.colorAttachments[0].loadAction = MTLLoadActionLoad;
                    desc.colorAttachments[0].storeAction = MTLStoreActionStore;
                    id<MTLRenderCommandEncoder> e = [commands renderCommandEncoderWithDescriptor:desc];
                    [e setRenderPipelineState:blurCopy];
                    [e setFragmentTexture:snapshot atIndex:0];
                    for (int i = 0; i < b.steps; ++i)
                    {
                        const float f = static_cast<float>(b.steps - i) / static_cast<float>(b.steps);
                        struct
                        {
                            float scale, angle, alpha, pad;
                            float offset[2];
                        } u = {(b.scale - 1.0f) * f + 1.0f, std::fabs(b.angle) >= 0.01570796f ? -b.angle * f : 0.0f, 1.0f / static_cast<float>(i + 1), 0.0f,
                               {b.dx * f / 256.0f, -b.dy * f / 224.0f}}; // PS2 pixels
                        [e setVertexBytes:&u length:sizeof(u) atIndex:0];
                        [e setFragmentBytes:&u length:sizeof(u) atIndex:0];
                        [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
                    }
                    [e endEncoding];
                };
                // bossDeathEffect (layer 8): the layer is drawn apart, on the
                // scene's depth, into a buffer cleared to nothing; reduced to 256,
                // 128 and 64 and blurred, back to 256; then laid over the picture
                // twice, sharp (amountA) and blurred (amountB), in the tint.
                auto doGlow = [&]
                {
                    pass(glowLayer, false, glow.order, glow.order + 1u, true, false, true);
                    if (!glowOver || !glowReducePipeline)
                        return;
                    auto quad = [&](id<MTLTexture> target, id<MTLRenderPipelineState> state, bool load, auto bind)
                    {
                        MTLRenderPassDescriptor *desc = [MTLRenderPassDescriptor renderPassDescriptor];
                        desc.colorAttachments[0].texture = target;
                        desc.colorAttachments[0].loadAction = load ? MTLLoadActionLoad : MTLLoadActionDontCare;
                        desc.colorAttachments[0].storeAction = MTLStoreActionStore;
                        id<MTLRenderCommandEncoder> e = [commands renderCommandEncoderWithDescriptor:desc];
                        [e setRenderPipelineState:state];
                        bind(e);
                        [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                        [e endEncoding];
                    };
                    auto reduce = [&](id<MTLTexture> from, id<MTLTexture> to, float shift)
                    {
                        const float offset[2] = {shift / static_cast<float>(from.width), shift / static_cast<float>(from.height)};
                        quad(to, glowReducePipeline, false, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:from atIndex:0];
                            if (shift != 0.0f || !setReduce(e, from, to, glowReducePipeline))
                                [e setFragmentBytes:offset length:sizeof(offset) atIndex:0];
                        });
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
                    static const int show = std::getenv("SOTC_GLOW_SHOW") ? std::atoi(std::getenv("SOTC_GLOW_SHOW")) : 0; // debugging: 1 the layer, 2 the blurred
                    if (show)
                    {
                        quad(color, copySmall, false, [&](id<MTLRenderCommandEncoder> e) { [e setFragmentTexture:show == 1 ? glowLayer : glowSmall[0] atIndex:0]; });
                        return;
                    }
                    const std::pair<id<MTLTexture>, float> layers[2] = {{glowLayer, glow.amountA}, {glowSmall[0], glow.amountB}};
                    for (const auto &[source, amount] : layers)
                    {
                        if (!(amount > 0.0f))
                            continue;
                        const float tint[4] = {glow.tint[0], glow.tint[1], glow.tint[2], amount};
                        quad(color, glowOver, true, [&](id<MTLRenderCommandEncoder> e)
                        {
                            [e setFragmentTexture:source atIndex:0];
                            [e setFragmentBytes:tint length:sizeof(tint) atIndex:0];
                        });
                    }
                };
                auto doShadow = [&]
                {
                    // MSAA: the volumes' count is in the multisampled stencil; the mask resolved.
                    const bool ms = samples > 1u && msp.shadowMask;
                    MTLRenderPassDescriptor *desc = [MTLRenderPassDescriptor renderPassDescriptor];
                    desc.colorAttachments[0].texture = ms ? shadowMaskMS : shadowMask;
                    desc.colorAttachments[0].loadAction = MTLLoadActionClear;
                    desc.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
                    desc.colorAttachments[0].storeAction = MTLStoreActionStore;
                    if (ms)
                    {
                        desc.colorAttachments[0].resolveTexture = shadowMask;
                        desc.colorAttachments[0].storeAction = MTLStoreActionMultisampleResolve;
                    }
                    desc.depthAttachment.texture = ms ? depthMS : depth;
                    desc.depthAttachment.loadAction = MTLLoadActionLoad;
                    desc.depthAttachment.storeAction = MTLStoreActionStore;
                    desc.stencilAttachment.texture = ms ? depthMS : depth;
                    desc.stencilAttachment.loadAction = MTLLoadActionLoad;
                    desc.stencilAttachment.storeAction = MTLStoreActionStore;
                    id<MTLRenderCommandEncoder> e = [commands renderCommandEncoderWithDescriptor:desc];
                    [e setRenderPipelineState:ms ? msp.shadowMask : shadowMaskPipeline];
                    [e setDepthStencilState:shadowedState]; // where the count is not 0; clears it
                    [e setStencilReferenceValue:0];
                    [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                    [e endEncoding];
                    auto quad = [&](id<MTLTexture> target, id<MTLRenderPipelineState> state, bool load, auto bind)
                    {
                        MTLRenderPassDescriptor *qd = [MTLRenderPassDescriptor renderPassDescriptor];
                        qd.colorAttachments[0].texture = target;
                        qd.colorAttachments[0].loadAction = load ? MTLLoadActionLoad : MTLLoadActionDontCare;
                        qd.colorAttachments[0].storeAction = MTLStoreActionStore;
                        id<MTLRenderCommandEncoder> q = [commands renderCommandEncoderWithDescriptor:qd];
                        [q setRenderPipelineState:state];
                        bind(q);
                        [q drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                        [q endEncoding];
                    };
                    // bloomTex[0..1] are free outside the bloom.
                    quad(bloomTex[0], shadowReducePipeline, false, [&](id<MTLRenderCommandEncoder> q) { [q setFragmentTexture:shadowMask atIndex:0]; });
                    static const int rounds = std::getenv("SOTC_SHADOW_BLUR") ? std::atoi(std::getenv("SOTC_SHADOW_BLUR")) : 1;
                    const float lw = static_cast<float>(bloomTex[0].width), lh = static_cast<float>(bloomTex[0].height);
                    for (int i = 0; i < rounds; ++i)
                    {
                        const float k = static_cast<float>(w) / 512.0f; // PS2 pixels
                        const float stepH[2] = {k / lw, 0.0f}, stepV[2] = {0.0f, k / lh};
                        quad(bloomTex[1], blurPipeline, false, [&](id<MTLRenderCommandEncoder> q)
                        {
                            [q setFragmentTexture:bloomTex[0] atIndex:0];
                            [q setFragmentBytes:stepH length:sizeof(stepH) atIndex:0];
                        });
                        quad(bloomTex[0], blurPipeline, false, [&](id<MTLRenderCommandEncoder> q)
                        {
                            [q setFragmentTexture:bloomTex[1] atIndex:0];
                            [q setFragmentBytes:stepV length:sizeof(stepV) atIndex:0];
                        });
                    }
                    quad(color, shadowSoftPipeline, true, [&](id<MTLRenderCommandEncoder> q)
                    {
                        [q setFragmentTexture:bloomTex[0] atIndex:0];
                        [q setFragmentBytes:&softAmount length:sizeof(softAmount) atIndex:0];
                    });
                };
                // waterRefractionBegin (layer 9): the picture so far reduced to
                // 256 x 256, to 128 x 128 and back (bilinear: a blur), which the
                // water surfaces then show at their screen position.
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
                    if (!copySmall)
                        return;
                    id<MTLBlitCommandEncoder> blit = [commands blitCommandEncoder];
                    [blit copyFromTexture:color toTexture:snapshot];
                    [blit endEncoding];
                    auto copy = [&](id<MTLTexture> from, id<MTLTexture> to)
                    {
                        MTLRenderPassDescriptor *qd = [MTLRenderPassDescriptor renderPassDescriptor];
                        qd.colorAttachments[0].texture = to;
                        qd.colorAttachments[0].loadAction = MTLLoadActionDontCare;
                        qd.colorAttachments[0].storeAction = MTLStoreActionStore;
                        id<MTLRenderCommandEncoder> q = [commands renderCommandEncoderWithDescriptor:qd];
                        setReduce(q, from, to, copySmall);
                        [q setFragmentTexture:from atIndex:0];
                        [q drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                        [q endEncoding];
                    };
                    copy(snapshot, refractionColor);
                    copy(refractionColor, refractionHalf);
                    copy(refractionHalf, refractionColor);
                };
                auto doDof = [&]
                {
                    if (!copySmall || !dofPipeline)
                        return;
                    id<MTLBlitCommandEncoder> blit = [commands blitCommandEncoder];
                    [blit copyFromTexture:color toTexture:snapshot];
                    [blit endEncoding];
                    auto quad = [&](id<MTLTexture> to, id<MTLRenderPipelineState> state, bool load, auto bind)
                    {
                        MTLRenderPassDescriptor *qd = [MTLRenderPassDescriptor renderPassDescriptor];
                        qd.colorAttachments[0].texture = to;
                        qd.colorAttachments[0].loadAction = load ? MTLLoadActionLoad : MTLLoadActionDontCare;
                        qd.colorAttachments[0].storeAction = MTLStoreActionStore;
                        id<MTLRenderCommandEncoder> q = [commands renderCommandEncoderWithDescriptor:qd];
                        [q setRenderPipelineState:state];
                        bind(q);
                        [q drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                        [q endEncoding];
                    };
                    // refractionColor / refractionHalf are free after layer 9.
                    quad(refractionColor, copySmall, false, [&](id<MTLRenderCommandEncoder> q)
                    {
                        setReduce(q, snapshot, refractionColor, copySmall);
                        [q setFragmentTexture:snapshot atIndex:0];
                    });
                    for (int i = 0; i < dof.rounds; ++i)
                    {
                        quad(refractionHalf, copySmall, false, [&](id<MTLRenderCommandEncoder> q) { [q setFragmentTexture:refractionColor atIndex:0]; });
                        quad(refractionColor, copySmall, false, [&](id<MTLRenderCommandEncoder> q) { [q setFragmentTexture:refractionHalf atIndex:0]; });
                    }
                    struct
                    {
                        float zA, zB;
                        uint32_t focusZ;
                        int32_t slices;
                    } p = {dof.zA, dof.zB, dof.focusZ, dof.slices};
                    quad(color, dofPipeline, true, [&](id<MTLRenderCommandEncoder> q)
                    {
                        [q setFragmentTexture:refractionColor atIndex:0];
                        [q setFragmentTexture:depth atIndex:1];
                        [q setFragmentBytes:&p length:sizeof(p) atIndex:0];
                    });
                };
                // zFog: the background over the picture by depth slices (layer 4).
                const bool fogComposite = fogOn && fogPipeline && fog.steps > 0;
                auto doFog = [&]
                {
                    MTLRenderPassDescriptor *qd = [MTLRenderPassDescriptor renderPassDescriptor];
                    qd.colorAttachments[0].texture = color;
                    qd.colorAttachments[0].loadAction = MTLLoadActionLoad;
                    qd.colorAttachments[0].storeAction = MTLStoreActionStore;
                    id<MTLRenderCommandEncoder> q = [commands renderCommandEncoderWithDescriptor:qd];
                    [q setRenderPipelineState:fogPipeline];
                    [q setFragmentTexture:backgroundSmall atIndex:0];
                    [q setFragmentTexture:depth atIndex:1];
                    const float p[4] = {fog.start, fog.width, static_cast<float>(fog.steps), 0.0f};
                    [q setFragmentBytes:p length:sizeof(p) atIndex:0];
                    [q drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
                    [q endEncoding];
                };
                // The 3D and 2D by layer, cut where an effect reads the picture so far.
                struct Event
                {
                    uint32_t order;
                    int blur; // -1: the bloom, -2: the glow composite, -3: the shadows, -4: the refraction copy, -5: the depth of field
                };
                std::vector<Event> events;
                if (dof.enabled && dof.slices > 0)
                    events.push_back({dof.order, -5});
                if (fogComposite)
                    events.push_back({fog.order, -6});
                if (softShadows)
                    events.push_back({softOrder, -3});
                if (haveRefraction)
                    events.push_back({9u, -4});
                if (screenCopyOrder) // the same reduced copy, later (refractionColor is free again)
                    events.push_back({screenCopyOrder, -4});
                if (glow.enabled)
                    events.push_back({glow.order, -2});
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
                    if (ev.blur != -4) // every effect but the refraction copy draws on the resolved picture
                        colorDirty = true;
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
                {
                    id<MTLBlitCommandEncoder> keep = [commands blitCommandEncoder];
                    [keep copyFromTexture:color toTexture:previousColor];
                    [keep endEncoding];
                }
                [commands commit];
                [commands waitUntilCompleted];
                std::lock_guard<std::mutex> lock(outputMutex);
                output.resize(static_cast<size_t>(w) * h * 4u);
                [color getBytes:output.data() bytesPerRow:w * 4u fromRegion:MTLRegionMake2D(0, 0, w, h) mipmapLevel:0];
                outputWidth = w;
                outputHeight = h;
                hasOutput = true;
            }
        }
    };

    class MetalBackend final : public native_renderer::detail::Backend
    {
    public:
        explicit MetalBackend(Renderer *renderer) : r(renderer) {}
        bool hasMesh(uint64_t id) override { return r->meshes.count(id) != 0; }
        void uploadMesh(uint64_t id, const native_renderer::Vertex *vertices, uint32_t count, bool) override
        {
            Mesh &mesh = r->meshes[id];
            [mesh.buffer release];
            mesh.buffer = count ? [r->device newBufferWithBytes:vertices length:count * sizeof(native_renderer::Vertex) options:MTLResourceStorageModeShared] : nil;
            mesh.count = count;
        }
        bool hasTexture(uint64_t id) override { return r->textures.count(id) != 0; }
        void uploadTexture(uint64_t key, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels) override
        {
            if (!width || !height)
                return;
            levels = std::max(1u, levels);
            MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:width height:height mipmapped:levels > 1u];
            desc.mipmapLevelCount = levels;
            desc.usage = MTLTextureUsageShaderRead;
            desc.storageMode = MTLStorageModeShared;
            id<MTLTexture> texture = [r->device newTextureWithDescriptor:desc];
            for (uint32_t level = 0, w = width, h = height; level < levels; ++level)
            {
                [texture replaceRegion:MTLRegionMake2D(0, 0, w, h) mipmapLevel:level withBytes:rgba bytesPerRow:w * 4u];
                rgba += static_cast<size_t>(w) * h * 4u;
                w = std::max(1u, w / 2u);
                h = std::max(1u, h / 2u);
            }
            auto it = r->textures.find(key);
            if (it != r->textures.end())
                [it->second release];
            r->textures[key] = texture;
        }
        void beginFrame() override
        {
            r->draws.clear();
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
        void endFrame(uint32_t width, uint32_t height) override { r->render(width, height); }
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
        Renderer *r;
    };
}

namespace native_renderer::detail
{
    Backend *createMetalBackend()
    {
        auto *renderer = new Renderer();
        if (!renderer->init())
        {
            delete renderer;
            return nullptr;
        }
        return new MetalBackend(renderer);
    }
}
