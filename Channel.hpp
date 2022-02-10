#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"

#include <vector>

struct User;

struct Channel
{
	std::vector<User*> operators;
	std::vector<User*> users;
};

#endif // CHANNEL_HPP
