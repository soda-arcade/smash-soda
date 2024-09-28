#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "parsec-dso.h"
#include "../ParsecSession.h"
#include "../Helpers/Stringer.h"
#include "../GamepadClient.h"
#include "../DX11.h"
#include "../AudioIn.h"
#include "../AudioOut.h"
#include "../Core/Cache.h"
#include "../CompilerDirectives.h"
#include "../MetadataCache.h"
#include "../Modules/Macro.h"
#include "../Modules/Hotseat.h"
#include "../Modules/Tournament.h"

#include "Commands/Base/ACommand.h"
#include "Commands/CommandBotMessage.h"
#include "Commands/CommandDefaultMessage.h"

class ChatBotCustom
{
public:
	ChatBotCustom(
		AudioIn& audioIn, AudioOut& audioOut, DX11& dx11,
		GamepadClient& gamepadClient, GuestList& guests, GuestDataList& guestHistory, ParsecDSO* parsec, ParsecHostConfig& hostConfig,
		ParsecSession& parsecSession, Macro& macro, bool& hostingLoopController, Guest& host,
		Hotseat& hotseat, Tournament& tournament
	)
		: _audioIn(audioIn), _audioOut(audioOut), _dx11(dx11),
		_gamepadClient(gamepadClient), _guests(guests), _guestHistory(guestHistory), _parsec(parsec), _hostConfig(hostConfig),
		_parsecSession(parsecSession), _macro(macro), _hostingLoopController(hostingLoopController), _host(host),
		_hotseat(hotseat), _tournament(tournament)
	{
		//_basicVersion = Config::cfg.general.basicVersion;
	}
	ACommand* isCustomCommand(const char* msg, Guest& sender, bool isHost, Tier tier, uint32_t previous);
private:
	// Dependency Injection
	ParsecDSO* _parsec;
	AudioIn& _audioIn;
	AudioOut& _audioOut;
	DX11& _dx11;
	GamepadClient& _gamepadClient;
	GuestList& _guests;
	GuestDataList& _guestHistory;
	ParsecHostConfig& _hostConfig;
	ParsecSession& _parsecSession;
	Macro& _macro;
	Hotseat& _hotseat;
	Tournament& _tournament;
	bool& _hostingLoopController;
	Guest& _host;

	bool msgStartsWith(const char* msg, const char* pattern);
	bool msgStartsWith(const char* msg, vector<const char*> patterns);
	bool msgIsEqual(const char* msg, const char* pattern);
	bool msgIsEqual(const char* msg, vector<const char*> patterns);
};