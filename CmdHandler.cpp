#include "CmdHandler.hpp"

const std::map<const std::string, pCmd> CmdHandler::cmd_lst = CmdHandler::buildMap();

const std::map<const std::string, pCmd> CmdHandler::buildMap()
{
	std::map<const std::string, pCmd> cmd_lst;

	cmd_lst["CAP"] = cmd::cap;
	cmd_lst["NICK"] = cmd::nick;
	cmd_lst["USER"] = cmd::user;
	cmd_lst["PING"] = cmd::ping;
	cmd_lst["JOIN"] = cmd::join;
	cmd_lst["PART"] = cmd::part;
	cmd_lst["PRIVMSG"] = cmd::privmsg;

	return (cmd_lst);
}