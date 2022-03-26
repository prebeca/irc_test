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

	std::string quit_msg = "";
	if (msg.getArgv().size() >= 2)
		quit_msg = msg.getArgv()[1];

	const char *acknowledge[] = {"ERROR", "QUIT command acknowledge", NULL};
	srv.sendMsg(user.getFd(), Message(SERVER_NAME, acknowledge));

	std::map<std::string, Channel *>::const_iterator it = user.getChannels().begin();
	for (; it != user.getChannels().end(); ++it)
	{
		Channel *chan = it->second;

		chan->removeUser(user);
		chan->removeOper(user);
		
		if (chan->getUsers().empty())
		{
			srv.removeChannel(chan->getName());
			delete chan;
			continue;
		}

		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(QUIT_MESSAGE(user.getFullName(), quit_msg)));
	}

	std::cout << COLOR_GREEN << "Client quit: " << inet_ntoa(user.getSocket().sin_addr) << COLOR_RESET << std::endl;
	srv.removeClient(&user);
	
	return (QUIT_RETURN);
}