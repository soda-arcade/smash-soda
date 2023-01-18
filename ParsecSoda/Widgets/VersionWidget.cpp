#include "VersionWidget.h"

std::string VersionWidget::version = "2.58";

bool VersionWidget::render()
{

    static ImVec2 res;
    static ImVec2 cursor;

    res = ImGui::GetMainViewport()->Size;
    cursor = ImGui::GetCursorPos();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::SetNextWindowPos(ImVec2(res.x - 150, res.y - 107));
    ImGui::SetNextWindowSize(ImVec2(150, 52));
    ImGui::Begin("##Version", (bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
    AppStyle::pushInput();
    ImGui::Text("FPS: ");
    ImGui::SameLine();
    ImGui::Text("%.0f", ImGui::GetIO().Framerate);

    std::string versionString = "Smash Soda v. " + version;
    ImGui::Text(versionString.c_str());
    AppStyle::pop();
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    return true;
}

bool VersionWidget::renderUpdateWindow() {

    AppStyle::pushTitle();

    static ImVec2 res;
    static ImVec2 size = ImVec2(400, 500);

    res = ImGui::GetMainViewport()->Size;

    ImGui::SetNextWindowPos(ImVec2(
        (res.x - size.x) * 0.5f,
        (res.y - size.y) * 0.5f
    ));
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowFocus();
    ImGui::Begin("Update###Update Window", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    AppStyle::pushInput();

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}