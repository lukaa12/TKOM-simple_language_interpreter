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

BOOST_AUTO_TEST_CASE(White_spaces_ignore_test)
{
	std::stringstream in{"		\n   ^"};
	Reader reader{ in };
	Scanner scanner{ reader };
	Token t = scanner.nextToken();
	BOOST_CHECK_EQUAL(t.getPosition().line, 2);
	BOOST_CHECK_EQUAL(t.getPosition().column, 3);
}

BOOST_AUTO_TEST_SUITE_END()