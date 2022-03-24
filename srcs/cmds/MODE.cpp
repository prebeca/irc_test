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

	if (msg.getArgv().size() < 2)
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
	Channel*	channel = srv.getChannel(msg.getArgv()[1]);

	if (!channel)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOSUCHCHANNEL(user.getNickname(), msg.getArgv()[1])));
		return 1;
	}

	if (msg.getArgv().size() == 2)
	{
		srv.sendMsg(user.getFd(), Message(RPL_CHANNELMODEIS(user.getNickname(), channel->getName(), channel->getMode())));
		return (1);
	}
	if (channel->getOpers().find(user.getFd()) == channel->getOpers().end())
	{
		srv.sendMsg(user.getFd(), Message(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel->getName())));
		return 1;
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
		if (flag[i] == 'i' || flag[i] == 'm')
		{
			if (add_mode == "+")
				channel->addMode(flag[i]);
			else
				channel->removeMode(flag[i]);
		}
		else if (flag[i] == 'k')
		{
			if (add_mode == "+")
			{
				if (msg.getArgv().size() < 4)
				{
					srv.sendMsg(user.getFd(), Message(ERR_NEEDMOREPARAMS(user.getNickname(), this->name)));
					return (1);
				}
				channel->setKey(msg.getArgv()[3]);
				channel->addMode(flag[i]);
			}
			else if (add_mode == "-")
			{
				channel->setKey("");
				channel->removeMode(flag[i]);
			}
			else
				continue;
		}
		else if (flag[i] == 'o')
		{
			if (msg.getArgv().size() < 4)
			{
				srv.sendMsg(user.getFd(), Message(ERR_NEEDMOREPARAMS(user.getNickname(), this->name)));
				return (1);
			}

			Client*	target;
			target = srv.getUser(msg.getArgv()[3]);

			if (!target)
			{
				srv.sendMsg(user.getFd(), Message(ERR_NOSUCHNICK(user.getNickname(), msg.getArgv()[1])));
				return 1;
			}
			if (channel->getUsers().find(target->getFd()) == channel->getUsers().end())
			{
				srv.sendMsg(user.getFd(), Message(ERR_NOTONCHANNEL(target->getNickname(), channel->getName())));
				return 1;
			}

			if (add_mode == "+")
			{
				if (channel->getOpers().find(target->getFd()) == channel->getOpers().end())
					channel->addOper(*target);
				else
					continue;
			}
			else
			{
				if (channel->getOpers().find(target->getFd()) != channel->getOpers().end())
					channel->removeOper(*target);
				else
					continue;
			}
			
		}
		else
		{
			srv.sendMsg(user.getFd(), Message(ERR_UMODEUNKNOWNFLAG(user.getNickname())));
			continue;
		}
		std::string s_flag = flag.substr(0, 1) + flag.substr(i, 1);

		std::map<int, Client *>::const_iterator it = channel->getUsers().begin();

		if (msg.getArgv().size() == 3)
			for (; it != channel->getUsers().end(); ++it)
				srv.sendMsg(it->second->getFd(), Message(":" + user.getNickname() + " MODE " + channel->getName() + " " + add_mode + flag[i] + CRLF));
		else if (msg.getArgv().size() == 4)
			for (; it != channel->getUsers().end(); ++it)
				srv.sendMsg(it->second->getFd(), Message(":" + user.getNickname() + " MODE " + channel->getName() + " " + add_mode + flag[i] + " " + msg.getArgv()[3] + CRLF));
	}

	return (0);
}