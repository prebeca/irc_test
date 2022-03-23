#ifndef WHO_HPP
#define WHO_HPP

#include "ACommand.hpp"

class ACommand;

class WHO: public ACommand
{
public:
	WHO();
	~WHO();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // WHO_HPP
