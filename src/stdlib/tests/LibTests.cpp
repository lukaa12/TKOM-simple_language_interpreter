#include <boost/test/auto_unit_test.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "../../Parser.h"
#include "../../Executor.h"
#include "../../Error.h"

using namespace tkom;
using namespace ast;

BOOST_AUTO_TEST_SUITE(Data_types_tests)

class ExecutionFix
{
public:

	std::unique_ptr<ast::Program> getProgram(std::string prog)
	{
		in.reset(new std::stringstream(prog));
		reader.reset(new Reader(*in));
		scanner.reset(new Scanner(*reader));
		parser.reset(new Parser(*scanner));

		return parser->parse();
	}

	SymbolTable& getSymbolTable()
	{
		return parser->getTable();
	}

	std::unique_ptr<std::stringstream> in;
	std::unique_ptr<Reader> reader;
	std::unique_ptr<Scanner> scanner;
	std::unique_ptr<Parser> parser;
};

BOOST_FIXTURE_TEST_CASE(Color_type_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	color col1;
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());

}

BOOST_FIXTURE_TEST_CASE(Graphic_type_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic circle;
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Library_functions_tests)

class ExecutionFix
{
public:

	std::unique_ptr<ast::Program> getProgram(std::string prog)
	{
		in.reset(new std::stringstream(prog));
		reader.reset(new Reader(*in));
		scanner.reset(new Scanner(*reader));
		parser.reset(new Parser(*scanner));

		return parser->parse();
	}

	SymbolTable& getSymbolTable()
	{
		return parser->getTable();
	}

	std::unique_ptr<std::stringstream> in;
	std::unique_ptr<Reader> reader;
	std::unique_ptr<Scanner> scanner;
	std::unique_ptr<Parser> parser;
};

BOOST_FIXTURE_TEST_CASE(Print_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	print("Hello World!");
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_SUITE_END()