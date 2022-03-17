#ifndef NUMERIC_REPLIES_H
#define NUMERIC_REPLIES_H

#define QUIT_RETURN -42

#ifndef CRLF
# define CRLF "\r\n"
#endif	//CRLF

#define RPL_WELCOME(nickname) ("001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + CRLF)
#define RPL_YOURHOST(nickname,servername,version) ("002 " + nickname + " :Your host is " + servername + ", running version " + version + CRLF)
#define RPL_CREATED(nickname,date) ("003 " + nickname + " :This server was created " + date + CRLF)
#define RPL_MYINFO(nickname,servername,version,usermodes,chanmodes) ("004 " + nickname + " :" + servername + " " + version + " " + usermodes + " " + chanmodes + CRLF)

#define RPL_UMODEIS(nickname,usermodes) ("221 " + nickname + " " + usermodes + CRLF)

#define RPL_NOTOPIC(nickname,channel) ("331 " + nickname + " " + channel + " :No topic is set" + CRLF)
#define RPL_TOPIC(nickname,channel,topic) ("332 " + nickname + " " + channel + " :" + topic + CRLF)

#define RPL_NAMREPLY(nickname,channel,userlist) ("353 " + nickname + " " + channel + " :" + userlist + CRLF)
#define RPL_ENDOFNAMES(nickname,channel) ("366 " + nickname + " " + channel + " :End of NAMES list" + CRLF)

#define ERR_NOSUCHNICK(user_nickname,nickname) ("401 " + user_nickname + " " + nickname + " :No such nick" + CRLF)
#define ERR_NOSUCHCHANNEL(nickname,channel) ("403 " + nickname + " " + channel + " :No such channel" + CRLF)
#define ERR_CANNOTSENDTOCHAN(nickname, channel) ("404 " + nickname + " " + channel + " :Cannot send to channel" + CRLF)

#define ERR_NONICKNAMEGIVEN(nickname) ("431 " + nickname + " :No nickname given" + CRLF)
#define ERR_ERRONEUSNICKNAME(nickname,newnickname) ("432 " + nickname + " " + newnickname + " :Erroneous nickname" + CRLF)
#define ERR_NICKNAMEINUSE(nickname,newnickname) ("433 " + nickname + " " + newnickname + " :Nickname is already in use" + CRLF)

// #define ERR_UNAVAILRESOURCE "437" //"<nick/channel> :Nick/channel is temporarily unavailable"

#define ERR_NOTONCHANNEL(nickname,channel) ("442 " + nickname + " " + channel + " :You're not on that channel" + CRLF)

#define ERR_NOTREGISTERED(nickname) ("451 " + nickname + " :You have not registered" + CRLF)

#define ERR_NEEDMOREPARAMS(nickname,command) ("461 " + nickname + " " + command + " :Not enough parameters" + CRLF)
#define ERR_ALREADYREGISTRED(nickname) ("462 " + nickname + " :Unauthorized command (already registered)" + CRLF)
#define ERR_PASSWDMISMATCH(nickname) ("464 " + nickname + " :Password incorrect" + CRLF)

#define ERR_INVITEONLYCHAN(nickname,channel) ("473 " + nickname + " " + channel + " :Cannot join channel (+i)" + CRLF)
#define ERR_BADCHANNELKEY(nickname,channel) ("475 " + nickname + " " + channel + " :Cannot join channel (+k)" + CRLF)

#define ERR_CHANOPRIVSNEEDED(nickname,channel) ("482 " + nickname + " " + channel + " :You're not channel operator" + CRLF)
// #define ERR_RESTRICTED(nickname) "484"	   //":Your connection is restricted!"

#define ERR_UMODEUNKNOWNFLAG(nickname) ("501 " + nickname +" :Unknown MODE flag" + CRLF)
#define ERR_USERSDONTMATCH(nickname) ("502" + nickname + " :Cannot change mode for other users" + CRLF)

#endif // NUMERIC_REPLIES_H
