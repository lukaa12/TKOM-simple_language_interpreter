#include "..\Reader.h"
#include <boost/test/unit_test.hpp>
#include <istream>
#include <sstream>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(FileReaderTests)


BOOST_AUTO_TEST_CASE(Invalid_source_stream)
{
	std::istringstream stream;
	stream.setstate(std::ios::failbit);
	BOOST_CHECK_THROW(Reader{ stream }, std::exception);
}

BOOST_AUTO_TEST_CASE(Open_empty_file)
{
	std::istringstream in;
	Reader reader{ in };
	BOOST_CHECK_EQUAL(reader.next(), '\0'); 
}

BOOST_AUTO_TEST_CASE(Peek_on_empty)
{
	std::istringstream in;
	Reader reader{ in };
	BOOST_CHECK_EQUAL(reader.peek(), '\0');
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

BOOST_AUTO_TEST_CASE(Get_initial_position_column_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };
	Token::Position position{};
	position.line = 1;
	position.column = 0;
	BOOST_CHECK_EQUAL(reader.getPosition().column, position.column);
}

BOOST_AUTO_TEST_CASE(Get_initial_position_line_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };
	Token::Position position{};
	position.line = 1;
	position.column = 0;
	BOOST_CHECK_EQUAL(reader.getPosition().line, position.line);
}

BOOST_AUTO_TEST_CASE(Get__position_column_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };
	
	for (int i = 0; i < 4; ++i)
		reader.next();

	Token::Position position{};
	position.line = 1;
	position.column = 4;
	BOOST_CHECK_EQUAL(reader.getPosition().column, position.column);
}

BOOST_AUTO_TEST_CASE(Get__position_line_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };

	for (int i = 0; i < 4; ++i)
		reader.next();

	Token::Position position{};
	position.line = 1;
	position.column = 4;
	BOOST_CHECK_EQUAL(reader.getPosition().line, position.line);
}

BOOST_AUTO_TEST_CASE(Get_next_line_position_column_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };

	for (int i = 0; i < 25; ++i)
		reader.next();

	Token::Position position{};
	position.line = 2;
	position.column = 4;
	BOOST_CHECK_EQUAL(reader.getPosition().column, position.column);
}

BOOST_AUTO_TEST_CASE(Get_next_line_position_line_test)
{
	std::istringstream in{ "This is test example\nSecond line x 14th col in x" };
	Reader reader{ in };

	for (int i = 0; i < 25; ++i)
		reader.next();

	Token::Position position{};
	position.line = 2;
	position.column = 4;
	BOOST_CHECK_EQUAL(reader.getPosition().line , position.line);
}

BOOST_AUTO_TEST_SUITE_END()