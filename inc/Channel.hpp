#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>

#include "Client.hpp"

#define VALID_CHANMODE "kimo"
/*
   i - invite only;
   k - key needed to enter channel.
   m - channel is moderated.
   o - addd operator
*/

class Client;

class Channel
{
public:
	Channel(std::string name);
	~Channel();

	std::string getName() const;

	bool addUser(Client &usr);
	bool removeUser(Client &usr);
	const std::map<int, Client *> &getUsers();

	bool addOper(Client &usr);
	bool removeOper(Client &usr);
	const std::map<int, Client *> &getOpers();

	bool addInvitation(Client &usr);
	bool removeInvitation(Client &usr);
	const std::map<int, Client *> &getInvitations();

	void setTopic(std::string topic);
	std::string getTopic() const;

	void setKey(std::string key);
	std::string getKey() const;

	int addMode(char mode);
	int removeMode(char mode);
	std::string getMode() const;

private:
	Channel();
	Channel(const Channel &cpy);
	const Channel &operator=(const Channel &rhs);

	std::string name;
	std::map<int, Client *> operators;
	std::map<int, Client *> users;
	std::map<int, Client *> invitations;
	std::string topic;
	std::string mode;
	std::string key;
};

#endif // CHANNEL_HPP
