#include "GameQueue.h"
#include "Game.h"
GameQueue::GameQueue()
{
	m_matchUsersInQueueThread = std::thread(&GameQueue::TryToMatchUsers, this);
}

GameQueue::~GameQueue()
{
	m_isMatchingRunning = false;

	if (m_matchUsersInQueueThread.joinable())
	{
		m_matchUsersInQueueThread.join();
	}
}

void GameQueue::RegisterUserLookingForGame(User* user)
{
	m_queueMutex.lock();
	m_usersLookingForGame.push_back(user);
	m_queueMutex.unlock();
}

void GameQueue::TryToMatchUsers()
{
	const int usersRequiredForGame = 2;

	auto GetUserAndPopFront = [&]() -> User* {
		auto user = m_usersLookingForGame.front();
		m_usersLookingForGame.pop_front();
		return user;
	};

	m_isMatchingRunning = true;
	while (m_isMatchingRunning)
	{
		m_queueMutex.lock();
		m_currentGamesMutex.lock();
		while (m_usersLookingForGame.size() >= usersRequiredForGame)
		{
			auto firstUser = GetUserAndPopFront();
			auto secondUser = GetUserAndPopFront();

			m_currentGames.push_back(Game(firstUser, secondUser, &m_defaultCardManager));
		}
		m_currentGamesMutex.unlock();
		m_queueMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
