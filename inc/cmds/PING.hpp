#ifndef PING_HPP
#define PING_HPP

#include "ACommand.hpp"

class ACommand;

class PING: public ACommand
{
public:
	PING();
	~PING();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PING_HPP
