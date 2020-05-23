#include "../Parser.h"
#include <boost/test/auto_unit_test.hpp>
#include "../Error.h"
#include <string>
#include <iostream>
#include <memory>

using namespace tkom;

BOOST_AUTO_TEST_SUITE(Parser_tests)

class ParserFix
{
public: 
	
	Parser* getParser(std::string prog) {
		in.reset(new std::stringstream(prog));
		reader.reset(new Reader(*in));
		scanner.reset(new Scanner(*reader));
		parser.reset(new Parser(*scanner));

		return parser.get();
	}
	
	std::unique_ptr<std::stringstream> in;
	std::unique_ptr<Reader> reader;
	std::unique_ptr<Scanner> scanner;
	std::unique_ptr<Parser> parser;
};

BOOST_FIXTURE_TEST_CASE(Parsing_empty_functions_test, ParserFix)
{
	Parser* parser = getParser(R"(
int main() 
{

}

string fun(int i)
{

}
)");

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	BOOST_CHECK_EQUAL(program->getFunctions().size(), 2);
}

BOOST_FIXTURE_TEST_CASE(Parsing_function_with_initialization, ParserFix)
{
	Parser* parser = getParser(R"(int main() 
{
	string name;
})");

 	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto init = dynamic_cast<ast::InitStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[0].get());
	BOOST_CHECK_EQUAL(init->getDataType(), ast::DataType::String);
	BOOST_CHECK_EQUAL(init->getInitiated().size(), 1);
	BOOST_CHECK_EQUAL(init->getInitiated()[0].first, "name");
	BOOST_CHECK_EQUAL(init->getInitiated()[0].second, nullptr);
}

BOOST_FIXTURE_TEST_CASE(Parsing_initialization_with_assigment, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	string name = "Grzegorz";
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto init = dynamic_cast<ast::InitStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[0].get());
	BOOST_CHECK_EQUAL(init->getDataType(), ast::DataType::String);
	BOOST_CHECK_EQUAL(init->getInitiated().size(), 1);
	BOOST_CHECK_EQUAL(init->getInitiated()[0].first, "name");
	auto rVal = init->getInitiated()[0].second.get();
	BOOST_CHECK_EQUAL(rVal->getValue<std::string>(0), "Grzegorz");
}

BOOST_FIXTURE_TEST_CASE(Parsing_int_initialization_with_assigment, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int name = 7;
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto init = dynamic_cast<ast::InitStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[0].get());
	auto rVal = init->getInitiated()[0].second.get();
	BOOST_CHECK_NO_THROW(rVal->getValue<ast::Expression>());
}

BOOST_FIXTURE_TEST_CASE(Parsing_multiple_initialization, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	string name, surname, fruit = "Banana", title;
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto init = dynamic_cast<ast::InitStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[0].get());
	BOOST_CHECK_EQUAL(init->getInitiated().size(), 4);
}

BOOST_FIXTURE_TEST_CASE(Parsing_assignment, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int j, i = 7;
	j = i;
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto assign = dynamic_cast<ast::AssignStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	BOOST_CHECK_EQUAL(assign->getIdentifier(), "j");
	BOOST_CHECK_NO_THROW(assign->getRval()->getValue<ast::Expression>());
}

BOOST_FIXTURE_TEST_CASE(Parsing_assignment_with_literal, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	string surname;
	surname = "Brzêczyszczykiweicz";
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto assign = dynamic_cast<ast::AssignStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	BOOST_CHECK_EQUAL(assign->getIdentifier(), "surname");
	BOOST_CHECK_NO_THROW(assign->getRval()->getValue<std::string>(0));
}

BOOST_FIXTURE_TEST_CASE(Parsing_if_statement, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if(1>0)
	{
		i = 1;
		i = 2;
	}
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto ifStatement = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	if (ifStatement->getCondition() == nullptr)
		BOOST_FAIL("Condition is nullptr");
	BOOST_CHECK_EQUAL(ifStatement->getIfBody()->getInstructions().size(), 2);
	BOOST_CHECK_EQUAL(ifStatement->getElseBody(), nullptr);
}

BOOST_FIXTURE_TEST_CASE(Parsing_if_statement2, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if(1>0)
		i = 1;

}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto ifStatement = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	if (ifStatement->getCondition() == nullptr)
		BOOST_FAIL("Condition is nullptr");
	BOOST_CHECK_EQUAL(ifStatement->getIfBody()->getInstructions().size(), 1);
	BOOST_CHECK_EQUAL(ifStatement->getElseBody(), nullptr);
}

BOOST_FIXTURE_TEST_CASE(Parsing_if_with_else_statement, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if(1>0)
	{
		i = 1;
	}
	else
	{
		i = 2;
	}
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto ifStatement = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	if (ifStatement->getCondition() == nullptr)
		BOOST_FAIL("Condition is nullptr");
	BOOST_CHECK_EQUAL(ifStatement->getIfBody()->getInstructions().size(), 1);
	BOOST_CHECK_EQUAL(ifStatement->getElseBody()->getInstructions().size(), 1);
}

BOOST_FIXTURE_TEST_CASE(Parsing_while_loop, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int j, i = 0;
	while(i < 10)
		j = i;

}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto whileStatement = dynamic_cast<ast::WhileLoop*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	if (whileStatement->getCondition() == nullptr)
		BOOST_FAIL("Condition is nullptr");
	BOOST_CHECK_EQUAL(whileStatement->getBody()->getInstructions().size(), 1);
}

BOOST_FIXTURE_TEST_CASE(Parsing_while_loop2, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int k, j, i = 0;
	while(i < 10)
	{
		j = i;
		k = j;
	}

}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto whileStatement = dynamic_cast<ast::WhileLoop*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get());
	if (whileStatement->getCondition() == nullptr)
		BOOST_FAIL("Condition is nullptr");
	BOOST_CHECK_EQUAL(whileStatement->getBody()->getInstructions().size(), 2);
}

BOOST_FIXTURE_TEST_CASE(Parsing_return_statement, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	return 10;
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto returnStatement = dynamic_cast<ast::ReturnStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[0].get());
	if (returnStatement->getValue() == nullptr)
		BOOST_FAIL("Value is nullptr");
}

BOOST_FIXTURE_TEST_CASE(Parsing_break_statement, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	while(10 == 10)
		break;
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto whileStatement = dynamic_cast<ast::WhileLoop*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[0].get());
	if (whileStatement->getBody()->getInstructions()[0]->getType() != ast::Instruction::Type::Break)
		BOOST_FAIL("Not a break instruction");
}

BOOST_FIXTURE_TEST_CASE(Function_with_no_params_exec_test, ParserFix)
{
	Parser* parser = getParser(
R"(string testFun()
{
	return "Test sequence";
}

int main() 
{
	testFun();
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto exec = dynamic_cast<ast::FunctionExec*>(program->getFunctions()[1]->getFunctionBody()->getInstructions()[0].get());
	BOOST_CHECK_EQUAL(exec->getFunctionCall()->getIdentifier(), "testFun");
	BOOST_CHECK_EQUAL(exec->getFunctionCall()->getCallOperator()->getArguments().size(), 0);
	BOOST_CHECK_EQUAL(std::get<ast::FunctionDef*>(parser->getTable().getSymbol(exec->getFunctionCall()->getIdentifier())->value),
		program->getFunctions()[0].get());
}

BOOST_FIXTURE_TEST_CASE(Parsing_function_with_params_definition, ParserFix)
{
	Parser* parser = getParser(
R"(string testFun(int i, graphic shape)
{
	
}

int main() 
{
	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto definition = program->getFunctions()[0].get();
	BOOST_CHECK_EQUAL(definition->getCallDef()->getArguments().size(), 2);
	BOOST_CHECK_EQUAL(definition->getCallDef()->getArguments()[0].first, ast::DataType::Int);
	BOOST_CHECK_EQUAL(definition->getCallDef()->getArguments()[0].second, "i");
	BOOST_CHECK_EQUAL(definition->getCallDef()->getArguments()[1].first, ast::DataType::Graphic);
	BOOST_CHECK_EQUAL(definition->getCallDef()->getArguments()[1].second, "shape");
}

BOOST_FIXTURE_TEST_CASE(Parsing_function_exec_with_params, ParserFix)
{
	Parser* parser = getParser(
R"(string testFun(int i, graphic shape)
{
	
}

int main() 
{
	int x;
	graphic circle;
	testFun(x, circle);
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto exec = dynamic_cast<ast::FunctionExec*>(program->getFunctions()[1]->getFunctionBody()->getInstructions()[2].get());
	BOOST_CHECK_EQUAL(exec->getFunctionCall()->getCallOperator()->getArguments().size(), 2);
	BOOST_CHECK_NO_THROW(exec->getFunctionCall()->getCallOperator()->getArguments()[0]->getValue<ast::Expression>());
	BOOST_CHECK_EQUAL(exec->getFunctionCall()->getCallOperator()->getArguments()[1]->getValue<std::string>(0), "circle");
}

BOOST_FIXTURE_TEST_CASE(Parsing_primary_expression_with_int_literal, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	
	i = 1;
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents()[0]->getValue<int>(0), 1);
}

BOOST_FIXTURE_TEST_CASE(Parsing__expression_starting_with_minus, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	
	i = -1;
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, true);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents()[0]->getValue<int>(0), 1);
}

BOOST_FIXTURE_TEST_CASE(Parsing_primary_expression_with_identifier, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j = 1;
	
	i = j;
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents()[0]->getValue<std::string>(0), "j");
}

BOOST_FIXTURE_TEST_CASE(Parsing_primary_expression_with_function_call, ParserFix)
{
	Parser* parser = getParser(
R"(int fun()
{
	return 1;
}
int main() 
{
	int i;
	
	i = fun();
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[1]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents().size(), 1);
	auto functionCall = expression->getComponents()[0].second->getComponents()[0]->getValue<ast::FunctionCall>();
	if (functionCall == nullptr)
		BOOST_FAIL("Function call is null");
	BOOST_CHECK_EQUAL(functionCall->getIdentifier(), "fun");
}

BOOST_FIXTURE_TEST_CASE(Parsing_primary_expression_with_bracket_expression, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j = 1;
	
	i = (j);
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents().size(), 1);
	if (expression->getComponents()[0].second->getComponents()[0]->getValue<ast::BracketExpression>() == nullptr)
		BOOST_FAIL("Braces expression is null");
}

BOOST_FIXTURE_TEST_CASE(Parsing_additive_expression, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j = 1;
	
	i = j+1 - 100 +9;
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 4);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[1].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[2].first, true);
	BOOST_CHECK_EQUAL(expression->getComponents()[3].first, false);
}

BOOST_FIXTURE_TEST_CASE(Parsing_multiplicative_expression, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j = 1;
	
	i = j*1/100/9;
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get()
		)->getRval()->getValue<ast::Expression>()->getComponents()[0].second.get();

	BOOST_CHECK_EQUAL(expression->getComponents().size(), 4);
	BOOST_CHECK_EQUAL(expression->getOperators()[0], false);
	BOOST_CHECK_EQUAL(expression->getOperators()[1], true);
	BOOST_CHECK_EQUAL(expression->getOperators()[2], true);
}

BOOST_FIXTURE_TEST_CASE(Parsing_expression, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j = 1;
	
	i = j * 1 + 100 / (9 - 1);
 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto expression = dynamic_cast<ast::AssignStatement*>(
		program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getRval()->getValue<ast::Expression>();
	BOOST_CHECK_EQUAL(expression->getComponents().size(), 2);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[1].first, false);
	BOOST_CHECK_EQUAL(expression->getComponents()[0].second->getComponents().size(), 2);
	BOOST_CHECK_NO_THROW(expression->getComponents()[1].second->getComponents()[1]->getValue<ast::BracketExpression>());
}

BOOST_FIXTURE_TEST_CASE(Parsing_primary_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if(i)
		{} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getFirst()->isNegated(), false);
	BOOST_CHECK_NO_THROW(condition->getComponents()[0]->getComponents()[0]->getFirst()->getCondition<ast::RightValue>());
}

BOOST_FIXTURE_TEST_CASE(Parsing_negated_primary_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if(!i)
		{} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getFirst()->isNegated(), true);
	BOOST_CHECK_NO_THROW(condition->getComponents()[0]->getComponents()[0]->getFirst()->getCondition<ast::RightValue>());
}

BOOST_FIXTURE_TEST_CASE(Parsing_braces_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if((i))
		{} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getFirst()->isNegated(), false);
	BOOST_CHECK_NO_THROW(condition->getComponents()[0]->getComponents()[0]->getFirst()->getCondition<ast::BracesCondition>());
	if (condition->getComponents()[0]->getComponents()[0]->getFirst()->getCondition<ast::BracesCondition>()->getCondition() == nullptr)
		BOOST_FAIL("Condition in braces is nullptr");
}

BOOST_FIXTURE_TEST_CASE(Parsing_relation_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if ( i < 10 )
	{
		
	} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents().size(), 1);
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getRelationOper(), ast::RelationOperator::Less);
	if (condition->getComponents()[0]->getComponents()[0]->getFirst() == nullptr)
		BOOST_FAIL("First of relation condition is nullptr");
	if (condition->getComponents()[0]->getComponents()[0]->getSecond() == nullptr)
		BOOST_FAIL("Second of relation condition is nullptr");
}

BOOST_FIXTURE_TEST_CASE(Parsing_relation_less_equal_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if ( i <= 10 ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getRelationOper(), ast::RelationOperator::LessEqual);
}

BOOST_FIXTURE_TEST_CASE(Parsing_relation_equal_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if ( i == 10 ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getRelationOper(), ast::RelationOperator::Equal);
}

BOOST_FIXTURE_TEST_CASE(Parsing_relation_not_equal_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if ( i != 10 ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getRelationOper(), ast::RelationOperator::NotEqual);
}

BOOST_FIXTURE_TEST_CASE(Parsing_relation_greater_equal_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if ( i >= 10 ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getRelationOper(), ast::RelationOperator::GreaterEqual);
}

BOOST_FIXTURE_TEST_CASE(Parsing_relation_greater_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i;
	if ( i > 10 ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents()[0]->getRelationOper(), ast::RelationOperator::Greater);
}

BOOST_FIXTURE_TEST_CASE(Parsing_and_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j;
	if ( i && 10 && !j ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents()[0]->getComponents().size(), 3);
	if (condition->getComponents()[0]->getComponents()[0] == nullptr)
		BOOST_ERROR("Part of and condition is nullptr");
}

BOOST_FIXTURE_TEST_CASE(Parsing_or_condition, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int i, j;
	if ( i || 10 && !j ) {} 	
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser->parse());
	auto condition = dynamic_cast<ast::IfStatement*>(program->getFunctions()[0]->getFunctionBody()->getInstructions()[1].get())->getCondition();
	BOOST_CHECK_EQUAL(condition->getComponents().size(), 2);
	if (condition->getComponents()[0] == nullptr)
		BOOST_ERROR("Part of or condition is nullptr");
}

BOOST_FIXTURE_TEST_CASE(Unexpected_token_test, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{

}
stringERROR fun(int i)
{

}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_EXCEPTION(program = parser->parse(), Error, [](const Error& e)->bool {
		return std::string{ "Line: 5 pos: 0 Unexpected token  of type: Identifier" }.compare(e.what()) == 0 ? true : false;
	});
}

BOOST_FIXTURE_TEST_CASE(Invalid_token_test, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{

}
string fun(int i)
{
	^
}
)" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_EXCEPTION(program = parser->parse(), Error, [](const Error& e)->bool {
		return std::string{ "Line: 7 pos: 1 Invalid token" }.compare(e.what()) == 0 ? true : false;
	});
}

BOOST_FIXTURE_TEST_CASE(Invalid_equal_sign_token_test, ParserFix)
{
	Parser* parser = getParser(
R"(int main() 
{
	int a, b = 7, c = -10;
	a = b + 1;
	if(b = c)
		a = a + 1;
})" );

	std::unique_ptr<ast::Program> program;
	BOOST_CHECK_EXCEPTION(program = parser->parse(), Error, [](const Error& e)->bool {
		return std::string{ "Line: 5 pos: 6 Unexpected token  of type: Assigment" }.compare(e.what()) == 0 ? true : false;
	});
}

BOOST_FIXTURE_TEST_CASE(Parsing_arguments_passing_test, ParserFix)
{
	auto parser = getParser(R"(
int fun(int x)
{
	return x * x;
}

int main() 
{
	int z = 2;	
	z = fun(z);
	return z;
}
)");

	BOOST_CHECK_NO_THROW(parser->parse());
}

BOOST_AUTO_TEST_SUITE_END()