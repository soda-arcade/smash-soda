#include "PathHelper.h"

/**
 * Gets the path to the Smash Soda config.json file.
 */
string PathHelper::GetConfigPath() {
	
	// If running in portable mode
	string currentPath = PathHelper::GetCurrentPath();
	if (MTY_FileExists((currentPath + "\\portable.txt").c_str())) {
		return currentPath;
	} else {

		// Get the appdata path
		string appDataPath = PathHelper::GetAppDataPath();
		if (appDataPath != "") {
			return appDataPath;
		} else {
			return "";
		}

	}

	return "";
}

/**
 * Get the current path Smash Soda is running from.
 */
string PathHelper::GetCurrentPath() {
	char cwd[1024];
	if (_getcwd(cwd, sizeof(cwd)) != NULL) {
		return string(cwd);
	}
	else {
		return "";
	}
}

/**
 * Get the path to the Smash Soda appdata directory.
 */
string PathHelper::GetAppDataPath() {
	string appDir = "\\SmashSodaTwo\\";

	TCHAR tAppdata[1024];
	if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA, NULL, 0, tAppdata))) {
		wstring wAppdata(tAppdata);
		string appdata(wAppdata.begin(), wAppdata.end());
		string dirPath = appdata + appDir;

		bool isDirOk = false;

		if (!MTY_FileExists(dirPath.c_str())) {
			if (MTY_Mkdir(dirPath.c_str())) {
				isDirOk = true;
			}
		} else {
			isDirOk = true;
		}

		if (isDirOk) {
			return dirPath;
		}
	}
}