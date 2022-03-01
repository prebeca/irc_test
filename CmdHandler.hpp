#ifndef CMDHANDLER_HPP
#define CMDHANDLER_HPP

#include "irc.hpp"

#include "Server.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iostream>

class Server;

namespace cmd
{
	int cap(Server& srv, int user_fd, const Message& msg);
	int nick(Server& srv, int user_fd, const Message& msg);
	int user(Server& srv, int user_fd, const Message& msg);
	int ping(Server& srv, int user_fd, const Message& msg);
	int join(Server& srv, int user_fd, const Message& msg);
	int part(Server& srv, int user_fd, const Message& msg);
	int privmsg(Server& srv, int user_fd, const Message& msg);
	int notice(Server& srv, int user_fd, const Message& msg);
	int list(Server& srv, int user_fd, const Message& msg);
	int quit(Server& srv, int user_fd, const Message& msg);
	int topic(Server& srv, int user_fd, const Message& msg);
	int kick(Server& srv, int user_fd, const Message& msg);
};

typedef int (*pCmd)(Server& srv, int user_fd, const Message& msg);

struct CmdHandler
{
	typedef const std::map<const std::string, pCmd>::const_iterator const_iterator;
	static const std::map<const std::string, pCmd>	cmd_lst;

private:
	static const std::map<const std::string, pCmd>	buildMap();
};

#endif // CMDHANDLER_HPP
