#ifndef PASS_HPP
#define PASS_HPP

#include "ACommand.hpp"

class ACommand;

class PASS: public ACommand
{
public:
	PASS();
	~PASS();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PASS_HPP
