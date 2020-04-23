#include "Scanner.h"
#include <cctype>
#include <string>

using namespace tkom;
using T = Token::Type;

Scanner::Scanner(Reader& _reader): reader(_reader)
{}

Token Scanner::nextToken()
{
	this->ignoreSpaces();

	Token::Position position = reader.getPosition();

	if (isalpha(reader.peek()) || reader.peek() == '_')
		return this->tryIdentifierOrKeyword(position);
	if (isdigit(reader.peek()))
		return this->tryIntegerLiteral(position);
	switch (reader.peek())
	{
	case '\0':
		return this->endOfFile(position);
	case '=':
		return this->tryEqualSign(position);
	case '!':
		return this->tryNegation(position);
	case '>':
		return this->tryGreaterSign(position);
	case '<':
		return this->tryLessSign(position);
	case '|':
		return this->tryOr(position);
	case '&':
		return this->tryAnd(position);
	case '/':
		return this->tryDivideSign(position);
	case '"':
		return this->tryStringLiteral(position);
	default:
		return this->trySimpleOperator(position);
	}
}

void Scanner::ignoreSpaces()
{
	while (isspace(reader.peek()))
		reader.next();
}

Token Scanner::endOfFile(const Token::Position& pos)
{
	if (reader.next() != '\0')
		throw std::exception("End of file cannot be reached");
	return Token{ T::Eof, pos };
}

Token Scanner::tryIdentifierOrKeyword(const Token::Position& pos)
{
	std::string buffer;

	buffer.push_back(reader.next());

	while (isalnum(reader.peek()) || reader.peek() == '_')
		buffer.push_back(reader.next());

	if (keywords.count(buffer) == 1)
		return Token{ keywords.at(buffer), pos };
	else
		return Token{ T::Identifier, pos, buffer };
}

Token Scanner::tryIntegerLiteral(const Token::Position& pos)
{	
	if (reader.peek() == '0')
	{
		reader.next();
		if (!isdigit(reader.peek()))
			return Token(T::IntLiteral, pos, 0);
		else
		{ //leading zero not allowed
			while (isdigit(reader.peek()))
				reader.next();
			return Token(T::Invalid, pos);
		}
	}

	std::string buffer;
	buffer.push_back(reader.next());
	
	while (isdigit(reader.peek()))
		buffer.push_back(reader.next());

	int i;
	try
	{
		i = std::stoi(buffer);
	}
	catch (std::out_of_range e)
	{
		throw std::out_of_range("Literal is too long for int value");
		return Token{ T::Invalid, pos };
	}
	return Token{ T::IntLiteral, pos, i };
}

Token Scanner::tryEqualSign(const Token::Position& pos)
{
	reader.next();
	if (reader.peek() == '=')
	{
		reader.next();
		return Token{ T::Equal, pos };
	}
	return Token{ T::Assigment, pos };
}

Token Scanner::tryLessSign(const Token::Position& pos)
{
	reader.next();
	if (reader.peek() == '=')
	{
		reader.next();
		return Token{ T::LessOrEqual, pos };
	}
	return Token{ T::Less, pos };
}

Token Scanner::tryGreaterSign(const Token::Position& pos)
{
	reader.next();
	if (reader.peek() == '=')
	{
		reader.next();
		return Token{ T::GreaterOrEqual, pos };
	}
	return Token{ T::Greater, pos };
}

Token Scanner::tryNegation(const Token::Position& pos)
{
	reader.next();
	if (reader.peek() == '=')
	{
		reader.next();
		return Token{ T::NotEqual, pos };
	}
	return Token{ T::Negation, pos };
}

Token Scanner::tryAnd(const Token::Position& pos)
{
	reader.next();
	if (reader.peek() == '&')
	{
		reader.next();
		return Token{ T::And, pos };
	}
	return Token{ T::Invalid, pos };
}

Token Scanner::tryOr(const Token::Position& pos)
{
	reader.next();
	if (reader.peek() == '|')
	{
		reader.next();
		return Token{ T::Or, pos };
	}
	return Token{ T::Invalid, pos };
}

Token Scanner::tryDivideSign(const Token::Position& pos)
{
	reader.next(); 
	if (reader.peek() == '/')
	{
		reader.next();
		while (reader.next() != '\n')
			continue;
		return this->nextToken();
	}
	return Token{ T::Divide, pos };
}

Token Scanner::tryStringLiteral(const Token::Position& pos)
{
	std::string buffer;
	reader.next();
	while (reader.peek() != '"')
		buffer.push_back(reader.next());
	reader.next();
	return Token{ T::StringLiteral, pos, buffer };
}

Token Scanner::trySimpleOperator(const Token::Position& pos)
{
	if (simpleSigns.count(reader.peek()) == 1)
	{
		return Token{ simpleSigns.at(reader.next()), pos };
	}
	return Token{ T::Invalid,pos };
}