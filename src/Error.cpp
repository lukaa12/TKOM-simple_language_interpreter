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
			whatMessage += "Invalid token";
		else
			whatMessage += "Unexpected token  of type: ";
		break;
	case Type::UndefinedReference:
		whatMessage += "Cannot resolve symbol  of type: ";
		break;
	default:
		break;
	}
	if (std::get<Token>(info).getType() != Token::Type::Invalid)
		whatMessage += Token::toString(std::get<Token>(info).getType());
}

Error::Error(Type _type): type(_type)
{
	switch (this->type)
	{
	case Type::MissingMain:
		whatMessage = "Main function not found";
		break;
	case Type::UncompatibleType:
		whatMessage = "Uncompatible type of variable";
		break;
	case Type::UndefinedReference:
		whatMessage = "Undefined reference to symbol";
		break;
	case Type::CallOnNonFunction:
		whatMessage = "Call on non function object";
		break;
	case Type::IncorrectParametersList:
		whatMessage = "Incorrect Parameter list";
		break;
	case Type::DivisionByZero:
		whatMessage = "Division by zero";
		break;
	case Type::BreakOutsideLoop:
		whatMessage = "Break outside loop";
		break;
	case Type::FunctionNotReturnedValue:
		whatMessage = "Function must return a value";
		break;
	default:
		whatMessage = "Unknown error";
		break;
	}
}

const char* Error::what() const
{
	return whatMessage.c_str();
}