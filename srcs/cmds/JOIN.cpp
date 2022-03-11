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
		const char *err[] = {ERR_NOTREGISTERED, ":You have not registered", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		const char *err[] = {ERR_NEEDMOREPARAMS, this->name.c_str(), ":Not enough parameters", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
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
			// handle invite
			const char *err[] = {ERR_INVITEONLYCHAN, chan->getName().c_str(), ":Cannot join channel (+i)", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
			continue;
		}
		// else if ()
		// {
		// check if user is banned
		// }
		else if (chan->getMode().find('k') != std::string::npos && i <= keys.size() && chan->getKey() != keys[i])
		{
			const char *err[] = {ERR_BADCHANNELKEY, chan->getName().c_str(), ":Cannot join channel (+k)", NULL};
			srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
			continue;
		}

		chan->addUser(user);
		user.addChannel(*chan);
		if (created)
			chan->addOper(user);

		const char *rpl[] = {this->name.c_str(), chan->getName().c_str(), NULL};
		
		std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
		for (; it != chan->getUsers().end(); ++it)
			srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));

		sendTopic(srv, user, *chan);
		sendNames(srv, user, *chan);
	}
	return (0);
}

void JOIN::sendTopic(Server &srv, Client &user, Channel& chan) const
{
	const char *rpl_topic[] = {RPL_TOPIC, chan.getName().c_str(), chan.getTopic().c_str(), NULL};
	const char *rpl_notopic[] = {RPL_NOTOPIC, chan.getName().c_str(), ":No topic is set", NULL};
	if (chan.getTopic().empty())
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl_notopic));
	else
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl_topic));
}

void JOIN::sendNames(Server &srv, Client &user, Channel& chan) const
{
	std::stringstream ss;
	ss << ":" << SERVER_NAME << " " << RPL_NAMREPLY << " " << user.getNickname() << " " << chan.getName() << " :";

	std::map<int, Client *>::const_iterator it = chan.getUsers().begin();
	for (; it != chan.getUsers().end(); ++it)
		ss << it->second->getNickname() << " ";
	ss << CRLF;
	srv.sendMsg(user.getFd(), Message(ss.str()));

	const char *rpl_endofnames[] = {RPL_ENDOFNAMES, user.getNickname().c_str(), chan.getName().c_str(), ":End of NAMES list", NULL};
	srv.sendMsg(user.getFd(), Message(SERVER_NAME, rpl_endofnames));
}