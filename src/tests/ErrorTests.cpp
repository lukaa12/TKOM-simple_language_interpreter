#include "../Error.h"
#include <boost/test/auto_unit_test.hpp>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(Error_tests)

void func()
{
	throw Error(Token::Position{ 3, 0 }, Error::Type::IncorrectParametersList);
}

BOOST_AUTO_TEST_CASE(Error_creation_test)
{
	Error error(Token::Position{1, 0}, Error::Type::MissingStatement);

	BOOST_CHECK_EQUAL(error.what(), "Line: 1 pos: 0 Missing statement: ");
}

BOOST_AUTO_TEST_CASE(Error_creation_test2)
{
	Token token(Token::Type::Invalid, Token::Position{ 2, 3 });
	Error error(token, Error::Type::UnexpectedToken);

	BOOST_CHECK_EQUAL(error.what(), "Line: 2 pos: 3 Invalid token");
}

BOOST_AUTO_TEST_CASE(Error_throwing_test)
{
	BOOST_CHECK_THROW(func(), Error);
}

BOOST_AUTO_TEST_CASE(Error_throwing_test2)
{
	try
	{
		func();
	}
	catch (const Error& e)
	{
		BOOST_CHECK_EQUAL(e.what(), "Line: 3 pos: 0 Incorrect parameters for function: ");
	}
}

BOOST_AUTO_TEST_SUITE_END()