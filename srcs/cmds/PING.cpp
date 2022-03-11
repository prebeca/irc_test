#include "cmds/PING.hpp"

PING::PING()
	: ACommand("PING")
{
}

PING::~PING()
{
}

int PING::execute(Server &srv, Client &user, const Message &msg) const
{
	(void)msg;
	const char *rpl[] = {"PONG", user.getNickname().c_str(), NULL};
	srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl));
	return (0);
}