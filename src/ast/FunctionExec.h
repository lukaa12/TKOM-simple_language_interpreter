#ifndef FUNCTION_EXEC_H
#define FUNCTION_EXEC_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionExec : public Instruction
		{
		public:
			FunctionCall* getFunctionCall()
			{
				return function.get();
			}

			void setFunctionCall(std::unique_ptr<FunctionCall> ptr)
			{
				ptr->parent = this;
				this->function = std::move(ptr);
			}

			Type getType()
			{
				return Type::FuncExec;
			}

		private:
			std::unique_ptr<FunctionCall> function;
		};

	}
}

#endif // !FUNCTION_EXEC_H