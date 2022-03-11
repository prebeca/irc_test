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
		const char *err[] = {ERR_NOTREGISTERED, ":You have not registered", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 3)
	{
		const char *err[] = {ERR_NEEDMOREPARAMS, this->name.c_str(), ":Not enough parameters", NULL};
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

	if (chan->getOpers().find(user.getFd()) == chan->getOpers().end())
	{
		const char *err[] = {ERR_CHANOPRIVSNEEDED, user.getNickname().c_str(), chan->getName().c_str(), ":You're not channel operator", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	Client *target = srv.getUser(msg.getArgv()[2]);
	if (target == NULL)
	{
		const char *rpl[] = {ERR_NOSUCHNICK, user.getNickname().c_str(), msg.getArgv()[2].c_str(), ":No such nick", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl));
		return (1);
	}

	std::string kick_msg = "";
	if (msg.getArgv().size() >= 4)
		kick_msg = msg.getArgv()[3];

	const char *rpl[] = {this->name.c_str(), chan->getName().c_str(), target->getNickname().c_str(), kick_msg.c_str(), NULL};

	std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
	for (; it != chan->getUsers().end(); ++it)
		srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));

	chan->removeOper(*target);
	chan->removeUser(*target);
	target->removeChannel(*chan);

	return (0);
}