#include "GameDataList.h"

GameDataList::GameDataList()
{
}

GameDataList::GameDataList(const std::vector<GameData> games)
{
	_games = games;
}

bool GameDataList::add(GameData game)
{
	bool found = find(game.itemID);
	if (!found)
	{
		_games.push_back(game);
		return true;
	}

	return false;
}

bool GameDataList::find(uint32_t itemID, function<void(GameData& game)> callback)
{
	bool found = findIterator(itemID, [&](vector<GameData>::iterator gi) {
		if (callback != nullptr)
		{
			callback(*gi);
		}
		});

	return found;
}

bool GameDataList::find(string name, function<void(GameData& game)> callback)
{
	bool found = findIterator(name, [&](vector<GameData>::iterator gi) {
		if (callback != nullptr)
		{
			callback(*gi);
		}
		});

	return found;
}

bool GameDataList::pop(uint32_t itemID, function<void(GameData& game)> callback)
{
	bool found = findIterator(itemID, [&](vector<GameData>::iterator gi) {
		if (callback != nullptr)
		{
			callback(*gi);
		}
		_games.erase(gi);
		});

	return found;
}

bool GameDataList::pop(string name, function<void(GameData& game)> callback)
{
	bool found = findIterator(name, [&](vector<GameData>::iterator gi) {
		if (callback != nullptr)
		{
			callback(*gi);
		}
		_games.erase(gi);
		});

	return found;
}

const bool GameDataList::remove(const uint32_t itemID, function<void(GameData&)> callback)
{
	bool found = GameDataList::pop(itemID, callback);
	if (found)
	{
		MetadataCache::saveGamesList(_games);
	}
	return found;
}

const bool GameDataList::remove(string name, function<void(GameData&)> callback)
{
	bool found = GameDataList::pop(name, callback);
	if (found)
	{
		MetadataCache::saveGamesList(_games);
	}
	return found;
}

vector<GameData>& GameDataList::getGames()
{
	return _games;
}


// =============================================================
//
//  Private
//
// =============================================================
bool GameDataList::findIterator(uint32_t itemID, function<void(vector<GameData>::iterator game)> callback)
{
	vector<GameData>::iterator it = _games.begin();

	for (; it != _games.end(); ++it)
	{
		if (itemID == (*it).itemID)
		{
			if (callback != nullptr)
			{
				callback(it);
			}
			return true;
		}
	}

	return false;
}

bool GameDataList::findIterator(string name, function<void(vector<GameData>::iterator game)> callback)
{
	vector<GameData>::iterator it = _games.begin();

	for (; it != _games.end(); ++it)
	{
		if (Stringer::isCloseEnough((*it).name.c_str(), name))
		{
			if (callback != nullptr)
			{
				callback(it);
			}
			return true;
		}
	}

	return false;
}
