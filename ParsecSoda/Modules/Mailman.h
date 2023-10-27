#pragma once

#include <string>
#include <future>
#include <iostream>
#include "matoya.h"

using namespace std;

class Mailman {
public:
	Mailman() {}
	
	future<string> GET(string url);
	future<string> POST(string url, string data);
	
};