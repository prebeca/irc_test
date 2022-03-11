#include "cmds/USER.hpp"

USER::USER()
	: ACommand("USER")
{
}

USER::~USER()
{
}

int USER::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!srv.getPassword().empty() && !user.isPassCheck())
	{
		srv.removeClient(&user);
		return (QUIT_RETURN);
	}

	if (user.isRegistered())
	{
		const char *err[] = {ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 5)
	{
		const char *err[10];
		size_t i = 0;
		err[i++] = ERR_NEEDMOREPARAMS;
		err[i++] = user.getNickname().c_str();
		for (; i < msg.getArgv().size(); ++i)
			err[i] = msg.getArgv()[i].c_str();
		err[i++] = ":Not enough parameters";
		err[i] = NULL;
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	user.setUsername(msg.getArgv()[1]);
	user.setRealname(msg.getArgv()[4]);

	// srv.sendMsg(user.getFd(), Message(":ircserv " + msg.getRaw())); // not sure if reply is needed here
	
	if (!user.getNickname().empty())
		srv.registerUser(user);
	return (0);
}