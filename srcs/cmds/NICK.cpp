#include "cmds/NICK.hpp"

NICK::NICK()
	: ACommand("NICK")
{
}

NICK::~NICK()
{
}

int NICK::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!srv.getPassword().empty() && !user.isPassCheck())
	{
		srv.removeClient(&user);
		return (QUIT_RETURN);
	}

	if (msg.getArgv().size() < 2)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NONICKNAMEGIVEN(user.getNickname())));
		return (1);
	}

	if (utils::strToUpper(user.getNickname()) != utils::strToUpper(msg.getArgv()[1]) && srv.getUser(msg.getArgv()[1]) != NULL)
	{
		std::string usernick = user.getNickname();
		if (usernick.empty())
			usernick = "*";
		srv.sendMsg(user.getFd(), Message(ERR_NICKNAMEINUSE(usernick, msg.getArgv()[1])));
		return (1);
	}

	if (!this->isValidNick(msg.getArgv()[1]))
	{
		srv.sendMsg(user.getFd(), Message(ERR_ERRONEUSNICKNAME(user.getNickname(), msg.getArgv()[1])));
		return (1);
	}

	std::map<std::string, Channel*>::const_iterator it_chan = user.getChannels().begin();
	for (; it_chan != user.getChannels().end(); ++it_chan)
	{
		std::map<int, Client*> list = it_chan->second->getUsers();
		std::map<int, Client *>::const_iterator it_user = list.begin();
		for (; it_user != list.end(); ++it_user)
			srv.sendMsg(it_user->second->getFd(), Message(":" + user.getNickname() + "!" + (std::string)SERVER_NAME + " " + this->name + " " + msg.getArgv()[1] + CRLF));
	}

	srv.removeUser(user);
	user.setNickname(msg.getArgv()[1]);
	srv.addUser(user);

	if (!user.isRegistered() && !user.getUsername().empty())
		srv.registerUser(user);
	return (0);
}

bool NICK::isValidNick(std::string nick) const
{
	if (nick.size() > 9)
		return (false);
	if (!std::isalpha(nick[0]) && !std::strchr(SPECIAL, nick[0]))
		return (false);
	if (nick.find_first_not_of(VALID) != std::string::npos)
		return (false);
	return (true);
}
