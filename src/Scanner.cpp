#include "Scanner.h"
#include <cctype>

using namespace tkom;

Scanner::Scanner(Reader& _reader): reader(_reader)
{}

const Token& Scanner::nextToken()
{
	char active, next;
	Token token;

	active = reader.next();

	while (isspace(active))
		active = reader.next();

	if (active == '(')
	{
		token.type = Token::Type::BracketOpen;
		token.line = reader.getLine();
		token.column = reader.getCol();
		return token;
	}
}