#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "ACommand.hpp"

class ACommand;

class TOPIC: public ACommand
{
public:
	TOPIC();
	~TOPIC();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // TOPIC_HPP
