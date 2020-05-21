#include <boost/test/auto_unit_test.hpp>
#include "../Executor.h"
#include "../Parser.h"
#include "../Error.h"
#include <string>
#include <iostream>
#include <memory>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(Execution_tests)

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

BOOST_FIXTURE_TEST_CASE(Symbol_table_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	int x = 0;
}

string fun(int i)
{
	string name = "Imie";
}
)");
	
	
	Executor executor{ program.release() };
	BOOST_CHECK_NO_THROW(executor.symbolTable.getSymbol("main"));
	BOOST_CHECK_NO_THROW(executor.symbolTable.getSymbol("fun"));
	BOOST_CHECK_THROW(executor.symbolTable.getSymbol("x"), std::exception);
	BOOST_CHECK_THROW(executor.symbolTable.getSymbol("name"), std::exception);

}

BOOST_FIXTURE_TEST_CASE(No_main_function_test, ExecutionFix)
{
	auto program = getProgram(R"(
string fun(int i)
{
	string name = "Imie";
}
)");

	Executor executor{ program.release() };
	BOOST_CHECK_EXCEPTION(executor.execute(), Error, [](const Error& e)->bool {
		return std::string{ "Main function not found" }.compare(e.what()) == 0 ? true : false;
	});
}

BOOST_AUTO_TEST_SUITE_END()