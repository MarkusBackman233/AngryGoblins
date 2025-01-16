#include "UserManager.h"

std::unordered_map<unsigned long long, User*> UserManager::m_connectedUsers;
std::mutex UserManager::m_mutex;

void UserManager::AddUser(User* user)
{
	m_mutex.lock();
	m_connectedUsers.emplace(user->GetId(), user);
	m_mutex.unlock();
}

const std::unordered_map<unsigned long long, User*>& UserManager::GetUsers()
{
	return m_connectedUsers;
}
