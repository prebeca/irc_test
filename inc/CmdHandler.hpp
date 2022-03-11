#ifndef CMDHANDLER_HPP
#define CMDHANDLER_HPP

#include <map>

#include "ACommand.hpp"

class ACommand;

class CmdHandler
{
public:
	CmdHandler();
	~CmdHandler();

	const ACommand* getCmd(std::string name) const;

private:
	std::map<std::string, ACommand*> cmds;

};

#endif // CMDHANDLER_HPP
