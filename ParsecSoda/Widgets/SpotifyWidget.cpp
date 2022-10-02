#include "SpotifyWidget.h"

bool SpotifyWidget::render() {

    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Spotify", (bool*)0);
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Spotify List", ImVec2(size.x, size.y));



    ImGui::EndChild(); // Smash Bros List

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;
}

bool SpotifyWidget::renderSugoiButton(const char* text, const wchar_t* link) {
    bool result = false;

    AppColors::pushPrimary();
    AppFonts::pushSugoiDekai();
    if (ImGui::Button(text))
    {
        ShellExecute(0, 0, link, 0, 0, SW_SHOW);
        result = true;
    }
    AppFonts::pop();
    AppColors::pop();

    return result;
}

bool SpotifyWidget::renderLinkButton(const char* text, const wchar_t* link) {
    bool result = false;

    AppColors::pushPrimary();
    AppFonts::pushTitle();
    if (ImGui::Button(text))
    {
        ShellExecute(0, 0, link, 0, 0, SW_SHOW);
        result = true;
    }
    AppFonts::pop();
    AppColors::pop();

    return result;
}