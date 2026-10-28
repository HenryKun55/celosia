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
    // A set of 64-bit ids (meshes, textures, packets) in one flat table, a
    // power of two long, probed in a line: looked up a few thousand times a
    // frame, where std::unordered_set's prime bucket count costs an integer
    // division (a library call on the Vita's Cortex-A9) per lookup and per
    // node walked. Ids 0 and ~0 are kept apart (the table's empty and erased
    // slots).
    class IdSet
    {
    public:
        bool count(uint64_t id) const
        {
            if (id == kEmpty || id == kErased)
                return id == kEmpty ? m_hasEmpty : m_hasErased;
            if (m_slots.empty())
                return false;
            for (size_t i = slot(id);; i = (i + 1u) & m_mask)
            {
                const uint64_t v = m_slots[i];
                if (v == id)
                    return true;
                if (v == kEmpty)
                    return false;
            }
        }
        void insert(uint64_t id)
        {
            if (id == kEmpty || id == kErased)
            {
                (id == kEmpty ? m_hasEmpty : m_hasErased) = true;
                return;
            }
            // At most half full (erased slots count): a lookup that misses
            // (most of them) walks to an empty slot, ~8 probes at 3/4.
            if ((m_used + 1u) * 2u > m_slots.size())
                grow();
            size_t erased = ~size_t(0);
            for (size_t i = slot(id);; i = (i + 1u) & m_mask)
            {
                const uint64_t v = m_slots[i];
                if (v == id)
                    return;
                if (v == kErased && erased == ~size_t(0))
                    erased = i;
                if (v == kEmpty)
                {
                    if (erased != ~size_t(0))
                        m_slots[erased] = id; // an erased slot taken back (m_used counts it already)
                    else
                    {
                        m_slots[i] = id;
                        ++m_used;
                    }
                    ++m_size;
                    return;
                }
            }
        }
        void erase(uint64_t id)
        {
            if (id == kEmpty || id == kErased)
            {
                (id == kEmpty ? m_hasEmpty : m_hasErased) = false;
                return;
            }
            if (m_slots.empty())
                return;
            for (size_t i = slot(id);; i = (i + 1u) & m_mask)
            {
                const uint64_t v = m_slots[i];
                if (v == id)
                {
                    m_slots[i] = kErased;
                    --m_size;
                    return;
                }
                if (v == kEmpty)
                    return;
            }
        }
        size_t size() const { return m_size + (m_hasEmpty ? 1u : 0u) + (m_hasErased ? 1u : 0u); }

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
            std::vector<uint64_t> old;
            old.swap(m_slots);
            size_t capacity = 64u;
            while (capacity < (m_size + 1u) * 4u) // live ids up to 1/4 after the growth
                capacity *= 2u;
            m_slots.assign(capacity, kEmpty);
            m_mask = capacity - 1u;
            m_used = m_size = 0;
            for (uint64_t v : old)
                if (v != kEmpty && v != kErased)
                    insert(v);
        }
        std::vector<uint64_t> m_slots;
        size_t m_mask = 0, m_used = 0, m_size = 0;
        bool m_hasEmpty = false, m_hasErased = false;
    };

    struct Vertex
    {
        float x, y, z;    // model space
        float nx, ny, nz; // normal (lit draws), model space
        float u, v;       // texture coordinates (repeat)
        uint8_t r, g, b, a; // 0x80 = 1.0, as the PS2's
    };

    bool available();
    // Running in an emulator (Vita3K), as the backend found out at its start
    // (the OpenGL backend on the Vita: a render target read back); false
    // elsewhere.
    bool inEmulator();
    // OpenGL backends: makes the renderer's context current on the calling
    // thread (the backend's render thread; every call below comes from the
    // game thread).
    void setGlContext(std::function<void()> makeCurrent);
    // OpenGL backends: hands each finished picture's GL texture (y-up) to the
    // host instead of reading it back (the Vita: Vita3K's CPU reads of render
    // targets do not see the GPU's output). Called from the render thread.
    void setPresentHook(std::function<void(uint32_t texture, uint32_t width, uint32_t height)> hook);

    bool hasMesh(uint64_t id);
    // The backend has this mesh now (not just a build of it on the worker).
    bool meshKnown(uint64_t id);
    // Counts the meshes the backend has let go (its cache budget): a frontend
    // that remembers meshKnown answers asks again when this changes.
    uint64_t meshEvictions();
    // A triangle list, kept on the GPU under `id` until the renderer goes.
    // perFrame: made again every frame (skinned, 2D, particles...): kept only
    // for this frame (one shared buffer per frame on GL).
    void uploadMesh(uint64_t id, const Vertex *vertices, uint32_t count, bool perFrame = false);
    // A per-frame mesh written in place (no copy): room for `count` vertices,
    // then commitFrameMesh(id, n) keeps the first n as uploadMesh(id, ...,
    // n, true) would. The room is valid until that commit; no other renderer
    // call in between. nullptr when there is no renderer.
    Vertex *frameMesh(uint32_t count);
    // Indexed: after frameMesh, room for `count` indices (32-bit), each
    // `base` + the vertex's place in this mesh, three a triangle; the
    // commit's indexCount says how many were written. Drawn as the triangle
    // list the indices pick out (backends without indices expand it).
    uint32_t *frameMeshIndices(uint32_t count, uint32_t &base);
    void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount = 0);
    void cancelFrameMesh(); // the room given back, no mesh made

    // Skinned meshes posed by the GPU (the OpenGL backend; gpuSkinning() says
    // whether this backend does it and the tuning wants it). The bind pose is
    // kept like a mesh under `id`, each vertex with three palette matrices
    // (the first qword of each, counted from the start of the draw's
    // palette) and their weights (0 for the ones not used); drawn as the
    // triangles the indices pick out. A draw names its palette with
    // DrawState::skinPalette, a frame's copy of the matrices (framePalette:
    // `qwords` qwords, four a matrix, columns as the VU1 has them).
    struct SkinVertex
    {
        float x, y, z, nx, ny, nz, u, v; // as Vertex
        uint8_t r, g, b, a;
        float bone[3];   // a matrix's first qword in the palette
        float weight[3];
    };
    constexpr uint32_t kSkinPaletteQwords = 192; // the most a draw's palette holds
    bool gpuSkinning();
    void uploadSkinnedMesh(uint64_t id, const SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount);
    uint32_t framePalette(const float *qwords, uint32_t count); // 0: not kept
    // The same palette (a handle of this frame's) with other matrices, before
    // the frame is drawn (SotC: the matrices as the display list's DMA reads them).
    void rewriteFramePalette(uint32_t handle, const float *qwords, uint32_t count);

    bool hasTexture(uint64_t id);
    // RGBA8 (alpha 0x80 = opaque, as the PS2's), kept on the GPU under `id`.
    // `levels` mip levels, each half the last, one after the other in `rgba`.
    void uploadTexture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels = 1);

    // Work off the game thread (tuning().asyncBuilds, Debug "Builds on a
    // worker"): `work` runs on a worker thread (it holds copies of what it
    // reads: no game memory, no renderer calls), `finish` on the game thread
    // at the first beginFrame after `work` is done (it uploads what work
    // made). Until then hasMesh / hasTexture say yes for `ids` (no second
    // build) and a draw naming one of them, as its mesh or its texture, is
    // left out. False (nothing run) when the tuning is off: the caller
    // builds it itself.
    bool buildAsync(std::vector<uint64_t> ids, std::function<void()> work, std::function<void()> finish);
    // The same as one object (round 14: an area change queues ~1 200 packet
    // builds in one frame, and the vector and two std::functions were three
    // heap allocations each, ~26 us a build in calibrated Vita3K, ~90 us on
    // the console): up to two ids in place, work() and finish() as above.
    // Owned by the queue from here on (deleted after finish).
    struct AsyncWork
    {
        uint64_t ids[2] = {0, 0};
        uint32_t idCount = 0;
        virtual ~AsyncWork() = default;
        virtual void work() = 0;
        virtual void finish() = 0;
        virtual const std::vector<uint64_t> *moreIds() const { return nullptr; } // ids past the first two
    };
    bool buildAsync(AsyncWork *job);
    // Whether this frame may still queue a build (SOTC_ASYNC_BUDGET=<n> builds a
    // frame; no limit by default): a caller over it leaves the draw out (held)
    // and asks again the next frame.
    bool asyncBudgetLeft();

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
        // A 2D draw tested against the picture's depth (no write), as the GS
        // tests it: positions (x w, y w, w) with w the view depth of its GS Z,
        // and the draw's matrix turns them into clip space (SotC's heat haze
        // behind the eagle, ZTE GEQUAL: it stays behind the bird).
        bool screenDepthTest = false;
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
        // A mesh from uploadSkinnedMesh: its palette (framePalette's handle)
        // and how many qwords of it the draw reads.
        uint32_t skinPalette = 0, skinPaletteCount = 0;
        bool lit = false;
        float light[32] = {};
    };
    void draw(uint64_t mesh, const float clip[16], const DrawState &state);
    // The same as draw() for each of `count` meshes in turn, all with this
    // matrix and state, recorded as one draw (a model's packets of one
    // surface: the state made and copied once). Taps, the draw dump and the
    // held / hidden tests see each mesh as its own draw.
    void drawMulti(const uint64_t *meshes, uint32_t count, const float clip[16], const DrawState &state);
    // Testing (SOTC_DIFFTEST_DRAWS): while a tap is set, every 3D draw (its
    // mesh, state, matrix and lights) is hashed into it, for two runs of the
    // same game code to be compared draw by draw (=2: 2D draws, per-frame
    // meshes by their vertices and skin palettes by their matrices too, some
    // made once a frame: only a first leg has them). nullptr: off.
    void setDrawTap(std::vector<uint64_t> *tap);
    std::vector<uint64_t> *drawTap();
    // With a tap set: draws are hashed into it and not drawn (a dry run of
    // the same draws, e.g. SOTC_PLAN_VERIFY's reference leg).
    void setDrawTapOnly(bool only);
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
    // Returns once every frame handed to a backend's own thread is drawn
    // (the recorder's replay reads each picture back right after).
    void finishFrames();

    // Shown instead of the emulated picture (the emulator's present path asks).
    void setShown(bool shown);
    bool shown();
    // The last finished frame (RGBA8); false before the first.
    bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height);

    // The frame timing log (Debug 'Frame timing log', SOTC_FRAME_LOG): what
    // one game frame spent where, filled by the frontend, the backends and the
    // game's hooks while tuning().frameLog is on; the game override prints
    // and clears it at each frame's end.
    struct FrameCounters
    {
        uint32_t draws = 0, targets = 0, texUploads = 0, meshUploads = 0;
        uint64_t texBytes = 0, meshBytes = 0;
        // Microseconds: backend endFrame (render: its CPU side, present: the
        // host's swap), uploads into the backend, texture decoding, the
        // native hooks' own work (decoding and uploads included).
        uint64_t renderUs = 0, presentUs = 0, texUs = 0, meshUs = 0, decodeUs = 0, hookUs = 0;
        // A backend drawing on a thread of its own (the OpenGL one): render and
        // present are that thread's, for the last frame it drew (they overlap
        // the game); waitUs is the game thread's wait at endFrame (for the
        // frame before when renderThread, for its own frame when not).
        uint64_t waitUs = 0;
        bool renderThread = false;
        // buildAsync: draws left out (their mesh or texture still on the
        // worker), jobs handed to it, and finished jobs taken up.
        uint32_t heldDraws = 0, asyncJobs = 0, asyncDone = 0;
        // The model draws' jobs on a worker (round 23): its time on them, the
        // game thread's waits for it (in hookUs too), the jobs.
        uint64_t jobUs = 0, joinUs = 0;
        uint32_t jobs = 0;
    };
    FrameCounters &frameCounters();
    uint64_t nowUs();

    // Host memory the game's hooks hold, for the backend's memory log
    // ([gl:mem]): the skinned packets' bind poses (kept on the CPU).
    void noteFrontendMemory(uint64_t skinnedPackets);
    uint64_t frontendMemory();
}
