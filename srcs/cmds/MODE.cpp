#include "cmds/MODE.hpp"

MODE::MODE()
	: ACommand("MODE")
{
}

MODE::~MODE()
{
}

int MODE::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOTREGISTERED(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() < 3)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NEEDMOREPARAMS(user.getNickname(), this->name)));
		return (1);
	}

	if (msg.getArgv()[1][0] == '#')
		chanMode(srv, user, msg);
	else
		userMode(srv, user, msg);

	return (0);
}

int MODE::userMode(Server &srv, Client &user, const Message &msg) const
{
	if (user.getNickname() != msg.getArgv()[1])
	{
		srv.sendMsg(user.getFd(), Message(ERR_USERSDONTMATCH(user.getNickname())));
		return (1);
	}

	if (msg.getArgv().size() == 2)
	{
		srv.sendMsg(user.getFd(), Message(RPL_UMODEIS(user.getNickname(), user.getMode())));
		return (1);
	}

	std::string flag = msg.getArgv()[2];

	std::string add_mode;
	if (flag[0] == '+')
		add_mode = "+";
	else if (flag[0] == '-')
		add_mode = "-";
	else
	{
		srv.sendMsg(user.getFd(), Message(ERR_UMODEUNKNOWNFLAG(user.getNickname())));
		return (1);
	}

	for (size_t i = 1; i < flag.size(); ++i)
	{
		if (flag[i] == 'O' || flag[i] == 'o')
		{
			if (add_mode == "+")
				continue;
			else
				user.removeMode(flag[i]);
		}
		else if (flag[i] == 'a')
			continue;
		else if (flag[i] == 'r')
		{
			if (add_mode == "+")
				user.addMode(flag[i]);
			else
				continue;
		}
		else
		{
			int ret = 0;
			if (add_mode == "+")
				ret = user.addMode(flag[i]);
			else
				ret = user.removeMode(flag[i]);
			if (ret)
			{
				srv.sendMsg(user.getFd(), Message(ERR_UMODEUNKNOWNFLAG(user.getNickname())));
				continue;
			}
		}
		std::string s_flag = flag.substr(0, 1) + flag.substr(i, 1);
		std::string	userNickname = user.getNickname();

		srv.sendMsg(user.getFd(), Message(":" + (std::string)SERVER_NAME + " MODE " + user.getNickname() + " " + add_mode + flag[i] + CRLF));
	}
	return (0);
}

int MODE::chanMode(Server &srv, Client &user, const Message &msg) const
{
	(void)srv;
	(void)user;
	(void)msg;
	return (0);
}