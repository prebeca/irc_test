#include "Client.hpp"

Client::Client(int fd, sockaddr_in socket)
	: registered(false), pass_check(false), socket(socket), fd(fd)
{
}

Client::~Client()
{
}

// getters/setters

void Client::setNickname(std::string nickname)
{
	this->nickname = nickname;
}

std::string Client::getNickname() const
{
	return (this->nickname);
}

void Client::setUsername(std::string username)
{
	this->username = username;
}

std::string Client::getUsername() const
{
	return (this->username);
}

void Client::setRealname(std::string realname)
{
	this->realname = realname;
}

std::string Client::getRealname() const
{
	return (this->realname);
}

sockaddr_in Client::getSocket() const
{
	return (this->socket);
}

int Client::getFd() const
{
	return (this->fd);
}

void Client::setRegistered(bool is_registered)
{
	this->registered = is_registered;
}

bool Client::isRegistered()
{
	return (this->registered);
}

void Client::setPassCheck(bool is_pass_check)
{
	this->pass_check = is_pass_check;
}

bool Client::isPassCheck()
{
	return (pass_check);
}

void Client::addChannel(Channel &chan)
{
	this->channels.insert(std::make_pair(chan.getName(), &chan));
}

bool Client::removeChannel(const Channel &chan)
{
	return (this->channels.erase(chan.getName()));
}

bool Client::removeChannel(std::string name)
{
	return (this->channels.erase(name));
}

const std::map<std::string, Channel *> &Client::getChannels()
{
	return (this->channels);
}

int Client::addMode(char mode)
{
	if (std::string(VALID_USERMODE).find(mode) == std::string::npos)
		return (1); // invalid usermode
	if (this->mode.find(mode) == std::string::npos)
		this->mode.push_back(mode);
	return (0); // mode added or is already present
}

int Client::removeMode(char mode)
{
	if (std::string(VALID_USERMODE).find(mode) == std::string::npos)
		return (1); // invalid usermode
	if (this->mode.find(mode) != std::string::npos)
		this->mode.erase(this->mode.find(mode), 1);
	return (0); // mode removed or is not present
}

std::string Client::getMode() const
{
	return (this->mode);
}

std::string &Client::getBuffer()
{
	return (buffer);
}