#pragma once

#include "../imgui/imgui.h"
#include "../Hosting.h"
#include "../MetadataCache.h"
#include "../Debouncer.h"
#include "../globals/AppIcons.h"
#include "../globals/AppFonts.h"
#include "../globals/AppColors.h"
#include "../globals/AppStyle.h"
#include "TitleTooltipWidget.h"
#include "TooltipWidget.h"
#include "IntRangeWidget.h"

class SettingsWidget
{
public:
	SettingsWidget(Hosting& hosting);
	bool render();

private:
	// Dependency injection
	Hosting& _hosting;
	uint32_t DEBOUNCER_TIME_MS = 2000;

	bool _basicVersion = false;
	bool _disableMicrophone = false;
	bool _disableGuideButton = false;
	bool _disableKeyboard = false;
	bool _latencyLimitEnabled = false;
	unsigned int _latencyLimitValue = 0;
	unsigned int _theme = 0;
	char _discord[HOST_NAME_LEN] = "";
};