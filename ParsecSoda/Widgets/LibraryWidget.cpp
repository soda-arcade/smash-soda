#include "LibraryWidget.h"

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

    ImGui::BeginChild("Library List", ImVec2(size.x, size.y));

        AppStyle::pushLabel();
        ImGui::Text("FILTER GAMES");
        AppStyle::pop();
        ImGui::SetNextItemWidth(size.x - 42);
        ImGui::InputText("### Game List Filter", _filterText, 256);
        TitleTooltipWidget::render("Filter Games", "Type game Name or ID to filter.");

        ImGui::Dummy(ImVec2(0, 5));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 5));

        renderGameList();

    ImGui::EndChild();

    // Widget bottom
    AppStyle::pop();
    ImGui::End();

    return true;

}

bool LibraryWidget::renderGameList() {

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
    ImGui::BeginChild("gameslist");

    static bool showRemovePopup = false;
    static bool showEditPopup = false;
    static string popupTitle = "";
    static size_t popupRemoveIndex = -1;
    static size_t popupEditIndex = -1;
    static string name;
    static string path;
    static string parameters;
    static string thumbnailPath;
    static uint32_t gameID;
    static vector<GameData>& _games = _gameList.getGames();
    static string filterTextStr;
    static bool filterSuccess = false;

    for (size_t i = 0; i < _games.size(); ++i)
    {
        name = _games[i].name;
        path = _games[i].path;
        parameters = _games[i].parameters;
        thumbnailPath = _games[i].thumbnailPath;
        gameID = _games[i].gameID;

        filterTextStr = _filterText;
        if (!filterTextStr.empty())
        {
            filterSuccess = (Stringer::fuzzyDistance(_filterText, name) == 0);
            if (!filterSuccess)
            {
                filterSuccess = (Stringer::fuzzyDistance(_filterText, to_string(gameID)) == 0);
            }

            if (!filterSuccess)
            {
                continue;
            }
        }

        // Remove game
        if (ImGui::ImageButton(AppIcons::trash, ImVec2(30, 30))) {
            popupTitle = string("Remove Game##Popup");
            showRemovePopup = true;
            popupRemoveIndex = i;
            popupEditIndex = -1;
            ImGui::OpenPopup(popupTitle.c_str());
        }

        if (i == popupRemoveIndex) {
            if (ConfirmPopupWidget::render(
                popupTitle.c_str(),
                showRemovePopup,
                ("Remove\n" + name).c_str()
            )) {
                
                // Remove from game list
                _hosting.removeGame(name);

            }

        }

        ImGui::SameLine();

        // Edit game
        if (ImGui::ImageButton(AppIcons::edit, ImVec2(30, 30))) {
            popupTitle = string("Remove Game##Popup");
            showEditPopup = true;
            popupEditIndex = i;
            popupRemoveIndex = -1;
            ImGui::OpenPopup(name.c_str());
        }

        if (ImGui::BeginPopup(name.c_str())) {

            AppStyle::pushLabel();
            ImGui::Text("Edit Game");

            AppStyle::pushInput();

            AppStyle::pop();
            ImGui::EndPopup();

        }

        ImGui::SameLine();

        ImGui::BeginGroup();
        AppStyle::pushInput();
        ImGui::SameLine();
        ImGui::Indent(10);
        ImGui::Text("%s", name.c_str());
        AppStyle::pop();
        ImGui::Unindent(10);
        AppFonts::pushInput();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.50f, 0.60f, 1.00f));
        AppStyle::pop();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::EndGroup();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();

    return true;

}