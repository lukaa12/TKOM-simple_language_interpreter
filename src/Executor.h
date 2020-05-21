#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "ast/AstNode.h"
#include "SymbolTable.h"

namespace tkom {

	class Executor
	{
	public:
		Executor(ast::Program* prog);

		static SymbolTable symbolTable;

		std::unique_ptr<ast::Program> program;

		void execute();
	};

}

#endif // !EXECUTOR_H

