#pragma once

// The scene recorder (round 25, Debug "Record scene" / "Gravar cena"): one
// click in the player menu and, from when the menu closes, every frame's
// draw list as the native renderer gets it goes to disk, so a scene nobody
// can reach on the Mac (a colossus's death on the console) can be looked at
// and drawn again offline (tools/rec.py, SOTC_REC_REPLAY).
//
// What is written (ux0:data/celosia/rec/<date_time>/ on the Vita,
// logs/rec/<date_time>/ elsewhere, SOTC_REC_DIR to change it):
// - info.txt: the platform, the renderer's settings, when it started and why
//   it stopped;
// - assets_NNN.bin: every mesh, skinned mesh and texture the renderer has
//   while recording (each once by id and content; the renderer is made to
//   upload them again when recording starts, so the ones it already had are
//   there too), with each texture's PS2 source (header, PSM, CLUT);
// - frames_NNN.bin: each frame's compact draw list (per draw: layer,
//   blend / depth state, texture, vertices and their colour range, held or
//   hidden), its effect settings (fog, bloom, glow composite, depth of
//   field, screen blurs), the frame log's split (game / hooks / render /
//   present / wait / jobs), the guest frame and VBlank, the pad as the game
//   read it, sampled guest pc / ra, and the GL backend's glow notes;
// - full_<frame>.bin: every so many frames (more often while layer 8 has
//   draws) the whole draw list with matrices, states, the per-frame meshes'
//   vertices and the skin palettes: enough to draw that frame again. Kept
//   as a ring (the oldest go past a budget);
// - mc_slot1.ps2: the memory card as it was when recording started.
// Blocks are zlib-compressed and written by a thread of the recorder's own.
//
// Recording reads what the game and the renderer already make: no guest
// memory or register is written (gameplay is the same with it on).

#include "runtime/native_renderer.h"

#include <cstdint>
#include <string>

namespace native_recorder
{
    // Recording now (one relaxed load: the renderer's calls test it).
    bool active();

    // The menu item: off -> armed (starts when the menu closes); armed or
    // recording -> off.
    void toggle();
    // The item's value text and the marker drawn over the picture.
    std::string statusText(bool portuguese);
    // The host's marker ("REC 1:23 45 MB"), empty when not recording or armed.
    std::string markerText(bool portuguese, bool &recording);

    // The game thread, once a frame (sotc_native's frame log, after the
    // renderer's endFrame): starts an armed recording when the menu is
    // closed, stops at the limits, and writes the frame's info.
    struct FrameInfo
    {
        uint64_t gameFrame = 0, vblank = 0, timeUs = 0, periodUs = 0;
        // The frame log's counters (native_renderer::FrameCounters) and marks.
        uint64_t hookUs = 0, decodeUs = 0, texUs = 0, meshUs = 0, renderUs = 0, presentUs = 0, waitUs = 0, jobUs = 0, joinUs = 0;
        uint32_t draws = 0, targets = 0, texUploads = 0, meshUploads = 0, heldDraws = 0, asyncJobs = 0, asyncDone = 0, jobs = 0;
        uint64_t texBytes = 0, meshBytes = 0;
        uint64_t schedWaitUs = 0, cdUs = 0, cdBytes = 0, cdReads = 0, vblanks = 0;
        float gameDt = 0.0f;
        bool renderThread = false;
        // Guest words worth having every frame (bossDeathEffect's 0x1186290..,
        // depthOfField's 0x11862d0..): raw.
        uint32_t guestWords[24] = {};
    };
    void frameInfo(const FrameInfo &info, bool menuOpen);

    // A texture's PS2 source (sotc_native's decode): kept with the texture.
    void textureSource(uint64_t id, uint32_t header, uint32_t psm, uint32_t width, uint32_t height, uint32_t format, uint64_t clutHash);
    // The pad as the game read it (PSPadBackend::readState: 32 bytes).
    void padRead(const uint8_t *data);
    // A note of the GL backend's (its [gl:glow] decision for a frame with
    // layer 8 draws; `frame` its own frame count).
    void backendNote(uint64_t frame, const char *text);
    // The guest pc / ra sampler (PS2X_PROFILE's source): returns false when
    // the EE waits. Set once by the runtime.
    void setSampler(bool (*sample)(void *user, uint32_t &pc, uint32_t &ra), void *user);

    // The native renderer's front (native_renderer.cpp) while active():
    namespace front
    {
        // Ids the renderer has but the recording has not: answered "not
        // there" so the hooks upload them again.
        bool known(uint64_t id);
        uint64_t epoch(); // added to meshEvictions(): the hooks ask again when recording starts
        void beginFrame();
        void endFrame(uint32_t width, uint32_t height, bool held);
        void mesh(uint64_t id, const void *vertices, uint32_t count, bool perFrame);
        void frameMeshRoom(void *vertices, uint32_t *indices, uint32_t base);
        void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount);
        void skinnedMesh(uint64_t id, const void *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount);
        void palette(uint32_t handle, const float *qwords, uint32_t count, bool rewrite);
        void texture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels);
        // fate: 0 drawn, 1 held (its build still on the worker), 2 hidden (Debug).
        void draw(const uint64_t *meshes, uint32_t count, const float clip[16], const native_renderer::DrawState &state, uint8_t fate);
        void fog(const native_renderer::Fog &fog);
        void bloom(const native_renderer::Bloom &bloom);
        void blur(const native_renderer::ScreenBlur &blur);
        void glow(const native_renderer::GlowComposite &glow);
        void dof(const native_renderer::DepthOfField &dof);
    }

    // SOTC_REC_REPLAY=<recording dir> (Mac): instead of the game, draws
    // recorded full captures again with this build's renderer and writes
    // PNGs (tools/rec.py drives it). Returns false when not asked.
    bool replayRequested();
    int runReplay();
}
