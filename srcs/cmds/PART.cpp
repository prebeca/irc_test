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
		srv.sendMsg(user.getFd(), Message(ERR_NOTREGISTERED(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NEEDMOREPARAMS(user.getNickname(), this->name)));
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
			srv.sendMsg(user.getFd(), Message(ERR_NOSUCHCHANNEL(user.getNickname(), channels[i])));
			continue;
		}

		if (chan->getUsers().find(user.getFd()) == chan->getUsers().end())
		{
			srv.sendMsg(user.getFd(), Message(ERR_NOTONCHANNEL(user.getNickname(), chan->getName())));
			return (1);
		}

		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(PART_MESSAGE(user.getFullName(), chan->getName(), part_msg)));
		
		chan->removeUser(user);
		chan->removeOper(user);
		user.removeChannel(*chan);

		if (chan->getUsers().empty())
		{
			srv.removeChannel(chan->getName());
			delete chan;
		}
	}
	return (0);
}