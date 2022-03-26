#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "ACommand.hpp"

#define NOTICE_FORMAT(prefix,nickname,target,msg) (":" + prefix + " NOTICE " + target + " :" + msg + CRLF)

class ACommand;

class NOTICE: public ACommand
{
public:
	NOTICE();
	~NOTICE();

	int execute(Server &srv, Client &user, const Message& msg) const;
};

#endif // NOTICE_HPP
