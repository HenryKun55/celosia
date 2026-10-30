// The scene recorder (runtime/native_recorder.h): the native renderer's
// calls while recording, to files a Mac tool reads (tools/rec.py), and the
// replay of its full captures (SOTC_REC_REPLAY).
//
// File format (all little-endian). A file is a run of blocks:
//   u32 magic 'CRB1', u8 kind (1 assets, 2 frame, 3 full), u8 zlib (1),
//   u16 0, u32 raw size, u32 stored size, u64 the recording's frame, data.
// A block's data is a run of records: u32 type, u32 size, payload.
// Record types and payloads: see the enum below and tools/rec.py.
#include "runtime/native_recorder.h"
#include "runtime/native_renderer.h"
#include "runtime/native_tuning.h"
#include "runtime/ps2_stats.h"
#include "ThreadNaming.h"

#include <zlib.h>

#include <algorithm>
#include <cerrno>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#if defined(__vita__)
#include <psp2/io/devctl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/rtc.h>
extern "C" unsigned int _pthread_stack_default_user; // ps2_vita_runtime.cpp
#else
#include <sys/stat.h>
#include <sys/statvfs.h>
#endif

bool ps2McSnapshot(int port, std::vector<uint8_t> &out); // MemoryCard2.cpp

namespace native_recorder::detail
{
    enum Kind : uint8_t
    {
        kAssets = 1,
        kFrame = 2,
        kFull = 3,
    };
    enum Record : uint32_t
    {
        // assets
        rMesh = 1,      // u64 id, u32 count, u32 0, Vertex[count] (36 bytes)
        rSkin = 2,      // u64 id, u32 count, u32 indexCount, SkinVertex[count] (60 bytes), u32 indices
        rTexture = 3,   // u64 id, u32 w, u32 h, u32 levels, u32 bytes, rgba
        rTexSource = 4, // u64 id, u32 header, psm, w, h, format, u64 clut hash
        // frame (compact) and full
        rBegin = 10,    // u64 frame, u8 full
        rDraw = 11,     // compact draw (48 bytes)
        rEnd = 12,      // u32 w, u32 h, u8 held
        rFog = 20, rBloom = 21, rBlur = 22, rGlow = 23, rDof = 24,
        rInfo = 30,     // FrameInfo
        rPad = 31,      // u64 vblank, u64 game frame, u8 data[8]
        rSamples = 32,  // u32 n, u32 idle, n x (u32 pc, u32 ra, u32 count)
        rNote = 33,     // u64 backend frame, u32 length, text
        // full only
        rFrameMesh = 40, // u64 id, u32 count, u32 indexCount, Vertex[count], u32 indices (from 0)
        rPalette = 41,   // u32 handle, u32 count, u8 rewrite, f32 qwords[count * 4]
        rFullDraw = 42,  // u32 n, u8 fate, u64 meshes[n], f32 clip[16], DrawState
    };

    struct Buf
    {
        std::vector<uint8_t> b;
        template <typename T> void put(const T &v)
        {
            const size_t at = b.size();
            b.resize(at + sizeof(T));
            std::memcpy(b.data() + at, &v, sizeof(T));
        }
        void bytes(const void *p, size_t n)
        {
            const size_t at = b.size();
            b.resize(at + n);
            if (n)
                std::memcpy(b.data() + at, p, n);
        }
        size_t open(uint32_t type)
        {
            put(type);
            put(uint32_t(0));
            return b.size();
        }
        void close(size_t at)
        {
            const uint32_t size = static_cast<uint32_t>(b.size() - at);
            std::memcpy(b.data() + at - 4, &size, 4);
        }
        void clear() { b.clear(); }
        size_t size() const { return b.size(); }
    };

    void putState(Buf &o, const native_renderer::DrawState &s)
    {
        o.put(s.texture);
        o.put(s.clamp);
        o.put(static_cast<uint8_t>(s.blend));
        o.put(s.order);
        o.put(s.opacity);
        o.put(s.uvOffset);
        o.put(s.lodK);
        o.put(s.lodScale);
        o.put(s.maxLod);
        for (bool f : {s.background, s.fogged, s.writeDepth, s.noDepthWrite, s.overlay, s.screen, s.screenDepthTest, s.shadowVolume})
            o.put(static_cast<uint8_t>(f));
        o.put(s.shadow);
        o.put(static_cast<uint8_t>(s.reflectionSource));
        o.put(static_cast<uint8_t>(s.reflectionSample));
        o.put(s.reflectionTint);
        o.put(static_cast<uint8_t>(s.refractionSample));
        o.put(static_cast<uint8_t>(s.fresnel));
        o.put(s.mirrorRipple);
        o.put(static_cast<uint8_t>(s.screenCopy));
        o.put(static_cast<uint8_t>(s.previousFrame));
        o.put(s.debugColour);
        o.put(s.skinPalette);
        o.put(s.skinPaletteCount);
        o.put(static_cast<uint8_t>(s.lit));
        o.put(s.light);
    }
    struct Reader
    {
        const uint8_t *p, *end;
        template <typename T> T get()
        {
            T v{};
            if (p + sizeof(T) <= end)
                std::memcpy(&v, p, sizeof(T));
            p += sizeof(T);
            return v;
        }
        template <typename T> void get(T &v) { v = get<T>(); }
        template <typename T, size_t N> void get(T (&v)[N])
        {
            for (T &x : v)
                x = get<T>();
        }
        bool ok() const { return p <= end; }
    };
    native_renderer::DrawState getState(Reader &r)
    {
        native_renderer::DrawState s;
        r.get(s.texture);
        r.get(s.clamp);
        s.blend = static_cast<native_renderer::Blend>(r.get<uint8_t>());
        r.get(s.order);
        r.get(s.opacity);
        r.get(s.uvOffset);
        r.get(s.lodK);
        r.get(s.lodScale);
        r.get(s.maxLod);
        for (bool *f : {&s.background, &s.fogged, &s.writeDepth, &s.noDepthWrite, &s.overlay, &s.screen, &s.screenDepthTest, &s.shadowVolume})
            *f = r.get<uint8_t>() != 0;
        r.get(s.shadow);
        s.reflectionSource = r.get<uint8_t>() != 0;
        s.reflectionSample = r.get<uint8_t>() != 0;
        r.get(s.reflectionTint);
        s.refractionSample = r.get<uint8_t>() != 0;
        s.fresnel = r.get<uint8_t>() != 0;
        r.get(s.mirrorRipple);
        s.screenCopy = r.get<uint8_t>() != 0;
        s.previousFrame = r.get<uint8_t>() != 0;
        r.get(s.debugColour);
        r.get(s.skinPalette);
        r.get(s.skinPaletteCount);
        s.lit = r.get<uint8_t>() != 0;
        r.get(s.light);
        // Not recorded (round 38's DrawState::alphaTest): the models' TEST for
        // every 3D draw in a replay (the backends apply it to normal-blended,
        // depth-writing draws only).
        if (!s.screen && !s.shadowVolume && !s.background)
            s.alphaTest = 0x140Bu;
        return s;
    }
    uint32_t stateFlags(const native_renderer::DrawState &s)
    {
        const bool f[] = {s.background, s.fogged, s.writeDepth, s.noDepthWrite, s.overlay, s.screen, s.screenDepthTest, s.shadowVolume,
                          s.reflectionSource, s.reflectionSample, s.refractionSample, s.fresnel, s.screenCopy, s.previousFrame, s.lit, s.skinPalette != 0u};
        uint32_t bits = 0;
        for (int i = 0; i < 16; ++i)
            bits |= f[i] ? 1u << i : 0u;
        return bits | (static_cast<uint32_t>(s.clamp & 3u) << 16);
    }

    void putFog(Buf &o, const native_renderer::Fog &f)
    {
        o.put(static_cast<uint8_t>(f.enabled));
        o.put(f.start), o.put(f.width), o.put(static_cast<int32_t>(f.steps)), o.put(f.order);
        o.put(static_cast<uint8_t>(f.vertexFog));
        o.put(f.base), o.put(f.scale), o.put(f.colour);
    }
    native_renderer::Fog getFog(Reader &r)
    {
        native_renderer::Fog f;
        f.enabled = r.get<uint8_t>() != 0;
        r.get(f.start), r.get(f.width);
        f.steps = r.get<int32_t>();
        r.get(f.order);
        f.vertexFog = r.get<uint8_t>() != 0;
        r.get(f.base), r.get(f.scale), r.get(f.colour);
        return f;
    }
    void putBloom(Buf &o, const native_renderer::Bloom &b)
    {
        o.put(static_cast<uint8_t>(b.enabled));
        o.put(b.order), o.put(b.threshold), o.put(b.zA), o.put(b.zB), o.put(b.maskColor), o.put(b.color), o.put(b.temporal);
    }
    native_renderer::Bloom getBloom(Reader &r)
    {
        native_renderer::Bloom b;
        b.enabled = r.get<uint8_t>() != 0;
        r.get(b.order), r.get(b.threshold), r.get(b.zA), r.get(b.zB), r.get(b.maskColor), r.get(b.color), r.get(b.temporal);
        return b;
    }
    void putBlur(Buf &o, const native_renderer::ScreenBlur &b)
    {
        o.put(b.order), o.put(b.angle), o.put(b.scale), o.put(b.dx), o.put(b.dy), o.put(static_cast<int32_t>(b.steps));
    }
    native_renderer::ScreenBlur getBlur(Reader &r)
    {
        native_renderer::ScreenBlur b;
        r.get(b.order), r.get(b.angle), r.get(b.scale), r.get(b.dx), r.get(b.dy);
        b.steps = r.get<int32_t>();
        return b;
    }
    void putGlow(Buf &o, const native_renderer::GlowComposite &g)
    {
        o.put(static_cast<uint8_t>(g.enabled));
        o.put(g.order), o.put(static_cast<int32_t>(g.rounds)), o.put(g.tint), o.put(g.amountA), o.put(g.amountB);
    }
    native_renderer::GlowComposite getGlow(Reader &r)
    {
        native_renderer::GlowComposite g;
        g.enabled = r.get<uint8_t>() != 0;
        r.get(g.order);
        g.rounds = r.get<int32_t>();
        r.get(g.tint), r.get(g.amountA), r.get(g.amountB);
        return g;
    }
    void putDof(Buf &o, const native_renderer::DepthOfField &d)
    {
        o.put(static_cast<uint8_t>(d.enabled));
        o.put(d.order), o.put(d.zA), o.put(d.zB), o.put(d.focusZ), o.put(static_cast<int32_t>(d.slices)), o.put(static_cast<int32_t>(d.rounds));
    }
    native_renderer::DepthOfField getDof(Reader &r)
    {
        native_renderer::DepthOfField d;
        d.enabled = r.get<uint8_t>() != 0;
        r.get(d.order), r.get(d.zA), r.get(d.zB), r.get(d.focusZ);
        d.slices = r.get<int32_t>();
        d.rounds = r.get<int32_t>();
        return d;
    }

    uint64_t contentHash(const void *p, size_t n, uint64_t h)
    {
        const uint8_t *b = static_cast<const uint8_t *>(p);
        size_t i = 0;
        for (; i + 8u <= n; i += 8u)
        {
            uint64_t w;
            std::memcpy(&w, b + i, 8);
            h = (h ^ w) * 0x9E3779B97F4A7C15ull;
            h ^= h >> 29;
        }
        for (; i < n; ++i)
            h = (h ^ b[i]) * 0x100000001b3ull;
        return h ^ n;
    }

    uint64_t nowUs() { return native_renderer::nowUs(); }

    bool makeDir(const std::string &path)
    {
#if defined(__vita__)
        const int r = sceIoMkdir(path.c_str(), 0777);
        return r >= 0 || r == static_cast<int>(0x80010011); // SCE_ERROR_ERRNO_EEXIST
#else
        return ::mkdir(path.c_str(), 0777) == 0 || errno == EEXIST;
#endif
    }
    // Free bytes on the recording's device (~0: not known).
    uint64_t freeBytes(const std::string &path)
    {
#if defined(__vita__)
        struct
        {
            int64_t maxSize, freeSize;
            uint32_t clusterSize;
            void *unk;
        } info{};
        const std::string dev = path.substr(0, path.find(':') + 1);
        if (sceIoDevctl(dev.c_str(), 0x3001, nullptr, 0, &info, sizeof(info)) < 0 || info.freeSize <= 0)
            return ~0ull;
        return static_cast<uint64_t>(info.freeSize);
#else
        struct statvfs s;
        if (statvfs(path.c_str(), &s) != 0)
            return ~0ull;
        return static_cast<uint64_t>(s.f_bavail) * s.f_frsize;
#endif
    }

    std::string baseDir()
    {
        if (const char *d = std::getenv("SOTC_REC_DIR"); d && *d)
            return d;
#if defined(__vita__)
        return "ux0:data/celosia/rec";
#else
        return "logs/rec";
#endif
    }
    uint64_t envNumber(const char *name, uint64_t fallback)
    {
        const char *v = std::getenv(name);
        return v && *v ? std::strtoull(v, nullptr, 10) : fallback;
    }

    // ---- The writer: blocks compressed and written off the game thread ----
    struct Job
    {
        uint8_t kind = 0;
        uint64_t frame = 0;
        std::vector<uint8_t> raw;
        std::string path; // full captures: a file each; "" = the kind's running file
        bool finish = false; // the last job: close the files, write the summary
        std::string summary;
    };

    struct Recorder
    {
        // State (g_state): 0 off, 1 armed (starts when the menu closes), 2 recording.
        std::atomic<int> state{0};
        std::atomic<bool> on{false};
        std::mutex mu; // the front's calls (game thread or the model jobs' worker, one at a time) and the frame
        std::string dir, stopReason;
        std::mutex textMu; // lastDir (read by the menu's thread)
        std::string lastDir;
        uint64_t startUs = 0, startFrame = 0, frame = 0, lastFull = 0, fullCount = 0;
        bool full = false, layer8 = false, glowOn = false, everFull = false;
        uint64_t limitUs = 0, limitBytes = 0, fullBudget = 0, fullEvery = 60, fullDense = 10;
        std::atomic<uint64_t> epoch{0};
        // ids recorded (by content) and each mesh's vertex count and colour range
        struct MeshMeta
        {
            uint64_t hash = 0;
            uint32_t count = 0;
            uint8_t cmin[4] = {255, 255, 255, 255}, cmax[4] = {0, 0, 0, 0};
        };
        std::unordered_map<uint64_t, MeshMeta> meshes;      // static and skinned
        std::unordered_map<uint64_t, uint64_t> textures;    // id -> content hash
        std::unordered_map<uint64_t, MeshMeta> frameMeshes; // this frame's per-frame meshes
        Buf assets, frameBuf, fullBuf;
        // frameMesh's room (commitFrameMesh reads it)
        void *roomVertices = nullptr;
        uint32_t *roomIndices = nullptr;
        uint32_t roomBase = 0;
        // Side inputs (other threads): the pad, the backend's notes, the samples.
        std::mutex sideMu;
        Buf side;
        std::vector<uint64_t> samples; // pc << 32 | ra
        uint32_t idleSamples = 0;
        bool (*sampleFn)(void *, uint32_t &, uint32_t &) = nullptr;
        void *sampleUser = nullptr;
        std::atomic<bool> sampling{false};
        // The writer.
        std::mutex qmu;
        std::condition_variable qcv, qroom;
        std::deque<Job> queue;
        std::atomic<uint64_t> queuedBytes{0}, writtenBytes{0}, fullBytes{0};
        bool writerStarted = false;
        std::atomic<bool> writerFailed{false};
        std::deque<std::pair<std::string, uint64_t>> fullFiles; // the ring
        std::atomic<uint64_t> freeAtStart{0};

        void push(Job &&job)
        {
            const uint64_t bytes = job.raw.size();
            std::unique_lock<std::mutex> lock(qmu);
            // Past 48 MB waiting the game waits (a burst of assets: never dropped).
            while (queuedBytes.load() > (48ull << 20) && !writerFailed.load())
                qroom.wait_for(lock, std::chrono::milliseconds(5));
            queuedBytes += bytes;
            queue.push_back(std::move(job));
            qcv.notify_one();
        }

        void writerLoop()
        {
            ThreadNaming::SetCurrentThreadName("RecWriter");
            native_renderer::threads::place(native_renderer::threads::kRecorder); // cores 1-2
            FILE *files[3] = {nullptr, nullptr, nullptr}; // assets, frames
            uint64_t fileBytes[3] = {0, 0, 0};
            uint32_t fileIndex[3] = {0, 0, 0};
            std::string jobDir;
            std::vector<uint8_t> packed;
            uint64_t lastSpaceCheck = 0;
            for (;;)
            {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(qmu);
                    while (queue.empty())
                        qcv.wait_for(lock, std::chrono::milliseconds(20));
                    job = std::move(queue.front());
                    queue.pop_front();
                }
                const uint64_t rawBytes = job.raw.size();
                if (job.finish)
                {
                    for (FILE *&f : files)
                        if (f)
                            std::fclose(f), f = nullptr;
                    if (FILE *f = std::fopen((job.path + "/info.txt").c_str(), "a"))
                    {
                        std::fputs(job.summary.c_str(), f);
                        std::fclose(f);
                    }
                    fileBytes[0] = fileBytes[1] = fileBytes[2] = 0;
                    fileIndex[0] = fileIndex[1] = fileIndex[2] = 0;
                    fullFiles.clear();
                    std::fprintf(stderr, "[rec] written: %s\n", job.path.c_str());
                    std::fflush(stderr);
                }
                else if (job.kind == 0 && !job.raw.empty())
                {
                    // A plain file (the memory card, info.txt's head).
                    if (FILE *f = std::fopen(job.path.c_str(), "wb"))
                    {
                        std::fwrite(job.raw.data(), 1, job.raw.size(), f);
                        std::fclose(f);
                        writtenBytes += job.raw.size();
                    }
                }
                else if (!job.raw.empty())
                {
                    uLongf stored = compressBound(static_cast<uLong>(job.raw.size()));
                    packed.resize(24u + stored);
                    const int z = compress2(packed.data() + 24u, &stored, job.raw.data(), static_cast<uLong>(job.raw.size()), 1);
                    const bool zipped = z == Z_OK;
                    if (!zipped)
                    {
                        stored = static_cast<uLongf>(job.raw.size());
                        std::memcpy(packed.data() + 24u, job.raw.data(), job.raw.size());
                    }
                    const uint32_t magic = 0x31425243u; // 'CRB1'
                    const uint32_t raw = static_cast<uint32_t>(job.raw.size()), st = static_cast<uint32_t>(stored);
                    std::memcpy(packed.data(), &magic, 4);
                    packed[4] = job.kind;
                    packed[5] = zipped ? 1u : 0u;
                    packed[6] = packed[7] = 0;
                    std::memcpy(packed.data() + 8, &raw, 4);
                    std::memcpy(packed.data() + 12, &st, 4);
                    std::memcpy(packed.data() + 16, &job.frame, 8);
                    const size_t total = 24u + stored;
                    bool ok = false;
                    if (job.kind == kFull)
                    {
                        if (FILE *f = std::fopen(job.path.c_str(), "wb"))
                        {
                            ok = std::fwrite(packed.data(), 1, total, f) == total;
                            std::fclose(f);
                        }
                        if (ok)
                        {
                            fullFiles.emplace_back(job.path, total);
                            fullBytes += total;
                            // The ring: the oldest full captures go past the budget.
                            while (fullBytes.load() > fullBudget && fullFiles.size() > 4u)
                            {
                                std::remove(fullFiles.front().first.c_str());
                                fullBytes -= fullFiles.front().second;
                                writtenBytes -= fullFiles.front().second;
                                fullFiles.pop_front();
                            }
                        }
                    }
                    else
                    {
                        const int k = job.kind == kAssets ? 0 : 1;
                        if (!files[k] || fileBytes[k] > (job.kind == kAssets ? (64ull << 20) : (32ull << 20)))
                        {
                            if (files[k])
                                std::fclose(files[k]);
                            char name[64];
                            std::snprintf(name, sizeof(name), "/%s_%03u.bin", k == 0 ? "assets" : "frames", fileIndex[k]++);
                            files[k] = std::fopen((job.path + name).c_str(), "wb");
                            if (files[k])
                                std::setvbuf(files[k], nullptr, _IOFBF, 256u * 1024u);
                            fileBytes[k] = 0;
                        }
                        ok = files[k] && std::fwrite(packed.data(), 1, total, files[k]) == total;
                        if (ok && job.kind == kFrame)
                            std::fflush(files[k]); // a crash keeps the frames up to here
                        fileBytes[k] += total;
                    }
                    if (ok)
                        writtenBytes += total;
                    else if (!writerFailed.exchange(true))
                        std::fprintf(stderr, "[rec] write failed (%s): recording stops\n", job.path.c_str());
                }
                {
                    std::lock_guard<std::mutex> lock(qmu);
                    queuedBytes -= rawBytes;
                }
                qroom.notify_all();
                const uint64_t now = nowUs();
                if (now - lastSpaceCheck > 5000000u && !job.path.empty())
                {
                    lastSpaceCheck = now;
                    const uint64_t free = freeBytes(job.path);
                    if (free != ~0ull && free < (64ull << 20) && !writerFailed.exchange(true))
                        std::fprintf(stderr, "[rec] %llu MB free on the card: recording stops\n", (unsigned long long)(free >> 20));
                }
            }
        }
        void startWriter()
        {
            if (writerStarted)
                return;
            writerStarted = true;
#if defined(__vita__)
            const unsigned int stackDefault = _pthread_stack_default_user;
            _pthread_stack_default_user = 128u * 1024u;
#endif
            std::thread([this] { writerLoop(); }).detach();
            std::thread([this] { samplerLoop(); }).detach();
#if defined(__vita__)
            _pthread_stack_default_user = stackDefault;
#endif
        }
        // The guest pc / ra, 500 times a second while recording.
        void samplerLoop()
        {
            ThreadNaming::SetCurrentThreadName("RecSampler");
            native_renderer::threads::place(native_renderer::threads::kRecorder); // cores 1-2
#if defined(__vita__)
            // Round 32: above the threads it shares cores 1-2 with (the model
            // jobs' worker, the render thread, the builds: all at the default
            // 160). At their priority a wake-up waited until one of them
            // blocked, so the samples fell where those cores went idle (the
            // frame's change, the 2D joins): round 30's console recording
            // had 16% at modelDisp's entry and 0.3% in its body, Vita3K 2.7%
            // and 3.7%. It sleeps between samples: a few microseconds taken
            // 500 times a second. SOTC_REC_SAMPLER_PRIO=<n> (0: as before).
            if (const uint64_t prio = envNumber("SOTC_REC_SAMPLER_PRIO", 96u); prio)
            {
                const int r = sceKernelChangeThreadPriority(sceKernelGetThreadId(), static_cast<int>(prio));
                std::fprintf(stderr, "[rec] sampler thread priority %llu: %s\n", (unsigned long long)prio, r < 0 ? "refused" : "ok");
            }
#endif
            static const uint64_t periodUs = std::max<uint64_t>(500u, envNumber("SOTC_REC_SAMPLE_US", 2000u));
            for (;;)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(sampling.load(std::memory_order_relaxed) ? periodUs : 50000u));
                if (!sampling.load(std::memory_order_relaxed) || !sampleFn)
                    continue;
                uint32_t pc = 0, ra = 0;
                const bool running = sampleFn(sampleUser, pc, ra);
                std::lock_guard<std::mutex> lock(sideMu);
                if (running)
                    samples.push_back((static_cast<uint64_t>(pc) << 32) | ra);
                else
                    ++idleSamples;
            }
        }

        std::string timestamp()
        {
            const std::time_t t = std::time(nullptr);
            std::tm tm{};
#if defined(__vita__)
            // (time() is not the wall clock there: the RTC's local time)
            SceDateTime now{};
            if (sceRtcGetCurrentClockLocalTime(&now) >= 0)
            {
                tm.tm_year = now.year - 1900, tm.tm_mon = now.month - 1, tm.tm_mday = now.day;
                tm.tm_hour = now.hour, tm.tm_min = now.minute, tm.tm_sec = now.second;
            }
            (void)t;
#else
            localtime_r(&t, &tm);
#endif
            char b[32];
            std::snprintf(b, sizeof(b), "%04d-%02d-%02d_%02d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            return b;
        }

        // Game thread, between frames (frameInfo).
        void start(uint64_t gameFrame)
        {
            const std::string base = baseDir();
            makeDir(base.substr(0, base.rfind('/')));
            makeDir(base);
            dir = base + "/" + timestamp();
            if (!makeDir(dir))
            {
                std::fprintf(stderr, "[rec] cannot make %s: not recording\n", dir.c_str());
                stopReason = "no folder";
                state = 0;
                return;
            }
            const uint64_t free = freeBytes(dir);
            freeAtStart = free;
            if (free != ~0ull && free < (200ull << 20))
            {
                std::fprintf(stderr, "[rec] only %llu MB free: not recording\n", (unsigned long long)(free >> 20));
                stopReason = "card full";
                state = 0;
                return;
            }
            limitUs = envNumber("SOTC_REC_MAX_MIN", 15u) * 60000000ull;
            if (const uint64_t secs = envNumber("SOTC_REC_MAX_S", 0u))
                limitUs = secs * 1000000ull;
            limitBytes = envNumber("SOTC_REC_MAX_MB", 700u) << 20;
            fullBudget = envNumber("SOTC_REC_FULL_MB", 250u) << 20;
            fullEvery = std::max<uint64_t>(1u, envNumber("SOTC_REC_FULL_EVERY", 60u));
            fullDense = std::max<uint64_t>(1u, envNumber("SOTC_REC_FULL_DENSE", 10u));
            if (free != ~0ull)
                limitBytes = std::min<uint64_t>(limitBytes, free - (100ull << 20));
            meshes.clear();
            textures.clear();
            frameMeshes.clear();
            assets.clear();
            frameBuf.clear();
            fullBuf.clear();
            {
                std::lock_guard<std::mutex> lock(sideMu);
                side.clear();
                samples.clear();
                idleSamples = 0;
            }
            writtenBytes = 0;
            fullBytes = 0;
            writerFailed = false;
            startWriter();
            // info.txt's head: the platform, the settings, the start.
            std::string info = "celosia scene recording, format 1\n";
#if defined(__vita__)
            info += native_renderer::inEmulator() ? "platform vita3k\n" : "platform vita\n";
#elif defined(__APPLE__)
            info += "platform mac\n";
#else
            info += "platform other\n";
#endif
            info += std::string("backend ") + native_renderer::backendName() + "\n";
            info += "started " + timestamp() + " game frame " + std::to_string(gameFrame) + " vblank " +
                    std::to_string(ps2_stats::vblanks.load(std::memory_order_relaxed)) + "\n";
            info += "free_mb " + std::to_string(free == ~0ull ? -1 : static_cast<long long>(free >> 20)) + "\n";
            info += "limits minutes " + std::to_string(limitUs / 60000000ull) + " mb " + std::to_string(limitBytes >> 20) + " full_mb " + std::to_string(fullBudget >> 20) +
                    " full_every " + std::to_string(fullEvery) + " full_dense " + std::to_string(fullDense) + "\n";
            const native_renderer::Tuning &t = native_renderer::tuning();
            char line[160];
            for (const native_renderer::TuningItem &item : native_renderer::tuningItems())
            {
                if (item.action)
                    continue;
                double v = 0.0;
                if (item.toggle)
                    v = t.*item.toggle ? 1.0 : 0.0;
                else if (item.mask)
                    v = ((t.*item.mask >> item.bit) & 1u) ? 1.0 : 0.0;
                else if (item.number)
                    v = t.*item.number;
                std::snprintf(line, sizeof(line), "tuning %s=%g\n", item.key, v);
                info += line;
            }
            for (const char *env : {"SOTC_GLOW_VITA", "SOTC_GL_REDRAW", "SOTC_HOOK_ASYNC", "SOTC_RENDER_SCALE", "SOTC_NATIVE", "SOTC_FRAME_INTERVAL"})
                if (const char *v = std::getenv(env))
                    info += std::string("env ") + env + "=" + v + "\n";
            Job head;
            head.path = dir + "/info.txt";
            head.raw.assign(info.begin(), info.end());
            push(std::move(head));
            std::vector<uint8_t> card;
            if (ps2McSnapshot(0, card))
            {
                Job mc;
                mc.path = dir + "/mc_slot1.ps2";
                mc.raw = std::move(card);
                push(std::move(mc));
            }
            startUs = nowUs();
            startFrame = gameFrame;
            frame = 0;
            lastFull = 0;
            fullCount = 0;
            everFull = false;
            stopReason.clear();
            ++epoch; // the hooks ask for every mesh again
            native_renderer::tuning().recording = true;
            sampling = true;
            on.store(true, std::memory_order_release);
            state = 2;
            std::fprintf(stderr, "[rec] recording to %s (game frame %llu; stops after %llu min or %llu MB)\n", dir.c_str(), (unsigned long long)gameFrame,
                         (unsigned long long)(limitUs / 60000000ull), (unsigned long long)(limitBytes >> 20));
            std::fflush(stderr);
        }
        void stop(const char *why)
        {
            on.store(false, std::memory_order_release);
            sampling = false;
            native_renderer::tuning().recording = false;
            flushAssets();
            stopReason = why;
            {
                std::lock_guard<std::mutex> text(textMu);
                lastDir = dir;
            }
            char b[256];
            const uint64_t secs = (nowUs() - startUs) / 1000000u;
            std::snprintf(b, sizeof(b), "stopped %s: %s after %llu s, %llu frames, %llu full captures, %llu MB written\n", timestamp().c_str(), why,
                          (unsigned long long)secs, (unsigned long long)frame, (unsigned long long)fullCount, (unsigned long long)(writtenBytes.load() >> 20));
            std::fprintf(stderr, "[rec] %s", b);
            Job end;
            end.finish = true;
            end.path = dir;
            end.summary = b;
            push(std::move(end));
            meshes.clear();
            textures.clear();
            frameMeshes.clear();
            state = 0;
        }
        void flushAssets()
        {
            if (!assets.size())
                return;
            Job j;
            j.kind = kAssets;
            j.frame = frame;
            j.path = dir;
            j.raw.swap(assets.b);
            assets.clear();
            push(std::move(j));
        }
        void meta(MeshMeta &m, const native_renderer::Vertex *v, uint32_t n)
        {
            m.count = n;
            for (int c = 0; c < 4; ++c)
                m.cmin[c] = 255, m.cmax[c] = 0;
            for (uint32_t i = 0; i < n; ++i)
            {
                const uint8_t c[4] = {v[i].r, v[i].g, v[i].b, v[i].a};
                for (int k = 0; k < 4; ++k)
                {
                    m.cmin[k] = std::min(m.cmin[k], c[k]);
                    m.cmax[k] = std::max(m.cmax[k], c[k]);
                }
            }
        }
    };
    Recorder &rec()
    {
        static Recorder *r = new Recorder(); // never destroyed: its threads run to the end
        return *r;
    }
}

namespace native_recorder
{
    using namespace detail;

    bool active() { return rec().on.load(std::memory_order_relaxed); }

    void toggle()
    {
        Recorder &r = rec();
        const int s = r.state.load();
        if (s == 0)
        {
            r.state = 1;
            std::fprintf(stderr, "[rec] armed: recording starts when the menu closes\n");
        }
        else if (s == 1)
            r.state = 0;
        else
        {
            std::lock_guard<std::mutex> lock(r.mu);
            if (r.state.load() == 2)
                r.stop("turned off in the menu");
        }
    }

    static std::string mmss(uint64_t us)
    {
        const uint64_t s = us / 1000000u;
        char b[16];
        std::snprintf(b, sizeof(b), "%llu:%02llu", (unsigned long long)(s / 60u), (unsigned long long)(s % 60u));
        return b;
    }

    std::string statusText(bool pt)
    {
        Recorder &r = rec();
        const int s = r.state.load();
        if (s == 1)
            return pt ? "Ligado: começa ao fechar o menu" : "On: starts when the menu closes";
        if (s == 2)
        {
            char b[96];
            std::snprintf(b, sizeof(b), pt ? "Gravando %s, %llu MB (X: parar)" : "Recording %s, %llu MB (X: stop)", mmss(nowUs() - r.startUs).c_str(),
                          (unsigned long long)(r.writtenBytes.load() >> 20));
            return b;
        }
        std::string last;
        {
            std::lock_guard<std::mutex> text(r.textMu);
            last = r.lastDir;
        }
        if (!last.empty())
            return (pt ? "Desligado (última: " : "Off (last: ") + last.substr(last.rfind('/') + 1) + ")";
        return pt ? "Desligado" : "Off";
    }

    std::string markerText(bool pt, bool &recording)
    {
        Recorder &r = rec();
        const int s = r.state.load();
        recording = s == 2;
        if (s == 1)
            return pt ? "REC: fechar o menu" : "REC: close the menu";
        if (s != 2)
            return "";
        char b[64];
        std::snprintf(b, sizeof(b), "REC %s %llu MB", mmss(nowUs() - r.startUs).c_str(), (unsigned long long)(r.writtenBytes.load() >> 20));
        return b;
    }

    void frameInfo(const FrameInfo &info, bool menuOpen)
    {
        Recorder &r = rec();
        // SOTC_REC_AT=<game frame> (tests, scripted runs): armed there, as if
        // the item had been clicked (once).
        static const uint64_t autoAt = envNumber("SOTC_REC_AT", 0u);
        static bool autoDone = false;
        if (autoAt && !autoDone && info.gameFrame >= autoAt)
        {
            autoDone = true;
            if (r.state.load() == 0)
            {
                r.state = 1;
                std::fprintf(stderr, "[rec] armed by SOTC_REC_AT at game frame %llu\n", (unsigned long long)info.gameFrame);
            }
        }
        const int s = r.state.load(std::memory_order_relaxed);
        if (s == 0)
            return;
        std::lock_guard<std::mutex> lock(r.mu);
        if (s == 1)
        {
            if (!menuOpen)
                r.start(info.gameFrame);
            return;
        }
        if (r.state.load() != 2)
            return;
        // This frame's info, the side inputs, then the frame's block.
        Buf &o = r.frameBuf;
        {
            const size_t at = o.open(rInfo);
            o.put(info.gameFrame), o.put(info.vblank), o.put(info.timeUs), o.put(info.periodUs);
            for (uint64_t v : {info.hookUs, info.decodeUs, info.texUs, info.meshUs, info.renderUs, info.presentUs, info.waitUs, info.jobUs, info.joinUs})
                o.put(v);
            for (uint32_t v : {info.draws, info.targets, info.texUploads, info.meshUploads, info.heldDraws, info.asyncJobs, info.asyncDone, info.jobs})
                o.put(v);
            for (uint64_t v : {info.texBytes, info.meshBytes, info.schedWaitUs, info.cdUs, info.cdBytes, info.cdReads, info.vblanks})
                o.put(v);
            o.put(info.gameDt);
            o.put(static_cast<uint8_t>(info.renderThread));
            o.put(info.guestWords);
            o.close(at);
        }
        {
            std::lock_guard<std::mutex> side(r.sideMu);
            o.bytes(r.side.b.data(), r.side.size());
            r.side.clear();
            std::sort(r.samples.begin(), r.samples.end());
            const size_t at = o.open(rSamples);
            const size_t countAt = o.size();
            o.put(uint32_t(0));
            o.put(r.idleSamples);
            uint32_t n = 0;
            for (size_t i = 0; i < r.samples.size();)
            {
                size_t j = i;
                while (j < r.samples.size() && r.samples[j] == r.samples[i])
                    ++j;
                o.put(static_cast<uint32_t>(r.samples[i] >> 32));
                o.put(static_cast<uint32_t>(r.samples[i]));
                o.put(static_cast<uint32_t>(j - i));
                ++n;
                i = j;
            }
            std::memcpy(o.b.data() + countAt, &n, 4);
            o.close(at);
            r.samples.clear();
            r.idleSamples = 0;
        }
        Job j;
        j.kind = kFrame;
        j.frame = r.frame;
        j.path = r.dir;
        j.raw.swap(o.b);
        o.clear();
        r.push(std::move(j));
        r.flushAssets();
        const uint64_t now = nowUs();
        if (r.writerFailed.load())
            r.stop("write failed or card full");
        else if (now - r.startUs > r.limitUs)
            r.stop("time limit");
        else if (r.writtenBytes.load() + r.queuedBytes.load() > r.limitBytes)
            r.stop("size limit");
    }

    void textureSource(uint64_t id, uint32_t header, uint32_t psm, uint32_t width, uint32_t height, uint32_t format, uint64_t clutHash)
    {
        if (!active())
            return;
        Recorder &r = rec();
        std::lock_guard<std::mutex> lock(r.mu);
        const size_t at = r.assets.open(rTexSource);
        r.assets.put(id), r.assets.put(header), r.assets.put(psm), r.assets.put(width), r.assets.put(height), r.assets.put(format), r.assets.put(clutHash);
        r.assets.close(at);
    }

    void padRead(const uint8_t *data)
    {
        if (!active())
            return;
        Recorder &r = rec();
        std::lock_guard<std::mutex> lock(r.sideMu);
        const size_t at = r.side.open(rPad);
        r.side.put(static_cast<uint64_t>(ps2_stats::vblanks.load(std::memory_order_relaxed)));
        r.side.put(static_cast<uint64_t>(ps2_stats::vif1Chains.load(std::memory_order_relaxed)));
        r.side.bytes(data, 8);
        r.side.close(at);
    }

    void backendNote(uint64_t frame, const char *text)
    {
        if (!active())
            return;
        Recorder &r = rec();
        std::lock_guard<std::mutex> lock(r.sideMu);
        const size_t at = r.side.open(rNote);
        const uint32_t n = static_cast<uint32_t>(std::strlen(text));
        r.side.put(frame);
        r.side.put(n);
        r.side.bytes(text, n);
        r.side.close(at);
    }

    void setSampler(bool (*sample)(void *, uint32_t &, uint32_t &), void *user)
    {
        rec().sampleUser = user;
        rec().sampleFn = sample;
    }

    namespace front
    {
        bool known(uint64_t id)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            return r.meshes.count(id) || r.textures.count(id);
        }
        uint64_t epoch() { return rec().epoch.load(std::memory_order_relaxed); }

        void beginFrame()
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load())
                return;
            ++r.frame;
            r.frameMeshes.clear();
            // A full capture: the first frame, then every fullEvery frames
            // (fullDense while layer 8 has draws or the glow composite is
            // on); none while the writer is behind.
            const uint64_t every = (r.layer8 || r.glowOn) ? r.fullDense : r.fullEvery;
            r.full = (!r.everFull || r.frame - r.lastFull >= every) && r.queuedBytes.load() < (24ull << 20);
            if (r.full)
            {
                r.lastFull = r.frame;
                r.everFull = true;
                r.fullBuf.clear();
                const size_t at = r.fullBuf.open(rBegin);
                r.fullBuf.put(r.frame);
                r.fullBuf.put(uint8_t(1));
                r.fullBuf.close(at);
            }
            r.layer8 = false;
            const size_t at = r.frameBuf.open(rBegin);
            r.frameBuf.put(r.frame);
            r.frameBuf.put(static_cast<uint8_t>(r.full));
            r.frameBuf.close(at);
        }
        void endFrame(uint32_t width, uint32_t height, bool held)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load())
                return;
            for (Buf *b : {&r.frameBuf, &r.fullBuf})
            {
                if (b == &r.fullBuf && !r.full)
                    continue;
                const size_t at = b->open(rEnd);
                b->put(width), b->put(height), b->put(static_cast<uint8_t>(held));
                b->close(at);
            }
            if (r.full)
            {
                Job j;
                j.kind = kFull;
                j.frame = r.frame;
                char name[64];
                std::snprintf(name, sizeof(name), "/full_%06llu.bin", (unsigned long long)r.frame);
                j.path = r.dir + name;
                j.raw.swap(r.fullBuf.b);
                r.fullBuf.clear();
                r.push(std::move(j));
                ++r.fullCount;
                r.full = false;
            }
        }
        void mesh(uint64_t id, const void *vertices, uint32_t count, bool perFrame)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load())
                return;
            const auto *v = static_cast<const native_renderer::Vertex *>(vertices);
            const size_t bytes = static_cast<size_t>(count) * sizeof(native_renderer::Vertex);
            if (perFrame)
            {
                r.meta(r.frameMeshes[id], v, count);
                if (r.full)
                {
                    const size_t at = r.fullBuf.open(rFrameMesh);
                    r.fullBuf.put(id), r.fullBuf.put(count), r.fullBuf.put(uint32_t(0));
                    r.fullBuf.bytes(v, bytes);
                    r.fullBuf.close(at);
                }
                return;
            }
            const uint64_t h = contentHash(v, bytes, 0x51ED27u);
            Recorder::MeshMeta &m = r.meshes[id];
            if (m.hash == h && m.count == count)
                return;
            r.meta(m, v, count);
            m.hash = h;
            const size_t at = r.assets.open(rMesh);
            r.assets.put(id), r.assets.put(count), r.assets.put(uint32_t(0));
            r.assets.bytes(v, bytes);
            r.assets.close(at);
            if (r.assets.size() > (2u << 20))
                r.flushAssets();
        }
        void frameMeshRoom(void *vertices, uint32_t *indices, uint32_t base)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (vertices)
                r.roomVertices = vertices, r.roomIndices = nullptr, r.roomBase = 0;
            if (indices)
                r.roomIndices = indices, r.roomBase = base;
        }
        void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load() || !r.roomVertices)
                return;
            const auto *v = static_cast<const native_renderer::Vertex *>(r.roomVertices);
            r.meta(r.frameMeshes[id], v, count);
            if (r.full)
            {
                const uint32_t n = r.roomIndices ? indexCount : 0u;
                const size_t at = r.fullBuf.open(rFrameMesh);
                r.fullBuf.put(id), r.fullBuf.put(count), r.fullBuf.put(n);
                r.fullBuf.bytes(v, static_cast<size_t>(count) * sizeof(native_renderer::Vertex));
                for (uint32_t i = 0; i < n; ++i)
                    r.fullBuf.put(r.roomIndices[i] - r.roomBase);
                r.fullBuf.close(at);
            }
            r.roomVertices = nullptr;
            r.roomIndices = nullptr;
        }
        void skinnedMesh(uint64_t id, const void *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load())
                return;
            const auto *v = static_cast<const native_renderer::SkinVertex *>(vertices);
            const size_t bytes = static_cast<size_t>(count) * sizeof(native_renderer::SkinVertex);
            const uint64_t h = contentHash(indices, static_cast<size_t>(indexCount) * 4u, contentHash(v, bytes, 0x5C1Bu));
            Recorder::MeshMeta &m = r.meshes[id];
            if (m.hash == h && m.count == indexCount)
                return;
            m.hash = h;
            m.count = indexCount;
            for (int c = 0; c < 4; ++c)
                m.cmin[c] = 255, m.cmax[c] = 0;
            for (uint32_t i = 0; i < count; ++i)
            {
                const uint8_t c[4] = {v[i].r, v[i].g, v[i].b, v[i].a};
                for (int k = 0; k < 4; ++k)
                    m.cmin[k] = std::min(m.cmin[k], c[k]), m.cmax[k] = std::max(m.cmax[k], c[k]);
            }
            const size_t at = r.assets.open(rSkin);
            r.assets.put(id), r.assets.put(count), r.assets.put(indexCount);
            r.assets.bytes(v, bytes);
            r.assets.bytes(indices, static_cast<size_t>(indexCount) * 4u);
            r.assets.close(at);
            if (r.assets.size() > (2u << 20))
                r.flushAssets();
        }
        void palette(uint32_t handle, const float *qwords, uint32_t count, bool rewrite)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load() || !r.full || !handle)
                return;
            const size_t at = r.fullBuf.open(rPalette);
            r.fullBuf.put(handle), r.fullBuf.put(count), r.fullBuf.put(static_cast<uint8_t>(rewrite));
            r.fullBuf.bytes(qwords, static_cast<size_t>(count) * 16u);
            r.fullBuf.close(at);
        }
        void texture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load())
                return;
            size_t bytes = 0;
            for (uint32_t l = 0, w = width, h = height; l < std::max(levels, 1u); ++l, w = std::max(w / 2u, 1u), h = std::max(h / 2u, 1u))
                bytes += static_cast<size_t>(w) * h * 4u;
            const uint64_t h = contentHash(rgba, bytes, 0x7E5u);
            auto found = r.textures.find(id);
            if (found != r.textures.end() && found->second == h)
                return;
            r.textures[id] = h;
            const size_t at = r.assets.open(rTexture);
            r.assets.put(id), r.assets.put(width), r.assets.put(height), r.assets.put(levels), r.assets.put(static_cast<uint32_t>(bytes));
            r.assets.bytes(rgba, bytes);
            r.assets.close(at);
            if (r.assets.size() > (2u << 20))
                r.flushAssets();
        }
        void draw(const uint64_t *meshes, uint32_t count, const float clip[16], const native_renderer::DrawState &s, uint8_t fate)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load() || !count)
                return;
            if (s.order == 8u)
                r.layer8 = true;
            uint32_t vertices = 0;
            uint8_t cmin[4] = {255, 255, 255, 255}, cmax[4] = {0, 0, 0, 0};
            for (uint32_t i = 0; i < count; ++i)
            {
                const Recorder::MeshMeta *m = nullptr;
                if (auto f = r.frameMeshes.find(meshes[i]); f != r.frameMeshes.end())
                    m = &f->second;
                else if (auto g = r.meshes.find(meshes[i]); g != r.meshes.end())
                    m = &g->second;
                if (!m)
                    continue;
                vertices += m->count;
                for (int k = 0; k < 4; ++k)
                    cmin[k] = std::min(cmin[k], m->cmin[k]), cmax[k] = std::max(cmax[k], m->cmax[k]);
            }
            Buf &o = r.frameBuf;
            const size_t at = o.open(rDraw);
            o.put(meshes[0]);
            o.put(s.texture);
            o.put(s.order);
            o.put(static_cast<uint8_t>(s.blend));
            o.put(fate);
            o.put(static_cast<uint16_t>(std::min<uint32_t>(count, 65535u)));
            o.put(stateFlags(s));
            o.put(vertices);
            o.bytes(cmin, 4);
            o.bytes(cmax, 4);
            o.put(s.opacity);
            o.put(static_cast<uint16_t>(s.skinPaletteCount));
            o.put(uint16_t(0));
            o.close(at);
            if (r.full)
            {
                Buf &f = r.fullBuf;
                const size_t fa = f.open(rFullDraw);
                f.put(count);
                f.put(fate);
                f.bytes(meshes, static_cast<size_t>(count) * 8u);
                f.bytes(clip, 64);
                putState(f, s);
                f.close(fa);
            }
        }
        template <typename T, typename P> static void effectRecord(uint32_t type, const T &value, P put)
        {
            Recorder &r = rec();
            std::lock_guard<std::mutex> lock(r.mu);
            if (!r.on.load())
                return;
            for (Buf *b : {&r.frameBuf, &r.fullBuf})
            {
                if (b == &r.fullBuf && !r.full)
                    continue;
                const size_t at = b->open(type);
                put(*b, value);
                b->close(at);
            }
        }
        void fog(const native_renderer::Fog &f) { effectRecord(rFog, f, putFog); }
        void bloom(const native_renderer::Bloom &b) { effectRecord(rBloom, b, putBloom); }
        void blur(const native_renderer::ScreenBlur &b) { effectRecord(rBlur, b, putBlur); }
        void glow(const native_renderer::GlowComposite &g)
        {
            rec().glowOn = g.enabled;
            effectRecord(rGlow, g, putGlow);
        }
        void dof(const native_renderer::DepthOfField &d) { effectRecord(rDof, d, putDof); }
    }

    // ---- Replay (SOTC_REC_REPLAY, the Mac) --------------------------------

    bool replayRequested() { return std::getenv("SOTC_REC_REPLAY") != nullptr; }

    namespace replay
    {
        struct Block
        {
            uint8_t kind = 0;
            uint64_t frame = 0;
            std::vector<uint8_t> data;
        };
        bool readBlocks(const std::string &path, std::vector<Block> &out)
        {
            FILE *f = std::fopen(path.c_str(), "rb");
            if (!f)
                return false;
            for (;;)
            {
                uint8_t head[24];
                if (std::fread(head, 1, 24, f) != 24)
                    break;
                uint32_t magic, raw, stored;
                std::memcpy(&magic, head, 4);
                std::memcpy(&raw, head + 8, 4);
                std::memcpy(&stored, head + 12, 4);
                if (magic != 0x31425243u)
                    break;
                Block b;
                b.kind = head[4];
                std::memcpy(&b.frame, head + 16, 8);
                std::vector<uint8_t> packed(stored);
                if (std::fread(packed.data(), 1, stored, f) != stored)
                    break;
                if (head[5])
                {
                    b.data.resize(raw);
                    uLongf n = raw;
                    if (uncompress(b.data.data(), &n, packed.data(), stored) != Z_OK)
                        break;
                }
                else
                    b.data.swap(packed);
                out.push_back(std::move(b));
            }
            std::fclose(f);
            return true;
        }

        void writePng(const std::string &path, const std::vector<uint8_t> &rgba, uint32_t w, uint32_t h)
        {
            std::vector<uint8_t> rows;
            rows.reserve(static_cast<size_t>(w * 4u + 1u) * h);
            for (uint32_t y = 0; y < h; ++y)
            {
                rows.push_back(0);
                for (uint32_t x = 0; x < w; ++x)
                {
                    const uint8_t *p = &rgba[(static_cast<size_t>(y) * w + x) * 4u];
                    rows.insert(rows.end(), {p[0], p[1], p[2], 255});
                }
            }
            uLongf n = compressBound(static_cast<uLong>(rows.size()));
            std::vector<uint8_t> z(n);
            compress2(z.data(), &n, rows.data(), static_cast<uLong>(rows.size()), 6);
            z.resize(n);
            FILE *f = std::fopen(path.c_str(), "wb");
            if (!f)
                return;
            const uint8_t sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
            std::fwrite(sig, 1, 8, f);
            auto chunk = [&](const char *type, const std::vector<uint8_t> &data)
            {
                const uint32_t len = static_cast<uint32_t>(data.size());
                const uint8_t l[4] = {static_cast<uint8_t>(len >> 24), static_cast<uint8_t>(len >> 16), static_cast<uint8_t>(len >> 8), static_cast<uint8_t>(len)};
                std::fwrite(l, 1, 4, f);
                std::fwrite(type, 1, 4, f);
                if (len)
                    std::fwrite(data.data(), 1, len, f);
                uLong crc = crc32(0, reinterpret_cast<const Bytef *>(type), 4);
                if (len)
                    crc = crc32(crc, data.data(), len);
                const uint8_t c[4] = {static_cast<uint8_t>(crc >> 24), static_cast<uint8_t>(crc >> 16), static_cast<uint8_t>(crc >> 8), static_cast<uint8_t>(crc)};
                std::fwrite(c, 1, 4, f);
            };
            std::vector<uint8_t> ihdr = {static_cast<uint8_t>(w >> 24), static_cast<uint8_t>(w >> 16), static_cast<uint8_t>(w >> 8), static_cast<uint8_t>(w),
                                         static_cast<uint8_t>(h >> 24), static_cast<uint8_t>(h >> 16), static_cast<uint8_t>(h >> 8), static_cast<uint8_t>(h),
                                         8, 6, 0, 0, 0};
            chunk("IHDR", ihdr);
            chunk("IDAT", z);
            chunk("IEND", {});
            std::fclose(f);
        }
    }

    namespace replay
    {
        // Round 27, previews for recordings made before the fixes (the fixes
        // are in the game's hooks, which a replay does not run):
        // SOTC_REC_FIX_BOSSMASK=<texture hex>,<u>,<v> and
        // SOTC_REC_FIX_RADIUS=<frame>:<r>,...: the draws with that texture get
        // bossDeathEffectBegin's mask instead (a 16-gon of radius 256 r
        // around (u, v) x 256, alpha 0x80 inside, normal blending, no mips),
        // as the hook now gives the flag-17 surfaces (the centre is not in
        // the recording: given). SOTC_REC_FIX_MESHDISP=1: layer 18's meshDisp
        // grids (mesh ids (1 << 60) | n) drawn with the frame's projection
        // (a GPU-skinned draw's matrix) instead of their recorded +0x5A0
        // matrix (approximate: their buffer's own matrix is not recorded and
        // is taken as the identity).
        void bossDisc(float u, float v, float r, std::vector<uint8_t> &rgba)
        {
            rgba.assign(256u * 256u * 4u, 0u);
            float px[17], py[17];
            for (int i = 0; i <= 16; ++i)
            {
                const float a = static_cast<float>(i % 16) * 3.14159274f * 2.0f * 0.0625f;
                px[i] = u * 256.0f + std::cos(a) * 256.0f * r;
                py[i] = v * 256.0f + std::sin(a) * 256.0f * r;
            }
            for (int y = 0; y < 256; ++y)
                for (int x = 0; x < 256; ++x)
                {
                    const float sx = static_cast<float>(x) + 0.5f, sy = static_cast<float>(y) + 0.5f;
                    bool inside = true;
                    for (int i = 0; i < 16 && inside; ++i)
                        inside = (px[i + 1] - px[i]) * (sy - py[i]) - (py[i + 1] - py[i]) * (sx - px[i]) >= 0.0f;
                    if (inside)
                        rgba[(static_cast<size_t>(y) * 256u + x) * 4u + 3u] = 0x80u;
                }
        }
    }

    int runReplay()
    {
        using namespace replay;
        uint64_t fixTexture = 0;
        float fixU = 0.5f, fixV = 0.5f;
        std::unordered_map<uint64_t, float> fixRadius;
        if (const char *e = std::getenv("SOTC_REC_FIX_BOSSMASK"))
        {
            fixTexture = std::strtoull(e, nullptr, 16);
            if (const char *c = std::strchr(e, ','))
                std::sscanf(c + 1, "%f,%f", &fixU, &fixV);
            if (const char *r = std::getenv("SOTC_REC_FIX_RADIUS"))
                for (const char *p = r; *p;)
                {
                    char *next = nullptr;
                    const uint64_t f = std::strtoull(p, &next, 10);
                    if (!next || *next != ':')
                        break;
                    fixRadius[f] = std::strtof(next + 1, &next);
                    p = next;
                    while (*p == ',')
                        ++p;
                }
        }
        const bool fixMeshDisp = std::getenv("SOTC_REC_FIX_MESHDISP") && std::atoi(std::getenv("SOTC_REC_FIX_MESHDISP")) != 0;
        const std::string dir = std::getenv("SOTC_REC_REPLAY");
        const std::string out = std::getenv("SOTC_REC_OUT") ? std::getenv("SOTC_REC_OUT") : dir + "/png";
        const std::string modes = std::getenv("SOTC_REC_MODES") ? std::getenv("SOTC_REC_MODES") : "full,noglow,layers";
        const int warm = std::max(1, std::atoi(std::getenv("SOTC_REC_WARM") ? std::getenv("SOTC_REC_WARM") : "2"));
        makeDir(out);
        if (!native_renderer::available())
        {
            std::fprintf(stderr, "[replay] no native renderer\n");
            return 1;
        }
        // The recording's renderer settings (tuning lines of info.txt).
        if (FILE *f = std::fopen((dir + "/info.txt").c_str(), "r"))
        {
            char line[256];
            while (std::fgets(line, sizeof(line), f))
            {
                char key[128];
                float value;
                if (std::sscanf(line, "tuning %127[^=]=%f", key, &value) == 2 && std::strcmp(key, "render_scale") != 0)
                    native_renderer::tuningRestore(key, value);
            }
            std::fclose(f);
        }
        native_renderer::tuning().asyncBuilds = false;
        native_renderer::tuning().renderThread = false;
        // Assets in order, by the frame they came in.
        std::vector<Block> assets;
        for (int i = 0;; ++i)
        {
            char name[64];
            std::snprintf(name, sizeof(name), "/assets_%03d.bin", i);
            if (!readBlocks(dir + name, assets))
                break;
        }
        // The full captures asked for (SOTC_REC_FRAMES=f,f,... or all).
        std::vector<uint64_t> frames;
        const char *want = std::getenv("SOTC_REC_FRAMES");
        if (want && std::strcmp(want, "all") != 0)
            for (const char *p = want; *p;)
            {
                frames.push_back(std::strtoull(p, const_cast<char **>(&p), 10));
                while (*p == ',' || *p == ' ')
                    ++p;
            }
        else
            for (uint64_t f = 1; f < 10000000u; ++f)
            {
                char name[64];
                std::snprintf(name, sizeof(name), "/full_%06llu.bin", (unsigned long long)f);
                if (FILE *t = std::fopen((dir + name).c_str(), "rb"))
                    std::fclose(t), frames.push_back(f);
                if (f > 2000000u)
                    break;
            }
        std::sort(frames.begin(), frames.end());
        std::fprintf(stderr, "[replay] %s: %zu asset blocks, %zu frames\n", dir.c_str(), assets.size(), frames.size());
        size_t nextAsset = 0;
        const uint32_t savedHidden = native_renderer::tuning().hiddenLayers;
        for (uint64_t frame : frames)
        {
            // Every asset up to this frame (the last version of each id wins).
            for (; nextAsset < assets.size() && assets[nextAsset].frame <= frame; ++nextAsset)
            {
                const std::vector<uint8_t> &d = assets[nextAsset].data;
                Reader r{d.data(), d.data() + d.size()};
                while (r.p + 8 <= r.end)
                {
                    const uint32_t type = r.get<uint32_t>(), size = r.get<uint32_t>();
                    const uint8_t *body = r.p;
                    Reader b{body, body + size};
                    if (type == rMesh)
                    {
                        const uint64_t id = b.get<uint64_t>();
                        const uint32_t n = b.get<uint32_t>();
                        b.get<uint32_t>();
                        native_renderer::uploadMesh(id, reinterpret_cast<const native_renderer::Vertex *>(b.p), n);
                    }
                    else if (type == rSkin)
                    {
                        const uint64_t id = b.get<uint64_t>();
                        const uint32_t n = b.get<uint32_t>(), ni = b.get<uint32_t>();
                        const auto *v = reinterpret_cast<const native_renderer::SkinVertex *>(b.p);
                        const auto *idx = reinterpret_cast<const uint32_t *>(b.p + static_cast<size_t>(n) * sizeof(native_renderer::SkinVertex));
                        if (native_renderer::gpuSkinning())
                            native_renderer::uploadSkinnedMesh(id, v, n, idx, ni);
                    }
                    else if (type == rTexture)
                    {
                        const uint64_t id = b.get<uint64_t>();
                        const uint32_t w = b.get<uint32_t>(), h = b.get<uint32_t>(), levels = b.get<uint32_t>();
                        b.get<uint32_t>();
                        native_renderer::uploadTexture(id, w, h, b.p, levels);
                    }
                    r.p = body + size;
                }
            }
            char name[64];
            std::snprintf(name, sizeof(name), "/full_%06llu.bin", (unsigned long long)frame);
            std::vector<Block> full;
            if (!readBlocks(dir + name, full) || full.empty())
            {
                std::fprintf(stderr, "[replay] frame %llu: no full capture\n", (unsigned long long)frame);
                continue;
            }
            const std::vector<uint8_t> &d = full[0].data;
            uint64_t fixMask = 0;
            if (fixTexture)
            {
                const auto it = fixRadius.find(frame);
                std::vector<uint8_t> mask;
                bossDisc(fixU, fixV, it != fixRadius.end() ? it->second : 0.0f, mask);
                fixMask = 0x0200B05500000000ull | (frame & 0xFFFFu);
                native_renderer::uploadTexture(fixMask, 256u, 256u, mask.data(), 1u);
            }
            float fixProjection[16] = {};
            bool haveProjection = false;
            if (fixMeshDisp)
            {
                Reader r{d.data(), d.data() + d.size()};
                while (r.p + 8 <= r.end && !haveProjection)
                {
                    const uint32_t type = r.get<uint32_t>(), size = r.get<uint32_t>();
                    if (type == rFullDraw)
                    {
                        Reader b{r.p, r.p + size};
                        const uint32_t n = b.get<uint32_t>();
                        b.get<uint8_t>();
                        b.p += static_cast<size_t>(n) * 8u;
                        float clip[16];
                        std::memcpy(clip, b.p, 64);
                        b.p += 64;
                        const native_renderer::DrawState st = getState(b);
                        if (st.skinPalette && !st.screen)
                        {
                            std::memcpy(fixProjection, clip, 64);
                            haveProjection = true;
                        }
                    }
                    r.p += size;
                }
            }
            std::vector<uint32_t> layers;
            {
                Reader r{d.data(), d.data() + d.size()};
                while (r.p + 8 <= r.end)
                {
                    const uint32_t type = r.get<uint32_t>(), size = r.get<uint32_t>();
                    if (type == rFullDraw)
                    {
                        Reader b{r.p, r.p + size};
                        const uint32_t n = b.get<uint32_t>();
                        b.get<uint8_t>();
                        b.p += static_cast<size_t>(n) * 8u + 64u;
                        const native_renderer::DrawState s = getState(b);
                        if (std::find(layers.begin(), layers.end(), s.order) == layers.end())
                            layers.push_back(s.order);
                    }
                    r.p += size;
                }
                std::sort(layers.begin(), layers.end());
            }
            // One picture: the frame's calls again (`warm` times: the bloom's
            // history and the last-frame feedback settle), then read back.
            auto play = [&](uint32_t hidden, bool glowOff, const std::string &file, bool isolate = false)
            {
                native_renderer::tuning().hiddenLayers = hidden;
                uint32_t w = 512, h = 448;
                for (int pass = 0; pass < warm; ++pass)
                {
                    std::unordered_map<uint32_t, uint32_t> palettes;
                    native_renderer::beginFrame();
                    Reader r{d.data(), d.data() + d.size()};
                    while (r.p + 8 <= r.end)
                    {
                        const uint32_t type = r.get<uint32_t>(), size = r.get<uint32_t>();
                        Reader b{r.p, r.p + size};
                        switch (type)
                        {
                        case rFrameMesh:
                        {
                            const uint64_t id = b.get<uint64_t>();
                            const uint32_t n = b.get<uint32_t>(), ni = b.get<uint32_t>();
                            const auto *v = reinterpret_cast<const native_renderer::Vertex *>(b.p);
                            if (!ni)
                                native_renderer::uploadMesh(id, v, n, true);
                            else
                            {
                                const auto *idx = reinterpret_cast<const uint32_t *>(b.p + static_cast<size_t>(n) * sizeof(native_renderer::Vertex));
                                std::vector<native_renderer::Vertex> tris(ni);
                                for (uint32_t i = 0; i < ni; ++i)
                                    tris[i] = v[std::min(idx[i], n ? n - 1u : 0u)];
                                native_renderer::uploadMesh(id, tris.data(), ni, true);
                            }
                            break;
                        }
                        case rPalette:
                        {
                            const uint32_t handle = b.get<uint32_t>(), n = b.get<uint32_t>();
                            const bool rewrite = b.get<uint8_t>() != 0;
                            const auto *q = reinterpret_cast<const float *>(b.p);
                            if (rewrite && palettes.count(handle))
                                native_renderer::rewriteFramePalette(palettes[handle], q, n);
                            else if (!rewrite)
                                palettes[handle] = native_renderer::framePalette(q, n);
                            break;
                        }
                        case rFullDraw:
                        {
                            const uint32_t n = b.get<uint32_t>();
                            const uint8_t fate = b.get<uint8_t>();
                            std::vector<uint64_t> meshes(n);
                            std::memcpy(meshes.data(), b.p, static_cast<size_t>(n) * 8u);
                            b.p += static_cast<size_t>(n) * 8u;
                            float clip[16];
                            std::memcpy(clip, b.p, 64);
                            b.p += 64;
                            native_renderer::DrawState s = getState(b);
                            if (s.skinPalette)
                                s.skinPalette = palettes.count(s.skinPalette) ? palettes[s.skinPalette] : 0u;
                            if (fixMask && s.texture == fixTexture)
                            {
                                s.texture = fixMask;
                                s.blend = native_renderer::Blend::Normal;
                                s.maxLod = 0.0f;
                                s.uvOffset[0] = s.uvOffset[1] = 0.0f;
                            }
                            if (haveProjection && s.order == 18u && n && (meshes[0] >> 60) == 1u)
                                std::memcpy(clip, fixProjection, 64);
                            if (fate == 1)
                                break; // held on the worker: not drawn then either
                            native_renderer::drawMulti(meshes.data(), n, clip, s);
                            break;
                        }
                        // A layer on its own (isolate): no fog, bloom, blur or
                        // depth of field over it (they read the whole picture).
                        case rFog:
                            if (!isolate)
                                native_renderer::setFog(getFog(b));
                            break;
                        case rBloom:
                            if (!isolate)
                                native_renderer::setBloom(getBloom(b));
                            break;
                        case rBlur:
                            if (!isolate)
                                native_renderer::addScreenBlur(getBlur(b));
                            break;
                        case rGlow:
                        {
                            native_renderer::GlowComposite g = getGlow(b);
                            if (glowOff)
                                g.enabled = false;
                            native_renderer::setGlowComposite(g);
                            break;
                        }
                        case rDof:
                            if (!isolate)
                                native_renderer::setDepthOfField(getDof(b));
                            break;
                        case rEnd:
                            w = b.get<uint32_t>();
                            h = b.get<uint32_t>();
                            break;
                        default: break;
                        }
                        r.p += size;
                    }
                    native_renderer::endFrame(w, h);
                    native_renderer::finishFrames();
                }
                std::vector<uint8_t> rgba;
                uint32_t pw = 0, ph = 0;
                if (native_renderer::latestFrame(rgba, pw, ph) && pw && ph)
                {
                    writePng(out + "/" + file, rgba, pw, ph);
                    std::fprintf(stderr, "[replay] %s (%ux%u)\n", file.c_str(), pw, ph);
                }
                else
                    std::fprintf(stderr, "[replay] %s: no picture\n", file.c_str());
            };
            char base[64];
            std::snprintf(base, sizeof(base), "frame_%06llu", (unsigned long long)frame);
            const auto has = [&](const char *m) { return modes.find(m) != std::string::npos; };
            if (has("full"))
                play(savedHidden, false, std::string(base) + ".png");
            if (has("noglow"))
                play(savedHidden, true, std::string(base) + "_noglow.png");
            if (has("layers"))
                for (uint32_t l : layers)
                {
                    char n[96];
                    std::snprintf(n, sizeof(n), "%s_layer%02u.png", base, l);
                    play(~(1u << l), false, n, true);
                    if (l == 8u)
                    {
                        // layer 8 drawn plainly too: its own vertex colours,
                        // as the Vita draws it without the glow composite
                        std::snprintf(n, sizeof(n), "%s_layer08_plain.png", base);
                        play(~(1u << l), true, n, true);
                    }
                }
            if (has("upto"))
                for (uint32_t l : layers)
                {
                    char n[96];
                    std::snprintf(n, sizeof(n), "%s_upto%02u.png", base, l);
                    play(l >= 31u ? 0u : ~((2u << l) - 1u), false, n);
                }
            native_renderer::tuning().hiddenLayers = savedHidden;
        }
        std::fprintf(stderr, "[replay] done\n");
        return 0;
    }
}
