#pragma once

#include <atomic>
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
        // Round 124: the Graphics page's preset (kPresetOff .. kPresetMax,
        // kPresetCustom: none matches) sets every item of presetFields() at
        // once: resolution, filtering, detail and each optional effect.
        float preset = 3.0f;        // SOTC_PRESET (0 Off, 1 Low, 2 Medium, 3 High, 4 Max); High is each platform's default look
        float msaa = 0.0f;          // Metal: 0 off, 1 2x, 2 4x, 3 8x samples a pixel, as far as the GPU has them (SOTC_MSAA=0/2/4/8)
        float textureFilter = 2.0f; // 0 nearest, 1 bilinear, 2 trilinear at the game's mip level, 3-6 anisotropic 2x-16x (Metal) (SOTC_TEXTURE_FILTER)
        float modelDetail = 0.0f;   // 0 the game's level of detail, 1 one level finer, 2 always the finest (SOTC_MODEL_DETAIL)
        float effectDetail = 0.0f;  // Metal: 0 the PS2's effect buffers; 1 reductions averaged over their footprint, the mirror 512; 2 the mirror 1024 (SOTC_EFFECT_DETAIL)
        bool bloom = true;          // SOTC_BLOOM_SCALE=0 turns it off
        float bloomStrength = 1.0f; // x the game's add strength (SOTC_BLOOM_SCALE); 1 = the PS2 (measured 2026-10-19 against the GS chain)
        float bloomRounds = 1.0f;   // blur rounds of +-3 texels at a quarter size, whole (SOTC_BLOOM_ROUNDS)
        float bloomSpread = 1.0f;   // each further round's growth (SOTC_BLOOM_SPREAD)
        bool vertexFog = true;      // the GS fog by distance (SOTC_NATIVE_NOVFOG)
        // x the fog's share (0: none, 1: the game's; SOTC_FOG_AMOUNT). 1 since
        // round 37: the 0.55 before was set by eye against an emulated picture
        // without GS fog; the GS picture with its fog matches 1.
        float vertexFogAmount = 1.0f;
        bool zFog = true;           // fade into the sky by depth (SOTC_NATIVE_ZFOG=0)
        bool glow = true;           // layer 8 glow (SOTC_NATIVE_NOGLOW)
        bool depthOfField = true;   // SOTC_NATIVE_NODOF
        bool shadows = true;        // SOTC_NATIVE_NOSHADOWS
        // Round 37: the skinned characters' body shadows (Wander, Agro, the
        // colossi: shadowDisp's skinned path, reached by a jump the hook never
        // saw before); off: only the rigid ones (the sword) as up to round 36
        // (SOTC_BODY_SHADOWS=0/1).
        bool bodyShadows = true;
        // Round 38: the GS alpha test per draw (DrawState::alphaTest; SotC's
        // models GEQUAL 0x40 / FB_ONLY: the pixels under half alpha draw
        // without writing depth). Off: the round 37 rule, every pixel above
        // alpha 0.02 writes depth (SOTC_ALPHA_TEST=0/1).
        bool alphaTest = true;
        // Round 38: layer 1 (sky, far landscape) depth tested into the
        // background image as the GS draws it (SOTC_BG_DEPTH=0/1).
        bool backgroundDepth = true;
        // Debugging (round 38): shadow volumes with every prism side, as up to
        // round 37 (SOTC_SHADOW_ALLSIDES=1); off: shared inner sides dropped.
        bool shadowAllSides = false;
        bool particles = true;      // SOTC_NATIVE_NOPTCL
        bool screenBlur = true;     // SOTC_NATIVE_NOSBLUR
        bool feedback = true;       // feedbackBlur: the last frame laid over the new one (SOTC_NATIVE_NOFEEDBACK)
        bool reflections = true;    // SOTC_NATIVE_NOREFLECT
        bool refraction = true;     // SOTC_NATIVE_NOREFRACT
        // Round 124: the heat haze (the screen-copy strips behind the bird,
        // the fights' shimmer); off: those draws left out (the underwater
        // view's ripple at layer 19 stays). SOTC_NATIVE_NOHAZE=1.
        bool heatHaze = true;

        // Debugging (the menus' Debug group): what to draw at all.
        uint32_t hiddenLayers = 0;  // bit n: the display list's layer n is not drawn
        bool showNative = true;     // off: the emulated GS picture of the same moment is shown (the native one still drawn)
        // SOTC_NATIVE=2 (both pictures drawn every frame, the emulated one
        // shown): on, the native one is shown instead (F9 flips it; not
        // saved). The GS keeps drawing either way, so the flip is instant.
        bool bothShowNative = false;
        // Set by the F8 capture for its few frames: the emulated picture
        // shown (and so drawn) whatever the two above say.
        bool forceEmulated = false;
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
        // Round 107, the player menu's "Frame rate" (frame_rate, SOTC_CAP20
        // over it): 0 Auto (as before: each frame shown as soon as it is
        // ready), 1 Steady 20: in frames with a scene the game starts a frame
        // only every third VBlank (its own clock, so its time step is 50 ms)
        // and the display holds each picture three refreshes (a frame that
        // runs over shows at the next refresh, the cadence goes on from
        // there). Over the 30 fps cap when both are on.
        float frameRate = 0.0f;
        // OpenGL / Vita: drawing and showing a frame on a thread of its own
        // while the game makes the next (at most one frame ahead); off: the
        // game waits for each frame to be drawn (SOTC_RENDER_THREAD=0/1; on
        // by default on the Vita).
        bool renderThread = false;
        bool frameLog = false;      // per-frame timing to the log (SOTC_FRAME_LOG; native_renderer::FrameCounters)
        bool frameLogAll = false;   // with frameLog: every frame on a line of its own (SOTC_FRAME_LOG_ALL)
        // The performance overlay (round 20): fps, the frame's split as the
        // frame log has it (averaged over ~0.5 s), the worst frame of the
        // last ~4 s and a frame-time graph, drawn over the picture by the
        // host (ps2_runtime.cpp drawHostFrame). Collects the frame log's
        // counters while on (frameTiming()); off: no cost.
        bool perfOverlay = false;
        // The sound continuity log (ps2x::iop::audio_stats::level):
        // 0 off, 1 the [audio] / [loader] lines every 5 s, 2 also each stream
        // event (SOTC_AUDIO_LOG; default 1 with SOTC_FRAME_LOG). The [loader]
        // line's message-box part needs it on at start.
        float audioLog = 0.0f;
        // The Vita's glow layer (round 19, SOTC_GLOW_VITA): 0 layer 8 drawn
        // plainly into the picture (round 18); 1 composited apart on the
        // console, plain in Vita3K (where it needs the picture below layer 8
        // drawn again); 2 apart in Vita3K too.
        float glowVita = 1.0f;
        // The Vita's bloom added at its layer the same frame (round 19,
        // SOTC_BLOOM_SAMEFRAME); off: the glow of frame n added in frame
        // n + 1 (round 18). Always the old order where the picture is drawn
        // again for mid-frame effects (Vita3K).
        bool bloomSameFrame = true;
        // With holdIncomplete: at most this many frames held in a row (SOTC_HOLD_FRAMES).
        float holdFrames = 3.0f;
        // The counters behind the frame log and the overlay are collected.
        // Set by the scene recorder while it records (Debug "Record scene",
        // runtime/native_recorder.h): it keeps the frame log's counters.
        bool recording = false;
        bool frameTiming() const { return frameLog || perfOverlay || recording; }
        // The game thread's time by subsystem to the log (SOTC_SUBSYS_PROF;
        // game/overrides/sotc_prof.cpp: wrapped at start only when on then).
        bool subsysProf = false;
        // The lean copies of the hot recompiled functions in the function
        // table (game/overrides/lean_hot.cpp; put in at start when SOTC_LEAN
        // is on, the Vita's default; this flips them at run time;
        // SOTC_LEAN=2 alternates).
        bool leanCopies = true;
        // With the lean copies: their second set (round 26), compiled with
        // PS2X_EE_TRUST_FINITE=2: the VU0 macro and FPU operands taken as
        // finite (not clamped; only results that can overflow are) and the
        // FPU adds plain host adds (no PS2 operand alignment): results can be
        // one unit in the last place apart. Not exact: off by default
        // (SOTC_EE_TRUST=0/1; SOTC_EE_TRUST_AB=<n> alternates).
        bool trustFinite = false;
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
        // The model draws' host work (modelDispHook past the game's own
        // results: packets, ids, palettes, skinning, the draws) on a worker
        // thread of its own while the game thread goes on (round 23); the
        // game sees nothing of it. On by default on the Vita (SOTC_HOOK_ASYNC=0/1).
        bool hookWorker = false;
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
        // Round 45 (the console's GPU: SGX draws a target in tiles and each
        // render-target switch is a scene of its own):
        // the Vita: each scene loads / stores its depth and stencil only when
        // a draw of the frame needs what was there or what it leaves (the
        // picture's first scene clears them, its last one is not read again;
        // the background, the mirror, the copies' scratch depth and the
        // display need neither); vitaGL forced both for every scene
        // (STORE_DEPTH_STENCIL). Read each frame (SOTC_GL_DEPTH_KEEP=0/1).
        bool glDepthKeep = true;
        // the Vita: the last frame shown is the host's copy of it (no copy
        // of its own), and the water's / heat haze's 256 x 256 copy is made
        // from the picture itself (no full-size snapshot first) when nothing
        // else reads the snapshot: the same pixels, two fewer picture-sized
        // scenes (SOTC_GL_FEW_COPIES=0/1).
        bool glFewCopies = true;
        // Round 55: a frame's one screen blur drawn on a second picture (the
        // picture so far copied there, the copies read from the first, the
        // rest of the frame drawn there on the first's depth) instead of from
        // the scene drawn again (Vita3K's mid-frame redraw) or a mid-frame
        // copy taken up again (the console). The Vita's default; the Mac's
        // OpenGL takes the copy unless SOTC_SBLUR_PINGPONG=1 (Metal: its own
        // path). SOTC_SBLUR_PINGPONG=0/1; A/B name `sblurapart`.
#if defined(__vita__)
        bool blurApart = true;
#else
        bool blurApart = false;
#endif
        // the shadow volumes through a program that only counts (the main
        // vertex program, a fragment program that writes nothing): the same
        // stencil, no fog / texture work per volume pixel (SOTC_GL_VOLUME_PROG=0/1).
        bool glVolumeProgram = true;
        // Round 56 (the Vita): vitaGL's per-draw work made lighter, the same
        // GPU commands' effect: a packed draw's attributes on one vertex
        // stream (one sceGxmSetVertexStream, not four) and its default uniform
        // buffers copied with the library's memcpy, not the sceClibMemcpy
        // import (vglCeloLeanDraw). SOTC_GL_LEAN_DRAW=0/1; A/B name `leandraw`.
        bool glLeanDraw = true;
        // Round 73 (the Vita): fewer GPU scenes a frame, the same pixels. The
        // picture is shown as drawn (two pictures in turn, the one not drawn
        // into this frame is the last one shown) instead of copied into a
        // picture of the host's first; when the soft shadows and the glow
        // layer stand at the same layer, the glow layer and its reductions
        // are drawn while the picture is put down for the shadows' mask
        // (they read its depth only), so the shadows' darkening and the
        // glow's composite go into one scene of the picture; the picture's
        // last scene does not store its depth / stencil (nothing reads them
        // after it). SOTC_GL_FEW_SCENES=0/1; A/B name `scenes73`.
        bool glFewScenes = true;
        // Round 73 (the Vita): textures uploaded in GXM's swizzled layout (the
        // same texels; vitaGL lays them out in rows), read at upload, so a
        // change applies to textures uploaded after it. SOTC_GL_SWIZZLE=0/1;
        // with SOTC_GL_SWIZZLE_BOTH=1 each texture also has the other copy
        // and the A/B name `swizzle73` flips which one draws, per frame.
        bool glSwizzle = true;
        // Round 73 (the Vita): the plain world draws whose texture has one
        // level read it straight from the interpolated UV (tex2D) instead of
        // at a computed LOD (tex2Dlod: a read that waits for the program's
        // arithmetic): the same texel and filter in principle, but Vita3K
        // draws up to 1 level apart on ~0.6% of the altar's pixels (its
        // texture coordinates for such reads), so off by default.
        // SOTC_GL_PLAIN_READS=0/1; A/B name `plainread73` (with it on).
        // Round 98: on by default on the Vita. Round 92's diagnostics put the
        // console's GPU time in the pixels, and round 98's pixel estimate put
        // every textured world pixel through the LOD programs (a texel read
        // that waits for the program's arithmetic); the 1 level on ~0.6% of
        // the pixels is Vita3K's texture coordinates for such reads (a
        // one-level texture read at LOD 0 or at the interpolated UV is the
        // same texel and filter on the GPU). Its window and r98all turn it off.
#if defined(__vita__)
        bool glPlainReads = true;
#else
        bool glPlainReads = false;
#endif
        // Round 74 (the render thread's CPU, the same GL calls in the same
        // order): each pass walks only the draws of its layers that are drawn
        // (merged-away draws and other layers' left out: each pass read every
        // draw of the frame before, ~7 passes at the altar), the frame's
        // draws put in layer order by counting (stable, as the sort), and
        // planFrame's state compare taking equal bits as equal before the
        // float compares. SOTC_GL_LEAN_PASS=0/1; A/B name `pass74`.
        bool glLeanPass = true;
        // Round 74 (the Vita): vitaGL's vertex layout cache kept when only
        // the vertex buffer changes (the attributes' formats, offsets and
        // stride and which of them share a buffer unchanged): a switch to
        // another mesh pool re-patched the vertex program before (a layout
        // miss on the next draw: ~170 a frame at the altar). The same
        // vertex programs and streams. SOTC_GL_KEEP_LAYOUT=0/1; A/B name `layout74`.
        bool glKeepLayout = true;
        // Round 75 (the Vita's GPU): the plain world draws' cut-out (alpha
        // under 0.02, the GS alpha test's side, layer 1's near plane) without
        // `discard` where the draw writes no depth or stencil, blends by the
        // source alpha (normal, additive, subtract) and draws into a target
        // whose alpha is kept or not read again (all but the glow layer), in
        // the program without BG (the BG one compiles to other roundings): the
        // cut pixel is drawn with colour and alpha 0, which leaves the
        // destination as it was. The alpha test's second passes, particles
        // and overlays: a discard makes the SGX's tile wait on the program
        // for the pixel's visibility. SOTC_GL_SOFT_CUT=0/1; A/B name `soft75`.
        bool glSoftCut = true;
        // Round 75: a background (layer 1) draw whose mesh lies wholly past
        // layer 1's near plane (its bounds' least clip w, a margin for the
        // GPU's rounding) is drawn without the near-plane cut (uGuard): no
        // pixel of it could be cut, and with alphas that cannot fall under
        // the cut-out it takes the program without discard. Every guarded
        // draw carried a discard for it; 338 of 400 a frame lie wholly past
        // (Vita3K, a frozen 2 342-draw frame).
        // SOTC_GL_FAR_GUARD=0/1; A/B name `guard75`.
        bool glFarGuard = true;
        // Round 86: the render thread gives the frame's data back to the game
        // thread before it puts the picture on screen (the host's present,
        // where vitaGL's display queue waits for the GPU), not after: the
        // game hands the next frame over while the swap waits, and the
        // render thread starts it as soon as the swap returns. Nothing the
        // present reads (the picture's texture, the host's menu) is part of
        // the hand-over. SOTC_GL_PRESENT86=0/1; A/B name `present86`.
        bool glPresent86 = true;
        // Round 87: within a layer, the draws whose order cannot change a
        // pixel (opaque, depth tested and written, no alpha test that can
        // fail, no overlay / volume / screen / mirror / water state) are put
        // in texture order between the draws that keep their place, so that
        // more neighbours share texture, state and matrix and merge into one
        // GL call. Equal depths keep the later draw: two such draws meeting
        // at exactly the same depth may swap which one shows. Layer 1 (far
        // scenery at the far plane: ties everywhere) keeps its order.
        // Measured: the render thread 0.55-0.90 ms SLOWER a frame (Vita3K 222
        // MHz, altar and route 6, matched): the world's neighbours differ by
        // matrix as well as texture, so sorting by texture buys few merges and
        // costs the sort. Off by default (SOTC_GL_SORT87=1 to try; A/B name `sort87`).
        bool glSort87 = false;
        // Round 87 (GPU, console A/B candidate): the shadow volumes' stencil
        // count drawn with the fragment program disabled (GXM's depth /
        // stencil-only primitives: the ISP alone, no USSE program, no blend)
        // instead of a program writing 0 under a ZERO / ONE blend.
        // Round 88: the console's A/B measured it -2.0 ms a frame (matched):
        // the Vita's default (the Mac has no GXM: unused there).
        // SOTC_GL_VOL87=0/1 (Vita default 1); A/B name `vol87` (its windows flip it).
#if defined(__vita__)
        bool glVol87 = true;
#else
        bool glVol87 = false;
#endif
        // Round 87 (GPU, console A/B candidate): the soft shadows' mask (the
        // 512 x 448 picture put down, its 4 x 4 reduction and blur, taken up
        // again) without the picture's own break; see native_renderer_gl.cpp.
        // Round 88: the console measured 0 ms: stays off.
        // SOTC_GL_SHMASK87=0/1 (default 0); A/B name `shmask87` (its windows flip it).
        bool glShmask87 = false;
        // Round 87 (GPU, console A/B candidate): a frame begins its scenes only
        // once the GPU has finished the frame before (at most one frame on the
        // GPU). Round 88: the console measured it +3.7 ms a frame (the GPU
        // sits idle between frames): stays off. SOTC_GL_INFLIGHT87=0/1
        // (default 0); A/B name `inflight87` (its windows flip it).
        bool glInflight87 = false;
        // Round 87 (GPU, console A/B candidate): with fog on, the background
        // layer drawn straight into a 256 x 256 target (all that is read of it
        // is its 256 x 256 reduction) instead of 512 x 448 and reduced.
        // Round 88: the console's A/B measured it -5.2 ms a frame (matched;
        // the background scene ~20 ms of GPU -> ~1 ms): the Vita's default.
        // SOTC_GL_BGSMALL87=0/1 (Vita default 1, Mac 0); A/B name `bgsmall87` (its windows flip it).
#if defined(__vita__)
        bool glBgSmall87 = true;
#else
        bool glBgSmall87 = false;
#endif
        // Round 88 (GPU, console A/B candidate): a Normal-blend draw whose
        // alpha cannot be under 1 (the texture's least texel alpha x the
        // mesh's least vertex alpha x the draw's opacity, as the cut-out's
        // test reads them) and that is drawn without discard is drawn with
        // blending off: Cs x 1 + Cd x 0 is Cs, the same pixels, but the
        // GPU's hidden surface removal can take it (a blended draw's pixels
        // are all shaded and read the tile, the scene's overdraw in full).
        // Not where the picture's alpha must be kept (the Vita bloom's mask).
        // SOTC_GL_OPAQUE88=0/1 (default 1 on the Vita since round 93, 0
        // elsewhere); A/B name `opaque88` (its windows flip it).
#if defined(__vita__)
        bool glOpaque88 = true;
#else
        bool glOpaque88 = false;
#endif
        // Round 88 (GPU, console A/B candidate): the mirror (the reflection
        // the water reads, the scene drawn again mirrored) drawn into a
        // 256 x 256 target, the PS2's own size (and the Mac's), instead of
        // 512 x 448: ~3.5x fewer pixels; the water reads it by its screen
        // position, rippled, as before.
        // SOTC_GL_MIRROR88=0/1 (default 1 on the Vita since round 93, 0
        // elsewhere); A/B name `mirror88` (its windows flip it).
#if defined(__vita__)
        bool glMirror88 = true;
#else
        bool glMirror88 = false;
#endif
        // Round 92 (GPU diagnostics for the console's A/B, default 0, the
        // picture WRONG while on; each A/B window turns its switch on):
        // diag92px: every target's viewport an eighth of its width and height
        // (the same draws, vertices, scenes and tile stores; ~1/64 of the
        // pixels shaded, textured and blended): what the pixels cost.
        // diag92tex: every game texture read as one grey texel (the same
        // programs and pixels, no texture memory traffic). diag92half: each
        // world / character / shadow draw sends the first half of its
        // triangles (the same draws and state changes, half the vertices).
        // SOTC_GL_DIAG92_PX / SOTC_GL_DIAG92_TEX / SOTC_GL_DIAG92_HALF=0/1.
        bool glDiag92Px = false;
        bool glDiag92Tex = false;
        bool glDiag92Half = false;
        // Round 92 (the Vita's GPU): the plain world draws without BG and
        // without the cut-out (their fragment program reads no screen
        // position) through a vertex program that does not write the screen
        // position varying (TEXCOORD2): three varyings a vertex instead of
        // four in GXM's parameter buffer (every vertex is stored there and
        // read back for each tile it touches). The same positions, the same
        // fragment arithmetic. SOTC_GL_NOSCREEN92=0/1 (default 1 on the
        // Vita); A/B name `noscreen92` (its window turns it off; r92all too).
#if defined(__vita__)
        bool glNoScreen92 = true;
#else
        bool glNoScreen92 = false;
#endif
        // Round 93 (the Vita's GPU). Exact ones (default 1 on the Vita; A/B
        // windows turn them off, `r93all` all together):
        // mirror93: the mirror (the scene drawn again mirrored) only in frames
        // where some draw reads it (the water's reflection draws); before, it
        // was drawn whenever the game sent mirrored draws, water in view or
        // not. SOTC_GL_MIRROR93=0/1; A/B `mirror93`.
        // atop93: a Normal-blend draw whose every drawn pixel has alpha 1
        // (the GS alpha test's first pass at AREF >= 0x80: only pixels at
        // AREF or over are drawn and the alpha is clamped to 1; or a draw
        // with a discard whose least alpha is 1) is drawn with blending off,
        // as opaque88 does for the draws without discard: Cs x 1 + Cd x 0.
        // Not the SOFT programs (they rely on blending), not while the
        // picture's alpha is kept. SOTC_GL_ATOP93=0/1; A/B `atop93`.
        // post93: the GS bloom composite's (mode 18) texture position from
        // the vertex program (it is the screen position plus a constant) and
        // its colour premultiplied on the CPU: one texture read and one
        // multiply a pixel over the whole picture instead of ~14 operations
        // and a dependent read. SOTC_GL_POST93=0/1; A/B `post93`.
#if defined(__vita__)
        bool glMirror93 = true;
        bool glAtOpaque93 = true;
        bool glPost93 = true;
#else
        bool glMirror93 = false;
        bool glAtOpaque93 = false;
        bool glPost93 = false;
#endif
        // Round 93 candidates, NOT the same picture (default 0; their A/B
        // windows turn them on):
        // blur93: the GS bloom chain's 12-tap blur (mode 16, 64 x 64 and
        // 128 x 128, ~270 instructions a pixel) as one weighted sum of the 13
        // texels (the weights of the 12 chained lerps, computed on the CPU),
        // the GS's 8-bit truncation after each tap replaced by its mean loss
        // taken off the sum (the blur feeds the next frame's temporal step:
        // without that the glow settled at about twice its strength), the
        // taps past the edge read at the edge; the composite reads the
        // 128 x 128 blur itself (no stretched 256 x 256 copy). Within a few
        // levels of the chain. SOTC_GL_BLUR93=0/1; A/B `blur93`.
        // glow93: the glow layer's 64 x 64 blur (bossDeathEffect: rounds + 1
        // shifted half-texel copies there and back, 8 scenes at the altar's
        // 3 rounds) as two passes of the same binomial kernel (across, then
        // down; the taps' weights of the repeated 2 x 2 box): without the
        // 8-bit rounding between the copies and with one edge clamp instead
        // of eight. SOTC_GL_GLOW93=0/1; A/B `glow93`.
        // blur93: console A/B 2026-11-06 busy -7.6 ms, frame -2.9 ms, the
        // sky glow judged the same on the console: default on on the Vita.
#if defined(__vita__)
        bool glBlur93 = true;
#else
        bool glBlur93 = false;
#endif
        bool glGlow93 = false;
        // Round 94 (the Vita's GPU: fewer scenes). Each render-target bind is
        // a GXM scene with a fixed cost (the firmware kick, the tiles set up,
        // colour / depth loaded and stored): 35 a frame at the altar before.
        // bgcopy94 (exact): the 256 x 256 background (bgsmall87) read where it
        // was drawn, not copied into backgroundSmall first (a copy at the same
        // size, texel for texel). SOTC_GL_BGCOPY94=0/1; A/B `bgcopy94`.
        // glow94: the glow layer's chain (512 x 448 -> 256 -> 128 -> 64, the
        // 64 x 64 blur, -> 128 -> 256: 7 scenes with glow93, 13 without) as
        // three: the 4 x 4 box of the 256 x 256 reduction straight to 64 x 64
        // (16 bilinear reads), glow93's binomial blur across, then down into
        // the 128 x 128 target (its bilinear read is the 64 -> 128 copy), the
        // composite reading that (one 2x stretch fewer). SOTC_GL_GLOW94=0/1;
        // A/B `glow94`.
        // fx94: the soft shadows' mask (4 x 4 reduction + the across blur in
        // one pass from the picture's alpha, then the down blur), the glow's
        // reduction / across blur (with glow94) and the water's copy (down to
        // 128 x 128 in one pass) drawn as regions of two shared targets
        // (one scene each instead of one a step); where the water's copy
        // follows the shadows with nothing drawn between (the altar), the
        // darkening and the glow composite go to the copy and to the start
        // of the picture's next scene (shmask87's order): one 512 x 448
        // scene fewer. SOTC_GL_FX94=0/1; A/B `fx94`.
        // bloom94: the GS bloom chain's two blurs (64 x 64, the next frame's
        // history, and 128 x 128) drawn as regions of one target, one scene
        // instead of two; with blur93 and post93 the composite reads the
        // 128 x 128 blur there (no stretched copy). The same programs and
        // stored values: exact. (A fused mask + reductions + temporal pass
        // was tried: the GS temporal step's truncation makes the glow
        // multi-stable (a history up to ~50 levels under the frame's value
        // never rises), so its rounding differences settled the glow into a
        // much brighter state in Vita3K.) SOTC_GL_BLOOM94=0/1; A/B `bloom94`.
        // `r94all`: all four off together; `r94on`: all four on together.
        // All four ship OFF (console candidates; their A/B windows turn them
        // on, `r94on` all four together). Vita3K: with the bloom off, the
        // frozen altar is identical with bgcopy94, within 1 level with fx94
        // (~1.7k of 2.1M capture pixels), glow94 up to ~22 levels where the
        // glow meets the screen's edge (glow93's kernel); bloom94 runs the
        // same programs on the same stored values. But with the bloom on, the
        // altar's sky glow settled in a different state (bright, vs r93's
        // dim) in every window with any of them on, the bloom mask itself
        // identical: the GS chain's temporal step is multi-stable and which
        // state it reaches could not be pinned down in Vita3K.
        // Console A/B 2026-11-06 (r94on: [gpu86] busy -9.9 ms, 3D frame
        // -5.4 ms; the altar's sky glow did not flicker): default on on the
        // Vita, their windows now turn them off.
#if defined(__vita__)
        bool glBgCopy94 = true;
        bool glGlow94 = true;
        bool glFx94 = true;
        bool glBloom94 = true;
#else
        bool glBgCopy94 = false;
        bool glGlow94 = false;
        bool glFx94 = false;
        bool glBloom94 = false;
#endif
        // Round 98 (a GPU diagnostic for the console's A/B, default 0, the
        // picture WRONG while on; its A/B window turns it on): the main
        // picture's draws of ONE class left out, the class moving on at each
        // window (SOTC_GL_DIAG98_CLASSES, default 1,2,3,4,5,6; "[diag98]"
        // lines name each window's class): 1 the alpha test's second passes,
        // 2 the blended draws of layers 9 and up (water, effects, particles),
        // 3 the draws through a discard (CUT programs), 4 the fogged draws
        // (the BG programs), 5 the characters (skinned), 6 the blended world
        // draws of layers 2-8. A window's busy drop is the class's GPU cost.
        // SOTC_GL_DIAG98=0/1; A/B `diag98`.
        bool glDiag98 = false;
        // Round 98 (the Vita's GPU): the GS alpha test's second pass (the
        // pixels under AREF, no depth write) drawn with a strict depth test
        // (LESS): the pixels its first pass drew wrote that very depth, so
        // the GPU drops them before their fragment program runs (with
        // LEQUAL every pixel of the draw was shaded a second time, those
        // at AREF or over only to be cut). Differs only where an earlier
        // surface lies at exactly the same depth as the draw's own cut
        // pixels. SOTC_GL_AT98=0/1; A/B `at98` (r98all too).
#if defined(__vita__)
        bool glAt98 = true;
#else
        bool glAt98 = false;
#endif
        // Round 98 (the Vita's GPU): untextured world draws (the full-screen
        // tints and veils of layers 20 / 27, untextured scenery) drew through
        // the LOD programs, which make the texel's LOD and read (a dependent
        // read) before dropping it for the untextured values; now through
        // programs with those values alone (the same arithmetic after them).
        // SOTC_GL_UNTEX98=0/1; A/B `untex98` (r98all too).
#if defined(__vita__)
        bool glUntex98 = true;
#else
        bool glUntex98 = false;
#endif
        // Round 98 (the Vita's GPU): the glow layer's two composites over the
        // whole picture (the sharp layer, then the blurred one, each Over)
        // as one premultiplied pass reading both: one full-screen blended
        // pass fewer (the first one's 8-bit store between them left out:
        // within a level). SOTC_GL_GLOW98=0/1; A/B `glow98` (r98all too).
#if defined(__vita__)
        bool glGlow98 = true;
#else
        bool glGlow98 = false;
#endif
        // Round 98 (the Vita's GPU), the world programs' pixels:
        // nofog98: a fogged draw (zFog's layers before the fog's own, or a
        // fogged model) whose mesh lies wholly nearer than the fog's start
        // (its bounds' greatest clip w) is fogged by 0 at every pixel: drawn
        // through the program without the background read (and so without
        // the screen varying, and with the cut-out without discard where it
        // may). At the altar ~1.6 M of the BG programs' ~1.7 M pixels a frame.
        // SOTC_GL_NOFOG98=0/1; A/B `nofog98`.
        // lod0_98: a mipped texture drawn with maxLod <= 0 (the GS's TEX1 at
        // level 0: the LOD program's lod is 0) is read by the plain program
        // (a texel read the GPU makes before the program runs, no LOD) with
        // the texture's sampling held to level 0 (a non-mipmap min filter:
        // vitaGL's mip count one) while such draws read it. At the altar
        // ~2.1 M of the LOD programs' ~3.1 M pixels a frame.
        // SOTC_GL_LOD0_98=0/1; A/B `lod0_98`.
        // psoft98: the plain (one-level read) draws get round 75's cut-out
        // without discard too (only the LOD programs had it).
        // SOTC_GL_PSOFT98=0/1; A/B `psoft98`. (All in r98all.)
#if defined(__vita__)
        bool glNoFog98 = true;
        bool glLod0_98 = true;
        bool glPlainSoft98 = true;
#else
        bool glNoFog98 = false;
        bool glLod0_98 = false;
        bool glPlainSoft98 = false;
#endif
        // Round 100 (stutter), each in `r100all` too:
        // glInflight100: at most two frames on the GPU. The render thread
        // begins a frame's scenes once the GPU has finished the frame before
        // the last one presented (round 87's inflight87 waited for the last
        // one: the GPU idle between frames, +3.7 ms). The console's GPU ran
        // ~3 frames behind ([gpu86] backlog 75-90 scenes at Avion; round 99):
        // a third frame queued only adds latency and keeps GXM's parameter
        // buffer and vitaGL's pools (uniforms, the circular vertex pool) full.
        // Same commands, same picture. SOTC_GL_INFLIGHT100=0/1; A/B `inflight100`.
        // cdAhead100: the disc's reads into EE memory read ahead as the sound
        // streams' are (round 12): after two reads in a row on one file the
        // next 256 KB are read by a thread of their own (64 KB pieces, each
        // usable once in), and a request it covers is copied from memory. The console's frame logs: the game thread idle ('sched
        // blocked', every guest thread waiting) 6-15 ms in the frames with
        // disc reads, the reads 3-5 ms per 32 KB. The same bytes, sooner.
        // SOTC_CD_AHEAD100=0/1; A/B `cdahead100`.
        // prio100: the disc reader threads (`loader`: the request worker and
        // the read-ahead) at priority 145, above the hook worker (150) and
        // the 191 of the render, lane, build and log threads they share cores
        // 1-2 with: they mostly wait on the card, and the game thread waits
        // on them. SOTC_PRIO100=0/1; A/B `prio100`.
#if defined(__vita__)
        bool glInflight100 = false; // console 2026-11-07: off was 1.0 ms faster (mean), p90 -1.4
        bool prio100 = true;
#else
        bool glInflight100 = false;
        bool prio100 = false;
#endif
        bool cdAhead100 = true;
        // Round 116 (the render thread's CPU, the same GL calls in the same
        // order): a drawMulti's record (one state, one matrix, several meshes)
        // kept whole on the render thread instead of copied into one ~330-byte
        // draw per mesh; the frame's draws are its members (record, mesh), the
        // plan looks the texture up once a record and takes a record's later
        // members as merging with the one before them (the same state and
        // matrix bit for bit) without comparing them again. SOTC_GL_FAT116=0/1;
        // A/B name `fat116`.
        bool glFat116 = true;
        // Round 74 (the console, an experiment): the render thread above the
        // hook worker in priority (149; 191 before) while a frame is drawn:
        // the console's render thread is ~13 ms a frame longer in the wall
        // than its CPU time (the worker and the audio thread share its core
        // at higher priority). SOTC_RENDER_PRIO=0/1 (default 0); A/B name `rprio74`.
        bool renderPrio = false;
        // the GS alpha test's two passes merged into one where they draw the
        // same: every pixel under AREF (the texture's and the mesh's
        // greatest alpha say), one pass without depth writes (SOTC_GL_AT_MERGE=0/1).
        bool alphaTestMerge = true;
        // the body shadows' volumes as indexed meshes (each corner once, the
        // prisms' triangles as indices; inner sides matched by vertex): the
        // same triangles, about a third of the bytes (SOTC_SHADOW_INDEXED=0/1).
        bool shadowIndexed = true;

        // Round 36: sacrifices for the console's game thread (the Debug
        // group's first items, saved in settings.cfg; the Vita's default:
        // the particles one; the Mac: none). Each leaves the game's logic alone and
        // takes something away from what is shown. SOTC_SACRIFICES=0 turns
        // them all off at start (each one's own variable wins over it).
        // Particles: every emitter makes half as many (half its rate, half
        // its live count, at least one) (SOTC_SAC_PARTICLES).
        bool sacParticles = false;
        // Models' detail levels picked as if 1.6x as far (coarser sooner;
        // none vanishes sooner) (SOTC_SAC_LOD).
        bool sacLod = false;
        // Round 65: the Quality menu's performance cuts (all off by default on
        // every platform; each saved in settings.cfg, each its own variable).
        // Camera checks: the camera's look / visibility segment tests (not
        // the one that keeps it out of walls) run every other frame, the
        // frame between given the last result (SOTC_SAC_CAMCOLL).
        bool sacCamColl = false;
        // Far characters: a character far from the camera has its pose
        // (GetMotionMatrix) made every other frame (SOTC_SAC_FARCHARA).
        bool sacFarChara = false;
        // Effects at half rate: the particle systems stepped every other
        // frame with both frames' time (SOTC_SAC_FX).
        bool sacFxHalf = false;
        // Shorter draw distance: models the game lets vanish past their
        // range vanish at 2/3 of it (terrain keeps its range) (SOTC_SAC_DRAWDIST).
        bool sacDrawDist = false;
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
        bool quality = false;           // listed in the Graphics page (round 124; was the Quality group)
        // Round 124: the Graphics page's group (0 none; 1 the preset, 2
        // resolution & filtering, 3 lighting & post, 4 shadows, 5 fog,
        // 6 effects & particles, 7 distance & detail, 8 performance cuts that
        // no preset touches); inPreset: the presets set it.
        uint8_t group = 0;
        bool inPreset = false;
        bool restart = false;           // takes effect only after a restart (none of the Graphics items so far)
        uint8_t backends = 3;           // bit 0: Metal, bit 1: OpenGL (the Vita) - where it changes anything
        // An action instead of a value (the Debug group's "Mark"): run on
        // Cross / left / right, never saved; its value text from `actionText`.
        void (*action)() = nullptr;
        std::string (*actionText)(bool portuguese) = nullptr;
    };
    // The Debug group's "Mark" (round 20): a "[mark] <n> frame <f>" line to
    // the log (err.txt on the Vita), to find a moment again ("aqui travou").
    void writeMark();
    int markCount();

    // The performance overlay's numbers (Tuning::perfOverlay): filled at
    // each game frame's end by the game's frame log code, read by the host's
    // overlay (another thread: perfSnapshot copies under a lock).
    // Round 36: the draws the native renderer got for the last frame (the
    // host loop turns the Vita's 30 fps cap on only in frames with a scene:
    // not on loading screens, logos and black frames).
    inline std::atomic<uint32_t> g_frameDraws{0}, g_lastFrameDraws{0};
    // Round 107: Steady 20 in effect now (Tuning::frameRate and a scene on
    // screen), set by the game's frame release (sotc_overrides.cpp cap20),
    // read by the host loop, which sets the display's interval from it.
    inline std::atomic<bool> g_cap20Active{false};
    // Round 110 (SOTC_PACE110): the game's frame releases counted (sotc_overrides.cpp
    // cap20, every release); the native renderer's hand-over tags each picture with the
    // count of the release its frame started at, carried through the render thread to the
    // present (the Vita's display queue shows it on the refresh its game time asks for).
    inline std::atomic<uint32_t> g_paceSeq{0};

    struct PerfStats
    {
        float fps = 0, frameMs = 0, gameMs = 0, hooksMs = 0, renderMs = 0, waitMs = 0, presentMs = 0, worstMs = 0;
        float draws = 0;
        uint64_t frame = 0;
        bool renderThread = false;
        static constexpr int kHistory = 120;
        float history[kHistory] = {}; // the last frames' times, ms, oldest first
    };
    void perfPublish(const PerfStats &stats);
    bool perfSnapshot(PerfStats &out); // false before the first
    const std::vector<TuningItem> &tuningItems();
    // The item's value as text ("On", "0.46", "3x"), in Portuguese when asked.
    std::string tuningValueText(const TuningItem &item, bool portuguese = false);
    // The name of named value i (a list item; the preset's own names, not the
    // preset the current values match).
    std::string tuningChoiceText(const TuningItem &item, int i, bool portuguese = false);
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

    // The render scale's range. The Vita: 0.5x to 2x (4x does not fit in
    // its memory; the presets, settings.cfg and SOTC_RENDER_SCALE are
    // clamped to it). The Mac: up to 4x.
    constexpr float kMinRenderScale = 0.5f;
#if defined(__vita__)
    constexpr float kMaxRenderScale = 2.0f;
#else
    constexpr float kMaxRenderScale = 4.0f;
#endif
    // Round 124: Off (every optional effect off, the least resolution: the
    // floor to measure against), Low, Medium, High (each platform's default
    // look: the Vita's 1x, the Mac's 3x), Max (everything at its best).
    // Gameplay is the same in all of them: no preset touches the camera's
    // checks (sac_camcoll) or the far characters' poses (sac_farchara).
    enum Preset
    {
        kPresetOff,
        kPresetLow,
        kPresetMedium,
        kPresetHigh,
        kPresetMax,
        kPresetCustom
    };
    // "Off" .. "Max", "Custom".
    const char *presetName(int preset);
    // Sets every preset item to a preset's values.
    void applyPreset(Tuning &t, int preset);
    // The preset these values match, or kPresetCustom.
    int matchingPreset(const Tuning &t);
    // Round 124: the preset items and their values now, one line
    // ("preset High render_scale=1 texture_filter=2 ..."), for [gfx124].
    std::string gfxLine(const Tuning &t);
    // Called once a game frame: a "[gfx124] frame F <gfxLine>" line at the
    // first call and whenever any preset item changed (the menu, the F10
    // panel, an A/B); keeps gfxPresetNow() for the frame log's lines.
    void gfxLogIfChanged(unsigned long long frame);
    inline std::atomic<int> g_gfxPresetNow{kPresetHigh};
    inline int gfxPresetNow() { return g_gfxPresetNow.load(std::memory_order_relaxed); }
    // The item's value came from a variable set at start (its own, or
    // SOTC_PRESET for a preset item): the menus' save keeps the file's.
    bool tuningFromEnv(const TuningItem &item);
    // A saved value (0/1 for a switch); false when the key is unknown or
    // its variable was set at start.
    bool tuningRestore(const char *key, float value);
}
