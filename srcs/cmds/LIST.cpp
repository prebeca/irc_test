#include "cmds/LIST.hpp"

LIST::LIST()
	: ACommand("LIST")
{
}

LIST::~LIST()
{
}

int LIST::execute(Server &srv, Client &user, const Message &msg) const
{
	if (!user.isRegistered())
	{
		srv.sendMsg(user.getFd(), Message(ERR_NOTREGISTERED(user.getNickname())));
		return (1);
	}

	(void)msg;
	std::map<std::string, Channel *>			channels = srv.getChannels();
	std::map<std::string, Channel *>::iterator	it = channels.begin();
	std::map<std::string, Channel *>::iterator	ite = channels.end();

	for (; it != ite; ++it) {
		std::stringstream	ss;
		// std::cout << "hello1" << std::endl;
		ss << "322 * " << it->second->getName() << " " << it->second->getUsers().size() << CRLF;
		std::string reply = ss.str();
		// std::cout << "hello2" << std::endl;
		// const char* reply_const = reply.c_str();
		// (void)reply2_const;
		srv.sendMsg(user.getFd(), Message(reply));
		// std::cout << "hello3" << std::endl;
	}
	// std::cout << "hello4" << std::endl;
	// std::string reply2 = "323 * :End of LIST" + CRLF;
	// const char* reply2_const = reply2.c_str();
	srv.sendMsg(user.getFd(), Message("323 * :End of LIST\r\n"));

	return 0;
}