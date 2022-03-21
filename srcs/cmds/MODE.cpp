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

	bool add_mode = false;
	if (flag[0] == '+')
		add_mode = true;
	else if (flag[0] == '-')
		add_mode = false;
	else
	{
		srv.sendMsg(user.getFd(), Message(ERR_UMODEUNKNOWNFLAG(user.getNickname())));
		return (1);
	}

	for (size_t i = 1; i < flag.size(); ++i)
	{
		if (flag[i] == 'O' || flag[i] == 'o')
		{
			if (add_mode)
				continue;
			else
				user.removeMode(flag[i]);
		}
		else if (flag[i] == 'a')
			continue;
		else if (flag[i] == 'r')
		{
			if (add_mode)
				user.addMode(flag[i]);
			else
				continue;
		}
		else
		{
			int ret = 0;
			if (add_mode)
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
		const char *rpl[] = {this->name.c_str(), userNickname.c_str(), s_flag.c_str(), NULL};
	
		std::map<std::string, Channel *>::const_iterator it = user.getChannels().begin();
		for (; it != user.getChannels().end(); ++it)
		{
			Channel *chan = it->second;
			std::map<int, Client *>::const_iterator it = chan->getUsers().begin();
			for (; it != chan->getUsers().end(); ++it)
				srv.sendMsg(it->second->getFd(), Message(user.getNickname(), rpl));
		}
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