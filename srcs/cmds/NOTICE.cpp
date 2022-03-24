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

		std::string	chanName = chan->getName();
		const char *rpl[] = {this->name.c_str(), chanName.c_str(), msg.getArgv()[2].c_str(), NULL};

		std::map<int, Client *> target_list = chan->getUsers();
		target_list.erase(user.getFd());
		std::map<int, Client *>::const_iterator it = target_list.begin();
		for (; it != target_list.end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));
	}
	else
	{
		Client *target_user = srv.getUser(target);
		if (target_user == NULL)
			return (1);

		std::string	targetNickname = target_user->getNickname();
		const char *rpl[] = {this->name.c_str(), targetNickname.c_str(), msg.getArgv()[2].c_str(), NULL};
		srv.sendMsg(target_user->getFd(), Message(user.getNickname(), rpl));
	}
	return (0);
}