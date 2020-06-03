#include "../AstNode.h"
#include <boost/test/unit_test.hpp>

using namespace tkom::ast;

BOOST_AUTO_TEST_SUITE(Expressions_tests)

BOOST_AUTO_TEST_CASE(Primary_expression_with_literal_test)
{
	PrimaryExpression expression;

	expression.setType(PrimaryExpression::Type::Literal);
	expression.setValue(100);
	BOOST_CHECK_EQUAL(expression.getValue<int>(0), 100);
}

BOOST_AUTO_TEST_CASE(Expressions_hierarchy)
{
	auto expr1 = std::make_unique<BracketExpression>();
	auto expr2 = std::make_unique<Expression>();
	auto expr3 = std::make_unique<MultiplicativeExpression>();
	auto expr4 = std::make_unique<PrimaryExpression>();

	expr4->setType(PrimaryExpression::Type::Identifier);
	expr4->setValue<std::string>("id");

	expr3->addComponent(std::move(expr4));
	expr2->addComponent(std::make_pair(false, std::move(expr3)));
	expr1->setExpression(std::move(expr2));

	BOOST_CHECK(expr1->getExpression()->getComponents()[0].second->getComponents()[0].get() != nullptr);
}

BOOST_AUTO_TEST_CASE(RightVal_test)
{
	auto rval = std::make_unique<RightValue>();
	rval->setType(RightValue::Type::Expression);
	rval->setValue(std::make_unique<Expression>());
	BOOST_CHECK_NO_THROW(rval->getValue<Expression>());
}

BOOST_AUTO_TEST_SUITE_END()