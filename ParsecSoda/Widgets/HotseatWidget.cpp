#include "HotseatWidget.h"
#include "../Hosting.h"
#include "../Modules/Hotseat.h"
extern Hosting g_hosting;

HotseatWidget::HotseatWidget(Hosting& hosting)
    : _hosting(hosting)
{
	_playTime = Config::cfg.hotseat.playTime;
	_resetTime = Config::cfg.hotseat.resetTime;
    _reminderInterval = Config::cfg.hotseat.reminderInterval;
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
            if (user.stopwatch->isRunning() && !user.cooldown) {
                AppColors::pushLabel();
                ImGui::Text("REMAINING ");
                AppColors::pop();
                ImGui::SameLine();
                AppColors::pushInput();
				ImGui::Text(user.stopwatch->getRemainingTime().c_str());
                AppColors::pop();
			}
			else {
                if (user.cooldown) {
                    AppColors::pushPrimary();
                    ImGui::Text("COOLDOWN ");
                    AppColors::pop();
                    ImGui::SameLine();
                    AppColors::pushInput();
                    ImGui::Text(Hotseat::instance.getCooldownRemaining(user.userId).c_str());
                    AppColors::pop();
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
    }

    if (ImForm::InputNumber("RESET TIME", _resetTime, _playTime, 999,
		"This is how often play time resets for each user (in minutes).")) {
        Config::cfg.hotseat.resetTime = _resetTime;
        _resetTime = Config::cfg.hotseat.resetTime;
	}

    if (ImForm::InputNumber("REMINDER INTERVAL", _reminderInterval, 0, _playTime-1,
        "Print remaining hotseat time in chat every (x) minutes. 0 for no reminders.")) {
        Config::cfg.hotseat.reminderInterval = _reminderInterval;
        _reminderInterval = Config::cfg.hotseat.reminderInterval;
    }

    ImGui::EndGroup();

}