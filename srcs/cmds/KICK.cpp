#include "cmds/KICK.hpp"

KICK::KICK()
	: ACommand("KICK")
{
}

KICK::~KICK()
{
}

int KICK::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOTREGISTERED(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() < 3)
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

	if (chan->getOpers().find(user.getFd()) == chan->getOpers().end())
	{
		srv.sendMsg(user.getFd(), Message(ERR_CHANOPRIVSNEEDED(user.getNickname(), chan->getName())));
		return (1);
	}

	Client *target = srv.getUser(msg.getArgv()[2]);
	if (target == NULL)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOSUCHNICK(user.getNickname(), msg.getArgv()[2])));
		return (1);
	}

	std::string kick_msg = "";
	if (msg.getArgv().size() >= 4)
		kick_msg = msg.getArgv()[3];

	std::string chanName = chan->getName();
	std::string targetNickname = target->getNickname();
	const char *rpl[] = {this->name.c_str(), chanName.c_str(), targetNickname.c_str(), kick_msg.c_str(), NULL};

	std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
	for (; it != chan->getUsers().end(); ++it)
		srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));

	chan->removeOper(*target);
	chan->removeUser(*target);
	target->removeChannel(*chan);

	return (0);
}