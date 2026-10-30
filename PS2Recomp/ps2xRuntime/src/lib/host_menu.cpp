#include "runtime/host_menu.h"
#include "runtime/native_recorder.h"
#include "runtime/native_tuning.h"

#include "raylib.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#if defined(PLATFORM_VITA)
#include <psp2/apputil.h>
#include <psp2/system_param.h>
#endif

namespace hostmenu
{
namespace
{
    Settings g_settings;
    bool g_open = false;
    bool g_help = false;
    bool g_quit = false;
    bool g_quitArmed = false; // "Quit game" needs a second Cross
    bool g_holdBlock = false; // a button pressed in the menu is still held
    int g_tab = 0, g_item = 0;
    double g_lastSelect = -1.0;

#if defined(PLATFORM_VITA)
    const char *kSettingsPath = "ux0:data/celosia/settings.cfg";
#else
    const char *kSettingsPath = "celosia_settings.cfg";
#endif
    constexpr double kDoubleTapSeconds = 0.35;

    // An item set by its variable at start (env / env.txt) is not the
    // player's choice: save() keeps the file's own value for it (or leaves it
    // out) unless it was changed in the menu since (round 24: env.txt's
    // SOTC_SUBSYS_PROF=1 of one console run went into settings.cfg at the
    // first menu change and stayed on in every later run, which also kept
    // the model draws' worker off).
    struct KeptValue
    {
        std::string key;
        double value;
    };
    std::vector<KeptValue> g_fileValues;  // as read from the file at start
    std::vector<KeptValue> g_startValues; // every item's value after load()
    const KeptValue *findKept(const std::vector<KeptValue> &v, const char *key)
    {
        for (const KeptValue &k : v)
            if (k.key == key)
                return &k;
        return nullptr;
    }

    // ---- Text -------------------------------------------------------------

    bool portuguese()
    {
        const int choice = g_settings.language.load();
        if (choice != 0)
            return choice == 1;
        static const bool systemPt = []
        {
#if defined(PLATFORM_VITA)
            SceAppUtilInitParam init{};
            SceAppUtilBootParam boot{};
            sceAppUtilInit(&init, &boot);
            int lang = 0;
            sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, &lang);
            return lang == SCE_SYSTEM_PARAM_LANG_PORTUGUESE_PT || lang == SCE_SYSTEM_PARAM_LANG_PORTUGUESE_BR;
#else
            const char *lang = std::getenv("LANG");
            return lang && std::strncmp(lang, "pt", 2) == 0;
#endif
        }();
        return systemPt;
    }

    // English text -> Portuguese.
    const char *T(const char *en)
    {
        if (!portuguese())
            return en;
        static const struct
        {
            const char *en, *pt;
        } kTexts[] = {
            {"SETTINGS", "AJUSTES"},
            {"Video", "Vídeo"},
            {"Sound", "Som"},
            {"System", "Sistema"},
            {"About", "Sobre"},
            {"Renderer", "Gráficos"},
            {"Quality", "Qualidade"},
            {"Debug", "Depuração"},
            {"Exact transparency", "Transparências precisas"},
            {"Show FPS", "Mostrar FPS"},
            {"Screen", "Tela"},
            {"Volume", "Volume"},
            {"Language", "Idioma"},
            {"Quit game", "Sair do jogo"},
            {"On", "Ligado"},
            {"Off", "Desligado"},
            {"Fit (4:3)", "Ajustar (4:3)"},
            {"Stretch", "Esticar"},
            {"Sharp (whole scale)", "Nítido (escala inteira)"},
            {"Automatic", "Automático"},
            {"Press X again to quit", "Aperte X de novo para sair"},
            {"L/R: tab   D-pad: choose   X: change   Triangle: help   O: close",
             "L/R: aba   Direcional: escolher   X: mudar   Triângulo: ajuda   O: fechar"},
            {"Triangle / O: back", "Triângulo / O: voltar"},
            {"Draws the effects that use the alpha test exactly like the PS2 (clouds, mist, shadows). "
             "Turning it off gains a few frames per second; some mist and edges look harder.",
             "Desenha os efeitos que usam o teste de alfa exatamente como o PS2 (nuvens, névoa, sombras). "
             "Desligar ganha alguns quadros por segundo; parte da névoa e das bordas fica mais dura."},
            {"Shows the game's frame rate in the corner of the screen.",
             "Mostra a taxa de quadros do jogo no canto da tela."},
            {"Fit keeps the original 4:3 picture with bars on the sides. Stretch fills the screen. "
             "Sharp uses the largest whole multiple of the original size, so every pixel has the same size.",
             "Ajustar mantém a imagem original 4:3 com faixas nas laterais. Esticar preenche a tela. "
             "Nítido usa o maior múltiplo inteiro do tamanho original, então todo pixel tem o mesmo tamanho."},
            {"Volume of the game's sound and music.", "Volume do som e da música do jogo."},
            {"Language of this menu. Automatic follows the system language.",
             "Idioma deste menu. Automático segue o idioma do sistema."},
            {"Closes the game. Unsaved progress is lost.", "Fecha o jogo. O progresso não salvo é perdido."},
            {"Shadow of the Colossus, recompiled to run natively.", "Shadow of the Colossus, recompilado para rodar nativamente."},
            {"celosia - port by HenryKun55", "celosia - port por HenryKun55"},
            {"Shadow of the Colossus (c) Sony Interactive Entertainment.", "Shadow of the Colossus (c) Sony Interactive Entertainment."},
            {"Built on PS2Recomp. Thanks to the sotc-pc project.", "Feito sobre o PS2Recomp. Obrigado ao projeto sotc-pc."},
        };
        for (const auto &t : kTexts)
            if (std::strcmp(t.en, en) == 0)
                return t.pt;
        return en;
    }

    // ---- Items ------------------------------------------------------------

    enum ItemId
    {
        kExactAlpha,
        kShowFps,
        kScreen,
        kVolume,
        kLanguage,
        kQuit,
    };

    struct Item
    {
        ItemId id;
        const char *label;
        const char *help;
    };

    struct Tab
    {
        const char *label;
        const Item *items;
        int count;
    };

    const Item kVideoItems[] = {
        {kExactAlpha, "Exact transparency",
         "Draws the effects that use the alpha test exactly like the PS2 (clouds, mist, shadows). "
         "Turning it off gains a few frames per second; some mist and edges look harder."},
        {kShowFps, "Show FPS", "Shows the game's frame rate in the corner of the screen."},
        {kScreen, "Screen",
         "Fit keeps the original 4:3 picture with bars on the sides. Stretch fills the screen. "
         "Sharp uses the largest whole multiple of the original size, so every pixel has the same size."},
    };
    const Item kSoundItems[] = {
        {kVolume, "Volume", "Volume of the game's sound and music."},
    };
    const Item kSystemItems[] = {
        {kLanguage, "Language", "Language of this menu. Automatic follows the system language."},
        {kQuit, "Quit game", "Closes the game. Unsaved progress is lost."},
    };
    // The Quality, Renderer and Debug tabs list native_renderer::tuningItems()
    // (the same list as the Mac's F10 panel), each with a last "Defaults" row:
    // Quality the preset, resolution, filtering...; Debug what to draw at
    // all, the layers; Renderer the rest.
    constexpr int kQualityTab = 1, kRendererTab = 2, kDebugTab = 3;
    const Tab kTabs[] = {
        {"Video", kVideoItems, 3},
        {"Quality", nullptr, -1},
        {"Renderer", nullptr, -1},
        {"Debug", nullptr, -1},
        {"Sound", kSoundItems, 1},
        {"System", kSystemItems, 2},
        {"About", nullptr, 0},
    };
    constexpr int kTabCount = 7;
    int g_tabNow = 0; // the tab the renderer helpers below list

    // The list's items of a tuning tab.
    const std::vector<int> &tuningRows(int tab)
    {
        static std::vector<int> normal, dbg, quality;
        if (normal.empty() && dbg.empty())
        {
            const auto &items = native_renderer::tuningItems();
            for (int i = 0; i < static_cast<int>(items.size()); ++i)
                (items[i].debug ? dbg : (items[i].quality ? quality : normal)).push_back(i);
        }
        return tab == kDebugTab ? dbg : (tab == kQualityTab ? quality : normal);
    }

    bool isTuningTab(int tab) { return tab == kQualityTab || tab == kRendererTab || tab == kDebugTab; }

    int itemCount(int tab)
    {
        return isTuningTab(tab) ? static_cast<int>(tuningRows(tab).size()) + 1 : kTabs[tab].count;
    }

    // Row i of the current tuning tab -> the list's item (or -1: the reset row).
    int tuningIndex(int i)
    {
        const auto &rows = tuningRows(g_tabNow);
        return i < static_cast<int>(rows.size()) ? rows[i] : -1;
    }

    const char *rendererLabel(int i)
    {
        const int k = tuningIndex(i);
        if (k < 0)
            return portuguese() ? "Restaurar padrões" : "Reset to defaults";
        const auto &item = native_renderer::tuningItems()[k];
        return portuguese() ? item.labelPt : item.label;
    }

    const char *rendererHelp(int i)
    {
        const int k = tuningIndex(i);
        if (k < 0)
            return portuguese() ? "Volta todos os ajustes do renderer aos valores padrão." : "Puts every renderer adjustment back to its default.";
        const auto &item = native_renderer::tuningItems()[k];
        if (!native_renderer::tuningApplies(item))
        {
            // Said first: it changes nothing on this renderer.
            static std::string text;
            text = std::string(portuguese() ? "Sem efeito no renderer " : "No effect on the ") + native_renderer::backendName() +
                   (portuguese() ? " em uso. " : " renderer in use. ") + (portuguese() ? item.helpPt : item.help);
            return text.c_str();
        }
        return portuguese() ? item.helpPt : item.help;
    }

    std::string rendererValue(int i)
    {
        const int k = tuningIndex(i);
        if (k < 0)
            return "";
        const auto &item = native_renderer::tuningItems()[k];
        std::string v = native_renderer::tuningValueText(item, portuguese());
        if (v == "On" || v == "Off")
            v = T(v.c_str());
        if (!native_renderer::tuningApplies(item)) // listed, but this renderer ignores it
            v += portuguese() ? (item.backends == 1 ? " (só Metal)" : " (só OpenGL)") : (item.backends == 1 ? " (Metal only)" : " (OpenGL only)");
        return v;
    }

    std::string valueText(ItemId id)
    {
        switch (id)
        {
        case kExactAlpha: return T(g_settings.exactAlpha ? "On" : "Off");
        case kShowFps: return T(g_settings.showFps ? "On" : "Off");
        case kScreen:
        {
            static const char *modes[] = {"Fit (4:3)", "Stretch", "Sharp (whole scale)"};
            return T(modes[std::clamp(g_settings.screenMode.load(), 0, 2)]);
        }
        case kVolume:
        {
            const int v = g_settings.volume.load();
            return std::string(static_cast<size_t>(v), '|') + std::string(static_cast<size_t>(10 - v), '.') + "  " + std::to_string(v * 10) + "%";
        }
        case kLanguage:
        {
            static const char *langs[] = {"Automatic", "Português", "English"};
            return T(langs[std::clamp(g_settings.language.load(), 0, 2)]);
        }
        case kQuit: return g_quitArmed ? T("Press X again to quit") : "";
        }
        return "";
    }

    // ---- Persistence --------------------------------------------------------

    double itemValue(const native_renderer::TuningItem &item)
    {
        const native_renderer::Tuning &t = native_renderer::tuning();
        if (item.toggle)
            return t.*item.toggle ? 1.0 : 0.0;
        if (item.mask) // hidden layers: 1 = hidden
            return ((t.*item.mask >> item.bit) & 1u) ? 1.0 : 0.0;
        if (item.number)
            return static_cast<double>(t.*item.number);
        return 0.0;
    }

    void save()
    {
        FILE *f = std::fopen(kSettingsPath, "w");
        if (!f)
            return;
        std::fprintf(f, "settings_version=37\n");
        std::fprintf(f, "exact_alpha=%d\nshow_fps=%d\nscreen=%d\nvolume=%d\nlanguage=%d\n", g_settings.exactAlpha ? 1 : 0,
                     g_settings.showFps ? 1 : 0, g_settings.screenMode.load(), g_settings.volume.load(), g_settings.language.load());
        for (const native_renderer::TuningItem &item : native_renderer::tuningItems())
        {
            if (item.action)
                continue; // (the Debug group's "Mark": nothing to keep)
            double value = itemValue(item);
            if (item.env && std::getenv(item.env))
            {
                // Set by its variable at start: saved only once changed here.
                const KeptValue *start = findKept(g_startValues, item.key);
                if (start && start->value == value)
                {
                    const KeptValue *file = findKept(g_fileValues, item.key);
                    if (!file)
                        continue;
                    value = file->value;
                }
            }
            std::fprintf(f, "%s=%g\n", item.key, value);
        }
        std::fclose(f);
    }

    // Changes an item by +1/-1 (Cross and right = +1).
    void change(ItemId id, int dir)
    {
        switch (id)
        {
        case kExactAlpha: g_settings.exactAlpha = !g_settings.exactAlpha; break;
        case kShowFps: g_settings.showFps = !g_settings.showFps; break;
        case kScreen: g_settings.screenMode = (g_settings.screenMode + 3 + dir) % 3; break;
        case kVolume: g_settings.volume = std::clamp(g_settings.volume + dir, 0, 10); break;
        case kLanguage: g_settings.language = (g_settings.language + 3 + dir) % 3; break;
        case kQuit:
            if (dir > 0)
            {
                if (g_quitArmed)
                    g_quit = true;
                g_quitArmed = true;
            }
            return;
        }
        save();
    }

    // ---- Input --------------------------------------------------------------

    enum Button
    {
        kUp, kDown, kLeft, kRight, kCross, kCircle, kTriangle, kL1, kR1, kSelect, kButtonCount
    };

    bool g_down[kButtonCount] = {};
    bool g_prev[kButtonCount] = {};

    void readButtons()
    {
        std::memcpy(g_prev, g_down, sizeof(g_down));
        std::memset(g_down, 0, sizeof(g_down));
        const int pad = 0;
        if (IsGamepadAvailable(pad))
        {
            // The stick as a d-pad with hysteresis: on past 0.6, off below
            // 0.4 (a stick resting near the threshold gave a press a frame).
            const float lx = GetGamepadAxisMovement(pad, GAMEPAD_AXIS_LEFT_X), ly = GetGamepadAxisMovement(pad, GAMEPAD_AXIS_LEFT_Y);
            auto stick = [](bool was, float v) { return v > (was ? 0.4f : 0.6f); };
            g_down[kUp] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_LEFT_FACE_UP) || stick(g_prev[kUp], -ly);
            g_down[kDown] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || stick(g_prev[kDown], ly);
            g_down[kLeft] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || stick(g_prev[kLeft], -lx);
            g_down[kRight] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || stick(g_prev[kRight], lx);
            g_down[kCross] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
            g_down[kCircle] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
            g_down[kTriangle] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_RIGHT_FACE_UP);
            g_down[kL1] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
            g_down[kR1] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
            g_down[kSelect] = IsGamepadButtonDown(pad, GAMEPAD_BUTTON_MIDDLE_LEFT);
        }
        // Keyboard: arrows, Enter = Cross, Backspace = Circle, H = help, Q/E = tabs.
        g_down[kUp] |= IsKeyDown(KEY_UP);
        g_down[kDown] |= IsKeyDown(KEY_DOWN);
        g_down[kLeft] |= IsKeyDown(KEY_LEFT);
        g_down[kRight] |= IsKeyDown(KEY_RIGHT);
        g_down[kCross] |= IsKeyDown(KEY_ENTER);
        g_down[kCircle] |= IsKeyDown(KEY_BACKSPACE);
        g_down[kTriangle] |= IsKeyDown(KEY_H);
        g_down[kL1] |= IsKeyDown(KEY_Q);
        g_down[kR1] |= IsKeyDown(KEY_E);
    }

    bool pressed(Button b) { return g_down[b] && !g_prev[b]; }

    bool anyDown()
    {
        for (bool d : g_down)
            if (d)
                return true;
        return false;
    }

    void close()
    {
        g_open = false;
        g_help = false;
        g_quitArmed = false;
        g_holdBlock = true; // until the closing button is released
    }

    // ---- Drawing --------------------------------------------------------------

    // Draws `text` wrapped at maxWidth; returns the number of lines (none
    // drawn when `draw` is false).
    int drawWrapped(const char *text, int x, int y, int maxWidth, int size, Color color, int lineGap, bool draw = true)
    {
        std::string line, word;
        int lines = 0;
        auto flush = [&]
        {
            ++lines;
            if (draw)
                DrawText(line.c_str(), x, y, size, color);
            y += size + lineGap;
            line.clear();
        };
        for (const char *p = text;; ++p)
        {
            if (*p == ' ' || *p == '\0')
            {
                const std::string candidate = line.empty() ? word : line + " " + word;
                if (!line.empty() && MeasureText(candidate.c_str(), size) > maxWidth)
                {
                    flush();
                    line = word;
                }
                else
                    line = candidate;
                word.clear();
                if (*p == '\0')
                    break;
            }
            else
                word += *p;
        }
        if (!line.empty())
            flush();
        return lines;
    }
}

Settings &settings() { return g_settings; }
void saveSettings() { save(); }

void load()
{
    if (const char *open = std::getenv("PS2X_SETTINGS_OPEN")) // development: open at start on tab N
    {
        g_open = true;
        g_tab = std::clamp(std::atoi(open), 0, kTabCount - 1);
    }
    struct StartValues
    {
        ~StartValues()
        {
            g_startValues.clear();
            for (const native_renderer::TuningItem &item : native_renderer::tuningItems())
                if (!item.action)
                    g_startValues.push_back({item.key, itemValue(item)});
        }
    } startValues; // taken when load() returns, whichever way
    g_fileValues.clear();
    FILE *f = std::fopen(kSettingsPath, "r");
    if (!f)
        return;
    char key[64];
    float number = 0.0f;
    while (std::fscanf(f, " %63[^=]=%f", key, &number) == 2)
        g_fileValues.push_back({key, static_cast<double>(number)});
    std::fclose(f);
    // A file from before round 24 (no settings_version) may hold a
    // measuring switch that came from env.txt, not from the player: the
    // subsystem profiler is not taken from it (it slows the game and keeps
    // the model draws' worker off).
    const bool old = !findKept(g_fileValues, "settings_version");
    if (old)
        std::fprintf(stderr, "[settings] %s is from before round 24: dbg_subsys_prof in it ignored\n", kSettingsPath);
    // Before round 37 the vertex fog's amount was 0.55, set by eye against
    // an emulated picture that had no GS fog; 1 is the game's own (measured
    // against the GS picture in round 37). A file from before keeps its other
    // values but takes the new fog default.
    const KeptValue *version = findKept(g_fileValues, "settings_version");
    const bool beforeExactFog = !version || version->value < 37.0;
    if (beforeExactFog)
        std::fprintf(stderr, "[settings] %s is from before round 37: fog_amount in it ignored (the game's own fog now)\n", kSettingsPath);
    for (const KeptValue &kv : g_fileValues)
    {
        const char *key = kv.key.c_str();
        const float number = static_cast<float>(kv.value);
        const int value = static_cast<int>(number);
        if (old && !std::strcmp(key, "dbg_subsys_prof"))
            continue;
        if (beforeExactFog && !std::strcmp(key, "fog_amount"))
            continue;
        if (native_renderer::tuningRestore(key, number))
            continue;
        if (!std::strcmp(key, "exact_alpha")) g_settings.exactAlpha = value != 0;
        else if (!std::strcmp(key, "show_fps")) g_settings.showFps = value != 0;
        else if (!std::strcmp(key, "screen")) g_settings.screenMode = std::clamp(value, 0, 2);
        else if (!std::strcmp(key, "volume")) g_settings.volume = std::clamp(value, 0, 10);
        else if (!std::strcmp(key, "language")) g_settings.language = std::clamp(value, 0, 2);
    }
    if (old) // (written as the file of this round on its next save)
        g_fileValues.erase(std::remove_if(g_fileValues.begin(), g_fileValues.end(), [](const KeptValue &k) { return k.key == "dbg_subsys_prof"; }),
                           g_fileValues.end());
}

bool isOpen() { return g_open; }
bool quitRequested() { return g_quit; }
bool inputBlocked() { return g_open || g_holdBlock; }

void update()
{
    readButtons();
    if (g_holdBlock && !anyDown())
        g_holdBlock = false;

    if (!g_open)
    {
        bool open = IsKeyPressed(KEY_F1);
        if (pressed(kSelect))
        {
            const double now = GetTime();
            open |= g_lastSelect >= 0.0 && now - g_lastSelect < kDoubleTapSeconds;
            g_lastSelect = open ? -1.0 : now;
        }
        if (open)
        {
            g_open = true;
            g_help = false;
            g_item = std::min(g_item, std::max(itemCount(g_tab) - 1, 0));
        }
        return;
    }

    if (IsKeyPressed(KEY_F1) || pressed(kSelect))
    {
        close();
        return;
    }
    if (g_help)
    {
        if (pressed(kTriangle) || pressed(kCircle))
            g_help = false;
        return;
    }
    if (pressed(kCircle))
    {
        close();
        return;
    }
    const Tab &tab = kTabs[g_tab];
    if (pressed(kL1) || pressed(kR1))
    {
        g_tab = (g_tab + (pressed(kR1) ? 1 : kTabCount - 1)) % kTabCount;
        g_item = 0;
        g_quitArmed = false;
        return;
    }
    const int count = itemCount(g_tab);
    if (count == 0)
        return;
    if (pressed(kUp) || pressed(kDown))
    {
        g_item = (g_item + (pressed(kDown) ? 1 : count - 1)) % count;
        g_quitArmed = false;
    }
    if (isTuningTab(g_tab))
    {
        g_tabNow = g_tab;
        int dir = (pressed(kCross) || pressed(kRight)) ? 1 : (pressed(kLeft) ? -1 : 0);
        {
            // An action (Mark this moment) fires once per press of X only:
            // not on left / right (the stick near its threshold toggled
            // kRight on and off: the console's count ran into the 70s), and
            // not again within 0.3 s.
            const int k = tuningIndex(g_item);
            if (k >= 0 && native_renderer::tuningItems()[k].action)
            {
                static double last = -1.0;
                const double now = GetTime();
                dir = (pressed(kCross) && (last < 0.0 || now - last >= 0.3)) ? 1 : 0;
                if (dir != 0)
                    last = now;
            }
        }
        if (dir != 0)
        {
            const int k = tuningIndex(g_item);
            if (k < 0)
                native_renderer::tuning() = native_renderer::defaultTuning();
            else
                native_renderer::tuningStep(native_renderer::tuningItems()[k], dir);
            save();
        }
        if (pressed(kTriangle))
            g_help = true;
        return;
    }
    const ItemId id = tab.items[g_item].id;
    if (pressed(kCross) || pressed(kRight))
        change(id, +1);
    else if (pressed(kLeft) && id != kQuit)
        change(id, -1);
    if (pressed(kTriangle))
        g_help = true;
}

void drawPerfOverlay(int screenWidth, int screenHeight)
{
    // The scene recorder's marker (Debug "Record scene"), top right.
    if (!g_open)
    {
        bool recording = false;
        const std::string rec = native_recorder::markerText(portuguese(), recording);
        if (!rec.empty())
        {
            const int size = std::max(10, screenHeight / 34 / 2 * 2);
            const int w = MeasureText(rec.c_str(), size) + size + 16, x = screenWidth - w - 8, y = 8;
            DrawRectangle(x, y, w, size + 8, Color{0, 0, 0, 170});
            const bool blink = recording && (static_cast<int>(GetTime() * 2.0) & 1) == 0;
            const int dot = size * 2 / 3;
            DrawRectangle(x + 6 + (size - dot) / 2, y + 4 + (size - dot) / 2, dot, dot, blink || !recording ? Color{230, 40, 40, 255} : Color{120, 20, 20, 255});
            DrawText(rec.c_str(), x + size + 10, y + 4, size, Color{240, 240, 240, 255});
        }
    }
    if (!native_renderer::tuning().perfOverlay || g_open)
        return;
    native_renderer::PerfStats p;
    const bool pt = portuguese();
    const int size = std::max(10, screenHeight / 34 / 2 * 2); // 16 px on the Vita
    const int x = 8, y = 8, lineH = size + 3;
    const int graphW = native_renderer::PerfStats::kHistory * 2, graphH = size * 4;
    int boxW = std::max(graphW, size * 19) + 12;
    if (!native_renderer::perfSnapshot(p))
    {
        DrawRectangle(x, y, boxW, lineH + 8, Color{0, 0, 0, 170});
        DrawText(pt ? "desempenho: esperando o jogo..." : "performance: waiting for the game...", x + 6, y + 4, size, Color{220, 220, 220, 255});
        return;
    }
    char l1[96], l2[96], l3[96];
    std::snprintf(l1, sizeof(l1), "%.1f fps  %.1f ms  %s %.0f", p.fps, p.frameMs, pt ? "pior" : "worst", p.worstMs);
    std::snprintf(l2, sizeof(l2), "%s %.1f  %s %.1f  %s %.1f", pt ? "jogo" : "game", p.gameMs, pt ? "ganchos" : "hooks", p.hooksMs, pt ? "espera" : "wait",
                  p.waitMs);
    std::snprintf(l3, sizeof(l3), "%s %.1f  present %.1f  draws %.0f", p.renderThread ? "render (thr)" : "render", p.renderMs, p.presentMs, p.draws);
    const int boxH = lineH * 3 + graphH + 14;
    for (const char *l : {l1, l2, l3})
        boxW = std::max(boxW, MeasureText(l, size) + 12);
    DrawRectangle(x, y, boxW, boxH, Color{0, 0, 0, 170});
    const Color good{120, 255, 120, 255}, mid{255, 210, 90, 255}, bad{255, 100, 90, 255}, text{225, 228, 230, 255};
    const Color headline = p.frameMs <= 34.0f ? good : (p.frameMs <= 50.0f ? mid : bad);
    DrawText(l1, x + 6, y + 4, size, headline);
    DrawText(l2, x + 6, y + 4 + lineH, size, text);
    DrawText(l3, x + 6, y + 4 + lineH * 2, size, text);
    // The last 120 frames, 0 to 100 ms; the line at 33 ms (30 fps), the dim one at 50.
    const int gx = x + 6, gy = y + 8 + lineH * 3, gh = graphH - 4;
    auto yOf = [&](float ms) { return gy + gh - static_cast<int>(std::min(ms, 100.0f) / 100.0f * static_cast<float>(gh)); };
    for (int i = 0; i < native_renderer::PerfStats::kHistory; ++i)
    {
        const float ms = p.history[i];
        if (ms <= 0.0f)
            continue;
        const int top = yOf(ms);
        DrawRectangle(gx + i * 2, top, 2, gy + gh - top, ms <= 34.0f ? good : (ms <= 50.0f ? mid : bad));
    }
    DrawRectangle(gx, yOf(33.3f), graphW, 1, Color{255, 255, 255, 140});
    DrawRectangle(gx, yOf(50.0f), graphW, 1, Color{255, 255, 255, 60});
    (void)screenWidth;
}

void draw(int screenWidth, int screenHeight)
{
    if (!g_open)
        return;
    const int size = std::max(10, screenHeight / 27 / 2 * 2); // ~20 px on the Vita
    const int small = std::max(10, size * 4 / 5);
    const int pad = size;
    const int boxW = std::min(screenWidth - 2 * pad, size * 34);
    const int boxH = std::min(screenHeight - 2 * pad, size * 20);
    const int boxX = (screenWidth - boxW) / 2, boxY = (screenHeight - boxH) / 2;
    const Color panel{12, 14, 18, 230}, line{70, 80, 90, 255}, text{225, 228, 230, 255}, dim{150, 156, 162, 255},
        accent{232, 196, 120, 255}, highlight{255, 255, 255, 28};

    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 110});
    DrawRectangle(boxX, boxY, boxW, boxH, panel);
    DrawRectangle(boxX, boxY, boxW, 2, accent);
    DrawText(T("SETTINGS"), boxX + pad, boxY + pad * 2 / 3, size, accent);

    // Tabs (smaller when they do not fit the box)
    int tabX = boxX + pad;
    const int tabY = boxY + pad * 2;
    int tabSize = size, tabGap = size * 3 / 2;
    {
        int total = 0;
        for (int i = 0; i < kTabCount; ++i)
            total += MeasureText(T(kTabs[i].label), size) + tabGap;
        if (total > boxW - 2 * pad)
        {
            tabGap = size;
            total = 0;
            for (int i = 0; i < kTabCount; ++i)
                total += MeasureText(T(kTabs[i].label), size) + tabGap;
            if (total > boxW - 2 * pad)
                tabSize = small;
        }
    }
    for (int i = 0; i < kTabCount; ++i)
    {
        const char *label = T(kTabs[i].label);
        const int w = MeasureText(label, tabSize);
        if (i == g_tab)
            DrawRectangle(tabX - size / 3, tabY + size + size / 4, w + size * 2 / 3, 2, accent);
        DrawText(label, tabX, tabY, tabSize, i == g_tab ? text : dim);
        tabX += w + tabGap;
    }
    DrawRectangle(boxX + pad, tabY + size * 3 / 2 + 2, boxW - 2 * pad, 1, line);

    const Tab &tab = kTabs[g_tab];
    const int listY = tabY + size * 2 + size / 2;
    const int helpY = boxY + boxH - size * 5;
    const bool renderer = isTuningTab(g_tab);
    g_tabNow = g_tab;
    if (g_help && itemCount(g_tab) > 0)
    {
        DrawText(renderer ? rendererLabel(g_item) : T(tab.items[g_item].label), boxX + pad, listY, size, accent);
        drawWrapped(renderer ? rendererHelp(g_item) : T(tab.items[g_item].help), boxX + pad, listY + size * 2, boxW - 2 * pad, small, text, small / 2);
        DrawText(T("Triangle / O: back"), boxX + pad, boxY + boxH - size * 3 / 2, small, dim);
        return;
    }
    if (itemCount(g_tab) == 0)
    {
        int y = listY;
        for (const char *l : {"Shadow of the Colossus, recompiled to run natively.", "celosia - port by HenryKun55", "",
                              "Shadow of the Colossus (c) Sony Interactive Entertainment.",
                              "Built on PS2Recomp. Thanks to the sotc-pc project."})
        {
            DrawText(T(l), boxX + pad, y, small, *l ? text : dim);
            y += small * 3 / 2;
        }
    }
    else
    {
        // Scrolls when the list is longer than the box (the Renderer tab).
        const int count = itemCount(g_tab);
        const int rows = std::max(1, (helpY - size - listY) / (size * 3 / 2));
        const int first = std::clamp(g_item - rows + 1, 0, std::max(0, count - rows));
        for (int i = first; i < count && i < first + rows; ++i)
        {
            const int y = listY + (i - first) * size * 3 / 2;
            if (i == g_item)
                DrawRectangle(boxX + pad / 2, y - size / 4, boxW - pad, size * 3 / 2, highlight);
            DrawText(renderer ? rendererLabel(i) : T(tab.items[i].label), boxX + pad, y, size, i == g_item ? text : dim);
            const std::string value = renderer ? rendererValue(i) : valueText(tab.items[i].id);
            DrawText(value.c_str(), boxX + boxW * 11 / 20, y, size, i == g_item ? accent : dim);
        }
        if (first > 0)
            DrawText("^", boxX + boxW - pad, listY, small, dim);
        if (first + rows < count)
            DrawText("v", boxX + boxW - pad, listY + (rows - 1) * size * 3 / 2, small, dim);
        DrawRectangle(boxX + pad, helpY - size / 2, boxW - 2 * pad, 1, line);
        // The help in the space above the controls line, smaller when long.
        const char *help = renderer ? rendererHelp(g_item) : T(tab.items[g_item].help);
        const int helpRoom = boxY + boxH - size * 3 / 2 - helpY;
        int helpSize = small;
        while (helpSize > 10 && drawWrapped(help, 0, 0, boxW - 2 * pad, helpSize, dim, helpSize / 3, false) * (helpSize + helpSize / 3) > helpRoom)
            helpSize -= 2;
        drawWrapped(help, boxX + pad, helpY, boxW - 2 * pad, helpSize, dim, helpSize / 3);
    }
    DrawText(T("L/R: tab   D-pad: choose   X: change   Triangle: help   O: close"), boxX + pad, boxY + boxH - size * 3 / 2, small, dim);
}
}
