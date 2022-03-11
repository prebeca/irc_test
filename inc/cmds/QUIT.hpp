#ifndef QUIT_HPP
#define QUIT_HPP

#include "ACommand.hpp"

class ACommand;

class QUIT: public ACommand
{
public:
	QUIT();
	~QUIT();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // QUIT_HPP
