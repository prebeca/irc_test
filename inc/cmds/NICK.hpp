#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"
#include "utils.h"

#define SPECIAL "[]\\`_^{|}"
#define VALID "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz[]\\`_^{|}-"
#define MAX_SIZE 9

#define NICK_MESSAGE(prefix, new_nick) (":" + prefix + " NICK :" + new_nick + CRLF)

class ACommand;

class NICK: public ACommand
{
public:
	NICK();
	~NICK();

	int execute(Server &srv, Client &user, const Message& msg) const;

	bool isValidNick(std::string nick) const;
};

#endif // NICK_HPP
