#ifndef KICK_HPP
#define KICK_HPP

#include "ACommand.hpp"

#define KICK_MESSAGE(prefix, channel, target, message) (":" + prefix + " KICK " + channel + " " + target + " :" + message + CRLF)

class ACommand;

class KICK: public ACommand
{
public:
	KICK();
	~KICK();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // KICK_HPP
