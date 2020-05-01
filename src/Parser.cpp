//#include "Parser.h"
//#include "Error.h"
//
//using namespace tkom;
//
//Parser::Parser(Scanner& _scanner): scanner(_scanner), token()
//{}
//
//std::shared_ptr<ast::Program> Parser::parse()
//{
//	auto program = std::make_shared<ast::Program>();
//	this->advance();
//	while (this->token.getType() != Token::Type::Eof)
//	{
//		auto func = this->readFunctionDef();
//		this->symbolTable.addGlobalSymbol(Symbol(func));
//		program->addFunction(func);
//	}
//
//	return program;
//}
//
//std::shared_ptr<ast::FunctionDef> Parser::readFunctionDef()
//{
//	auto func = std::make_shared<ast::FunctionDef>();
//	
//	func->returnType = getDataType();
//	this->advance();
//	if (!this->checkType(Token::Type::Identifier))
//		throw Error(this->token, Error::Type::UnexpectedToken);
//	func->identifier = this->token.getStrVal();
//	this->advance();
//	func->requiredArguments = this->readCallDef();
//	func->functionBody = this->readBodyBlock();
//	return func;
//}
//
//std::shared_ptr<ast::CallDef> Parser::readCallDef()
//{
//	auto callDef = std::make_shared<ast::CallDef>();
//	if (!this->checkType(Token::Type::BracketOpen))
//		throw Error(this->token, Error::Type::UnexpectedToken);
//	this->advance();
//	while (!this->checkType(Token::Type::BracketClose))
//	{
//		auto type = getDataType();
//		this->advance();
//		if (!this->checkType(Token::Type::Identifier))
//			throw Error(this->token, Error::Type::UnexpectedToken);
//		std::string argName = this->token.getStrVal();
//		callDef->argumentsList.push_back(std::make_pair(type, argName));
//		this->advance();
//		if(!this->checkType(Token::Type::BracketClose) && !this->checkType(Token::Type::Comma))
//			throw Error(this->token, Error::Type::UnexpectedToken);
//		if (this->checkType(Token::Type::Comma))
//			this->advance();
//	}
//	if(!this->checkType(Token::Type::BracketClose))
//		throw Error(this->token, Error::Type::UnexpectedToken);
//	this->advance();
//	return callDef;
//}
//
//std::shared_ptr<ast::Body> Parser::readBodyBlock()
//{
//	auto body = std::make_shared<ast::Body>();
//	if (!this->checkType(Token::Type::CurlyOpen))
//		throw Error(this->token, Error::Type::UnexpectedToken);
//	this->advance();
//	while (!this->checkType(Token::Type::CurlyClose))
//	{
//		body->instructions.push_back(this->readInstruction());
//		this->advance();
//	}
//	this->advance();
//	return body;
//}
//
//std::shared_ptr<ast::Instruction> Parser::readInstruction()
//{
//	throw std::exception("TODO");
//	//if (this->token.getType() == Token::Type::If)
//	//	auto ptr = std::make_shared<ast::Instruction>(this->readIfStatement().release());
//	//else if (this->token.getType() == Token::Type::While)
//	//	auto ptr = std::make_shared<ast::Instruction>(this->readWhileLoop().release());
//	//else if (token.getType() == Token::Type::Identifier)
//	//	if (symbolTable.getSymbol(token.getStrVal()).type == ast::IdType::Function)
//	//		auto ptr = std::make_shared<ast::Instruction>(this->readFunctionCall().release());
//	//	else
//	//		auto ptr = std::make_shared<ast::Instruction>(this->readAsignStatement().release());
//	//else /*if (token.getType() == Token::Type::Return)*/
//	//	auto ptr = std::make_shared<ast::Instruction>(this->readReturnStatement().release());
//	////else if (token.getType() == Token::Type::Break)
//	////{
//	////	this->advance();
//	////	if(token.getType() != Token::Type::Semicolon)
//	////		throw Error(this->token, Error::Type::UnexpectedToken);
//	////	this->advance();
//	////	//TODO
//	////}
//	//return ptr;
//}
//
//void Parser::advance()
//{
//	this->token = this->scanner.nextToken();
//}
//
//ast::DataType Parser::getDataType()
//{
//	switch (this->token.getType())
//	{
//	case Token::Type::IntType:
//		return ast::DataType::Int;
//	case Token::Type::StringType:
//		return ast::DataType::String;
//	case Token::Type::GraphicType:
//		return ast::DataType::Graphic;
//	case Token::Type::ColorType:
//		return ast::DataType::Color;
//	default:
//		throw Error(token, Error::Type::UnexpectedToken);
//	}
//}
//
//bool Parser::checkType(const Token::Type& type)
//{
//	return this->token.getType() == type ? true : false;
//}
//
//SymbolTable& Parser::getTable()
//{
//	return this->symbolTable;
//}