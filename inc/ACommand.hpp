#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <string>
#include <vector>

#include "Server.hpp"

class Server;

class ACommand
{
public:
	ACommand(std::string name, bool is_operator_only = false);
	virtual ~ACommand();

	std::string getName();

	virtual int execute(Server &srv, Client &user, const Message& msg) const = 0;
	bool isOperator();

protected:
	const std::string name;
	bool is_operator_only;

private:
	ACommand();
	const ACommand &operator=(const ACommand &rhs);
};

#endif // ACOMMAND_HPP
