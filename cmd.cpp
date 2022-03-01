#include "CmdHandler.hpp"
#include "utils.hpp"

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

void sendToList(std::string msg, std::map<int, User*> users)
{
	std::map<int, User*>::iterator it;
	for (it = users.begin(); it != users.end(); ++it)
		send(it->second->fd, msg.c_str(), msg.size(), 0);
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
		srv.user_by_nick.erase(srv.user_by_fd[user_fd].nickname);
		srv.user_by_fd[user_fd].nickname = msg.getArgv()[1];
		srv.user_by_nick[msg.getArgv()[1]] = srv.user_by_fd[user_fd];
		
		std::stringstream ss;
		ss << ":" << srv.user_by_fd[user_fd].nickname << "!" << srv.user_by_fd[user_fd].username << "@"  << inet_ntoa(srv.user_by_fd[user_fd].socket.sin_addr) << " " << msg.getRaw();
		send(user_fd, ss.str().c_str(), ss.str().size(), 0);
		
		if (!srv.user_by_fd[user_fd].registered && !srv.user_by_fd[user_fd].username.empty())
			registerUser(user_fd, srv.user_by_fd[user_fd]);
		return (0);
	}

	int user(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 5 || srv.user_by_fd[user_fd].registered == true)
			return(1); // handle error
		srv.user_by_fd[user_fd].username = msg.getArgv()[1];
		// msg.getArgv()[2]; handle user mode
		srv.user_by_fd[user_fd].realname = msg.getArgv()[5];
		if (!srv.user_by_fd[user_fd].nickname.empty())
			registerUser(user_fd, srv.user_by_fd[user_fd]);
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
		
		if (msg.getArgv()[1].compare("0") == 0)
		{
			std::map<std::string, Channel*>::iterator it = srv.user_by_fd[user_fd].channels.begin();

			for (; it != srv.user_by_fd[user_fd].channels.end(); ++it)
			{
				it->second->users.erase(user_fd);
				srv.user_by_fd[user_fd].channels.erase(it->second->name);

				std::stringstream ss;
				ss << ":" << srv.user_by_fd[user_fd].nickname << " PART " << it->second->name << CRLF;
				sendToList(ss.str(), it->second->users);
				send(user_fd, ss.str().c_str(), ss.str().size(), 0);
				std::cout << ss.str() << std::endl;
			}
		}

		std::vector<std::string> channels = ft_split(msg.getArgv()[1], ",");

		for (size_t i = 0; i < channels.size(); ++i)
		{
			// srv.chan_lst.insert(std::make_pair(msg.getArgv()[1], Channel()));
			srv.chan_lst[channels[i]].name = channels[i];
			srv.chan_lst[channels[i]].users.insert(std::make_pair(user_fd, &srv.user_by_fd[user_fd]));

			srv.user_by_fd[user_fd].channels.insert(std::make_pair(channels[i], &srv.chan_lst[channels[i]]));

			std::stringstream ss;
			ss << ":" << srv.user_by_fd[user_fd].nickname << " JOIN " << channels[i] << CRLF;
			sendToList(ss.str(), srv.chan_lst[channels[i]].users);
			std::cout << ss.str() << std::endl;
			
			ss.str("");
			ss << ":" << "ircserv " << RPL_TOPIC << " " << srv.user_by_fd[user_fd].nickname << " " << channels[i] << " :" << srv.chan_lst[channels[i]].topic << CRLF;
			send(user_fd, ss.str().c_str(), ss.str().size(), 0);
			std::cout << ss.str() << std::endl;
			
			ss.str("");
			ss << ":" << "ircserv " << RPL_NAMREPLY << " " << srv.user_by_fd[user_fd].nickname << " " << channels[i] << " :";
			std::map<int, User*>::iterator it;
			for (it = srv.chan_lst[channels[i]].users.begin(); it != srv.chan_lst[channels[i]].users.end(); ++it)
			{
				ss << it->second->nickname << " ";
			}
			ss << CRLF;
			send(user_fd, ss.str().c_str(), ss.str().size(), 0);
			std::cout << ss.str() << std::endl;
			ss.str("");
			ss << ":" << "ircserv " << RPL_ENDOFNAMES << " " << srv.user_by_fd[user_fd].nickname << " " << channels[i] << " :End Of Users" << CRLF;
			send(user_fd, ss.str().c_str(), ss.str().size(), 0);
			std::cout << ss.str() << std::endl;
		}

		return (0);
	}

	int part(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 2)
			return(1); // handle error

		std::vector<std::string> channels = ft_split(msg.getArgv()[1], ",");

		for (size_t i = 0; i < channels.size(); ++i)
		{
			std::map<std::string, Channel*>::iterator it;
			it = srv.user_by_fd[user_fd].channels.find(channels[i]);

			if (it != srv.user_by_fd[user_fd].channels.end())
			{
				srv.chan_lst[channels[i]].users.erase(user_fd);
				srv.user_by_fd[user_fd].channels.erase(channels[i]);
			}

			std::stringstream ss;
			ss << ":" << srv.user_by_fd[user_fd].nickname << " PART " << channels[i] << CRLF;
			sendToList(ss.str(), srv.chan_lst[channels[i]].users);
			send(user_fd, ss.str().c_str(), ss.str().size(), 0);
			std::cout << ss.str() << std::endl;
		}
		return (0);
	}

	int privmsg(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 3)
			return(1); // handle error

		std::string chan_identifier("&#");
		std::stringstream ss;
		ss << ":" << srv.user_by_fd[user_fd].nickname << " " << msg.getRaw();

		if (chan_identifier.find(msg.getArgv()[1][0], 0) != std::string::npos)
		{
			std::map<std::string, Channel*>::iterator it;
			it = srv.user_by_fd[user_fd].channels.find(msg.getArgv()[1]);

			if (it != srv.user_by_fd[user_fd].channels.end())
			{
				std::map<int, User*> lst = srv.chan_lst[msg.getArgv()[1]].users;
				lst.erase(user_fd);
				sendToList(ss.str(), lst);
				std::cout << ss.str() << std::endl;
			}
		}
		else
		{
			std::map<int, User>::iterator it;
			for (it = srv.user_by_fd.begin(); it != srv.user_by_fd.end(); ++it)
			{
				if (it->second.nickname != msg.getArgv()[1])
					continue;
				send(it->second.fd, ss.str().c_str(), ss.str().size(), 0);
			}
		}
		return (0);
	}

	int notice(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 3)
			return(1); // handle error

		std::string chan_identifier("&#");
		std::stringstream ss;
		ss << ":" << srv.user_by_fd[user_fd].nickname << " " << msg.getRaw();

		if (chan_identifier.find(msg.getArgv()[1][0], 0) != std::string::npos)
		{
			std::map<std::string, Channel*>::iterator it;
			it = srv.user_by_fd[user_fd].channels.find(msg.getArgv()[1]);

			if (it != srv.user_by_fd[user_fd].channels.end())
			{
				std::map<int, User*> lst = srv.chan_lst[msg.getArgv()[1]].users;
				lst.erase(user_fd);
				sendToList(ss.str(), lst);
				std::cout << ss.str() << std::endl;
			}
		}
		else
		{
			std::map<int, User>::iterator it;
			for (it = srv.user_by_fd.begin(); it != srv.user_by_fd.end(); ++it)
			{
				if (it->second.nickname != msg.getArgv()[1])
					continue;
				send(it->second.fd, ss.str().c_str(), ss.str().size(), 0);
			}
		}
		return (0);
	}

	int list(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() > 2)
			return(1); // handle error

		std::stringstream ss;

		if (msg.getArgv().size() == 1)
		{
 			std::map<std::string, Channel>::iterator it;
			for (it = srv.chan_lst.begin(); it != srv.chan_lst.end(); ++it)
			{
				ss.str("");
				ss << ":ircserv" << " " << RPL_LIST << " " << srv.user_by_fd[user_fd].nickname << " ";
				ss << it->second.name << " :" << it->second.topic;
				ss << CRLF;
				send(user_fd, ss.str().c_str(), ss.str().size(), 0);
				std::cout << ss.str() << std::endl;
			}
		}
		else if (srv.chan_lst.find(msg.getArgv()[1]) != srv.chan_lst.end())
		{
			ss.str("");
			ss << ":ircserv" << " " << RPL_LIST << " " << srv.user_by_fd[user_fd].nickname << " :";
			ss << srv.chan_lst[msg.getArgv()[1]].name << " :" << srv.chan_lst[msg.getArgv()[1]].topic;
			ss << CRLF;
			send(user_fd, ss.str().c_str(), ss.str().size(), 0);
			std::cout << ss.str() << std::endl;
		}
		ss.str("");
		ss << ":ircserv" << " " << RPL_LISTEND << " " << srv.user_by_fd[user_fd].nickname << " :End of List" << CRLF;
		send(user_fd, ss.str().c_str(), ss.str().size(), 0);
		std::cout << ss.str() << std::endl;
		return (0);
	}

	int quit(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() > 2)
			return(1);

		User* usr = srv.getUser(user_fd);
		std::stringstream ss;
		ss << ":" << usr->nickname << " " << msg.getRaw();

		std::map<std::string, Channel*>::iterator it;

		for (it = usr->channels.begin(); it != usr->channels.end(); ++it)
		{
			sendToList(ss.str(), it->second->users);
			it->second->users.erase(user_fd);
		}
		srv.removeUser(*usr);
		return (0);
 	}
	
	int topic(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() == 2)
		{
			std::stringstream ss;
			ss << ":" << "ircserv " << RPL_TOPIC << " " << srv.getUser(user_fd)->nickname << " " << msg.getArgv()[1] << " :" << srv.getChannel(msg.getArgv()[1])->topic << CRLF;
			send(user_fd, ss.str().c_str(), ss.str().size(), 0);
			std::cout << ss.str() << std::endl;
		}
		else if (msg.getArgv().size() == 3)
		{
			srv.getChannel(msg.getArgv()[1])->topic = std::string(&msg.getArgv()[2][1]);
			std::cout << std::string(&msg.getArgv()[2][1]) << std::endl;
		}
		return (0);
	}
	
	int kick(Server& srv, int user_fd, const Message& msg)
	{
		if (msg.getArgv().size() != 3)
			return(1);
		return(0);
	}
};