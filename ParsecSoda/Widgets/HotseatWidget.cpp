#include "HotseatWidget.h"
#include "../Hosting.h"
#include "../Modules/Hotseat.h"
extern Hosting g_hosting;

HotseatWidget::HotseatWidget(Hosting& hosting)
    : _hosting(hosting)
{
	_playTime = Config::cfg.hotseat.playTime;
	_resetTime = Config::cfg.hotseat.resetTime;
    _minResetTime = Config::cfg.hotseat.minResetTime;
}

bool HotseatWidget::render(bool& showWindow) {

    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(800, 900));
    ImGui::Begin("Hotseat", &showWindow);
    if (!showWindow) Config::cfg.widgets.hotseat = showWindow;
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Hotseat List", ImVec2(size.x, size.y));

    AppStyle::pushLabel();
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 42);
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(0, 5));

    if (ImGui::BeginTabBar("Hotseat Tabs", ImGuiTabBarFlags_None))
    {
        AppFonts::pushInput();
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("Users"))
        {
            renderOverview();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings")) {
            renderSettings();
            ImGui::EndTabItem();
        }
        AppColors::pop();
        AppFonts::pop();
        ImGui::EndTabBar();
    }

    ImGui::EndChild();

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}

/// <summary>
/// Render users in the hotseat
/// </summary>
void HotseatWidget::renderOverview() {

    static ImVec2 size;
    size = ImGui::GetContentRegionAvail();
    static ImVec2 cursor;

    cursor = ImGui::GetCursorPos();
    ImGui::BeginGroup();

    ImGui::Dummy(ImVec2(0, 5));

    if (Config::cfg.hotseat.enabled) {

        // Calculate the maximum to parent height
        float groupMaxHeight = size.y - 10;

        // Begin a scrolling region with the maximum height
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, groupMaxHeight), true, ImGuiWindowFlags_HorizontalScrollbar);

        // For each HotseatUser in Hotseat::instance.users
        for (Hotseat::HotseatUser& user : Hotseat::instance.users) {

            cursor = ImGui::GetCursorPos();

            AppStyle::pushInput();
            ImGui::Indent(5);
            ImGui::Text("%s", user.userName.c_str());
            ImGui::Unindent(5);
            AppStyle::pop();

            ImGui::SameLine();
            AppStyle::pushPositive();
            ImGui::Text("#%d", user.userId);
            AppStyle::pop();

            ImGui::Indent(5);
            AppStyle::pushLabel();
            //ImGui::Text(user.status.c_str());

            // Get the time remaining for the user (in minutes)
            if (user.stopwatch->isRunning()) {
				ImGui::Text(user.stopwatch->getRemainingTime().c_str());
			}
			else {
                int cooldownTime = Hotseat::instance.getCoolDownTime(user.userId);
                if (cooldownTime > 0) {
					ImGui::Text("%d minute(s) to wait", cooldownTime);
				}
				else {
                    ImGui::Text(user.stopwatch->getRemainingTime().c_str());
				}
			}

            AppStyle::pop();
            ImGui::Unindent(5);

            ImGui::Dummy(ImVec2(0, 5));

        }

        ImGui::EndChild();

    }

    ImGui::EndGroup();
    
}

/// <summary>
/// Render the hotseat settings
/// </summary>
void HotseatWidget::renderSettings() {

    static ImVec2 size;
    size = ImGui::GetContentRegionAvail();
    static ImVec2 cursor;

    ImGui::Dummy(ImVec2(0, 5));

    cursor = ImGui::GetCursorPos();
    ImGui::BeginGroup();

    if (ImForm::InputNumber("PLAY TIME", _playTime, 1, 999,
        "This is how much time a user is allowed to play every reset (in minutes).")) {
        Config::cfg.hotseat.playTime = _playTime;
        _playTime = Config::cfg.hotseat.playTime;
        Config::cfg.Save();
    }

    if (ImForm::InputNumber("RESET TIME", _resetTime, 0, 999,
		"This is how often play time resets for each user (in minutes).")) {
        if (_resetTime < 0) {
            _resetTime = 0;
        }
        else {
            Config::cfg.hotseat.resetTime = _resetTime;
            _resetTime = Config::cfg.hotseat.resetTime;
            Config::cfg.Save();
        }
	}

    if (ImForm::InputNumber("MINIMUM RESET TIME", _minResetTime, 0, 999,
        "This is the time a user must wait by default each time their play time runs out (in minutes).")) {
        if (_minResetTime < 0) {
            _minResetTime = 0;
        }
        else {
            Config::cfg.hotseat.minResetTime = _minResetTime;
            _minResetTime = Config::cfg.hotseat.minResetTime;
            Config::cfg.Save();
        }
    }

    ImGui::EndGroup();

}