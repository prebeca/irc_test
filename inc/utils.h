#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace utils
{
	std::string strToUpper(const std::string str);
	std::vector<std::string> split(std::string str, std::string delimiter);
}

#endif // UTILS_H
