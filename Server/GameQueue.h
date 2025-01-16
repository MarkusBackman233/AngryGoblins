#pragma once
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include "DefaultCardManager.h"

class User;
class Game;

class GameQueue
{

public:
	GameQueue();
	~GameQueue();

	void RegisterUserLookingForGame(User* user);

private:
	void TryToMatchUsers();

	std::deque<User*> m_usersLookingForGame;

	std::mutex m_queueMutex;
	std::mutex m_currentGamesMutex;

	std::thread m_matchUsersInQueueThread;
	std::atomic<bool> m_isMatchingRunning;

	std::vector<Game> m_currentGames;

	DefaultCardManager m_defaultCardManager;
};

