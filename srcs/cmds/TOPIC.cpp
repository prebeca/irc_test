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
		const char *err[] = {ERR_NOTREGISTERED, ":You have not registered", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		const char *err[] = {ERR_NEEDMOREPARAMS, msg.getArgv()[0].c_str(), NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	Channel *chan = srv.getChannel(msg.getArgv()[1]);
	if (chan == NULL)
	{
		const char *err[] = {ERR_NOSUCHCHANNEL, user.getNickname().c_str(), msg.getArgv()[1].c_str(), ":No such channel", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (chan->getUsers().find(user.getFd()) == chan->getUsers().end())
	{
		const char *err[] = {ERR_NOTONCHANNEL, user.getNickname().c_str(), chan->getName().c_str(), ":You are not on that channel", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() == 2)
	{
		const char *rpl_topic[] = {RPL_TOPIC, chan->getName().c_str(), chan->getTopic().c_str(), NULL};
		const char *rpl_notopic[] = {RPL_NOTOPIC, chan->getName().c_str(), ":No topic is set", NULL};
		if (chan->getTopic().empty())
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl_notopic));
		else
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl_topic));
	}
	else
	{
		if (chan->getOpers().find(user.getFd()) == chan->getOpers().end())
		{
			const char *err[] = {ERR_CHANOPRIVSNEEDED, user.getNickname().c_str(), chan->getName().c_str(), ":You're not channel operator", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
			return (1);
		}
		chan->setTopic(msg.getArgv()[2]);
		const char *rpl_topic[] = {RPL_TOPIC, user.getNickname().c_str(), chan->getName().c_str(), chan->getTopic().c_str(), NULL};
		srv.sendMsg(user.getFd(), Message(user.getNickname(), rpl_topic));
	}
	return (0);
}
