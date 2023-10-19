#include "LibraryWidget.h"
#include "../ImGui/imform.h"

LibraryWidget::LibraryWidget(Hosting& hosting)
    : _hosting(hosting), _gameList(hosting.getGameList())
{
}

bool LibraryWidget::render() {

    // Widget top
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(150, 150), ImVec2(800, 900));
    ImGui::Begin("Library", (bool*)0);

    AppStyle::pushInput();
    ImVec2 size = ImGui::GetContentRegionAvail();

    if (!showEditForm) {
		renderGameList();
	}
	else {
		renderForm(popupEditIndex);
    }

    return true;

}

bool LibraryWidget::renderGameList() {
    
    static bool showRemovePopup = false;
    static bool showEditPopup = false;
    static string popupTitle = "";
    static string name;
    static string path;
    static string parameters;
    static string thumbnailPath;
    static string gameID;
    static vector<GameData>& _games = _gameList.getGames();
    static string filterTextStr;
    static bool filterSuccess = false;
	string defaultID = "1wdoHfhhZH5lPuZCwGBete0HIAj";

    AppStyle::pushInput();
    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Library List", ImVec2(size.x, size.y));

    AppColors::pushInput();
    AppStyle::pushLabel();
    ImGui::Text("FILTER GAMES");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x - 100);
    ImGui::InputText("### Game List Filter", _filterText, 256);
    TitleTooltipWidget::render("Filter Games", "Type game Name or ID to filter.");

    ImGui::SameLine();

    ImGui::BeginGroup();
        ImGui::Indent(10);
        AppColors::pushButtonSolid();
        if (ImGui::Button("Add Game")) {
			popupEditIndex = -1;
            memset(_editName, 0, 256);
            memset(_editPath, 0, 256);
            memset(_editParam, 0, 256);
            memset(_editThumb, 0, 256);
            strncpy_s(_editID, defaultID.c_str(), 256);
			_editKiosk = false;
			_editHotseat = false;
			_editSeats = 1;
			showEditForm = true;
        }
        AppColors::pushButton();
	    ImGui::Unindent(10);
	ImGui::EndGroup();

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 10));

    ImGui::BeginChild("gameslist");

        for (size_t i = 0; i < _games.size(); ++i){

            name = _games[i].name;
            path = _games[i].path;
            parameters = _games[i].parameters;
            thumbnailPath = _games[i].thumbnailPath;
            gameID = _games[i].gameID;
        
            // Remove game
            IconButton::render(AppIcons::trash, AppColors::primary, ImVec2(30, 30));
            if (ImGui::IsItemActive()) {
                popupTitle = string("Remove Game##Popup");
                showRemovePopup = true;
                popupRemoveIndex = i;
                ImGui::OpenPopup(popupTitle.c_str());
            }

			// Edit game
			ImGui::SameLine();
			IconButton::render(AppIcons::edit, AppColors::primary, ImVec2(30, 30));
			if (ImGui::IsItemActive()) {
                strncpy_s(_editName, _games[i].name.c_str(), 256);
                strncpy_s(_editPath, _games[i].path.c_str(), 256);
                strncpy_s(_editParam, _games[i].parameters.c_str(), 256);
                strncpy_s(_editThumb, _games[i].thumbnailPath.c_str(), 256);
				strncpy_s(_editID, _games[i].gameID.c_str(), 256);
				_editKiosk = _games[i].kiosk;
				_editHotseat = _games[i].hotseat;
				_editSeats = _games[i].seats;
				popupEditIndex = i;
                showEditForm = true;
			}

            // Remove game popup
            if (i == popupRemoveIndex) {

                if (ConfirmPopupWidget::render(
                    popupTitle.c_str(),
                    showRemovePopup,
                    ("Remove\n" + name).c_str()
                )) {

                    // Remove from game list
                    _hosting.removeGame(name);
                    popupRemoveIndex = -1;
                    showRemovePopup = false;

                }
                
            }

            // Game title and path
            ImGui::SameLine();
            ImGui::BeginGroup();
			    ImGui::Indent(10);
                AppStyle::pushInput();
                ImGui::Text("%s", name.c_str());
				AppStyle::pushLabel();
				ImGui::Text("%s", path.c_str());
				ImGui::Unindent(10);
            ImGui::EndGroup();

            ImGui::Dummy(ImVec2(0, 5));
            
        }

    ImGui::EndChild();

    // Widget bottom
    AppStyle::pop();
    ImGui::End();

    return true;

}

bool LibraryWidget::renderForm(int index) {

	ImForm::InputText("Game Name", _editName, "Name of the game you want to add.");
    
	ImForm::InputText("Game Path", _editPath, "Path to the game executable.");

	ImForm::InputText("Launch Parameters", _editParam, "Parameters to pass to the game executable when launching in kiosk mode.");
    
	ImForm::InputText("Game ID", _editID, "The game ID that Parsec will use to identify the game and display the correct thumbnail. Leave as default if you do not know ID.");

	ImForm::InputCheckbox("Kiosk Mode", _editKiosk, "If checked, the game will be launched in kiosk mode. This will restart the game if it closes whilst the room is still open.");

	ImForm::InputCheckbox("Hotseat", _editHotseat, "If checked, the game will be launched in hotseat mode. This will cycle through guests in your room and allow them to play the game.");

	ImForm::InputNumber("Seats", _editSeats, 1, 100, "The number of seats available for guests to play the game. This will be ignored if hotseat is not enabled.");

    // Save button
    AppStyle::pushPositive();
	if (ImGui::Button("Save")) {

		// New game
        if (index < 0) {
            
			GameData game = GameData();
            game.itemID = _hosting.getGameList().getGames().size() + 1;
			game.name = _editName;
			game.path = _editPath;
			game.parameters = _editParam;
			game.thumbnailPath = _editThumb;
			game.gameID = _editID;
			game.kiosk = _editKiosk;
			game.hotseat = _editHotseat;
			game.seats = _editSeats;

			// Add to game list
            _hosting.getGameList().getGames().push_back(game);
            MetadataCache::saveGamesList(_hosting.getGameList().getGames());

		}
        
		// Edit game
		else {

			// Update game
			_hosting.getGameList().getGames()[index].name = _editName;
			_hosting.getGameList().getGames()[index].path = _editPath;
			_hosting.getGameList().getGames()[index].parameters = _editParam;
			_hosting.getGameList().getGames()[index].thumbnailPath = _editThumb;
			_hosting.getGameList().getGames()[index].gameID = _editID;
			_hosting.getGameList().getGames()[index].kiosk = _editKiosk;
			_hosting.getGameList().getGames()[index].hotseat = _editHotseat;
			_hosting.getGameList().getGames()[index].seats = _editSeats;

            MetadataCache::saveGamesList(_hosting.getGameList().getGames());
            
        }

		// Close form
		showEditForm = false;
        
	}

	ImGui::SameLine();

    // Cancel
	AppStyle::pushNegative();
	if (ImGui::Button("Cancel")) {
		showEditForm = false;
	}

    return true;
    
}