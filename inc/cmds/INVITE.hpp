#ifndef INVITE_HPP
#define INVITE_HPP

#include "ACommand.hpp"

class ACommand;

class INVITE: public ACommand
{
public:
	INVITE();
	~INVITE();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // INVITE_HPP