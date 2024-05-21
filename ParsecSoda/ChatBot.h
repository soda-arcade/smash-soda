#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "parsec-dso.h"
#include "ParsecSession.h"
#include "Helpers/Stringer.h"
#include "GamepadClient.h"
#include "DX11.h"
#include "TierList.h"

#include "Commands/ACommand.h"
#include "Commands/Command8ball.h"
#include "Commands/CommandAddXbox.h"
#include "Commands/CommandAddPS.h"
#include "Commands/CommandAFK.h"
#include "Commands/CommandBan.h"
#include "Commands/CommandBB.h"
#include "Commands/CommandBonk.h"
#include "Commands/CommandBotMessage.h"
#include "Commands/CommandDecrease.h"
#include "Commands/CommandDecreaseCD.h"
#include "Commands/CommandDefaultMessage.h"
#include "Commands/CommandDiscord.h"
#include "Commands/CommandDC.h"
#include "Commands/CommandExtend.h"
#include "Commands/CommandExtendCD.h"
#include "Commands/CommandFF.h"
#include "Commands/CommandFortune.h"
#include "Commands/CommandGuests.h"
#include "Commands/CommandHelp.h"
#include "Commands/CommandHotseat.h"
#include "Commands/CommandIpFilter.h"
#include "Commands/CommandKeyboard.h"
#include "Commands/CommandKick.h"
#include "Commands/CommandMic.h"
#include "Commands/CommandMirror.h"
#include "Commands/CommandMod.h"
#include "Commands/CommandMute.h"
#include "Commands/CommandName.h"
#include "Commands/CommandLimit.h"
#include "Commands/CommandLock.h"
#include "Commands/CommandLockAll.h"
#include "Commands/CommandOne.h"
#include "Commands/CommandPads.h"
#include "Commands/CommandPing.h"
#include "Commands/CommandPlayTime.h"
#include "Commands/CommandPrivate.h"
#include "Commands/CommandPublic.h"
#include "Commands/CommandQuit.h"
#include "Commands/CommandRC.h"
#include "Commands/CommandRestart.h"
#include "Commands/CommandRequest.h"
#include "Commands/CommandRollCall.h"
#include "Commands/CommandRPG.h"
#include "Commands/CommandSetConfig.h"
#include "Commands/CommandSFX.h"
#include "Commands/CommandSpeakers.h"
#include "Commands/CommandSpectate.h"
#include "Commands/CommandSpot.h"
#include "Commands/CommandStrip.h"
#include "Commands/CommandStripAll.h"
#include "Commands/CommandSwap.h"
#include "Commands/CommandTimer.h"
#include "Commands/CommandTriangle.h"
#include "Commands/CommandUnban.h"
#include "Commands/CommandUnmod.h"
#include "Commands/CommandUnmute.h"
#include "Commands/CommandUnVIP.h"
#include "Commands/CommandVideoFix.h"
#include "Commands/CommandVIP.h"
#include "Commands/CommandWin.h"
#include "CompilerDirectives.h"
#include "MetadataCache.h"
#include "Modules/Macro.h"
#include "Modules/Hotseat.h"
#include "Modules/Tournament.h"
#include "VIPList.h"

#define BOT_GUESTID 0

class ChatBot
{
public:

	ChatBot(
		AudioIn& audioIn, AudioOut& audioOut, BanList& ban, DX11& dx11, ModList& mod, VIPList& vip,
		GamepadClient& gamepadClient, GuestList& guests, GuestDataList& guestHistory, ParsecDSO* parsec, ParsecHostConfig& hostConfig,
		ParsecSession& parsecSession, SFXList& sfxList, TierList& _tierList, Macro& macro, bool& hostingLoopController, Guest& host,
		Hotseat& hotseat, Tournament& tournament
	)
		: _audioIn(audioIn), _audioOut(audioOut), _ban(ban), _dx11(dx11), _mod(mod), _vip(vip),
		_gamepadClient(gamepadClient), _guests(guests), _guestHistory(guestHistory), _parsec(parsec), _hostConfig(hostConfig), 
		_parsecSession(parsecSession), _sfxList(sfxList), _tierList(_tierList), _macro(macro), _hostingLoopController(hostingLoopController), _host(host),
		_hotseat(hotseat), _tournament(tournament)
	{
		//_basicVersion = Config::cfg.general.basicVersion;
	}

	ACommand * identifyUserDataMessage(const char* msg, Guest& sender, bool isHost = false);

	const uint32_t getLastUserId() const;
	void setLastUserId(const uint32_t lastId = BOT_GUESTID);

	const std::string formatGuestConnection(Guest guest, ParsecGuestState state, ParsecStatus status);
	const std::string formatBannedGuestMessage(Guest guest);
	const std::string formatModGuestMessage(Guest guest);
	CommandBotMessage sendBotMessage(const char * msg);
	void updateSettings();

private:
	static bool msgStartsWith(const char* msg, const char* pattern);
	static bool msgStartsWith(const char* msg, vector<const char*> patterns);
	static bool msgIsEqual(const char* msg, const char* pattern);
	static bool msgIsEqual(const char* msg, vector<const char*> patterns);

	uint32_t _lastUserId = 0;

	// Dependency Injection
	ParsecDSO* _parsec;
	AudioIn& _audioIn;
	AudioOut& _audioOut;
	BanList& _ban;
	ModList& _mod;
	VIPList& _vip;
	DX11 &_dx11;
	GamepadClient& _gamepadClient;
	GuestList& _guests;
	GuestDataList& _guestHistory;
	ParsecHostConfig &_hostConfig;
	ParsecSession &_parsecSession;
	SFXList& _sfxList;
	TierList& _tierList;
	Macro& _macro;
	Hotseat& _hotseat;
	Tournament& _tournament;
	bool &_hostingLoopController;
	Guest& _host;
	bool _basicVersion = false;
};