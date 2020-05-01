#ifndef FUNCTION_EXEC_H
#define FUNCTION_EXEC_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionExec : public Instruction
		{
		public:
			std::shared_ptr<FunctionCall> getFunctionCall()
			{
				return function;
			}

			void setFunctionCall(const std::shared_ptr<FunctionCall>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				this->function = ptr;
			}

			Type getType()
			{
				return Type::FuncExec;
			}

		private:
			std::shared_ptr<FunctionCall> function;
		};

	}
}

#endif // !FUNCTION_EXEC_H