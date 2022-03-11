#include <iostream>
#include <string>
#include <cstdlib>

#include "Server.hpp"

#define ERR_USAGE "usage: ircserv [port] [password]"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << ERR_USAGE << std::endl;
		return (1);
	}

	Server ircserv(SERVER_NAME, argv[2], std::atoi(argv[1]));
	ircserv.start();
	return (0);
}