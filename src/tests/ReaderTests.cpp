#include "..\Reader.h"
#include <boost/test/unit_test.hpp>
#include <cstdio>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(FileReaderTests)

struct ReaderTestFixtureFile
{
	ReaderTestFixtureFile()
	{
		std::ofstream test1;
		test1.open("test_1.txt");
		if (test1.fail())
			throw std::exception("Fixture failed");
		test1 << "This is test example" << std::endl << "Second line x 14th col in x";
	}

	~ReaderTestFixtureFile()
	{
		if (std::remove("test_1.txt"))
			throw std::exception("Fixture cleanup failed");
	}
};

struct ReaderTestEmptyFileFixture
{
	ReaderTestEmptyFileFixture()
	{
		std::ofstream testEmpty;
		testEmpty.open("test_empty.txt");
		if (testEmpty.fail())
			throw std::exception("Fixture failed");
	}

	~ReaderTestEmptyFileFixture()
	{
		if (std::remove("test_empty.txt"))
			throw std::exception("Fixture cleanup failed");
	}
};

BOOST_AUTO_TEST_CASE(Invalid_source_file_name)
{
	BOOST_CHECK_THROW(FileReader{"invalid.name"}, std::exception);
}

BOOST_FIXTURE_TEST_CASE(Open_source_file, ReaderTestFixtureFile)
{
	BOOST_CHECK_NO_THROW(FileReader{ "test_1.txt" });
}

BOOST_FIXTURE_TEST_CASE(Open_empty_file, ReaderTestEmptyFileFixture)
{
	FileReader reader{ "test_empty.txt" };
	BOOST_CHECK_EQUAL(reader.next(), '\0'); 
}

BOOST_FIXTURE_TEST_CASE(ReadFromFile, ReaderTestFixtureFile)
{
	FileReader reader{ "test_1.txt" };
	std::string test_str{ "This is test example\nSecond line x 14th col in x" };
	for (auto i = 0; i < test_str.size(); ++i)
		BOOST_CHECK_EQUAL(reader.next(), test_str[i]);
}

BOOST_FIXTURE_TEST_CASE(PeekTest, ReaderTestFixtureFile)
{
	FileReader reader{ "test_1.txt" };
	std::string test_str{ "This is test example\nSecond line x 14th col in x" };
	BOOST_CHECK_EQUAL(reader.peek(), test_str[0]);
	for (auto i = 0; i != 10; ++i)
		reader.next();
	BOOST_CHECK_EQUAL(reader.peek(), test_str[10]);
	char p = reader.peek();
	BOOST_CHECK_EQUAL(p, reader.next());
}

BOOST_FIXTURE_TEST_CASE(PeekTest2, ReaderTestFixtureFile)
{
	FileReader reader{ "test_1.txt" };
	std::string test_str{ "This is test example\nSecond line x 14th col in x" };
	for (auto i = 0; i < test_str.size(); ++i)
	{
		BOOST_CHECK_EQUAL(reader.peek(), test_str[i]);
		reader.next();
	}
}

BOOST_FIXTURE_TEST_CASE(PeekAndNextCompatibilityTest, ReaderTestFixtureFile)
{
	FileReader reader{ "test_1.txt" };
	char p;
	while ((p = reader.peek()) != '\0')
	{
		BOOST_CHECK_EQUAL(p, reader.next());
	}
}

BOOST_FIXTURE_TEST_CASE(GetLineTest, ReaderTestFixtureFile)
{
	FileReader reader{ "test_1.txt" };
	BOOST_CHECK_EQUAL(reader.getLine(), 1);
	for (auto i = 0; i != 25; ++i)
		reader.next();
	BOOST_CHECK_EQUAL(reader.getLine(), 2);
}

BOOST_FIXTURE_TEST_CASE(GetColumnTest, ReaderTestFixtureFile)
{
	FileReader reader{ "test_1.txt" };
	BOOST_CHECK_EQUAL(reader.getCol(), 0);
	while (reader.next() != '\n')
		continue;
	for (auto i = 0; i < 15; ++i)
	{
		BOOST_CHECK_EQUAL(reader.getCol(), i);
		reader.next();
	}
}

BOOST_AUTO_TEST_SUITE_END()