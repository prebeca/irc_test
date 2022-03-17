#ifndef MODE_HPP
#define MODE_HPP

#include "ACommand.hpp"

class ACommand;

class MODE: public ACommand
{
public:
	MODE();
	~MODE();

	int execute(Server &srv, Client &user, const Message& msg) const;
	int userMode(Server &srv, Client &user, const Message& msg) const;
	int chanMode(Server &srv, Client &user, const Message& msg) const;
};

#endif // MODE_HPP
