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
	unsigned int line, col;

	ch = reader.next();

	while (isspace(ch))
		ch = reader.next();

	line = reader.getLine();
	col = reader.getCol() - 1;

	if (ch == '\0')
	{
		return Token{T::Eof, line, col};
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
			return Token{ keywords.at(buffer), line, col };
		}
		else
		{
			return Token{ T::Identifier, line, col, buffer };
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
			return Token{ line, col };
		}
		return Token{ T::IntLiteral, line, col, i };
	}
	switch (ch)
	{
	case '0' :
	{
		if (!isdigit(reader.peek()))
			return Token(T::IntLiteral, line, col, 0);
	}
	case '=':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::Equal, line, col };
		}
		return Token{ T::Assigment, line, col };
	}
	case '!':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::NotEqual, line, col };
		}
		return Token{ T::Negation, line, col };
	}
	case '>':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::GreaterOrEqual, line, col };
		}
		return Token{ T::Greater, line, col };
	}
	case '<':
	{
		if (reader.peek() == '=')
		{
			reader.next();
			return Token{ T::LessOrEqual, line, col };
		}
		return Token{ T::Less, line, col };
	}
	case '|':
	{
		if (reader.peek() == '|')
		{
			reader.next();
			return Token{ T::Or, line, col };
		}
		return Token{ line, col };
	}
	case '&':
	{
		if (reader.peek() == '&')
		{
			reader.next();
			return Token{ T::And, line, col };
		}
		return Token{ line, col };
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
		return Token{ T::Divide, line, col };
	}
	case '"':
	{
		std::string buffer;
		while (reader.peek() != '"')
			buffer.push_back(reader.next());
		reader.next();
		return Token{ T::StringLiteral, line, col, buffer };
	}
	default:
	{
		if (simpleSigns.count(ch) == 1)
		{
			return Token{ simpleSigns.at(ch), line, col };
		}
	}
	}
	return Token{line, col};
}