#include "ACommand.hpp"

ACommand::ACommand(std::string name, bool is_operator_only)
	: name(name), is_operator_only(is_operator_only)
{
}

ACommand::~ACommand()
{
}

std::string ACommand::getName()
{
	return (this->name);
}

bool ACommand::isOperator()
{
	return (this->is_operator_only);
}
