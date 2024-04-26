#pragma once

#include <string>
#include <thread>
#include "matoya.h"
#include "../imgui/imgui.h"
#include "../ImGui/imgui_markdown.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"
#include "../ImGui/imform.h"
#include "../Core/Config.h"
#include "../Modules/Arcade.h"

#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

class VersionWidget
{
public:
	bool showUpdate = false;
	bool showDownload = false;
	std::string version;
	std::string latestVersion;
	std::string changeLog;
	std::string error;

	char _email[256];
	char _password[256];

	ImGui::MarkdownConfig mdConfig;
	
	// Constructor
	VersionWidget();

	bool render();
	bool renderLoginWindow();
	bool renderUpdateWindow();
	bool renderDownloadWindow();
};

