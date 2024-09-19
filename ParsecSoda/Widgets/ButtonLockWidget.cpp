#include "ButtonLockWidget.h"

ButtonLockWidget::ButtonLockWidget(Hosting& hosting)
    : _hosting(hosting)
{
    lockedGamepad.bLeftTrigger = Config::cfg.input.lockedGamepadLeftTrigger;
    lockedGamepad.bRightTrigger = Config::cfg.input.lockedGamepadRightTrigger;
    lockedGamepad.sThumbLX = Config::cfg.input.lockedGamepadLX;
    lockedGamepad.sThumbLY = Config::cfg.input.lockedGamepadLY;
    lockedGamepad.sThumbRX = Config::cfg.input.lockedGamepadRX;
    lockedGamepad.sThumbRY = Config::cfg.input.lockedGamepadRY;
    lockedGamepad.wButtons = Config::cfg.input.lockedGamepadButtons;
}

void ButtonLockWidget::setColor(float hue) {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.8f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 1.0f, 0.5f));
}

void ButtonLockWidget::makeButton(string buttonText, ImVec2 size, uint16_t bitwise) {
    if (lockedGamepad.wButtons & bitwise) setColor(0.0f);
    else setColor(0.35f);
    if (ImGui::Button(buttonText.c_str(), size)) {
        if (lockedGamepad.wButtons & bitwise) {
            lockedGamepad.wButtons &= ~bitwise;
        }
        else {
            lockedGamepad.wButtons |= bitwise;
        }
        _hosting.updateButtonLock(lockedGamepad);
    }
    ImGui::PopStyleColor(3);
}

void ButtonLockWidget::makeButton(string buttonText, ImVec2 size, bool& boolToChange) {
    if (boolToChange) setColor(0.0f);
    else setColor(0.35f);
    if (ImGui::Button(buttonText.c_str(), size)) {
        boolToChange = !boolToChange;
        _hosting.updateButtonLock(lockedGamepad);
    }
    ImGui::PopStyleColor(3);
}

void ButtonLockWidget::makeButton(string buttonText, ImVec2 size, bool& boolToChange1, bool& boolToChange2) {
    if (boolToChange1) setColor(0.0f);
    else setColor(0.35f);
    if (ImGui::Button(buttonText.c_str(), size)) {
        boolToChange1 = !boolToChange1;
        boolToChange2 = !boolToChange2;
        _hosting.updateButtonLock(lockedGamepad);
    }
    ImGui::PopStyleColor(3);
}

bool ButtonLockWidget::render(bool& showWindow)
{
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 240), ImVec2(400, 240));
    ImGui::Begin("Button Lock", &showWindow);
    if (!showWindow) Config::cfg.widgets.buttonLock = showWindow;
    AppStyle::pop();

    AppStyle::pushInput();

    ImGui::SetCursorPos(ImVec2(25, 50));
    makeButton("LT", wideButton, lockedGamepad.bLeftTrigger);
    ImGui::SetCursorPos(ImVec2(25, 80));
    makeButton("LB", wideButton, GAMEPAD_STATE_LEFT_SHOULDER);

    ImGui::SetCursorPos(ImVec2(310, 50));
    makeButton("RT", wideButton, lockedGamepad.bRightTrigger);
    ImGui::SetCursorPos(ImVec2(310, 80));
    makeButton("RB", wideButton, GAMEPAD_STATE_RIGHT_SHOULDER);

    ImGui::SetCursorPos(ImVec2(135, 50));
    makeButton("Guide Button", ultrawideButton, GAMEPAD_STATE_GUIDE);
    TitleTooltipWidget::render("Guide button", "Can be permanently disabled in Settings");
    ImGui::SetCursorPos(ImVec2(135, 80));
    makeButton("Back", wideButton, GAMEPAD_STATE_BACK);
    ImGui::SetCursorPos(ImVec2(200, 80));
    makeButton("Start", wideButton, GAMEPAD_STATE_START);

    ImGui::SetCursorPos(ImVec2(135, 130));
    makeButton("Left\nAnalog", bigButton, lockedGamepad.sThumbLX, lockedGamepad.sThumbLY);
    ImGui::SetCursorPos(ImVec2(152, 190));
    makeButton("LS", smallButton, GAMEPAD_STATE_LEFT_THUMB);

    ImGui::SetCursorPos(ImVec2(200, 130));
    makeButton("Right\nAnalog", bigButton, lockedGamepad.sThumbRX, lockedGamepad.sThumbRY);
    ImGui::SetCursorPos(ImVec2(217, 190));
    makeButton("RS", smallButton, GAMEPAD_STATE_RIGHT_THUMB);

    ImGui::SetCursorPos(ImVec2(55, 130));
    makeButton("^", smallButton, GAMEPAD_STATE_DPAD_UP);
    ImGui::SetCursorPos(ImVec2(55, 190));
    makeButton("v", smallButton, GAMEPAD_STATE_DPAD_DOWN);
    ImGui::SetCursorPos(ImVec2(25, 160));
    makeButton("<", smallButton, GAMEPAD_STATE_DPAD_LEFT);
    ImGui::SetCursorPos(ImVec2(85, 160));
    makeButton(">", smallButton, GAMEPAD_STATE_DPAD_RIGHT);

    ImGui::SetCursorPos(ImVec2(315, 190));
    makeButton("A", smallButton, GAMEPAD_STATE_A);
    ImGui::SetCursorPos(ImVec2(345, 160));
    makeButton("B", smallButton, GAMEPAD_STATE_B);
    ImGui::SetCursorPos(ImVec2(285, 160));
    makeButton("X", smallButton, GAMEPAD_STATE_X);
    ImGui::SetCursorPos(ImVec2(315, 130));
    makeButton("Y", smallButton, GAMEPAD_STATE_Y);

    //ImGui::PopStyleColor();

    AppStyle::pop();

    ImGui::End();
    return true;
}
