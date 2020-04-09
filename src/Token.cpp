#include "Token.h"

using namespace tkom;

Token::Token(): type(Token::Type::Invalid)
{}

//Token::Token(const Token& other): type(other.type), value(other.value)

Token::Token(const Type& _type): type(_type)
{}
Token::Token(const Type& _type, const std::string& _str): type(_type)
{
	value.s = _str;
}

Token::Token(const int& _value) : type(Type::IntLiteral)
{
	value.i = _value;
}