#ifndef FUNCTION_EXEC_H
#define FUNCTION_EXEC_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionExec : public Instruction
		{
		public:
			std::unique_ptr<FunctionCall> function;

			Type getType()
			{
				return Type::FuncExec;
			}
		};

	}
}

#endif // !FUNCTION_EXEC_H