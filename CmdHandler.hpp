#ifndef CMDHANDLER_HPP
#define CMDHANDLER_HPP

#include <map>
#include <string>
#include <iostream>

inline int cmdCap(void)
{
	std::cout << "handled CAP cmd" << std::endl;
	return(0);
}

typedef int (*pCmd)(void);

struct CmdHandler
{
	typedef const std::map<const std::string, pCmd>::const_iterator const_iterator;
	static const std::map<const std::string, pCmd>	cmd_lst;

private:
	static const std::map<const std::string, pCmd>	buildMap();
};

#endif // CMDHANDLER_HPP
