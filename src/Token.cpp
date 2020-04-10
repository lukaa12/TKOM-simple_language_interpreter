#include "Token.h"

using namespace tkom;

Token::Token(unsigned int _line, unsigned int _column) : type(Type::Invalid), line(_line), column(_column), int_value(0), s_value("")
{

}

Token::Token(Type _type, unsigned int _line, unsigned int _column) : type(_type), line(_line), column(_column), int_value(0), s_value("")
{

}

Token::Token(Type _type, unsigned int _line, unsigned int _column, int val) : type(_type), line(_line), column(_column), int_value(val), s_value("")
{

}

Token::Token(Type _type, unsigned int _line, unsigned int _column, std::string val) : type(_type), line(_line), column(_column), int_value(0), s_value(val)
{

}

Token::Token(const Token& other) : type(other.type), line(other.line), column(other.column), int_value(other.int_value), s_value(other.s_value)
{

}

Token::Type Token::getType()
{
	return type;
}
int Token::getIntVal()
{
	if (type != Type::IntLiteral)
		throw std::exception("Cannot get int value of token");
	return int_value;
}
std::string Token::getStrVal()
{
	if (type != Type::Identifier && type != Type::StringLiteral)
		throw std::exception("Cannot get string value of token");
	return s_value;
}
unsigned int Token::getLine()
{
	return line;
}

unsigned int Token::getColumn()
{
	return column;
}

std::string Token::toString(Token::Type type)
{
	switch (type)
	{
    case tkom::Token::Type::Eof:
        return "EOF";
    case tkom::Token::Type::Invalid:
        return "Invalid";
    case tkom::Token::Type::IntType:
        return "IntType";
    case tkom::Token::Type::StringType:
        return "StringType";
    case tkom::Token::Type::GraphicType:
        return "GraphicType";
    case tkom::Token::Type::ColorType:
        return "ColorType";
    case tkom::Token::Type::Identifier:
        return "Identifier";
    case tkom::Token::Type::IntLiteral:
        return "IntLiteral";
    case tkom::Token::Type::StringLiteral:
        return "StringLiteral";
    case tkom::Token::Type::Less:
        return "Less";
    case tkom::Token::Type::Greater:
        return "Greater";
    case tkom::Token::Type::LessOrEqual:
        return "LessOrEqual";
    case tkom::Token::Type::GreaterOrEqual:
        return "GreaterOrEqual";
    case tkom::Token::Type::Equal:
        return "Equal";
    case tkom::Token::Type::NotEqual:
        return "NotEqual";
    case tkom::Token::Type::Negation:
        return "Negation";
    case tkom::Token::Type::BracketOpen:
        return "BracketOpen";
    case tkom::Token::Type::BracketClose:
        return "BracketClose";
    case tkom::Token::Type::CurlyOpen:
        return "CurlyOpen";
    case tkom::Token::Type::CurlyClose:
        return "CurlyClose";
    case tkom::Token::Type::Comma:
        return "Comma";
    case tkom::Token::Type::Semicoln:
        return "Semicoln";
    case tkom::Token::Type::If:
        return "If";
    case tkom::Token::Type::While:
        return "While";
    case tkom::Token::Type::Else:
        return "Else";
    case tkom::Token::Type::Return:
        return "Return";
    case tkom::Token::Type::Break:
        return "Break";
    case tkom::Token::Type::Assigment:
        return "Assigment";
    case tkom::Token::Type::Or:
        return "Or";
    case tkom::Token::Type::And:
        return "And";
    case tkom::Token::Type::Plus:
        return "Plus";
    case tkom::Token::Type::Minus:
        return "Minus";
    case tkom::Token::Type::Multiply:
        return "Multiply";
    case tkom::Token::Type::Divide:
        return "Divide";
    default:
        break;
	}
}