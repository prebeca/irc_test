#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "ACommand.hpp"

class ACommand;

class PRIVMSG: public ACommand
{
public:
	PRIVMSG();
	~PRIVMSG();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PRIVMSG_HPP
