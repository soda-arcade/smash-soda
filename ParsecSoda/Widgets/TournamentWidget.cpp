#include "TournamentWidget.h"

TournamentWidget::TournamentWidget(Hosting& hosting)
    : _hosting(hosting)
{
}

bool TournamentWidget::render() {

    // Widget top
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Tournament", (bool*)0);

    AppStyle::pushInput();
    ImVec2 size = ImGui::GetContentRegionAvail();

    // Content

    // Widget bottom
    AppStyle::pop();
    ImGui::End();

    return true;

}