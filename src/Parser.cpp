#include "Parser.h"
#include "Error.h"

using namespace tkom;

Parser::Parser(Scanner& _scanner): scanner(_scanner), token()
{}

std::shared_ptr<ast::Program> Parser::parse()
{
	auto program = std::make_shared<ast::Program>();
	this->advance();
	while (this->token.getType() != Token::Type::Eof)
	{
		auto func = this->readFunctionDef();
		this->symbolTable.addGlobalSymbol(Symbol(func));
		program->addFunction(func);
	}

	return program;
}

std::shared_ptr<ast::FunctionDef> Parser::readFunctionDef()
{
	auto func = std::make_shared<ast::FunctionDef>();
	this->symbolTable.enterScope();
	func->setReturnType(getDataType());
	this->advance();
	this->requireType({ Token::Type::Identifier });
	func->setIdentifier(token.getStrVal());
	this->advance();
	func->setCallDef(readCallDef());
	func->seyFunctionBody(readBodyBlock());
	return func;
}

std::shared_ptr<ast::CallDef> Parser::readCallDef()
{
	auto callDef = std::make_shared<ast::CallDef>();
	this->requireType({ Token::Type::BracketOpen });
	this->advance();
	while (!this->checkType(Token::Type::BracketClose))
	{
		auto type = getDataType();
		this->advance();
		this->requireType({ Token::Type::Identifier });
		std::string argName = this->token.getStrVal();
		callDef->addArgument(std::make_pair(type, argName));
		this->advance();
		this->requireType({ Token::Type::BracketClose, Token::Type::Comma });
		if (this->checkType(Token::Type::Comma))
			this->advance();
	}
	this->requireType({ Token::Type::BracketClose });
	this->advance();
	return callDef;
}

std::shared_ptr<ast::Body> Parser::readBodyBlock()
{
	auto body = std::make_shared<ast::Body>();
	this->requireType({ Token::Type::CurlyOpen });
	this->advance();
	while (!this->checkType(Token::Type::CurlyClose))
	{
		body->addInstruction(readInstruction());
	}
	this->advance();
	return body;
}

std::shared_ptr<ast::CallOperator> Parser::readCallOperator()
{
	auto oper = std::make_shared<ast::CallOperator>();
	this->requireType({ Token::Type::BracketOpen });
	this->advance();
	while (!this->checkType(Token::Type::BracketClose))
	{
		oper->addArgument(this->readRightValue());
		if (this->checkType(Token::Type::Comma))
			this->advance();
	}
	this->advance();
	return oper;
}

std::shared_ptr<ast::Instruction> Parser::readInstruction()
{
	this->requireType({ Token::Type::If, Token::Type::While, Token::Type::Identifier, 
		Token::Type::Return, Token::Type::Break, Token::Type::IntType, Token::Type::StringType,
		Token::Type::GraphicType, Token::Type::ColorType });
	if (this->token.getType() == Token::Type::If)
		return std::shared_ptr<ast::Instruction>(this->readIfStatement());
	else if (this->token.getType() == Token::Type::While)
		return std::shared_ptr<ast::Instruction>(this->readWhileLoop());
	else if (token.getType() == Token::Type::Identifier)
		if (symbolTable.getSymbol(token.getStrVal()).type == ast::IdType::Function)
			return std::shared_ptr<ast::Instruction>(this->readFunctionExec());
		else
			return std::shared_ptr<ast::Instruction>(this->readAssignStatement());
	else if (token.getType() == Token::Type::Return)
		return std::shared_ptr<ast::Instruction>(this->readReturnStatement());
	else if (token.getType() == Token::Type::Break)
	{
		this->advance();
		this->requireType({ Token::Type::Semicolon });
		this->advance();
		return std::make_shared<ast::Instruction>();
	}
	else
		return std::shared_ptr<ast::Instruction>(this->readInitStatement());
}

std::shared_ptr<ast::IfStatement> Parser::readIfStatement()
{
	auto ifStatement = std::make_shared<ast::IfStatement>();
	this->requireType({ Token::Type::If });
	this->advance();
	this->requireType({ Token::Type::BracketOpen });
	this->advance();
	ifStatement->setCondition(this->readCondition());
	this->requireType({ Token::Type::BracketClose });
	this->advance();
	this->requireType({ Token::Type::CurlyOpen, Token::Type::If, Token::Type::While, Token::Type::Identifier,
		Token::Type::Return, Token::Type::Break });
	if (this->checkType(Token::Type::CurlyOpen))
		ifStatement->setIfBody(this->readBodyBlock());
	else
	{
		auto instr = std::make_shared<ast::Body>();
		instr->addInstruction(this->readInstruction());
		ifStatement->setIfBody(instr);
	}
	if (this->checkType(Token::Type::Else))
	{
		this->advance();
		if (this->checkType(Token::Type::CurlyOpen))
			ifStatement->setElseBody(this->readBodyBlock());
		else
		{
			auto instr = std::make_shared<ast::Body>();
			instr->addInstruction(this->readInstruction());
			ifStatement->setIfBody(instr);
		}
	}
	return ifStatement;
}

std::shared_ptr<ast::WhileLoop> Parser::readWhileLoop()
{
	auto whileLoop = std::make_shared<ast::WhileLoop>();
	this->requireType({ Token::Type::While });
	this->advance();
	this->requireType({ Token::Type::BracketOpen });
	this->advance();
	whileLoop->setCondition(this->readCondition());
	this->requireType({ Token::Type::BracketClose });
	this->advance();
	this->requireType({ Token::Type::CurlyOpen, Token::Type::If, Token::Type::While, Token::Type::Identifier,
		Token::Type::Return, Token::Type::Break });
	if (this->checkType(Token::Type::CurlyOpen))
		whileLoop->setBody(this->readBodyBlock());
	else
	{
		auto instr = std::make_shared<ast::Body>();
		instr->addInstruction(this->readInstruction());
		whileLoop->setBody(instr);
	}
	return whileLoop;
}

std::shared_ptr<ast::AssignStatement> Parser::readAssignStatement()
{
	auto assign = std::make_shared<ast::AssignStatement>();
	this->requireType({ Token::Type::Identifier });
	assign->setIdentifier(token.getStrVal());
	this->advance();
	this->requireType({ Token::Type::Assigment });
	this->advance();
	assign->setRval(this->readRightValue());
	this->requireType({ Token::Type::Semicolon });
	this->advance();
	return assign;
}

std::shared_ptr<ast::InitStatement> Parser::readInitStatement()
{
	auto initStatement = std::make_shared<ast::InitStatement>();
	initStatement->setDataType(this->getDataType());
	this->advance();
	while (true)
	{
		this->requireType({ Token::Type::Identifier });
		std::string id = this->token.getStrVal();
		this->advance();
		this->requireType({ Token::Type::Comma, Token::Type::Assigment, Token::Type::Semicolon });
		if (this->checkType(Token::Type::Assigment))
		{
			this->advance();
			initStatement->addInitiated(std::make_pair(id, this->readRightValue()));
			symbolTable.addLocalSymbol(Symbol(initStatement->getDataType(), id));
		}
		else {
			initStatement->addInitiated(std::make_pair(id, std::shared_ptr<ast::RightValue>()));
			symbolTable.addLocalSymbol(Symbol(initStatement->getDataType(), id));
		}
		this->requireType({ Token::Type::Comma, Token::Type::Semicolon });
		if (this->checkType(Token::Type::Semicolon))
			break;
		this->advance();
	}
	this->advance();
	return initStatement;
}

std::shared_ptr<ast::FunctionExec> Parser::readFunctionExec()
{
	auto exec = std::make_shared<ast::FunctionExec>();
	exec->setFunctionCall(this->readFunctionCall());
	this->requireType({ Token::Type::Semicolon });
	this->advance();
	return exec;
}

std::shared_ptr<ast::ReturnStatement> Parser::readReturnStatement()
{
	auto statement = std::make_shared<ast::ReturnStatement>();
	this->requireType({ Token::Type::Return });
	this->advance();
	statement->setValue(this->readRightValue());
	this->requireType({ Token::Type::Semicolon });
	this->advance();
	return statement;
}

std::shared_ptr<ast::RightValue> Parser::readRightValue()
{
	auto right = std::make_shared<ast::RightValue>();
	this->requireType({ Token::Type::StringLiteral, Token::Type::IntLiteral, Token::Type::Identifier, Token::Type::Minus, Token::Type::BracketOpen });
	if (this->checkType(Token::Type::StringLiteral))
	{
		right->setType(ast::RightValue::Type::StringLiteral);
		right->setValue(this->token.getStrVal());
		this->advance();
	}
	else if (this->checkType(Token::Type::Identifier) && symbolTable.getSymbol(this->token.getStrVal()).dataType != ast::DataType::Int)
	{
		if (symbolTable.getSymbol(this->token.getStrVal()).type == ast::IdType::Variable)
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

std::shared_ptr<ast::FunctionCall> Parser::readFunctionCall()
{
	auto call = std::make_shared<ast::FunctionCall>();
	this->requireType({ Token::Type::Identifier });
	call->setIdentifier(this->token.getStrVal());
	this->advance();
	call->setCallOperator(this->readCallOperator());
	return call;
}

std::shared_ptr<ast::Expression> Parser::readExpression()
{
	auto expr = std::make_shared<ast::Expression>();
	bool minus = false;
	minus = this->checkType(Token::Type::Minus);
	if (minus)
		this->advance();
	expr->addComponent(std::make_pair(minus, this->readMultiplicativeExpression()));
	while (this->checkType(Token::Type::Plus) || this->checkType(Token::Type::Minus))
	{
		minus = this->checkType(Token::Type::Minus);
		this->advance();
		expr->addComponent(std::make_pair(minus, this->readMultiplicativeExpression()));
	}
	return expr;
}

std::shared_ptr<ast::MultiplicativeExpression> Parser::readMultiplicativeExpression()
{
	auto expr = std::make_shared<ast::MultiplicativeExpression>();
	expr->addComponent(this->readPrimaryExpression());
	while (this->checkType(Token::Type::Multiply) || this->checkType(Token::Type::Divide))
	{
		expr->addOperator(this->checkType(Token::Type::Divide));
		this->advance();
		expr->addComponent(this->readPrimaryExpression());
	}
	return expr;
}

std::shared_ptr<ast::PrimaryExpression> Parser::readPrimaryExpression()
{
	auto expr = std::make_shared<ast::PrimaryExpression>();
	this->requireType({ Token::Type::Identifier, Token::Type::IntLiteral, Token::Type::BracketOpen });
	if (this->checkType(Token::Type::Identifier))
	{
		if (symbolTable.getSymbol(this->token.getStrVal()).type == ast::IdType::Function)
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
	else if (this->checkType(Token::Type::IntLiteral))
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

std::shared_ptr<ast::BracketExpression> Parser::readBracketExpression()
{
	auto expr = std::make_shared<ast::BracketExpression>();
	this->requireType({ Token::Type::BracketOpen });
	this->advance();
	expr->setExpression(this->readExpression());
	this->requireType({ Token::Type::BracketClose });
	this->advance();
	return expr;
}


std::shared_ptr<ast::Condition> Parser::readCondition()
{
	auto cond = std::make_shared<ast::Condition>();
	cond->addComponent(this->readAndCondition());
	while (this->checkType(Token::Type::Or))
	{
		this->advance();
		cond->addComponent(this->readAndCondition());
	}
	return cond;
}

std::shared_ptr<ast::AndCondition> Parser::readAndCondition()
{
	auto cond = std::make_shared<ast::AndCondition>();
	cond->addComponent(this->readRelationCondition());
	while (this->checkType(Token::Type::And))
	{
		this->advance();
		cond->addComponent(this->readRelationCondition());
	}
	return cond;
}

std::shared_ptr<ast::RelationCondition> Parser::readRelationCondition()
{
	auto cond = std::make_shared<ast::RelationCondition>();
	cond->setFirst(this->readPrimaryCondition());
	ast::RelationOperator oper;
	try
	{
		oper = this->getRelationOper();
	}
	catch (const std::exception&)
	{
		return cond;
	}
	cond->setRelationOper(oper);
	this->advance();
	cond->setSecond(this->readPrimaryCondition());
	return cond;
}

std::shared_ptr<ast::PrimaryCondition> Parser::readPrimaryCondition()
{
	auto cond = std::make_shared<ast::PrimaryCondition>();
	if (this->checkType(Token::Type::Negation))
	{
		cond->setNegated(true);
		this->advance();
	}
	if (this->checkType(Token::Type::BracketOpen))
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

std::shared_ptr<ast::BracesCondition> Parser::readBracesCondition()
{
	auto cond = std::make_shared<ast::BracesCondition>();
	this->requireType({ Token::Type::BracketOpen });
	this->advance();
	cond->setCondition(this->readCondition());
	this->requireType({ Token::Type::BracketClose });
	this->advance();
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
	throw std::exception("Not relation operator");
}

bool Parser::checkType(const Token::Type& type)
{
	return this->token.getType() == type ? true : false;
}

SymbolTable& Parser::getTable()
{
	return this->symbolTable;
}

void Parser::requireType(std::initializer_list<Token::Type> types)
{
	for (auto i : types)
		if (this->token.getType() == i)
			return;
	throw Error(this->token, Error::Type::UnexpectedToken);
}