#pragma once

#include <string>
#include <direct.h>
#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include "matoya.h"
#include <nlohmann/json.hpp>

using namespace std;

class PathHelper {
public:
    static string GetConfigPath();
    static string GetCurrentPath();
    static string GetAppDataPath();
};