#include "OverlayWidget.h"

OverlayWidget::OverlayWidget(Hosting& hosting)
    : _hosting(hosting)
{
}

bool OverlayWidget::render() {

    // Widget top
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Overlay", (bool*)0);

    AppStyle::pushInput();
    ImVec2 size = ImGui::GetContentRegionAvail();

    // Launch Overlay
    AppColors::pushButtonSolid();
    if (ImGui::Button("Launch Overlay")) {
        _hosting.getOverlay().start();
    }
    AppColors::pushButton();

    // Widget bottom
    AppStyle::pop();
    ImGui::End();

    return true;

}