#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "ACommand.hpp"

class ACommand;

class NOTICE: public ACommand
{
public:
	NOTICE();
	~NOTICE();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // NOTICE_HPP
