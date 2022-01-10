#include "VersionWidget.h"

bool VersionWidget::render()
{

    //LARGE_INTEGER timeNow, counts, frequency, fps; // , ms;
    //QueryPerformanceCounter(&timeNow);
    //QueryPerformanceFrequency(&frequency);
    //static LARGE_INTEGER lastTime;
    //if (lastTime.QuadPart > 0)
    //{
    //    counts.QuadPart = timeNow.QuadPart - lastTime.QuadPart;
    //    fps.QuadPart = frequency.QuadPart / counts.QuadPart;
    //    //ms.QuadPart = (counts.QuadPart*1000 / frequency.QuadPart);
    //}
    //else {
    //    fps.QuadPart = 0;
    //}
    //lastTime = timeNow;

    static ImVec2 res;
    static ImVec2 cursor;

    res = ImGui::GetMainViewport()->Size;
    cursor = ImGui::GetCursorPos();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    //ImGui::SetNextWindowPos(ImVec2(res.x - 100, res.y - 87));
     ImGui::SetNextWindowPos(ImVec2(res.x - 100, res.y - 107));
    //ImGui::SetNextWindowPos(ImVec2(res.x - 200, res.y - 187));
    //ImGui::SetNextWindowSize(ImVec2(100, 32));
    ImGui::SetNextWindowSize(ImVec2(100, 52));
    //ImGui::SetNextWindowSize(ImVec2(200, 132));
    ImGui::Begin("##Version", (bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
    AppStyle::pushLabel();
    //if (fps.QuadPart > 0)
    //    ImGui::Text("%lld", fps.QuadPart);
        //ImGui::Text("counts: %lld\nfrequency: %lld\nfps: %lld\nms: %lld",
        //    counts.QuadPart,
        //    frequency.QuadPart,
        //    fps.QuadPart,
        //    ms.QuadPart);
    ImGui::Text("v. 1.1.0 V");
    AppStyle::pop();
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    return true;
}