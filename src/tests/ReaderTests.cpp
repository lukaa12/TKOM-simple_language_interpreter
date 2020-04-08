#include "..\Reader.h"
#include <boost/test/unit_test.hpp>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(FileReaderTests)

BOOST_AUTO_TEST_CASE(Invalid_source_file_name)
{
	FileReader reader("invalid.name");

	BOOST_CHECK_EQUAL(reader.next(), '\0');
}

BOOST_AUTO_TEST_SUITE_END()