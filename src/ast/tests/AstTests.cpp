#include "../AstNode.h"
#include <boost/test/unit_test.hpp>

using namespace tkom::ast;

BOOST_AUTO_TEST_SUITE(Expressions_tests)

BOOST_AUTO_TEST_CASE(Primary_expression_with_literal_test)
{
	PrimaryExpression expression;

	expression.type = PrimaryExpression::Type::Literal;
	expression.value = 100;
	BOOST_CHECK_EQUAL(std::get<int>(expression.value), 100);
}

//TODO

BOOST_AUTO_TEST_SUITE_END()