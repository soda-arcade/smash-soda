#pragma once

#define _WINSOCKAPI_
#define D3D_DEBUG_INFO

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <thread>
#include <chrono>
#include <mutex>
#include "parsec-dso.h"
#include "ParsecSession.h"
#include "DX11.h"
#include "matoya.h"
#include "TierList.h"
#include "ChatBot.h"
#include "ChatLog.h"
#include "Helpers/Stringer.h"
#include "AudioIn.h"
#include "AudioOut.h"
#include "AudioMix.h"
#include "GamepadClient.h"
#include "BanList.h"
#include "ModList.h"
#include "VIPList.h"
#include "GuestList.h"
#include "GameDataList.h"
#include "SFXList.h"
#include "MetadataCache.h"
#include "CompilerDirectives.h"
#include "Helpers/Stopwatch.h"
#include "MasterOfPuppets.h"
#include "WebSocket.h"
#include "ButtonLock.h"
#include "Helpers/Debouncer.h"
#include "Modules/Tournament.h"
#include "Modules/Macro.h"
#include "Modules/Hotseat.h"

#define PARSEC_APP_CHAT_MSG 0
#define HOSTING_CHAT_MSG_ID 0

#define ROOM_NAME "Coding my own Parsec\nGamepad streaming\0"
#define ROOM_SECRET "melonsod"

using namespace std;

class Hosting
{
public:
	Hosting();
	void applyHostConfig();
	void broadcastChatMessage(string message);
	void init();
	void release();
	bool isReady();
	bool isRunning();
	bool isLatencyRunning();
	bool isGamepadRunning();
	bool& isGamepadLock();
	bool& isGamepadLockButtons();
	Guest& getHost();
	ParsecSession& getSession();
	void fetchAccountData(bool sync = false);
	ParsecHostConfig& getHostConfig();
	DX11& getDX11();
	ChatBot* getChatBot();
	vector<string>& getMessageLog();
	vector<string>& getCommandLog();
	vector<Guest>& getGuestList();
	vector<GuestData>& getGuestHistory();
	MyMetrics getMetrics(uint32_t id);
	BanList& getBanList();
	ModList& getModList();
	VIPList& getVIPList();
	GameDataList& getGameList();
	vector<AGamepad*>& getGamepads();
	GamepadClient& getGamepadClient();
	MasterOfPuppets& getMasterOfPuppets();
	Macro& getMacro();
	const char** getGuestNames();
	void toggleGamepadLock();
	void toggleGamepadLockButtons();
	void setGameID(string gameID);
	void setMaxGuests(uint8_t maxGuests);
	void setHostConfig(string roomName, string gameId, uint8_t roomSlots, bool isPublicRoom);
	void setHostConfig(string roomName, string gameId, uint8_t roomSlots, bool isPublicRoom, string secret);
	void setHostVideoConfig(uint32_t fps, uint32_t bandwidth);
	void setPublicRoom(bool isPublicRoom);
	void setRoomName(string roomName);
	void setRoomSecret(string secret);
	void startHosting();
	void stopHosting();
	void stripGamepad(int index);
	void setOwner(AGamepad& gamepad, Guest newOwner, int padId);
	WebSocket& getWebSocket();
	void webSocketStart(string uri, string password);
	void webSocketRun(string uri, string password);
	void webSocketStop();
	bool webSocketRunning();

	bool removeGame(string name);

	LockedGamepadState _lockedGamepad;
	void updateButtonLock(LockedGamepadState lockedGamepad);

	int _lastMessageIndex;
	vector<string> _lastMessageList;
	Stopwatch _lastMessageTimer;

	void handleMessage(const char* message, Guest& guest, bool isHost = false, bool isHidden = false, bool outside = false);
	bool handleMuting(const char* message, Guest& guest);
	void sendHostMessage(const char* message, bool isHidden = false);

	AudioIn audioIn;
	AudioOut audioOut;
	HWND mainWindow;
	bool _latencyLimitEnabled = false;
	unsigned int _latencyLimitValue = 0;
	bool _disableMicrophone = false;
	bool _disableGuideButton = false;
	bool _disableKeyboard = false;

private:
	void initAllModules();
	void submitSilence();
	void liveStreamMedia();
	void mainLoopControl();
	void pollEvents();
	void pollInputs();
	void pollLatency();
	void pollSmashSoda();	// Custom features thread
	void pollGamepad();
	bool parsecArcadeStart();
	bool isFilteredCommand(ACommand* command);
	void onGuestStateChange(ParsecGuestState& state, Guest& guest, ParsecStatus& status);

	void kioskMode();
	void welcomeMessage();

	bool isSpectator(int index);

	// Attributes
	AudioMix _audioMix;
	DX11 _dx11;
	BanList _banList;
	ModList _modList;
	VIPList _vipList;
	GuestDataList _guestHistory;
	GameDataList _gamesList;
	ChatBot *_chatBot;
	ChatLog _chatLog;
	GamepadClient _gamepadClient;
	GuestList _guestList;
	MasterOfPuppets _masterOfPuppets;
	WebSocket _webSocket;

	ParsecDSO* _parsec;
	ParsecHostConfig _hostConfig;
	ParsecSession _parsecSession;
	ParsecStatus _parsecStatus;
	Guest _host;
	SFXList _sfxList;
	TierList _tierList;
	Tournament _tournament;
	Macro _macro;
	Hotseat _hotseat;

	bool _isRunning = false;
	bool _isMediaThreadRunning = false;
	bool _isInputThreadRunning = false;
	bool _isEventThreadRunning = false;
	bool _isLatencyThreadRunning = false;
	bool _isSmashSodaThreadRunning = false;
	bool _isGamepadThreadRunning = false;
	bool _isWebSocketThreadRunning = false;

	Stopwatch _mediaClock;
	Stopwatch _welcomeClock;

	string _welcomeGuest;
	bool _showWelcome = false;

	thread _mainLoopControlThread;
	thread _mediaThread;
	thread _inputThread;
	thread _eventThread;
	thread _latencyThread;
	thread _smashSodaThread;
	thread _gamepadThread;
	thread _webSocketThread;
	thread _createGamepadsThread;
	thread _connectGamepadsThread;

	mutex _mediaMutex;
	mutex _inputMutex;
	mutex _eventMutex;
	mutex _latencyMutex;
	mutex _smashSodaMutex;
	mutex _gamepadMutex;
	mutex _webSocketMutex;
};
