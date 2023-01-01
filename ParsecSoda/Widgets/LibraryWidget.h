#pragma once

#include "../ImGui/imgui.h"
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
	void renderPopup(GameData game);

private:
	
	// Dependency injection
	Hosting& _hosting;
	GameDataList& _gameList;

	char _filterText[256] = "";

	char _editName[256] = "";
	char _editPath[256] = "";
	char _editParam[256] = "";
	char _editThumb[256] = "";

};