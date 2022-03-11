#ifndef USER_HPP
#define USER_HPP

#include "ACommand.hpp"

class ACommand;

class USER: public ACommand
{
public:
	USER();
	~USER();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // USER_HPP
