#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"

#include <map>

struct User;

struct Channel
{
	std::map<int, User*> operators;
	std::map<int, User*> users;
};

#endif // CHANNEL_HPP
