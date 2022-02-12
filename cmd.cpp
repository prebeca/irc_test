#include "CmdHandler.hpp"

void registerUser(int fd, User& usr)
{
	usr.registered = true;
	std::stringstream ss;

	ss << ":" << "ircserv" << " " << RPL_WELCOME << " " << usr.nickname << " " << RPL_WELCOME_MSG << CRLF;
	send(fd, ss.str().c_str(), ss.str().size(), 0);
	std::cout << ss.str() << std::endl;
	ss.str("");

	ss << ":" << "ircserv" << " " << RPL_YOURHOST << " " << usr.nickname << " " << RPL_YOURHOST_MSG << CRLF;
	send(fd, ss.str().c_str(), ss.str().size(), 0);
	std::cout << ss.str() << std::endl;
	ss.str("");

	ss << ":" << "ircserv" << " " << RPL_CREATED << " " << usr.nickname << " " << RPL_CREATED_MSG << CRLF;
	send(fd, ss.str().c_str(), ss.str().size(), 0);
	std::cout << ss.str() << std::endl;
	ss.str("");

	ss << ":" << "ircserv" << " " << RPL_MYINFO << " " << usr.nickname << " " << RPL_MYINFO_MSG << CRLF;
	send(fd, ss.str().c_str(), ss.str().size(), 0);
	std::cout << ss.str() << std::endl;

	std::cout << COLOR_GREEN << usr.nickname << " succesfully registered !" << COLOR_RESET << std::endl;
}

namespace cmd
{
	int cap(Server& srv, int user_fd, const Message& msg)
	{
		(void)srv;
		(void)user_fd;
		(void)msg;
		return (0);
	}

	int nick(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 2)
			return(1); // handle error
		srv.user_lst[user_fd].nickname = msg.getArgv()[1];
		
		std::stringstream ss;
		ss << ":" << srv.user_lst[user_fd].nickname << "!" << srv.user_lst[user_fd].username << "@"  << inet_ntoa(srv.user_lst[user_fd].socket.sin_addr) << " " << msg.getRaw();
		send(user_fd, ss.str().c_str(), ss.str().size(), 0);
		
		if (!srv.user_lst[user_fd].registered && !srv.user_lst[user_fd].username.empty())
			registerUser(user_fd, srv.user_lst[user_fd]);
		return (0);
	}

	int user(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 5 || srv.user_lst[user_fd].registered == true)
			return(1); // handle error
		srv.user_lst[user_fd].username = msg.getArgv()[1];
		// msg.getArgv()[2]; handle user mode
		srv.user_lst[user_fd].realname = msg.getArgv()[5];
		if (!srv.user_lst[user_fd].nickname.empty())
			registerUser(user_fd, srv.user_lst[user_fd]);
		return (0);
	}

	int ping(Server& srv, int user_fd, const Message& msg)
	{
		(void)srv;
		if (msg.getArgv().size() < 2)
			return(1); // handle error
		std::stringstream ss;
		ss << "PONG" << " " << msg.getArgv()[1] << CRLF;
		send(user_fd, ss.str().c_str(), ss.str().size(), 0);
		return (0);
	}

	int join(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 2)
			return(1); // handle error
		
		srv.chan_lst.insert(std::make_pair(msg.getArgv()[1], Channel()));
		srv.chan_lst[msg.getArgv()[1]].users.push_back(&srv.user_lst[user_fd]);

		srv.user_lst[user_fd].channels.push_back(&srv.chan_lst[msg.getArgv()[1]]);

		std::stringstream ss;
		ss << "JOIN" << " " << msg.getRaw();
		send(user_fd, ss.str().c_str(), ss.str().size(), 0);
		return (0);
	}
};