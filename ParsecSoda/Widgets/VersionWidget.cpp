#include "VersionWidget.h"

// Constructor
VersionWidget::VersionWidget() {
	version = "3.0.0";
	latestVersion = "";
	changeLog = "";
}

// Update the version
void VersionWidget::update(std::string jsonString) {

	thread parseThread = thread([this, jsonString]() {

		// Parse JSON
		json j = json::parse(jsonString);
        
        j.at("version").get_to(latestVersion);
        j.at("content").get_to(changeLog);

		// Check if update is available
		if (version != latestVersion) {
			showUpdate = true;
		}

	});
    
	parseThread.detach();
    
}

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
    AppStyle::pop();
    ImGui::SameLine();
    AppStyle::pushPositive();
    ImGui::Text("%.0f", ImGui::GetIO().Framerate);
    AppStyle::pop();

    AppStyle::pushInput();
    std::string versionString = "Smash Soda v. ";
    ImGui::Text(versionString.c_str());
    AppStyle::pop();

    ImGui::SameLine();
    AppStyle::pushPositive();
	ImGui::Text(version.c_str());
    
    AppStyle::pop();
    ImGui::End();

    //ImGui::PopStyleColor();
    //ImGui::PopStyleColor();
    //ImGui::PopStyleColor();
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
    ImGui::Begin("Update Available###Update Window", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    
	AppStyle::pushPositive();
	ImGui::Text("Version: ");
    ImGui::SameLine();
    ImGui::Text(latestVersion.c_str());
    
    AppStyle::pushSmall();
	
    // Text with wrap
	ImGui::TextWrapped("A new version of Smash Soda is available. Please close this, pull the latest commit and rebuild!");
    
    ImGui::Spacing();
	ImGui::Separator();
    ImGui::Spacing();
    
    AppStyle::pushInput();
    ImGui::Markdown(changeLog.c_str(), changeLog.length(), mdConfig);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 50);
    AppStyle::pushButton();
    AppColors::pushButtonSolid();
	if (ImGui::Button("Close")) {
		showUpdate = false;
	}

    AppColors::pop();
    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}