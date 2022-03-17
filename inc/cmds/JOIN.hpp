#ifndef JOIN_HPP
#define JOIN_HPP

#include "ACommand.hpp"
#include "utils.h"

class ACommand;

class JOIN: public ACommand
{
public:
	JOIN();
	~JOIN();

	int execute(Server &srv, Client &user, const Message& msg) const;
	void sendNames(Server &srv, Client &user, Channel& chan) const;
};

#endif // JOIN_HPP
