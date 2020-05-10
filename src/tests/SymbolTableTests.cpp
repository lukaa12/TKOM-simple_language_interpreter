#include "../SymbolTable.h"
#include <boost/test/unit_test.hpp>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(SymbolTableTests)

BOOST_AUTO_TEST_CASE(Try_to_get_symbol_from_empty)
{
	SymbolTable table;

	BOOST_CHECK_THROW(table.getSymbol("id"), std::exception);
}

BOOST_AUTO_TEST_CASE(Add_symbol_to_global_scope)
{
	SymbolTable table;

	BOOST_CHECK_NO_THROW(table.addGlobalSymbol(Symbol(ast::DataType::Int, "i")));
}

BOOST_AUTO_TEST_CASE(Get_symbol_from_global)
{
	SymbolTable table;
	table.addGlobalSymbol(Symbol(ast::DataType::Int, "i"));

	Symbol symbol;

	BOOST_CHECK_NO_THROW(symbol = table.getSymbol("i"));
	BOOST_CHECK_EQUAL(symbol.dataType, ast::DataType::Int);
	BOOST_CHECK_EQUAL(symbol.type, ast::IdType::Variable);
	BOOST_CHECK_EQUAL(symbol.identifier, "i");
}

BOOST_AUTO_TEST_CASE(Try_to_get_uninitialized_symbol)
{
	SymbolTable table;
	table.addGlobalSymbol(Symbol(ast::DataType::Int, "i"));

	BOOST_CHECK_THROW(table.getSymbol("j"), std::exception);
}

BOOST_AUTO_TEST_CASE(Adding_local_symbols_test)
{
	SymbolTable table;
	table.enterScope();
	BOOST_CHECK_NO_THROW(table.addLocalSymbol(Symbol(ast::DataType::Int, "i")));
}

BOOST_AUTO_TEST_CASE(Overlapping_symbols_names_test)
{
	SymbolTable table;
	table.addGlobalSymbol(Symbol(ast::DataType::Int, "i"));
	table.enterScope();
	table.addGlobalSymbol(Symbol(ast::DataType::Int, "j"));

	BOOST_CHECK_EQUAL(table.getSymbol("i").dataType, ast::DataType::Int);
	BOOST_CHECK_EQUAL(table.getSymbol("j").dataType, ast::DataType::Int);

	BOOST_CHECK_NO_THROW(table.addLocalSymbol(Symbol(ast::DataType::String, "i")));

	BOOST_CHECK_EQUAL(table.getSymbol("i").dataType, ast::DataType::String);

	table.enterScope();
	BOOST_CHECK_NO_THROW(table.addLocalSymbol(Symbol(ast::DataType::Color, "i")));

	BOOST_CHECK_EQUAL(table.getSymbol("i").dataType, ast::DataType::Color);

	table.leaveScope();
	BOOST_CHECK_EQUAL(table.getSymbol("i").dataType, ast::DataType::String);
	table.leaveScope();
	BOOST_CHECK_EQUAL(table.getSymbol("i").dataType, ast::DataType::Int);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ScopeTest)

BOOST_AUTO_TEST_CASE(Adding_symbol_test)
{
	Scope scope;

	BOOST_CHECK_NO_THROW(scope.addSymbol(Symbol(ast::DataType::Int, "i")));
}

BOOST_AUTO_TEST_CASE(Getting_symbol_from_empty)
{
	Scope scope;

	BOOST_CHECK_EQUAL(scope.getSymbol("id").identifier, "!");
}

BOOST_AUTO_TEST_CASE(Getting_symbol_from_scope)
{
	Scope scope;
	scope.addSymbol(Symbol(ast::DataType::Int, "i"));

	Symbol symbol = scope.getSymbol("i");

	BOOST_CHECK_EQUAL(symbol.dataType, ast::DataType::Int);
	BOOST_CHECK_EQUAL(symbol.type, ast::IdType::Variable);
	BOOST_CHECK_EQUAL(symbol.identifier, "i");
}

BOOST_AUTO_TEST_CASE(Trying_to_get_uninitialized_symbol)
{
	Scope scope;
	scope.addSymbol(Symbol(ast::DataType::Int, "i"));

	BOOST_CHECK_EQUAL(scope.getSymbol("j").identifier, "!");
}

BOOST_AUTO_TEST_CASE(Redefining_the_symbol)
{
	Scope scope;
	scope.addSymbol(Symbol(ast::DataType::Int, "i"));
	BOOST_CHECK_THROW(scope.addSymbol(Symbol(ast::DataType::Int, "i")), std::exception);
}

BOOST_AUTO_TEST_CASE(More_symbols_in_scope)
{
	Scope scope;
	scope.addSymbol(Symbol(ast::DataType::Int, "i"));
	scope.addSymbol(Symbol(ast::DataType::Int, "j"));
	scope.addSymbol(Symbol(ast::DataType::String, "name"));
	scope.addSymbol(Symbol(ast::DataType::Int, "func"));

	Symbol symbol = scope.getSymbol("j");

	BOOST_CHECK_EQUAL(symbol.dataType, ast::DataType::Int);
	BOOST_CHECK_EQUAL(symbol.type, ast::IdType::Variable);
	BOOST_CHECK_EQUAL(symbol.identifier, "j");
}

BOOST_AUTO_TEST_SUITE_END()