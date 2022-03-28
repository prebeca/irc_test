#include "cmds/TOPIC.hpp"

TOPIC::TOPIC()
	: ACommand("TOPIC")
{
}

TOPIC::~TOPIC()
{
}

int TOPIC::execute(Server &srv, Client &user, const Message &msg) const
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

	Channel *chan = srv.getChannel(msg.getArgv()[1]);
	if (chan == NULL)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOSUCHCHANNEL(user.getNickname(), msg.getArgv()[1])));
		return (1);
	}

	if (chan->getUsers().find(user.getFd()) == chan->getUsers().end())
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOTONCHANNEL(user.getNickname(), chan->getName())));
		return (1);
	}

	if (msg.getArgv().size() == 2)
	{
		if (chan->getTopic().empty())
			srv.sendMsg(user.getFd(), Message(RPL_NOTOPIC(user.getNickname(), chan->getName())));
		else
			srv.sendMsg(user.getFd(), Message(RPL_TOPIC(user.getNickname(), chan->getName(), chan->getTopic())));
	}
	else
	{
		if (chan->getOpers().find(user.getFd()) == chan->getOpers().end())
		{
			srv.sendMsg(user.getFd(), Message(ERR_CHANOPRIVSNEEDED(user.getNickname(), chan->getName())));
			return (1);
		}

		chan->setTopic(msg.getArgv()[2]);

		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(":" + user.getFullName() + " " + msg.getRaw()));
	}
	return (0);
}
