#include "cmds/PART.hpp"

PART::PART()
	: ACommand("PART")
{
}

PART::~PART()
{
}

int PART::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
	{
		const char *err[] = {ERR_NOTREGISTERED, ":You have not registered", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		const char *err[] = {ERR_NEEDMOREPARAMS, user.getNickname().c_str(), ":Not enough parameters", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	std::string part_msg = "";
	if (msg.getArgv().size() == 3)
		part_msg = msg.getArgv()[2];

	std::vector<std::string> channels;
	channels = utils::split(msg.getArgv()[1], ",");

	for (size_t i = 0; i < channels.size(); ++i)
	{
		Channel *chan = srv.getChannel(channels[i]);

		if (chan == NULL)
		{
			const char *err[] = {ERR_NOSUCHCHANNEL, user.getNickname().c_str(), channels[i].c_str(), ":No such channel", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
			continue;
		}

		if (chan->getUsers().find(user.getFd()) == chan->getUsers().end())
		{
			const char *err[] = {ERR_NOTONCHANNEL, user.getNickname().c_str(), chan->getName().c_str(), ":You are not on that channel", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
			return (1);
		}

		const char *rpl[] = {this->name.c_str(), chan->getName().c_str(), part_msg.c_str(), NULL};

		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));
		
		chan->removeUser(user);
		chan->removeOper(user);
		user.removeChannel(*chan);
	}

	return (0);
}