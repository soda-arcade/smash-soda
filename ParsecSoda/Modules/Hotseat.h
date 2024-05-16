#pragma once

#include <vector>
#include <thread>
#include <direct.h>
#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "../Helpers/StopwatchTimer.h"
#include "../Models/GuestData.h"

#include <nlohmann/json.hpp>
using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

/// <summary>
/// Handles the hotseat system.
/// </summary>
class Hotseat {
public:

	class HotseatUser {
	public:
		bool cooldown = false;
		int userId = 0;
		string userName = "";

		bool inSeat = false;
		StopwatchTimer* stopwatch;
		time_t timeLastPlayed = 0;

		string status = "Not played yet";
	};

	vector<HotseatUser> users;
	
	Hotseat();

	void Start();
	void Stop();

	HotseatUser* getUser(int id);

	bool checkUser(int id, string name);
	void pauseUser(int id);
	void extendUser(int id, long ms);
	void screwUser(int id, long ms);
	void deductUser(int id, long ms);
	void rewardUser(int id, long ms);

	string getUserTimeRemaining(int id);
	string getCooldownRemaining(int id);

	int getCoolDownTime(int id);

	void Log(string message);

	static Hotseat instance;
	bool updated = false;

private:
	thread hotseatThread;
	bool running = false;

	void _createUser(int id, string name);

	time_t getCurrentTimestamp();
	std::time_t addMinutesToTimestamp(std::time_t timestamp, int minutesToAdd);
	std::time_t subtractMinutesFromTimestamp(std::time_t timestamp, int minutesToSubtract);
	int getMinutesDifference(std::time_t timestamp1, std::time_t timestamp2);
	
};