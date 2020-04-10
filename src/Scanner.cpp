#include "Scanner.h"
#include <cctype>

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

	return Token{line, col};
}