#pragma once

//-- CodeSomnia Modified Start--
#include "parsec.h"
#include "./KeyboardMapsUtil.h"
#include "matoya.h"
#include <vector>
#include <map>


typedef vector<uint16_t> MappingKeyList_t;
typedef vector<KeyMapInfo> KeySet_t;

class KeyboardMap
{

public:
	static KeyboardMap defaultMap() {
		KeyboardMap keyMap = KeyboardMap();
		keyMap.loadKeyMaps();

		return keyMap;
	};

	//CodeSomnia : 각 키값을 게임패드 키 리스트에 추가.
	void SetToDefaultKeyInfo(GamePadButtonType_e keyType, uint16_t keyVal)
	{
		switch (keyType)
		{
		case GamePadButtonType_e::LLeft:
			LLeft.push_back(keyVal);
			break;
		case GamePadButtonType_e::LRight:
			LRight.push_back(keyVal);
			break;
		case GamePadButtonType_e::LUp:
			LUp.push_back(keyVal);
			break;
		case GamePadButtonType_e::LDown:
			LDown.push_back(keyVal);
			break;
		case GamePadButtonType_e::RLeft:
			RLeft.push_back(keyVal);
			break;
		case GamePadButtonType_e::RRight:
			RRight.push_back(keyVal);
			break;
		case GamePadButtonType_e::RUp:
			RUp.push_back(keyVal);
			break;
		case GamePadButtonType_e::RDown:
			RDown.push_back(keyVal);
			break;
		case GamePadButtonType_e::DLeft:
			DLeft.push_back(keyVal);
			break;
		case GamePadButtonType_e::DRight:
			DRight.push_back(keyVal);
			break;
		case GamePadButtonType_e::DUp:
			DUp.push_back(keyVal);
			break;
		case GamePadButtonType_e::DDown:
			DDown.push_back(keyVal);
			break;
		case GamePadButtonType_e::A:
			A.push_back(keyVal);
			break;
		case GamePadButtonType_e::B:
			B.push_back(keyVal);
			break;
		case GamePadButtonType_e::X:
			X.push_back(keyVal);
			break;
		case GamePadButtonType_e::Y:
			Y.push_back(keyVal);
			break;
		case GamePadButtonType_e::LB:
			LB.push_back(keyVal);
			break;
		case GamePadButtonType_e::RB:
			RB.push_back(keyVal);
			break;
		case GamePadButtonType_e::LT:
			LT.push_back(keyVal);
			break;
		case GamePadButtonType_e::RT:
			RT.push_back(keyVal);
			break;
		case GamePadButtonType_e::Back:
			Back.push_back(keyVal);
			break;
		case GamePadButtonType_e::Start:
			Start.push_back(keyVal);
			break;
		case GamePadButtonType_e::LThumb:
			LThumb.push_back(keyVal);
			break;
		case GamePadButtonType_e::RThumb:
			RThumb.push_back(keyVal);
			break;
		default:
			break;
		};
	};

	void EntryKeySet(string presetName, MappingKeyList_t vc, GamePadButtonType_e btnType, KeyType_e keyType)
	{
		auto rslt = _keyTable.find(presetName);

		if (rslt == _keyTable.end())
		{
			return;
		}


		for (auto itr = vc.begin(); itr != vc.end(); itr++)
		{
			KeyMapInfo info;
			info.gamePadKeyType = btnType;
			info.keyType = keyType;
			info.keyVal = *itr;

			rslt->second.push_back(info);
		}
	};

	void SetKeyTableDefaultKeyInfo(string presetName)
	{
		EntryKeySet(presetName, LLeft, GamePadButtonType_e::LLeft, KeyType_e::ORIGIN);
		EntryKeySet(presetName, LRight, GamePadButtonType_e::LRight, KeyType_e::ORIGIN);
		EntryKeySet(presetName, LUp, GamePadButtonType_e::LUp, KeyType_e::ORIGIN);
		EntryKeySet(presetName, LDown, GamePadButtonType_e::LDown, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RLeft, GamePadButtonType_e::RLeft, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RRight, GamePadButtonType_e::RRight, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RUp, GamePadButtonType_e::RUp, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RDown, GamePadButtonType_e::RDown, KeyType_e::ORIGIN);
		EntryKeySet(presetName, DLeft, GamePadButtonType_e::DLeft, KeyType_e::ORIGIN);
		EntryKeySet(presetName, DRight, GamePadButtonType_e::DRight, KeyType_e::ORIGIN);
		EntryKeySet(presetName, DUp, GamePadButtonType_e::DUp, KeyType_e::ORIGIN);
		EntryKeySet(presetName, DDown, GamePadButtonType_e::DDown, KeyType_e::ORIGIN);
		EntryKeySet(presetName, A, GamePadButtonType_e::A, KeyType_e::ORIGIN);
		EntryKeySet(presetName, B, GamePadButtonType_e::B, KeyType_e::ORIGIN);
		EntryKeySet(presetName, X, GamePadButtonType_e::X, KeyType_e::ORIGIN);
		EntryKeySet(presetName, Y, GamePadButtonType_e::Y, KeyType_e::ORIGIN);
		EntryKeySet(presetName, LB, GamePadButtonType_e::LB, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RB, GamePadButtonType_e::RB, KeyType_e::ORIGIN);
		EntryKeySet(presetName, LT, GamePadButtonType_e::LT, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RT, GamePadButtonType_e::RT, KeyType_e::ORIGIN);
		EntryKeySet(presetName, Back, GamePadButtonType_e::Back, KeyType_e::ORIGIN);
		EntryKeySet(presetName, Start, GamePadButtonType_e::Start, KeyType_e::ORIGIN);
		EntryKeySet(presetName, LThumb, GamePadButtonType_e::LThumb, KeyType_e::ORIGIN);
		EntryKeySet(presetName, RThumb, GamePadButtonType_e::RThumb, KeyType_e::ORIGIN);
	};

	//CodeSomnia : 저장된 파일이 없을 경우 기본 키로 초기화
	void SetInitialKey(string presetName) {
		ClearAllKeys();

		_curPresetName = presetName;

		SetToDefaultKeyInfo(GamePadButtonType_e::LLeft, KEY_A);
		SetToDefaultKeyInfo(GamePadButtonType_e::LRight, KEY_D);
		SetToDefaultKeyInfo(GamePadButtonType_e::LUp, KEY_W);
		SetToDefaultKeyInfo(GamePadButtonType_e::LDown, KEY_S);

		SetToDefaultKeyInfo(GamePadButtonType_e::RLeft, KEY_LEFT);
		SetToDefaultKeyInfo(GamePadButtonType_e::RRight, KEY_RIGHT);
		SetToDefaultKeyInfo(GamePadButtonType_e::RUp, KEY_UP);
		SetToDefaultKeyInfo(GamePadButtonType_e::RDown, KEY_DOWN);

		SetToDefaultKeyInfo(GamePadButtonType_e::DLeft, KEY_KP_4);
		SetToDefaultKeyInfo(GamePadButtonType_e::DRight, KEY_KP_6);
		SetToDefaultKeyInfo(GamePadButtonType_e::DUp, KEY_KP_8);
		SetToDefaultKeyInfo(GamePadButtonType_e::DDown, KEY_KP_5);

		SetToDefaultKeyInfo(GamePadButtonType_e::A, KEY_L);
		SetToDefaultKeyInfo(GamePadButtonType_e::B, KEY_O);
		SetToDefaultKeyInfo(GamePadButtonType_e::X, KEY_K);
		SetToDefaultKeyInfo(GamePadButtonType_e::Y, KEY_I);
		SetToDefaultKeyInfo(GamePadButtonType_e::LB, KEY_Q);
		SetToDefaultKeyInfo(GamePadButtonType_e::RB, KEY_P);
		SetToDefaultKeyInfo(GamePadButtonType_e::LT, KEY_F);
		SetToDefaultKeyInfo(GamePadButtonType_e::RT, KEY_J);

		SetToDefaultKeyInfo(GamePadButtonType_e::Back, KEY_BACKSPACE);
		SetToDefaultKeyInfo(GamePadButtonType_e::Start, KEY_ENTER);
		SetToDefaultKeyInfo(GamePadButtonType_e::LThumb, KEY_C);
		SetToDefaultKeyInfo(GamePadButtonType_e::RThumb, KEY_M);

		_keyTable[presetName] = KeySet_t();
		SetKeyTableDefaultKeyInfo(presetName);
	}

	//저장 파일로부터 키값을 Load
	void loadKeyMaps() {
		string dirPath = KeyboardMapsUtil::getKeyMapSavePath();
		bool needInitialSet = true;

		if (!dirPath.empty())
		{
			string filePath = dirPath + "ParsecSodaKeyMap.json";

			if (MTY_FileExists(filePath.c_str()))
			{
				MTY_JSON* json = MTY_JSONReadFile(filePath.c_str());
				char curPresetNameBuffer[PRESET_NAME_LEN];

				if (!MTY_JSONObjGetString(json, "CurrentPresetName", curPresetNameBuffer, PRESET_NAME_LEN))
				{
					_curPresetName = "";
				}

				_curPresetName = string(curPresetNameBuffer);

				const MTY_JSON* presetLst = MTY_JSONObjGetItem(json, "PresetList");
				uint32_t keyMapCount = MTY_JSONGetLength(presetLst);


				for (uint32_t idx = 0; idx < keyMapCount; idx++)
				{
					const MTY_JSON* presetItem = MTY_JSONArrayGetItem(presetLst, idx);

					string presetName;
					char presetNameBuffer[PRESET_NAME_LEN] = { 0, };
					vector<KeyMapInfo> keyLst;

					if (!MTY_JSONObjGetString(presetItem, "PresetName", presetNameBuffer, PRESET_NAME_LEN))
					{
						presetName = "UNKNOWN_PRESET_" + to_string(idx);
					}
					else
					{
						presetName = string(presetNameBuffer);
					}

					const MTY_JSON* keySets = MTY_JSONObjGetItem(presetItem, "KeySet");
					uint32_t keyCnt = MTY_JSONGetLength(keySets);

					for (uint32_t jdx = 0; jdx < keyCnt; jdx++)
					{
						const MTY_JSON* keyItem = MTY_JSONArrayGetItem(keySets, jdx);
						KeyMapInfo info;

						if (!MTY_JSONObjGetInt(keyItem, "GamePadKeyType", &info.gamePadKeyType)) {
							info.gamePadKeyType = GamePadButtonType_e::None;
						}

						if (!MTY_JSONObjGetUInt16(keyItem, "KeyValue", &info.keyVal)) {
							info.keyVal = 0;
						}

						if (!MTY_JSONObjGetInt(keyItem, "KeyType", &info.keyType)) {
							info.keyType = KeyType_e::NONE;
						}

						keyLst.push_back(info);
					}

					_keyTable[presetName] = keyLst;
				}

				LoadPreset(_curPresetName);
				needInitialSet = false;
			}
		}

		if (needInitialSet)
			SetInitialKey("Default");
	};

	// 각 키값들이 매칭되는지 확인을 위한 함수들.
	bool isLLeft(uint16_t val)
	{
		if (LLeft.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LLeft.begin(); itr < LLeft.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isLRight(uint16_t val) {

		if (LRight.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LRight.begin(); itr < LRight.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isLUp(uint16_t val) {

		if (LUp.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LUp.begin(); itr < LUp.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isLDown(uint16_t val) {

		if (LDown.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LDown.begin(); itr < LDown.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRLeft(uint16_t val) {

		if (RLeft.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RLeft.begin(); itr < RLeft.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRRight(uint16_t val) {

		if (RRight.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RRight.begin(); itr < RRight.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRUp(uint16_t val) {

		if (RUp.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RUp.begin(); itr < RUp.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRDown(uint16_t val) {

		if (RDown.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RDown.begin(); itr < RDown.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isDLeft(uint16_t val) {

		if (DLeft.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = DLeft.begin(); itr < DLeft.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isDRight(uint16_t val) {

		if (DRight.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = DRight.begin(); itr < DRight.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isDUp(uint16_t val) {

		if (DUp.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = DUp.begin(); itr < DUp.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isDDown(uint16_t val) {

		if (DDown.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = DDown.begin(); itr < DDown.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isA(uint16_t val) {

		if (A.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = A.begin(); itr < A.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isB(uint16_t val) {

		if (B.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = B.begin(); itr < B.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isX(uint16_t val) {

		if (X.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = X.begin(); itr < X.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isY(uint16_t val) {

		if (Y.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = Y.begin(); itr < Y.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isLB(uint16_t val) {

		if (LB.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LB.begin(); itr < LB.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRB(uint16_t val) {

		if (RB.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RB.begin(); itr < RB.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isLT(uint16_t val) {

		if (LT.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LT.begin(); itr < LT.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRT(uint16_t val) {

		if (RT.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RT.begin(); itr < RT.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isBack(uint16_t val) {

		if (Back.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = Back.begin(); itr < Back.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isStart(uint16_t val) {

		if (Start.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = Start.begin(); itr < Start.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isLThumb(uint16_t val) {

		if (LThumb.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = LThumb.begin(); itr < LThumb.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	bool isRThumb(uint16_t val) {

		if (RThumb.size() <= 0)
			return false;

		bool isConfirmed = false;

		for (MappingKeyList_t::iterator itr = RThumb.begin(); itr < RThumb.end(); itr++)
		{
			isConfirmed |= *itr == val;

			if (isConfirmed)
				break;
		}

		return isConfirmed;
	};

	inline uint16_t getDefaultLLeft() { return LLeft.size() > 0 ? LLeft[0] : KEY_A; };
	inline uint16_t getDefaultLRight() { return LRight.size() > 0 ? LRight[0] : KEY_D; };
	inline uint16_t getDefaultLDown() { return LDown.size() > 0 ? LDown[0] : KEY_W; };
	inline uint16_t getDefaultLUp() { return LUp.size() > 0 ? LUp[0] : KEY_S; };

	inline uint16_t getDefaultRLeft() { return RLeft.size() > 0 ? RLeft[0] : KEY_LEFT; };
	inline uint16_t getDefaultRRight() { return RRight.size() > 0 ? RRight[0] : KEY_RIGHT; };
	inline uint16_t getDefaultRUp() { return RUp.size() > 0 ? RUp[0] : KEY_UP; };
	inline uint16_t getDefaultRDown() { return RDown.size() > 0 ? RDown[0] : KEY_DOWN; };

	inline uint16_t getDefaultDLeft() { return DLeft.size() > 0 ? DLeft[0] : KEY_KP_4; };
	inline uint16_t getDefaultDRight() { return DRight.size() > 0 ? DRight[0] : KEY_KP_6; };
	inline uint16_t getDefaultDUp() { return DUp.size() > 0 ? DUp[0] : KEY_KP_8; };
	inline uint16_t getDefaultDDown() { return DDown.size() > 0 ? DDown[0] : KEY_KP_5; };

	inline uint16_t getDefaultA() { return A.size() > 0 ? A[0] : KEY_L; };
	inline uint16_t getDefaultB() { return B.size() > 0 ? B[0] : KEY_O; };
	inline uint16_t getDefaultX() { return X.size() > 0 ? X[0] : KEY_K; };
	inline uint16_t getDefaultY() { return Y.size() > 0 ? Y[0] : KEY_I; };

	inline uint16_t getDefaultLB() { return LB.size() > 0 ? LB[0] : KEY_Q; };
	inline uint16_t getDefaultRB() { return RB.size() > 0 ? RB[0] : KEY_P; };
	inline uint16_t getDefaultLT() { return LT.size() > 0 ? LT[0] : KEY_F; };
	inline uint16_t getDefaultRT() { return RT.size() > 0 ? RT[0] : KEY_J; };

	inline uint16_t getDefaultBack() { return Back.size() > 0 ? Back[0] : KEY_BACKSPACE; };
	inline uint16_t getDefaultStart() { return Start.size() > 0 ? Start[0] : KEY_ENTER; };
	inline uint16_t getDefaultLThumb() { return LThumb.size() > 0 ? LThumb[0] : KEY_C; };
	inline uint16_t getDefaultRThumb() { return RThumb.size() > 0 ? RThumb[0] : KEY_M; };


	void ClearAllKeys()
	{//게임패드에 매핑되어있는 모든 키정보를 클리어
		LLeft.clear();
		LRight.clear();
		LUp.clear();
		LDown.clear();

		RLeft.clear();
		RRight.clear();
		RUp.clear();
		RDown.clear();

		DLeft.clear();
		DRight.clear();
		DUp.clear();
		DDown.clear();

		A.clear();
		B.clear();
		X.clear();
		Y.clear();

		LB.clear();
		RB.clear();
		LT.clear();
		RT.clear();

		Back.clear();
		Start.clear();

		LThumb.clear();
		RThumb.clear();

	};

	// Left Stick
	MappingKeyList_t LLeft;
	MappingKeyList_t LRight;
	MappingKeyList_t LUp;
	MappingKeyList_t LDown;

	// Right Stick
	MappingKeyList_t RLeft;
	MappingKeyList_t RRight;
	MappingKeyList_t RUp;
	MappingKeyList_t RDown;

	// DPad
	MappingKeyList_t DLeft;
	MappingKeyList_t DRight;
	MappingKeyList_t DUp;
	MappingKeyList_t DDown;

	// Face Buttons
	MappingKeyList_t A;
	MappingKeyList_t B;
	MappingKeyList_t X;
	MappingKeyList_t Y;

	// Top
	MappingKeyList_t LB;
	MappingKeyList_t RB;
	MappingKeyList_t LT;
	MappingKeyList_t RT;

	// Menu
	MappingKeyList_t Back;
	MappingKeyList_t Start;

	// Stick Buttons
	MappingKeyList_t LThumb;
	MappingKeyList_t RThumb;

	vector<KeyMapInfo> _customKeyList;

	string _curPresetName;

	map<string, KeySet_t> _keyTable;
	//설정한 키매핑 정보를 모두 소유한 테이블.


	string AddNewPreset() {
		vector<KeyMapInfo> keySet;
		string presetName;
		int idx = 0;

		while (true)
		{
			presetName = "New Preset_" + to_string(_keyTable.size() + idx);
			auto item = _keyTable.find(presetName);

			if (item == _keyTable.end())
				break;
			idx++;
		}

		_keyTable[presetName] = keySet;
		_curPresetName = presetName;
		_customKeyList.clear();

		ClearAllKeys();
		SetInitialKey(presetName);
		return presetName;
	};

	bool UpdateCurrentStateToTable() {
		//현재 프리셋 정보를 키테이블에 입력합니다.
		auto rslt = _keyTable.find(_curPresetName);

		if (rslt == _keyTable.end())
		{
			return false;
		}

		rslt->second.clear();
		SetKeyTableDefaultKeyInfo(_curPresetName);

		for (auto itr = _customKeyList.begin(); itr != _customKeyList.end(); itr++)
		{
			rslt->second.push_back(*itr);
		}

	};

	bool RemoveCurPreset()
	{//현재 설정되어있는 프리셋을 삭제
		auto rslt = _keyTable.find(_curPresetName);

		if (rslt == _keyTable.end() || _keyTable.size() <= 1)
		{//최소한 한개의 프리셋은 존재해야하므로 해당 프리셋이 존재하지 않거나, 키맵테이블에 최소 한개이상 남아있을 수 있도록 예외처리
			return false;
		}

		rslt->second.clear();
		_keyTable.erase(rslt);

		auto firstItem = _keyTable.begin();

		LoadPreset(firstItem->first);

		return true;
	}

	bool isAvailablePresetName(string newPresetName)
	{
		//새로운 프리셋이름을 사용 할 수 있는지 확인.

		auto rslt = _keyTable.find(newPresetName);

		if (rslt == _keyTable.end())
		{
			return true;
		}

		return false;
	}

	bool UpdatePresetName(string newPresetName, string oldPresetName)
	{//프리셋 이름을 업데이트합니다.
		auto rslt = _keyTable.find(oldPresetName);

		if (rslt == _keyTable.end())
		{//이전 이름의 프리셋이 키테이블에 없는경우 업데이트하지 할수 없는 예외처리.
			return false;
		}

		vector<KeyMapInfo> keySet = _keyTable[oldPresetName];
		_keyTable.erase(rslt);

		_keyTable[newPresetName] = keySet;
		_curPresetName = newPresetName;

		return true;
	};

	void LoadPreset(string presetName)
	{ //프리셋 이름을 통해 해당이름의 프리셋 정보를 불러와 적용합니다.
		if (_keyTable.find(presetName) == _keyTable.end())
		{
			return;
		}

		ClearAllKeys();
		_customKeyList.clear();

		vector<KeyMapInfo> keySet = _keyTable[presetName];
		_curPresetName = presetName;

		for (auto itr = keySet.begin(); itr != keySet.end(); itr++)
		{
			SetToDefaultKeyInfo((GamePadButtonType_e)itr->gamePadKeyType, itr->keyVal);

			if (itr->keyType == KeyType_e::CUSTOM)
			{
				_customKeyList.push_back(*itr);
			}
		}
	};

	bool SaveKeyMap()
	{ //설정한 키매핑을 파일로 저장합니다
		string dirPath = KeyboardMapsUtil::getKeyMapSavePath();

		if (!dirPath.empty())
		{
			string filePath = dirPath + +"ParsecSodaKeyMap.json";
			MTY_JSON* json = MTY_JSONObjCreate();

			MTY_JSONObjSetString(json, "CurrentPresetName", _curPresetName.c_str());

			MTY_JSON* presetLst = MTY_JSONArrayCreate();

			for (auto itr = _keyTable.begin(); itr != _keyTable.end(); itr++)
			{
				MTY_JSON* presetInfo = MTY_JSONObjCreate();

				MTY_JSONObjSetString(presetInfo, "PresetName", itr->first.c_str());
				MTY_JSON* keyLst = MTY_JSONArrayCreate();

				for (auto keyItemItr = itr->second.begin(); keyItemItr != itr->second.end(); keyItemItr++)
				{
					MTY_JSON* keyInfo = MTY_JSONObjCreate();

					MTY_JSONObjSetInt(keyInfo, "GamePadKeyType", keyItemItr->gamePadKeyType);
					MTY_JSONObjSetInt(keyInfo, "KeyValue", keyItemItr->keyVal);
					MTY_JSONObjSetInt(keyInfo, "KeyType", keyItemItr->keyType);

					MTY_JSONArrayAppendItem(keyLst, keyInfo);
				}

				MTY_JSONObjSetItem(presetInfo, "KeySet", keyLst);
				MTY_JSONArrayAppendItem(presetLst, presetInfo);
			}
			//
			MTY_JSONObjSetItem(json, "PresetList", presetLst);

			MTY_JSONWriteFile(filePath.c_str(), json);
			MTY_JSONDestroy(&json);

			return true;

		}

		return false;

	};
};
//-- CodeSomnia Modified End--
