#ifndef LIST_HPP
#define LIST_HPP

#include "ACommand.hpp"

class ACommand;

class LIST: public ACommand
{
public:
	LIST();
	~LIST();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // LIST_HPP