#ifndef PING_HPP
#define PING_HPP

#include "ACommand.hpp"

#define PING_MESSAGE(prefix, target, nickname) (":" + prefix + " PONG " + target + " :" + nickname + CRLF)

class ACommand;

class PING: public ACommand
{
public:
	PING();
	~PING();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PING_HPP
