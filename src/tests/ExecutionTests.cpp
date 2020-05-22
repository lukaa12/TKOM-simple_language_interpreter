#include <boost/test/auto_unit_test.hpp>
#include "../Executor.h"
#include "../Parser.h"
#include "../Error.h"
#include <string>
#include <iostream>
#include <memory>

using namespace tkom;
using namespace ast;

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

class RightValFix
{
public:
	std::unique_ptr<RightValue> getStringRval()
	{
		auto rv = std::make_unique<RightValue>();
		rv->setType(RightValue::Type::StringLiteral);
		rv->setValue<std::string>("test string");
		return std::move(rv);
	}
	std::unique_ptr<RightValue> getIntRval(int i)
	{
		auto rv = std::make_unique<RightValue>();
		rv->setType(RightValue::Type::IntLiteral);
		rv->setValue<int>(i);
		return std::move(rv);
	}
};

class RelationCondFix
{
public:
	std::unique_ptr<RelationCondition> getRelationCond(bool val)
	{
		auto rv = std::make_unique<RightValue>();
		rv->setType(RightValue::Type::IntLiteral);
		rv->setValue<int>(val ? 1 : 0);
		auto cond = std::make_unique<RelationCondition>();
		auto primary = std::make_unique<PrimaryCondition>();
		primary->setType(PrimaryCondition::Type::RightVal);
		primary->setNegated(false);
		primary->setCondition(std::move(rv));
		cond->setFirst(std::move(primary));
		return std::move(cond);
	}
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

BOOST_AUTO_TEST_CASE(Right_value_string_literal_test)
{
	RightValue rv;
	rv.setType(RightValue::Type::StringLiteral);
	rv.setValue<std::string>("Test");

	BOOST_CHECK_EQUAL(rv.getDataType(), DataType::String);
	BOOST_CHECK_EQUAL(rv.eval(), DataType::String);
	BOOST_CHECK_EQUAL(std::get<std::string>(rv.returned), "Test");
}

BOOST_AUTO_TEST_CASE(Right_value_int_literal_test)
{
	RightValue rv;
	rv.setType(RightValue::Type::IntLiteral);
	rv.setValue<int>(7);

	BOOST_CHECK_EQUAL(rv.getDataType(), DataType::Int);
	BOOST_CHECK_EQUAL(rv.eval(), DataType::Int);
	BOOST_CHECK_EQUAL(std::get<int>(rv.returned), 7);
}

BOOST_AUTO_TEST_CASE(Right_value_variable_test)
{
	Executor::symbolTable.enterScope();
	Symbol sym{ DataType::String, "name" };
	sym.value = "Adam";
	Executor::symbolTable.addLocalSymbol(sym);
	RightValue rv;
	rv.setType(RightValue::Type::Identifier);
	rv.setValue<std::string>("name");

	BOOST_CHECK_EQUAL(std::get<std::string>(Executor::symbolTable.getSymbol("name")->value), "Adam");

	BOOST_CHECK_EQUAL(rv.getDataType(), DataType::String);
	BOOST_CHECK_EQUAL(rv.eval(), DataType::String);
	BOOST_CHECK_EQUAL(std::get<std::string>(rv.returned), "Adam");
}

BOOST_AUTO_TEST_CASE(Right_value_modified_variable_test)
{
	Executor::symbolTable.enterScope();
	Symbol sym{ DataType::Int, "number" };
	sym.value = 1;
	Executor::symbolTable.addLocalSymbol(sym);
	RightValue rv;
	rv.setType(RightValue::Type::Identifier);
	rv.setValue<std::string>("number");

	BOOST_CHECK_EQUAL(rv.getDataType(), DataType::Int);
	BOOST_CHECK_EQUAL(rv.eval(), DataType::Int);
	BOOST_CHECK_EQUAL(std::get<int>(rv.returned), 1);

	Executor::symbolTable.getSymbol("number")->value = 2;

	BOOST_CHECK_EQUAL(rv.eval(), DataType::Int);
	BOOST_CHECK_EQUAL(std::get<int>(rv.returned), 2);
}

BOOST_AUTO_TEST_CASE(Primary_expression_with_literal)
{
	PrimaryExpression expr;
	expr.setType(PrimaryExpression::Type::Literal);
	expr.setValue<int>(10);

	BOOST_CHECK_EQUAL(expr.eval(), 10);
}

BOOST_AUTO_TEST_CASE(Primary_expression_with_variable)
{
	Executor::symbolTable.enterScope();
	Symbol sym{ DataType::Int, "number" };
	sym.value = 1;
	Executor::symbolTable.addLocalSymbol(sym);
	PrimaryExpression expr;
	expr.setType(PrimaryExpression::Type::Identifier);
	expr.setValue<std::string>("number");

	BOOST_CHECK_EQUAL(expr.eval(), 1);
}

BOOST_AUTO_TEST_CASE(Multiplicative_expression_test)
{
	MultiplicativeExpression mexpr;
	auto expr = std::make_unique<PrimaryExpression>(), expr2 = std::make_unique<PrimaryExpression>();
	expr->setType(PrimaryExpression::Type::Literal);
	expr->setValue<int>(2);
	expr2->setType(PrimaryExpression::Type::Literal);
	expr2->setValue<int>(3);
	mexpr.addComponent(std::move(expr));
	mexpr.addOperator(false);
	mexpr.addComponent(std::move(expr2));

	BOOST_CHECK_EQUAL(mexpr.eval(), 6);
}

BOOST_AUTO_TEST_CASE(Multiplicative_expression_division_test)
{
	MultiplicativeExpression mexpr;
	auto expr = std::make_unique<PrimaryExpression>(), expr2 = std::make_unique<PrimaryExpression>();
	expr->setType(PrimaryExpression::Type::Literal);
	expr->setValue<int>(20);
	expr2->setType(PrimaryExpression::Type::Literal);
	expr2->setValue<int>(5);
	mexpr.addComponent(std::move(expr));
	mexpr.addOperator(true);
	mexpr.addComponent(std::move(expr2));

	BOOST_CHECK_EQUAL(mexpr.eval(), 4);
}

BOOST_AUTO_TEST_CASE(Triple_multiplicative_expression_test)
{
	MultiplicativeExpression mexpr;
	auto expr = std::make_unique<PrimaryExpression>(), expr2 = std::make_unique<PrimaryExpression>(), expr3 = std::make_unique<PrimaryExpression>();
	expr->setType(PrimaryExpression::Type::Literal);
	expr->setValue<int>(20);
	expr2->setType(PrimaryExpression::Type::Literal);
	expr2->setValue<int>(5);
	expr3->setType(PrimaryExpression::Type::Literal);
	expr3->setValue<int>(2);
	mexpr.addComponent(std::move(expr));
	mexpr.addOperator(true);
	mexpr.addComponent(std::move(expr2));
	mexpr.addOperator(false);
	mexpr.addComponent(std::move(expr3));

	BOOST_CHECK_EQUAL(mexpr.eval(), 8);
}

BOOST_AUTO_TEST_CASE(Additive_expression_test)
{
	Expression expr;
	auto mexpr1 = std::make_unique<MultiplicativeExpression>(), mexpr2 = std::make_unique<MultiplicativeExpression>();
	auto pexpr1 = std::make_unique<PrimaryExpression>(), pexpr2 = std::make_unique<PrimaryExpression>();
	pexpr1->setType(PrimaryExpression::Type::Literal);
	pexpr1->setValue<int>(20);
	pexpr2->setType(PrimaryExpression::Type::Literal);
	pexpr2->setValue<int>(3);
	mexpr1->addComponent(std::move(pexpr1));
	mexpr2->addComponent(std::move(pexpr2));
	expr.addComponent(std::make_pair(false, std::move(mexpr1)));
	expr.addComponent(std::make_pair(false, std::move(mexpr2)));

	BOOST_CHECK_EQUAL(expr.eval(), 23);
}

BOOST_AUTO_TEST_CASE(Additive_expression_substraction_test)
{
	Expression expr;
	auto mexpr1 = std::make_unique<MultiplicativeExpression>(), mexpr2 = std::make_unique<MultiplicativeExpression>();
	auto pexpr1 = std::make_unique<PrimaryExpression>(), pexpr2 = std::make_unique<PrimaryExpression>();
	pexpr1->setType(PrimaryExpression::Type::Literal);
	pexpr1->setValue<int>(20);
	pexpr2->setType(PrimaryExpression::Type::Literal);
	pexpr2->setValue<int>(3);
	mexpr1->addComponent(std::move(pexpr1));
	mexpr2->addComponent(std::move(pexpr2));
	expr.addComponent(std::make_pair(false, std::move(mexpr1)));
	expr.addComponent(std::make_pair(true, std::move(mexpr2)));

	BOOST_CHECK_EQUAL(expr.eval(), 17);
}

BOOST_AUTO_TEST_CASE(Additive_expression_leading_minus_test)
{
	Expression expr;
	auto mexpr1 = std::make_unique<MultiplicativeExpression>(), mexpr2 = std::make_unique<MultiplicativeExpression>();
	auto pexpr1 = std::make_unique<PrimaryExpression>(), pexpr2 = std::make_unique<PrimaryExpression>();
	pexpr1->setType(PrimaryExpression::Type::Literal);
	pexpr1->setValue<int>(20);
	pexpr2->setType(PrimaryExpression::Type::Literal);
	pexpr2->setValue<int>(3);
	mexpr1->addComponent(std::move(pexpr1));
	mexpr2->addComponent(std::move(pexpr2));
	expr.addComponent(std::make_pair(true, std::move(mexpr1)));
	expr.addComponent(std::make_pair(false, std::move(mexpr2)));

	BOOST_CHECK_EQUAL(expr.eval(), -17);
}

BOOST_AUTO_TEST_CASE(Bracket_expression_test)
{
	PrimaryExpression pexpr;
	auto bexpr = std::make_unique<BracketExpression>();
	auto expr = std::make_unique<Expression>();
	auto mexpr1 = std::make_unique<MultiplicativeExpression>(), mexpr2 = std::make_unique<MultiplicativeExpression>();
	auto pexpr1 = std::make_unique<PrimaryExpression>(), pexpr2 = std::make_unique<PrimaryExpression>();
	pexpr1->setType(PrimaryExpression::Type::Literal);
	pexpr1->setValue<int>(20);
	pexpr2->setType(PrimaryExpression::Type::Literal);
	pexpr2->setValue<int>(3);
	mexpr1->addComponent(std::move(pexpr1));
	mexpr2->addComponent(std::move(pexpr2));
	expr->addComponent(std::make_pair(true, std::move(mexpr1)));
	expr->addComponent(std::make_pair(false, std::move(mexpr2)));
	bexpr->setExpression(std::move(expr));
	pexpr.setType(PrimaryExpression::Type::Bracket);
	pexpr.setValue<std::unique_ptr<BracketExpression>>(std::move(bexpr));

	BOOST_CHECK_EQUAL(pexpr.eval(), -17);
}

BOOST_FIXTURE_TEST_CASE(PrimaryCondition_test, RightValFix)
{
	PrimaryCondition primary;
	primary.setType(PrimaryCondition::Type::RightVal);
	primary.setNegated(false);
	primary.setCondition(getIntRval(3));

	BOOST_CHECK_EQUAL(primary.eval(), 3);
}

BOOST_FIXTURE_TEST_CASE(Negated_PrimaryCondition_test, RightValFix)
{
	PrimaryCondition primary;
	primary.setType(PrimaryCondition::Type::RightVal);
	primary.setNegated(true);
	primary.setCondition(getIntRval(3));

	BOOST_CHECK_EQUAL(primary.eval(), 0);
}

BOOST_FIXTURE_TEST_CASE(Negated_PrimaryCondition_test_2, RightValFix)
{
	PrimaryCondition primary;
	primary.setType(PrimaryCondition::Type::RightVal);
	primary.setNegated(true);
	primary.setCondition(getIntRval(0));

	BOOST_CHECK_EQUAL(primary.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(PrimaryCondition_incompatible_type_test, RightValFix)
{
	PrimaryCondition primary;
	primary.setType(PrimaryCondition::Type::RightVal);
	primary.setNegated(true);
	primary.setCondition(getStringRval());

	BOOST_CHECK_THROW(primary.eval(), Error);
}

BOOST_FIXTURE_TEST_CASE(Single_RelationCondition_test, RightValFix)
{
	RelationCondition cond;
	auto primary = std::make_unique<PrimaryCondition>();
	primary->setType(PrimaryCondition::Type::RightVal);
	primary->setNegated(false);
	primary->setCondition(getIntRval(3));

	cond.setFirst(std::move(primary));

	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(RelationCondition_equal_test, RightValFix)
{
	RelationCondition cond;
	auto primary = std::make_unique<PrimaryCondition>(), primary2 = std::make_unique<PrimaryCondition>();
	primary->setType(PrimaryCondition::Type::RightVal);
	primary->setNegated(false);
	primary->setCondition(getIntRval(3));

	primary2->setType(PrimaryCondition::Type::RightVal);
	primary2->setNegated(false);
	primary2->setCondition(getIntRval(2));

	cond.setFirst(std::move(primary));
	cond.setRelationOper(RelationOperator::Equal);
	cond.setSecond(std::move(primary2));

	BOOST_CHECK_EQUAL(cond.eval(), 0);
}

BOOST_FIXTURE_TEST_CASE(RelationCondition_equal2_test, RightValFix)
{
	RelationCondition cond;
	auto primary = std::make_unique<PrimaryCondition>(), primary2 = std::make_unique<PrimaryCondition>();
	primary->setType(PrimaryCondition::Type::RightVal);
	primary->setNegated(false);
	primary->setCondition(getIntRval(3));

	primary2->setType(PrimaryCondition::Type::RightVal);
	primary2->setNegated(false);
	primary2->setCondition(getIntRval(3));

	cond.setFirst(std::move(primary));
	cond.setRelationOper(RelationOperator::Equal);
	cond.setSecond(std::move(primary2));

	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(RelationCondition_not_equal_test, RightValFix)
{
	RelationCondition cond;
	auto primary = std::make_unique<PrimaryCondition>(), primary2 = std::make_unique<PrimaryCondition>();
	primary->setType(PrimaryCondition::Type::RightVal);
	primary->setNegated(false);
	primary->setCondition(getIntRval(3));

	primary2->setType(PrimaryCondition::Type::RightVal);
	primary2->setNegated(false);
	primary2->setCondition(getIntRval(2));

	cond.setFirst(std::move(primary));
	cond.setRelationOper(RelationOperator::NotEqual);
	cond.setSecond(std::move(primary2));

	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(RelationCondition_greater_test, RightValFix)
{
	RelationCondition cond;
	auto primary = std::make_unique<PrimaryCondition>(), primary2 = std::make_unique<PrimaryCondition>();
	primary->setType(PrimaryCondition::Type::RightVal);
	primary->setNegated(false);
	primary->setCondition(getIntRval(3));

	primary2->setType(PrimaryCondition::Type::RightVal);
	primary2->setNegated(false);
	primary2->setCondition(getIntRval(2));

	cond.setFirst(std::move(primary));
	cond.setRelationOper(RelationOperator::Greater);
	cond.setSecond(std::move(primary2));

	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(Single_AndConditionTest, RelationCondFix)
{
	AndCondition cond;
	cond.addComponent(getRelationCond(true));

	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(Single_negative_AndConditionTest, RelationCondFix)
{
	AndCondition cond;
	cond.addComponent(getRelationCond(false));

	BOOST_CHECK_EQUAL(cond.eval(), 0 );
}

BOOST_FIXTURE_TEST_CASE(Positive_AndConditionTest, RelationCondFix)
{
	AndCondition cond;
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(true));
	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(Positive_AndConditionTest_2, RelationCondFix)
{
	AndCondition cond;
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(true));
	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(Negative_AndConditionTest, RelationCondFix)
{
	AndCondition cond;
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(false));
	BOOST_CHECK_EQUAL(cond.eval(), 0);
}

BOOST_FIXTURE_TEST_CASE(Negative_AndConditionTest2, RelationCondFix)
{
	AndCondition cond;
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(false));
	cond.addComponent(getRelationCond(true));
	cond.addComponent(getRelationCond(true));
	BOOST_CHECK_EQUAL(cond.eval(), 0);
}

BOOST_FIXTURE_TEST_CASE(ConditionTest, RelationCondFix)
{
	Condition cond;
	auto acond = std::make_unique< AndCondition>();
	acond->addComponent(getRelationCond(true));
	acond->addComponent(getRelationCond(false));
	cond.addComponent(std::move(acond));
	BOOST_CHECK_EQUAL(cond.eval(), 0);
}

BOOST_FIXTURE_TEST_CASE(ConditionTest2, RelationCondFix)
{
	Condition cond;
	auto acond = std::make_unique< AndCondition>(), acond2 = std::make_unique< AndCondition>();
	acond->addComponent(getRelationCond(true));
	acond2->addComponent(getRelationCond(false));
	cond.addComponent(std::move(acond));
	cond.addComponent(std::move(acond2));
	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(ConditionTest3, RelationCondFix)
{
	Condition cond;
	auto acond = std::make_unique< AndCondition>(), acond2 = std::make_unique< AndCondition>();
	acond->addComponent(getRelationCond(false));
	acond2->addComponent(getRelationCond(false));
	cond.addComponent(std::move(acond));
	cond.addComponent(std::move(acond2));
	BOOST_CHECK_EQUAL(cond.eval(), 0);
}

BOOST_FIXTURE_TEST_CASE(ConditionTest4, RelationCondFix)
{
	Condition cond;
	auto acond = std::make_unique< AndCondition>(), acond2 = std::make_unique< AndCondition>(), acond3 = std::make_unique< AndCondition>();
	acond->addComponent(getRelationCond(true));
	acond2->addComponent(getRelationCond(false));
	acond3->addComponent(getRelationCond(true));
	cond.addComponent(std::move(acond));
	cond.addComponent(std::move(acond2));
	cond.addComponent(std::move(acond3));
	BOOST_CHECK_EQUAL(cond.eval(), 1);
}

BOOST_FIXTURE_TEST_CASE(BracesConditionTest, RelationCondFix)
{
	PrimaryCondition pcond;
	auto bcond = std::make_unique<BracesCondition>();
	auto cond = std::make_unique<Condition>();
	auto acond = std::make_unique< AndCondition>(), acond2 = std::make_unique< AndCondition>(), acond3 = std::make_unique< AndCondition>();
	acond->addComponent(getRelationCond(true));
	acond2->addComponent(getRelationCond(false));
	acond3->addComponent(getRelationCond(true));
	cond->addComponent(std::move(acond));
	cond->addComponent(std::move(acond2));
	cond->addComponent(std::move(acond3));
	bcond->setCondition(std::move(cond));
	pcond.setNegated(false);
	pcond.setType(PrimaryCondition::Type::Bracket);
	pcond.setCondition(std::move(bcond));

	BOOST_CHECK_EQUAL(pcond.eval(), 1);
}

BOOST_AUTO_TEST_SUITE_END()