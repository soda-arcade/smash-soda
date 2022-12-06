#pragma once

#include "../imgui/imgui.h"
#include "../Hosting.h"
#include "../globals/AppIcons.h"
#include "../globals/AppStyle.h"
#include "IconButton.h"
#include "TitleTooltipWidget.h"
#include "ConfirmPopupWidget.h"
#include "PopupWidgetEdit.h"

class LibraryWidget
{
public:

	LibraryWidget(Hosting& hosting);
	bool render();
	bool renderGameList();

private:
	
	// Dependency injection
	Hosting& _hosting;
	GameDataList& _gameList;

	char _filterText[256] = "";

};