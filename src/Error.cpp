#include "Error.h"

using namespace tkom;

Error::Error(const Token::Position& pos, Type _type): info(pos), type(_type)
{
	whatMessage += "Line: " + std::to_string(std::get<Token::Position>(info).line) + " pos: "
		+ std::to_string(std::get<Token::Position>(info).column) + " ";
	switch (this->type)
	{
	case Type::MissingStatement:
		whatMessage += "Missing statement: ";
		break;
	case Type::IncorrectParametersList:
		whatMessage += "Incorrect parameters for function: ";
		break;
	default:
		break;
	}
}

Error::Error(const Token& _token, Type _type): info(_token), type(_type)
{
	whatMessage += "Line: " + std::to_string(std::get<Token>(info).getPosition().line) + " pos: " 
		+ std::to_string(std::get<Token>(info).getPosition().column) + " ";
	switch (this->type)
	{
	case Type::UnexpectedToken:
		if (std::get<Token>(info).getType() == Token::Type::Invalid)
			whatMessage += "Invalid token: ";
		else
			whatMessage += "Unexpected token: ";
		break;
	case Type::UndefinedReference:
		whatMessage += "Cannot resolve symbol: ";
		break;
	default:
		break;
	}
}

const char* Error::what() const
{
	return whatMessage.c_str();
}