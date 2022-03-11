#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Channel.hpp"

#define VALID_USERMODE "aiwroOs"
/*
   a - user is flagged as away;
   i - marks a users as invisible;
   w - user receives wallops;
   r - restricted user connection;
   o - operator flag;
   O - local operator flag;
   s - marks a user for receipt of server notices.
*/

class Channel;

class Client
{
public:
	Client(int fd, sockaddr_in socket);
	~Client();

	void setNickname(std::string nickname);
	std::string getNickname() const;

	void setUsername(std::string username);
	std::string getUsername() const;

	void setRealname(std::string realname);
	std::string getRealname() const;

	sockaddr_in getSocket() const;

	int getFd() const;

	void setRegistered(bool is_registered);
	bool isRegistered();

	void setPassCheck(bool is_pass_check);
	bool isPassCheck();

	void addChannel(Channel &chan);
	bool removeChannel(const Channel &chan);
	bool removeChannel(std::string name);
	const std::map<std::string, Channel *> &getChannels();

	int addMode(char mode);
	int removeMode(char mode);
	std::string getMode() const;

	std::string &getBuffer();

private:
	Client();
	Client(const Client &copy);
	const Client &operator=(const Client &rhs);

	bool registered;
	bool pass_check;
	std::string nickname;
	std::string username;
	std::string realname;
	std::string mode;
	sockaddr_in socket;
	int fd;

	std::map<std::string, Channel *> channels;
	std::string buffer;
};

#endif // CLIENT_HPP
