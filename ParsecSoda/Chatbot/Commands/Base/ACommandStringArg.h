#pragma once

#include "ACommandPrefix.h"
#include "../../../Helpers/Stringer.h"

class ACommandStringArg : public ACommandPrefix
{
public:

	/**
	 * @brief Construct a new ACommandStringArg object
	 * 
	 * @param msg The message to parse
	 * @param prefixes The prefixes to check for
	 */
	ACommandStringArg(const char* msg, vector<const char*> prefixes)
		: ACommandPrefix(msg, prefixes), _stringArg("")
	{}

	/**
	 * @brief Get the string argument
	 */
	bool run() {
		if (!ACommandPrefix::run())
		{
			return false;
		}

		size_t cmdSize = strlen(_prefix);
		std::string str = _msg;
		_stringArg = str.substr(cmdSize);

		return true;
	}

protected:
	string _stringArg;
};
