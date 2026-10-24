#pragma once

// The native renderer's backends (Metal on macOS, OpenGL on macOS and the
// Vita through vitaGL): runtime/native_renderer.h picks one (native_renderer.cpp).

#include "runtime/native_renderer.h"

#include <functional>

namespace native_renderer::detail
{
    void setInEmulator(bool emulator); // the backend's probe (native_renderer::inEmulator)

    class Backend
    {
    public:
        virtual ~Backend() = default;
        virtual bool hasMesh(uint64_t id) = 0;
        // Meshes let go so far (native_renderer::meshEvictions); none by default.
        virtual uint64_t meshEvictions() { return 0; }
        virtual void uploadMesh(uint64_t id, const Vertex *vertices, uint32_t count, bool perFrame) = 0;
        // native_renderer::frameMesh: by default a scratch buffer handed to
        // uploadMesh at the commit (the GL backend writes into its frame arena).
        virtual Vertex *frameMesh(uint32_t count)
        {
            if (m_frameScratch.size() < count)
                m_frameScratch.resize(count);
            return m_frameScratch.data();
        }
        virtual uint32_t *frameMeshIndices(uint32_t count, uint32_t &base)
        {
            base = 0;
            if (m_indexScratch.size() < count)
                m_indexScratch.resize(count);
            return m_indexScratch.data();
        }
        virtual void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount)
        {
            if (!indexCount)
            {
                uploadMesh(id, m_frameScratch.data(), count, true);
                return;
            }
            m_expanded.resize(indexCount); // the triangle list
            for (uint32_t i = 0; i < indexCount; ++i)
                m_expanded[i] = m_frameScratch[m_indexScratch[i]];
            uploadMesh(id, m_expanded.data(), indexCount, true);
        }
        virtual void cancelFrameMesh() {}
        // native_renderer::gpuSkinning: none by default (the CPU poses).
        virtual bool canSkin() { return false; }
        virtual void uploadSkinnedMesh(uint64_t, const SkinVertex *, uint32_t, const uint32_t *, uint32_t) {}
        virtual uint32_t framePalette(const float *, uint32_t) { return 0; }
        virtual bool hasTexture(uint64_t id) = 0;
        virtual void uploadTexture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels) = 0;
        virtual void beginFrame() = 0;
        virtual void draw(uint64_t mesh, const float clip[16], const DrawState &state) = 0;
        // Meshes drawn one after the other with one matrix and state (by default one draw each).
        virtual void drawMulti(const uint64_t *meshes, uint32_t count, const float clip[16], const DrawState &state)
        {
            for (uint32_t i = 0; i < count; ++i)
                draw(meshes[i], clip, state);
        }
        virtual void setFog(const Fog &fog) = 0;
        virtual void setBloom(const Bloom &bloom) = 0;
        virtual void addScreenBlur(const ScreenBlur &blur) = 0;
        virtual void setGlowComposite(const GlowComposite &glow) = 0;
        virtual void setDepthOfField(const DepthOfField &dof) = 0;
        virtual void endFrame(uint32_t width, uint32_t height) = 0;
        // The frame ending next is not shown (its uploads still made): the
        // last one stays on screen (holdIncomplete). Backends that cannot: no-op.
        virtual void holdFrame() {}
        virtual bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height) = 0;
        // A backend that draws on a thread of its own: returns once every
        // frame handed over is drawn.
        virtual void finishFrames() {}
        // It fills the frame log's render / present times itself (else the
        // front times endFrame).
        virtual bool timesItself() { return false; }

    private:
        std::vector<Vertex> m_frameScratch, m_expanded;
        std::vector<uint32_t> m_indexScratch;
    };

    // nullptr when the platform or the device does not have it.
    Backend *createMetalBackend();
    // makeCurrent: makes the OpenGL context current (on the thread that will draw).
    Backend *createGlBackend(std::function<void()> makeCurrent);
    // The host's present hook (native_renderer::setPresentHook), for the GL backend.
    const std::function<void(uint32_t, uint32_t, uint32_t)> &presentHook();
}
