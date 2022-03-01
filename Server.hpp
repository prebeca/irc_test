#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Message.hpp"
#include "CmdHandler.hpp"

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

#define COLOR_RED		"\x1B[31m"
#define COLOR_GREEN		"\x1B[32m"
#define COLOR_YELLOW	"\x1B[33m"
#define COLOR_RESET		"\033[0m"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE		4096
#endif

class Server
{
	public:

		Server(std::string name, std::string password, unsigned int port);
		~Server();
		
		int run();
		int stop();
		
		User*	addUser(User& new_user);
		void	removeUser(User& new_user);
		User*	getUser(int user_fd) const;
		User*	getUser(std::string nickname) const;

		Channel*	createChannel(std::string name, User* creator);
		Channel*	getChannel(std::string name);

		std::string		getPassword() const;
		std::string		getName() const;
		unsigned int	getPort() const;


	private:
		Server();
		Server(const Server&);
		const Server& operator=(const Server&);

		int setup();
		int start();
		
		int		receiveMsg(int fd, int index);
		void	handleCmd(int user_fd, const Message& msg);
		int		acceptClient();

		std::string		name;
		std::string		password;
		unsigned int	port;

		std::vector<pollfd>				fd_lst;
		std::map<int, User*>				user_by_fd;
		std::map<std::string, User*>		user_by_nick;
		std::map<std::string, Channel*>	chan_lst;
};

#endif // SERVER_HPP
