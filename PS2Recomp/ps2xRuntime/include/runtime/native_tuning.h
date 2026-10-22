#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Live adjustments of the native renderer's effects: read every frame by the
// renderer and the game's hooks, changed by the tuning panel (F10). The
// strengths multiply the game's own, which it keeps changing by scene and
// camera; the defaults were set by eye against the PS2 (2026-10-17). The
// SOTC_* variables set them at start. Plain fields: the panel and the renderer may race on a value for
// a frame, which only delays the change.
namespace native_renderer
{
    struct Tuning
    {
        float renderScale = 1.0f;   // the picture drawn at 512 x 448 times this, 0.5 to 4 (SOTC_RENDER_SCALE; 3 on the Mac)
        // Quality (the menus' Quality group). The graphics preset sets these
        // and the render scale together; `preset` is the one they match
        // (kPresetCustom: none), kept up to date by the menus.
        float preset = 2.0f;        // SOTC_PRESET (0 Low .. 3 Ultra); High on the Mac, Medium on the Vita
        float msaa = 0.0f;          // Metal: 0 off, 1 2x, 2 4x, 3 8x samples a pixel, as far as the GPU has them (SOTC_MSAA=0/2/4/8)
        float textureFilter = 2.0f; // 0 nearest, 1 bilinear, 2 trilinear at the game's mip level, 3-6 anisotropic 2x-16x (Metal) (SOTC_TEXTURE_FILTER)
        float modelDetail = 0.0f;   // 0 the game's level of detail, 1 one level finer, 2 always the finest (SOTC_MODEL_DETAIL)
        float effectDetail = 0.0f;  // Metal: 0 the PS2's effect buffers; 1 reductions averaged over their footprint, the mirror 512; 2 the mirror 1024 (SOTC_EFFECT_DETAIL)
        bool bloom = true;          // SOTC_BLOOM_SCALE=0 turns it off
        float bloomStrength = 1.0f; // x the game's add strength (SOTC_BLOOM_SCALE); 1 = the PS2 (measured 2026-10-19 against the GS chain)
        float bloomRounds = 1.0f;   // blur rounds of +-3 texels at a quarter size, whole (SOTC_BLOOM_ROUNDS)
        float bloomSpread = 1.0f;   // each further round's growth (SOTC_BLOOM_SPREAD)
        bool vertexFog = true;      // the GS fog by distance (SOTC_NATIVE_NOVFOG)
        float vertexFogAmount = 0.55f; // x the fog's share (0: none, 1: the game's; SOTC_FOG_AMOUNT)
        bool zFog = true;           // fade into the sky by depth (SOTC_NATIVE_ZFOG=0)
        bool glow = true;           // layer 8 glow (SOTC_NATIVE_NOGLOW)
        bool depthOfField = true;   // SOTC_NATIVE_NODOF
        bool shadows = true;        // SOTC_NATIVE_NOSHADOWS
        bool particles = true;      // SOTC_NATIVE_NOPTCL
        bool screenBlur = true;     // SOTC_NATIVE_NOSBLUR
        bool feedback = true;       // feedbackBlur: the last frame laid over the new one (SOTC_NATIVE_NOFEEDBACK)
        bool reflections = true;    // SOTC_NATIVE_NOREFLECT
        bool refraction = true;     // SOTC_NATIVE_NOREFRACT

        // Debugging (the menus' Debug group): what to draw at all.
        uint32_t hiddenLayers = 0;  // bit n: the display list's layer n is not drawn
        bool showNative = true;     // off: the emulated GS picture of the same moment is shown (the native one still drawn)
        bool draw2D = true;         // sprites, fades, menus, subtitles
        bool drawWorld = true;      // everything that is not 2D
        bool textures = true;       // off: every draw untextured (its vertex colour)
        bool lighting = true;       // off: lit models at their vertex colour
        bool additive = true;       // draws that add (glows, light shafts)
        bool subtract = true;       // draws that subtract
        bool overlays = true;       // second passes over the same geometry
        bool rawMirror = false;     // water shows the mirror image untinted (what the reflection holds)
        bool skyImage = true;       // the sky image laid behind the scene (bgEnd's copy)
        bool mipmaps = true;        // SOTC_NATIVE_NOMIPS
        bool textureScroll = true;  // VIF ROW scrolling (SOTC_NATIVE_NOROW)
        // The picture shown at most 30 times a second, evenly (SOTC_CAP30).
        // Off by default: above 30 fps (logos, menus, loading) the game
        // thread is held back, and with it every guest thread (in Vita3K's
        // speed mode the boot's loading screen took ~170 s instead of ~5).
        // With the render thread the swap is that thread's, but the game
        // still waits for it when a whole frame ahead.
        bool frameCap30 = false;
        // OpenGL / Vita: drawing and showing a frame on a thread of its own
        // while the game makes the next (at most one frame ahead); off: the
        // game waits for each frame to be drawn (SOTC_RENDER_THREAD=0/1; on
        // by default on the Vita).
        bool renderThread = false;
        bool frameLog = false;      // per-frame timing to the log (SOTC_FRAME_LOG; native_renderer::FrameCounters)
        // The game thread's time by subsystem to the log (SOTC_SUBSYS_PROF;
        // game/overrides/sotc_prof.cpp: wrapped at start only when on then).
        bool subsysProf = false;
        // The lean copies of the hot recompiled functions in the function
        // table (game/overrides/lean_hot.cpp; put in at start when SOTC_LEAN
        // is on, the Vita's default; this flips them at run time;
        // SOTC_LEAN=2 alternates).
        bool leanCopies = true;
        // The game hooks' faster paths (skinned meshes posed once per vertex
        // and written in place, a texture's and a packet's id sampled once a
        // frame); off: the earlier code, to compare (SOTC_NATIVE_SLOWHOOKS).
        bool fastHooks = true;
        // With fastHooks: a model packet the game has never rewritten in
        // place is taken as it was while it is drawn frame after frame (four
        // of its words checked when it comes back after a gap), its full
        // sample (33 words) taken one frame in 16; a packet seen rewritten
        // keeps the full sample every frame (SOTC_PACKET_QUICK=0/1).
        bool packetQuickCheck = true;
        // The game's own PS2 display lists for what the native renderer
        // draws: modelDisp's VIF packets and texture requests, the screen
        // effects' sprites (spriteTexFast). Off: only what the rest of the
        // game reads back is kept (SOTC_PS2_LISTS=1: built as before). They
        // are built anyway while the GS draws (the emulated picture shown).
        bool ps2Lists = false;
        // OpenGL / Vita: skinned meshes posed by the GPU (the bind pose kept,
        // the bone palette sent per draw) instead of on the CPU (SOTC_GPU_SKIN=0/1).
        bool gpuSkinning = true;
        // What the heat haze (layer 23) and the water (layer 9) read as "the
        // picture so far" on the OpenGL renderer: 0 the 256 x 256 copy made at
        // their layer (the PS2's way), 1 a full-size copy made there, 2 the
        // last frame shown. On the Vita a copy made mid-frame came out white.
        float hazeSource = 0.0f;    // SOTC_HAZE_SOURCE
        float waterSource = 0.0f;   // SOTC_WATER_SOURCE
        // OpenGL: an effect that reads the picture so far (water refraction,
        // heat haze, screen blur) gets that picture drawn again into a target
        // of its own instead of the picture being left and taken up again
        // (which lost it in Vita3K: white screens). 0 off, 1 on, 2 only when
        // running in an emulator (native_renderer::inEmulator: Vita3K; the
        // Vita's default: on the console the copies are made at the layer,
        // without drawing the world again). SOTC_GL_REDRAW=0/1/2.
        float midFrameRedraw = 0.0f;
        // OpenGL / Vita: every shader program drawn once with every blend
        // state the renderer uses when it starts (vitaGL patches a fragment
        // program per blend state; the first use mid-game made the patch then).
        // Read at the renderer's start (SOTC_GL_WARM=0/1; on by default on the Vita).
        bool warmShaders = false;
        // Texture decoding and model mesh building on a worker thread
        // (native_renderer::buildAsync) instead of inline in the game's
        // hooks: a scene cut no longer stalls the game thread for them; what
        // is not ready is left out of a frame or two. On by default on the
        // Vita (SOTC_ASYNC_BUILDS=0/1).
        bool asyncBuilds = false;
        // With asyncBuilds: a frame that had to leave out more than an eighth
        // of its draws (their meshes or textures still on the worker, at a
        // camera cut) is not shown; the last one stays up instead, at most 3
        // frames in a row (SOTC_HOLD_INCOMPLETE=0/1; on by default on the Vita).
        bool holdIncomplete = false;
        // OpenGL / Vita, the render thread's cost per draw:
        // kept meshes packed into a few shared vertex buffers (attributes set
        // once per buffer, not per mesh; SOTC_GL_POOLS=0/1, read when a mesh
        // is uploaded);
        bool glMeshPools = true;
        // a run of draws with the same state and matrix whose meshes share a
        // buffer drawn as one (a range, or indices made per frame;
        // SOTC_GL_MERGE=0/1);
        bool glMergeDraws = true;
        // the plain world draws (textured or not, no mirror, refraction or
        // copies) through programs that read only the textures they use (vitaGL
        // sets every sampler of a program at each draw; SOTC_GL_LEAN=0/1);
        bool glLeanPrograms = true;
        // with glLeanPrograms: draws whose texture and vertex alphas cannot
        // fall under the cut-out (0.02) through a variant without `discard`
        // (the same picture; on the Vita's GPU a discard costs its hidden
        // surface removal; SOTC_GL_NODISCARD=0/1).
        bool glNoDiscard = true;
    };

    // The settings (made from the environment on first use). Inline: the
    // game's hooks ask for them hundreds of times a frame.
    extern Tuning *g_tuning;
    Tuning &tuningFirst();
    inline Tuning &tuning() { return g_tuning ? *g_tuning : tuningFirst(); }
    // This platform's defaults (no environment).
    Tuning defaultTuning();
    // The values that differ from the defaults, as environment variables
    // (to keep a setting, or to tell it).
    std::string tuningAsEnv();

    // The adjustments as one list, for every menu that shows them: the F10
    // panel (ImGui, the Mac) and the player menu's Renderer tab (the Vita's
    // buttons too), so the two stay mirrors. Saved with the player settings
    // under `key`; a SOTC_* variable set at start wins over the saved value.
    struct TuningItem
    {
        const char *key;
        const char *label, *labelPt;
        const char *help, *helpPt;
        bool Tuning::*toggle = nullptr; // a switch, or
        float Tuning::*number = nullptr; // a value from min to max by step
        float min = 0.0f, max = 1.0f, step = 0.05f;
        const char *env = nullptr;      // the variable that sets it at start
        uint32_t Tuning::*mask = nullptr; // or one bit of a mask: on = the bit clear
        int bit = 0;
        bool debug = false;             // listed in the Debug group
        bool quality = false;           // listed in the Quality group
        uint8_t backends = 3;           // bit 0: Metal, bit 1: OpenGL (the Vita) - where it changes anything
    };
    const std::vector<TuningItem> &tuningItems();
    // The item's value as text ("On", "0.46", "3x"), in Portuguese when asked.
    std::string tuningValueText(const TuningItem &item, bool portuguese = false);
    // Named values (the preset, filtering, MSAA...): how many there are
    // (0: a plain number or switch), for the F10 panel's lists.
    int tuningChoices(const TuningItem &item);
    // One step up (dir > 0) or down; a switch flips. The preset item applies
    // its preset; any other quality item makes the preset follow (Custom).
    void tuningStep(const TuningItem &item, int dir);
    // After a menu changed a value in place (the F10 panel): the preset item
    // applies its preset, the others update the preset shown.
    void tuningChanged(const TuningItem &item);
    // False when the item changes nothing on the renderer in use (an
    // OpenGL-only switch on Metal...); the menus say so.
    bool tuningApplies(const TuningItem &item);
    // "Metal" or "OpenGL".
    const char *backendName();

    enum Preset
    {
        kPresetLow,
        kPresetMedium,
        kPresetHigh,
        kPresetUltra,
        kPresetCustom
    };
    // Sets the render scale and the quality items to a preset's values.
    void applyPreset(Tuning &t, int preset);
    // The preset these values match, or kPresetCustom.
    int matchingPreset(const Tuning &t);
    // A saved value (0/1 for a switch); false when the key is unknown or
    // its variable was set at start.
    bool tuningRestore(const char *key, float value);
}
