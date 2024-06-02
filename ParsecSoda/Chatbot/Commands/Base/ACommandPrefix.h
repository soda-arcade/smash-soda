#pragma once

#include "ACommand.h"
#include "../../../Helpers/Stringer.h"
#include <vector>

class ACommandPrefix : public ACommand
{
public:

	/**
	 * @brief Constructor
	 * @param msg The message to parse
	 * @param prefixes The prefixes to check for
	 */
	ACommandPrefix(const char* msg, vector<const char*> prefixes)
		: _msg(msg), _prefixes(prefixes), _prefix("")
	{}

	/**
	 * @brief Run the command
	 * @return true if the command was successful
	 */
	bool run() override {
		vector<const char*>::iterator prefix = _prefixes.begin();
		for (; prefix != _prefixes.end(); ++prefix)
		{
			if (Stringer::startsWithPattern(_msg, *prefix))
			{
				_prefix = *prefix;
				return true;
			}
		}

		return false;
	}

protected:
	const char* _msg;
	const char* _prefix;
	vector<const char*> _prefixes;
};
