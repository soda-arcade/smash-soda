#include "Hosting.h"

using namespace std;

#if defined(_WIN32)
	#if !defined(BITS)
		#define BITS 64
	#endif
	#if (BITS == 64)
		#define SDK_PATH "./parsec.dll"
	#else
		#define SDK_PATH "./parsec32.dll"
	#endif
#endif

// ============================================================
// 
//  PUBLIC
// 
// ============================================================

Hosting::Hosting()
{
	
	// Create a random 8 character string
	srand(time(NULL));
	string secret = "";
	for (int i = 0; i < 8; i++) {
		secret += (char)(rand() % 26 + 97);
	}
	
	_hostConfig = EMPTY_HOST_CONFIG;
	MetadataCache::loadPreferences();
	setHostConfig(
		MetadataCache::preferences.roomName,
		MetadataCache::preferences.gameID,
		MetadataCache::preferences.guestCount,
		true,
		secret
	);
	setHostVideoConfig(MetadataCache::preferences.fps, MetadataCache::preferences.bandwidth);

	_sfxList.init("./SFX/custom/_sfx.json");
	
	_tierList.loadTiers();
	_tierList.saveTiers();

	vector<GuestData> banned = MetadataCache::loadBannedUsers();
	_banList = BanList(banned);

	vector<GuestData> modded = MetadataCache::loadModdedUsers();
	_modList = ModList(modded);

	vector<GuestData> vip = MetadataCache::loadVIPUsers();
	_vipList = VIPList(vip);

	vector<GameData> games = MetadataCache::loadGamesList();
	_gamesList = GameDataList(games);

	// Record last 3 messages for auto mute
	_lastMessageIndex = 0;
	_lastMessageList.push_back("");
	_lastMessageList.push_back("");
	_lastMessageList.push_back("");

	_parsec = nullptr;

	SDL_Init(SDL_INIT_JOYSTICK);
	_masterOfPuppets.init(_gamepadClient);
	_masterOfPuppets.start();

	_latencyLimitEnabled = MetadataCache::preferences.latencyLimitEnabled;
	_latencyLimitValue = MetadataCache::preferences.latencyLimitValue;
	_disableMicrophone = MetadataCache::preferences.disableMicrophone;
	_disableGuideButton = MetadataCache::preferences.disableGuideButton;
	_disableKeyboard = MetadataCache::preferences.disableKeyboard;
	_lockedGamepad.bLeftTrigger = MetadataCache::preferences.lockedGamepadLeftTrigger;
	_lockedGamepad.bRightTrigger = MetadataCache::preferences.lockedGamepadRightTrigger;
	_lockedGamepad.sThumbLX = MetadataCache::preferences.lockedGamepadLX;
	_lockedGamepad.sThumbLY = MetadataCache::preferences.lockedGamepadLY;
	_lockedGamepad.sThumbRX = MetadataCache::preferences.lockedGamepadRX;
	_lockedGamepad.sThumbRY = MetadataCache::preferences.lockedGamepadRY;
	_lockedGamepad.wButtons = MetadataCache::preferences.lockedGamepadButtons;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void Hosting::applyHostConfig() {
	
	if (isRunning()){
		ParsecHostSetConfig(_parsec, &_hostConfig, _parsecSession.sessionId.c_str());
	}
}

void Hosting::broadcastChatMessage(string message)
{
	vector<Guest> guests = _guestList.getGuests();
	vector<Guest>::iterator gi;

	for (gi = guests.begin(); gi != guests.end(); ++gi)
	{
		ParsecHostSendUserData(_parsec, (*gi).id, HOSTING_CHAT_MSG_ID, message.c_str());
	}
}

void Hosting::init()
{
	_parsecStatus = ParsecInit(NULL, NULL, (char *)SDK_PATH, &_parsec);
	_dx11.init();
	_gamepadClient.setParsec(_parsec);
	_gamepadClient.init();

	MetadataCache::Preferences preferences = MetadataCache::loadPreferences();

	_createGamepadsThread = thread([&]() {
		_gamepadClient.createAllGamepads();
		_createGamepadsThread.detach();
		_macro.init(_gamepadClient, _guestList);
		_processMan.init(_chatLog);
		_overlay.init(_processMan, _chatLog);
		_tournament.init(*_parsec, _guestList, _gamepadClient, _chatLog, _macro);
	});

	audioOut.fetchDevices();
	vector<AudioOutDevice> audioOutDevices = audioOut.getDevices();
	if (preferences.audioOutputDevice >= audioOutDevices.size()) {
		preferences.audioOutputDevice = 0;
	}
	audioOut.setOutputDevice(preferences.audioOutputDevice);		// TODO Fix leak in setOutputDevice
	audioOut.captureAudio();
	audioOut.volume = 0.3f;
	audioOut.setFrequency((Frequency)MetadataCache::preferences.speakersFrequency);

	vector<AudioInDevice> audioInputDevices = audioIn.listInputDevices();
	if (preferences.audioInputDevice >= audioInputDevices.size()) {
		preferences.audioInputDevice = 0;
	}
	AudioInDevice device = audioIn.selectInputDevice(preferences.audioInputDevice);
	audioIn.init(device);
	audioIn.volume = 0.8f;
	audioIn.setFrequency((Frequency)MetadataCache::preferences.micFrequency);

	preferences.isValid = true;
	MetadataCache::savePreferences(preferences);
	_parsecSession.loadSessionCache();

	fetchAccountData();

	_chatBot = new ChatBot(
		audioIn, audioOut, _banList, _dx11, _modList, _vipList,
		_gamepadClient, _guestList, _guestHistory, _parsec,
		_hostConfig, _parsecSession, _sfxList, _tierList, 
		_macro, _isRunning, _host, _hotseat, _tournament
	);

	CommandBonk::init();

	// A way to test chat and guest managment features
	// without hosting the room
	_isTestMode = false;
	if (_isTestMode) {

		// Add (n) number of fake guests
		srand(time(NULL));
		addFakeGuests(8);

	}
	
}

void Hosting::release()
{
	stopHosting();
	while (_isRunning)
	{
		Sleep(5);
	}
	_dx11.clear();
	_gamepadClient.release();
	_masterOfPuppets.stop();
}

bool Hosting::isReady() {
	
	return _parsecStatus == PARSEC_OK;
}

bool Hosting::isRunning() {
	return _isTestMode || _isRunning;
}

bool& Hosting::isGamepadLock()
{
	return _gamepadClient.lock;
}

bool& Hosting::isGamepadLockButtons()
{
	return _gamepadClient.lockButtons;
}

Guest& Hosting::getHost()
{
	return _host;
}

ParsecSession& Hosting::getSession()
{
	return _parsecSession;
}

void Hosting::fetchAccountData(bool sync)
{
	_host.name = "Host";
	_host.status = Guest::Status::INVALID;
	if (isReady()) {
		if (sync) {
			_parsecSession.fetchAccountDataSync(&_host);
		}
		else {
			_parsecSession.fetchAccountData(&_host);
		}
	}
}

ParsecHostConfig& Hosting::getHostConfig()
{
	return _hostConfig;
}

DX11& Hosting::getDX11()
{
	return _dx11;
}

ChatBot* Hosting::getChatBot()
{
	return _chatBot;
}

vector<string>& Hosting::getMessageLog()
{
	return _chatLog.getMessageLog();
}

vector<string>& Hosting::getCommandLog()
{
	return _chatLog.getCommandLog();
}

GuestList& Hosting::getGuestList() {
	return _guestList;
}

vector<Guest>& Hosting::getGuests() {
	return _guestList.getGuests();
}

vector<Guest>& Hosting::getPlayingGuests() {
	return _guestList.getPlayingGuests();
}

vector<Guest>& Hosting::getRandomGuests() {
	return _guestList.getRandomGuests();
}

Guest& Hosting::getGuest(uint32_t id) {
	
	// For each guest in the guest list
	for (Guest& guest : _guestList.getGuests()) {

		// If the guest ID matches the target ID
		if (guest.id == id) {

			// Return the guest
			return guest;
		}
	}
	
}

int Hosting::getGuestIndex(uint32_t id) {

	// For each guest in the guest list
	for (int i = 0; i < _guestList.getGuests().size(); i++) {

		// If the guest ID matches the target ID
		if (_guestList.getGuests()[i].userID == id) {

			// Return the guest
			return i;
		}
	}

	return -1;

}

bool Hosting::guestExists(uint32_t id) {
	for (Guest& guest : _guestList.getGuests()) {
		if (guest.id == id) {
			return true;
		}
	}
	return false;
}

vector<Guest>& Hosting::getGuestsAfterGuest(uint32_t targetGuestID, int count, bool ignoreSpectators) {
	return _guestList.getGuestsAfterGuest(targetGuestID, count, ignoreSpectators);
}

vector<GuestData>& Hosting::getGuestHistory()
{
	return _guestHistory.getGuests();
}

MyMetrics Hosting::getMetrics(uint32_t id)
{
	return _guestList.getMetrics(id);
}

BanList& Hosting::getBanList()
{
	return _banList;
}

ModList& Hosting::getModList()
{
	return _modList;
}

VIPList& Hosting::getVIPList()
{
	return _vipList;
}

GameDataList& Hosting::getGameList()
{
	return _gamesList;
}

vector<AGamepad*>& Hosting::getGamepads() {
	return _gamepadClient.gamepads;
}

GamepadClient& Hosting::getGamepadClient()
{
	return _gamepadClient;
}

MasterOfPuppets& Hosting::getMasterOfPuppets()
{
	return _masterOfPuppets;
}

const char** Hosting::getGuestNames()
{
	return _guestList.guestNames;
}

void Hosting::toggleGamepadLock()
{
	_gamepadClient.toggleLock();
}

void Hosting::toggleGamepadLockButtons()
{
	_gamepadClient.toggleLockButtons();
}

void Hosting::setGameID(string gameID)
{
	try
	{
		strcpy_s(_hostConfig.gameID, gameID.c_str());
	}
	catch (const std::exception&) {}
}

void Hosting::setMaxGuests(uint8_t maxGuests)
{
	_hostConfig.maxGuests = maxGuests;
}

void Hosting::setHostConfig(string roomName, string gameId, uint8_t maxGuests, bool isPublicRoom)
{
	setRoomName(roomName);
	setGameID(gameId);
	setMaxGuests(maxGuests);
	setPublicRoom(isPublicRoom);
}

void Hosting::setHostConfig(string roomName, string gameId, uint8_t maxGuests, bool isPublicRoom, string secret)
{
	setRoomName(roomName);
	setGameID(gameId);
	setMaxGuests(maxGuests);
	setPublicRoom(isPublicRoom);
	setRoomSecret(secret);
}

void Hosting::setHostVideoConfig(uint32_t fps, uint32_t bandwidth)
{
	_hostConfig.video->encoderFPS = fps;
	_hostConfig.video->encoderMaxBitrate = bandwidth;
	MetadataCache::preferences.fps = fps;
	MetadataCache::preferences.bandwidth = bandwidth;
}

void Hosting::setPublicRoom(bool isPublicRoom)
{
	_hostConfig.publicGame = isPublicRoom;
}

void Hosting::setRoomName(string roomName)
{
	try
	{
		strcpy_s(_hostConfig.name, roomName.c_str());
	}
	catch (const std::exception&) {}
}

void Hosting::setRoomSecret(string secret)
{

	try
	{
		strcpy_s(_hostConfig.secret, secret.c_str());
	}
	catch (const std::exception&) {}
}

void Hosting::startHosting() {

	if (!_isRunning) {
		_isRunning = true;
		initAllModules();

		try
		{
			if (_parsec != nullptr)
			{
				_mediaThread = thread ( [this]() {liveStreamMedia(); } );
				_inputThread = thread ([this]() {pollInputs(); });
				_eventThread = thread ([this]() {pollEvents(); });
				_latencyThread = thread([this]() {pollLatency(); });
				_smashSodaThread = thread([this]() { pollSmashSoda(); });
				//_gamepadThread = thread([this]() {pollGamepad(); });
				_mainLoopControlThread = thread ([this]() {mainLoopControl(); });

				// Hotseat mode
				if (MetadataCache::preferences.hotseat)
					_hotseat.start();

				// Start kiosk mode
				if (MetadataCache::preferences.kioskMode)
					startKioskMode();

			}
		}
		catch (const exception&)
		{}
	}

	bool debug = true;
}

void Hosting::stopHosting()
{
	_isRunning = false;
	_guestList.clear();

	// Stop hotseat
	_hotseat.stop();

	// Disable overlay
	if (MetadataCache::preferences.overlayShow) {
		_overlay.stop();
	}

	// Clear kiosk mode
	/*if (MetadataCache::preferences.kioskMode) {
		_processMan.stop();
	}*/

}

void Hosting::startKioskMode() {

	_chatLog.logMessage("Kiosk mode temporarily disabled...will fix in next update!");
	return;

	_chatLog.logMessage("Starting kiosk mode...");
	
	// Find selected game
	for (int i = 0; i < _gamesList.getGames().size(); ++i) {
		if (_gamesList.getGames()[i].itemID == MetadataCache::preferences.selectedGame) {
			GameData selectedGame = _gamesList.getGames()[i];
			

			// Start kiosk mode
			_chatLog.logCommand("Starting: " + selectedGame.path);
			DWORD pid = _processMan.start(selectedGame.path, "");
			if (pid != 0) {
				_chatLog.logCommand("Kiosk mode started!");
			}
			else {
				_chatLog.logCommand("Kiosk mode failed to start!");
			}
			
			return;
		}
	}

	// Kiosk mode can't start without a selected game
	_chatLog.logCommand("Kiosk mode can't start without a selected game!");

}

void Hosting::stripGamepad(int index)
{
	_gamepadClient.clearOwner(index);
}

void Hosting::setOwner(AGamepad& gamepad, Guest newOwner, int padId)
{
	bool found = _gamepadClient.findPreferences(newOwner.userID, [&](GamepadClient::GuestPreferences& prefs) {
		gamepad.setOwner(newOwner, padId, prefs.mirror);
	});

	if (!found) {
		gamepad.setOwner(newOwner, padId, false);
	}
}

void Hosting::handleMessage(const char* message, Guest& guest, bool isHost, bool isHidden, bool outside)
{
	// Handle all the auto muting stuff
	if (!handleMuting(message, guest)) return;

	ACommand* command = _chatBot->identifyUserDataMessage(message, guest, isHost);
	command->run();

	// Non-blocked default message
	if (!isFilteredCommand(command))
	{
		Tier tier = _tierList.getTier(guest.userID);

		CommandDefaultMessage defaultMessage(message, guest, _chatBot->getLastUserId(), tier, _vipList);
		defaultMessage.run();
		_chatBot->setLastUserId(guest.userID);

		if (!defaultMessage.replyMessage().empty() && !isHidden)
		{
			broadcastChatMessage(defaultMessage.replyMessage());
			
			string adjustedMessage = defaultMessage.replyMessage();
			Stringer::replacePatternOnce(adjustedMessage, "%", "%%");
			_chatLog.logMessage(adjustedMessage);

			_overlay.sendChatMessage(guest, message);
		}
	}

	// Chatbot's command reply
	if (!command->replyMessage().empty() && command->type() != COMMAND_TYPE::DEFAULT_MESSAGE)
	{

		broadcastChatMessage(command->replyMessage());
		_chatLog.logCommand(command->replyMessage());
		_chatBot->setLastUserId();
	}

	delete command;

}

/// <summary>
/// This handles muted guests and auto mutes anybody spamming chat.
/// </summary>
/// <param name="message">The message sent.</param>
/// <param name="guest">The guest who sent the message.</param>
/// <returns>false if muted</returns>
bool Hosting::handleMuting(const char* message, Guest& guest) {

	// Has the guest been muted?
	for (int i = 0; i < MetadataCache::preferences.mutedGuests.size(); i++) {
		if (guest.userID == MetadataCache::preferences.mutedGuests[i].id) {

			// Has their suffering gone on long enough?
			if (MetadataCache::preferences.mutedGuests[i].stopwatch.isFinished()) {
				MetadataCache::preferences.mutedGuests.erase(MetadataCache::preferences.mutedGuests.begin() + i);
				MetadataCache::preferences.mutedGuests.shrink_to_fit();
				return true;
			}
			else {

				// Shhhhhhhhh
				return false;

			}

		}
	}

	// Auto muting
	if (MetadataCache::preferences.autoMute && guest.userID != _host.userID) {

		// Record last message(s) for auto muting
		if (_lastMessageTimer.isFinished()) {
			_lastMessageIndex = 0;
		}
		_lastMessageList[_lastMessageIndex] = message;

		// Check duration between last and current message
		if (!_lastMessageTimer.isFinished() && _lastMessageIndex < 2) {
			_lastMessageIndex++;
			_lastMessageTimer.setDuration(_lastMessageTimer.getRemainingTime() + MetadataCache::preferences.autoMuteTime);
		}
		else {
			_lastMessageIndex = 0;
			_lastMessageTimer.stop();
			_lastMessageTimer.setDuration(MetadataCache::preferences.autoMuteTime);
			_lastMessageTimer.start();
		}

		// Is guest sending messages too quickly?
		if (_lastMessageIndex > 1) {

			// Mute guest temporarily
			MetadataCache::Preferences::MutedGuest mutedGuest = MetadataCache::Preferences::MutedGuest();
			mutedGuest.id = guest.userID;
			mutedGuest.name = guest.name;
			mutedGuest.stopwatch.setDuration(60000 * 2);
			mutedGuest.stopwatch.start();
			MetadataCache::preferences.mutedGuests.push_back(mutedGuest);

			string message = MetadataCache::preferences.chatbotName + " " + guest.name +
				" is sending messages too quickly and was muted for 2 minutes.";

			broadcastChatMessage(message);
			_chatLog.logMessage(message);

			return false;

		}

	}

	return true;

}

void Hosting::sendHostMessage(const char* message, bool isHidden) {
	static bool isAdmin = true;
	handleMessage(message, _host, true, isHidden);
}


// ============================================================
// 
//  PRIVATE
// 
// ============================================================
void Hosting::initAllModules()
{
	// Instance all gamepads at once
	_connectGamepadsThread = thread([&]() {
		_gamepadClient.sortGamepads();
		_connectGamepadsThread.detach();
	});

	parsecArcadeStart();
}

void Hosting::submitSilence()
{
	ParsecHostSubmitAudio(_parsec, PCM_FORMAT_INT16, audioOut.getFrequencyHz(), nullptr, 0);
}

void Hosting::liveStreamMedia()
{
	_mediaMutex.lock();
	_isMediaThreadRunning = true;

	static uint32_t sleepTimeMs = 4;
	_mediaClock.setDuration(sleepTimeMs);
	_mediaClock.start();

	while (_isRunning)
	{
		_mediaClock.reset();

		_dx11.captureScreen(_parsec);

		if (!_disableMicrophone && audioIn.isEnabled && audioOut.isEnabled)
		{
			audioIn.captureAudio();
			audioOut.captureAudio();
			if (audioIn.isReady() && audioOut.isReady())
			{
				vector<int16_t> mixBuffer = _audioMix.mix(audioIn.popBuffer(), audioOut.popBuffer());
				ParsecHostSubmitAudio(_parsec, PCM_FORMAT_INT16, audioOut.getFrequencyHz(), mixBuffer.data(), (uint32_t)mixBuffer.size() / 2);
			}
			else submitSilence();
		}
		else if (audioOut.isEnabled)
		{
			audioOut.captureAudio();
			if (audioOut.isReady())
			{
				vector<int16_t> buffer = audioOut.popBuffer();
				ParsecHostSubmitAudio(_parsec, PCM_FORMAT_INT16, audioOut.getFrequencyHz(), buffer.data(), (uint32_t)buffer.size() / 2);
			}
			else submitSilence();
		}
		else if (!_disableMicrophone && audioIn.isEnabled)
		{
			audioIn.captureAudio();
			if (audioIn.isReady())
			{
				vector<int16_t> buffer = audioIn.popBuffer();
				ParsecHostSubmitAudio(_parsec, PCM_FORMAT_INT16, (uint32_t)audioIn.getFrequency(), buffer.data(), (uint32_t)buffer.size() / 2);
			}
			else submitSilence();
		}
		else submitSilence();

		sleepTimeMs = _mediaClock.getRemainingTime();
		if (sleepTimeMs > 0)
		{
			Sleep(sleepTimeMs);
		}
	}

	_isMediaThreadRunning = false;
	_mediaMutex.unlock();
	_mediaThread.detach();
}

void Hosting::mainLoopControl()
{
	do
	{
		Sleep(50);
	} while (!_isRunning);

	_isRunning = true;

	_mediaMutex.lock();
	_inputMutex.lock();
	_eventMutex.lock();

	ParsecHostStop(_parsec);
	_isRunning = false;

	_mediaMutex.unlock();
	_inputMutex.unlock();
	_eventMutex.unlock();

	_mainLoopControlThread.detach();
}

void Hosting::pollEvents()
{
	_eventMutex.lock();
	_isEventThreadRunning = true;

	string chatBotReply;

	ParsecGuest* guests = nullptr;
	int guestCount = 0;

	ParsecHostEvent event;

	while (_isRunning)
	{
		if (ParsecHostPollEvents(_parsec, 30, &event)) {
			ParsecGuest parsecGuest = event.guestStateChange.guest;
			ParsecGuestState state = parsecGuest.state;
			Guest guest = Guest(parsecGuest.name, parsecGuest.userID, parsecGuest.id);
			guestCount = ParsecHostGetGuests(_parsec, GUEST_CONNECTED, &guests);
			_guestList.setGuests(guests, guestCount);

			switch (event.type)
			{
			case HOST_EVENT_GUEST_STATE_CHANGE:
				onGuestStateChange(state, guest, event.guestStateChange.status);
				break;

			case HOST_EVENT_USER_DATA:
				char* msg = (char*)ParsecGetBuffer(_parsec, event.userData.key);

				if (event.userData.id == PARSEC_APP_CHAT_MSG)
				{
					handleMessage(msg, guest);
				}

				ParsecFree(_parsec, msg);
				break;
			}
		}
	}

	ParsecFree(_parsec, guests);
	_isEventThreadRunning = false;
	_eventMutex.unlock();
	_eventThread.detach();
}

void Hosting::pollLatency()
{
	_latencyMutex.lock();
	_isLatencyThreadRunning = true;
	int guestCount = 0;
	ParsecGuest* guests = nullptr;
	while (_isRunning)
	{
		Sleep(2000);
		guestCount = ParsecHostGetGuests(_parsec, GUEST_CONNECTED, &guests);
		if (guestCount > 0) {
			_guestList.updateMetrics(guests, guestCount);

			// Latency limiter
			if (MetadataCache::preferences.latencyLimitEnabled) {
				for (size_t mi = 0; mi < guestCount; mi++) {
					MyMetrics m = _guestList.getMetrics(guests[mi].id);

					if (m.averageNetworkLatencySize > 5 &&
						m.averageNetworkLatency > MetadataCache::preferences.latencyLimitValue) {
						ParsecHostKickGuest(_parsec, guests[mi].id);
					}
				}
			}

		}
		
	}
	_isLatencyThreadRunning = false;
	_latencyMutex.unlock();
	_latencyThread.detach();
}

/// <summary>
/// This thread handles all the new Smash Soda features!
/// </summary>
void Hosting::pollSmashSoda() {
	_smashSodaMutex.lock();
	_isSmashSodaThreadRunning = true;
	while (_isRunning) {

		Sleep(200);

		// Handles welcome messages and other new guest stuff
		handleNewGuests();

		// Handles all the automatic button press stuff
		_macro.run();

		// Handles the hotseat cycling
		//_hotseat.run();

		// Handle overlay communication
		//_overlay.run();

		// Updated room settings?
		if (MetadataCache::preferences.roomChanged) {
			ParsecHostSetConfig(_parsec, &_hostConfig, _parsecSession.sessionId.c_str());
			MetadataCache::preferences.roomChanged = false;
		}

	}
	
	_isSmashSodaThreadRunning = false;
	_smashSodaMutex.unlock();
	_smashSodaThread.detach();

}

/// <summary>
/// Checks to see if a guest is a spectator.
/// </summary>
/// <param name="index"></param>
bool Hosting::isSpectator(int index) {

	return MetadataCache::isSpectating(_guestList.getGuests()[index].userID);

}


void Hosting::addNewGuest(Guest guest) {

	NewGuest newGuest;
	newGuest.guest = guest;
	newGuest.timer.setDuration(newGuestList.size() * 2000);
	newGuest.timer.start();
	newGuestList.push_back(newGuest);

	// Hotseat enabled?
	if (MetadataCache::preferences.hotseat) {
		GuestData guestData = GuestData(guest.name, guest.userID);
		_hotseat.enqueue(guestData);
	}

}

void Hosting::handleNewGuests() {

	// ParsecHostSendUserData(_parsec, guest.id, HOSTING_CHAT_MSG_ID, test.c_str());

	if (newGuestList.size() > 0) {

		// Get next guest
		NewGuest newGuest = newGuestList.front();

		// Ready to process
		if (newGuestList.front().timer.isFinished()) {

			// Welcome message
			string msg = MetadataCache::preferences.welcomeMessage;
			msg = regex_replace(msg, regex("_PLAYER_"), newGuest.guest.name);
			ParsecHostSendUserData(_parsec, newGuestList.front().guest.id, HOSTING_CHAT_MSG_ID, msg.c_str());

			// Remove from new guests
			newGuestList.erase(newGuestList.begin());

		}

	}

}

bool Hosting::isLatencyRunning()
{
	return _isLatencyThreadRunning;
}

void Hosting::pollGamepad()
{
	_gamepadMutex.lock();
	_isGamepadThreadRunning = true;
	while (_isRunning) {
		Sleep(100);
		if (_overlay.isActive) {
			_overlay.sendGamepads(_gamepadClient.gamepads);
		}
	}
	_isGamepadThreadRunning = false;
	_gamepadMutex.unlock();
	_gamepadThread.detach();
}

bool Hosting::isGamepadRunning() {
	return _isGamepadThreadRunning;
}

void Hosting::pollInputs() {
	
	_inputMutex.lock();
	_isInputThreadRunning = true;

	ParsecGuest inputGuest;
	ParsecMessage inputGuestMsg;

	while (_isRunning)
	{
		if (ParsecHostPollInput(_parsec, 4, &inputGuest, &inputGuestMsg))
		{
			if (!_gamepadClient.lock)
			{
					_gamepadClient.sendMessage(inputGuest, inputGuestMsg);
			}

		}
	}

	_isInputThreadRunning = false;
	_inputMutex.unlock();
	_inputThread.detach();
}

void Hosting::updateButtonLock(LockedGamepadState lockedGamepad)
{
	_lockedGamepad = lockedGamepad;
	MetadataCache::preferences.lockedGamepadLeftTrigger = _lockedGamepad.bLeftTrigger;
	MetadataCache::preferences.lockedGamepadRightTrigger = _lockedGamepad.bRightTrigger;
	MetadataCache::preferences.lockedGamepadLX = _lockedGamepad.sThumbLX;
	MetadataCache::preferences.lockedGamepadLY = _lockedGamepad.sThumbLY;
	MetadataCache::preferences.lockedGamepadRX = _lockedGamepad.sThumbRX;
	MetadataCache::preferences.lockedGamepadRY = _lockedGamepad.sThumbRY;
	MetadataCache::preferences.lockedGamepadButtons = _lockedGamepad.wButtons;
}

bool Hosting::parsecArcadeStart()
{
	if (isReady()) {
		ParsecStatus status = ParsecHostStart(_parsec, HOST_DESKTOP, &_hostConfig, _parsecSession.sessionId.c_str());
		Sleep(2000);
		ParsecHostStop(_parsec);
		Sleep(1000);
		status = ParsecHostStart(_parsec, HOST_GAME, &_hostConfig, _parsecSession.sessionId.c_str());

		// Init overlay
		if (MetadataCache::preferences.overlayShow)
			_overlay.start();
		
		return status == PARSEC_OK;
	}
	return false;
}


bool Hosting::isFilteredCommand(ACommand* command) {
	
	static vector<COMMAND_TYPE> filteredCommands{ COMMAND_TYPE::IP };

	COMMAND_TYPE type;
	for (vector<COMMAND_TYPE>::iterator it = filteredCommands.begin(); it != filteredCommands.end(); ++it)
	{
		type = command->type();
		if (command->type() == *it)
		{
			return true;
		}
	}

	return false;
}

void Hosting::onGuestStateChange(ParsecGuestState& state, Guest& guest, ParsecStatus& status) {

	static string logMessage;

	/*static string trickDesc = "";
	static Debouncer debouncer = Debouncer(500, [&]() {
		if (_hostConfig.maxGuests > 0 && _guestList.getGuests().size() + 1 == _hostConfig.maxGuests)
		{
			try
			{
				if (trickDesc.size() > 0) trickDesc = "";
				else trickDesc = "-";
				strcpy_s(_hostConfig.desc, trickDesc.c_str());
				applyHostConfig();
			}
			catch (const std::exception&) {}
		}
	});*/

	// Is the connecting guest the host?
	if ((state == GUEST_CONNECTED || state == GUEST_CONNECTING) && (_host.userID == guest.userID))
	{
		_tierList.setTier(guest.userID, Tier::GOD);
		MetadataCache::addActiveGuest(guest);

		addNewGuest(guest);
	}
	else

	if ((state == GUEST_CONNECTED || state == GUEST_CONNECTING) && _banList.isBanned(guest.userID))
	{
		ParsecHostKickGuest(_parsec, guest.id);
		logMessage = _chatBot->formatBannedGuestMessage(guest);
		broadcastChatMessage(logMessage);
		_chatLog.logCommand(logMessage);
	}
	else if ((state == GUEST_CONNECTED || state == GUEST_CONNECTING) && _modList.isModded(guest.userID))
	{
		logMessage = _chatBot->formatModGuestMessage(guest);
		broadcastChatMessage(logMessage);
		_tierList.setTier(guest.userID, Tier::MOD);
		_chatLog.logCommand(logMessage);
		MetadataCache::addActiveGuest(guest);
		addNewGuest(guest);
	}
	else if (state == GUEST_FAILED)
	{
		logMessage = _chatBot->formatGuestConnection(guest, state, status);
		broadcastChatMessage(logMessage);
		_chatLog.logCommand(logMessage);
	}
	else if (state == GUEST_CONNECTED || state == GUEST_DISCONNECTED)
	{
		static string guestMsg;
		guestMsg.clear();
		guestMsg = string(guest.name);

		if (_banList.isBanned(guest.userID)) {
			logMessage = _chatBot->formatBannedGuestMessage(guest);
			broadcastChatMessage(logMessage);
			_chatLog.logCommand(logMessage);
			/*if (_hostConfig.maxGuests > 0 && _guestList.getGuests().size() + 1 == _hostConfig.maxGuests)
				debouncer.start();*/
		}
		else
		{
			logMessage = _chatBot->formatGuestConnection(guest, state, status);
			broadcastChatMessage(logMessage);
			_chatLog.logCommand(logMessage);
		}

		if (state == GUEST_CONNECTED) {
			GuestData data = GuestData(guest.name, guest.userID);
			_guestHistory.add(data);
			MetadataCache::addActiveGuest(guest);

			// Show welcome message
			addNewGuest(guest);

		}
		else
		{
			
			// Were extra spots made?
			if (status != 11 && MetadataCache::preferences.extraSpots > 0) {
				_hostConfig.maxGuests = _hostConfig.maxGuests - 1;
				MetadataCache::preferences.extraSpots--;
				ParsecHostSetConfig(_parsec, &_hostConfig, _parsecSession.sessionId.c_str());
			}

			// Remove from active guests list
			MetadataCache::removeActiveGuest(guest);
			if (MetadataCache::hotseat.spectators.empty() == false) {
				for (int i = MetadataCache::hotseat.spectators.size() - 1; i >= 0; i--) {
					if (MetadataCache::hotseat.spectators.at(i) == guest.userID) {
						MetadataCache::hotseat.spectators.erase(MetadataCache::hotseat.spectators.begin() + i);
					}
				}
			}

			// Remove from hotseat queue
			GuestData data = GuestData(guest.name, guest.userID);
			_hotseat.dequeue(data);

			_guestList.deleteMetrics(guest.id);
			int droppedPads = 0;
			CommandFF command(guest, _gamepadClient, _hotseat);
			command.run();
			if (droppedPads > 0)
			{
				logMessage = command.replyMessage();
				broadcastChatMessage(logMessage);
				_chatLog.logCommand(logMessage);
			}
		}
	}
}

bool Hosting::removeGame(string name) {

	_gamesList.remove(name, [&](GameData& guest) {
		
	});

	return true;

}

void Hosting::logMessage(string message) {

	// Get chatbot name
	string chatbotName = MetadataCache::preferences.chatbotName;
	_chatLog.logCommand(chatbotName + " " + message);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool Hosting::isHotseatEnabled() {
	return _hotseat.isRunning;
}

/// <summary>
/// 
/// </summary>
/// <param name="count"></param>
void Hosting::addFakeGuests(int count) {

	_guestList.getGuests().reserve(_guestList.getGuests().size() + count);
	for (int i = 0; i < count; ++i) {
		Guest guest = Guest(
			randomString(5),
			i+1, i+1
		);

		_guestList.getGuests().push_back(guest);
		MetadataCache::addActiveGuest(guest);
	}

}

string Hosting::randomString(const int len) {

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

Overlay& Hosting::getOverlay() {

	return _overlay;

}

Hotseat& Hosting::getHotseat() {

	return _hotseat;

}

ProcessMan& Hosting::getProcessMan() {
	return _processMan;
}