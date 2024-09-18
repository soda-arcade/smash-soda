#include "VideoWidget.h"

VideoWidget::VideoWidget(Hosting& hosting)
    : _hosting(hosting), _dx11(_hosting.getDX11())
{
    _fps = (int)Config::cfg.video.fps;
    _bandwidth = (int)Config::cfg.video.bandwidth;
}

bool VideoWidget::render(bool& showWindow)
{
    static float indentSize = 0;
    static ImVec2 dummySize = ImVec2(0.0f, 5.0f);
    static ImVec2 cursor;

    AppStyle::pushTitle();

    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(800, 900));
    ImGui::Begin("Video", &showWindow);
    if (!showWindow) Config::cfg.widgets.video = showWindow;


    AppStyle::pushLabel();

    static ImVec2 size;
    size = ImGui::GetContentRegionAvail();

    // =========================================================
    // Monitors
    // =========================================================
    static UINT currentScreen = 0;
    currentScreen = _dx11.getScreen();

    static vector<string> screens = _dx11.listScreens();
    static string screenName = (currentScreen < screens.size()) ? screens[currentScreen] : "";

    ImGui::SetNextItemWidth(size.x);

    AppStyle::pushLabel();
    ImGui::Text("Monitors");
    AppStyle::pop();
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("##Monitor list", screenName.c_str()))
    {
        for (UINT i = 0; i < screens.size(); ++i)
        {
            bool isSelected = (currentScreen == i);
            if (ImGui::Selectable(screens[i].c_str(), isSelected))
            {
                currentScreen = i;
                screenName = screens[currentScreen];
                _dx11.setScreen(i);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppStyle::pop();
    TitleTooltipWidget::render("Monitor List", "Choose the screen you want to display.\n\n* Avoid changing this while stream is already running.");
    
    ImGui::Dummy(dummySize);

    // =========================================================
    // Adapters
    // =========================================================
    static size_t currentGPU = 0;
    currentGPU = _dx11.getGPU();

    static vector<string> gpus = _dx11.listGPUs();
    static string gpuName = (currentGPU < gpus.size()) ? gpus[currentGPU] : "";

    ImGui::SetNextItemWidth(size.x);

    AppStyle::pushLabel();
    ImGui::Text("Adapters");
    AppStyle::pop();
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("##Adapter list", gpuName.c_str()))
    {
        for (UINT i = 0; i < gpus.size(); ++i)
        {
            bool isSelected = (currentGPU == i);
            if (ImGui::Selectable(gpus[i].c_str(), isSelected))
            {
                currentGPU = i;
                gpuName = gpus[currentGPU];
                _dx11.setGPU(i);
                screens = _dx11.listScreens();
                _dx11.setScreen(currentScreen);
                currentScreen = _dx11.getScreen();
                screenName = currentScreen >= screens.size() ? "" : screens[currentScreen];
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppStyle::pop();
    TitleTooltipWidget::render(
        "Adapter List",
        (
            string() +
            "Choose the GPU that renders your desktop.\n" +
            "Picking the wrong GPU may result in black screen.\n\n" +
            "* Avoid changing this while stream is already running."
        ).c_str()
    );

    ImGui::Dummy(dummySize);

    // AppStyle::pushLabel();
    // ImGui::Text("RESOLUTION");
    // AppStyle::pushInput();
    // ImGui::SetNextItemWidth(size.x);
    // if (ImGui::BeginCombo("### Thumbnail picker combo", Config::cfg.resolutions[Config::cfg.video.resolutionIndex].label.c_str(), ImGuiComboFlags_HeightLarge)) {
    //     for (size_t i = 0; i < 16; ++i) {
    //         bool isSelected = (i == Config::cfg.video.resolutionIndex);
    //         if (ImGui::Selectable(Config::cfg.resolutions[i].label.c_str(), isSelected)) {
	// 			Config::cfg.video.resolutionIndex = i;
	// 			Config::cfg.Save();
    //         }
    //         if (isSelected) {
    //             ImGui::SetItemDefaultFocus();
    //         }
    //     }
    //     ImGui::EndCombo();
    // }
    // AppStyle::pushLabel();
    // ImGui::TextWrapped("Choose the output resolution for the stream.");
    // AppStyle::pop();

    // =========================================================
    // Bandwidth
    // =========================================================
    static int previousBandwidth;
    static int previousFps;
    previousBandwidth = _bandwidth;
    previousFps = _fps;

    cursor = ImGui::GetCursorPos();

    ImGui::BeginGroup();
    AppStyle::pushLabel();
    ImGui::Text("Bitrate");
    AppStyle::pop();
    if (IntRangeWidget::render("Bandwidth (Mbps)", _bandwidth, 1, 1000, 1))
    {
        TitleTooltipWidget::render("Encoder Bandwidth", "Limits the encoder bandwidth usage.\n(This value is split between guests.)");
    }
    ImGui::EndGroup();

    ImGui::SetCursorPos(ImVec2(cursor.x + size.x - 105, cursor.y));

    // =========================================================
    // FPS
    // =========================================================
    ImGui::BeginGroup();
    AppStyle::pushLabel();
    ImGui::Text("FPS");
    AppStyle::pop();
    if (IntRangeWidget::render("Encoder FPS", _fps, 10, 250, 0.5f)) {
        TitleTooltipWidget::render("Encoder FPS", "Limits the amount of frames per second.");
    }
    ImGui::EndGroup();

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    static Debouncer debouncer(DEBOUNCER_TIME_MS, [&]() {
        _hosting.setHostVideoConfig(_fps, _bandwidth);
        _hosting.applyHostConfig();
        Config::cfg.Save();
    });
    if (previousFps != _fps || previousBandwidth != _bandwidth)
    {
        debouncer.start();
    }

    return true;
}