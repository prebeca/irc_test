#ifndef PART_HPP
#define PART_HPP

#include "ACommand.hpp"

class ACommand;

class PART: public ACommand
{
public:
	PART();
	~PART();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PART_HPP
