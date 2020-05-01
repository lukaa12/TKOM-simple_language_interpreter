#include "../AstNode.h"
#include <boost/test/unit_test.hpp>

using namespace tkom::ast;

BOOST_AUTO_TEST_SUITE(Expressions_tests)

BOOST_AUTO_TEST_CASE(Primary_expression_with_literal_test)
{
	PrimaryExpression expression;

	expression.setType(PrimaryExpression::Type::Literal);
	expression.setValue(100);
	BOOST_CHECK_EQUAL(expression.getValue<int>(), 100);
}

BOOST_AUTO_TEST_CASE(Expressions_hierarchy)
{
	auto expr1 = std::make_shared<BracketExpression>();
	auto expr2 = std::make_shared<Expression>();
	auto expr3 = std::make_shared<MultiplicativeExpression>();
	auto expr4 = std::make_shared<PrimaryExpression>();

	expr4->setType(PrimaryExpression::Type::Identifier);
	expr4->setValue<std::string>("id");

	expr3->addComponent(expr4);
	expr2->addComponent(std::make_pair(false, expr3));
	expr1->setExpression(expr2);
	
	BOOST_CHECK_EQUAL(expr4, expr3->getComponents()[0]);
	BOOST_CHECK_EQUAL(expr3, expr2->getComponents()[0].second);
	BOOST_CHECK_EQUAL(expr2, expr1->getExpression());
}

BOOST_AUTO_TEST_SUITE_END()