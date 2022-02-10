// #include "ft_irc.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

#include "Server.hpp"

#define	ERR_USAGE "usage: ircserv [port] [password]"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << ERR_USAGE << std::endl;
		return (1);
	}

	Server ircserv(argv[0], argv[2] , std::atoi(argv[1]));
	ircserv.run();
	
	// if (argc != 3)
	// return(1);

	// int fd = socket(AF_INET, SOCK_STREAM, 0);			// create socket
	// if (fd == -1)
	// 	return(1);
	// // std::cout << "fd = " << fd << std::endl;

	// // create a socketaddr_in to bind the address to the socket
	// sockaddr_in hint;
	// hint.sin_family = AF_INET;
	// hint.sin_port = htons(atoi(argv[1]));				// address to bind
	// hint.sin_addr.s_addr = INADDR_ANY;					// give it any address

	// if (bind(fd, (sockaddr*)&hint, sizeof(hint)) == -1)	// bind socket to a port
	// {
	// 	std::cout << strerror(errno) << std::endl;
	// 	return(1);
	// }
	
	// // std::cout << "server addr = " << inet_ntoa(hint.sin_addr) << std::endl;

	// listen(fd, SOMAXCONN);								// listen to the socket

	// // (void)client_fd;

	// // char client_name[NI_MAXHOST];
	// // char client_port[NI_MAXSERV];
	// // memset(client_name, 0, NI_MAXHOST);
	// // memset(client_port, 0, NI_MAXSERV);


	// fd_set master;
	// FD_ZERO(&master);
	// FD_SET(fd, &master);

	// std::map<int, User> user_list;

	// while (true)
	// {
	// 	fd_set cpy = master;
	// 	int sock_ready = select(FD_SETSIZE, &cpy, NULL, NULL, NULL);
	// 	if (sock_ready == -1)
	// 	{
	// 		std::cerr << "error: select" << std::endl;
	// 		exit(-1);
	// 	}
		
	// 	// std::cout << "sockets ready = " << sock_ready << std::endl;

	// 	for (int i = 0; i < sock_ready; ++i)
	// 	{
	// 		int curr_fd = 0;
	// 		while (!FD_ISSET(curr_fd, &cpy))
	// 			++curr_fd;
	// 		FD_CLR(curr_fd, &cpy);

	// 		if (curr_fd == fd)
	// 		{
	// 			sockaddr_in client;
	// 			socklen_t client_len = sizeof(client);

	// 			int client_fd = accept(fd, (sockaddr*)&client, &client_len);
	// 			FD_SET(client_fd, &master);
	// 			std::cout << "new client = " << inet_ntoa(client.sin_addr) << std::endl;
	// 			user_list.insert(std::make_pair(client_fd, User()));
	// 			user_list[client_fd].registered = false;
	// 		}
	// 		else
	// 		{
	// 			char buf[BUFFER_SIZE];
	// 			memset(buf, 0, BUFFER_SIZE);
	// 			int bit_read = recv(curr_fd, buf, BUFFER_SIZE, 0);
	// 			if (bit_read <= 0)
	// 			{
	// 				close(curr_fd);
	// 				FD_CLR(curr_fd, &master);
	// 				std::cout << "a client quit" << std::endl;
	// 			}
	// 			else
	// 			{
	// 				std::cout << "new msg = " << buf << std::endl;
	// 				// std::string token = msg.substr(0, msg.find(" "));
	// 				if (std::strncmp(buf,"CAP ", 4) == 0 || std::strncmp(buf,"USER ", 5) == 0 || std::strncmp(buf,"NICK ", 5) == 0)
	// 				{
	// 					std::string msg = buf;
	// 					std::string cmd = msg.substr(0, msg.find(" "));
	// 					msg.erase(0, msg.find(" ") + 1);
	// 					msg.erase(msg.find("\n"), 1);
	// 					if (std::strncmp(buf,"USER", 4) == 0)
	// 					{
	// 						std::cout << "change username from \"" << user_list[curr_fd].username << "\" to \"" << msg << "\"" << std::endl;
	// 						user_list[curr_fd].username = msg;
	// 						user_list[curr_fd].realname = msg;
	// 					}
	// 					if (std::strncmp(buf,"NICK", 4) == 0)
	// 					{
	// 						std::cout << "change username from \"" << user_list[curr_fd].nickname << "\" to \"" << msg << "\"" << std::endl;
	// 						user_list[curr_fd].nickname = msg;
	// 					}

	// 					if (user_list[curr_fd].nickname != "" && user_list[curr_fd].username != "")
	// 					{
	// 						std::stringstream ss;
	// 						ss << "RPL_WELCOME \"Welcome to my IRC test server!\"" << std::endl;
	// 						std::cout << "RPL send" << std::endl;
	// 						send(curr_fd, ss.str().c_str(), ss.str().length(), 0);
	// 					}
	// 				}
	// 				else
	// 					for(int j = 0; j < FD_SETSIZE; ++j)
	// 					{
	// 						if (FD_ISSET(j, &master) && j != fd && j != curr_fd)
	// 							send(j, buf, bit_read, 0);
	// 					}
	// 			}
	// 		}
	// 		// static int loop_count = 0;
	// 		// std::cout << "looping " << loop_count++ << std::endl;
	// 	}
	// }

	// close(fd);
	// // close(client_fd);

	return (0);
}