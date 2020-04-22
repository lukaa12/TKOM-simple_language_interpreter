#include "Scanner.h"
#include <cctype>
#include <string>

using namespace tkom;
using T = Token::Type;

Scanner::Scanner(Reader& _reader): reader(_reader)
{}

Token Scanner::nextToken()
{
	char ch, nextCh;
	Token::Position position = reader.getPosition();
	ch = reader.next();

	while (isspace(ch))
		ch = reader.next();
	

	if (ch == '\0')
	{
		return Token{T::Eof, position};
	}
	
	if (isalpha(ch) || ch == '_')
	{
		std::string buffer;

		buffer.push_back(ch);
		nextCh = reader.peek();
		while (isalnum(nextCh) || nextCh == '_')
		{
			ch = reader.next();
			buffer.push_back(ch);
			nextCh = reader.peek();
		}

		if (keywords.count(buffer) == 1)
		{
			return Token{ keywords.at(buffer), position };
		}
		else
		{
			return Token{ T::Identifier, position, buffer };
		}
	}
	if (isdigit(ch) && ch - '0' != 0)
	{
		std::string buffer;
		buffer.push_back(ch);
		nextCh = reader.peek();
		while (isdigit(nextCh))
		{
			ch = reader.next();
			buffer.push_back(ch);
			nextCh = reader.peek();
		}
		int i;
		try 
		{
			i = std::stoi(buffer);
		}
		catch (std::out_of_range e)
		{
			throw std::out_of_range("Literal is too long for int value");
			return Token{ position };
		}
		return Token{ T::IntLiteral, position, i };
	}
	switch (ch)
	{
	case '0' :
	{
		if (!isdigit(reader.peek()))
			return Token(T::IntLiteral, position, 0);
	}
	case '=':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::Equal, position };
		}
		return Token{ T::Assigment, position };
	}
	case '!':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::NotEqual, position };
		}
		return Token{ T::Negation, position };
	}
	case '>':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::GreaterOrEqual, position };
		}
		return Token{ T::Greater, position };
	}
	case '<':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::LessOrEqual, position };
		}
		return Token{ T::Less, position };
	}
	case '|':
	{
		if (reader.peek() == '|')
		{
			reader.next();
			return Token{ T::Or, position };
		}
		return Token{ position };
	}
	case '&':
	{
		if (reader.peek() == '&')
		{
			reader.next();
			return Token{ T::And, position };
		}
		return Token{ position };
	}
	case '/':
	{
		if (reader.peek() == '/')
		{
			reader.next();
			while (reader.next() != '\n')
				continue;
			return this->nextToken();
		}
		return Token{ T::Divide, position };
	}
	case '"':
	{
		std::string buffer;
		while (reader.peek() != '"')
			buffer.push_back(reader.next());
		reader.next();
		return Token{ T::StringLiteral, position, buffer };
	}
	default:
	{
		if (simpleSigns.count(ch) == 1)
		{
			return Token{ simpleSigns.at(ch), position };
		}
	}
	}
	return Token{ position };
}