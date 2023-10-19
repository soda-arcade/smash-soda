#pragma once

#include <string>
#include "matoya.h"

using namespace std;

class Mailman {
public:
	Mailman() {}
	
	void createRoom(
		string title,
		string host,
		int guests,
		string link,
		int expires_at,
		string room_token,
		string version
	);
};