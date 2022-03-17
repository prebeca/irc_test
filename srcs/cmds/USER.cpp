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
		srv.sendMsg(user.getFd(), Message(ERR_ALREADYREGISTRED(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() < 5)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NEEDMOREPARAMS(user.getNickname(), this->name)));
		return (1);
	}

	user.setUsername(msg.getArgv()[1]);
	user.setRealname(msg.getArgv()[4]);

	if (!user.getNickname().empty())
		srv.registerUser(user);
	return (0);
}