#pragma once
#include "KeyboardMapWidget.h"
#include <assert.h>


KeyboardMapWidget::KeyboardMapWidget(Hosting& hosting)
	: _hosting(hosting)
{
    // CodeSomnia : 객체 생성 및 초기화. 호스트에 등록되어있는 기본 키값으로 필수 키설정 변수를 초기화한다.
  

    //strcpy(_presetNameBuffer, hostingKeyMap._curPresetName.c_str());
    /*_presetNameBuffer = hostingKeyMap._curPresetName;*/
    InitializeKeyMapSetting(hosting);
}

inline void KeyboardMapWidget::resetPresetName(const char* str, int len)
{
    memset(_presetNameBuffer, 0, PRESET_NAME_LEN);
    memcpy(_presetNameBuffer, str, len);
}

void KeyboardMapWidget::InitializeKeyMapSetting(Hosting& hosting)
{
    KeyboardMap& hostingKeyMap = hosting.getGamepadClient().getKeyMap();

    _lLeft = hostingKeyMap.getDefaultLLeft();
    _lRight = hostingKeyMap.getDefaultLRight();
    _lUp = hostingKeyMap.getDefaultLUp();
    _lDown = hostingKeyMap.getDefaultLDown();

    _rLeft = hostingKeyMap.getDefaultRLeft();
    _rRight = hostingKeyMap.getDefaultRRight();
    _rUp = hostingKeyMap.getDefaultRUp();
    _rDown = hostingKeyMap.getDefaultRDown();

    _dLeft = hostingKeyMap.getDefaultDLeft();
    _dRight = hostingKeyMap.getDefaultDRight();
    _dUp = hostingKeyMap.getDefaultDUp();
    _dDown = hostingKeyMap.getDefaultDDown();

    _a = hostingKeyMap.getDefaultA();
    _b = hostingKeyMap.getDefaultB();
    _x = hostingKeyMap.getDefaultX();
    _y = hostingKeyMap.getDefaultY();

    _lB = hostingKeyMap.getDefaultLB();
    _rB = hostingKeyMap.getDefaultRB();
    _lT = hostingKeyMap.getDefaultLT();
    _rT = hostingKeyMap.getDefaultRT();

    _back = hostingKeyMap.getDefaultBack();
    _start = hostingKeyMap.getDefaultStart();

    _lThumb = hostingKeyMap.getDefaultLThumb();
    _rThumb = hostingKeyMap.getDefaultRThumb();

    memcpy(_presetNameBuffer, hostingKeyMap._curPresetName.c_str(), hostingKeyMap._curPresetName.length());
}

void KeyboardMapWidget::ApplyKeyMap()
{
    // CodeSomnia : 설정한 키맵을 Hosting에서 적용 될수 있도록 한다
    //        
    KeyboardMap& hostingKeyMap = this->_hosting.getGamepadClient().getKeyMap();
    hostingKeyMap.ClearAllKeys();

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LLeft , _lLeft);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LRight, _lRight);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LUp, _lUp);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LDown, _lDown);

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RLeft, _rLeft);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RRight, _rRight);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RUp, _rUp);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RDown, _rDown);

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::DLeft, _dLeft);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::DRight, _dRight);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::DUp, _dUp);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::DDown, _dDown);

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::A, _a);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::B, _b);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::X, _x);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::Y, _y);

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LB, _lB);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RB, _rB);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LT, _lT);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RT, _rT);

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::Back, _back);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::Start, _start);

    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::LThumb, _lThumb);
    hostingKeyMap.SetToDefaultKeyInfo(GamePadButtonType_e::RThumb, _rThumb);

    hostingKeyMap.UpdateCurrentStateToTable();

    for (auto itr = hostingKeyMap._customKeyList.begin();
        itr < hostingKeyMap._customKeyList.end(); itr++)
    {
        hostingKeyMap.SetToDefaultKeyInfo((GamePadButtonType_e)itr->gamePadKeyType, itr->keyVal);
    }

}
void KeyboardMapWidget::AddJsonKeyMapItem(MTY_JSON* json, GamePadButtonType_e gamePadKeyType, uint16_t keyVal, int32_t keyType)
{
    MTY_JSON* item = MTY_JSONObjCreate();
    MTY_JSONObjSetUInt(item, "GamePadKeyType", gamePadKeyType);
    MTY_JSONObjSetUInt(item, "KeyValue", keyVal);
    MTY_JSONObjSetUInt(item, "KeyType", keyType);

    MTY_JSONArrayAppendItem(json,item);
}

bool KeyboardMapWidget::SaveKeyFile()
{
    return _hosting.getGamepadClient().getKeyMap().SaveKeyMap();
}

/*
    CodeSomnia : 커스텀키 UI를 드로잉 커스텀 키를 설정하기 위한 컨트롤을 한라인씩 생성하는 함수
*/ 
bool KeyboardMapWidget::drawCustomKey(int32_t& gamePadKeyType, uint16_t& key,int idx) {
    
    AppFonts::pushInput();
    bool isNeedDelete = false;
    int allowedKeyLen = sizeof(_allowedSettionKeys) / sizeof(uint16_t);
    
    string keyLabelName = KeyboardMapsUtil::getGamePadKeyTypeToString((GamePadButtonType_e)gamePadKeyType);
    string labelName = "##" + keyLabelName +"_Custom_KeyName"+ to_string(idx);
    
    ImGui::SetNextItemWidth(COLUMN_1_SIZE+13); 
    // 첫번째 열의 기본사이즈를 적용하기 위해 그다음에 오는 콤보 박스의 사이즈를 결정
    
    if (ImGui::BeginCombo(labelName.c_str(), keyLabelName.c_str()))
    {
        for (int idx = 0; idx < GAMEPAD_KEY_COUNT; idx++)
        {
            bool isSelected = keyLabelName == _allowedCustomKeys[idx].name;

            if (ImGui::Selectable(_allowedCustomKeys[idx].name.c_str(), isSelected))
            {
                //콤보 박스가 Expand 될때 표출되는 항목드를 Drawing한다.
                //만약 항목이 선택되면 기본 콤보박스에 선택된 항목이 대표적으로 표출 될수 있도록한다.
                keyLabelName = _allowedCustomKeys[idx].name;
                gamePadKeyType = _allowedCustomKeys[idx].buttonType;
            }

            if (isSelected)
            {
                //선택된 항목에 기본 포커스를 설정한다.
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
        //콤보박스 그리기를 끝낸다
    }

    ImGui::SameLine();
    // 게임키패드 종류 콤보박스와 키보드 매핑키 설정 콤보박스를 같은 라인에 드로잉 할 수 있도록 한다.
   
    string curKeyName = KeyboardMapsUtil::getKeyToString(key);
    string keyValName = "##" + keyLabelName + "_Custom_Val" + to_string(idx);
    ImGui::SetNextItemWidth(150);

    if (ImGui::BeginCombo(keyValName.c_str(), curKeyName.c_str()))
    {
        //설정할수 있는 키보드들을 콤보박스에 추가한다. 기본적인 로직은 게임키패드 콤보박스 그리기와 같다.
        for (int idx = 0; idx < allowedKeyLen; idx++)
        {
            bool isSelected = key == _allowedSettionKeys[idx];

            string itemKeyName = KeyboardMapsUtil::getKeyToString(_allowedSettionKeys[idx]);

            if (ImGui::Selectable(itemKeyName.c_str(), isSelected))
            {
                curKeyName = itemKeyName;
                key = _allowedSettionKeys[idx];
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    string btnLabel = "##Remove_CustomKey" + std::to_string(idx);
    //커스텀 키 항목을 삭제 할 수 있도록 버튼을 추가한다.
    if (IconButton::renderWithLabel(btnLabel.c_str(), AppIcons::trash, AppColors::primary,ImVec2(20.0,20.0))) {
        isNeedDelete = true;
    }

    AppFonts::pop();

    return isNeedDelete;

}

bool KeyboardMapWidget::render(bool &showWindow)
{
    KeyboardMap& hostingKeyMap = _hosting.getGamepadClient().getKeyMap();
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 50), ImVec2(3840, 2160));
    ImGui::Begin("Keyboard Mapper", &showWindow);
    if (!showWindow) Config::cfg.widgets.keyMapper = showWindow;
    AppStyle::pushLabel();

    static ImVec2 size;
    size = ImGui::GetContentRegionAvail();
    drawPresetList(hostingKeyMap._curPresetName);

    ImGui::Dummy(ImVec2(0, 10));

    lLeftSetRender();
    lRightSetRender();
    lUpSetRender();
    lDownSetRender();

    RLeftSetRender();
    RRightSetRender();
    RUpSetRender();
    RDownSetRender();

    DLeftSetRender();
    DRightSetRender();
    DUpSetRender();
    DDownSetRender();

    ASetRender();
    BSetRender();
    XSetRender();
    YSetRender();

    LBSetRender();
    RBSetRender();
    LTSetRender();
    RTSetRender();

    BackSetRender();
    StartBSetRender();

    LThumbSetRender();
    RThumbSetRender();

    ImGui::Dummy(ImVec2(0, 10));

    AppFonts::pushInput();
    AppColors::pushButtonSolid();
    if (ImGui::Button("Reset to Default")) {
        hostingKeyMap.resetProfile(hostingKeyMap._curPresetID);
    }
    AppColors::popButton();
    AppFonts::pop();

    // Hiding custom key stuff for now because makes things too complicated
    //int totalSize = hostingKeyMap._customKeyList.size();
    ////추가된 커스텀키 항목을 설정할 수 있도록 그린다.
    //for (int idx = 0; idx < hostingKeyMap._customKeyList.size();)
    //{
    //    if (drawCustomKey(hostingKeyMap._customKeyList[idx].gamePadKeyType,
    //        hostingKeyMap._customKeyList[idx].keyVal, idx))
    //    {
    //        auto it = hostingKeyMap._customKeyList.begin();
    //        std::advance(it, idx);
    //        hostingKeyMap._customKeyList.erase(it);
    //    }
    //    else
    //        idx++;
    //}

    //// 파일을 저장하고 키매핑 설정이 적용 될 수 있도록 버튼을 추가한다
    //if (ImGui::Button("Apply", ImVec2(100, 50))) {
    //    ApplyKeyMap();
    //    SaveKeyFile();
    //}

    //ImGui::SameLine();

    //// 커스텀키를 추가 할 수 있는 버튼을 추가한다.
    //if (ImGui::Button("Add Custom Key", ImVec2(150, 50))) {
    //    KeyMapInfo newItem;
    //    newItem.gamePadKeyType = GamePadButtonType_e::None;
    //    newItem.keyVal = 0;
    //    newItem.keyType = KeyType_e::CUSTOM;
    //    hostingKeyMap._customKeyList.push_back(newItem);
    //}
  
    //// 기본 게임 키보드 매핑을 할 수 있도록 UI를 그린다.

    AppStyle::pop();

    ImGui::End();
    AppStyle::pop();

	return true;
}

string KeyboardMapWidget::drawPresetList(string currentPresetName)
{
    string selectedPresetName = currentPresetName;
    KeyboardMap& hostingKeyMap = _hosting.getGamepadClient().getKeyMap();
    ImGui::SetNextItemWidth(300);

    if (ImGui::BeginCombo("##PresetListCombo", selectedPresetName.c_str())) {

        for (auto& profile : hostingKeyMap.profiles) {
            string presetName = profile.name;
            bool isSelected = selectedPresetName == presetName;

            if (ImGui::Selectable((std::to_string(profile.userID) + ":" + presetName).c_str(), isSelected)) {
                hostingKeyMap._curPresetName = presetName;
                hostingKeyMap._curPresetID = profile.userID;
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}

        ImGui::EndCombo();
    }

    // Profiles are now created by the user, so the Add button is no longer needed.
    /*if (ImGui::Button("Add", ImVec2(30, 25))) {
        ApplyKeyMap();
        selectedPresetName = hostingKeyMap.AddNewPreset();
        InitializeKeyMapSetting(_hosting);
    }*/

    //int limitLen = PRESET_NAME_LEN - selectedPresetName.length();
    //if(ImGui::InputText(" ", _presetNameBuffer, PRESET_NAME_LEN, ImGuiInputTextFlags_EnterReturnsTrue))
    //{
    //    string newName = string(_presetNameBuffer); // 입력받은 문자에 개행 문자 혹은 쓰레기값이 들어갈수 있어 문자열로 변환하며 클리어 시켜준다
    //    KeyboardMapsUtil::trim(newName); //공백을 제거한다.
    //    UpdatePresetName(newName, currentPresetName);
    //}

    if (hostingKeyMap._curPresetID != 0) {
        ImGui::SameLine();
        if (IconButton::render(AppIcons::trash, AppColors::primary, ImVec2(20.0, 20.0))) {
            selectedPresetName = "Default";
            uint32_t curPresetID = hostingKeyMap._curPresetID;
            hostingKeyMap._curPresetID = 0;
            hostingKeyMap._curPresetName = selectedPresetName;
            hostingKeyMap.deleteProfile(curPresetID);
        }
    }

    return selectedPresetName;
}


void KeyboardMapWidget::UpdatePresetName(string newName, string oldName) {
    //PresetName을 변경한다 하지만 기존에 같은 이름을 갖고 있거나, 빈공백으로만 이뤄진 이름은 허용하지 않는다
    KeyboardMap& hostingKeyMap = _hosting.getGamepadClient().getKeyMap();

    if (newName.empty())
        return;

    if (hostingKeyMap.isAvailablePresetName(newName))
    {
        hostingKeyMap.UpdatePresetName(newName, oldName);

        resetPresetName(newName.c_str(), newName.length());
        //적용된 이름이므로 새로운 이름으로 원복한다
    }
    else
    {
        resetPresetName(oldName.c_str(), oldName.length());
        //이용할수 없는 이름이므로 이전 이름으로 원복한다
    }

}
void KeyboardMapWidget::drawKeySetLine(string keyName, uint16_t& key)
{
    static ImVec2 cursor;

    AppFonts::pushInput();
    ImVec2 textSize = ImGui::CalcTextSize(keyName.c_str());
    ImGui::Text(keyName.c_str());
    ImGui::SameLine();

    cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPosX(textSize.x + (COLUMN_1_SIZE - textSize.x) + 25);

    KeyboardMap& hostingKeyMap = _hosting.getGamepadClient().getKeyMap();
    string curKeyName = KeyboardMapsUtil::getKeyToString(hostingKeyMap.getKeyForButton(keyName));
    int allowedKeyLen = sizeof(_allowedSettionKeys) / sizeof(uint16_t);
    string labelName = "##" + keyName;
    ImGui::SetNextItemWidth(150);

    if (ImGui::BeginCombo(labelName.c_str(), curKeyName.c_str()))
    {
        for (int idx = 0; idx < allowedKeyLen; idx++)
        {
            //bool isSelected = hostingKeyMap.isKeySetForButton(keyName, key);

            string itemKeyName = KeyboardMapsUtil::getKeyToString(_allowedSettionKeys[idx]);

            if (ImGui::Selectable(itemKeyName.c_str(), false))
            {
                curKeyName = itemKeyName;
                hostingKeyMap.setButton(keyName, _allowedSettionKeys[idx]);
                hostingKeyMap.saveProfiles();
            }

            /*if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }*/
        }
        ImGui::EndCombo();
    }

    AppFonts::pop();
}

void KeyboardMapWidget::lLeftSetRender() {
    drawKeySetLine("LLeft", _lLeft);
}

void KeyboardMapWidget::lRightSetRender() {
    drawKeySetLine("LRight", _lRight);
}

void KeyboardMapWidget::lUpSetRender() {
    drawKeySetLine("LUp", _lUp);

}

void KeyboardMapWidget::lDownSetRender() {
    drawKeySetLine("LDown", _lDown);

}

void KeyboardMapWidget::RLeftSetRender() {
    drawKeySetLine("RLeft", _rLeft);

}

void KeyboardMapWidget::RRightSetRender() {
    drawKeySetLine("RRight", _rRight);

}

void KeyboardMapWidget::RUpSetRender() {
    drawKeySetLine("RUp", _rUp);
}

void KeyboardMapWidget::RDownSetRender() {
    drawKeySetLine("RDown", _rDown);
}

void KeyboardMapWidget::DLeftSetRender() {
    drawKeySetLine("DLeft", _dLeft);
}

void KeyboardMapWidget::DRightSetRender() {
    drawKeySetLine("DRight", _dRight);
}

void KeyboardMapWidget::DUpSetRender() {
    drawKeySetLine("DUp", _dUp);
}

void KeyboardMapWidget::DDownSetRender() {
    drawKeySetLine("DDown", _dDown);
}

void KeyboardMapWidget::ASetRender() {
    drawKeySetLine("A", _a);
}

void KeyboardMapWidget::BSetRender() {
    drawKeySetLine("B", _b);
}

void KeyboardMapWidget::XSetRender() {
    drawKeySetLine("X", _x);
}

void KeyboardMapWidget::YSetRender() {
    drawKeySetLine("Y", _y);
}

void KeyboardMapWidget::LBSetRender() {
    drawKeySetLine("LB", _lB);
}

void KeyboardMapWidget::RBSetRender() {
    drawKeySetLine("RB", _rB);
}

void KeyboardMapWidget::LTSetRender() {
    drawKeySetLine("LT", _lT);
}

void KeyboardMapWidget::RTSetRender() {
    drawKeySetLine("RT", _rT);
}

void KeyboardMapWidget::BackSetRender() {
    drawKeySetLine("Back", _back);
}

void KeyboardMapWidget::StartBSetRender() {
    drawKeySetLine("Start", _start);
}

void KeyboardMapWidget::LThumbSetRender() {
    drawKeySetLine("LThumb", _lThumb);
}

void KeyboardMapWidget::RThumbSetRender() {
    drawKeySetLine("RThumb", _rThumb);
}
