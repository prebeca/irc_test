#include "CmdHandler.hpp"
#include "cmds/CAP.hpp"
#include "cmds/NICK.hpp"
#include "cmds/USER.hpp"
#include "cmds/JOIN.hpp"
#include "cmds/PING.hpp"
#include "cmds/PART.hpp"
#include "cmds/QUIT.hpp"
#include "cmds/PRIVMSG.hpp"
#include "cmds/TOPIC.hpp"
#include "cmds/KICK.hpp"
#include "cmds/PASS.hpp"
#include "cmds/MODE.hpp"
#include "cmds/NOTICE.hpp"
#include "cmds/LIST.hpp"
#include "cmds/INVITE.hpp"
#include "cmds/WHO.hpp"

CmdHandler::CmdHandler()
{
	this->cmds["CAP"] = new CAP();
	this->cmds["NICK"] = new NICK();
	this->cmds["USER"] = new USER();
	this->cmds["JOIN"] = new JOIN();
	this->cmds["PING"] = new PING();
	this->cmds["PART"] = new PART();
	this->cmds["QUIT"] = new QUIT();
	this->cmds["PRIVMSG"] = new PRIVMSG();
	this->cmds["TOPIC"] = new TOPIC();
	this->cmds["KICK"] = new KICK();
	this->cmds["PASS"] = new PASS();
	this->cmds["MODE"] = new MODE();
	this->cmds["NOTICE"] = new NOTICE();
	this->cmds["LIST"] = new LIST();
	this->cmds["INVITE"] = new INVITE();
	this->cmds["WHO"] = new WHO();
}

CmdHandler::~CmdHandler()
{
	std::map<std::string, ACommand*>::iterator it;

	for (it = this->cmds.begin(); it != this->cmds.end(); ++it)
		delete it->second;
}

const ACommand* CmdHandler::getCmd(std::string name) const
{
	std::map<std::string, ACommand*>::const_iterator it;
	it = this->cmds.find(name);
	if (it != this->cmds.end())
		return (it->second);
	return (NULL);
}