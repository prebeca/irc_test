#include "cmds/JOIN.hpp"

JOIN::JOIN()
	: ACommand("JOIN")
{
}

JOIN::~JOIN()
{
}

int JOIN::execute(Server &srv, Client &user, const Message &msg) const
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

	std::vector<std::string> channels;
	channels = utils::split(msg.getArgv()[1], ",");

	std::vector<std::string> keys;
	if (msg.getArgv().size() >= 3)
		keys = utils::split(msg.getArgv()[2], ",");

	for (size_t i = 0; i < channels.size(); ++i)
	{
		bool created = false;
		Channel *chan = srv.getChannel(channels[i]);
		if (chan == NULL)
		{
			chan = srv.addChannel(channels[i]); // if no channel of that name, create one
			created = true;
		}
		else if (chan->getMode().find('i') != std::string::npos)
		{
			// TODO handle invite
			srv.sendMsg(user.getFd(), Message(ERR_INVITEONLYCHAN(user.getNickname(), chan->getName())));
			continue;
		}
		else if (chan->getMode().find('k') != std::string::npos && i <= keys.size() && chan->getKey() != keys[i])
		{
			srv.sendMsg(user.getFd(), Message(ERR_BADCHANNELKEY(user.getNickname(), chan->getName())));
			continue;
		}

		chan->addUser(user);
		user.addChannel(*chan);
		if (created)
			chan->addOper(user);

		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(":" + user.getNickname() + " " + this->name + " " + chan->getName() + CRLF));

		if (!chan->getTopic().empty())
			srv.sendMsg(user.getFd(), Message(RPL_TOPIC(user.getNickname(), chan->getName(), chan->getTopic())));
		
		sendNames(srv, user, *chan);
	}
	return (0);
}

void JOIN::sendNames(Server &srv, Client &user, Channel& chan) const
{
	std::stringstream ss;

	std::map<int, Client *>::const_iterator it = chan.getUsers().begin();
	for (; it != chan.getUsers().end(); ++it)
	{	
		// TODO check if invisible or operator
		ss << " " << it->second->getNickname();
	}
	srv.sendMsg(user.getFd(), Message(RPL_NAMREPLY(user.getNickname(), chan.getName(), ss.str())));
	srv.sendMsg(user.getFd(), Message(RPL_ENDOFNAMES(user.getNickname(), chan.getName())));
}