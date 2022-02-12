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

		std::string		name;
		std::string		password;
		unsigned int	port;

		std::vector<pollfd>	fd_lst;
		std::map<int, User>	user_lst;
		std::map<std::string, Channel> chan_lst;

	private:
		Server();
		Server(const Server&);
		const Server& operator=(const Server&);

		int setup();
		int start();
		
		int		receiveMsg(int index);
		void	handleCmd(int user_fd, const Message& msg);
		int		acceptClient();

};

#endif // SERVER_HPP
