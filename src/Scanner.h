#ifndef SCANNER_H
#define SCANNER_H

#include "Reader.h"
#include "Token.h"

namespace tkom {

	class Scanner
	{
	public:
		Scanner(Reader& _reader);
		Token nextToken();
	private:
		Reader& reader;

		void ignoreSpaces();
		Token endOfFile(const Token::Position &pos);
		Token tryIdentifierOrKeyword(const Token::Position& pos);
		Token tryIntegerLiteral(const Token::Position& pos);
		Token tryEqualSign(const Token::Position& pos);
		Token tryLessSign(const Token::Position& pos);
		Token tryGreaterSign(const Token::Position& pos);
		Token tryNegation(const Token::Position& pos);
		Token tryAnd(const Token::Position& pos);
		Token tryOr(const Token::Position& pos);
		Token tryDivideSign(const Token::Position& pos);
		Token tryStringLiteral(const Token::Position& pos);
		Token trySimpleOperator(const Token::Position& pos);
	};
}


#endif // !SCANNER_H