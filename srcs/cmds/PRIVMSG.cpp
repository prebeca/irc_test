#include "cmds/PRIVMSG.hpp"

PRIVMSG::PRIVMSG()
	: ACommand("PRIVMSG")
{
}

PRIVMSG::~PRIVMSG()
{
}

int PRIVMSG::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
	{
		const char *err[] = {ERR_NOTREGISTERED, ":You have not registered", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 3)
		return (1); // handle error

	std::string target = msg.getArgv()[1];
	if (target[0] == '#')
	{
		Channel *chan = srv.getChannel(target);
		if (chan == NULL || chan->getUsers().find(user.getFd()) == chan->getUsers().end())
		{
			const char *rpl[] = {ERR_CANNOTSENDTOCHAN, user.getNickname().c_str(), target.c_str(), ":Cannot send to channel", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl));
			return (1);
		}

		const char *rpl[] = {this->name.c_str(), chan->getName().c_str(), msg.getArgv()[2].c_str(), NULL};

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
		{
			const char *rpl[] = {ERR_NOSUCHNICK, user.getNickname().c_str(), target.c_str(), ":No such nick", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl));
			return (1);
		}

		// RPL_AWAY
		const char *rpl[] = {this->name.c_str(), target_user->getNickname().c_str(), msg.getArgv()[2].c_str(), NULL};
		srv.sendMsg(target_user->getFd(), Message(user.getNickname(), rpl));
	}
	return (0);
}