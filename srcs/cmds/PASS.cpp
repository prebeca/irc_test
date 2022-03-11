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
	if (user.isRegistered())
	{
		const char *err[] = {ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		return (1);
	}

	if (msg.getArgv().size() < 2)
	{
		const char *err[] = {ERR_NEEDMOREPARAMS, msg.getArgv()[0].c_str(), NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		// srv.removeClient(&user);
		// return (QUIT_RETURN);
		return (1);
	}

	if (!srv.getPassword().empty() && srv.getPassword().compare(msg.getArgv()[1]) != 0)
	{
		const char *err[] = {ERR_PASSWDMISMATCH, msg.getArgv()[0].c_str(), ":Password incorrect", NULL};
		srv.sendMsg(user.getFd(), Message(SERVER_NAME, err));
		// srv.removeClient(&user);
		// return (QUIT_RETURN);
		return (1);
	}

	user.setPassCheck(true);

	return (0);
}