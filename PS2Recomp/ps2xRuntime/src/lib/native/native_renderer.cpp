// The native renderer's front: picks a backend (native_backend.h) the first
// time it is used. macOS: Metal, or OpenGL with SOTC_NATIVE_GL=1 (to check
// the GL path against Metal); the Vita: OpenGL (vitaGL); elsewhere none.
#include "native_backend.h"
#include "runtime/native_tuning.h"
#include "ThreadNaming.h"

#include <algorithm>
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
#include <thread>
#include <unordered_set>
#if defined(__vita__)
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
        d.holdIncomplete = true;
        d.warmShaders = true;
#endif
        return d;
    }

    // The quality items' variables (over a preset).
    static void qualityFromEnv(Tuning &d)
    {
        auto number = [](const char *name, float fallback) { return std::getenv(name) ? static_cast<float>(std::atof(std::getenv(name))) : fallback; };
        d.renderScale = number("SOTC_RENDER_SCALE", d.renderScale);
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
        if (t.frameLog)
            add("SOTC_FRAME_LOG=1");
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

    const std::vector<TuningItem> &tuningItems()
    {
        static const std::vector<TuningItem> items = []
        {
        std::vector<TuningItem> v = {
            {"quality_preset", "Graphics preset", "Predefinição gráfica",
             "Sets resolution, anti-aliasing, texture filtering, model detail and effect detail at once. Low: half resolution (faster). "
             "Medium: the PS2's 512 x 448 (the Vita's default). High: 3x (the Mac's default). Ultra: 4x, 4x MSAA, 16x anisotropic, finest models, "
             "high effect detail (Mac). Changing any of those makes it Custom.",
             "Ajusta resolução, anti-aliasing, filtro de texturas, detalhe dos modelos e dos efeitos de uma vez. Baixo: metade da resolução (mais rápido). "
             "Médio: os 512 x 448 do PS2 (o padrão do Vita). Alto: 3x (o padrão do Mac). Ultra: 4x, MSAA 4x, anisotrópico 16x, modelos mais detalhados, "
             "efeitos em alto detalhe (Mac). Mudar qualquer um deles vira Personalizado.",
             nullptr, &Tuning::preset, 0.0f, 4.0f, 1.0f, "SOTC_PRESET"},
            {"render_scale", "Resolution", "Resolução",
             "The picture is drawn at 512 x 448 times this: 0.5 is faster, 1 is the PS2's, 2 to 4 are sharper.",
             "A imagem é desenhada em 512 x 448 vezes este valor: 0,5 é mais rápido, 1 é a do PS2, 2 a 4 ficam mais nítidas.",
             nullptr, &Tuning::renderScale, 0.5f, 4.0f, 0.5f, "SOTC_RENDER_SCALE"},
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
        debugToggle("dbg_frame_log", "Frame timing log", "Log de tempo por quadro",
                    "Writes to the log every 60 frames where the time went (game, hooks, texture decoding, uploads, drawing, swap) and every slow frame.",
                    "Escreve no log a cada 60 quadros onde o tempo foi (jogo, ganchos, decodificação de texturas, envios, desenho, troca) e cada quadro lento.",
                    &Tuning::frameLog, "SOTC_FRAME_LOG");
        debugToggle("dbg_subsys_prof", "Subsystem timing log", "Log de tempo por subsistema",
                    "Writes to the log every 120 frames the game thread's time by subsystem (display walk, characters, AI, collision, camera, particles, the hooks' parts, each task's step), and once at start a probe of the memory and call costs. The functions are wrapped at start only when this is on then (or SOTC_SUBSYS_PROF=1); off at run time: paused.",
                    "Escreve no log a cada 120 quadros o tempo da thread do jogo por subsistema (percurso de exibição, personagens, IA, colisão, câmera, partículas, as partes dos ganchos, o passo de cada tarefa) e uma vez no início uma sondagem dos custos de memória e de chamadas. As funções são embrulhadas no início só se isto estiver ligado então (ou SOTC_SUBSYS_PROF=1); desligado durante o jogo: pausado.",
                    &Tuning::subsysProf, "SOTC_SUBSYS_PROF");
        debugToggle("dbg_lean", "Lean hot code", "Código quente enxuto",
                    "With SOTC_LEAN=1 in env.txt: the ~1 850 hottest game functions run from copies with cheaper memory reads and writes (half the code). Off: the original copies, to compare speed (the picture should not change).",
                    "Com SOTC_LEAN=1 no env.txt: as ~1 850 funções mais quentes do jogo rodam de cópias com leituras e escritas de memória mais baratas (metade do código). Desligado: as cópias originais, pra comparar a velocidade (a imagem não deve mudar).",
                    &Tuning::leanCopies);
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
        debugToggle("dbg_hold_incomplete", "Hold incomplete frames", "Segurar quadros incompletos",
                    "With builds on a worker: a frame missing more than an eighth of its draws (still being built, at a camera cut) is not shown; the last one stays up (at most 3 frames).",
                    "Com montagem em outra thread: um quadro sem mais de um oitavo dos desenhos (ainda montando, num corte de câmera) não é mostrado; o anterior fica na tela (no máximo 3 quadros).",
                    &Tuning::holdIncomplete, "SOTC_HOLD_INCOMPLETE");
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
            if (key == "bloom_rounds" || key == "bloom_spread" || key == "dbg_haze_source" || key == "dbg_water_source" || key == "dbg_redraw" ||
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
        t.renderScale = p.renderScale;
        t.msaa = p.msaa;
        t.textureFilter = p.textureFilter;
        t.modelDetail = p.modelDetail;
        t.effectDetail = p.effectDetail;
        t.preset = static_cast<float>(preset);
    }

    int matchingPreset(const Tuning &t)
    {
        for (int i = 0; i < kPresetCustom; ++i)
        {
            const PresetValues &p = kPresets[i];
            if (t.renderScale == p.renderScale && t.msaa == p.msaa && t.textureFilter == p.textureFilter && t.modelDetail == p.modelDetail &&
                t.effectDetail == p.effectDetail)
                return i;
        }
        return kPresetCustom;
    }

    int tuningChoices(const TuningItem &item) { return choicesOf(item).count; }

    std::string tuningValueText(const TuningItem &item, bool portuguese)
    {
        const Tuning &t = tuning();
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

    bool tuningRestore(const char *key, float value)
    {
        for (const TuningItem &item : tuningItems())
        {
            if (std::strcmp(item.key, key) != 0)
                continue;
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
                g_backend = native_renderer::detail::createGlBackend(g_glMakeCurrent);
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
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    }
    std::atomic<uint64_t> g_frontSkinned{0};
    void noteFrontendMemory(uint64_t skinnedPackets) { g_frontSkinned.store(skinnedPackets, std::memory_order_relaxed); }
    uint64_t frontendMemory() { return g_frontSkinned.load(std::memory_order_relaxed); }
    // buildAsync's worker: one thread, jobs in the order given. The waits
    // look again every 2 ms (Vita3K lost newlib condition wake-ups now and
    // then, see the GL backend's hand-over).
    struct AsyncBuilds
    {
        struct Job
        {
            std::vector<uint64_t> ids;
            std::function<void()> work, finish;
        };
        std::mutex mutex;
        std::condition_variable wake;
        std::deque<Job> todo;
        std::vector<Job> done;
        bool started = false;
        std::unordered_set<uint64_t> pending; // the game thread's: ids of jobs not taken up yet
        std::vector<Job> finished;            // the game thread's (kept for its capacity)

        void loop()
        {
            ThreadNaming::SetCurrentThreadName("BuildWorker");
#if defined(__vita__)
            // Off core 0; the game thread keeps to cores 0-1, the render thread to 2.
            sceKernelChangeThreadCpuAffinityMask(sceKernelGetThreadId(), SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2);
#endif
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
                if (job.work)
                    job.work();
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
        // At a frame's start (game thread): what the worker made goes to the backend.
        void takeUp()
        {
            if (pending.empty())
                return;
            {
                std::lock_guard<std::mutex> lock(mutex);
                finished.swap(done);
            }
            for (Job &job : finished)
            {
                for (uint64_t id : job.ids)
                    pending.erase(id);
                if (job.finish)
                    job.finish();
            }
            if (tuning().frameLog)
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
    bool buildAsync(std::vector<uint64_t> ids, std::function<void()> work, std::function<void()> finish)
    {
        if (!tuning().asyncBuilds || !backend())
            return false;
        AsyncBuilds &a = asyncBuilds();
        a.start();
        for (uint64_t id : ids)
            a.pending.insert(id);
        {
            std::lock_guard<std::mutex> lock(a.mutex);
            a.todo.push_back({std::move(ids), std::move(work), std::move(finish)});
        }
        a.wake.notify_one();
        if (tuning().frameLog)
            ++frameCounters().asyncJobs;
        return true;
    }
    bool asyncPending(uint64_t id)
    {
        const AsyncBuilds &a = asyncBuilds();
        return !a.pending.empty() && a.pending.count(id) != 0;
    }

    bool hasMesh(uint64_t id) { return backend() && (g_backend->hasMesh(id) || asyncPending(id)); }
    // SOTC_NATIVE_LOG_UPLOADS=1: uploads per frame (meshes: static / per-frame
    // ones, the ids with a top bit set; textures), every 60 frames.
    struct UploadStats
    {
        bool on = std::getenv("SOTC_NATIVE_LOG_UPLOADS") != nullptr;
        uint64_t frames = 0, staticMeshes = 0, staticBytes = 0, frameMeshes = 0, frameBytes = 0, textures = 0, textureBytes = 0, draws = 0;
    } g_uploads;
    void uploadMesh(uint64_t id, const Vertex *vertices, uint32_t count, bool perFrame)
    {
        if (g_uploads.on)
        {
            (perFrame ? g_uploads.frameMeshes : g_uploads.staticMeshes) += 1;
            (perFrame ? g_uploads.frameBytes : g_uploads.staticBytes) += static_cast<uint64_t>(count) * sizeof(Vertex);
        }
        if (!backend())
            return;
        if (!tuning().frameLog)
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
    Vertex *frameMesh(uint32_t count) { return backend() ? g_backend->frameMesh(count) : nullptr; }
    uint32_t *frameMeshIndices(uint32_t count, uint32_t &base)
    {
        base = 0;
        return backend() ? g_backend->frameMeshIndices(count, base) : nullptr;
    }
    void commitFrameMesh(uint64_t id, uint32_t count, uint32_t indexCount)
    {
        const uint64_t bytes = static_cast<uint64_t>(count) * sizeof(Vertex) + static_cast<uint64_t>(indexCount) * 4u;
        if (g_uploads.on)
        {
            g_uploads.frameMeshes += 1;
            g_uploads.frameBytes += bytes;
        }
        if (!backend())
            return;
        g_backend->commitFrameMesh(id, count, indexCount);
        if (tuning().frameLog)
        {
            FrameCounters &c = frameCounters();
            ++c.meshUploads;
            c.meshBytes += bytes;
        }
    }
    bool gpuSkinning() { return tuning().gpuSkinning && backend() && g_backend->canSkin(); }
    void uploadSkinnedMesh(uint64_t id, const SkinVertex *vertices, uint32_t count, const uint32_t *indices, uint32_t indexCount)
    {
        if (!backend())
            return;
        const uint64_t start = tuning().frameLog ? nowUs() : 0u;
        g_backend->uploadSkinnedMesh(id, vertices, count, indices, indexCount);
        if (start)
        {
            FrameCounters &c = frameCounters();
            c.meshUs += nowUs() - start;
            ++c.meshUploads;
            c.meshBytes += static_cast<uint64_t>(count) * sizeof(SkinVertex) + static_cast<uint64_t>(indexCount) * 4u;
        }
    }
    uint32_t framePalette(const float *qwords, uint32_t count) { return backend() ? g_backend->framePalette(qwords, count) : 0u; }
    void cancelFrameMesh()
    {
        if (backend())
            g_backend->cancelFrameMesh();
    }
    bool hasTexture(uint64_t id) { return backend() && (g_backend->hasTexture(id) || asyncPending(id)); }
    void uploadTexture(uint64_t id, uint32_t width, uint32_t height, const uint8_t *rgba, uint32_t levels)
    {
        if (g_uploads.on)
        {
            ++g_uploads.textures;
            g_uploads.textureBytes += static_cast<uint64_t>(width) * height * 4u;
        }
        if (!backend())
            return;
        if (!tuning().frameLog)
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
        asyncBuilds().takeUp();
        if (backend())
            g_backend->beginFrame();
    }
    void draw(uint64_t mesh, const float clip[16], const DrawState &state)
    {
        ++g_uploads.draws;
        if (g_dumpFile)
        {
            const DrawState &s = state;
            std::fprintf(g_dumpFile,
                         "order %2u mesh %016llx tex %016llx blend %d opacity %.3f screen %d bg %d fogged %d overlay %d writeZ %d lit %d clamp %u "
                         "shadowVol %d mirrorSrc %d mirrorSample %d refract %d screenCopy %d prev %d tint %.3f %.3f %.3f %.3f uvOff %.3f %.3f lod %.2f/%.2f/%.2f\n",
                         s.order, (unsigned long long)mesh, (unsigned long long)s.texture, (int)s.blend, s.opacity, s.screen, s.background, s.fogged,
                         s.overlay, s.writeDepth, s.lit, s.clamp, s.shadowVolume, s.reflectionSource, s.reflectionSample, s.refractionSample,
                         s.screenCopy, s.previousFrame, s.reflectionTint[0], s.reflectionTint[1], s.reflectionTint[2], s.reflectionTint[3],
                         s.uvOffset[0], s.uvOffset[1], s.lodK, s.lodScale, s.maxLod);
        }
        // Its mesh or texture still on the worker (buildAsync): left out.
        if (!asyncBuilds().pending.empty() && (asyncPending(mesh) || (state.texture && asyncPending(state.texture))))
        {
            ++frameCounters().heldDraws;
            return;
        }
        // The menus' Debug group: what is drawn at all.
        const Tuning &t = tuning();
        if (((t.hiddenLayers >> (state.order & 31u)) & 1u) || (state.screen ? !t.draw2D : !t.drawWorld) || (state.overlay && !t.overlays) ||
            (state.blend == Blend::Additive && !t.additive) || (state.blend == Blend::Subtract && !t.subtract))
            return;
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
    void setFog(const Fog &fog)
    {
        if (backend())
            g_backend->setFog(fog);
    }
    void setBloom(const Bloom &bloom)
    {
        if (backend())
            g_backend->setBloom(bloom);
    }
    void addScreenBlur(const ScreenBlur &blur)
    {
        if (backend())
            g_backend->addScreenBlur(blur);
    }
    void setGlowComposite(const GlowComposite &glow)
    {
        if (backend())
            g_backend->setGlowComposite(glow);
    }
    void setDepthOfField(const DepthOfField &dof)
    {
        if (backend())
            g_backend->setDepthOfField(dof);
    }
    void endFrame(uint32_t width, uint32_t height)
    {
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
            const FrameCounters &c = frameCounters();
            const bool hold = tuning().asyncBuilds && tuning().holdIncomplete && c.heldDraws * 8u > c.draws + c.heldDraws && heldRun < 3u;
            heldRun = hold ? heldRun + 1u : 0u;
            if (hold && backend())
                g_backend->holdFrame();
        }
        if (backend())
        {
            if (tuning().frameLog && !g_backend->timesItself())
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
    void setShown(bool shown) { g_shown.store(shown, std::memory_order_relaxed); }
    bool shown() { return g_shown.load(std::memory_order_relaxed); }
    bool latestFrame(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height)
    {
        // Not backend(): the presenting thread must not create it.
        return g_backend && g_backend->latestFrame(rgba, width, height);
    }
}
