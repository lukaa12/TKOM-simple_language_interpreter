#include "Parser.h"
#include "Error.h"
#include "Executor.h"

using namespace tkom;

Parser::Parser(Scanner& _scanner): scanner(_scanner), token()
{}

std::unique_ptr<ast::Program> Parser::parse()
{
	auto program = std::make_unique<ast::Program>();
	this->advance();
	while (this->token.getType() != Token::Type::Eof)
	{
		auto func = this->readFunctionDef();
		this->symbolTable.addGlobalSymbol(Symbol(func.get()));
		program->addFunction(std::move(func));
	}
	this->symbolTable.leaveAllScopes();
	Executor::symbolTable = symbolTable;
	return program;
}

std::unique_ptr<ast::FunctionDef> Parser::readFunctionDef()
{
	auto func = std::make_unique<ast::FunctionDef>();
	this->symbolTable.enterScope();
	func->setReturnType(getDataType());
	this->advance();
	this->requireType({ Token::Type::Identifier });
	func->setIdentifier(token.getStrVal());
	this->advance();
	func->setCallDef(readCallDef());
	func->setFunctionBody(readBodyBlock());
	return func;
}

std::unique_ptr<ast::CallDef> Parser::readCallDef()
{
	auto callDef = std::make_unique<ast::CallDef>();
	this->requireAndConsume({ Token::Type::BracketOpen });

	while (!this->checkType({ Token::Type::BracketClose }))
	{
		auto type = getDataType();
		this->advance();
		this->requireType({ Token::Type::Identifier });
		std::string argName = this->token.getStrVal();
		callDef->addArgument(std::make_pair(type, argName));
		this->symbolTable.addLocalSymbol(Symbol(type, argName));
		this->advance();
		this->requireType({ Token::Type::BracketClose, Token::Type::Comma });
		if (this->checkType({ Token::Type::Comma }))
			this->advance();
	}

	this->requireAndConsume({ Token::Type::BracketClose });
	return callDef;
}

std::unique_ptr<ast::Body> Parser::readBodyBlock()
{
	auto body = std::make_unique<ast::Body>();
	this->requireAndConsume({ Token::Type::CurlyOpen });

	while (!this->checkType({ Token::Type::CurlyClose }))
	{
		body->addInstruction(readInstruction());
	}
	this->advance();
	return body;
}

std::unique_ptr<ast::CallOperator> Parser::readCallOperator()
{
	auto oper = std::make_unique<ast::CallOperator>();
	this->requireAndConsume({ Token::Type::BracketOpen });

	while (!this->checkType({ Token::Type::BracketClose }))
	{
		oper->addArgument(this->readRightValue());
		if (this->checkType({ Token::Type::Comma }))
			this->advance();
	}
	this->advance();
	return oper;
}

std::unique_ptr<ast::Instruction> Parser::readInstruction()
{
	this->requireType({ Token::Type::If, Token::Type::While, Token::Type::Identifier, 
		Token::Type::Return, Token::Type::Break, Token::Type::IntType, Token::Type::StringType,
		Token::Type::GraphicType, Token::Type::ColorType });
	
	if (this->token.getType() == Token::Type::If)
		return std::unique_ptr<ast::Instruction>(this->readIfStatement());
	else if (this->token.getType() == Token::Type::While)
		return std::unique_ptr<ast::Instruction>(this->readWhileLoop());
	else if (token.getType() == Token::Type::Identifier)
		if (symbolTable.getSymbol(token.getStrVal())->type == ast::IdType::Function)
			return std::unique_ptr<ast::Instruction>(this->readFunctionExec());
		else
			return std::unique_ptr<ast::Instruction>(this->readAssignStatement());
	else if (token.getType() == Token::Type::Return)
		return std::unique_ptr<ast::Instruction>(this->readReturnStatement());
	else if (token.getType() == Token::Type::Break)
	{
		this->advance();
		this->requireAndConsume({ Token::Type::Semicolon });
		return std::make_unique<ast::Instruction>();
	}
	else
		return std::unique_ptr<ast::Instruction>(this->readInitStatement());
}

std::unique_ptr<ast::IfStatement> Parser::readIfStatement()
{
	auto ifStatement = std::make_unique<ast::IfStatement>();
	this->requireAndConsume({ Token::Type::If });
	this->requireAndConsume({ Token::Type::BracketOpen });
	ifStatement->setCondition(this->readCondition());
	this->requireAndConsume({ Token::Type::BracketClose });
	this->requireType({ Token::Type::CurlyOpen, Token::Type::If, Token::Type::While, Token::Type::Identifier,
		Token::Type::Return, Token::Type::Break });
	if (this->checkType({ Token::Type::CurlyOpen }))
		ifStatement->setIfBody(this->readBodyBlock());
	else
	{
		auto instr = std::make_unique<ast::Body>();
		instr->addInstruction(this->readInstruction());
		ifStatement->setIfBody(std::move(instr));
	}
	if (this->checkType({ Token::Type::Else }))
	{
		this->advance();
		if (this->checkType({ Token::Type::CurlyOpen }))
			ifStatement->setElseBody(this->readBodyBlock());
		else
		{
			auto instr = std::make_unique<ast::Body>();
			instr->addInstruction(this->readInstruction());
			ifStatement->setIfBody(std::move(instr));
		}
	}
	return ifStatement;
}

std::unique_ptr<ast::WhileLoop> Parser::readWhileLoop()
{
	auto whileLoop = std::make_unique<ast::WhileLoop>();
	this->requireAndConsume({ Token::Type::While });
	this->requireAndConsume({ Token::Type::BracketOpen });

	whileLoop->setCondition(this->readCondition());
	this->requireAndConsume({ Token::Type::BracketClose });
	this->requireType({ Token::Type::CurlyOpen, Token::Type::If, Token::Type::While, Token::Type::Identifier,
		Token::Type::Return, Token::Type::Break });
	if (this->checkType({ Token::Type::CurlyOpen }))
		whileLoop->setBody(this->readBodyBlock());
	else
	{
		auto instr = std::make_unique<ast::Body>();
		instr->addInstruction(this->readInstruction());
		whileLoop->setBody(std::move(instr));
	}
	return whileLoop;
}

std::unique_ptr<ast::AssignStatement> Parser::readAssignStatement()
{
	auto assign = std::make_unique<ast::AssignStatement>();
	this->requireType({ Token::Type::Identifier });
	assign->setIdentifier(token.getStrVal());
	this->advance();
	this->requireAndConsume({ Token::Type::Assigment });
	assign->setRval(this->readRightValue());
	this->requireAndConsume({ Token::Type::Semicolon });
	return assign;
}

std::unique_ptr<ast::InitStatement> Parser::readInitStatement()
{
	auto initStatement = std::make_unique<ast::InitStatement>();
	initStatement->setDataType(this->getDataType());

	do
	{
		this->advance();
		this->requireType({ Token::Type::Identifier });
		std::string id = this->token.getStrVal();
		this->advance();
		this->requireType({ Token::Type::Comma, Token::Type::Assigment, Token::Type::Semicolon });
		if (this->checkType({ Token::Type::Assigment }))
		{
			this->advance();
			initStatement->addInitiated(std::make_pair(id, this->readRightValue()));
		}
		else {
			initStatement->addInitiated(std::make_pair(id, std::unique_ptr<ast::RightValue>()));
		}
		symbolTable.addLocalSymbol(Symbol(initStatement->getDataType(), id));
		this->requireType({ Token::Type::Comma, Token::Type::Semicolon });
	} while (!this->checkType({ Token::Type::Semicolon }));

	this->advance();
	return initStatement;
}

std::unique_ptr<ast::FunctionExec> Parser::readFunctionExec()
{
	auto exec = std::make_unique<ast::FunctionExec>();
	exec->setFunctionCall(this->readFunctionCall());
	this->requireAndConsume({ Token::Type::Semicolon });
	return exec;
}

std::unique_ptr<ast::ReturnStatement> Parser::readReturnStatement()
{
	auto statement = std::make_unique<ast::ReturnStatement>();
	this->requireAndConsume({ Token::Type::Return });
	statement->setValue(this->readRightValue());
	this->requireAndConsume({ Token::Type::Semicolon });
	return statement;
}

std::unique_ptr<ast::RightValue> Parser::readRightValue()
{
	auto right = std::make_unique<ast::RightValue>();
	this->requireType({ Token::Type::StringLiteral, Token::Type::IntLiteral, Token::Type::Identifier, Token::Type::Minus, Token::Type::BracketOpen });
	if (this->checkType({ Token::Type::StringLiteral }))
	{
		right->setType(ast::RightValue::Type::StringLiteral);
		right->setValue(this->token.getStrVal());
		this->advance();
	}
	else if (this->checkType({ Token::Type::Identifier }) && symbolTable.getSymbol(this->token.getStrVal())->dataType != ast::DataType::Int)
	{
		if (symbolTable.getSymbol(this->token.getStrVal())->type == ast::IdType::Variable)
		{
			right->setType(ast::RightValue::Type::Identifier);
			right->setValue(this->token.getStrVal());
			this->advance();
		}
		else {
			right->setType(ast::RightValue::Type::Function);
			right->setValue(this->readFunctionCall());
		}
	}
	else {			
		right->setType(ast::RightValue::Type::Expression);
		right->setValue(this->readExpression());
	}
	return right;
}

std::unique_ptr<ast::FunctionCall> Parser::readFunctionCall()
{
	auto call = std::make_unique<ast::FunctionCall>();
	this->requireType({ Token::Type::Identifier });
	call->setIdentifier(this->token.getStrVal());
	this->advance();
	call->setCallOperator(this->readCallOperator());
	return call;
}

std::unique_ptr<ast::Expression> Parser::readExpression()
{
	auto expr = std::make_unique<ast::Expression>();
	bool minus = false;
	minus = this->checkType({ Token::Type::Minus });
	if (minus)
		this->advance();
	expr->addComponent(std::make_pair(minus, this->readMultiplicativeExpression()));
	while (this->checkType({ Token::Type::Plus, Token::Type::Minus }))
	{
		minus = this->checkType({ Token::Type::Minus });
		this->advance();
		expr->addComponent(std::make_pair(minus, this->readMultiplicativeExpression()));
	}
	return expr;
}

std::unique_ptr<ast::MultiplicativeExpression> Parser::readMultiplicativeExpression()
{
	auto expr = std::make_unique<ast::MultiplicativeExpression>();
	expr->addComponent(this->readPrimaryExpression());
	while (this->checkType({ Token::Type::Multiply, Token::Type::Divide }))
	{
		expr->addOperator(this->checkType({ Token::Type::Divide }));
		this->advance();
		expr->addComponent(this->readPrimaryExpression());
	}
	return expr;
}

std::unique_ptr<ast::PrimaryExpression> Parser::readPrimaryExpression()
{
	auto expr = std::make_unique<ast::PrimaryExpression>();
	this->requireType({ Token::Type::Identifier, Token::Type::IntLiteral, Token::Type::BracketOpen });
	if (this->checkType({ Token::Type::Identifier }))
	{
		if (symbolTable.getSymbol(this->token.getStrVal())->type == ast::IdType::Function)
		{
			expr->setType(ast::PrimaryExpression::Type::Function);
			expr->setValue(this->readFunctionCall());
		} 
		else
		{
			expr->setType(ast::PrimaryExpression::Type::Identifier);
			expr->setValue(this->token.getStrVal());
			this->advance();
		}
	}
	else if (this->checkType({ Token::Type::IntLiteral }))
	{
		expr->setType(ast::PrimaryExpression::Type::Literal);
		expr->setValue(this->token.getIntVal());
		this->advance();
	}
	else
	{
		expr->setType(ast::PrimaryExpression::Type::Bracket);
		expr->setValue(this->readBracketExpression());
	}
	return expr;
}

std::unique_ptr<ast::BracketExpression> Parser::readBracketExpression()
{
	auto expr = std::make_unique<ast::BracketExpression>();
	this->requireAndConsume({ Token::Type::BracketOpen });
	expr->setExpression(this->readExpression());
	this->requireAndConsume({ Token::Type::BracketClose });
	return expr;
}


std::unique_ptr<ast::Condition> Parser::readCondition()
{
	auto cond = std::make_unique<ast::Condition>();
	cond->addComponent(this->readAndCondition());
	while (this->checkType({ Token::Type::Or }))
	{
		this->advance();
		cond->addComponent(this->readAndCondition());
	}
	return cond;
}

std::unique_ptr<ast::AndCondition> Parser::readAndCondition()
{
	auto cond = std::make_unique<ast::AndCondition>();
	cond->addComponent(this->readRelationCondition());
	while (this->checkType({ Token::Type::And }))
	{
		this->advance();
		cond->addComponent(this->readRelationCondition());
	}
	return cond;
}

std::unique_ptr<ast::RelationCondition> Parser::readRelationCondition()
{
	auto cond = std::make_unique<ast::RelationCondition>();
	cond->setFirst(this->readPrimaryCondition());
	ast::RelationOperator oper;
	
	if(checkType({ Token::Type::Less, Token::Type::LessOrEqual, Token::Type::Equal, Token::Type::GreaterOrEqual,
				  Token::Type::Greater, Token::Type::NotEqual }))
	{
		oper = this->getRelationOper();
	}
	else {
		return cond;
	}
	cond->setRelationOper(oper);
	this->advance();
	cond->setSecond(this->readPrimaryCondition());
	return cond;
}

std::unique_ptr<ast::PrimaryCondition> Parser::readPrimaryCondition()
{
	auto cond = std::make_unique<ast::PrimaryCondition>();
	if (this->checkType({ Token::Type::Negation }))
	{
		cond->setNegated(true);
		this->advance();
	}
	if (this->checkType({ Token::Type::BracketOpen }))
	{
		cond->setType(ast::PrimaryCondition::Type::Bracket);
		cond->setCondition(this->readBracesCondition());
	}
	else
	{
		cond->setType(ast::PrimaryCondition::Type::RightVal);
		cond->setCondition(this->readRightValue());
	}
	return cond;
}

std::unique_ptr<ast::BracesCondition> Parser::readBracesCondition()
{
	auto cond = std::make_unique<ast::BracesCondition>();
	this->requireAndConsume({ Token::Type::BracketOpen });
	cond->setCondition(this->readCondition());
	this->requireAndConsume({ Token::Type::BracketClose });
	return cond;
}


void Parser::advance()
{
	this->token = this->scanner.nextToken();
}

ast::DataType Parser::getDataType()
{
	switch (this->token.getType())
	{
	case Token::Type::IntType:
		return ast::DataType::Int;
	case Token::Type::StringType:
		return ast::DataType::String;
	case Token::Type::GraphicType:
		return ast::DataType::Graphic;
	case Token::Type::ColorType:
		return ast::DataType::Color;
	default:
		throw Error(token, Error::Type::UnexpectedToken);
	}
}

ast::RelationOperator Parser::getRelationOper()
{
	switch (this->token.getType())
	{
	case Token::Type::Less:
		return ast::RelationOperator::Less;
	case Token::Type::LessOrEqual:
		return ast::RelationOperator::LessEqual;
	case Token::Type::Equal:
		return ast::RelationOperator::Equal;
	case Token::Type::GreaterOrEqual:
		return ast::RelationOperator::GreaterEqual;
	case Token::Type::Greater:
		return ast::RelationOperator::Greater;
	case Token::Type::NotEqual:
		return ast::RelationOperator::NotEqual;
	}
	throw Error(this->token, Error::Type::UnexpectedToken);
}

bool Parser::checkType(std::initializer_list<Token::Type> types)
{
	for (auto i : types)
		if (this->token.getType() == i)
			return true;
	return false;
}

SymbolTable& Parser::getTable()
{
	return this->symbolTable;
}

void Parser::requireType(std::initializer_list<Token::Type> types)
{
	if(!checkType(types))
		throw Error(this->token, Error::Type::UnexpectedToken);
}

void Parser::requireAndConsume(std::initializer_list<Token::Type> types)
{
	this->requireType(types);
	this->advance();
}