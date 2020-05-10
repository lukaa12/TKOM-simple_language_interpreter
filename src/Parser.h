#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "SymbolTable.h"
#include"ast/AstNode.h"

namespace tkom {

	class Parser
	{
	public:
		Parser(Scanner& scanner);
		std::unique_ptr<ast::Program> parse();
		SymbolTable& getTable();
	private:
		Scanner& scanner;
		SymbolTable symbolTable;
		Token token;

		void advance();
		ast::DataType getDataType();
		ast::RelationOperator getRelationOper();
		bool checkType(std::initializer_list<Token::Type> types);
		void requireType(std::initializer_list<Token::Type> types);
		void requireAndConsume(std::initializer_list<Token::Type> types);

		std::unique_ptr<ast::FunctionDef> readFunctionDef();
		std::unique_ptr<ast::CallDef> readCallDef();
		std::unique_ptr<ast::Body> readBodyBlock();
		std::unique_ptr<ast::CallOperator> readCallOperator();

		std::unique_ptr<ast::Instruction> readInstruction();
		std::unique_ptr<ast::IfStatement> readIfStatement();
		std::unique_ptr<ast::WhileLoop> readWhileLoop();
		std::unique_ptr<ast::AssignStatement> readAssignStatement();
		std::unique_ptr<ast::InitStatement> readInitStatement();
		std::unique_ptr<ast::FunctionExec> readFunctionExec();
		std::unique_ptr<ast::ReturnStatement> readReturnStatement();

		std::unique_ptr<ast::RightValue> readRightValue();
		std::unique_ptr<ast::FunctionCall> readFunctionCall();

		std::unique_ptr<ast::Expression> readExpression();
		std::unique_ptr<ast::MultiplicativeExpression> readMultiplicativeExpression();
		std::unique_ptr<ast::PrimaryExpression> readPrimaryExpression();
		std::unique_ptr<ast::BracketExpression> readBracketExpression();

		std::unique_ptr<ast::Condition> readCondition();
		std::unique_ptr<ast::AndCondition> readAndCondition();
		std::unique_ptr<ast::RelationCondition> readRelationCondition();
		std::unique_ptr<ast::PrimaryCondition> readPrimaryCondition();
		std::unique_ptr<ast::BracesCondition> readBracesCondition();
	};

}

#endif // !PARSER_H
