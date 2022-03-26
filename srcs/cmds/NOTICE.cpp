#include "cmds/NOTICE.hpp"

NOTICE::NOTICE()
	: ACommand("NOTICE")
{
}

NOTICE::~NOTICE()
{
}

int NOTICE::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
		return (1);

	if (msg.getArgv().size() < 3)
		return (1); // handle error

	std::string target = msg.getArgv()[1];
	if (target[0] == '#')
	{
		Channel *chan = srv.getChannel(target);
		if (chan == NULL || chan->getUsers().find(user.getFd()) == chan->getUsers().end())
			return (1);

		std::map<int, Client *> target_list = chan->getUsers();
		target_list.erase(user.getFd());
		std::map<int, Client *>::const_iterator it = target_list.begin();
		for (; it != target_list.end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(NOTICE_FORMAT(user.getFullName(), user.getNickname(), chan->getName(), msg.getArgv()[2])));
	}
	else
	{
		Client *target_user = srv.getUser(target);
		if (target_user == NULL)
			return (1);

		srv.sendMsg(target_user->getFd(), Message(NOTICE_FORMAT(user.getFullName(), user.getNickname(), target_user->getNickname(), msg.getArgv()[2])));
	}
	return (0);
}