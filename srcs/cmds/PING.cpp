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
	srv.sendMsg(user.getFd(), Message(PING_MESSAGE((std::string)SERVER_NAME, msg.getArgv()[1], user.getNickname())));
	return (0);
}