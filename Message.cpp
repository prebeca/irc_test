#include "Message.hpp"

Message::Message(std::string raw_msg)
: raw_msg(raw_msg)
{
	parseMsg();
}

Message::Message(std::string prefix, std::vector<std::string> argv)
: prefix(prefix), argv(argv)
{
	raw_msg = toRaw();
}

Message::~Message()
{}

const std::string&	Message::getRaw() const
{ return (raw_msg); }

const std::string&	Message::getPrefix() const
{ return (prefix); }

const std::vector<std::string>&	Message::getArgv() const
{ return (argv); }

void Message::parseMsg()
{
	std::string	msg = raw_msg;
	size_t		sub_len;

	msg.erase(msg.find("\r\n", 0));
	while ((sub_len = msg.find(' ')) != std::string::npos)
	{
		argv.push_back(msg.substr(0, sub_len));
		msg.erase(0, sub_len + 1);
	}
	if (!msg.empty())
		argv.push_back(msg);
	if (argv.size() > 0 && argv[0][0] == ':')
	{
		argv[0].erase(0, 1);
		prefix = argv[0];
		argv.erase(argv.begin());
	}
}

std::string Message::toRaw()
{
	std::stringstream raw;

	if (!prefix.empty())
		raw << ":" << prefix << " ";
	for (size_t i = 0; i < argv.size(); ++i)
	{
		raw << argv[i];
		if (i != argv.size() - 1)
			raw << " ";
	}
	raw << "\r\n";
	return(raw.str());
}
