#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"
#include "utils.h"

#define SPECIAL "[]\\`_^{|}"
#define VALID "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz[]\\`_^{|}-"

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
