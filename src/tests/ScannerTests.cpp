#include <boost/test/unit_test.hpp>
#include "..\Scanner.h"
#include <sstream>
#include <iostream>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(ScannerTests)


BOOST_AUTO_TEST_CASE(Initialization_test)
{
	std::stringstream in{};
	Reader reader{ in };
	BOOST_CHECK_NO_THROW(Scanner{ reader });
}

BOOST_AUTO_TEST_CASE(End_of_file_Token)
{
	std::stringstream in{ "" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(White_spaces_ignore_test)
{
	std::stringstream in{"		\n   ^"};
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getPosition().line, 2);
	BOOST_CHECK_EQUAL(t.getPosition().column, 3);
}

BOOST_AUTO_TEST_CASE(Token_position_test)
{
	std::stringstream in{ "\n\n\n       token" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getPosition().line, 4);
	BOOST_CHECK_EQUAL(t.getPosition().column, 7);
}

BOOST_AUTO_TEST_CASE(Comment_ignore_test)
{
	std::stringstream in{ "//this is comment\n0" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntLiteral);
	BOOST_CHECK_EQUAL(t.getPosition().line, 2);
}

BOOST_AUTO_TEST_CASE(Integer_literal_scanning_test)
{
	std::stringstream in{ "10" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntLiteral);
	BOOST_CHECK_EQUAL(t.getIntVal(), 10);
}

BOOST_AUTO_TEST_CASE(Following_zero_test)
{
	std::stringstream in{ "0123" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::Invalid);
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(Zero_test)
{
	std::stringstream in{ " 0 " };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntLiteral);
	BOOST_CHECK_EQUAL(t.getIntVal(), 0);
}

BOOST_AUTO_TEST_CASE(Int_out_of_range_test)
{
	std::stringstream in{ " 999999999999999999999999999999999 " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_THROW(scanner.nextToken(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(String_literal_test)
{
	std::stringstream in{ " \"Test of string\" " };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::StringLiteral);
	BOOST_CHECK_EQUAL(t.getStrVal(), "Test of string");
}

BOOST_AUTO_TEST_CASE(Identifier_test)
{
	std::stringstream in{ " identifier Id0 _Next " };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
	BOOST_CHECK_EQUAL(t.getStrVal(), "identifier");
	t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
	t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
	t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(Keyword_test)
{
	std::stringstream in{ " while " };
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getType(), Token::Type::While);
}

BOOST_AUTO_TEST_CASE(Equal_sign_test)
{
	std::stringstream in{ " = == " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Assigment);
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Equal);
}

BOOST_AUTO_TEST_CASE(Negation_sign_test)
{
	std::stringstream in{ " ! != " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Negation);
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::NotEqual);
}

BOOST_AUTO_TEST_CASE(Greater_sign_test)
{
	std::stringstream in{ " > >= " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Greater);
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::GreaterOrEqual);
}

BOOST_AUTO_TEST_CASE(Less_sign_test)
{
	std::stringstream in{ " < <= " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Less);
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::LessOrEqual);
}

BOOST_AUTO_TEST_CASE(And_operator_test)
{
	std::stringstream in{ " && " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::And);
}

BOOST_AUTO_TEST_CASE(Or_operator_test)
{
	std::stringstream in{ " || " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Or);
}

BOOST_AUTO_TEST_CASE(Divide_operator_test)
{
	std::stringstream in{ " // \n / " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Divide);
}

BOOST_AUTO_TEST_CASE(Simple_operator_test)
{
	std::stringstream in{ " * " };
	Reader reader{ in };
	Scanner scanner{ reader };
	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::Multiply);
}

BOOST_AUTO_TEST_CASE(Tokens_without_white_spaces)
{
	std::stringstream in{ "\"Hello\"123,0;,int-==Na_m4e" };
	Reader reader{ in };
	Scanner scanner{ reader };
	std::vector<Token::Type> tokens{ Token::Type::StringLiteral, Token::Type::IntLiteral, Token::Type::Comma, Token::Type::IntLiteral,
									 Token::Type::Semicolon, Token::Type::Comma, Token::Type::IntType, Token::Type::Minus,
									 Token::Type::Equal, Token::Type::Identifier };
	std::vector<Token::Type> fromScanner;
	Token t = scanner.nextToken();

	while (t.getType() != Token::Type::Eof)
	{
		fromScanner.push_back(t.getType());
		t = scanner.nextToken();
	}
	BOOST_CHECK_EQUAL_COLLECTIONS(tokens.begin(), tokens.end(), fromScanner.begin(), fromScanner.end());
}

BOOST_AUTO_TEST_SUITE_END()