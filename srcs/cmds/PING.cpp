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
	std::string nickname = user.getNickname();
	const char *rpl[] = {"PONG", nickname.c_str(), NULL};
	srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl));
	return (0);
}