#ifndef CALL_OPERATOR_H
#define CALL_OPERATOR_H

#include "AstNode.h"
#include "RightValue.h"

namespace tkom {
	namespace ast {

		class CallOperator : public Node
		{
		public:

			void addArgument(std::unique_ptr<RightValue> ptr)
			{
				ptr->parent = this;
				arguments.push_back(std::move(ptr));
			}

			std::vector<std::unique_ptr<RightValue>>& getArguments()
			{
				return arguments;
			}

		private:
			std::vector<std::unique_ptr<RightValue>> arguments;
		};

	}
}

#endif // !CALL_OPERATOR_H