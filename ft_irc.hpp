#ifndef FT_IRC
#define FT_IRC

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <unistd.h>
#include <errno.h>

#include <cstring>		// memset

#include <sstream>

#include <arpa/inet.h>	// htons

#include <sys/time.h>
#include <sys/types.h>  // socket, sockaddr_in
#include <sys/socket.h> // socket
#include <sys/select.h>

#include <netinet/in.h> // sockaddr_in
#include <netinet/ip.h> // sockaddr_in

#include <netdb.h>		// getaddrrinfo

#include <map>

#define BUFFER_SIZE 4096

struct User
{
	bool registered;
	std::string nickname;
	std::string username;
	std::string realname;
	std::string host;
};

#endif /* FT_IRC */
