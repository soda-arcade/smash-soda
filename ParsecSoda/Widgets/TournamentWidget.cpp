#include "TournamentWidget.h"

TournamentWidget::TournamentWidget(Hosting& hosting)
    : _hosting(hosting)
{
}

bool TournamentWidget::render(bool& showWindow) {

    // Widget top
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Tournament", &showWindow);
    // not done yet
    //if (!showWindow) Config::cfg.widgets.tournament = showWindow;

    AppStyle::pushInput();
    ImVec2 size = ImGui::GetContentRegionAvail();

    // Content

    // Widget bottom
    AppStyle::pop();
    ImGui::End();

    return true;

}