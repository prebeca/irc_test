#ifndef IRC_HPP
#define IRC_HPP

#define CRLF				"\r\n"

/*
**	Greetings code
*/

#define RPL_WELCOME			"001"
#define RPL_YOURHOST		"002"
#define RPL_CREATED			"003"
#define RPL_MYINFO			"004"

#define RPL_WELCOME_MSG		"Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_YOURHOST_MSG	"Your host is <servername>, running version <ver>"
#define RPL_CREATED_MSG		"This server was created <date>"
#define RPL_MYINFO_MSG		"<servername> <version> <available user modes> <available channel modes>"

/*
**	Error code
*/

#define RPL_LIST			"322"
#define RPL_LISTEND			"323"

#endif // IRC_HPP
