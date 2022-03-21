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

	if (msg.getArgv().size() == 1) {
		std::map<std::string, Channel *>			channels = srv.getChannels();
		std::map<std::string, Channel *>::iterator	it = channels.begin();
		std::map<std::string, Channel *>::iterator	ite = channels.end();

		for (; it != ite; ++it) {
			std::stringstream	ss;
			ss << "322 * " << it->second->getName() << " " << it->second->getUsers().size() << " " << it->second->getTopic() << CRLF;
			std::string reply = ss.str();
			srv.sendMsg(user.getFd(), Message(reply));
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
				std::stringstream	ss;
				ss << "322 * " << chan->getName() << " " << chan->getUsers().size() << " " << chan->getTopic() << CRLF;
				std::string reply = ss.str();
				srv.sendMsg(user.getFd(), Message(reply));
			}

		}
	}
	
	srv.sendMsg(user.getFd(), Message("323 * :End of LIST\r\n"));

	return 0;
}