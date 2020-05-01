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
		std::shared_ptr<ast::Program> parse();
		SymbolTable& getTable();				//For testing
	private:
		Scanner& scanner;
		SymbolTable symbolTable;
		Token token;

		void advance();
		ast::DataType getDataType();
		ast::RelationOperator getRelationOper();
		bool checkType(const Token::Type& type);
		void requireType(std::initializer_list<Token::Type> types);

		std::shared_ptr<ast::FunctionDef> readFunctionDef();
		std::shared_ptr<ast::CallDef> readCallDef();
		std::shared_ptr<ast::Body> readBodyBlock();
		std::shared_ptr<ast::CallOperator> readCallOperator();

		std::shared_ptr<ast::Instruction> readInstruction();
		std::shared_ptr<ast::IfStatement> readIfStatement();
		std::shared_ptr<ast::WhileLoop> readWhileLoop();
		std::shared_ptr<ast::AssignStatement> readAssignStatement();
		std::shared_ptr<ast::InitStatement> readInitStatement();
		std::shared_ptr<ast::FunctionExec> readFunctionExec();
		std::shared_ptr<ast::ReturnStatement> readReturnStatement();

		std::shared_ptr<ast::RightValue> readRightValue();
		std::shared_ptr<ast::FunctionCall> readFunctionCall();

		std::shared_ptr<ast::Expression> readExpression();
		std::shared_ptr<ast::MultiplicativeExpression> readMultiplicativeExpression();
		std::shared_ptr<ast::PrimaryExpression> readPrimaryExpression();
		std::shared_ptr<ast::BracketExpression> readBracketExpression();

		std::shared_ptr<ast::Condition> readCondition();
		std::shared_ptr<ast::AndCondition> readAndCondition();
		std::shared_ptr<ast::RelationCondition> readRelationCondition();
		std::shared_ptr<ast::PrimaryCondition> readPrimaryCondition();
		std::shared_ptr<ast::BracesCondition> readBracesCondition();
	};

}

#endif // !PARSER_H
