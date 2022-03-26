#ifndef PART_HPP
#define PART_HPP

#include "ACommand.hpp"

#define PART_MESSAGE(prefix, channel, msg) (":" + prefix + " PART " + channel + " :" + msg + CRLF)

class ACommand;

class PART: public ACommand
{
public:
	PART();
	~PART();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // PART_HPP
