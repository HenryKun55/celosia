// The native renderer's front: picks a backend (native_backend.h) the first
// time it is used. macOS: Metal, or OpenGL with SOTC_NATIVE_GL=1 (to check
// the GL path against Metal); the Vita: OpenGL (vitaGL); elsewhere none.
#include "native_backend.h"
#include "runtime/native_tuning.h"
#include "runtime/native_recorder.h"
#include "ThreadNaming.h"
#include "runtime/ps2_stats.h"
#include "ps2x/iop/audio_stats.h"

#include <algorithm>
#include <cstdarg>
#include <atomic>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <tuple>
#include <functional>
#include <iostream>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#if defined(__vita__)
#include <unistd.h>
#endif
#include <thread>
#include <unordered_set>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
extern "C" unsigned int _pthread_stack_default_user; // ps2_vita_runtime.cpp
#endif

#if defined(__APPLE__) || (defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM))
#define PS2X_NATIVE_GL 1
#endif

namespace native_renderer::detail
{
#if !defined(PS2X_HAVE_METAL)
    Backend *createMetalBackend() { return nullptr; }
#endif
#if !defined(PS2X_NATIVE_GL)
    Backend *createGlBackend(std::function<void()>) { return nullptr; }
#endif
}

namespace native_renderer
{
    Tuning defaultTuning()
    {
        Tuning d;
        // The Mac: High (render scale 3); the Vita: Medium (the PS2's 512 x 448).
#if defined(__APPLE__)
        applyPreset(d, kPresetHigh);
#else
        applyPreset(d, kPresetMedium);
#endif
#if defined(__vita__)
        // Copies of the picture made mid-frame came out white in Vita3K when
        // the picture was taken up again after them: drawn again instead, so
        // the heat haze and the water read the PS2's 256 x 256 copy as on the Mac.
        d.midFrameRedraw = 2.0f; // only in Vita3K (inEmulator)
        // Render and present on a core of their own while the game makes the
        // next frame.
        d.renderThread = true;
        // Textures decoded and meshes built on a worker (the game thread
        // stalled ~80 ms on them at a camera cut, throttled).
        d.asyncBuilds = true;
        d.hookWorker = true;
        d.holdIncomplete = true;
        d.warmShaders = true;
#endif
        return d;
    }

    // The quality items' variables (over a preset).
    static void qualityFromEnv(Tuning &d)
    {
        auto number = [](const char *name, float fallback) { return std::getenv(name) ? static_cast<float>(std::atof(std::getenv(name))) : fallback; };
        d.renderScale = std::clamp(number("SOTC_RENDER_SCALE", d.renderScale), kMinRenderScale, kMaxRenderScale);
        if (std::getenv("SOTC_MSAA")) // samples: 1/2/4/8
        {
            const int samples = std::atoi(std::getenv("SOTC_MSAA"));
            d.msaa = samples >= 8 ? 3.0f : samples >= 4 ? 2.0f : samples >= 2 ? 1.0f : 0.0f;
        }
        d.textureFilter = std::clamp(std::round(number("SOTC_TEXTURE_FILTER", d.textureFilter)), 0.0f, 6.0f);
        d.modelDetail = std::clamp(std::round(number("SOTC_MODEL_DETAIL", d.modelDetail)), 0.0f, 2.0f);
        d.effectDetail = std::clamp(std::round(number("SOTC_EFFECT_DETAIL", d.effectDetail)), 0.0f, 2.0f);
    }

    Tuning *g_tuning = nullptr;
    Tuning &tuningFirst()
    {
        static Tuning t = []
        {
            Tuning d = defaultTuning();
            auto on = [](const char *name) { return std::getenv(name) != nullptr; };
            auto number = [](const char *name, float fallback) { return std::getenv(name) ? static_cast<float>(std::atof(std::getenv(name))) : fallback; };
            // The preset first; a variable for one of its items overrides it.
            if (on("SOTC_PRESET"))
                applyPreset(d, std::clamp(std::atoi(std::getenv("SOTC_PRESET")), 0, static_cast<int>(kPresetUltra)));
            qualityFromEnv(d);
            const float strength = number("SOTC_BLOOM_SCALE", d.bloomStrength);
            d.bloom = strength > 0.0f;
            if (d.bloom)
                d.bloomStrength = strength;
            d.bloomRounds = number("SOTC_BLOOM_ROUNDS", d.bloomRounds);
            d.bloomSpread = number("SOTC_BLOOM_SPREAD", 1.0f);
            d.vertexFog = !on("SOTC_NATIVE_NOVFOG");
            d.vertexFogAmount = number("SOTC_FOG_AMOUNT", d.vertexFogAmount);
            d.zFog = !on("SOTC_NATIVE_ZFOG") || std::atoi(std::getenv("SOTC_NATIVE_ZFOG")) != 0;
            d.glow = !on("SOTC_NATIVE_NOGLOW");
            d.depthOfField = !on("SOTC_NATIVE_NODOF");
            d.shadows = !on("SOTC_NATIVE_NOSHADOWS");
            d.particles = !on("SOTC_NATIVE_NOPTCL");
            d.screenBlur = !on("SOTC_NATIVE_NOSBLUR");
            d.feedback = !on("SOTC_NATIVE_NOFEEDBACK");
            d.reflections = !on("SOTC_NATIVE_NOREFLECT");
            d.refraction = !on("SOTC_NATIVE_NOREFRACT");
            d.mipmaps = !on("SOTC_NATIVE_NOMIPS");
            d.textureScroll = !on("SOTC_NATIVE_NOROW");
            d.rawMirror = on("SOTC_RAW_MIRROR");
            d.hazeSource = number("SOTC_HAZE_SOURCE", d.hazeSource);
            d.waterSource = number("SOTC_WATER_SOURCE", d.waterSource);
            d.frameLog = on("SOTC_FRAME_LOG");
            d.frameLogAll = on("SOTC_FRAME_LOG_ALL");
            d.perfOverlay = on("SOTC_PERF_OVERLAY") && std::atoi(std::getenv("SOTC_PERF_OVERLAY")) != 0;
            d.audioLog = static_cast<float>(std::clamp(ps2x::iop::audio_stats::levelFromEnv(), 0, 2));
            if (on("SOTC_GLOW_VITA"))
                d.glowVita = static_cast<float>(std::clamp(std::atoi(std::getenv("SOTC_GLOW_VITA")), 0, 2));
            if (on("SOTC_BLOOM_SAMEFRAME"))
                d.bloomSameFrame = std::atoi(std::getenv("SOTC_BLOOM_SAMEFRAME")) != 0;
            if (on("SOTC_HOLD_FRAMES"))
                d.holdFrames = static_cast<float>(std::clamp(std::atoi(std::getenv("SOTC_HOLD_FRAMES")), 0, 8));
            d.subsysProf = on("SOTC_SUBSYS_PROF") && std::atoi(std::getenv("SOTC_SUBSYS_PROF")) != 0;
            d.fastHooks = !on("SOTC_NATIVE_SLOWHOOKS");
            if (on("SOTC_PACKET_QUICK"))
                d.packetQuickCheck = std::atoi(std::getenv("SOTC_PACKET_QUICK")) != 0;
            if (on("SOTC_PS2_LISTS"))
                d.ps2Lists = std::atoi(std::getenv("SOTC_PS2_LISTS")) != 0;
            if (on("SOTC_GPU_SKIN"))
                d.gpuSkinning = std::atoi(std::getenv("SOTC_GPU_SKIN")) != 0;
            if (on("SOTC_CAP30"))
                d.frameCap30 = std::atoi(std::getenv("SOTC_CAP30")) != 0;
            if (on("SOTC_RENDER_THREAD"))
                d.renderThread = std::atoi(std::getenv("SOTC_RENDER_THREAD")) != 0;
            if (on("SOTC_GL_WARM"))
                d.warmShaders = std::atoi(std::getenv("SOTC_GL_WARM")) != 0;
            if (on("SOTC_HOLD_INCOMPLETE"))
                d.holdIncomplete = std::atoi(std::getenv("SOTC_HOLD_INCOMPLETE")) != 0;
            if (on("SOTC_ASYNC_BUILDS"))
                d.asyncBuilds = std::atoi(std::getenv("SOTC_ASYNC_BUILDS")) != 0;
            if (on("SOTC_HOOK_ASYNC"))
                d.hookWorker = std::atoi(std::getenv("SOTC_HOOK_ASYNC")) != 0;
            if (on("SOTC_EE_TRUST"))
                d.trustFinite = std::atoi(std::getenv("SOTC_EE_TRUST")) != 0;
            for (const auto &[name, flag] : {std::pair<const char *, bool Tuning::*>{"SOTC_GL_POOLS", &Tuning::glMeshPools}, {"SOTC_GL_MERGE", &Tuning::glMergeDraws},
                                             {"SOTC_GL_LEAN", &Tuning::glLeanPrograms}, {"SOTC_GL_NODISCARD", &Tuning::glNoDiscard}})
                if (on(name))
                    d.*flag = std::atoi(std::getenv(name)) != 0;
            if (on("SOTC_GL_REDRAW"))
                d.midFrameRedraw = static_cast<float>(std::clamp(std::atoi(std::getenv("SOTC_GL_REDRAW")), 0, 2));
            if (const char *h = std::getenv("SOTC_HIDE_LAYERS")) // n,n,n
                for (const char *c = h; *c;)
                {
                    d.hiddenLayers |= 1u << (std::atoi(c) & 31);
                    while (*c && *c != ',')
                        ++c;
                    if (*c)
                        ++c;
                }
            d.preset = static_cast<float>(matchingPreset(d));
            return d;
        }();
        g_tuning = &t;
        return t;
    }

    std::string tuningAsEnv()
    {
        const Tuning &t = tuning(), d = defaultTuning();
        std::string out;
        auto add = [&](const std::string &s) { out += (out.empty() ? "" : " ") + s; };
        auto number = [](float v) { char b[32]; std::snprintf(b, sizeof(b), "%g", v); return std::string(b); };
        if (t.renderScale != d.renderScale)
            add("SOTC_RENDER_SCALE=" + number(t.renderScale));
        if (t.msaa != d.msaa)
            add("SOTC_MSAA=" + std::to_string(t.msaa > 0.5f ? 1 << static_cast<int>(t.msaa + 0.5f) : 1));
        if (t.textureFilter != d.textureFilter)
            add("SOTC_TEXTURE_FILTER=" + number(t.textureFilter));
        if (t.modelDetail != d.modelDetail)
            add("SOTC_MODEL_DETAIL=" + number(t.modelDetail));
        if (t.effectDetail != d.effectDetail)
            add("SOTC_EFFECT_DETAIL=" + number(t.effectDetail));
        if (!t.bloom)
            add("SOTC_BLOOM_SCALE=0");
        else if (t.bloomStrength != d.bloomStrength)
            add("SOTC_BLOOM_SCALE=" + number(t.bloomStrength));
        if (t.bloomRounds != d.bloomRounds)
            add("SOTC_BLOOM_ROUNDS=" + number(t.bloomRounds));
        if (t.bloomSpread != d.bloomSpread)
            add("SOTC_BLOOM_SPREAD=" + number(t.bloomSpread));
        if (!t.vertexFog)
            add("SOTC_NATIVE_NOVFOG=1");
        if (t.vertexFogAmount != d.vertexFogAmount)
            add("SOTC_FOG_AMOUNT=" + number(t.vertexFogAmount));
        if (!t.zFog)
            add("SOTC_NATIVE_ZFOG=0");
        const std::pair<bool, const char *> offs[] = {{t.glow, "SOTC_NATIVE_NOGLOW=1"}, {t.depthOfField, "SOTC_NATIVE_NODOF=1"}, {t.shadows, "SOTC_NATIVE_NOSHADOWS=1"},
                                                      {t.particles, "SOTC_NATIVE_NOPTCL=1"}, {t.screenBlur, "SOTC_NATIVE_NOSBLUR=1"}, {t.feedback, "SOTC_NATIVE_NOFEEDBACK=1"}, {t.reflections, "SOTC_NATIVE_NOREFLECT=1"},
                                                      {t.refraction, "SOTC_NATIVE_NOREFRACT=1"}};
        for (const auto &[value, env] : offs)
            if (!value)
                add(env);
        if (t.frameCap30 != d.frameCap30)
            add(std::string("SOTC_CAP30=") + (t.frameCap30 ? "1" : "0"));
        if (t.midFrameRedraw != d.midFrameRedraw)
            add("SOTC_GL_REDRAW=" + number(t.midFrameRedraw));
        if (t.renderThread != d.renderThread)
            add(std::string("SOTC_RENDER_THREAD=") + (t.renderThread ? "1" : "0"));
        if (t.warmShaders != d.warmShaders)
            add(std::string("SOTC_GL_WARM=") + (t.warmShaders ? "1" : "0"));
        for (const auto &[name, flag] : {std::pair<const char *, bool Tuning::*>{"SOTC_GL_POOLS", &Tuning::glMeshPools}, {"SOTC_GL_MERGE", &Tuning::glMergeDraws},
                                         {"SOTC_GL_LEAN", &Tuning::glLeanPrograms}, {"SOTC_GL_NODISCARD", &Tuning::glNoDiscard}})
            if (t.*flag != d.*flag)
                add(std::string(name) + "=" + (t.*flag ? "1" : "0"));
        if (t.holdIncomplete != d.holdIncomplete)
            add(std::string("SOTC_HOLD_INCOMPLETE=") + (t.holdIncomplete ? "1" : "0"));
        if (t.asyncBuilds != d.asyncBuilds)
            add(std::string("SOTC_ASYNC_BUILDS=") + (t.asyncBuilds ? "1" : "0"));
        if (t.hookWorker != d.hookWorker)
            add(std::string("SOTC_HOOK_ASYNC=") + (t.hookWorker ? "1" : "0"));
        if (t.trustFinite != d.trustFinite)
            add(std::string("SOTC_EE_TRUST=") + (t.trustFinite ? "1" : "0"));
        if (t.frameLog)
            add("SOTC_FRAME_LOG=1");
        if (t.frameLogAll)
            add("SOTC_FRAME_LOG_ALL=1");
        if (t.perfOverlay)
            add("SOTC_PERF_OVERLAY=1");
        if (static_cast<int>(t.audioLog + 0.5f) != (t.frameLog ? 1 : 0))
            add("SOTC_AUDIO_LOG=" + number(t.audioLog));
        if (t.glowVita != d.glowVita)
            add("SOTC_GLOW_VITA=" + number(t.glowVita));
        if (t.bloomSameFrame != d.bloomSameFrame)
            add(std::string("SOTC_BLOOM_SAMEFRAME=") + (t.bloomSameFrame ? "1" : "0"));
        if (t.holdFrames != d.holdFrames)
            add("SOTC_HOLD_FRAMES=" + number(t.holdFrames));
        if (t.subsysProf)
            add("SOTC_SUBSYS_PROF=1");
        if (!t.fastHooks)
            add("SOTC_NATIVE_SLOWHOOKS=1");
        if (t.packetQuickCheck != d.packetQuickCheck)
            add(std::string("SOTC_PACKET_QUICK=") + (t.packetQuickCheck ? "1" : "0"));
        if (t.ps2Lists)
            add("SOTC_PS2_LISTS=1");
        if (t.gpuSkinning != d.gpuSkinning)
            add(std::string("SOTC_GPU_SKIN=") + (t.gpuSkinning ? "1" : "0"));
        if (t.hiddenLayers)
        {
            std::string layers;
            for (int i = 0; i < 32; ++i)
                if ((t.hiddenLayers >> i) & 1u)
                    layers += (layers.empty() ? "" : ",") + std::to_string(i);
            add("SOTC_HIDE_LAYERS=" + layers);
        }
        return out.empty() ? "(the defaults)" : out;
    }

#if defined(__vita__)
#define PS2X_VITA_SCALE_NOTE_EN " On the Vita the resolution goes up to 2x (High and Ultra draw at 2x there: more does not fit in its memory)."
#define PS2X_VITA_SCALE_NOTE_PT " No Vita a resolução vai até 2x (Alto e Ultra desenham em 2x lá: mais que isso não cabe na memória dele)."
#else
#define PS2X_VITA_SCALE_NOTE_EN ""
#define PS2X_VITA_SCALE_NOTE_PT ""
#endif
    const std::vector<TuningItem> &tuningItems()
    {
        static const std::vector<TuningItem> items = []
        {
        std::vector<TuningItem> v = {
            {"quality_preset", "Graphics preset", "Predefinição gráfica",
             "Sets resolution, anti-aliasing, texture filtering, model detail and effect detail at once. Low: half resolution (faster). "
             "Medium: the PS2's 512 x 448 (the Vita's default). High: 3x (the Mac's default). Ultra: 4x, 4x MSAA, 16x anisotropic, finest models, "
             "high effect detail (Mac). Changing any of those makes it Custom." PS2X_VITA_SCALE_NOTE_EN,
             "Ajusta resolução, anti-aliasing, filtro de texturas, detalhe dos modelos e dos efeitos de uma vez. Baixo: metade da resolução (mais rápido). "
             "Médio: os 512 x 448 do PS2 (o padrão do Vita). Alto: 3x (o padrão do Mac). Ultra: 4x, MSAA 4x, anisotrópico 16x, modelos mais detalhados, "
             "efeitos em alto detalhe (Mac). Mudar qualquer um deles vira Personalizado." PS2X_VITA_SCALE_NOTE_PT,
             nullptr, &Tuning::preset, 0.0f, 4.0f, 1.0f, "SOTC_PRESET"},
            {"render_scale", "Resolution", "Resolução",
#if defined(__vita__)
             "The picture is drawn at 512 x 448 times this: 0.5 is faster, 1 is the PS2's, 1.5 and 2 are sharper. At most 2 on the Vita (more does not fit in its memory).",
             "A imagem é desenhada em 512 x 448 vezes este valor: 0,5 é mais rápido, 1 é a do PS2, 1,5 e 2 ficam mais nítidas. No máximo 2 no Vita (mais que isso não cabe na memória dele).",
#else
             "The picture is drawn at 512 x 448 times this: 0.5 is faster, 1 is the PS2's, 2 to 4 are sharper.",
             "A imagem é desenhada em 512 x 448 vezes este valor: 0,5 é mais rápido, 1 é a do PS2, 2 a 4 ficam mais nítidas.",
#endif
             nullptr, &Tuning::renderScale, kMinRenderScale, kMaxRenderScale, 0.5f, "SOTC_RENDER_SCALE"},
            {"msaa", "Anti-aliasing (MSAA)", "Anti-aliasing (MSAA)",
             "Metal (Mac) only: smooths the edges of the 3D models with 2, 4 or 8 samples a pixel (as many as the GPU has). Costs GPU memory and time, more at a high resolution. "
             "OpenGL / Vita: not available.",
             "Só Metal (Mac): suaviza as bordas dos modelos 3D com 2, 4 ou 8 amostras por pixel (até o que a GPU tiver). Custa memória e tempo de GPU, mais em resolução alta. "
             "OpenGL / Vita: não disponível.",
             nullptr, &Tuning::msaa, 0.0f, 3.0f, 1.0f, "SOTC_MSAA"},
            {"texture_filter", "Texture filtering", "Filtro de texturas",
             "Nearest: hard square texels. Bilinear: smooth, one mip level. Trilinear: smooth between mip levels at the level the game picks by distance (the default). "
             "Anisotropic 2x-16x (Metal only; OpenGL uses trilinear): the GPU picks the level from each pixel's footprint, so far ground and slopes stay sharp at a high resolution.",
             "Mais próximo: texels quadrados. Bilinear: suave, um nível de mip. Trilinear: suave entre níveis de mip, no nível que o jogo escolhe pela distância (o padrão). "
             "Anisotrópico 2x-16x (só Metal; no OpenGL vale trilinear): a GPU escolhe o nível pela área de cada pixel, então chão distante e encostas ficam nítidos em resolução alta.",
             nullptr, &Tuning::textureFilter, 0.0f, 6.0f, 1.0f, "SOTC_TEXTURE_FILTER"},
            {"model_detail", "Model detail", "Detalhe dos modelos",
             "Which of a model's detail levels is drawn. Game's choice: the one the game picks by distance (the PS2's). Finer: one level more detailed. "
             "Finest: always the most detailed (far hills and colossi lose their blocky outlines; more geometry to draw).",
             "Qual nível de detalhe de cada modelo é desenhado. Escolha do jogo: o que o jogo escolhe pela distância (o do PS2). Mais fino: um nível mais detalhado. "
             "Máximo: sempre o mais detalhado (morros e colossos distantes perdem o contorno quadrado; mais geometria a desenhar).",
             nullptr, &Tuning::modelDetail, 0.0f, 2.0f, 1.0f, "SOTC_MODEL_DETAIL"},
            {"effect_detail", "Effect detail", "Detalhe dos efeitos",
             "Metal (Mac) only. PS2: the effects' buffers as the PS2 has them. High: the picture reduced for depth of field, distance fog, water and the "
             "glow by averaging all of it (at a high resolution one sample of it made far cliffs blocky and pixelated) and the mirror image on water "
             "and floors at 512 x 512 (the PS2's: 256). Very high: the mirror at 1024. OpenGL / Vita: always the PS2's.",
             "Só Metal (Mac). PS2: os buffers dos efeitos como no PS2. Alto: a imagem reduzida para profundidade de campo, névoa, água e brilho pela "
             "média de toda ela (em resolução alta uma amostra só deixava os penhascos distantes quadriculados e pixelados) e a imagem espelhada na "
             "água e no chão em 512 x 512 (a do PS2: 256). Muito alto: o espelho em 1024. OpenGL / Vita: sempre a do PS2.",
             nullptr, &Tuning::effectDetail, 0.0f, 2.0f, 1.0f, "SOTC_EFFECT_DETAIL"},
            {"bloom", "Bloom", "Bloom", "The glow around bright light and sky.", "O brilho em volta da luz forte e do céu.",
             &Tuning::bloom, nullptr, 0, 1, 1, "SOTC_BLOOM_SCALE"},
            {"bloom_strength", "Bloom strength", "Força do bloom",
             "Times the game's own strength, which it keeps raising and lowering by scene and camera.",
             "Vezes a força do próprio jogo, que ele aumenta e diminui conforme a cena e a câmera.",
             nullptr, &Tuning::bloomStrength, 0.0f, 2.0f, 0.02f, "SOTC_BLOOM_SCALE"},
            {"bloom_rounds", "Bloom blur", "Borrão do bloom", "Blur rounds: more spreads the glow wider. OpenGL / Vita only: Metal draws the PS2's own bloom chain, whose sizes are fixed (this applies there only with SOTC_BLOOM_OLD=1).",
             "Rodadas de borrão: mais espalha o brilho. Só OpenGL / Vita: o Metal desenha a própria cadeia de bloom do PS2, de tamanhos fixos (aqui só vale com SOTC_BLOOM_OLD=1).", nullptr, &Tuning::bloomRounds, 1.0f, 6.0f, 1.0f, "SOTC_BLOOM_ROUNDS"},
            {"bloom_spread", "Bloom spread", "Espalhamento", "How much each further blur round grows (from the second round). OpenGL / Vita only, like Bloom blur.",
             "Quanto cada rodada de borrão a mais cresce (a partir da segunda). Só OpenGL / Vita, como o Borrão do bloom.", nullptr, &Tuning::bloomSpread, 0.0f, 4.0f, 0.1f,
             "SOTC_BLOOM_SPREAD"},
            {"vertex_fog", "Distance fog", "Névoa por distância", "The PS2's fog that fades far things into the air colour.",
             "A névoa do PS2 que apaga o que está longe na cor do ar.", &Tuning::vertexFog, nullptr, 0, 1, 1, "SOTC_NATIVE_NOVFOG"},
            {"fog_amount", "Fog amount", "Quantidade de névoa", "Times the game's fog: 1 is the game's own.",
             "Vezes a névoa do jogo: 1 é a do próprio jogo.", nullptr, &Tuning::vertexFogAmount, 0.0f, 2.0f, 0.05f, "SOTC_FOG_AMOUNT"},
            {"zfog", "Depth fog", "Névoa de profundidade", "Far scenery fades into the sky.", "O cenário distante some no céu.",
             &Tuning::zFog, nullptr, 0, 1, 1, "SOTC_NATIVE_ZFOG"},
            {"glow", "Glow", "Brilho (glow)", "The cutscenes' glowing layer.", "A camada brilhante das cenas.", &Tuning::glow, nullptr, 0, 1, 1,
             "SOTC_NATIVE_NOGLOW"},
            {"dof", "Depth of field", "Profundidade de campo", "Far things blurred in some scenes.", "O fundo desfocado em algumas cenas.",
             &Tuning::depthOfField, nullptr, 0, 1, 1, "SOTC_NATIVE_NODOF"},
            {"shadows", "Shadows", "Sombras", "Characters' and colossi's shadows.", "As sombras dos personagens e colossos.", &Tuning::shadows,
             nullptr, 0, 1, 1, "SOTC_NATIVE_NOSHADOWS"},
            {"particles", "Particles", "Partículas", "Dust, sparks, birds, grass.", "Poeira, faíscas, pássaros, grama.", &Tuning::particles,
             nullptr, 0, 1, 1, "SOTC_NATIVE_NOPTCL"},
            {"screen_blur", "Screen blur", "Borrão de tela", "The motion and impact blurs.", "Os borrões de movimento e de impacto.",
             &Tuning::screenBlur, nullptr, 0, 1, 1, "SOTC_NATIVE_NOSBLUR"},
            {"feedback", "Motion trail", "Rastro", "The last frame laid over the new one (the eagle's flight, the title).",
             "O quadro anterior por cima do novo (o voo da águia, a tela-título).", &Tuning::feedback, nullptr, 0, 1, 1, "SOTC_NATIVE_NOFEEDBACK"},
            {"reflections", "Reflections", "Reflexos", "Mirror reflections on water and floors.", "Reflexos na água e no chão.",
             &Tuning::reflections, nullptr, 0, 1, 1, "SOTC_NATIVE_NOREFLECT"},
            {"refraction", "Water refraction", "Refração da água", "The scene seen through water.", "O cenário visto através da água.",
             &Tuning::refraction, nullptr, 0, 1, 1, "SOTC_NATIVE_NOREFRACT"},
            {"cap30", "Cap at 30 fps", "Limitar a 30 fps",
             "The picture changes at most every other screen refresh: steady 30 fps instead of a mix of 30 and 60 (or 20 and 30).",
             "A imagem muda no máximo a cada duas atualizações da tela: 30 fps estáveis em vez de uma mistura de 30 e 60 (ou 20 e 30).",
             &Tuning::frameCap30, nullptr, 0, 1, 1, "SOTC_CAP30"},
        };
        auto debugToggle = [&](const char *key, const char *label, const char *labelPt, const char *help, const char *helpPt, bool Tuning::*flag,
                               const char *env = nullptr)
        {
            TuningItem item{key, label, labelPt, help, helpPt, flag, nullptr, 0, 1, 1, env};
            item.debug = true;
            v.push_back(item);
        };
        // Round 20: the switches to flip on the console without a reboot,
        // first in the Debug group.
        {
            // The scene recorder (round 25), first in the Debug tab: one click,
            // then everything the renderer gets goes to disk from when the
            // menu closes.
            TuningItem item{"dbg_record", "Record scene", "Gravar cena",
                            "X: on, and recording starts when this menu closes (REC in the corner). Everything the renderer draws goes to "
                            "ux0:data/celosia/rec/<date> (logs/rec on the Mac): each frame's draw list and timing, the pad, and every few frames "
                            "the whole frame, to be drawn again on the Mac (tools/rec.py). It goes on through cutscenes and area changes and stops "
                            "on X again, after 15 minutes, at 700 MB or when the card is nearly full. The game may run a little slower while it records.",
                            "X: liga, e a gravação começa quando este menu fecha (REC no canto). Tudo o que o renderizador desenha vai para "
                            "ux0:data/celosia/rec/<data> (logs/rec no Mac): a lista de desenho e o tempo de cada quadro, o controle, e a cada "
                            "poucos quadros o quadro inteiro, pra ser desenhado de novo no Mac (tools/rec.py). Continua por cutscenes e trocas de "
                            "área e para com X de novo, depois de 15 minutos, em 700 MB ou com o cartão quase cheio. O jogo pode ficar um pouco "
                            "mais lento enquanto grava.",
                            nullptr, nullptr, 0, 1, 1, nullptr};
            item.action = native_recorder::toggle;
            item.actionText = native_recorder::statusText;
            item.debug = true;
            v.push_back(item);
        }
        {
            TuningItem item{"dbg_mark", "Mark this moment", "Marcar este momento",
                            "Writes \"[mark] <n> frame <f>\" to the log (err.txt on the Vita), to find this moment in it later (a hitch, a glitch).",
                            "Escreve \"[mark] <n> frame <f>\" no log (err.txt no Vita), pra achar este momento depois (um travamento, um defeito).",
                            nullptr, nullptr, 0, 1, 1, nullptr};
            item.action = writeMark;
            item.actionText = [](bool pt)
            {
                const int n = markCount();
                if (pt)
                    return std::string("X: marcar (") + std::to_string(n) + (n == 1 ? " marca no log)" : " marcas no log)");
                return std::string("X: mark (") + std::to_string(n) + (n == 1 ? " mark in the log)" : " marks in the log)");
            };
            item.debug = true;
            v.push_back(item);
        }
        debugToggle("dbg_perf_overlay", "Performance overlay", "Painel de desempenho",
                    "Over the picture, with the menu closed: fps, the frame's time split (game, hooks, render thread, wait, present) averaged over half a second, the worst frame of the last 4 s and a graph of the last 120 frames (the line: 33 ms). Costs nothing when off.",
                    "Sobre a imagem, com o menu fechado: fps, o tempo do quadro dividido (jogo, ganchos, thread de render, espera, apresentação) na média de meio segundo, o pior quadro dos últimos 4 s e um gráfico dos últimos 120 quadros (a linha: 33 ms). Não custa nada desligado.",
                    &Tuning::perfOverlay, "SOTC_PERF_OVERLAY");
        debugToggle("dbg_frame_log", "Frame timing log", "Log de tempo por quadro",
                    "Writes to the log every 60 frames where the time went (game, hooks, texture decoding, uploads, drawing, swap) and every slow frame.",
                    "Escreve no log a cada 60 quadros onde o tempo foi (jogo, ganchos, decodificação de texturas, envios, desenho, troca) e cada quadro lento.",
                    &Tuning::frameLog, "SOTC_FRAME_LOG");
        debugToggle("dbg_frame_log_all", "Log every frame", "Log de cada quadro",
                    "With the frame timing log: every frame on a line of its own, not only the slow ones (a lot of text: for a short moment).",
                    "Com o log de tempo por quadro: cada quadro numa linha própria, não só os lentos (muito texto: para um momento curto).",
                    &Tuning::frameLogAll, "SOTC_FRAME_LOG_ALL");
        {
            TuningItem item{"dbg_audio_log", "Sound log", "Log de som",
                            "0 off; 1 every 5 s an [audio] line (streams, voices, disc reads, the mixer) and a [loader] line; 2 also each stream event. The [loader] line's message boxes only when on at start.",
                            "0 desligado; 1 a cada 5 s uma linha [audio] (streams, vozes, leituras do disco, o mixer) e uma [loader]; 2 também cada evento de stream. As caixas de mensagem da linha [loader] só se ligado no início.",
                            nullptr, &Tuning::audioLog, 0.0f, 2.0f, 1.0f, "SOTC_AUDIO_LOG"};
            item.debug = true;
            v.push_back(item);
        }
        {
            TuningItem item{"dbg_glow_vita", "Glow layer (Vita)", "Camada de glow (Vita)",
                            "OpenGL / Vita: the glow layer (layer 8: the dead colossus, the sigils, the shrine's shadow figures). Plain: drawn into the picture as is (round 18). Auto: blurred and tinted apart on the console, plain in Vita3K. Always: apart in Vita3K too (it draws the picture below layer 8 twice there).",
                            "OpenGL / Vita: a camada de glow (camada 8: o colosso morto, os sigilos, as sombras do santuário). Simples: desenhada direto na imagem (rodada 18). Auto: borrada e tingida à parte no console, simples no Vita3K. Sempre: à parte no Vita3K também (lá desenha duas vezes a imagem abaixo da camada 8).",
                            nullptr, &Tuning::glowVita, 0.0f, 2.0f, 1.0f, "SOTC_GLOW_VITA"};
            item.debug = true;
            v.push_back(item);
        }
        debugToggle("dbg_bloom_sameframe", "Bloom in the same frame", "Bloom no mesmo quadro",
                    "OpenGL / Vita: the bloom's glow added in the frame it was made from (as the PS2). Off: one frame late (round 18; one less split of the frame). Always late where the picture is redrawn for copies (Vita3K).",
                    "OpenGL / Vita: o brilho do bloom somado no mesmo quadro de onde saiu (como no PS2). Desligado: um quadro atrasado (rodada 18; uma divisão a menos do quadro). Sempre atrasado onde a imagem é redesenhada para cópias (Vita3K).",
                    &Tuning::bloomSameFrame, "SOTC_BLOOM_SAMEFRAME");
        debugToggle("dbg_show_native", "Show native picture", "Mostrar imagem nativa",
                    "Off: the emulated PS2 picture of the same moment (F9 flips it), to compare the two.",
                    "Desligado: a imagem emulada do PS2 no mesmo instante (F9 alterna), pra comparar as duas.", &Tuning::showNative);
        debugToggle("dbg_2d", "Draw 2D", "Desenhar 2D", "Sprites, fades, menus, subtitles.", "Sprites, fades, menus, legendas.", &Tuning::draw2D);
        debugToggle("dbg_world", "Draw world", "Desenhar mundo", "Everything that is not 2D.", "Tudo que não é 2D.", &Tuning::drawWorld);
        debugToggle("dbg_textures", "Textures", "Texturas", "Off: every draw at its vertex colour, no texture.",
                    "Desligado: tudo na cor do vértice, sem textura.", &Tuning::textures);
        debugToggle("dbg_lighting", "Lighting", "Iluminação", "Off: lit models at their vertex colour.", "Desligado: modelos iluminados na cor do vértice.",
                    &Tuning::lighting);
        debugToggle("dbg_additive", "Additive draws", "Desenhos aditivos", "Draws that add light (glows, light shafts).",
                    "Desenhos que somam luz (brilhos, feixes).", &Tuning::additive);
        debugToggle("dbg_subtract", "Subtract draws", "Desenhos subtrativos", "Draws that darken by subtracting.", "Desenhos que escurecem subtraindo.",
                    &Tuning::subtract);
        debugToggle("dbg_overlays", "Second passes", "Segundas passadas", "A second pass drawn over the same geometry.",
                    "Uma segunda passada desenhada sobre a mesma geometria.", &Tuning::overlays);
        debugToggle("dbg_raw_mirror", "Raw mirror image", "Espelho cru", "Water shows the mirror image as it is, without the game's tint.",
                    "A água mostra a imagem espelhada como ela é, sem o tom do jogo.", &Tuning::rawMirror);
        debugToggle("dbg_sky", "Sky image", "Imagem do céu", "The sky image laid behind the scene.", "A imagem do céu posta atrás da cena.", &Tuning::skyImage);
        for (const auto &[key, label, labelPt, member, env] :
             {std::tuple<const char *, const char *, const char *, float Tuning::*, const char *>{"dbg_haze_source", "Heat haze reads", "Calor lê", &Tuning::hazeSource, "SOTC_HAZE_SOURCE"},
              {"dbg_water_source", "Water reads", "Água lê", &Tuning::waterSource, "SOTC_WATER_SOURCE"}})
        {
            TuningItem item{key, label, labelPt,
                            "The picture this effect bends: small copy (the PS2's), full copy, or last frame (OpenGL / Vita only).",
                            "A imagem que este efeito distorce: cópia pequena (a do PS2), cópia inteira ou quadro anterior (só OpenGL / Vita).",
                            nullptr, member, 0.0f, 2.0f, 1.0f, env};
            item.debug = true;
            v.push_back(item);
        }
        {
            // A new key: the earlier on / off one ("dbg_redraw") was saved on
            // by default and would keep it on on the console.
            TuningItem item{"dbg_redraw_mode", "Redraw for copies", "Redesenhar para cópias",
                            "OpenGL / Vita: water, heat haze and screen blur get the picture so far drawn again apart, instead of leaving the picture and taking it up again (white screens in Vita3K). Costs those draws twice in such frames. 0 off, 1 on, 2 only in Vita3K (the console copies at the layer).",
                            "OpenGL / Vita: água, calor e borrão de tela recebem a imagem até ali desenhada de novo à parte, em vez de largar a imagem e retomá-la (telas brancas no Vita3K). Custa esses desenhos duas vezes nesses quadros. 0 desligado, 1 ligado, 2 só no Vita3K (o console copia na camada).",
                            nullptr, &Tuning::midFrameRedraw, 0.0f, 2.0f, 1.0f, "SOTC_GL_REDRAW"};
            item.debug = true;
            v.push_back(item);
        }
        debugToggle("dbg_mips", "Mipmaps", "Mipmaps", "Smaller textures far away.", "Texturas menores ao longe.", &Tuning::mipmaps, "SOTC_NATIVE_NOMIPS");
        debugToggle("dbg_scroll", "Texture scrolling", "Rolagem de textura", "Water and cloth textures that move.", "Texturas que se movem (água, tecido).",
                    &Tuning::textureScroll, "SOTC_NATIVE_NOROW");
        debugToggle("dbg_subsys_prof", "Subsystem timing log", "Log de tempo por subsistema",
                    "Writes to the log every 120 frames the game thread's time by subsystem (display walk, characters, AI, collision, camera, particles, the hooks' parts, each task's step), and once at start a probe of the memory and call costs. The functions are wrapped at start only when this is on then (or SOTC_SUBSYS_PROF=1); off at run time: paused.",
                    "Escreve no log a cada 120 quadros o tempo da thread do jogo por subsistema (percurso de exibição, personagens, IA, colisão, câmera, partículas, as partes dos ganchos, o passo de cada tarefa) e uma vez no início uma sondagem dos custos de memória e de chamadas. As funções são embrulhadas no início só se isto estiver ligado então (ou SOTC_SUBSYS_PROF=1); desligado durante o jogo: pausado.",
                    &Tuning::subsysProf, "SOTC_SUBSYS_PROF");
        debugToggle("dbg_lean", "Lean hot code", "Código quente enxuto",
                    "On the Vita by default (SOTC_LEAN=0 in env.txt: never): the ~1 850 hottest game functions run from copies packed together in memory. Off: the original copies, to compare speed (the picture should not change).",
                    "No Vita por padrão (SOTC_LEAN=0 no env.txt: nunca): as ~1 850 funções mais quentes do jogo rodam de cópias juntas na memória. Desligado: as cópias originais, pra comparar a velocidade (a imagem não deve mudar).",
                    &Tuning::leanCopies);
        debugToggle("dbg_ee_trust", "Plain floats (lean code)", "Floats simples (código enxuto)",
                    "With lean hot code: its second copy, with plain floats: the PS2 float emulation leaves out the operand clamps of each VU0 / FPU add, subtract and multiply and the PS2 adder's operand alignment (results can differ in the last bit). ~3% faster; not exact, so off by default.",
                    "Com código quente enxuto: a segunda cópia, com floats simples: a emulação de float do PS2 deixa de limitar os operandos de cada soma, subtração e multiplicação VU0 / FPU e de alinhar os operandos como o somador do PS2 (resultados podem diferir no último bit). ~3% mais rápido; não exato, por isso desligado por padrão.",
                    &Tuning::trustFinite, "SOTC_EE_TRUST");
        debugToggle("dbg_fast_hooks", "Fast hooks", "Ganchos rápidos",
                    "Characters posed by the faster path and textures looked up once a frame. Off: the earlier code, to compare (the picture should not change).",
                    "Personagens posados pelo caminho mais rápido e texturas buscadas uma vez por quadro. Desligado: o código anterior, pra comparar (a imagem não deve mudar).",
                    &Tuning::fastHooks, "SOTC_NATIVE_SLOWHOOKS");
        debugToggle("dbg_packet_quick", "Quick packet check", "Checagem rápida de pacotes",
                    "With fast hooks: a model packet the game never rewrote is taken as it was while drawn frame after frame (a few of its words checked after a gap, fully one frame in 16); off: the full sample every frame.",
                    "Com ganchos rápidos: um pacote de modelo que o jogo nunca reescreveu é tomado como estava enquanto desenhado quadro após quadro (algumas palavras checadas depois de uma pausa, por inteiro um quadro em 16); desligado: a amostra inteira a cada quadro.",
                    &Tuning::packetQuickCheck, "SOTC_PACKET_QUICK");
        debugToggle("dbg_ps2_lists", "PS2 display lists", "Listas de exibição do PS2",
                    "On: the game also builds its PS2 display lists (VIF packets of every model for the VU1, texture uploads, the screen effects' sprites), which nothing draws, as before. Off: only what the game reads back is kept (faster).",
                    "Ligado: o jogo também monta suas listas de exibição do PS2 (pacotes VIF de cada modelo para a VU1, envios de textura, os sprites dos efeitos de tela), que nada desenha, como antes. Desligado: só o que o jogo lê de volta é mantido (mais rápido).",
                    &Tuning::ps2Lists, "SOTC_PS2_LISTS");
        debugToggle("dbg_gpu_skin", "GPU skinning", "Skinning na GPU",
                    "OpenGL / Vita: characters posed by the GPU from their bind pose (off: posed on the CPU every frame). Metal always poses on the CPU.",
                    "OpenGL / Vita: personagens posados pela GPU a partir da pose base (desligado: posados na CPU a cada quadro). O Metal sempre posa na CPU.",
                    &Tuning::gpuSkinning, "SOTC_GPU_SKIN");
        debugToggle("dbg_render_thread", "Render thread", "Thread de renderização",
                    "OpenGL / Vita: each frame drawn and shown on a thread of its own while the game makes the next (off: the game waits for each frame to be drawn).",
                    "OpenGL / Vita: cada quadro desenhado e mostrado numa thread própria enquanto o jogo faz o próximo (desligado: o jogo espera cada quadro ser desenhado).",
                    &Tuning::renderThread, "SOTC_RENDER_THREAD");
        debugToggle("dbg_async_builds", "Builds on a worker", "Montagem em outra thread",
                    "Textures decoded and model meshes built on a worker thread instead of in the game's own time (a camera cut or a new area no longer freezes the game for them); what is not ready yet is left out of a frame or two.",
                    "Texturas decodificadas e malhas de modelos montadas numa thread à parte em vez de no tempo do próprio jogo (um corte de câmera ou área nova não trava mais o jogo por elas); o que ainda não ficou pronto fica de fora por um ou dois quadros.",
                    &Tuning::asyncBuilds, "SOTC_ASYNC_BUILDS");
        debugToggle("dbg_hook_worker", "Model draws on a worker", "Desenho dos modelos em outra thread",
                    "The models' draws worked out (packets, palettes, skinning) on a thread of their own while the game goes on; the game sees nothing of it. Off: in the game's own time, as before round 23.",
                    "Os desenhos dos modelos (pacotes, paletas, skinning) calculados numa thread própria enquanto o jogo continua; o jogo não vê nada disso. Desligado: no tempo do próprio jogo, como antes da rodada 23.",
                    &Tuning::hookWorker, "SOTC_HOOK_ASYNC");
        debugToggle("dbg_hold_incomplete", "Hold incomplete frames", "Segurar quadros incompletos",
                    "With builds on a worker: a frame missing more than an eighth of its draws (still being built, at a camera cut) is not shown; the last one stays up (at most 3 frames).",
                    "Com montagem em outra thread: um quadro sem mais de um oitavo dos desenhos (ainda montando, num corte de câmera) não é mostrado; o anterior fica na tela (no máximo 3 quadros).",
                    &Tuning::holdIncomplete, "SOTC_HOLD_INCOMPLETE");
        {
            TuningItem item{"dbg_hold_frames", "Frames held at most", "Quadros segurados no máximo",
                            "With hold incomplete frames: how many frames in a row the last picture may stay up (3).",
                            "Com segurar quadros incompletos: quantos quadros seguidos a imagem anterior pode ficar na tela (3).",
                            nullptr, &Tuning::holdFrames, 0.0f, 8.0f, 1.0f, "SOTC_HOLD_FRAMES"};
            item.debug = true;
            v.push_back(item);
        }
        debugToggle("dbg_warm_shaders", "Warm shaders at start", "Preparar shaders no início",
                    "OpenGL / Vita: each shader drawn once with every blend state when the renderer starts, so their first use in the game does not stall (takes effect at the next start).",
                    "OpenGL / Vita: cada shader desenhado uma vez com cada modo de mistura quando o renderizador começa, pra que o primeiro uso no jogo não trave (vale a partir do próximo início).",
                    &Tuning::warmShaders, "SOTC_GL_WARM");
        debugToggle("dbg_gl_pools", "Shared mesh buffers", "Buffers de malha compartilhados",
                    "OpenGL / Vita: models' meshes packed into a few large buffers, set up once each instead of once per mesh (applies to meshes uploaded after the change).",
                    "OpenGL / Vita: as malhas dos modelos guardadas em poucos buffers grandes, configurados uma vez cada em vez de uma vez por malha (vale para as malhas enviadas depois da mudança).",
                    &Tuning::glMeshPools, "SOTC_GL_POOLS");
        debugToggle("dbg_gl_merge", "Merge draws", "Juntar desenhos",
                    "OpenGL / Vita: consecutive draws with the same texture, blending and matrix drawn as one (the picture does not change).",
                    "OpenGL / Vita: desenhos seguidos com a mesma textura, mistura e matriz desenhados como um só (a imagem não muda).",
                    &Tuning::glMergeDraws, "SOTC_GL_MERGE");
        debugToggle("dbg_gl_lean", "Lean shaders", "Shaders enxutos",
                    "OpenGL / Vita: plain world draws through shaders that read only the textures they use (the picture does not change).",
                    "OpenGL / Vita: desenhos comuns do mundo com shaders que leem só as texturas que usam (a imagem não muda).",
                    &Tuning::glLeanPrograms, "SOTC_GL_LEAN");
        debugToggle("dbg_gl_nodiscard", "Opaque shader variant", "Variante opaca do shader",
                    "OpenGL / Vita, with lean shaders: draws that cannot have a cut-out pixel use a shader without the cut-out test (faster on the Vita's GPU; the picture does not change).",
                    "OpenGL / Vita, com shaders enxutos: desenhos que não podem ter pixel recortado usam um shader sem o teste de recorte (mais rápido na GPU do Vita; a imagem não muda).",
                    &Tuning::glNoDiscard, "SOTC_GL_NODISCARD");
        static char labels[32][2][24];
        for (int i = 0; i < 32; ++i)
        {
            std::snprintf(labels[i][0], sizeof(labels[i][0]), "Layer %d", i);
            std::snprintf(labels[i][1], sizeof(labels[i][1]), "Camada %d", i);
            TuningItem item{nullptr, labels[i][0], labels[i][1], "Draws of this display list layer (1 sky, 2-3 world, 8 glow, 12-15 mirror, 19 refraction, 29 trail).",
                            "Desenhos desta camada da lista de exibição (1 céu, 2-3 mundo, 8 glow, 12-15 espelho, 19 refração, 29 rastro).",
                            nullptr, nullptr, 0, 1, 1, "SOTC_HIDE_LAYERS"};
            static char keys[32][24]; // "dbg_hide_layer31" and its terminator
            std::snprintf(keys[i], sizeof(keys[i]), "dbg_hide_layer%d", i); // saved 1 = hidden
            item.key = keys[i];
            item.mask = &Tuning::hiddenLayers;
            item.bit = i;
            item.debug = true;
            v.push_back(item);
        }
        // The groups, and where each item changes anything.
        for (TuningItem &item : v)
        {
            const std::string key = item.key;
            item.quality = key == "quality_preset" || key == "render_scale" || key == "msaa" || key == "texture_filter" || key == "model_detail" ||
                           key == "effect_detail";
            if (key == "msaa" || key == "effect_detail")
                item.backends = 1;
            if (key == "bloom_rounds" || key == "bloom_spread" || key == "dbg_haze_source" || key == "dbg_water_source" || key == "dbg_redraw_mode" || key == "dbg_glow_vita" || key == "dbg_bloom_sameframe" ||
                key == "dbg_gpu_skin" || key == "dbg_render_thread" || key == "dbg_gl_pools" || key == "dbg_gl_merge" || key == "dbg_gl_lean" ||
                key == "dbg_gl_nodiscard")
                item.backends = 2;
        }
        return v;
        }();
        return items;
    }

    // The presets' values: render scale, MSAA, texture filter, model
    // detail, effect detail. Medium is the Vita's old default, High the
    // Mac's (nothing changes until another is picked).
    namespace
    {
        struct PresetValues
        {
            float renderScale, msaa, textureFilter, modelDetail, effectDetail;
        };
        constexpr PresetValues kPresets[4] = {
            {0.5f, 0.0f, 1.0f, 0.0f, 0.0f}, // Low: half the PS2's resolution, bilinear
            {1.0f, 0.0f, 2.0f, 0.0f, 0.0f}, // Medium: the PS2's 512 x 448
            {3.0f, 0.0f, 2.0f, 0.0f, 0.0f}, // High: 3x
            {4.0f, 2.0f, 6.0f, 2.0f, 1.0f}, // Ultra: 4x, 4x MSAA, 16x anisotropic, the finest models, high effect detail
        };

        // Named values: count, English, Portuguese.
        struct Choices
        {
            int count;
            const char *const *en, *const *pt;
        };
        Choices choicesOf(const TuningItem &item)
        {
            static const char *presetEn[] = {"Low", "Medium", "High", "Ultra", "Custom"};
            static const char *presetPt[] = {"Baixo", "Médio", "Alto", "Ultra", "Personalizado"};
            static const char *msaaEn[] = {"Off", "2x", "4x", "8x"};
            static const char *msaaPt[] = {"Desligado", "2x", "4x", "8x"};
            static const char *filterEn[] = {"Nearest", "Bilinear", "Trilinear", "Anisotropic 2x", "Anisotropic 4x", "Anisotropic 8x", "Anisotropic 16x"};
            static const char *filterPt[] = {"Mais próximo", "Bilinear", "Trilinear", "Anisotrópico 2x", "Anisotrópico 4x", "Anisotrópico 8x", "Anisotrópico 16x"};
            static const char *detailEn[] = {"Game's choice", "Finer", "Finest"};
            static const char *detailPt[] = {"Escolha do jogo", "Mais fino", "Máximo"};
            static const char *effectEn[] = {"PS2", "High", "Very high"};
            static const char *effectPt[] = {"PS2", "Alto", "Muito alto"};
            static const char *glowEn[] = {"Plain", "Auto", "Always"};
            static const char *glowPt[] = {"Simples", "Auto", "Sempre"};
            static const char *audioEn[] = {"Off", "Summary", "Summary + events"};
            static const char *audioPt[] = {"Desligado", "Resumo", "Resumo + eventos"};
            static const char *sourceEn[] = {"small copy", "full copy", "last frame"};
            static const char *sourcePt[] = {"cópia pequena", "cópia inteira", "quadro anterior"};
            if (item.number == &Tuning::preset)
                return {5, presetEn, presetPt};
            if (item.number == &Tuning::msaa)
                return {4, msaaEn, msaaPt};
            if (item.number == &Tuning::textureFilter)
                return {7, filterEn, filterPt};
            if (item.number == &Tuning::modelDetail)
                return {3, detailEn, detailPt};
            if (item.number == &Tuning::effectDetail)
                return {3, effectEn, effectPt};
            if (item.number == &Tuning::glowVita)
                return {3, glowEn, glowPt};
            if (item.number == &Tuning::audioLog)
                return {3, audioEn, audioPt};
            if (item.number == &Tuning::hazeSource || item.number == &Tuning::waterSource)
                return {3, sourceEn, sourcePt};
            return {0, nullptr, nullptr};
        }
    }

    void applyPreset(Tuning &t, int preset)
    {
        if (preset < 0 || preset >= kPresetCustom)
            return;
        const PresetValues &p = kPresets[preset];
        t.renderScale = std::clamp(p.renderScale, kMinRenderScale, kMaxRenderScale); // the Vita: at most 2x (kMaxRenderScale)
        t.msaa = p.msaa;
        t.textureFilter = p.textureFilter;
        t.modelDetail = p.modelDetail;
        t.effectDetail = p.effectDetail;
        t.preset = static_cast<float>(preset);
    }

    static bool presetMatches(const Tuning &t, int i)
    {
        const PresetValues &p = kPresets[i];
        return t.renderScale == std::clamp(p.renderScale, kMinRenderScale, kMaxRenderScale) && t.msaa == p.msaa && t.textureFilter == p.textureFilter &&
               t.modelDetail == p.modelDetail && t.effectDetail == p.effectDetail;
    }
    int matchingPreset(const Tuning &t)
    {
        // The one picked last when it still matches (should two presets
        // ever have the same values on a platform, cycling still moves on).
        const int last = static_cast<int>(t.preset + 0.5f);
        if (last >= 0 && last < kPresetCustom && presetMatches(t, last))
            return last;
        for (int i = 0; i < kPresetCustom; ++i)
            if (presetMatches(t, i))
                return i;
        return kPresetCustom;
    }

    int tuningChoices(const TuningItem &item) { return choicesOf(item).count; }

    std::string tuningValueText(const TuningItem &item, bool portuguese)
    {
        const Tuning &t = tuning();
        if (item.action)
            return item.actionText ? item.actionText(portuguese) : "";
        if (item.toggle)
            return t.*item.toggle ? "On" : "Off";
        if (item.mask)
            return ((t.*item.mask >> item.bit) & 1u) ? "Off" : "On";
        char b[48];
        const float v = t.*item.number;
        if (const Choices c = choicesOf(item); c.count)
        {
            const int i = item.number == &Tuning::preset ? matchingPreset(t) : std::clamp(static_cast<int>(v + 0.5f), 0, c.count - 1);
            return (portuguese ? c.pt : c.en)[i];
        }
        if (item.number == &Tuning::renderScale)
        {
            if (v == 1.0f)
                std::snprintf(b, sizeof(b), portuguese ? "1x nativa (512x448)" : "1x native (512x448)");
            else
                std::snprintf(b, sizeof(b), "%gx (%dx%d)", v, static_cast<int>(512 * v + 0.5f), static_cast<int>(448 * v + 0.5f));
        }
        else
            std::snprintf(b, sizeof(b), item.step >= 1.0f ? "%.0f" : "%.2f", v);
        return b;
    }

    void tuningChanged(const TuningItem &item)
    {
        Tuning &t = tuning();
        if (item.number == &Tuning::preset)
        {
            const int p = std::clamp(static_cast<int>(t.preset + 0.5f), 0, static_cast<int>(kPresetCustom));
            if (p != kPresetCustom)
                applyPreset(t, p);
        }
        t.preset = static_cast<float>(matchingPreset(t));
    }

    void tuningStep(const TuningItem &item, int dir)
    {
        Tuning &t = tuning();
        if (item.action)
        {
            item.action();
            return;
        }
        if (item.toggle)
        {
            t.*item.toggle = !(t.*item.toggle);
            return;
        }
        if (item.mask)
        {
            t.*item.mask ^= 1u << item.bit;
            return;
        }
        if (item.number == &Tuning::preset)
        {
            // Low .. Ultra, round; from Custom to the platform's own.
            int p = matchingPreset(t);
            p = p == kPresetCustom ? static_cast<int>(defaultTuning().preset) : (p + (dir > 0 ? 1 : kPresetCustom - 1)) % kPresetCustom;
            applyPreset(t, p);
            return;
        }
        float &v = t.*item.number;
        const float n = std::round((v + item.step * static_cast<float>(dir > 0 ? 1 : -1)) / item.step) * item.step;
        v = std::clamp(n, item.min, item.max);
        t.preset = static_cast<float>(matchingPreset(t));
    }

    namespace
    {
        std::atomic<int> g_marks{0};
        std::mutex g_perfMutex;
        PerfStats g_perf;
        bool g_perfSet = false;
    }
    void writeMark()
    {
        const int n = g_marks.fetch_add(1, std::memory_order_relaxed) + 1;
        std::fprintf(stderr, "[mark] %d frame %llu vblank %llu t=%.2f\n", n, static_cast<unsigned long long>(ps2_stats::vif1Chains.load(std::memory_order_relaxed)),
                     static_cast<unsigned long long>(ps2_stats::vblanks.load(std::memory_order_relaxed)), static_cast<double>(nowUs()) / 1e6);
        std::fflush(stderr);
    }
    int markCount() { return g_marks.load(std::memory_order_relaxed); }
    void perfPublish(const PerfStats &stats)
    {
        std::lock_guard<std::mutex> lock(g_perfMutex);
        g_perf = stats;
        g_perfSet = true;
    }
    bool perfSnapshot(PerfStats &out)
    {
        std::lock_guard<std::mutex> lock(g_perfMutex);
        if (!g_perfSet)
            return false;
        out = g_perf;
        return true;
    }

    bool tuningRestore(const char *key, float value)
    {
        for (const TuningItem &item : tuningItems())
        {
            if (std::strcmp(item.key, key) != 0)
                continue;
            if (item.action)
                return true; // (never saved)
            if (item.env && std::getenv(item.env))
                return false;
            if (item.toggle)
                tuning().*item.toggle = value != 0.0f;
            else if (item.mask)
                tuning().*item.mask = (tuning().*item.mask & ~(1u << item.bit)) | (value != 0.0f ? 1u << item.bit : 0u);
            else
                tuning().*item.number = std::clamp(value, item.min, item.max);
            if (item.number == &Tuning::preset)
            {
                tuningChanged(item); // the preset (saved first) applies; the items saved after it override,
                qualityFromEnv(tuning()); // and so do the variables set at start
            }
            tuning().preset = static_cast<float>(matchingPreset(tuning()));
            return true;
        }
        return false;
    }
}

namespace
{
    // SOTC_NATIVE_GL=check (macOS): Metal draws what is shown and OpenGL the
    // same frames alongside; every 60 frames the two pictures are compared
    // ([native:check] mean difference per channel, and the share of pixels
    // off by more than 8), the GL one written next to it (PS2X_NATIVE_CHECK_DIR).
    class CheckBackend final : public native_renderer::detail::Backend
    {
    public:
        CheckBackend(native_renderer::detail::Backend *shown, native_renderer::detail::Backend *other) : a(shown), b(other) {}
        bool hasMesh(uint64_t id) override { return a->hasMesh(id) && b->hasMesh(id); }
        uint64_t meshEvictions() override { return a->meshEvictions() + b->meshEvictions(); }
        void uploadMesh(uint64_t id, const native_renderer::Vertex *v, uint32_t n, bool f) override { a->uploadMesh(id, v, n, f), b->uploadMesh(id, v, n, f); }
        bool hasTexture(uint64_t id) override { return a->hasTexture(id) && b->hasTexture(id); }
        void uploadTexture(uint64_t id, uint32_t w, uint32_t h, const uint8_t *p, uint32_t l) override { a->uploadTexture(id, w, h, p, l), b->uploadTexture(id, w, h, p, l); }
        void beginFrame() override { a->beginFrame(), b->beginFrame(); }
        void draw(uint64_t m, const float c[16], const native_renderer::DrawState &s) override { a->draw(m, c, s), b->draw(m, c, s); }
        void setFog(const native_renderer::Fog &f) override { a->setFog(f), b->setFog(f); }
        void setBloom(const native_renderer::Bloom &x) override { a->setBloom(x), b->setBloom(x); }
        void addScreenBlur(const native_renderer::ScreenBlur &x) override { a->addScreenBlur(x), b->addScreenBlur(x); }
        void setGlowComposite(const native_renderer::GlowComposite &x) override { a->setGlowComposite(x), b->setGlowComposite(x); }
        void setDepthOfField(const native_renderer::DepthOfField &x) override { a->setDepthOfField(x), b->setDepthOfField(x); }
        void endFrame(uint32_t w, uint32_t h) override
        {
            a->endFrame(w, h);
            b->endFrame(w, h);
            b->finishFrames(); // the same frame from both
            if (++m_frames % 60u != 0u)
                return;
            std::vector<uint8_t> x, y;
            uint32_t xw = 0, xh = 0, yw = 0, yh = 0;
            if (!a->latestFrame(x, xw, xh) || !b->latestFrame(y, yw, yh) || xw != yw || xh != yh)
                return;
            uint64_t sum = 0, off = 0;
            for (size_t i = 0; i < x.size(); i += 4)
            {
                int worst = 0;
                for (int c = 0; c < 3; ++c)
                {
                    const int d = std::abs(static_cast<int>(x[i + c]) - static_cast<int>(y[i + c]));
                    sum += static_cast<uint64_t>(d);
                    worst = std::max(worst, d);
                }
                off += worst > 8 ? 1u : 0u;
            }
            const double pixels = static_cast<double>(x.size() / 4u);
            std::cerr << "[native:check] frame " << m_frames << " mean " << static_cast<double>(sum) / (pixels * 3.0) << " off>8 " << 100.0 * static_cast<double>(off) / pixels
                      << "%" << std::endl;
            if (const char *dir = std::getenv("PS2X_NATIVE_CHECK_DIR"))
            {
                for (int k = 0; k < 2; ++k)
                {
                    const std::string path = std::string(dir) + "/" + (k ? "gl_" : "metal_") + std::to_string(m_frames) + ".rgba";
                    if (FILE *f = std::fopen(path.c_str(), "wb"))
                    {
                        const auto &p = k ? y : x;
                        std::fwrite(p.data(), 1, p.size(), f);
                        std::fclose(f);
                    }
                }
            }
        }
        bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &w, uint32_t &h) override { return a->latestFrame(rgba, w, h); }

    private:
        native_renderer::detail::Backend *a, *b;
        uint64_t m_frames = 0;
    };

    native_renderer::detail::Backend *g_backend = nullptr;
    bool g_tried = false;
    std::atomic<bool> g_shown{false};
    std::function<void()> g_glMakeCurrent;
    std::function<void(uint32_t, uint32_t, uint32_t)> g_presentHook;

    bool wantGl()
    {
#if defined(__vita__)
        return true;
#else
        const char *gl = std::getenv("SOTC_NATIVE_GL");
        return gl && (std::atoi(gl) != 0 || std::string(gl) == "check");
#endif
    }

    // OpenGL: created by the first call after available() (the Vita: by
    // setGlContext, on the main thread); the context is made current on the
    // backend's render thread, which makes every GL call from then on.
    native_renderer::detail::Backend *backend()
    {
        if (!g_tried)
        {
            g_tried = true;
            if (wantGl() && g_glMakeCurrent)
            {
                ps2_stats::bootMark("native renderer: building");
                g_backend = native_renderer::detail::createGlBackend(g_glMakeCurrent);
                ps2_stats::bootMark("native renderer: built");
                std::cerr << "[native] OpenGL renderer " << (g_backend ? "up" : "failed") << std::endl;
                const char *mode = std::getenv("SOTC_NATIVE_GL");
                if (g_backend && mode && std::string(mode) == "check")
                {
                    if (auto *metal = native_renderer::detail::createMetalBackend())
                        g_backend = new CheckBackend(metal, g_backend);
                }
            }
            if (!g_backend && !wantGl())
                g_backend = native_renderer::detail::createMetalBackend();
        }
        return g_backend;
    }
}

namespace native_renderer::detail
{
    const std::function<void(uint32_t, uint32_t, uint32_t)> &presentHook() { return g_presentHook; }
    std::atomic<bool> g_inEmulator{false};
    void setInEmulator(bool emulator) { g_inEmulator.store(emulator, std::memory_order_relaxed); }
}

namespace native_renderer
{
    const char *backendName() { return wantGl() ? "OpenGL" : "Metal"; }
    bool inEmulator() { return detail::g_inEmulator.load(std::memory_order_relaxed); }
    bool tuningApplies(const TuningItem &item) { return (item.backends & (wantGl() ? 2u : 1u)) != 0u; }

    void setGlContext(std::function<void()> makeCurrent)
    {
        g_glMakeCurrent = std::move(makeCurrent);
#if defined(__vita__)
        // The CG compiler needs the main thread's stack: build it now.
        backend();
#endif
    }
    void setPresentHook(std::function<void(uint32_t, uint32_t, uint32_t)> hook) { g_presentHook = std::move(hook); }

    bool available()
    {
        if (!g_tried && wantGl())
            return static_cast<bool>(g_glMakeCurrent); // not created here: this may not be the game thread
        return backend() != nullptr;
    }
    FrameCounters &frameCounters()
    {
        static FrameCounters counters;
        return counters;
    }
    uint64_t nowUs()
    {
#if defined(__vita__)
        return sceKernelGetProcessTimeWide(); // no 64-bit division (see audio_stats::nowUs)
#endif
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    }
    namespace threads
    {
        int layout()
        {
            // Read at the first call (a thread's start: after env.txt).
            static const int v = []
            {
                const char *e = std::getenv("SOTC_THREADS");
                return e ? std::atoi(e) : 1;
            }();
            return v;
        }
#if defined(__vita__)
        namespace
        {
            // The noted threads (a kind may have several: the disc readers,
            // the recorder's two): slots taken once, read by the game thread.
            constexpr int kSlots = 24;
            struct Slot
            {
                int kind = 0;
                SceUID id = 0;
                uint64_t clocks = 0;
                uint32_t preempts = 0, moves = 0;
                bool seen = false;
            };
            Slot g_slots[kSlots];
            std::atomic<int> g_slotCount{0};
            std::mutex g_slotMutex;
            uint64_t g_cpuUs[kKinds] = {}, g_preempts[kKinds] = {}, g_moves[kKinds] = {};
            uint64_t g_lastCpuUs[kKinds] = {}, g_lastPreempts[kKinds] = {}, g_lastMoves[kKinds] = {}; // the last sample's alone
            const char *const kNames[kKinds] = {"game", "worker", "render", "build", "audio", "main", "loader", "recorder", "log"};
        }
        void place(Kind kind)
        {
            const SceUID self = sceKernelGetThreadId();
            {
                std::lock_guard<std::mutex> lock(g_slotMutex);
                const int n = g_slotCount.load(std::memory_order_relaxed);
                bool known = false;
                for (int i = 0; i < n; ++i)
                    known |= g_slots[i].id == self;
                if (!known && n < kSlots)
                {
                    g_slots[n].kind = kind;
                    g_slots[n].id = self;
                    g_slotCount.store(n + 1, std::memory_order_release);
                }
            }
            const bool r33 = layout() != 0;
            int mask = 0, prio = 0;
            switch (kind)
            {
            case kGame:
                mask = r33 ? SCE_KERNEL_CPU_MASK_USER_0 : SCE_KERNEL_CPU_MASK_USER_0 | SCE_KERNEL_CPU_MASK_USER_1;
                break;
            case kWorker:
                mask = SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2;
                if (r33)
                {
                    const char *e = std::getenv("SOTC_HOOK_WORKER_PRIO");
                    prio = e ? std::atoi(e) : 150;
                }
                break;
            case kRender:
                mask = SCE_KERNEL_CPU_MASK_USER_2;
                break;
            case kBuild:
            case kRecorder:
                mask = SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2;
                break;
            case kLog:
                // The default priority (the pthreads' own; it only has to
                // keep up over seconds).
                mask = SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2;
                break;
            case kAudio:
            case kMain:
            case kLoader:
                mask = r33 ? SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2 : 0;
                break;
            default:
                break;
            }
            const int maskResult = mask ? sceKernelChangeThreadCpuAffinityMask(self, mask) : 0;
            const int prioResult = prio >= 64 && prio <= 191 ? sceKernelChangeThreadPriority(self, prio) : 0;
            SceKernelThreadInfo info{};
            info.size = sizeof(info);
            if (sceKernelGetThreadInfo(self, &info) >= 0)
                std::fprintf(stderr, "[threads] %s (%s, layout %d): cores 0x%x%s, priority %d%s\n", kNames[kind], info.name, layout() ? 1 : 0,
                             static_cast<unsigned>(info.currentCpuAffinityMask), maskResult < 0 ? " (refused)" : "", info.currentPriority,
                             prioResult < 0 ? " (refused)" : "");
        }
        void sample()
        {
            const int n = g_slotCount.load(std::memory_order_acquire);
            for (int k = 0; k < kKinds; ++k)
                g_lastCpuUs[k] = g_lastPreempts[k] = g_lastMoves[k] = 0;
            for (int i = 0; i < n; ++i)
            {
                Slot &t = g_slots[i];
                SceKernelThreadInfo info{};
                info.size = sizeof(info);
                if (sceKernelGetThreadInfo(t.id, &info) < 0)
                    continue;
                if (t.seen)
                {
                    g_lastCpuUs[t.kind] += info.runClocks - t.clocks;
                    g_lastPreempts[t.kind] += info.threadPreemptCount - t.preempts;
                    g_lastMoves[t.kind] += static_cast<uint32_t>(info.changeCpuCount) - t.moves;
                }
                t.seen = true;
                t.clocks = info.runClocks;
                t.preempts = info.threadPreemptCount;
                t.moves = static_cast<uint32_t>(info.changeCpuCount);
            }
            for (int k = 0; k < kKinds; ++k)
                g_cpuUs[k] += g_lastCpuUs[k], g_preempts[k] += g_lastPreempts[k], g_moves[k] += g_lastMoves[k];
        }
        int line(char *out, size_t room, double frames, bool reset, bool lastFrame)
        {
            const uint64_t *cpu = lastFrame ? g_lastCpuUs : g_cpuUs, *pre = lastFrame ? g_lastPreempts : g_preempts, *mov = lastFrame ? g_lastMoves : g_moves;
            if (lastFrame)
                frames = 1.0;
            int at = 0;
            if (room)
                out[0] = 0;
            bool any = false;
            {
                // Nothing counted (Vita3K has no run clocks): no column.
                uint64_t total = 0;
                for (int k = 0; k < kKinds; ++k)
                    total += cpu[k] + pre[k] + mov[k];
                if (!total)
                {
                    if (reset)
                        for (int k = 0; k < kKinds; ++k)
                            g_cpuUs[k] = g_preempts[k] = g_moves[k] = 0;
                    return 0;
                }
            }
            for (int k = 0; k < kKinds && static_cast<size_t>(at) < room; ++k)
            {
                bool have = false;
                for (int i = 0; i < g_slotCount.load(std::memory_order_acquire); ++i)
                    have |= g_slots[i].kind == k;
                if (!have)
                    continue;
                at += std::snprintf(out + at, room - at, "%s %s %.1f (%.0fp %.0fm)", any ? "," : " | cpu", kNames[k], cpu[k] / frames / 1000.0,
                                    pre[k] / frames, mov[k] / frames);
                any = true;
            }
            if (reset)
                for (int k = 0; k < kKinds; ++k)
                    g_cpuUs[k] = g_preempts[k] = g_moves[k] = 0;
            return at;
        }
#else
        void place(Kind) {}
        void sample() {}
        int line(char *out, size_t room, double, bool, bool)
        {
            if (room)
                out[0] = 0;
            return 0;
        }
#endif
    }

    namespace
    {
        // Round 34 (the Vita, logCaptureStderr): stderr's own file, which the
        // writer alone writes once every stderr line goes through the ring.
        int g_logSinkFd = -1;
        void logSinkWrite(const char *p, size_t n)
        {
#if defined(__vita__)
            if (g_logSinkFd >= 0)
            {
                while (n)
                {
                    const ssize_t w = ::write(g_logSinkFd, p, n);
                    if (w <= 0)
                        return;
                    p += w;
                    n -= static_cast<size_t>(w);
                }
                return;
            }
#endif
            std::fwrite(p, 1, n, stderr);
        }
        struct LogRing
        {
            static constexpr size_t kBytes = 512u * 1024u;
            std::mutex mutex, outMutex;
            std::vector<char> data = std::vector<char>(kBytes);
            size_t used = 0;     // bytes waiting
            uint64_t dropped = 0; // lines not taken (no room)
            bool started = false;
            std::vector<char> out = std::vector<char>(kBytes); // the writer's
            void write()
            {
                std::lock_guard<std::mutex> outLock(outMutex);
                size_t n;
                uint64_t lost;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    n = used;
                    lost = dropped;
                    if (n)
                        std::memcpy(out.data(), data.data(), n);
                    used = 0;
                    dropped = 0;
                }
                if (n)
                    logSinkWrite(out.data(), n);
                if (lost)
                {
                    char line[96];
                    const int m = std::snprintf(line, sizeof(line), "[log] %llu lines dropped (the ring was full)\n", static_cast<unsigned long long>(lost));
                    if (m > 0)
                        logSinkWrite(line, static_cast<size_t>(m));
                }
            }
            void loop()
            {
                ThreadNaming::SetCurrentThreadName("LogWriter");
                threads::place(threads::kLog);
                for (;;)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    write();
                }
            }
        };
        LogRing &logRing()
        {
            static LogRing *r = new LogRing; // never destroyed (the writer runs to the end)
            return *r;
        }
        bool logAsync()
        {
            static const bool on = []
            {
                const char *e = std::getenv("SOTC_LOG_ASYNC");
#if defined(__vita__)
                return !e || std::atoi(e) != 0;
#else
                return e && std::atoi(e) != 0;
#endif
            }();
            return on;
        }
    }
    void logf(const char *format, ...)
    {
        char line[2048];
        va_list args;
        va_start(args, format);
        if (!logAsync())
        {
            std::vfprintf(stderr, format, args);
            va_end(args);
            return;
        }
        int n = std::vsnprintf(line, sizeof(line), format, args);
        va_end(args);
        if (n <= 0)
            return;
        if (static_cast<size_t>(n) >= sizeof(line))
        {
            n = static_cast<int>(sizeof(line) - 1u);
            line[n - 1] = '\n'; // cut, still a line
        }
        logBytes(line, static_cast<size_t>(n));
    }
    void logBytes(const char *bytes, size_t n)
    {
        if (!n)
            return;
        LogRing &r = logRing();
        bool start = false;
        {
            std::lock_guard<std::mutex> lock(r.mutex);
            if (!r.started)
                r.started = start = true;
            if (r.used + n <= LogRing::kBytes)
            {
                std::memcpy(r.data.data() + r.used, bytes, n);
                r.used += n;
            }
            else
                ++r.dropped;
        }
        if (start)
        {
#if defined(__vita__)
            const unsigned int stackDefault = _pthread_stack_default_user;
            _pthread_stack_default_user = 64u * 1024u;
#endif
            std::thread([] { logRing().loop(); }).detach();
            std::atexit([] { logRing().write(); }); // what is left at a normal exit
#if defined(__vita__)
            _pthread_stack_default_user = stackDefault;
#endif
        }
    }
    void logFlush()
    {
        if (logAsync())
            logRing().write();
    }
#if defined(__vita__)
    namespace
    {
        _READ_WRITE_RETURN_TYPE stderrToRing(struct _reent *, void *, const char *buf, _READ_WRITE_BUFSIZE_TYPE n)
        {
            if (n > 0)
                logBytes(buf, static_cast<size_t>(n));
            return n;
        }
    }
#endif
    void logCaptureStderr()
    {
#if defined(__vita__)
        const char *e = std::getenv("SOTC_LOG_STDERR");
        if (!logAsync() || (e && std::atoi(e) == 0))
            return;
        std::fflush(stderr);
        g_logSinkFd = fileno(stderr);
        if (g_logSinkFd < 0)
            return;
        stderr->_write = stderrToRing;
        std::fprintf(stderr, "[log] every stderr line through the log ring from here on (SOTC_LOG_STDERR=0: each written at once)\n");
#endif
    }

    std::atomic<uint64_t> g_frontSkinned{0};
    void noteFrontendMemory(uint64_t skinnedPackets) { g_frontSkinned.store(skinnedPackets, std::memory_order_relaxed); }
    uint64_t frontendMemory() { return g_frontSkinned.load(std::memory_order_relaxed); }
    // buildAsync's worker: one thread, jobs in the order given. The waits
    // look again every 2 ms (Vita3K lost newlib condition wake-ups now and
    // then, see the GL backend's hand-over).
    struct AsyncBuilds
    {
        // A job: the AsyncWork, or (the std::function form) a FnWork around it.
        struct FnWork final : AsyncWork
        {
            std::vector<uint64_t> more; // ids past the first two
            std::function<void()> fnWork, fnFinish;
            void work() override
            {
                if (fnWork)
                    fnWork();
            }
            void finish() override
            {
                if (fnFinish)
                    fnFinish();
            }
            const std::vector<uint64_t> *moreIds() const override { return &more; }
        };
        struct Job
        {
            AsyncWork *w = nullptr;
        };
        std::mutex mutex;
        std::condition_variable wake;
        std::deque<Job> todo;
        std::vector<Job> done;
        bool started = false;
        // The game thread's: ids of jobs not taken up yet (asked at every draw
        // while builds are out: a flat set, no division per lookup).
        IdSet pending;
        bool anyPending() const { return pending.size() != 0u; }
        // The game thread's: jobs made this frame not handed over yet (round
        // 13: handed over 32 at a time and at the frame's end, one lock and
        // one wake-up for them, not one each: an area change queues ~1 200).
        std::vector<Job> staged;
        void flush()
        {
            if (staged.empty())
                return;
            {
                std::lock_guard<std::mutex> lock(mutex);
                for (Job &job : staged)
                    todo.push_back(std::move(job));
            }
            staged.clear();
            wake.notify_one();
        }
        std::vector<Job> finished;            // the game thread's (kept for its capacity)

        void loop()
        {
            ThreadNaming::SetCurrentThreadName("BuildWorker");
            // Off core 0 (the game thread's); the render thread has core 2.
            threads::place(threads::kBuild);
            for (;;)
            {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    while (todo.empty())
                        wake.wait_for(lock, std::chrono::milliseconds(2));
                    job = std::move(todo.front());
                    todo.pop_front();
                }
                if (job.w)
                    job.w->work();
                std::lock_guard<std::mutex> lock(mutex);
                done.push_back(std::move(job));
            }
        }
        void start()
        {
            if (started)
                return;
            started = true;
#if defined(__vita__)
            const unsigned int stackDefault = _pthread_stack_default_user;
            _pthread_stack_default_user = 256u * 1024u;
#endif
            std::thread([this] { loop(); }).detach();
#if defined(__vita__)
            _pthread_stack_default_user = stackDefault;
#endif
        }
        // At a frame's start (game thread): what the worker made goes to the
        // backend; with SOTC_TAKEUP_BUDGET=<n>, at most n jobs a frame, the
        // rest staying pending (their draws held) for the next frames, in
        // order. Off by default: round 12 tried it (160, with
        // SOTC_ASYNC_BUDGET=200) on an area change (~1 200 builds finished at
        // once, 1.7 MB of meshes in one frame): in calibrated Vita3K the
        // transition's frames stayed 160-220 ms (the held packets go through
        // the hook every frame) and the transition took ~900 ms more than
        // once, not less (docs/decomp.md, round 12).
        static uint32_t takeUpBudget()
        {
            static const uint32_t budget = std::getenv("SOTC_TAKEUP_BUDGET") ? static_cast<uint32_t>(std::atoi(std::getenv("SOTC_TAKEUP_BUDGET"))) : 0u;
            return budget;
        }
        // Jobs a frame may queue (asyncBudgetLeft): SOTC_ASYNC_BUDGET, 0 = no limit.
        static uint32_t queueBudget()
        {
            static const uint32_t budget = std::getenv("SOTC_ASYNC_BUDGET") ? static_cast<uint32_t>(std::atoi(std::getenv("SOTC_ASYNC_BUDGET"))) : 0u;
            return budget;
        }
        uint32_t queuedThisFrame = 0;
        void takeUp()
        {
            queuedThisFrame = 0;
            flush();
            if (!anyPending())
                return;
            {
                std::lock_guard<std::mutex> lock(mutex);
                const uint32_t budget = takeUpBudget();
                if (!budget || done.size() <= budget)
                    finished.swap(done);
                else
                {
                    finished.assign(std::make_move_iterator(done.begin()), std::make_move_iterator(done.begin() + budget));
                    done.erase(done.begin(), done.begin() + budget);
                }
            }
            for (Job &job : finished)
            {
                AsyncWork *w = job.w;
                for (uint32_t i = 0; i < w->idCount && i < 2u; ++i)
                    pending.erase(w->ids[i]);
                if (const std::vector<uint64_t> *more = w->moreIds())
                    for (uint64_t id : *more)
                        pending.erase(id);
                w->finish();
                delete w;
            }
            if (tuning().frameTiming())
                frameCounters().asyncDone += static_cast<uint32_t>(finished.size());
            finished.clear();
        }
    };
    AsyncBuilds *g_asyncBuilds = nullptr;
    AsyncBuilds &asyncBuildsFirst()
    {
        static AsyncBuilds *a = new AsyncBuilds(); // never destroyed: its thread runs to the end
        g_asyncBuilds = a;
        return *a;
    }
    // Asked for every draw: a plain pointer once made (a function-local static
    // costs a guard check with a memory barrier at each call on the Vita).
    inline AsyncBuilds &asyncBuilds() { return g_asyncBuilds ? *g_asyncBuilds : asyncBuildsFirst(); }
    bool buildAsync(AsyncWork *job)
    {
        if (!tuning().asyncBuilds || !backend())
        {
            delete job;
            return false;
        }
        AsyncBuilds &a = asyncBuilds();
        a.start();
        ++a.queuedThisFrame;
        for (uint32_t i = 0; i < job->idCount && i < 2u; ++i)
            a.pending.insert(job->ids[i]);
        if (const std::vector<uint64_t> *more = job->moreIds())
            for (uint64_t id : *more)
                a.pending.insert(id);
        a.staged.push_back({job});
        if (a.staged.size() >= 32u)
            a.flush();
        if (tuning().frameTiming())
            ++frameCounters().asyncJobs;
        return true;
    }
    bool buildAsync(std::vector<uint64_t> ids, std::function<void()> work, std::function<void()> finish)
    {
        if (!tuning().asyncBuilds || !backend())
            return false;
        auto *job = new AsyncBuilds::FnWork();
        for (uint64_t id : ids)
        {
            if (job->idCount < 2u)
                job->ids[job->idCount++] = id;
            else
                job->more.push_back(id);
        }
        job->fnWork = std::move(work);
        job->fnFinish = std::move(finish);
        return buildAsync(job);
    }
    bool asyncBudgetLeft()
    {
        const AsyncBuilds &a = asyncBuilds();
        const uint32_t budget = AsyncBuilds::queueBudget();
        return !budget || a.queuedThisFrame < budget;
    }
    bool asyncPending(uint64_t id)
    {
        const AsyncBuilds &a = asyncBuilds();
        return a.anyPending() && a.pending.count(id);
    }

    // While the scene recorder records, what it has not got yet is "not
    // there": the hooks build and upload it again, and it is recorded.
    inline bool recorded(uint64_t id) { return !native_recorder::active() || native_recorder::front::known(id); }
    bool hasMesh(uint64_t id) { return backend() && ((g_backend->hasMesh(id) && recorded(id)) || asyncPending(id)); }
    bool meshKnown(uint64_t id) { return backend() && g_backend->hasMesh(id) && recorded(id); }
    uint64_t meshEvictions() { return (backend() ? g_backend->meshEvictions() : 0u) + native_recorder::front::epoch(); }
    // SOTC_NATIVE_LOG_UPLOADS=1: uploads per frame (meshes: static / per-frame
    // ones, the ids with a top bit set; textures), every 60 frames.
    struct UploadStats
    {
        bool on = std::getenv("SOTC_NATIVE_LOG_UPLOADS") != nullptr;
        uint64_t frames = 0, staticMeshes = 0, staticBytes = 0, frameMeshes = 0, frameBytes = 0, textures = 0, textureBytes = 0, draws = 0;
    } g_uploads;
    std::vector<uint64_t> *g_drawTap = nullptr;
    Vertex *g_tapFrameMesh = nullptr;
    bool g_drawTapOnly = false;
    void setDrawTap(std::vector<uint64_t> *tap) { g_drawTap = tap; }
    std::vector<uint64_t> *drawTap() { return g_drawTap; }
    void setDrawTapOnly(bool only) { g_drawTapOnly = only; }
    void (*g_drawCheck)() = nullptr;
    void setDrawCheck(void (*check)()) { g_drawCheck = check; }
    // SOTC_DIFFTEST_DRAWS=2: the per-frame meshes' vertices and the skin
    // palettes too (some are made once a frame: only the first leg has them).
    bool tapMeshes()
    {
        static const bool on = std::getenv("SOTC_DIFFTEST_DRAWS") && std::atoi(std::getenv("SOTC_DIFFTEST_DRAWS")) >= 2;
        return on;
    }
    uint64_t tapHash(uint64_t h, const void *p, size_t n)
    {
        const uint8_t *b = static_cast<const uint8_t *>(p);
        for (size_t i = 0; i < n; ++i)
            h = (h ^ b[i]) * 0x100000001b3ull;
        return h;
    }
    void uploadMesh(uint64_t id, const Vertex *vertices, uint32_t count, bool perFrame)
    {
        if (g_drawTap && perFrame && tapMeshes())
            g_drawTap->push_back(tapHash(0xcbf29ce484222325ull, vertices, static_cast<size_t>(count) * sizeof(Vertex)));
        if (g_uploads.on)
        {
            (perFrame ? g_uploads.frameMeshes : g_uploads.staticMeshes) += 1;
            (perFrame ? g_uploads.frameBytes : g_uploads.staticBytes) += static_cast<uint64_t>(count) * sizeof(Vertex);
        }
        if (native_recorder::active())
            native_recorder::front::mesh(id, vertices, count, perFrame);
        if (!backend())
            return;
        if (!tuning().frameTiming())
        {
            g_backend->uploadMesh(id, vertices, count, perFrame);
            return;
        }
        const uint64_t start = nowUs();
        g_backend->uploadMesh(id, vertices, count, perFrame);
        FrameCounters &c = frameCounters();
        c.meshUs += nowUs() - start;
        ++c.meshUploads;
        c.meshBytes += static_cast<uint64_t>(count) * sizeof(Vertex);
    }
    Vertex *frameMesh(uint32_t count)
    {
        Vertex *v = backend() ? g_backend->frameMesh(count) : nullptr;
        g_tapFrameMesh = v;
        if (native_recorder::active())
            native_recorder::front::frameMeshRoom(v, nullptr, 0);
        return v;
    }
    uint32_t *frameMeshIndices(uint32_t count, uint32_t &base)
    {
        base = 0;
        uint32_t *indices = backend() ? g_backend->frameMeshIndices(count, base) : nullptr;
        if (native_recorder::active() && indices)
            native_recorder::front::frameMeshRoom(nullptr, indices, base);
        return indices;
    }
    void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount)
    {
        if (g_drawTap && g_tapFrameMesh && tapMeshes())
            g_drawTap->push_back(tapHash(0xcbf29ce484222325ull, g_tapFrameMesh, static_cast<size_t>(count) * sizeof(Vertex)));
        const uint64_t bytes = static_cast<uint64_t>(count) * sizeof(Vertex) + static_cast<uint64_t>(indexCount) * 4u;
        if (g_uploads.on)
        {
            g_uploads.frameMeshes += 1;
            g_uploads.frameBytes += bytes;
        }
        if (native_recorder::active())
            native_recorder::front::commitFrameMesh(id, count, indexCount);
        if (!backend())
            return;
        g_backend->commitFrameMesh(id, count, indexCount);
        if (tuning().frameTiming())
        {
            FrameCounters &c = frameCounters();
            ++c.meshUploads;
            c.meshBytes += bytes;
        }
    }
    bool gpuSkinning() { return tuning().gpuSkinning && backend() && g_backend->canSkin(); }
    void uploadSkinnedMesh(uint64_t id, const SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount)
    {
        if (native_recorder::active())
            native_recorder::front::skinnedMesh(id, vertices, count, indices, indexCount);
        if (!backend())
            return;
        const uint64_t start = tuning().frameTiming() ? nowUs() : 0u;
        g_backend->uploadSkinnedMesh(id, vertices, count, indices, indexCount);
        if (start)
        {
            FrameCounters &c = frameCounters();
            c.meshUs += nowUs() - start;
            ++c.meshUploads;
            c.meshBytes += static_cast<uint64_t>(count) * sizeof(SkinVertex) + static_cast<uint64_t>(indexCount) * 4u;
        }
    }
    uint32_t framePalette(const float *qwords, uint32_t count)
    {
        if (g_drawTap && tapMeshes())
            g_drawTap->push_back(tapHash(0xcbf29ce484222325ull, qwords, static_cast<size_t>(count) * 16u));
        const uint32_t handle = backend() ? g_backend->framePalette(qwords, count) : 0u;
        if (native_recorder::active())
            native_recorder::front::palette(handle, qwords, count, false);
        return handle;
    }
    void rewriteFramePalette(uint32_t handle, const float *qwords, uint32_t count)
    {
        if (native_recorder::active())
            native_recorder::front::palette(handle, qwords, count, true);
        if (backend() && handle)
            g_backend->rewriteFramePalette(handle, qwords, count);
    }
    void cancelFrameMesh()
    {
        if (backend())
            g_backend->cancelFrameMesh();
    }
    bool hasTexture(uint64_t id) { return backend() && ((g_backend->hasTexture(id) && recorded(id)) || asyncPending(id)); }
    void uploadTexture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels)
    {
        if (g_uploads.on)
        {
            ++g_uploads.textures;
            g_uploads.textureBytes += static_cast<uint64_t>(width) * height * 4u;
        }
        if (native_recorder::active())
            native_recorder::front::texture(id, width, height, rgba, levels);
        if (!backend())
            return;
        if (!tuning().frameTiming())
        {
            g_backend->uploadTexture(id, width, height, rgba, levels);
            return;
        }
        const uint64_t start = nowUs();
        g_backend->uploadTexture(id, width, height, rgba, levels);
        FrameCounters &c = frameCounters();
        c.texUs += nowUs() - start;
        ++c.texUploads;
        c.texBytes += static_cast<uint64_t>(width) * height * 4u;
    }
    // requestDrawDump: 1 waiting for a frame start, 2 recording that frame.
    int g_dumpState = 0;
    std::string g_dumpPath;
    FILE *g_dumpFile = nullptr;
    void requestDrawDump(const std::string &path)
    {
        g_dumpPath = path;
        g_dumpState = 1;
    }

    void beginFrame()
    {
        if (g_dumpState == 1)
        {
            g_dumpFile = std::fopen(g_dumpPath.c_str(), "w");
            g_dumpState = 2;
        }
        if (native_recorder::active())
            native_recorder::front::beginFrame();
        asyncBuilds().takeUp();
        if (backend())
            g_backend->beginFrame();
    }
    // The draw dump's hash of a matrix / the lights (their bits).
    uint64_t dumpHash(const float *v, int n)
    {
        uint64_t h = 0xcbf29ce484222325ull;
        for (int i = 0; i < n; ++i)
        {
            uint32_t b;
            std::memcpy(&b, &v[i], 4);
            h = (h ^ b) * 0x100000001b3ull;
        }
        return h;
    }
    void tapDraw(uint64_t mesh, const float clip[16], const DrawState &s)
    {
        // 2D draws are batched and flushed by the next draw of a frame, wherever
        // that falls (their content comes from guest packets, which a difftest
        // compares itself): the 3D draws only, unless =2.
        if (s.screen && !tapMeshes())
            return;
        uint64_t h = 0xcbf29ce484222325ull;
        // A per-frame mesh's id is a family tag (top byte) and a frame counter
        // (the skinned draws 1 << 63, the particles 1 << 58 ...): the tag only
        // (=2: its vertices were hashed at its commit). Packet ids are hashes.
        const uint64_t id = (mesh & 0x00FFFFFFFF000000ull) == 0u ? (mesh & 0xFF00000000000000ull) : mesh;
        h = tapHash(h, &id, 8);
        h = tapHash(h, &s.texture, 8);
        const uint32_t ints[] = {s.clamp, static_cast<uint32_t>(s.blend), s.order, s.background, s.fogged, s.writeDepth, s.noDepthWrite, s.overlay, s.screen,
                                 s.screenDepthTest, s.shadowVolume, s.reflectionSource, s.reflectionSample, s.refractionSample, s.fresnel, s.screenCopy,
                                 s.previousFrame, s.skinPaletteCount, s.skinPalette != 0u, s.lit};
        h = tapHash(h, ints, sizeof(ints));
        const float floats[] = {s.opacity, s.uvOffset[0], s.uvOffset[1], s.lodK, s.lodScale, s.maxLod, s.shadow, s.reflectionTint[0], s.reflectionTint[1],
                                s.reflectionTint[2], s.reflectionTint[3], s.mirrorRipple[0], s.mirrorRipple[1]};
        h = tapHash(h, floats, sizeof(floats));
        h = tapHash(h, clip, 64);
        if (s.lit || s.fresnel)
            h = tapHash(h, s.light, sizeof(s.light));
        g_drawTap->push_back(h);
        // SOTC_DIFFTEST_DRAWS_LOG=1: the first draws of each leg, readable (debugging the tap).
        // =<n>: the draws from the n-th on.
        static const long logFrom = std::getenv("SOTC_DIFFTEST_DRAWS_LOG") ? std::atol(std::getenv("SOTC_DIFFTEST_DRAWS_LOG")) : -1;
        if (logFrom >= 0 && g_drawTap->size() >= static_cast<size_t>(logFrom) && g_drawTap->size() < static_cast<size_t>(logFrom) + 4u)
        {
            uint64_t ch = tapHash(0xcbf29ce484222325ull, clip, 64), lh = tapHash(0xcbf29ce484222325ull, s.light, sizeof(s.light));
            std::fprintf(stderr, "[tap %p #%zu] mesh %016llx tex %016llx order %u blend %d op %08x clip %016llx light %016llx lit %d fres %d scr %d uv %g %g lod %g %g %g tint %g %g %g %g skin %u/%u flags %d%d%d%d%d%d%d%d%d%d%d%d\n",
                         (void *)g_drawTap, g_drawTap->size(), (unsigned long long)mesh, (unsigned long long)s.texture, s.order, (int)s.blend,
                         *reinterpret_cast<const uint32_t *>(&s.opacity), (unsigned long long)ch, (unsigned long long)lh, s.lit, s.fresnel, s.screen,
                         s.uvOffset[0], s.uvOffset[1], s.lodK, s.lodScale, s.maxLod, s.reflectionTint[0], s.reflectionTint[1], s.reflectionTint[2],
                         s.reflectionTint[3], s.skinPalette, s.skinPaletteCount, s.clamp, s.background, s.fogged, s.writeDepth, s.noDepthWrite, s.overlay,
                         s.screenDepthTest, s.shadowVolume, s.reflectionSource, s.reflectionSample, s.refractionSample, s.screenCopy);
        }
    }

    void dumpDraw(uint64_t mesh, const float clip[16], const DrawState &state)
    {
        {
            const DrawState &s = state;
            std::fprintf(g_dumpFile,
                         "order %2u mesh %016llx tex %016llx blend %d opacity %.3f screen %d bg %d fogged %d overlay %d writeZ %d lit %d clamp %u "
                         "shadowVol %d mirrorSrc %d mirrorSample %d refract %d screenCopy %d prev %d tint %.3f %.3f %.3f %.3f uvOff %.3f %.3f lod %.2f/%.2f/%.2f "
                         "noZ %d clip %016llx light %016llx skin %u/%u\n",
                         s.order, (unsigned long long)mesh, (unsigned long long)s.texture, (int)s.blend, s.opacity, s.screen, s.background, s.fogged,
                         s.overlay, s.writeDepth, s.lit, s.clamp, s.shadowVolume, s.reflectionSource, s.reflectionSample, s.refractionSample,
                         s.screenCopy, s.previousFrame, s.reflectionTint[0], s.reflectionTint[1], s.reflectionTint[2], s.reflectionTint[3],
                         s.uvOffset[0], s.uvOffset[1], s.lodK, s.lodScale, s.maxLod, s.noDepthWrite, (unsigned long long)dumpHash(clip, 16),
                         (unsigned long long)(s.lit || s.fresnel ? dumpHash(s.light, 32) : 0ull), s.skinPalette ? 1u : 0u, s.skinPaletteCount);
        }
    }
    // A draw of the world's rigid surfaces (the sky and the terrain's layers).
    inline bool terrainDraw(const DrawState &s) { return s.order >= 1u && s.order <= 3u && !s.screen && !s.skinPalette; }
    // Still on the worker, and not in the backend already: a build the scene
    // recorder asked for again (hasMesh says no for what it has not got) is
    // of the same content (ids hash it), so the backend's copy is drawn.
    inline bool heldMesh(uint64_t id) { return asyncPending(id) && !(native_recorder::active() && backend() && g_backend->hasMesh(id)); }
    inline bool heldTexture(uint64_t id) { return id && asyncPending(id) && !(native_recorder::active() && backend() && g_backend->hasTexture(id)); }
    void draw(uint64_t mesh, const float clip[16], const DrawState &state)
    {
        if (g_drawCheck)
            g_drawCheck();
        if (g_drawTap)
        {
            tapDraw(mesh, clip, state);
            if (g_drawTapOnly)
                return;
        }
        ++g_uploads.draws;
        if (g_dumpFile)
            dumpDraw(mesh, clip, state);
        // Its mesh or texture still on the worker (buildAsync): left out.
        if (asyncBuilds().anyPending() && (heldMesh(mesh) || heldTexture(state.texture)))
        {
            ++frameCounters().heldDraws;
            if (terrainDraw(state))
                ++frameCounters().heldTerrain;
            if (native_recorder::active())
                native_recorder::front::draw(&mesh, 1, clip, state, 1);
            return;
        }
        // The menus' Debug group: what is drawn at all.
        const Tuning &t = tuning();
        if (((t.hiddenLayers >> (state.order & 31u)) & 1u) || (state.screen ? !t.draw2D : !t.drawWorld) || (state.overlay && !t.overlays) ||
            (state.blend == Blend::Additive && !t.additive) || (state.blend == Blend::Subtract && !t.subtract))
        {
            if (native_recorder::active())
                native_recorder::front::draw(&mesh, 1, clip, state, 2);
            return;
        }
        if (native_recorder::active())
            native_recorder::front::draw(&mesh, 1, clip, state, 0);
        if (!t.textures || !t.lighting)
        {
            DrawState s = state;
            if (!t.textures)
                s.texture = 0;
            if (!t.lighting)
                s.lit = false;
            if (backend())
                g_backend->draw(mesh, clip, s);
            ++frameCounters().draws;
            return;
        }
        if (backend())
            g_backend->draw(mesh, clip, state);
        ++frameCounters().draws;
    }
    void drawMulti(const uint64_t *meshes, uint32_t count, const float clip[16], const DrawState &state)
    {
        if (count == 1u)
            return draw(meshes[0], clip, state);
        if (g_drawCheck)
            g_drawCheck();
        if (g_drawTap)
        {
            for (uint32_t i = 0; i < count; ++i)
                tapDraw(meshes[i], clip, state);
            if (g_drawTapOnly)
                return;
        }
        g_uploads.draws += count;
        if (g_dumpFile)
            for (uint32_t i = 0; i < count; ++i)
                dumpDraw(meshes[i], clip, state);
        // Held meshes (still on the worker) left out one by one, as draw()
        // leaves them out; the texture is the same for all.
        const bool texturePending = asyncBuilds().anyPending() && heldTexture(state.texture);
        if (texturePending)
        {
            frameCounters().heldDraws += count;
            if (terrainDraw(state))
                frameCounters().heldTerrain += count;
            if (native_recorder::active())
                native_recorder::front::draw(meshes, count, clip, state, 1);
            return;
        }
        static std::vector<uint64_t> kept;
        const uint64_t *list = meshes;
        uint32_t n = count;
        if (asyncBuilds().anyPending())
        {
            kept.clear();
            const bool terrain = terrainDraw(state);
            for (uint32_t i = 0; i < count; ++i)
                if (heldMesh(meshes[i]))
                {
                    ++frameCounters().heldDraws;
                    frameCounters().heldTerrain += terrain ? 1u : 0u;
                }
                else
                    kept.push_back(meshes[i]);
            list = kept.data();
            n = static_cast<uint32_t>(kept.size());
            if (native_recorder::active() && n < count)
            {
                // the held ones (fate 1), apart
                static std::vector<uint64_t> held;
                held.clear();
                for (uint32_t i = 0; i < count; ++i)
                    if (heldMesh(meshes[i]))
                        held.push_back(meshes[i]);
                native_recorder::front::draw(held.data(), static_cast<uint32_t>(held.size()), clip, state, 1);
            }
            if (n == 0u)
                return;
        }
        const Tuning &t = tuning();
        if (((t.hiddenLayers >> (state.order & 31u)) & 1u) || (state.screen ? !t.draw2D : !t.drawWorld) || (state.overlay && !t.overlays) ||
            (state.blend == Blend::Additive && !t.additive) || (state.blend == Blend::Subtract && !t.subtract))
        {
            if (native_recorder::active())
                native_recorder::front::draw(list, n, clip, state, 2);
            return;
        }
        if (native_recorder::active())
            native_recorder::front::draw(list, n, clip, state, 0);
        if (!t.textures || !t.lighting)
        {
            DrawState s = state;
            if (!t.textures)
                s.texture = 0;
            if (!t.lighting)
                s.lit = false;
            if (backend())
                g_backend->drawMulti(list, n, clip, s);
            frameCounters().draws += n;
            return;
        }
        if (backend())
            g_backend->drawMulti(list, n, clip, state);
        frameCounters().draws += n;
    }
    void setFog(const Fog &fog)
    {
        if (native_recorder::active())
            native_recorder::front::fog(fog);
        if (backend())
            g_backend->setFog(fog);
    }
    void setBloom(const Bloom &bloom)
    {
        if (native_recorder::active())
            native_recorder::front::bloom(bloom);
        if (backend())
            g_backend->setBloom(bloom);
    }
    void addScreenBlur(const ScreenBlur &blur)
    {
        if (g_drawCheck)
            g_drawCheck();
        if (native_recorder::active())
            native_recorder::front::blur(blur);
        if (backend())
            g_backend->addScreenBlur(blur);
    }
    void setGlowComposite(const GlowComposite &glow)
    {
        if (native_recorder::active())
            native_recorder::front::glow(glow);
        if (backend())
            g_backend->setGlowComposite(glow);
    }
    void setDepthOfField(const DepthOfField &dof)
    {
        if (native_recorder::active())
            native_recorder::front::dof(dof);
        if (backend())
            g_backend->setDepthOfField(dof);
    }
    void endFrame(uint32_t width, uint32_t height)
    {
        if (g_asyncBuilds)
            g_asyncBuilds->flush(); // this frame's builds start now
        // SOTC_QUALITY_CYCLE=N (comparing the quality settings on one scene,
        // e.g. the game paused): every N frames the next of these; the log
        // says which ([quality] lines).
        static const int cycle = std::getenv("SOTC_QUALITY_CYCLE") ? std::atoi(std::getenv("SOTC_QUALITY_CYCLE")) : 0;
        if (cycle > 0)
        {
            static uint64_t frames = 0;
            static int next = 0;
            if (frames++ % static_cast<uint64_t>(cycle) == 0u)
            {
                static const char *names[] = {"high", "medium", "low", "ultra", "high+msaa8", "high+aniso16", "high+finest", "high+nearest", "high+effects2"};
                Tuning &t = tuning();
                const int i = next++ % 9;
                applyPreset(t, i == 1 ? kPresetMedium : (i == 2 ? kPresetLow : (i == 3 ? kPresetUltra : kPresetHigh)));
                if (i == 4)
                    t.msaa = 3.0f;
                else if (i == 5)
                    t.textureFilter = 6.0f;
                else if (i == 6)
                    t.modelDetail = 2.0f;
                else if (i == 7)
                    t.textureFilter = 0.0f;
                else if (i == 8)
                    t.effectDetail = 2.0f;
                t.preset = static_cast<float>(matchingPreset(t));
                std::fprintf(stderr, "[quality] frame %llu: %s\n", static_cast<unsigned long long>(frames - 1u), names[i]);
            }
        }
        // SOTC_SCALE_CYCLE=N (testing a live resolution change, round 25):
        // every N frames the next render scale of 0.5, 1, 1.5, 2 (up to
        // kMaxRenderScale), as the menu's Resolution item sets it.
        static const int scaleCycle = std::getenv("SOTC_SCALE_CYCLE") ? std::atoi(std::getenv("SOTC_SCALE_CYCLE")) : 0;
        if (scaleCycle > 0)
        {
            static uint64_t frames = 0;
            static int next = 0;
            if (++frames % static_cast<uint64_t>(scaleCycle) == 0u)
            {
                static const float scales[] = {0.5f, 1.0f, 1.5f, 2.0f, 1.5f, 1.0f};
                Tuning &t = tuning();
                t.renderScale = std::clamp(scales[next++ % 6], kMinRenderScale, kMaxRenderScale);
                t.preset = static_cast<float>(matchingPreset(t));
                std::fprintf(stderr, "[scale-cycle] frame %llu: render scale %g\n", static_cast<unsigned long long>(frames), t.renderScale);
            }
        }
        if (g_dumpFile)
        {
            std::fclose(g_dumpFile);
            g_dumpFile = nullptr;
            g_dumpState = 0;
        }
        // A cut whose meshes / textures are still on the worker: the last
        // picture stays up rather than one with holes (holdIncomplete).
        {
            static uint32_t heldRun = 0;
            static uint64_t heldSince = 0;
            FrameCounters &c = frameCounters();
            // Up to SOTC_HOLD_FRAMES frames in a row (3) when an eighth of the
            // draws are missing. Round 27: also when any of the terrain's
            // (layers 1-3) is, for up to SOTC_HOLD_TERRAIN_MS (1500 ms; 0: not) in a
            // row: at 8-20 fps a fight's disc burst (650-870 builds) kept 1-2
            // frames with a few terrain packets missing, under the eighth,
            // and they were shown with holes. Past that time the frame is
            // shown as it is (a worker that never catches up must not freeze
            // the picture).
            static const uint64_t terrainMs = std::getenv("SOTC_HOLD_TERRAIN_MS") ? static_cast<uint64_t>(std::max(0, std::atoi(std::getenv("SOTC_HOLD_TERRAIN_MS")))) : 1500u;
            const uint32_t holdFrames = static_cast<uint32_t>(tuning().holdFrames + 0.5f);
            const uint64_t now = nowUs();
            const bool on = tuning().asyncBuilds && tuning().holdIncomplete;
            const bool many = c.heldDraws * 8u > c.draws + c.heldDraws && heldRun < holdFrames;
            const bool terrain = terrainMs && c.heldTerrain != 0u && (heldRun == 0u || now - heldSince < terrainMs * 1000u);
            const bool hold = on && (many || terrain);
            if (hold && heldRun == 0u)
                heldSince = now;
            heldRun = hold ? heldRun + 1u : 0u;
            frameCounters().heldPicture = hold ? 1u : 0u;
            if (hold && backend())
                g_backend->holdFrame();
            if (native_recorder::active())
                native_recorder::front::endFrame(width, height, hold);
        }
        if (backend())
        {
            if (tuning().frameTiming() && !g_backend->timesItself())
            {
                // The backend adds its present (swap) time itself: the rest is render.
                FrameCounters &c = frameCounters();
                const uint64_t start = nowUs(), present = c.presentUs;
                g_backend->endFrame(width, height);
                c.renderUs += (nowUs() - start) - (c.presentUs - present);
            }
            else
                g_backend->endFrame(width, height);
        }
        if (g_uploads.on && ++g_uploads.frames % 60u == 0u)
        {
            UploadStats &u = g_uploads;
            std::fprintf(stderr, "[uploads] per frame: static meshes %.1f (%.0f KB), per-frame meshes %.1f (%.0f KB), textures %.2f (%.0f KB), draws %.0f\n",
                         u.staticMeshes / 60.0, u.staticBytes / 60.0 / 1024.0, u.frameMeshes / 60.0, u.frameBytes / 60.0 / 1024.0,
                         u.textures / 60.0, u.textureBytes / 60.0 / 1024.0, u.draws / 60.0);
            u.staticMeshes = u.staticBytes = u.frameMeshes = u.frameBytes = u.textures = u.textureBytes = u.draws = 0;
        }
    }
    void finishFrames()
    {
        if (g_backend)
            g_backend->finishFrames();
    }
    void setShown(bool shown) { g_shown.store(shown, std::memory_order_relaxed); }
    bool shown() { return g_shown.load(std::memory_order_relaxed); }
    bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height)
    {
        // Not backend(): the presenting thread must not create it.
        return g_backend && g_backend->latestFrame(rgba, width, height);
    }
}
