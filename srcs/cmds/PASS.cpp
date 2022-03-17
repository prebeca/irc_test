#include "cmds/PASS.hpp"

PASS::PASS()
	: ACommand("PASS")
{
}

PASS::~PASS()
{
}

int PASS::execute(Server &srv, Client &user, const Message &msg) const
{
	if (user.isRegistered() || user.isPassCheck())
	{
		srv.sendMsg(user.getFd(), Message(ERR_ALREADYREGISTRED(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NEEDMOREPARAMS(user.getNickname(), this->name)));
		srv.removeClient(&user);
		return (QUIT_RETURN);
	}

	if (!srv.getPassword().empty() && srv.getPassword().compare(msg.getArgv()[1]) != 0)
	{
		srv.sendMsg(user.getFd(), Message(ERR_PASSWDMISMATCH(user.getNickname())));
		srv.removeClient(&user);
		return (QUIT_RETURN);
	}

	user.setPassCheck(true);

	return (0);
}