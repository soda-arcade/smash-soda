#pragma once

#include <string>
#include "../imgui/imgui.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"
#include "curl/curl.h"

class VersionWidget
{
public:
	static std::string version;

	static bool render();
	static bool renderPopup();
	static bool checkForUpdate();
	static bool update();
	static bool downloadFile();
};

