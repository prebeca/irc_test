#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"

#include <map>

struct User;

struct Channel
{
	std::string				name;
	std::map<int, User*>	operators;
	std::map<int, User*>	users;
	std::string				topic;
};

#endif // CHANNEL_HPP
