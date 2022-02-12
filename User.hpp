#ifndef USER_HPP
#define USER_HPP

#include "Channel.hpp"

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct Channel;

struct User
{
	bool registered;
	std::string nickname;
	std::string username;
	std::string realname;
	sockaddr_in	socket;

	std::vector<Channel*> channels;
	std::string buffer;
};


#endif // USER_HPP
