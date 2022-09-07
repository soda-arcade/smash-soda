#pragma once

#include "../imgui/imgui.h"
#include "../Hosting.h"
#include "../MetadataCache.h"
#include "../globals/AppIcons.h"
#include "../globals/AppFonts.h"
#include "../globals/AppColors.h"
#include "../globals/AppStyle.h"
#include "TitleTooltipWidget.h"
#include "TooltipWidget.h"
#include "IntRangeWidget.h"
#include "../MetadataCache.h"

class WebSocketWidget
{
public:
	WebSocketWidget(Hosting& hosting);
	bool render();
private:
	Hosting& _hosting;
	char _ws_uri[50] = "ws://127.0.0.1:9002";
	char _ws_password[32] = "";
};
