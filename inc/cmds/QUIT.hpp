#ifndef QUIT_HPP
#define QUIT_HPP

#include "ACommand.hpp"

#define QUIT_MESSAGE(prefix, message) (":" + prefix + " QUIT :" + message + CRLF)
#define QUIT_ACKNOWLEDGE(prefix) (":" + prefix + " ERROR: QUIT command acknowledge" + CRLF)

class ACommand;

class QUIT: public ACommand
{
public:
	QUIT();
	~QUIT();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // QUIT_HPP
 