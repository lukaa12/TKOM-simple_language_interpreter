#include <boost/test/unit_test.hpp>
#include "..\Scanner.h"

#include <iostream>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(ScannerTests)

struct IdOnlyStream
{
	TestReader reader{ "Jeden_ID Drugi2 _next3" };
};

BOOST_AUTO_TEST_CASE(InitializationTest)
{
	FileReader reader{"test.txt"};

	Scanner scanner{ reader };
}

BOOST_FIXTURE_TEST_CASE(ScanningIdentificators, IdOnlyStream)
{
	Scanner scanner{ reader };

	Token token = scanner.nextToken();
	BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
	BOOST_CHECK_EQUAL(token.getStrVal(), "Jeden_ID");
}

BOOST_AUTO_TEST_SUITE_END()