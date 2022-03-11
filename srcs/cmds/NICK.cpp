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
		// srv.removeClient(&user);
		// return (QUIT_RETURN);
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		const char *err[] = {ERR_NONICKNAMEGIVEN, user.getNickname().c_str(), ":No nickname given", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (utils::strToUpper(user.getNickname()) != utils::strToUpper(msg.getArgv()[1]) && srv.getUser(msg.getArgv()[1]) != NULL)
	{
		const char *err[] = {ERR_NICKNAMEINUSE, user.getNickname().c_str(), msg.getArgv()[1].c_str(), ":Nickname is already in use", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (!this->isValidNick(msg.getArgv()[1]))
	{
		const char *err[] = {ERR_ERRONEUSNICKNAME, user.getNickname().c_str(), msg.getArgv()[1].c_str(), ":Erroneous nickname", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	srv.removeUser(user);
	user.setNickname(msg.getArgv()[1]);
	srv.addUser(user);

	srv.sendMsg(user.getFd(), Message(":ircserv " + msg.getRaw()));

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
