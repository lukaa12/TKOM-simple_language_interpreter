//#include <boost/test/unit_test.hpp>
//#include "..\Scanner.h"
//
//#include <iostream>
//
//using namespace tkom;
//
//BOOST_AUTO_TEST_SUITE(ScannerTests)
//
//struct IdOnlyStream
//{
//	TestReader reader{ "Jeden_ID Drugi2 _next3" };
//};
//
//struct KeywordsStream
//{
//	TestReader reader{ "if while else return break \nif return\n   int string graphic\n color" };
//};
//
//BOOST_AUTO_TEST_CASE(InitializationTest)
//{
//	FileReader reader{"test.txt"};
//	BOOST_CHECK_NO_THROW(Scanner{ reader });
//}
//
//BOOST_FIXTURE_TEST_CASE(ScanningIdentificators, IdOnlyStream)
//{
//	Scanner scanner{ reader };
//
//	std::vector<Token> tokens{};
//	std::string names[] = { "Jeden_ID", "Drugi2", "_next3" };
//	Token t{ 1, 1 };
//	int i = 0;
//	while (t.getType() != Token::Type::Eof)
//	{
//		t = scanner.nextToken();
//		tokens.push_back(t);
//		if (i < 3)
//		{
//			BOOST_CHECK_EQUAL(t.getStrVal(), names[i]);
//		}
//		++i;
//	}
//	BOOST_CHECK_EQUAL(i, 4);
//
//	for (auto i = tokens.begin(); i != --tokens.end(); ++i)
//	{
//		BOOST_CHECK_EQUAL(i->getType(), Token::Type::Identifier);
//	}
//
//}
//
//BOOST_FIXTURE_TEST_CASE(ScanningKeywords, KeywordsStream)
//{
//	Scanner scanner{ reader };
//
//	Token::Type types[] = { Token::Type::If, Token::Type::While, Token::Type::Else, Token::Type::Return, Token::Type::Break,
//							Token::Type::If, Token::Type::Return, Token::Type::IntType, Token::Type::StringType, Token::Type::GraphicType,
//							Token::Type::ColorType };
//	std::vector<Token> tokens{};
//	Token t = scanner.nextToken();
//	while (t.getType() != Token::Type::Eof)
//	{
//		tokens.push_back(t);
//		t = scanner.nextToken();
//	}
//
//	for (auto i = 0; i < tokens.size(); ++i)
//	{
//		BOOST_CHECK_EQUAL(tokens[i].getType(), types[i]);
//	}
//}
//
//BOOST_AUTO_TEST_CASE(ScannerCommentTest)
//{
//	TestReader reader{ "//this is comment\nint x =10;" };
//	Scanner scanner{ reader };
//
//	BOOST_CHECK_EQUAL(scanner.nextToken().getType(), Token::Type::IntType);
//	BOOST_CHECK_THROW(scanner.nextToken().getIntVal(), std::exception);
//	BOOST_CHECK_THROW(scanner.nextToken().getStrVal(), std::exception);
//
//}
//
//BOOST_AUTO_TEST_CASE(StringLiteralTest)
//{
//	TestReader reader{ "\"This is string literal\"" };
//	Scanner scanner{ reader };
//
//	Token t = scanner.nextToken();
//	BOOST_CHECK_EQUAL(t.getType(), Token::Type::StringLiteral);
//	BOOST_CHECK_EQUAL(t.getStrVal(), "This is string literal");
//	BOOST_CHECK_THROW(t.getIntVal(), std::exception);
//}
//
//BOOST_AUTO_TEST_CASE(IntLiteralsSeparatedCommas)
//{
//	TestReader reader{ "101,50,0,10" };
//	Scanner scanner{ reader };
//
//	for (int i = 0; i < 7; ++i)
//	{
//		Token t = scanner.nextToken();
//		if (i % 2 == 0)
//			BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntLiteral);
//		else
//			BOOST_CHECK_EQUAL(t.getType(), Token::Type::Comma);
//		switch (i)
//		{
//		case 0:
//			BOOST_CHECK_EQUAL(t.getIntVal(), 101);
//			break;
//		case 2:
//			BOOST_CHECK_EQUAL(t.getIntVal(), 50);
//			break;
//		case 4:
//			BOOST_CHECK_EQUAL(t.getIntVal(), 0);
//			break;
//		case 6:
//			BOOST_CHECK_EQUAL(t.getIntVal(), 10);
//			break;
//		}
//	}
//}
//
//BOOST_AUTO_TEST_SUITE_END()