#pragma once

// Native renderer: a game override hands it meshes once and, every frame, a
// list of draws (mesh + matrix). It draws them on the host GPU (Metal on
// macOS), with no PS2 hardware in between. The picture can replace the
// emulated one on screen (native_renderer::setShown).

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace native_renderer
{
    struct Vertex
    {
        float x, y, z;    // model space
        float nx, ny, nz; // normal (lit draws), model space
        float u, v;       // texture coordinates (repeat)
        uint8_t r, g, b, a; // 0x80 = 1.0, as the PS2's
    };

    bool available();
    // OpenGL backends: makes the renderer's context current on the calling
    // thread (the game thread, where every call below comes from).
    void setGlContext(std::function<void()> makeCurrent);
    // OpenGL backends: hands each finished picture's GL texture (y-up) to the
    // host instead of reading it back (the Vita: Vita3K's CPU reads of render
    // targets do not see the GPU's output). Called from endFrame's thread.
    void setPresentHook(std::function<void(uint32_t texture, uint32_t width, uint32_t height)> hook);

    bool hasMesh(uint64_t id);
    // A triangle list, kept on the GPU under `id` until the renderer goes.
    // perFrame: made again every frame (skinned, 2D, particles...): kept only
    // for this frame (one shared buffer per frame on GL).
    void uploadMesh(uint64_t id, const Vertex *vertices, uint32_t count, bool perFrame = false);

    bool hasTexture(uint64_t id);
    // RGBA8 (alpha 0x80 = opaque, as the PS2's), kept on the GPU under `id`.
    // `levels` mip levels, each half the last, one after the other in `rgba`.
    void uploadTexture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels = 1);

    // Draws of one frame: `clip` (column-major) takes model space to clip
    // space with x, y and w as the game's projection gives them (x/w, y/w in
    // -1..1, y up); depth comes from w (near plane at w = 0.1).
    void beginFrame();
    enum class Blend : uint8_t
    {
        Normal,   // Cs x As + Cd x (1 - As), alpha = texture x vertex (0x80 = 1)
        Additive, // Cs x As + Cd
        Opaque,   // no blending (alpha still cuts out)
        Subtract, // Cd - Cs x As
    };
    struct DrawState
    {
        uint64_t texture = 0; // 0: none
        // The GS's CLAMP (WMS / WMT): bit 0 clamps u to the texture's edge, bit 1
        // v; otherwise the texture repeats.
        uint8_t clamp = 0;
        Blend blend = Blend::Normal;
        uint32_t order = 0;   // draws go in this order (stable), like the game's display list layers
        // Lit draws (the VU1's lit microcode): d = max(0, L x (n, 1)), with
        // L the columns light[0..15]; colour = C x (d.xyz, 1), C the columns
        // light[16..31] (three light colours, then ambient); it scales the
        // vertex colour.
        float opacity = 1.0f; // scales the alpha
        float uvOffset[2] = {0.0f, 0.0f}; // added to the texture coordinates (VIF ROW scrolling)
        // Mip level as the GS picks it (TEX1): log2(w) x lodScale + lodK, up
        // to maxLod; maxLod <= 0: level 0.
        float lodK = 0.0f, lodScale = 1.0f, maxLod = 0.0f;
        bool background = false; // also drawn into the background image (what distance fades to)
        bool fogged = false;     // fades to the background image with distance (setFog)
        bool writeDepth = false; // write depth even when additive (a second pass follows)
        // No depth write at all: the GS's TEST AFAIL FB_ONLY with every pixel
        // under AREF (SotC's water: vertex alpha 0x20 < 0x40), so what is drawn
        // after it on or just under the surface (the rings) is not hidden.
        bool noDepthWrite = false;
        bool overlay = false; // a second pass over the same geometry: depth equal passes, no depth write
        bool screen = false; // 2D: positions already in clip space, no depth test or write
        // A shadow volume: no colour; its faces count on the stencil (front +1,
        // back -1) where they pass the depth test, and before the first draw
        // of a later layer the pixels with a non-zero count are darkened by
        // `shadow` (0..1).
        bool shadowVolume = false;
        float shadow = 0.5f;
        // Planar reflections: reflectionSource draws (the game's mirrored
        // geometry) go to an off-screen image; a reflectionSample draw shows
        // that image at its own screen position, times reflectionTint
        // (0..1, alpha the blend amount), instead of its texture.
        bool reflectionSource = false;
        bool reflectionSample = false;
        float reflectionTint[4] = {1, 1, 1, 1};
        // Water refraction: shows the picture as it was before layer 9,
        // reduced and blurred, at its own screen position, times
        // reflectionTint (the same packet's).
        bool refractionSample = false;
        // SotC's water microcode 1: the vertex alpha times 0.2 + 0.8 (1 - |n . v|),
        // v the normalised camera-space position, n the model's up axis, with
        // light[0..15] the model-view matrix (columns) instead of the lights.
        bool fresnel = false;
        // reflectionBegin's ripple of the mirror image (t, H; H = 0: none).
        float mirrorRipple[2] = {0.0f, 0.0f};
        // A screen-copy draw (SotC's 0x10f37c8 at layer 23): its texture is
        // the picture before its layer reduced to 256 x 256, at its own UV.
        bool screenCopy = false;
        // The last frame shown (SotC's feedbackBlur: the display buffer drawn
        // over the new frame at layer 29), at its own UV, times the colour.
        bool previousFrame = false;
        uint32_t debugColour = 0; // non-zero: drawn flat in this RGB (who drew what)
        bool lit = false;
        float light[32] = {};
    };
    void draw(uint64_t mesh, const float clip[16], const DrawState &state);
    // The next whole frame's draws, one line each, to `path` (F8 in the runtime).
    void requestDrawDump(const std::string &path);
    // SotC's zFog: before layer `order`, every pixel blends towards the
    // background image (bgEnd's reduced copy of layer 1) by the fog amount
    // of its depth w: (i x 128 / steps) / 128 for the last i in 0..steps
    // with w >= start + width x i / steps (empty pixels: all fog). Fogged
    // draws (per draw, the older approximation) use clamp((w - start) /
    // width) in `steps` steps instead.
    struct Fog
    {
        bool enabled = false;
        float start = 0.0f, width = 1.0f;
        int steps = 0; // 0: smooth
        uint32_t order = 4;
        // The GS's per-vertex fog on 3D draws: F = clamp(base + scale / w,
        // 0, 255); colour = mix(fogColour, colour, F / 255).
        bool vertexFog = false;
        float base = 255.0f, scale = 0.0f;
        float colour[3] = {0, 0, 0}; // 0..1
    };
    void setFog(const Fog &fog);

    // SotC's bloom: a mask of what lies beyond `threshold` (GS Z: zA + zB / w;
    // empty pixels are 0), in `maskColor`, blurred at a quarter of the
    // resolution, lerped with the last frame's by `temporal` and added over
    // the 3D (draws before `order`) in `color` (0x80 = 1, alpha the amount).
    struct Bloom
    {
        bool enabled = false;
        uint32_t order = 20;
        float threshold = 0.0f, zA = 0.0f, zB = 0.0f;
        float maskColor[3] = {1, 1, 1};
        float color[4] = {0, 0, 0, 0};
        float temporal = 1.0f; // the history's share: last frame's bloom drawn over this one's with this alpha
    };
    void setBloom(const Bloom &bloom);

    // SotC's screenBlur: at display list layer `order`, the picture so far is
    // drawn over itself `steps` times, copy i scaled from `scale` towards 1,
    // turned by `angle` (radians, the same fraction), shifted by (dx, dy)
    // pixels (same fraction), with alpha 1 / (i + 1): the average of the
    // copies. Cleared each frame by beginFrame.
    struct ScreenBlur
    {
        uint32_t order = 0;
        float angle = 0.0f, scale = 1.0f, dx = 0.0f, dy = 0.0f;
        int steps = 0;
    };
    void addScreenBlur(const ScreenBlur &blur);

    // SotC's bossDeathEffect: the draws of layer `order` go to a buffer of
    // their own (cleared to nothing, on the scene's depth), which is reduced
    // to 256, 128, 64 and blurred by `rounds`, and laid over the picture
    // sharp (alpha amountA) and blurred (amountB), in `tint` (0x80 = 1).
    struct GlowComposite
    {
        bool enabled = false;
        uint32_t order = 8;
        int rounds = 2;
        float tint[3] = {1, 1, 1};
        float amountA = 0.0f, amountB = 0.0f;
    };
    void setGlowComposite(const GlowComposite &glow);

    // SotC's depthOfField (layer 20): the picture reduced to 256 x 256 and
    // blurred `rounds` times (256 -> 128 -> 256) is laid over the pixels
    // beyond the focus in `slices` passes: pass i where the GS Z (zA + zB / w,
    // empty pixels 0) is at most focusZ >> i, alpha (i + 1) / slices.
    struct DepthOfField
    {
        bool enabled = false;
        uint32_t order = 20;
        float zA = 0.0f, zB = 0.0f;
        uint32_t focusZ = 0;
        int slices = 0, rounds = 0;
    };
    void setDepthOfField(const DepthOfField &dof);
    void endFrame(uint32_t width, uint32_t height);

    // Shown instead of the emulated picture (the emulator's present path asks).
    void setShown(bool shown);
    bool shown();
    // The last finished frame (RGBA8); false before the first.
    bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height);
}
