#include "utils.hpp"

std::vector<std::string> ft_split(std::string str, std::string delimiter)
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