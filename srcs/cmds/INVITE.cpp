#include "cmds/INVITE.hpp"

INVITE::INVITE()
	: ACommand("INVITE")
{
}

INVITE::~INVITE()
{
}

int INVITE::execute(Server &srv, Client &user, const Message &msg) const
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

	Channel*	channel;
	channel = srv.getChannel(msg.getArgv()[2]);

	if (!channel)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOSUCHCHANNEL(user.getNickname(), msg.getArgv()[2])));
		return 1;	
	}
	if (channel->getUsers().find(user.getFd()) == channel->getUsers().end())
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOTONCHANNEL(user.getNickname(), channel->getName())));
		return 1;
	}
	if (channel->getOpers().find(user.getFd()) == channel->getOpers().end())
	{
		srv.sendMsg(user.getFd(), Message(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel->getName())));
		return (1);
	}

	Client*	target;
	target = srv.getUser(msg.getArgv()[1]);

	if (!target)
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOSUCHNICK(user.getNickname(), msg.getArgv()[1])));
		return 1;
	}
	if (channel->getUsers().find(target->getFd()) != channel->getUsers().end())
	{
		srv.sendMsg(user.getFd(), Message(ERR_USERONCHANNEL(user.getNickname(), target->getNickname(), channel->getName())));
		return 1;
	}

	channel->addInvitation(*target);

	std::string replyTarget;
	replyTarget = ":" + user.getNickname() + "!" + (std::string)SERVER_NAME + " " + this->name + " " + target->getNickname() + " :" + channel->getName() + CRLF;
	srv.sendMsg(target->getFd(), Message(replyTarget));

	std::string	replySender;
	replySender = ":" + user.getNickname() + " 341 " + user.getNickname() + " " + target->getNickname() + " :" + channel->getName() + CRLF;
	srv.sendMsg(user.getFd(), Message(replySender));

	return (0);
}