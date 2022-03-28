#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>

#include "utils.h"

#define CRLF "\r\n"

class Message
{
public:
	Message(std::string raw_msg);
	~Message();

	const std::string &getRaw() const;
	const std::string &getPrefix() const;
	const std::vector<std::string> &getArgv() const;

private:
	Message();
	Message(const Message &rhs);
	const Message &operator=(const Message &rhs);

	void parseMsg();
	std::string toRaw();

	std::string raw_msg;
	std::string prefix;
	std::vector<std::string> argv;
};

#endif // MESSAGE_HPP
