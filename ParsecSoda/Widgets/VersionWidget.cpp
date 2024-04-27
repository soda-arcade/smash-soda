#include "VersionWidget.h"

// Constructor
VersionWidget::VersionWidget() {
	version = "4.0.1";
	latestVersion = "";
	changeLog = "";
	error = "";
    strcpy_s(_email, "");
    strcpy_s(_password, "");
}

bool VersionWidget::render() {

    static ImVec2 res;
    static ImVec2 cursor;

    res = ImGui::GetMainViewport()->Size;
    cursor = ImGui::GetCursorPos();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::SetNextWindowPos(ImVec2(res.x - 160, res.y - 107));
    ImGui::SetNextWindowSize(ImVec2(160, 52));
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

/// <summary>
/// Login window for Soda Arcade
/// </summary>
/// <returns></returns>
bool VersionWidget::renderLoginWindow() {

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
    ImGui::Begin("Soda Arcade###Soda Arcade Login Window", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    // Center image
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 150);
    ImGui::Image(AppIcons::sodaArcadeLogo, ImVec2(300, 70));

    AppStyle::pushSmall();

    // Text with wrap
	ImGui::TextWrapped("Soda Arcade is a new service for advertising your Parsec room when you mark it as public. You will need to create a account to use this service.");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    AppStyle::pushInput();

	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 48);
    AppStyle::pushButton();
    AppColors::pushButtonSolid();
    if (ImGui::Button("Create account")) {
		const wchar_t* link = L"https://soda-arcade.com/register";
        ShellExecute(0, 0, link, 0, 0, SW_SHOW);
    }
    
	AppStyle::pushInput();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 96);
    AppStyle::pushButton();
    AppColors::pushButtonSolid();
    if (ImGui::Button("I don't want to use Soda Arcade")) {
		Config::cfg.room.privateRoom = true;
		Config::cfg.arcade.token = "";
		Config::cfg.arcade.showLogin = false;
		Config::cfg.Save();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

	ImGui::Dummy(ImVec2(0, 10));

	if (ImForm::InputText("EMAIL ADDRESS", _email, "The e-mail address you signed up to Soda Arcade with.")) {
        	
    }

	if (ImForm::InputPassword("PASSWORD", _password, "The password you signed up to Soda Arcade with.")) {

	}

	ImGui::Spacing();

	AppStyle::pushButton();
	AppColors::pushButtonSolid();
    if (ImGui::Button("Login")) {

		error = "";

		std::string email = _email;
		std::string password = _password;

        if (Arcade::instance.login(email, password)) {
            Config::cfg.arcade.showLogin = false;
            Config::cfg.Save();
		} else {
			error = "Invalid e-mail or password.";
		}

    }

    ImGui::Spacing();

    AppStyle::pop();
    AppStyle::pushLabel();

	if (error.length() > 0) {
        AppStyle::pop();
        AppStyle::pushInput();
		ImGui::TextWrapped(error.c_str());
    }

    AppColors::pop();
    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}

/// <summary>
/// Renders the overlay download window
/// </summary>
/// <returns></returns>
bool VersionWidget::renderDownloadWindow() {

    AppStyle::pushTitle();

    static ImVec2 res;
    static ImVec2 size = ImVec2(400, 200);

    res = ImGui::GetMainViewport()->Size;

    ImGui::SetNextWindowPos(ImVec2(
        (res.x - size.x) * 0.5f,
        (res.y - size.y) * 0.5f
    ));
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowFocus();
    ImGui::Begin("Update Overlay###Overlay Download Window", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    AppStyle::pushSmall();

    // Text with wrap
    ImGui::TextWrapped("The overlay files need to be updated. Delete the contents of the overlay folder wherever you have Smash Soda installed, and download the new files.");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 35);
    AppStyle::pushButton();
    AppColors::pushButtonSolid();
    if (ImGui::Button("Download")) {
        const wchar_t* link = L"https://github.com/MickeyUK/smash-soda-overlay/releases/latest";
        ShellExecute(0, 0, link, 0, 0, SW_SHOW);
        Config::cfg.overlay.update = false;
        Config::cfg.Save();
        showDownload = false;
    }

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 85);
    AppStyle::pushButton();
    AppColors::pushButtonSolid();
    if (ImGui::Button("I've already downloaded")) {
        showDownload = false;
        Config::cfg.overlay.update = false;
        Config::cfg.Save();
    }

    AppColors::pop();
    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;

}

/// <summary>
/// Renders the update log window
/// </summary>
/// <returns></returns>
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