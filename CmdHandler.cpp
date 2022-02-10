#include "CmdHandler.hpp"

const std::map<const std::string, pCmd> CmdHandler::cmd_lst = CmdHandler::buildMap();

const std::map<const std::string, pCmd> CmdHandler::buildMap()
{
	std::map<const std::string, pCmd> cmd_lst;

	cmd_lst["CAP"] = cmdCap;

	return (cmd_lst);
}