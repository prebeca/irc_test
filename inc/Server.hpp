#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>

#include <vector>
#include <map>

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "CmdHandler.hpp"
#include "numeric_replies.h"
#include "utils.h"

#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_RESET "\033[0m"

#define SERVER_NAME "IRCserv"
#define VERSION "0.1"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

class CmdHandler;

class Server
{
public:
	Server(std::string name, std::string password = "", unsigned int port = 6667, std::string config_file = "");
	~Server();

	std::string getPassword() const;
	std::string getName() const;
	unsigned int getPort() const;

	int start();
	int stop();

	Client *addUser(Client &new_user);
	int registerUser(Client &user);

	Client *getUser(int user_fd) const;
	Client *getUser(std::string nickname) const;

	void removeUser(Client &user);
	void removeUser(int user_fd);
	void removeUser(std::string nickname);

	void removeClient(Client *user, int fd_index = -1);

	Channel *addChannel(std::string name);
	int removeChannel(std::string name);
	Channel *getChannel(std::string name);

	const std::map<std::string, Channel *> &getChannels() const;

	int sendMsg(int fd, const Message &msg) const;

private:
	Server();
	Server(const Server &);
	const Server &operator=(const Server &);

	int init(std::string config_file);

	int receiveMsg(int fd, int index);
	int handleCmd(int user_fd, const Message &msg);
	int acceptClient();

	std::string config_file;

	std::string name;
	std::string password;
	unsigned int port;

	CmdHandler* cmd_handler;

	std::vector<pollfd> fd_lst;
	std::map<int, Client *> user_by_fd;
	std::map<std::string, Client *> user_by_nick;
	std::map<std::string, Channel *> chan_lst;
};

#endif // SERVER_HPP
