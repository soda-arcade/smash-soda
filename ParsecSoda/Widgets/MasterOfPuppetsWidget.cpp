#include "MasterOfPuppetsWidget.h"

MasterOfPuppetsWidget::MasterOfPuppetsWidget(Hosting& hosting)
    : _gamepadClient(hosting.getGamepadClient()), _masterOfPuppets(hosting.getMasterOfPuppets())
{
}

bool MasterOfPuppetsWidget::render(bool& showWindow)
{
    static bool isWindowLocked = false;
    static int masterIndex = -1;
    static ImVec2 cursor;
    static ImVec2 size;
    size = ImGui::GetWindowSize();

    AppStyle::pushTitle();

    ImGui::SetNextWindowSizeConstraints(ImVec2(450, 100), ImVec2(1300, 900));
    ImGui::Begin("Master of Puppets##Master of Puppets", &showWindow, isWindowLocked ? ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize : 0);
    if (!showWindow) Config::cfg.widgets.masterOfPuppets = showWindow;
    AppStyle::pushInput();


    if (_masterOfPuppets.isSDLEngine)
    {
        static Stopwatch& stopwatch = _masterOfPuppets.stopwatch;
        static float fill;
        
        fill = 1.0f - (float)stopwatch.getRemainingTime() / stopwatch.getDuration();
        
        if (IconButton::render(AppIcons::sdl, AppColors::primary, ImVec2(30, 30)))
        {
            _masterOfPuppets.isSDLEngine = !_masterOfPuppets.isSDLEngine;
        }

        TitleTooltipWidget::render("Master Gamepad Engine", _sdlTooltipString.c_str());

        static float yPos = ImGui::GetCursorPosY();

        ImGui::SameLine();

        cursor = ImGui::GetCursorPos();
        ProgressCircularWidget::render(15, 8, fill);
        
        ImGui::SetCursorPos(cursor);

        if (ImGui::Button("### SDL Joysticks Refresh", ImVec2(35, 35)))
        {
            static Debouncer debouncer = Debouncer(500, [&]() {
                _masterOfPuppets.fetchSDLGamepads();
                stopwatch.reset();
            });
            debouncer.start();
        }
        TitleTooltipWidget::render("Gamepad refresh", "Click to manually refresh the gamepad detection list.");

        ImGui::SetCursorPosY(yPos);
    }
    else
    {
        if (IconButton::render(AppIcons::windows, AppColors::primary, ImVec2(30, 30)))
        {
            _masterOfPuppets.isSDLEngine = !_masterOfPuppets.isSDLEngine;
            _masterOfPuppets.fetchSDLGamepads();
        }
        TitleTooltipWidget::render("Master Gamepad Engine", _wgiTooltipString.c_str());
    }

    //ImGui::SameLine();
    //ImGui::Dummy(ImVec2(5, 0));
    ImGui::SameLine();
    static bool showVPad = false;
    if (IconButton::render(AppIcons::vpad, showVPad ? AppColors::positive : AppColors::negative, ImVec2(30, 30)))
    {
        showVPad = !showVPad;
        //isWindowLocked = showVPad;
    }
    TitleTooltipWidget::render("Virtual Master Gamepad", "Click to toggle Master VPad.");

    //ImGui::SameLine();
    //ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 20);
    //if (IconButton::render(AppIcons::move, isWindowLocked ? AppColors::negative : AppColors::positive, ImVec2(30, 30)))
    //{
    //    isWindowLocked = !isWindowLocked;
    //}
    //if (isWindowLocked) TitleTooltipWidget::render("Window Locked", "This window cannot move or resize.");
    //else TitleTooltipWidget::render("Window Unlocked", "This window can move and resize.");

    //ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

    static XINPUT_GAMEPAD vpadState;
    static VirtualGamepadWidget vpad;
    _masterOfPuppets.setVirtualMaster(showVPad);
    if (showVPad)
    {
        vpadState = vpad.render(40);

        ImGui::Dummy(ImVec2(0, 5));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 5));

        _masterOfPuppets.setVirtualMasterState(vpadState);
    }

    _masterOfPuppets.inputMutex.lock();
    ImGui::BeginGroup();
    renderMaster();
    ImGui::SameLine();

    //if (_masterOfPuppets.isSDLEngine)
    //{
        ImGui::Dummy(ImVec2(50, 0));
        ImGui::SameLine();
    //}

    renderPuppets();
    ImGui::EndGroup();
    _masterOfPuppets.inputMutex.unlock();

    AppStyle::pop();
    AppStyle::pop();
    ImGui::End();

    return true;
}

void MasterOfPuppetsWidget::renderMaster()
{
    if (_masterOfPuppets.isSDLEngine)
        renderMasterSDL();
    else
        renderMasterXInput();
}

// ===============================================================
// Render engines
// ===============================================================
void MasterOfPuppetsWidget::renderMasterSDL()
{
    static vector<AnimatedGamepadWidget*> agws;
    vector<SDLGamepad>& sdlGamepads = _masterOfPuppets.getSDLGamepads();
    static ImVec2 cursor;

    if (_masterOfPuppets.getMasterIndex() >= sdlGamepads.size()) {
        _masterOfPuppets.setMasterIndex(-1);
    }
    ImGui::BeginGroup();

    AppFonts::pushSugoiDekai();
    ImGui::Text("Master");
    AppFonts::pop();

    ImGui::Dummy(ImVec2(200, 0));

    for (int i = 0; i < sdlGamepads.size(); ++i)
    {
        while (agws.size() <= i) {
            agws.push_back(new AnimatedGamepadWidget);
        }
        AnimatedGamepadWidget* agw = agws[i];
        static float shift1 = 25.0f, shift2 = 30.0f, shift3 = 5.0f;
        cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
        AppFonts::pushSugoiDekai();
        ImGui::Text("%d ", i + 1);
        AppFonts::pop();

        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(shift1);

        ImGui::PushID(i);

        static bool isMappingBtnPressed = false;
        switch (sdlGamepads[i].type)
        {
        case SDLGamepad::Type::XBox:
            isMappingBtnPressed = IconButton::render(AppIcons::xinput, AppColors::primary, ImVec2(20, 20));
            TitleTooltipWidget::render("Button Mapping", _sdlXInputMapTooltip.c_str());
            break;
        case SDLGamepad::Type::DS:
            isMappingBtnPressed = IconButton::render(AppIcons::dinput, AppColors::primary, ImVec2(20, 20));
            TitleTooltipWidget::render("Button Mapping", _sdlDualshockMapTooltip.c_str());
            break;
        case SDLGamepad::Type::DS4:
            isMappingBtnPressed = IconButton::render(AppIcons::ds4, AppColors::primary, ImVec2(20, 20));
            TitleTooltipWidget::render("Button Mapping", _sdlDS4MapTooltip.c_str());
            break;
        default:
            isMappingBtnPressed = false;
            break;
        }

        if (isMappingBtnPressed)
        {
            sdlGamepads[i].cycleType();
        }

        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(shift1 + shift2);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

        if (ImGui::Selectable(
            (
                string() + "### Master Gamepad " + to_string(i)
            ).c_str(),
            _masterOfPuppets.getMasterIndex() == i, 0, ImVec2(155, 20))
        )
        {
            setMaster(i);
        }

        TitleTooltipWidget::render(
            "Master Gamepad",
            (_masterOfPuppets.getMasterIndex() == i) ? _masterTooltipOn.c_str() : _masterTooltipOff.c_str()
        );

        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(shift1 + shift2 + shift3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

        XINPUT_GAMEPAD gg = sdlGamepads[i].getGamepadState().state.Gamepad;
        //AnimatedGamepadWidget::render(sdlGamepads[i].getGamepadState().state.Gamepad, 20);
        agw->render(sdlGamepads[i].getGamepadState().state.Gamepad, 20);
        ImGui::PopStyleVar();

        ImGui::PopID();
    }
    ImGui::EndGroup();
}

void MasterOfPuppetsWidget::renderMasterXInput()
{
    static vector<AnimatedGamepadWidget*> agws;
    vector<GamepadState>& xinputGamepads = _masterOfPuppets.getXInputGamepads();
    static ImVec2 cursor;

    if (_masterOfPuppets.getMasterIndex() >= xinputGamepads.size()) {
        _masterOfPuppets.setMasterIndex(-1);
    }
    ImGui::BeginGroup();

    AppFonts::pushSugoiDekai();
    ImGui::Text("Master");
    AppFonts::pop();

    ImGui::Dummy(ImVec2(200, 0));

    for (int i = 0; i < xinputGamepads.size(); ++i)
    {
        while (agws.size() <= i) {
            agws.push_back(new AnimatedGamepadWidget);
        }
        AnimatedGamepadWidget* agw = agws[i];

        static float shift1 = 25.0f, shift2 = 30.0f, shift3 = 5.0f;
        cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
        AppFonts::pushSugoiDekai();
        ImGui::Text("%d ", i + 1);
        AppFonts::pop();

        ImGui::PushID(i);
        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(shift1 + shift2);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
        if (ImGui::Selectable(
            (
                string() + "### Master Gamepad " + to_string(i)
                ).c_str(),
            _masterOfPuppets.getMasterIndex() == i, 0, ImVec2(155, 20))
            )
        {
            setMaster(i);
        }
        TitleTooltipWidget::render(
            "Master Gamepad",
            (_masterOfPuppets.getMasterIndex() == i) ? _masterTooltipOn.c_str() : _masterTooltipOff.c_str()
        );

        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(shift1 + shift2 + shift3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

        //AnimatedGamepadWidget::render(xinputGamepads[i].state.Gamepad, 20);
        agw->render(xinputGamepads[i].state.Gamepad, 20);
        ImGui::PopStyleVar();

        ImGui::PopID();
    }
    ImGui::EndGroup();
}


// ===============================================================
// UI Control
// ===============================================================
void MasterOfPuppetsWidget::setMaster(int value)
{
    _masterOfPuppets.setMasterIndex( (_masterOfPuppets.getMasterIndex() == value) ? -1 : value );
    _gamepadClient.isPuppetMaster = value >= 0;
    if (!_gamepadClient.isPuppetMaster)
    {
        clearPuppets();
    }
}

void MasterOfPuppetsWidget::clearPuppets()
{
    vector<AGamepad*>& gamepads = _gamepadClient.gamepads;

    for (size_t i = 0; i < gamepads.size(); ++i)
    {
        if (gamepads[i]->isPuppet)
        {
            gamepads[i]->clearState();
        }
    }
}

void MasterOfPuppetsWidget::renderPuppets()
{
    static vector<AnimatedGamepadWidget*> agws;
    vector<AGamepad*>& gamepads = _gamepadClient.gamepads;

    static ImVec2 cursor;

    ImGui::BeginGroup();

    AppFonts::pushSugoiDekai();
    ImGui::Text("Puppets");
    AppFonts::pop();

    ImGui::Dummy(ImVec2(0, 0));

    for (int i = 0; i < gamepads.size(); ++i)
    {
        while (agws.size() <= i) {
            agws.push_back(new AnimatedGamepadWidget);
        }
        AnimatedGamepadWidget* agw = agws[i];
        static float shift1 = 10.0f, shift2 = 10.0f, shift3 = 5.0f;
        cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
        AppFonts::pushSugoiDekai();
        ImGui::Text("%d ", i + 1);
        AppFonts::pop();

        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(cursor.x + shift1 + shift2);

        ImGui::PushID(i);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

        if (ImGui::Selectable(
            (
                string() + "### Puppet Gamepad " + to_string(i)
                ).c_str(),
            gamepads[i]->isPuppet, 0, ImVec2(155, 20))
            )
        {
            gamepads[i]->isPuppet = !gamepads[i]->isPuppet;
            if (!gamepads[i]->isPuppet)
            {
                gamepads[i]->clearState();
            }
        }
        TitleTooltipWidget::render("Puppets (Virtual Gamepads)", "Click to mark and control this Puppet.");

        ImGui::SetCursorPos(cursor);
        ImGui::SetCursorPosX(cursor.x + shift1 + shift2 + shift3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

        //AnimatedGamepadWidget::render(gamepads[i]->getState().Gamepad, 20);
        //static AnimatedGamepadWidget agw;
        agw->render(gamepads[i]->getState().Gamepad, 20);
        ImGui::PopStyleVar();

        ImGui::PopID();
    }
    ImGui::EndGroup();
}