#include <boost/test/unit_test.hpp>
#include "..\Scanner.h"

using namespace tkom;

BOOST_AUTO_TEST_SUITE(ScannerTests)

BOOST_AUTO_TEST_CASE(InitializationTest)
{
	FileReader reader{"test.txt"};

	Scanner scanner{ reader };
}

BOOST_AUTO_TEST_SUITE_END()