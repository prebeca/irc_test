#include "Channel.hpp"

Channel::Channel(std::string name)
	: name(name)
{
}

Channel::~Channel()
{
}

std::string Channel::getName() const
{
	return (this->name);
}

bool Channel::addUser(Client &usr)
{
	return (this->users.insert(std::make_pair(usr.getFd(), &usr)).second);
}

bool Channel::removeUser(Client &usr)
{
	return (this->users.erase(usr.getFd()));
}

const std::map<int, Client *> &Channel::getUsers()
{
	return (this->users);
}

bool Channel::addOper(Client &usr)
{
	return (this->operators.insert(std::make_pair(usr.getFd(), &usr)).second);
}

bool Channel::removeOper(Client &usr)
{
	return (this->operators.erase(usr.getFd()));
}

const std::map<int, Client *> &Channel::getOpers()
{
	return (this->operators);
}

bool Channel::addInvitation(Client &usr)
{
	return (this->invitations.insert(std::make_pair(usr.getFd(), &usr)).second);
}

bool Channel::removeInvitation(Client &usr)
{
	return (this->invitations.erase(usr.getFd()));
}

const std::map<int, Client *> &Channel::getInvitations()
{
	return (this->invitations);
}

void Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

std::string Channel::getTopic() const
{
	return (this->topic);
}

void Channel::setKey(std::string key)
{
	this->key = key;
}

std::string Channel::getKey() const
{
	return (this->key);
}

int Channel::addMode(char mode)
{
	if (std::string(VALID_USERMODE).find(mode) == std::string::npos)
		return (1); // invalid usermode
	if (this->mode.find(mode) == std::string::npos)
		this->mode.push_back(mode);
	return (0); // mode added or is already present
}

int Channel::removeMode(char mode)
{
	if (std::string(VALID_USERMODE).find(mode) == std::string::npos)
		return (1); // invalid usermode
	if (this->mode.find(mode) != std::string::npos)
		this->mode.erase(this->mode.find(mode), 1);
	return (0); // mode removed or is not present
}

std::string Channel::getMode() const
{
	return (this->mode);
}