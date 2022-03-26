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

	bool _basicVersion = false;
};