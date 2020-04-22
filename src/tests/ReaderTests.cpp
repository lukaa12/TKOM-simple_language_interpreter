#include "..\Reader.h"
#include <boost/test/unit_test.hpp>
#include <istream>
#include <sstream>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(FileReaderTests)


//BOOST_AUTO_TEST_CASE(Invalid_source_file_name)
//{
//	BOOST_CHECK_THROW(FileReader{"invalid.name"}, std::exception);
//}

//BOOST_AUTO_TEST_CASE(Open_source_file)
//{
//	BOOST_CHECK_NO_THROW(FileReader{ "test_1.txt" });
//}

BOOST_AUTO_TEST_CASE(Open_empty_file)
{
	std::istringstream in;
	Reader reader{ in };
	BOOST_CHECK_EQUAL(reader.next(), '\0'); 
}

BOOST_AUTO_TEST_CASE(Read_from_stream)
{
	std::string test_str{ "This is test example\nSecond line x 14th col in x" };
	std::string readed;
	std::istringstream in{ test_str };
	Reader reader{ in };
	for (auto i = 0; i < test_str.size(); ++i)
		readed.push_back(reader.next());
	
	BOOST_CHECK_EQUAL_COLLECTIONS(test_str.begin(), test_str.end(), readed.begin(), readed.end());
}

BOOST_AUTO_TEST_CASE(Peek_test)
{
	std::string test_str{ "This is test example\nSecond line x 14th col in x" };
	std::string readed;
	std::istringstream in{ test_str };
	Reader reader{ in };
	for (auto i = 0; i < test_str.size(); ++i)
	{
		readed.push_back(reader.peek());
		reader.next();
	}
	BOOST_CHECK_EQUAL_COLLECTIONS(test_str.begin(), test_str.end(), readed.begin(), readed.end());
}

BOOST_AUTO_TEST_CASE(Peek_and_next_compatibility_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };
	std::string readed, peeked;
	while (reader.peek() != '\0')
	{
		peeked.push_back(reader.peek());
		readed.push_back(reader.next());
	}
	BOOST_CHECK_EQUAL_COLLECTIONS(peeked.begin(), peeked.end(), readed.begin(), readed.end());
}

//BOOST_FIXTURE_TEST_CASE(PeekTest2, ReaderTestFixtureFile)
//{
//	FileReader reader{ "test_1.txt" };
//	std::string test_str{ "This is test example\nSecond line x 14th col in x" };
//	for (auto i = 0; i < test_str.size(); ++i)
//	{
//		BOOST_CHECK_EQUAL(reader.peek(), test_str[i]);
//		reader.next();
//	}
//}
//
//BOOST_FIXTURE_TEST_CASE(PeekAndNextCompatibilityTest, ReaderTestFixtureFile)
//{
//	FileReader reader{ "test_1.txt" };
//	char p;
//	while ((p = reader.peek()) != '\0')
//	{
//		BOOST_CHECK_EQUAL(p, reader.next());
//	}
//}
//
//BOOST_FIXTURE_TEST_CASE(GetLineTest, ReaderTestFixtureFile)
//{
//	FileReader reader{ "test_1.txt" };
//	BOOST_CHECK_EQUAL(reader.getLine(), 1);
//	for (auto i = 0; i != 25; ++i)
//		reader.next();
//	BOOST_CHECK_EQUAL(reader.getLine(), 2);
//}
//
//BOOST_FIXTURE_TEST_CASE(GetColumnTest, ReaderTestFixtureFile)
//{
//	FileReader reader{ "test_1.txt" };
//	BOOST_CHECK_EQUAL(reader.getCol(), 0);
//	while (reader.next() != '\n')
//		continue;
//	for (auto i = 0; i < 15; ++i)
//	{
//		BOOST_CHECK_EQUAL(reader.getCol(), i);
//		reader.next();
//	}
//}

BOOST_AUTO_TEST_SUITE_END()