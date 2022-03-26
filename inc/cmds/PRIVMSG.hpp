#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "ACommand.hpp"

#define PRIVMSG_FORMAT(prefix, nickname, target, msg) (":" + prefix + " PRIVMSG " + target + " :" + msg + CRLF)

class ACommand;

class PRIVMSG: public ACommand
{
public:
	PRIVMSG();
	~PRIVMSG();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PRIVMSG_HPP
