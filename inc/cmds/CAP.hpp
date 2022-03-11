#ifndef CAP_HPP
#define CAP_HPP

#include "ACommand.hpp"

class ACommand;

class CAP: public ACommand
{
public:
	CAP();
	~CAP();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // CAP_HPP
