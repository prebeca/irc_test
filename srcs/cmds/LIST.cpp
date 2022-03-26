#include "cmds/LIST.hpp"

LIST::LIST()
	: ACommand("LIST")
{
}

LIST::~LIST()
{
}

int LIST::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOTREGISTERED(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() == 1)
	{
		std::map<std::string, Channel *> channels = srv.getChannels();
		std::map<std::string, Channel *>::iterator it = channels.begin();
		std::map<std::string, Channel *>::iterator ite = channels.end();

		for (; it != ite; ++it)
		{
			std::stringstream nb_users;
			nb_users << it->second->getUsers().size();
			srv.sendMsg(user.getFd(), Message(RPL_LIST(user.getNickname(), it->second->getName(), nb_users.str(), it->second->getTopic())));
		}
	}
	else
	{
		std::vector<std::string> channels;
		channels = utils::split(msg.getArgv()[1], ",");

		for (size_t i = 0; i < channels.size(); ++i)
		{
			Channel *chan = srv.getChannel(channels[i]);
			if (chan != NULL)
			{
				std::stringstream nb_users;
				nb_users << chan->getUsers().size();
				srv.sendMsg(user.getFd(), Message(RPL_LIST(user.getNickname(), chan->getName(), nb_users.str(), chan->getTopic())));
			}
		}
	}

	srv.sendMsg(user.getFd(), Message(RPL_LISTEND(user.getNickname())));

	return 0;
}