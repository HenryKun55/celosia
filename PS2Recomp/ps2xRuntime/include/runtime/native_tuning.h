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
        // What the heat haze (layer 23) and the water (layer 9) read as "the
        // picture so far" on the OpenGL renderer: 0 the 256 x 256 copy made at
        // their layer (the PS2's way), 1 a full-size copy made there, 2 the
        // last frame shown. On the Vita a copy made mid-frame came out white.
        float hazeSource = 0.0f;    // SOTC_HAZE_SOURCE
        float waterSource = 0.0f;   // SOTC_WATER_SOURCE
    };

    Tuning &tuning();
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
    };
    const std::vector<TuningItem> &tuningItems();
    // The item's value as text ("On", "0.46", "3x").
    std::string tuningValueText(const TuningItem &item);
    // One step up (dir > 0) or down; a switch flips.
    void tuningStep(const TuningItem &item, int dir);
    // A saved value (0/1 for a switch); false when the key is unknown or
    // its variable was set at start.
    bool tuningRestore(const char *key, float value);
}
