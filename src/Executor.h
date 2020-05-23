#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "ast/AstNode.h"
#include "SymbolTable.h"
#include "stdlib/Functions.h"

namespace tkom {

	class Executor
	{
	public:
		Executor(ast::Program* prog);
		static SymbolTable symbolTable;
		static ast::DataType libAdapter(ast::FunctionCall* call);
		static bool checkFunction(std::string identifier);
		std::unique_ptr<ast::Program> program;
		void execute();

	private:
		static bool checkArguments(ast::CallOperator* oper, std::initializer_list<ast::DataType> argTypes);
	};

}

#endif // !EXECUTOR_H

