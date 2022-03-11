#include "cmds/CAP.hpp"

CAP::CAP()
	: ACommand("CAP")
{
}

CAP::~CAP()
{
}

int CAP::execute(Server &srv, Client &user, const Message &msg) const
{
	(void)srv;
	(void)user;
	(void)msg;

	return (0);
}