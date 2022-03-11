#include "cmds/QUIT.hpp"

QUIT::QUIT()
	: ACommand("QUIT")
{
}

QUIT::~QUIT()
{
}

int QUIT::execute(Server &srv, Client &user, const Message &msg) const
{
	std::map<std::string, Channel *>::const_iterator it = user.getChannels().begin();

	std::string quit_msg = "";
	if (msg.getArgv().size() >= 2)
		quit_msg = msg.getArgv()[1];

	const char *acknowledge[] = {"ERROR", "QUIT command acknowledge", NULL};
	srv.sendMsg(user.getFd(), Message(SERVER_NAME, acknowledge));

	if (!user.getChannels().empty())
	{

		for (; it != user.getChannels().end(); ++it)
		{
			Channel *chan = it->second;

			chan->removeUser(user);
			chan->removeOper(user);

			const char *rpl[] = {this->name.c_str(), quit_msg.c_str(), NULL};

			std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
			for (; it != chan->getUsers().end(); ++it)
				srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));
		}
	}
	srv.removeClient(&user);
	return (QUIT_RETURN);
}