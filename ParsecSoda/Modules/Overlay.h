#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <tchar.h>


class Overlay {
public:
	void start();
	void run();
	void stop();

private:
	bool _isActive = false;
};