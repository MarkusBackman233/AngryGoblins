#pragma once
#include <unordered_map>
#include <string>
#include "User.h"
#include <memory>
#include <mutex>

class UserManager
{
public:
	static void AddUser(User* user);

	static const std::unordered_map<unsigned long long, User*>& GetUsers();

private:
	static std::unordered_map<unsigned long long, User*> m_connectedUsers;
	static std::mutex m_mutex;
};

