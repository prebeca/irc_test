#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "ACommand.hpp"

#define PRIVMSG_FORMAT(nickname,target,msg) (":" + nickname + " PRIVMSG " + target + " " + msg + CRLF)

class ACommand;

class PRIVMSG: public ACommand
{
public:
	PRIVMSG();
	~PRIVMSG();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PRIVMSG_HPP
