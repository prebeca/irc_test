#ifndef NUMERIC_REPLIES_H
#define NUMERIC_REPLIES_H

#define QUIT_RETURN -42

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"

#define RPL_NOTOPIC "331" //"<channel> :No topic is set"
#define RPL_TOPIC "332"	  //"<channel> :<topic>"

#define RPL_NAMREPLY "353"	 //<channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
#define RPL_ENDOFNAMES "366" //"<channel> :End of NAMES list"

#define ERR_NOSUCHNICK "401"	   //"<nickname> :No such nick/channel"
#define ERR_NOSUCHCHANNEL "403"	   //"<channel name> :No such channel"
#define ERR_CANNOTSENDTOCHAN "404" //"<channel name> :Cannot send to channel"

#define ERR_NONICKNAMEGIVEN "431"  //":No nickname given"
#define ERR_ERRONEUSNICKNAME "432" //"<nick> :Erroneous nickname"
#define ERR_NICKNAMEINUSE "433"	   //"<nick> :Nickname is already in use"
#define ERR_NICKCOLLISION "436"	   //"<nick> :Nickname collision KILL from <user>@<host>"

#define ERR_UNAVAILRESOURCE "437" //"<nick/channel> :Nick/channel is temporarily unavailable"

#define ERR_NOTONCHANNEL "442" //"<channel> :You're not on that channel"

#define ERR_NOTREGISTERED "451" //":You have not registered"

#define ERR_NEEDMOREPARAMS "461"   //"<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED "462" //":Unauthorized command (already registered)"
#define ERR_PASSWDMISMATCH "464" // ":Password incorrect"

#define ERR_INVITEONLYCHAN "473" // "<channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN "474" //"<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY "475"	 // "<channel> :Cannot join channel (+k)"


#define ERR_CHANOPRIVSNEEDED "482" //"<channel> :You're not channel operator"
#define ERR_RESTRICTED "484"	   //":Your connection is restricted!"

#endif // NUMERIC_REPLIES_H
