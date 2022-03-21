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

		std::string	chanName = chan->getName();
		const char *rpl[] = {this->name.c_str(), chanName.c_str(), part_msg.c_str(), NULL};

		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));
		
		chan->removeUser(user);
		chan->removeOper(user);
		user.removeChannel(*chan);
	}
	// to do supprimer chanel si lus d'user
	return (0);
}