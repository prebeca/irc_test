#include "cmds/WHO.hpp"

WHO::WHO()
	: ACommand("WHO")
{
}

WHO::~WHO()
{
}

int WHO::execute(Server &srv, Client &user, const Message &msg) const
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

	Channel* chan = srv.getChannel(msg.getArgv()[1]);
	if (chan == NULL)
		return (1);

	std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
	for (; it != chan->getUsers().end(); ++it)
	{
		std::string ope(" ");
		if (chan->getOpers().find(it->second->getFd()) != chan->getOpers().end())
			ope = "@";
		srv.sendMsg(user.getFd(), Message(RPL_WHOREPLY(user.getNickname(), chan->getName(), it->second->getUsername(), inet_ntoa(it->second->getSocket().sin_addr), it->second->getNickname(), ope, it->second->getRealname())));
		srv.sendMsg(user.getFd(), Message(RPL_ENDOFWHO(user.getNickname(), chan->getName())));
	}

	return (0);
}