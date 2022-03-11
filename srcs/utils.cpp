#include "utils.h"

namespace utils
{
	std::string strToUpper(const std::string str)
	{
		std::string cpy(str);
		std::string::iterator it;

		for (it = cpy.begin(); it != cpy.end(); ++it)
			*it = std::toupper(*it);
		return (cpy);
	}

	std::vector<std::string> split(std::string str, std::string delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;

		while (str.empty() == false)
		{
			size_t token_size = str.find(delimiter);
			token = str.substr(0, token_size);
			tokens.push_back(token);
			str.erase(0, token.size() + delimiter.size());
		}
		return (tokens);
	}
}